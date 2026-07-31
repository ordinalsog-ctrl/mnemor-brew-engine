# Requirements

## Requirement Format

Each requirement uses:

- ID
- Description
- Priority
- Status
- Acceptance criteria

Statuses:

- Open
- In Progress
- Verified
- Rejected

Priorities:

- P0: required for validation
- P1: important for product usefulness
- P2: later product quality

## Functional Requirements

### REQ-001 - Known Lunar Connection

Priority: P0

Status: Open

Description: The device connects to a known Acaia Lunar without user interaction after startup.

Acceptance criteria:

- 95% success rate across 20 normal startup attempts
- connection status is visible in logs
- retry behavior is documented

### REQ-002 - Weight Stream

Priority: P0

Status: Open

Description: The device receives timestamped weight values during an espresso shot.

Acceptance criteria:

- data includes timestamp and grams
- gaps are detected and logged
- raw stream can be exported

### REQ-003 - Tare Command

Priority: P0

Status: Open

Description: The device can send a tare command to the scale.

Acceptance criteria:

- tare command succeeds during normal preparation
- failure is logged
- no corrupt shot data after tare

### REQ-004 - Automatic Shot Start

Priority: P0

Status: Open

Description: The system detects shot start from weight or flow behavior.

Acceptance criteria:

- at least 95% correct on validation shots
- false starts from vibration are classified

### REQ-005 - Automatic Shot End

Priority: P0

Status: Open

Description: The system detects shot end and final yield.

Acceptance criteria:

- final yield error stays within acceptable espresso workflow tolerance
- late drips are handled consistently

### REQ-006 - Shot JSON Export

Priority: P0

Status: Open

Description: Each shot can be exported as structured JSON.

Acceptance criteria:

- schema version is included
- machine, grinder, bean, dose, yield, time, flow metrics, and feedback fields exist

### REQ-007 - One-Variable Recommendation

Priority: P0

Status: Open

Description: The recommendation engine changes at most one main brew variable per next-shot suggestion.

Acceptance criteria:

- recommendation identifies the target variable
- recommendation includes reason and confidence
- recommendation can be compared to actual next-shot outcome

### REQ-008 - Taste Feedback

Priority: P1

Status: Open

Description: User can provide minimal taste feedback after a shot.

Acceptance criteria:

- input takes less than 5 seconds
- feedback maps to structured fields
- "skip" is allowed and tracked

## Non-Functional Requirements

### NFR-001 - Local First

Priority: P0

Status: Open

Description: The system must work without cloud connectivity for capture and recommendation.

### NFR-002 - Repeatability

Priority: P0

Status: Open

Description: Identical input must produce identical recommendation output for a fixed model version.

### NFR-003 - Data Export

Priority: P1

Status: Open

Description: User data must be exportable in human-readable structured format.
