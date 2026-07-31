# Decision Log

## DEC-001 - Start Narrow

Date: 2026-07-31

Decision: The first reference setup is limited to Acaia Lunar 2021 / AL008 class scales and a premium home espresso workflow.

Reason: Broad scale support is a distraction before user value is proven.

## DEC-002 - Algorithm First

Date: 2026-07-31

Decision: Shot detection, flow calculation, and recommendation logic are validated with recorded data before being ported deeply into firmware.

Reason: Firmware polish does not prove product value.

## DEC-003 - Recommendation Over Tracking

Date: 2026-07-31

Decision: The product is judged by next-shot recommendation quality, not by capture or visualization alone.

Reason: Tracking and graphing are already available in existing coffee apps.

## DEC-004 - Local First

Date: 2026-07-31

Decision: Capture and recommendation must work locally without cloud dependency in the first version.

Reason: Local-first operation reduces workflow friction and preserves user ownership of data.

## DEC-005 - M1 Uses LunarGateway Protocol Notes, Not Deep Product Coupling

Date: 2026-07-31

Decision: The first Lunar proof uses the public LunarGateway protocol constants and message behavior as a reference, with explicit attribution, but keeps the code isolated in a local protocol adapter.

Reason: This accelerates proof of data while preserving the option to replace or rewrite the scale layer later.
