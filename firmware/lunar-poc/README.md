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

Out of scope:

- OLED UI
- buttons
- recommendation logic
- enclosure
- app integration
- broad scale support

## Target Serial Output

```text
[BLE] scan started
[BLE] Lunar found: Acaia Lunar
[BLE] connected
[SCALE] battery=82
[SCALE] weight=0.00
[SCALE] weight=186.42
[SCALE] tare sent
[SCALE] weight=0.00
[BLE] stream_ok duration_s=600 gaps=0
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

## Notes

Keep this proof boring. No UI polish until this passes.
