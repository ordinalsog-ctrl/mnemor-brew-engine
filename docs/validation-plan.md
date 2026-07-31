# Validation Plan

## Objective

Decide within 6 to 8 weeks whether Mnemor Brew Engine should continue as a product.

## Week 1 - Scope and Instrumentation

Actions:

- finalize reference setup
- define shot schema
- define taste feedback scale
- build or adapt Lunar BLE proof of data
- capture stable serial output

Deliverables:

- sample raw weight stream
- sample structured shot JSON
- first BLE reliability notes

Decision question:

- Can the scale data be captured without constant intervention?

## Week 2 - Controlled Internal Data

Actions:

- capture first 20 controlled shots
- log dose, yield, grind, time, bean age, taste, and comments
- change only one variable between comparable shots
- document failure modes

Deliverables:

- 20 shot JSON files
- first flow plots
- first baseline rules

Decision question:

- Is the data clean enough to support useful analysis?

## Week 3 - Shot Detection and Metrics

Actions:

- implement shot start/end detection in analysis environment
- calculate flow metrics
- compare detected events with manual notes
- define confidence scoring

Deliverables:

- shot detection spec
- flow calculation spec
- detection accuracy report

Decision question:

- Can shot boundaries be detected reliably enough?

## Week 4 - Recommendation Baseline

Actions:

- write deterministic rule engine
- define "one variable only" recommendation policy
- create baseline barista-rule comparison
- test on internal data

Deliverables:

- recommendation rules
- recommendation output format
- first comparison report

Decision question:

- Does the engine produce concrete, non-generic advice?

## Week 5 - Internal Recommendation Loop

Actions:

- follow recommendations during real brewing
- record whether the next shot improved
- document ignored recommendations and reasons
- refine confidence rules

Deliverables:

- 50 controlled shots
- 5 recommendation case studies
- pass/fail against baseline

Decision question:

- Is there measurable value beyond normal human judgement?

## Week 6 - External Interviews

Actions:

- interview 10 premium espresso users
- avoid "do you like the idea" questions
- focus on recent dial-in pain and current behavior
- test price sensitivity

Deliverables:

- interview notes
- problem severity summary
- early positioning language

Decision question:

- Is the problem painful enough outside the founder setup?

## Weeks 7-8 - External Prototype Test

Actions:

- test with 5 to 10 external users if hardware is stable
- alternatively run concierge analysis from manually submitted shot data
- measure retention and willingness to pay

Deliverables:

- external usage report
- Go/No-Go recommendation
- revised roadmap

Decision question:

- Should this become a sellable product?
