# T015: Level Configuration

## Status: done
## Phase: 2

## Goal
Level configuration helpers for all 6 levels and chest sub-locations.

## Depends On
- T003 (Core Types)

## Spec References
Load these before starting:
- `game/09-level-progression.md` → level table and progression
- `raylib-cpp/02-gameplay-systems.md` → "Level Config" section

## Files to Create/Modify
- `src/logic/level_config.h` (NEW)
- `src/logic/level_config.cpp` (NEW)
- `src/logic/test_level_config.cpp` (NEW)

## Implementation Steps
1. Implement `getLevelConfig(level)` returning `LevelConfig` for levels 1-6
2. Implement `getChestConfig(chestType, level)` for regular/golden chests
3. Implement `getAvailableBiomes(level, visitedBiomes)` with exclusion logic
4. All 6 level configs from `game/09` table (rows, rates, monster values)
5. Chest configs: regular chest and golden chest per level
6. Write unit tests in `test_level_config.cpp`: configs for all 6 levels, chest configs, biome exclusion

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- Data-only functions, no side effects

## Acceptance Criteria
- [x] Correct configs for all 6 levels
- [x] Chest configs match spec
- [x] Biome exclusion works correctly
- [x] `make test` passes

## Notes
Extracted level config functions from board_gen.h/cpp into separate level_config.h/cpp module.
Tests expanded to cover all 6 levels, both chest types, and biome exclusion edge cases.
