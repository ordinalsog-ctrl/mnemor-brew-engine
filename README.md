# Mnemor Brew Engine

Mnemor Brew Engine is a validation-first espresso intelligence project for premium home espresso setups.

The first target setup is intentionally narrow:

- La Marzocco Linea Micra or Linea Mini class machine
- Acaia Lunar 2021 / AL008 or compatible Bluetooth scale
- Niche Zero or comparable single-dose grinder
- ESP32-based local device

The product hypothesis is not "another espresso tracker". The product hypothesis is:

> An always-ready local assistant can capture each shot automatically, learn from controlled changes, and recommend the single most useful next adjustment.

## Current Thesis

This is not yet a venture-scale company. It is a capital-light premium niche product hypothesis with one potentially strong value core:

- reduce wasted dial-in shots
- make good recipes reproducible
- learn how a specific user setup behaves
- avoid smartphone friction during brewing
- generate concrete next-shot recommendations

The project becomes commercially weak if it only shows weight, flow, timers, or shot history. Those features already exist in other products and apps.

## Validation Order

1. Proof of Data
   Stable Lunar connection, clean weight stream, reliable shot capture.

2. Proof of Learning
   Recommendations from real shot history outperform simple barista rules.

3. Proof of Value
   External users need fewer attempts and are willing to pay.

Only after those three proofs should the project expand into industrial design, broad device support, app polish, partnerships, or investor materials.

## Repository Map

```text
mnemor-brew-engine/
├── README.md
├── ROADMAP.md
├── CHANGELOG.md
├── docs/
│   ├── product-brief.md
│   ├── investor-ceo-assessment.md
│   ├── product-hypotheses.md
│   ├── validation-plan.md
│   ├── go-no-go-criteria.md
│   ├── requirements.md
│   ├── technical-architecture.md
│   ├── work-packages.md
│   └── decision-log.md
├── firmware/
│   └── lunar-poc/
├── analysis/
│   ├── shot-schema.md
│   └── notebooks/
├── data/
│   └── samples/
├── research/
│   └── interviews/
└── .github/
    └── ISSUE_TEMPLATE/
```

## Immediate Next Step

Do not start with a polished device.

Start with a Lunar proof of data:

- ESP32 scans for the Lunar
- ESP32 connects reliably
- weight is streamed over serial
- tare command is tested
- 5 to 10 minutes of stable streaming are captured
- the first sample shot JSON is generated

## Non-Goals

For the first validation cycle, this project explicitly does not attempt to:

- support every Bluetooth scale
- build a mobile app
- build a cloud backend
- design a custom PCB
- contact Acaia or La Marzocco
- market itself as AI-first
- create a subscription business

The first product proof is narrower and harder: generate a next-shot recommendation that a serious home barista actually follows.
