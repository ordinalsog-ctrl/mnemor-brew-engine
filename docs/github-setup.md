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

## Current Remote Status

Checked on 2026-07-31:

```text
origin = https://github.com/ordinalsog-ctrl/mnemor-brew-engine.git
branch = feature/lunar-ble
```

GitHub App repository permissions:

```text
pull=true
push=false
```

Local push attempt:

```text
Permission to ordinalsog-ctrl/mnemor-brew-engine.git denied to weissio.
HTTP 403
```

Required fix:

- grant write access to the GitHub account used locally, or
- grant Contents write permission to the connected GitHub App, or
- switch `origin` to a repository where the local account has push permission.

## First Issues

Use `tools/work-packages.csv` as the source list for the first GitHub issues.
