#!/usr/bin/env python3
"""Analyze Acaia Lunar serial monitor capture logs."""

from __future__ import annotations

import argparse
import json
import math
import re
import sys
from collections import Counter
from pathlib import Path
from typing import Any


SAMPLE_RE = re.compile(
    r"(?:^|>\s*)(?P<ts>\d+),(?P<weight>-?\d+(?:\.\d+)?),(?P<battery>-?\d+),(?P<connected>[01])\s*$"
)
ERROR_RE = re.compile(
    r"\b(disconnected|failed|overflow|invalid|Guru|panic|abort|ERROR|Error|error|resetting|timeout|exception)\b"
)


def repo_root() -> Path:
    return Path(__file__).resolve().parents[2]


def latest_capture() -> Path:
    log_dir = repo_root() / "data" / "raw" / "m1-lunar-ble"
    captures = sorted(log_dir.glob("lunar-poc-*.log"), key=lambda path: path.stat().st_mtime)
    if not captures:
        raise FileNotFoundError(f"No capture logs found in {log_dir}")
    return captures[-1]


def parse_args() -> argparse.Namespace:
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument(
        "log_file",
        nargs="?",
        type=Path,
        help="Capture log to analyze. Defaults to the latest data/raw/m1-lunar-ble/lunar-poc-*.log.",
    )
    parser.add_argument(
        "--m1-long",
        action="store_true",
        help="Apply M1 long-capture gates: >=600s, >=1000 samples, all samples connected, no error lines, max gap <=5000ms.",
    )
    parser.add_argument("--min-duration-sec", type=float, default=None)
    parser.add_argument("--min-samples", type=int, default=None)
    parser.add_argument("--max-gap-ms", type=int, default=None)
    parser.add_argument("--allow-error-lines", type=int, default=0)
    parser.add_argument("--require-connected", action="store_true")
    parser.add_argument("--json", action="store_true", help="Emit JSON instead of human-readable text.")
    return parser.parse_args()


def load_capture(path: Path) -> dict[str, Any]:
    samples: list[dict[str, Any]] = []
    error_lines: list[dict[str, Any]] = []

    with path.open("r", encoding="utf-8", errors="replace") as handle:
        for line_number, raw_line in enumerate(handle, start=1):
            line = raw_line.replace("\r", "").rstrip("\n")
            match = SAMPLE_RE.search(line)
            if match:
                samples.append(
                    {
                        "line": line_number,
                        "timestamp_ms": int(match.group("ts")),
                        "weight_g": float(match.group("weight")),
                        "battery_percent": int(match.group("battery")),
                        "connected": int(match.group("connected")),
                        "raw": match.group(0).lstrip("> "),
                    }
                )
                continue

            if ERROR_RE.search(line):
                error_lines.append({"line": line_number, "text": line})

    if not samples:
        raise ValueError(f"No CSV samples found in {path}")

    return {"path": path, "samples": samples, "error_lines": error_lines}


def summarize(capture: dict[str, Any]) -> dict[str, Any]:
    samples = capture["samples"]
    first = samples[0]
    last = samples[-1]
    weights = [sample["weight_g"] for sample in samples]
    timestamps = [sample["timestamp_ms"] for sample in samples]
    gaps = [later - earlier for earlier, later in zip(timestamps, timestamps[1:])]
    batteries = Counter(str(sample["battery_percent"]) for sample in samples)
    connections = Counter(str(sample["connected"]) for sample in samples)
    duration_ms = last["timestamp_ms"] - first["timestamp_ms"]
    duration_s = duration_ms / 1000
    mean_weight = sum(weights) / len(weights)
    variance = sum((weight - mean_weight) ** 2 for weight in weights) / len(weights)

    return {
        "log_file": str(capture["path"]),
        "samples": len(samples),
        "first_sample": sample_payload(first),
        "last_sample": sample_payload(last),
        "first_timestamp_ms": first["timestamp_ms"],
        "last_timestamp_ms": last["timestamp_ms"],
        "duration_ms": duration_ms,
        "duration_s": duration_s,
        "sample_rate_hz": len(samples) / duration_s if duration_s > 0 else None,
        "weight_g": {
            "min": min(weights),
            "max": max(weights),
            "mean": mean_weight,
            "stddev": math.sqrt(variance),
        },
        "sample_gap_ms": {
            "max": max(gaps) if gaps else 0,
            "mean": sum(gaps) / len(gaps) if gaps else 0,
            "non_positive": sum(1 for gap in gaps if gap <= 0),
        },
        "battery_counts": dict(sorted(batteries.items(), key=lambda item: int(item[0]))),
        "connection_counts": dict(sorted(connections.items(), key=lambda item: int(item[0]))),
        "error_lines": capture["error_lines"],
    }


