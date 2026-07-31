# M1 Lunar BLE Sprint

Date: 2026-07-31

Branch: `feature/lunar-ble`

## Sprint Goal

ESP32 connects to the Acaia Lunar AL008 and continuously prints weight over USB serial.

No recommendation logic, no shot detection, no product UI.

## Implemented

- PlatformIO project in `firmware/lunar-poc/`
- ESP32-S3 Arduino environment
- OLED boot status:
  - `Mnemor Brew`
  - `Booting...`
  - `Searching...`
  - `Connected`
  - weight in grams
- BLE scan for Lunar by configured MAC or name substring
- Lunar service and characteristic lookup
- notification subscription
- notification request
- heartbeat loop
- battery/settings request
- tare, start timer, stop timer, reset timer commands over serial
- CSV serial output:
  - `timestamp_ms,weight_g,battery_percent,connected`
- reconnect scan loop after disconnect or failed connect

## Build Verification

Command:

```bash
/Users/jonasweiss/Documents/Codex/2026-07-31/new-chat/work/platformio-venv/bin/pio run
```

Result:

```text
SUCCESS
RAM: 13.7%
Flash: 28.7%
```

## Hardware Verification

Blocked in the current environment.

Visible serial ports:

```text
/dev/cu.debug-console
/dev/cu.Bluetooth-Incoming-Port
```

No ESP32 USB serial port was visible, so upload and live monitor were not executed.

## GitHub Push Verification

Blocked by repository permissions.

Remote:

```text
https://github.com/ordinalsog-ctrl/mnemor-brew-engine.git
```

Observed result:

```text
Permission to ordinalsog-ctrl/mnemor-brew-engine.git denied to weissio.
HTTP 403
```

The connected GitHub App currently has read access but no push permission.

Expected next local command after connecting the board:

```bash
pio device list
pio run -t upload --upload-port /dev/cu.usbmodemXXXX
pio device monitor -b 115200 --port /dev/cu.usbmodemXXXX
```

## Acceptance Status

- AP-010 PlatformIO Project: Build verified
- AP-011 OLED Initialization: Code implemented, hardware not verified
- AP-012 ESP32 Lunar BLE Scan: Code implemented, hardware not verified
- AP-013 Lunar Connection: Code implemented, hardware not verified
- AP-014 Notifications Subscribe: Code implemented, hardware not verified
- AP-015 Weight Stream Capture: Code implemented, hardware not verified
- AP-016 Tare Command: Code implemented, hardware not verified
- AP-017 Reconnect Handling: Code implemented, hardware not verified
- AP-018 CSV Logger: Code implemented, hardware not verified

## Reference

Protocol constants and message behavior were derived from the MIT-licensed LunarGateway project:

```text
https://github.com/frowin/LunarGateway
```
