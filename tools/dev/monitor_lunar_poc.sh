#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
FIRMWARE_DIR="$ROOT_DIR/firmware/lunar-poc"
LOG_DIR="$ROOT_DIR/data/raw/m1-lunar-ble"
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

PORT="${1:-${MONITOR_PORT:-}}"
if [[ -z "$PORT" ]]; then
  PORT="$(find_port || true)"
fi

if [[ -z "$PORT" ]]; then
  echo "[ERROR] No ESP32 USB serial port found." >&2
  echo "[NEXT] Connect the ESP32-S3 and run: tools/dev/check_ports.sh" >&2
  exit 2
fi

mkdir -p "$LOG_DIR"
STAMP="$(date +%Y%m%d-%H%M%S)"
LOG_FILE="$LOG_DIR/lunar-poc-$STAMP.log"

echo "[Mnemor] Monitoring $PORT at 115200 baud"
echo "[Mnemor] Log file: $LOG_FILE"
echo "[Mnemor] Commands: t=tare, n=notifications, b=battery, ?=help"
echo "[Mnemor] Stop monitor with Ctrl-C"

(
  cd "$FIRMWARE_DIR"
  "$PIO_BIN" device monitor --port "$PORT" --baud 115200 | tee "$LOG_FILE"
)
