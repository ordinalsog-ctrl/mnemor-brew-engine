# Lunar Proof of Data

This folder contains the first firmware experiment.

## Goal

Prove that the ESP32 can read useful data from the Acaia Lunar reliably enough for product validation.

## Scope

In scope:

- BLE scan
- Lunar identification
- connection
- weight stream
- tare command
- battery reading if available
- serial logging
- disconnect and reconnect notes
- OLED boot and connected status

Out of scope:

- polished OLED UI
- buttons beyond serial commands
- recommendation logic
- enclosure
- app integration
- broad scale support

## PlatformIO

Default environment:

```text
freenove_esp32_wrover
```

Supported environments:

```text
freenove_esp32_s3_wroom
esp32-s3-devkitc-1
freenove_esp32_wrover
```

Use the Freenove ESP32 WROVER first. The available ESP32-S3 WROOM-1 is blocked for M1 because its USB-C connector is assembled incorrectly and cannot connect.

If the exact ESP32-S3 board uses a different PlatformIO board ID or OLED I2C pins, update:

- `platformio.ini`
- `include/MnemorConfig.h`

For a private scale MAC, create:

```text
include/MnemorConfig.local.h
```

Example:

```cpp
#pragma once
#define LUNAR_TARGET_MAC "34:81:f4:e0:1b:ff"
#define OLED_SDA_PIN 8
#define OLED_SCL_PIN 9
```

`MnemorConfig.local.h` is gitignored.

## Build

```bash
pio run
```

## Flash

Connect the Freenove ESP32 WROVER over USB and run:

```bash
pio run -t upload
```

If multiple ports are visible:

```bash
pio device list
pio run -t upload --upload-port /dev/cu.usbmodemXXXX
```

## Serial Monitor

```bash
pio device monitor -b 115200
```

From repo root, the wrapped local workflow is:

```bash
tools/dev/check_ports.sh
tools/dev/flash_lunar_poc.sh
tools/dev/monitor_lunar_poc.sh
```

To force the WROVER environment:

```bash
PIO_ENV=freenove_esp32_wrover tools/dev/flash_lunar_poc.sh
```

## Target Serial Output

```text
[BOOT] Mnemor Brew Engine Lunar PoC
[BLE] scan started
[BLE] Searching...
[BLE] found name=Acaia Lunar address=34:81:f4:e0:1b:ff rssi=-58
[BLE] connecting address=34:81:f4:e0:1b:ff
[BLE] connected
[BLE] service found
[BLE] command characteristic found
[BLE] data characteristic found
[BLE] notifications subscribed
timestamp_ms,weight_g,battery_percent,connected
4812,0.00,82,1
[SCALE] weight=0.00 g
[SCALE] tare sent
```

## Serial Commands

Send newline-terminated commands at 115200 baud:

```text
t = tare
n = request notifications
b = request settings / battery
s = start timer
h = stop timer
r = reset timer
m = toggle raw BLE payload debug
? = help
```

## Test Protocol

1. Ensure the scale is not connected to another app.
2. Start ESP32.
3. Record time to connect.
4. Stream idle weight for 2 minutes.
5. Place cup on scale.
6. Send tare.
7. Pull or simulate shot.
8. Continue stream until at least 10 minutes total runtime.
9. Save log and convert to sample shot JSON if possible.

## Pass Criteria

- connect succeeds within 10 seconds in normal conditions
- stream remains stable for 10 minutes
- tare command works without breaking stream
- logs are detailed enough to debug failures

## Protocol Notes

This proof uses protocol behavior from the MIT-licensed LunarGateway reference implementation by Frowin Ellermann:

- service UUID: `49535343-FE7D-4AE5-8FA9-9FAFD205E455`
- command characteristic: `49535343-8841-43f4-a8d4-ecbe34729bb3`
- data characteristic: `49535343-1e4d-4bd9-ba61-23c647249616`
- heartbeat must be sent before the scale closes the BLE connection
- weight streaming must be requested explicitly

## Notes

Keep this proof boring. No UI polish until this passes.
