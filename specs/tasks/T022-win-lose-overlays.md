# T022: Win/Lose Overlays

## Status: pending
## Phase: 4

## Goal
Win and lose overlay screens with stats and action buttons.

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "WIN/LOSE Screens" section

## Files to Create/Modify
- `src/render/win_overlay.h` (NEW)
- `src/render/win_overlay.cpp` (NEW)
- `src/render/lose_overlay.h` (NEW)
- `src/render/lose_overlay.cpp` (NEW)

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
- [ ] Win shows full gold
- [ ] Lose shows half gold
- [ ] Buttons visible and positioned correctly

## Notes
( filled in during/after implementation )
