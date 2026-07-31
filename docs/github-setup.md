# GitHub Setup

## Repository Type

Recommendation for now: private repository.

Reason: The project is in validation phase and may later contain:

- proprietary BLE integration notes
- controlled shot data
- early product positioning
- interview evidence
- business assumptions

Open source can be reconsidered after the commercial direction is clear.

## Branches

Use a simple branch model:

- `main`: stable documentation and validated work
- `develop`: integration branch while the project is active
- `feature/*`: focused implementation or documentation branches

Examples:

- `feature/lunar-ble-scan`
- `feature/shot-schema`
- `feature/flow-calculation`
- `feature/interview-guide`

## Commit Convention

Use short conventional commits:

```text
docs: define validation plan
feat(ble): scan for lunar scale
feat(data): add shot schema v0.1.0
test(flow): add shot detection fixtures
fix(ble): handle reconnect after disconnect
```

## GitHub Project Board

Columns:

- Backlog
- Ready
- Doing
- Testing
- Done

Labels:

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

## First Remote Setup Commands

Run these from the repository root after creating the remote on GitHub:

```bash
git add .
git commit -m "docs: scaffold validation-first brew engine repo"
git branch -M main
git remote add origin git@github.com:<owner>/mnemor-brew-engine.git
git push -u origin main
```

Replace `<owner>` with the GitHub account or organization.

## Remote Status

Checked on 2026-07-31 after local account switch:

```text
origin = https://github.com/ordinalsog-ctrl/mnemor-brew-engine.git
branch = feature/lunar-ble
default branch = main
```

GitHub CLI active account:

```text
ordinalsog-ctrl
```

Remote branches:

```text
main
feature/lunar-ble
```

Note: Push originally failed because `weissio` was the active local GitHub account. Switching GitHub CLI to `ordinalsog-ctrl` fixed push access.

## First Issues

Use `tools/work-packages.csv` as the source list for the first GitHub issues.
