#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
FIRMWARE_DIR="$ROOT_DIR/firmware/lunar-poc"
PIO_BIN="${PIO_BIN:-/Users/jonasweiss/Documents/Codex/2026-07-31/new-chat/work/platformio-venv/bin/pio}"

find_port() {
  for pattern in /dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.SLAB_USB* /dev/cu.wchusb*; do
    for port in $pattern; do
      [[ -e "$port" ]] && echo "$port" && return 0
    done
  done
  return 1
}

if [[ ! -x "$PIO_BIN" ]]; then
  echo "[ERROR] PlatformIO not found at: $PIO_BIN" >&2
  exit 1
fi

PORT="${1:-${UPLOAD_PORT:-}}"
if [[ -z "$PORT" ]]; then
  PORT="$(find_port || true)"
fi

if [[ -z "$PORT" ]]; then
  echo "[ERROR] No ESP32 USB serial port found." >&2
  echo "[NEXT] Connect the ESP32-S3 and run: tools/dev/check_ports.sh" >&2
  exit 2
fi

echo "[Mnemor] Building firmware"
(
  cd "$FIRMWARE_DIR"
  "$PIO_BIN" run
)

echo "[Mnemor] Flashing firmware to $PORT"
(
  cd "$FIRMWARE_DIR"
  "$PIO_BIN" run -t upload --upload-port "$PORT"
)

echo "[Mnemor] Flash complete"
echo "[NEXT] Run: tools/dev/monitor_lunar_poc.sh $PORT"
