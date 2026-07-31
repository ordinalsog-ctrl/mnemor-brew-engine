# Shot Schema

The shot schema is the central data contract for firmware, analysis, and recommendation work.

## Version

Current schema version: `0.1.0`

## Required Top-Level Fields

- `schema_version`
- `shot_id`
- `captured_at`
- `setup`
- `bean`
- `recipe`
- `raw_weight_stream`
- `metrics`
- `feedback`
- `recommendation`
- `outcome`

## Field Notes

### setup

Describes the hardware and relevant configuration.

Fields:

- `machine`
- `grinder`
- `scale`
- `basket`
- `water`

### bean

Describes the coffee.

Fields:

- `name`
- `roaster`
- `roast_date`
- `days_off_roast`
- `process`
- `origin`

Unknown values must be `null`, not omitted.

### recipe

Describes the intended shot.

Fields:

- `dose_g`
- `target_yield_g`
- `target_ratio`
- `target_time_s`
- `grind_setting`
- `temperature_c`

### raw_weight_stream

Array of timestamped weight values.

Each entry:

- `t_ms`
- `weight_g`

### metrics

Computed shot values.

Fields:

- `detected_start_ms`
- `detected_end_ms`
- `shot_time_s`
- `final_yield_g`
- `average_flow_g_s`
- `peak_flow_g_s`
- `end_flow_g_s`
- `flow_stability_score`
- `data_quality_score`

### feedback

User feedback after tasting.

Fields:

- `overall_score`
- `sourness`
- `bitterness`
- `body`
- `sweetness`
- `astringency`
- `notes`

Use small scales, for example 1 to 5. The exact scale can change, but it must be documented.

### recommendation

The recommendation made after the shot.

Fields:

- `action`
- `magnitude`
- `reason`
- `confidence`
- `keep_constant`

### outcome

Result after the recommendation is tried.

Fields:

- `recommendation_followed`
- `next_shot_id`
- `improved`
- `outcome_notes`

## Principle

Missing data must be explicit. Bad data is still useful if marked clearly.
