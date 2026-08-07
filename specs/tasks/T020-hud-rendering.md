# T020: HUD Rendering

## Status: done
## Phase: 4

## Goal
Top HUD showing level indicator, gold earned, and visual effects.

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "HUD Drawing" section

## Files to Create/Modify
- `src/render/hud_draw.h` (NEW)
- `src/render/hud_draw.cpp` (NEW)

## Implementation Steps
1. Create `hud_draw.h` with `drawHud()` declaration in `game::render::`
2. Level indicator: top-left "Level X"
3. Gold earned this run: top-right
4. Screen shake: offset all draw calls by random(-intensity, +intensity) for duration

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Level displayed correctly
- [x] Gold earned shown
- [x] Screen shake offsets rendering

## Notes
Implemented drawHud() with level indicator (top-left), gold earned (top-right), and screen shake effect that offsets all draw calls by random(-intensity, +intensity) when ScreenShake effect is active. Also created components.cpp for AnimState method implementations.

Note: drawHud() is not yet wired into any screen's draw() method. This is consistent with the current phase (building render functions ahead of screen implementations). Wiring into the game screen is a follow-up task.
