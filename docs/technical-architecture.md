# Technical Architecture

## Architecture Principle

Algorithm first, firmware second.

The ESP32 is the first runtime environment, not the source of product value. The product value is the data model, shot interpretation, and recommendation loop.

## System Overview

```text
Acaia Lunar
  -> BLE Adapter
  -> Weight Stream
  -> Filter
  -> Shot Detector
  -> Flow Engine
  -> Metrics
  -> Recommendation Engine
  -> User Feedback
  -> Personal Model
```

## Firmware Boundary

The firmware should do only what must happen close to the machine:

- scan and connect to scale
- receive weight data
- send tare command
- timestamp events
- expose logs
- optionally show minimal local state
- export raw or structured shot data

The firmware should not initially contain complex learning logic.

## Analysis Boundary

The analysis environment owns:

- shot detection experiments
- flow filtering
- rule validation
- recommendation tests
- plots and regression checks

Only proven algorithms move into firmware.

## Scale Interface

Future scale support must go through a stable interface.

```cpp
class ScaleInterface {
public:
    virtual bool connect() = 0;
    virtual bool isConnected() const = 0;
    virtual float getWeightGrams() const = 0;
    virtual int getBatteryPercent() const = 0;
    virtual bool tare() = 0;
};
```

Initial implementation:

- `AcaiaLunarAdapter`

Do not add Bookoo, DiFluid, Decent, or generic scale support until the first product proof is complete.

## Recommendation Engine

The recommendation engine is deterministic in the first phase.

Inputs:

- current shot metrics
- previous comparable shots
- bean age
- target recipe
- taste feedback
- last adjustment

Outputs:

- action
- magnitude
- reason
- confidence
- expected effect
- variables to keep constant

Example:

```json
{
  "action": "grind_finer",
  "magnitude": "small",
  "reason": "Shot ran fast and taste feedback indicates sourness while yield target was reached.",
  "confidence": 0.62,
  "keep_constant": ["dose_g", "yield_g", "temperature_c"]
}
```

## Data Storage

First phase:

- local JSON files
- one file per shot
- schema versioned
- no cloud dependency

Later:

- local database
- optional export/import
- optional encrypted sync

## Risk Register

### BLE Stability

Risk: Medium

Mitigation:

- measure reconnect success
- log disconnect reasons
- keep protocol layer isolated

### Data Quality

Risk: High

Mitigation:

- controlled experiments
- one-variable changes
- explicit missing-data fields

### Recommendation Validity

Risk: High

Mitigation:

- compare against baseline rules
- store outcome of every recommendation
- track confidence calibration

### Hardware Distraction

Risk: Medium

Mitigation:

- defer enclosure, PCB, production, and app polish until validation passes
