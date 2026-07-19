# T020: HUD Rendering

## Status: pending

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
- [ ] Level displayed correctly
- [ ] Gold earned shown
- [ ] Screen shake offsets rendering

## Notes
( filled in during/after implementation )
