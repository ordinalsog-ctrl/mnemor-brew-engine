#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)"
PIO_BIN="${PIO_BIN:-/Users/jonasweiss/Documents/Codex/2026-07-31/new-chat/work/platformio-venv/bin/pio}"

echo "[Mnemor] Checking serial devices"
echo

if [[ -x "$PIO_BIN" ]]; then
  "$PIO_BIN" device list || true
else
  echo "[WARN] PlatformIO not found at: $PIO_BIN"
fi

echo
echo "[Mnemor] Raw /dev candidates"
ls /dev/cu.* /dev/tty.* 2>/dev/null | sed -n '1,160p' || true

echo
echo "[Mnemor] Likely ESP32 ports"
ls /dev/cu.usb* /dev/cu.SLAB_USB* /dev/cu.wchusb* /dev/cu.usbserial* 2>/dev/null || {
  echo "[WARN] No likely ESP32 USB serial port found."
  echo "[NEXT] Connect the ESP32-S3 by USB, then run this script again."
}

echo
echo "[Mnemor] Repo: $ROOT_DIR"