def sample_payload(sample: dict[str, Any]) -> str:
    return (
        f"{sample['timestamp_ms']},{sample['weight_g']:.2f},"
        f"{sample['battery_percent']},{sample['connected']}"
    )


def criteria_from_args(args: argparse.Namespace) -> dict[str, Any]:
    criteria = {
        "min_duration_sec": args.min_duration_sec,
        "min_samples": args.min_samples,
        "max_gap_ms": args.max_gap_ms,
        "allow_error_lines": args.allow_error_lines,
        "require_connected": args.require_connected,
    }
    if args.m1_long:
        criteria["min_duration_sec"] = 600 if args.min_duration_sec is None else args.min_duration_sec
        criteria["min_samples"] = 1000 if args.min_samples is None else args.min_samples
        criteria["max_gap_ms"] = 5000 if args.max_gap_ms is None else args.max_gap_ms
        criteria["require_connected"] = True
    return criteria


def evaluate(summary: dict[str, Any], criteria: dict[str, Any]) -> list[str]:
    failures: list[str] = []

    min_duration_sec = criteria["min_duration_sec"]
    if min_duration_sec is not None and summary["duration_s"] < min_duration_sec:
        failures.append(f"duration_s {summary['duration_s']:.3f} < {min_duration_sec}")

    min_samples = criteria["min_samples"]
    if min_samples is not None and summary["samples"] < min_samples:
        failures.append(f"samples {summary['samples']} < {min_samples}")

    max_gap_ms = criteria["max_gap_ms"]
    if max_gap_ms is not None and summary["sample_gap_ms"]["max"] > max_gap_ms:
        failures.append(f"max_gap_ms {summary['sample_gap_ms']['max']} > {max_gap_ms}")

    if criteria["require_connected"] and set(summary["connection_counts"].keys()) != {"1"}:
        failures.append(f"connection_counts {summary['connection_counts']} includes disconnected samples")

    allowed_errors = criteria["allow_error_lines"]
    if len(summary["error_lines"]) > allowed_errors:
        failures.append(f"error_lines {len(summary['error_lines'])} > {allowed_errors}")

    if summary["sample_gap_ms"]["non_positive"] > 0:
        failures.append(f"non_positive_sample_gaps {summary['sample_gap_ms']['non_positive']} > 0")

    return failures


def print_text(summary: dict[str, Any], failures: list[str], criteria: dict[str, Any]) -> None:
    result = "FAIL" if failures else "PASS"
    weight = summary["weight_g"]
    gaps = summary["sample_gap_ms"]

    print("[Mnemor] Lunar capture analysis")
    print(f"log_file: {summary['log_file']}")
    print(f"result: {result}")
    print(f"samples: {summary['samples']}")
    print(f"duration_s: {summary['duration_s']:.3f}")
    print(f"sample_rate_hz: {summary['sample_rate_hz']:.2f}")
    print(f"first_sample: {summary['first_sample']}")
    print(f"last_sample: {summary['last_sample']}")
    print(
        "weight_g: "
        f"min={weight['min']:.2f} max={weight['max']:.2f} "
        f"mean={weight['mean']:.4f} stddev={weight['stddev']:.4f}"
    )
    print(f"sample_gap_ms: max={gaps['max']} mean={gaps['mean']:.1f} non_positive={gaps['non_positive']}")
    print(f"battery_counts: {format_counts(summary['battery_counts'])}")
    print(f"connection_counts: {format_counts(summary['connection_counts'])}")
    print(f"error_lines: {len(summary['error_lines'])}")

    active_criteria = {key: value for key, value in criteria.items() if value not in (None, False)}
    if active_criteria:
        print(f"criteria: {json.dumps(active_criteria, sort_keys=True)}")

    for failure in failures:
        print(f"failure: {failure}")

    for error in summary["error_lines"][:10]:
        print(f"error_line[{error['line']}]: {error['text']}")


def format_counts(counts: dict[str, int]) -> str:
    return " ".join(f"{key}:{value}" for key, value in counts.items())


def main() -> int:
    args = parse_args()
    path = args.log_file if args.log_file is not None else latest_capture()
    criteria = criteria_from_args(args)

    try:
        summary = summarize(load_capture(path))
    except (FileNotFoundError, ValueError) as exc:
        print(f"[ERROR] {exc}", file=sys.stderr)
        return 2

    failures = evaluate(summary, criteria)

    if args.json:
        payload = {"result": "FAIL" if failures else "PASS", "summary": summary, "failures": failures}
        print(json.dumps(payload, indent=2, sort_keys=True))
    else:
        print_text(summary, failures, criteria)

    return 1 if failures else 0


if __name__ == "__main__":
    raise SystemExit(main())
