# Roadmap

This roadmap is designed to prevent premature platform work. Each milestone must create evidence.

## M0 - Repository and Decision System

Status: Open

Goal: Create a repository that can hold product, technical, and validation work without becoming theatre.

Exit criteria:

- README defines the narrow target user and non-goals.
- Product hypotheses are written.
- Go/No-Go criteria are explicit.
- Initial work packages are ready as GitHub issues.
- Shot data schema exists.

## M1 - Proof of Data

Status: Open

Goal: Capture reliable raw data from the Acaia Lunar.

Exit criteria:

- Lunar is found by ESP32 BLE scan.
- ESP32 connects to known scale without manual reset in at least 95% of normal startup attempts.
- Weight stream is captured with timestamps.
- Battery level is captured if available.
- Tare command is tested.
- At least 50 controlled shots are stored as structured JSON.
- Data gaps and BLE disconnects are logged.

Kill condition:

- BLE stability cannot be made boring enough for daily use.

## M2 - Proof of Shot Detection

Status: Open

Goal: Convert weight stream into useful shot events.

Exit criteria:

- Shot start is detected automatically.
- Shot end is detected automatically.
- Flow curve is calculated.
- Dose, yield, ratio, time, peak flow, average flow, and end flow are exported.
- Detection accuracy reaches at least 95% across normal user shots.
- False starts from vibration, cup movement, or scale noise are documented.

Kill condition:

- User must still manually manage start/stop often enough that the device feels like work.

## M3 - Proof of Recommendation

Status: Open

Goal: Recommend one controlled next adjustment.

Exit criteria:

- Recommendation engine never changes more than one main variable at a time.
- System can choose between grind, yield, dose, temperature, and "repeat unchanged".
- Recommendation includes reason, confidence, and expected effect.
- Same input produces same recommendation.
- Recommendations are benchmarked against simple baseline rules.
- At least 5 recommendations produce better next shots than the baseline decision.

Kill condition:

- The recommendation is mostly generic advice disguised as intelligence.

## M4 - Proof of Value

Status: Open

Goal: Test with external premium espresso users.

Exit criteria:

- 10 to 20 external testers complete onboarding.
- At least 60% are still using the system after 4 weeks.
- At least 50% report fewer dial-in attempts.
- At least 40% state willingness to pay 149 EUR or more.
- Interview evidence shows the device solves a real workflow pain.

Kill condition:

- Users call it interesting but do not use it repeatedly.

## M5 - Productization Decision

Status: Future

Goal: Decide whether to build a sellable device.

Possible outcomes:

- Continue as premium hardware accessory.
- Pivot to software-only companion.
- Pivot to data/algorithm library.
- Stop commercial work and keep as personal tool.

Productization is not started until M1-M4 evidence exists.
