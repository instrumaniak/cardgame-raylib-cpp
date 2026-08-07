# T022: Win/Lose Overlays

## Status: done
## Phase: 4

## Goal
Win and lose overlay screens with stats and action buttons.

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "WIN/LOSE Screens" section

## Files to Create/Modify
- `src/render/overlay.h` (NEW)
- `src/render/overlay.cpp` (NEW)

## Implementation Steps
1. Create overlay headers with `drawWinOverlay()` and `drawLoseOverlay()` in `game::render::`
2. Semi-transparent dark overlay
3. Win: "Victory!" title (gold), stats (hero name, gold earned, total gold, score), "Play Again" and "Main Menu" buttons
4. Lose: "Defeat" title (red), same stats with half gold, same buttons

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Win shows full gold
- [x] Lose shows half gold
- [x] Buttons visible and positioned correctly

## Notes
- Merged win/lose into single `overlay.h`/`overlay.cpp` with shared `drawOverlay()` helper
- Added `heroName` param to both signatures (required by task but missing from spec)
- Added `Button&` params for Play Again/Try Again and Main Menu (enables hit-testing by caller, matches menu_draw.cpp pattern)
- Lose overlay displays `(goldEarned + 1) / 2` (rounding, not truncation)
- `screenWidth`/`screenHeight` naming follows coding style (no abbreviations)
- Buttons positioned side-by-side centered below stats
- Build: 0 warnings, format clean, 93 tests pass
