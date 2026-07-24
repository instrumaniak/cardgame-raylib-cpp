# T014: Fog Visibility

## Status: pending
## Phase: 2

## Goal
Fog visibility logic for row display based on monster-eye count.

## Depends On
- T010 (Item System)

## Spec References
Load these before starting:
- `game/08-fog-visibility.md` → fog mechanics
- `raylib-cpp/02-gameplay-systems.md` → getNbRowsHidden in combat.h

## Files to Create/Modify
- Add functions to `src/logic/board_gen.h` (or create `src/logic/fog.h` if preferred)

## Implementation Steps
1. Implement row visibility logic: 3 rows visible from currentRow
2. Fog based on monster-eye count (0/1/2+ eyes)
3. Visibility rule: `rowId >= currentRow && rowId < currentRow + 3`
4. Fog rule: `rowId > currentRow + (2 - nbRowsHidden)`
5. Cap `nbRowsHidden` at 2
6. Integrate with `getNbRowsHidden` from items

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- Pure function, no side effects

## Acceptance Criteria
- [ ] Correct visibility for 0 monster-eyes (all 3 rows visible)
- [ ] Correct visibility for 1 monster-eye (1 row fogged)
- [ ] Correct visibility for 2+ monster-eyes (2 rows fogged)
- [ ] Fog correctly hides/shows rows
- [ ] `make test` passes

## Notes
( filled in during/after implementation )
