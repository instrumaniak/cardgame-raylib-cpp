# T009: Board Generation

## Status: pending
## Phase: 2

## Goal
Board generation from LevelConfig with weighted random card placement.

## Depends On
- T003 (Core Types)

## Spec References
Load these before starting:
- `game/01-board-generation.md` → "Board Generation" section
- `game/09-level-progression.md` → relevant section
- `raylib-cpp/02-gameplay-systems.md` → "Board Generation" section

## Files to Create/Modify
- `src/logic/board_gen.h` (NEW)
- `src/logic/board_gen.cpp` (NEW)
- `src/logic/test_board_gen.cpp` (NEW)

## Implementation Steps
1. Create `board_gen.h` with `GeneratedBoard` struct and `generateBoard()` function in `game::logic::`
2. Implement `weightedPick<T>()` template helper
3. Implement `generateBoard()` algorithm: create rows x 3 slots, place chests in rows 0..totalRows-4, place items in rows 0..totalRows-3, fill first row (no monsters), fill last row (biome or end card), fill second-to-last row (boss in center), fill remaining rows with weighted random
4. Card rates per level from `game/01` table
5. Cards-per-row distribution per level
6. Write unit tests in `test_board_gen.cpp`: dimensions, first row no monsters, boss placement, chest rows, weighted rates

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] Generates correct board dimensions
- [ ] First row has no monsters
- [ ] Last row has biome/end card
- [ ] Boss in second-to-last row center
- [ ] Chest placement correct
- [ ] `make test` passes
- [ ] No warnings with `-Wall -Wextra`

## Notes
( filled in during/after implementation )