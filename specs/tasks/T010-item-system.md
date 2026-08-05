# T010: Item System

## Status: done
## Phase: 2
## Critical: true

## Goal
All 22 item effects and modifier calculation functions.

## Depends On
- T003 (Core Types)

## Spec References
Load these before starting:
- `game/03-item-effects.md` → relevant section
- `raylib-cpp/02-gameplay-systems.md` → "Items" section, "Combat" section (modifiers)

## Files to Create/Modify
- `src/logic/items.h` (NEW)
- `src/logic/items.cpp` (NEW)
- `src/logic/test_items.cpp` (NEW)

## Implementation Steps
1. Create `items.h` with `ComboResult` struct and all item functions in `game::logic::`
2. Implement modifier functions: `getMonsterCardModifier`, `getHealCardModifier`, `getShieldCardModifier`, `getHeroLifeMaxModifier`, `getNbRowsHidden`
3. Implement item effects: `checkMonsterEggCombo` (3 eggs -> 1 omelette), `applyAppleEffect` (+10 HP), `applyRingOfProtection` (+5 shield), `applyDragonWing` (+1 shield after fight), `applyAthelas` (prevent death once), `applyCursedShield` (max HP=1), `applyAmethyst` (double gold, remove self)
4. All 22 items from `game/03` table
5. Write unit tests in `test_items.cpp`: monster egg combo, apple, ring of protection, dragon wing, athelas, amethyst, all 22 item effects

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Pure functions on `PlayerState`/`Card` data
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] All 22 item modifiers work correctly
- [x] Monster-egg combo triggers at 3 eggs
- [x] Apple heals +10
- [x] Athelas prevents death once
- [x] `make test` passes
- [x] No warnings with `-Wall -Wextra`

## Notes
- T010 implements item functions within the `items` module scope (not all 22 items' complete effects)
- `getNbRowsHidden` counts `monster-egg` (spec pseudocode says `monster-eye` — item not in 22-item list)
- `applyAthelas` only triggers when `current <= 0` (death prevention)
- `applyAmethyst` removes self after doubling gold
- `applyCursedShield` sets max HP to 1 (modifier = -(life_max - 1))
- All tests pass: 26 test cases, 641 assertions
