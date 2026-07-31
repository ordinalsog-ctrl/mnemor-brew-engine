# M1 Hardware Test Protocol

## Objective

Prove the first real hardware loop:

```text
Freenove ESP32 WROVER -> BLE scan -> Acaia Lunar -> notifications -> USB serial CSV weight stream
```

## Board Choice

Use this order:

1. `Freenove ESP32 WROVER`
2. `ESP32-S3 WROOM-1`
3. `Freenove ESP32 GPIO Expansion Board`

The GPIO Expansion Board is not the controller. Use it only as a breakout or wiring helper.

The preferred first controller is now the Freenove ESP32 WROVER.

Reason: the available ESP32-S3 WROOM-1 is self-assembled and has a USB-C orientation fault, so it cannot connect over USB.

The firmware target now defaults to:

```text
freenove_esp32_wrover
```

The ESP32-S3 WROOM target remains buildable for a later replacement board:

```bash
PIO_ENV=freenove_esp32_s3_wroom tools/dev/flash_lunar_poc.sh
```

## Preflight

1. Connect the Freenove ESP32 WROVER with a data-capable USB cable.
2. Turn on the Acaia Lunar AL008.
3. Ensure the Lunar is not connected to the Acaia app or another device.
4. If possible, update Lunar firmware through the Acaia updater app before testing.
5. Confirm that a USB serial port appears.

## Step 1 - Check Port

From repo root:

```bash
tools/dev/check_ports.sh
```

Expected:

```text
/dev/cu.usbmodemXXXX
```

or another explicit USB serial candidate.

## Step 2 - Optional Private Config

If BLE name scan fails, set the scale MAC address locally.

Create:

```text
firmware/lunar-poc/include/MnemorConfig.local.h
```

Example:

```cpp
#pragma once
#define LUNAR_TARGET_MAC "34:81:f4:e0:1b:ff"
```

This file is ignored by Git.

## Step 3 - Flash

Automatic port detection:

```bash
tools/dev/flash_lunar_poc.sh
```

Explicit port:

```bash
tools/dev/flash_lunar_poc.sh /dev/cu.usbmodemXXXX
```

Explicit WROVER environment, if needed:

```bash
PIO_ENV=freenove_esp32_wrover tools/dev/flash_lunar_poc.sh /dev/cu.usbserialXXXX
```

## Step 4 - Monitor

```bash
tools/dev/monitor_lunar_poc.sh /dev/cu.usbmodemXXXX
```

Expected boot output:

```text
[BOOT] Mnemor Brew Engine Lunar PoC
[BLE] scan started
[BLE] Searching...
```

Expected connection output:

```text
[BLE] found name=Acaia Lunar address=... rssi=...
[BLE] connected
[BLE] notifications subscribed
timestamp_ms,weight_g,battery_percent,connected
```

Expected weight output:

```text
4812,0.00,82,1
[SCALE] weight=0.00 g
```

## Step 5 - Tare Test

In the serial monitor, send:

```text
t
```

Expected:

```text
[SCALE] tare sent
```

Then place or remove a cup and confirm CSV weight changes.

## Step 6 - Stability Test

Let the monitor run for at least 10 minutes.

Pass signal:

- no repeated disconnect loop
- heartbeat appears periodically
- weight stream continues
- CSV lines remain parseable

## Step 7 - Reconnect Test

1. Turn Lunar off.
2. Confirm disconnect or stale-stream logs.
3. Turn Lunar on.
4. Confirm scan/connect resumes.

## Evidence To Attach To Issues

- log file from `data/raw/m1-lunar-ble/`
- exact ESP32 board model
- serial port
- Lunar model and firmware if known
- whether OLED worked
- whether tare worked
- 10-minute stability result

## Current Known Blocker

In the Codex runtime on 2026-07-31, no ESP32 USB serial device was visible. Only:

```text
/dev/cu.debug-console
/dev/cu.Bluetooth-Incoming-Port
```

Hardware verification therefore starts when the ESP32 appears as a USB serial device.
