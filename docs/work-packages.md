# Work Packages

Use these as the first GitHub issues.

## Board Columns

- Backlog
- Ready
- Doing
- Testing
- Done

## Labels

- `type:docs`
- `type:firmware`
- `type:analysis`
- `type:research`
- `type:data`
- `priority:p0`
- `priority:p1`
- `risk:high`
- `milestone:m0`
- `milestone:m1`
- `milestone:m2`
- `milestone:m3`
- `milestone:m4`

## M0 - Repository

### AP-001 - Create Repository

Labels: `type:docs`, `priority:p0`, `milestone:m0`

Acceptance criteria:

- repository exists locally
- README and ROADMAP are present
- `.gitignore` is present

### AP-002 - Define Product Hypotheses

Labels: `type:docs`, `priority:p0`, `milestone:m0`

Acceptance criteria:

- hypotheses are testable
- each hypothesis has pass/fail signal

### AP-003 - Define Go/No-Go Criteria

Labels: `type:docs`, `priority:p0`, `milestone:m0`

Acceptance criteria:

- technical, product, and business criteria exist
- stop conditions are explicit

### AP-004 - Define Shot Schema

Labels: `type:data`, `priority:p0`, `milestone:m0`

Acceptance criteria:

- schema supports raw stream, metrics, feedback, recommendation, and outcome
- sample shot file exists

## M1 - Proof of Data

### AP-010 - PlatformIO Project

Labels: `type:firmware`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- `firmware/lunar-poc/platformio.ini` exists
- project builds for ESP32-S3 target
- build and flash commands are documented

### AP-011 - OLED Initialization

Labels: `type:firmware`, `priority:p1`, `milestone:m1`

Acceptance criteria:

- OLED shows `Mnemor Brew` and `Booting...`
- OLED failure does not block BLE logging

### AP-012 - ESP32 Lunar BLE Scan

Labels: `type:firmware`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- device scans for BLE peripherals
- Lunar can be identified in logs
- scan failure is logged

### AP-013 - Lunar Connection

Labels: `type:firmware`, `priority:p0`, `milestone:m1`, `risk:high`

Acceptance criteria:

- ESP32 connects to the Lunar
- reconnect behavior is tested
- 20 startup attempts are measured

### AP-014 - Notifications Subscribe

Labels: `type:firmware`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- data characteristic notifications are registered
- notification request is sent after connection
- heartbeat keeps stream alive

### AP-015 - Weight Stream Capture

Labels: `type:firmware`, `type:data`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- weight is printed over serial with timestamp
- at least 10 minutes of stream is captured
- gaps are counted

### AP-016 - Tare Command

Labels: `type:firmware`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- tare can be triggered from firmware
- result is logged
- scale returns to usable stream after tare

### AP-017 - Reconnect Handling

Labels: `type:firmware`, `priority:p0`, `milestone:m1`, `risk:high`

Acceptance criteria:

- turning the Lunar off logs disconnect or stale stream
- turning it back on starts scan/connect loop
- recovered stream continues CSV output

### AP-018 - CSV Logger

Labels: `type:firmware`, `type:data`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- serial output includes `timestamp_ms,weight_g,battery_percent,connected`
- values are readable by a CSV parser
- logs can be saved from USB serial monitor

### AP-019 - First 50 Shot Dataset

Labels: `type:data`, `type:analysis`, `priority:p0`, `milestone:m1`

Acceptance criteria:

- 50 shots are recorded
- each shot follows schema
- missing fields are marked explicitly

## M2 - Shot Engine

### AP-020 - Flow Calculation

Labels: `type:analysis`, `priority:p0`, `milestone:m2`

Acceptance criteria:

- flow is calculated from weight over time
- filtering method is documented
- sample plots are generated

### AP-021 - Shot Start Detection

Labels: `type:analysis`, `priority:p0`, `milestone:m2`

Acceptance criteria:

- start threshold is documented
- false-start cases are tested
- accuracy target is measured

### AP-022 - Shot End Detection

Labels: `type:analysis`, `priority:p0`, `milestone:m2`

Acceptance criteria:

- end condition is documented
- late drips are handled consistently
- final yield is calculated

## M3 - Recommendation

### AP-030 - Baseline Rule Set

Labels: `type:analysis`, `priority:p0`, `milestone:m3`

Acceptance criteria:

- baseline rules are written
- rules are deterministic
- each rule explains which variable changes and why

### AP-031 - Recommendation Output Format

Labels: `type:data`, `type:analysis`, `priority:p0`, `milestone:m3`

Acceptance criteria:

- recommendation contains action, magnitude, reason, confidence, and keep-constant fields

### AP-032 - Recommendation Case Studies

Labels: `type:analysis`, `priority:p0`, `milestone:m3`

Acceptance criteria:

- at least 5 cases compare product recommendation against baseline
- next-shot outcome is documented

## M4 - User Validation

### AP-040 - Interview Guide

Labels: `type:research`, `priority:p0`, `milestone:m4`

Acceptance criteria:

- guide avoids leading questions
- focuses on recent real behavior
- includes willingness-to-pay test

### AP-041 - 10 External Interviews

Labels: `type:research`, `priority:p0`, `milestone:m4`

Acceptance criteria:

- 10 interviews completed
- notes stored in `research/interviews/`
- problem severity summarized

### AP-042 - Prototype Tester Cohort

Labels: `type:research`, `type:firmware`, `priority:p1`, `milestone:m4`

Acceptance criteria:

- 5 to 10 testers selected
- retention and usage measured
- final Go/No-Go recommendation written
