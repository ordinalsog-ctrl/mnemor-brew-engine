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
shopt -s nullglob
ports=(/dev/cu.usbmodem* /dev/cu.usbserial* /dev/cu.SLAB_USB* /dev/cu.wchusb*)
shopt -u nullglob

if (( ${#ports[@]} > 0 )); then
  printf '%s\n' "${ports[@]}"
else
  echo "[WARN] No likely ESP32 USB serial port found."
  echo "[NEXT] Connect the Freenove ESP32 WROVER by USB, then run this script again."
fi

echo
echo "[Mnemor] Repo: $ROOT_DIR"
echo
echo "[Mnemor] Primary board now: Freenove ESP32 WROVER"
echo "[Mnemor] Primary PlatformIO env: freenove_esp32_wrover"
echo "[Mnemor] ESP32-S3 WROOM-1 is blocked because its USB-C connector is assembled incorrectly."
