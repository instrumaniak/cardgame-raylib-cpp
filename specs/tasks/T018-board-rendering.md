# T018: Board Rendering

## Status: done
## Phase: 4

## Goal
Draw the full board grid with correct fog behavior.

## Depends On
- T017 (Card Rendering)
- T014 (Fog Visibility)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "Board Drawing" section
- `game/08-fog-visibility.md`

## Files to Create/Modify
- `src/render/board_draw.h` (NEW)
- `src/render/board_draw.cpp` (NEW)

## Implementation Steps
1. Create `board_draw.h` with `drawBoard()` declaration in `game::render::`
2. Center board in right 70% of screen
3. Each row: 3 card slots equally spaced, row height ~120px
4. Fog logic: revealed rows show face-up, fogged rows show card-back with dark overlay, hidden rows not drawn
5. Active row highlight glow
6. Chest sub-location overlay

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Board centered correctly
- [x] Fog shows/hides rows per monster-eye count
- [x] Active row highlighted

## Notes
- Chest sub-location overlay deferred to Game Screen (T024) since spec API has no chest parameters
- Active row uses `Fade(GoldBg, 0.3f)` highlight; animation enhancement for T028
- `getNbRowsHidden` already existed in `logic/items.h` — reused instead of duplicating in `fog.h`
