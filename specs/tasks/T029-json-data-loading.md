# T029: JSON Data Loading

## Status: pending
## Phase: 6

## Goal
Load game data (heroes, cards, items, levels) from JSON files.

## Depends On
- T003 (types)
- T002 (assets)

## Spec References
- `game/00-overview.md` (hero table)
- `game/02-card-types.md`
- `game/09-level-progression.md`
- `raylib-cpp/01-architecture.md` (mentioned in data/)

## Files to Create/Modify
- `src/data/game_data.h` (NEW)
- `src/data/game_data.cpp` (NEW)

## Implementation Steps
1. Create game_data.h with load functions in `game::data::` namespace
2. Create JSON files: heroes.json (4 heroes), cards.json (all card types), items.json (22 items), levels.json (6 level configs), biomes.json
3. Implement loadHeroes(), loadCards(), loadItems(), loadLevels()
4. Use nlohmann/json for parsing
5. Fallback to hardcoded defaults if JSON missing

## Constraints
- `game::data::` namespace
- `#pragma once` header guard
- nlohmann/json
- graceful fallback
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] All data loads from JSON
- [ ] Fallback works
- [ ] Data matches spec tables

## Notes
(filled in during/after implementation)
