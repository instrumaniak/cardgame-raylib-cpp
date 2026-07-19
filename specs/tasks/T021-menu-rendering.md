# T021: Menu Rendering

## Status: pending

## Goal
HOME screen UI (title, hero grid, start button, gold display).

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "Menu Drawing" section

## Files to Create/Modify
- `src/render/menu_draw.h` (NEW)
- `src/render/menu_draw.cpp` (NEW)

## Implementation Steps
1. Create `menu_draw.h` with `drawTitle`, `drawHeroGrid`, `drawStartButton`, `drawGoldDisplay` in `game::render::`
2. `drawTitle()`: "Drop the Volunteer" centered, big (PearSoda ~48px)
3. `drawHeroGrid()`: 2 rows x 4 cols, hero art + name + stats, locked heroes show lock icon + cost, selected hero highlight border
4. `drawStartButton()`: centered below grid
5. `drawGoldDisplay()`: top-right coin icon + amount

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] Title centered
- [ ] Hero grid shows 8 heroes with locked state
- [ ] Start button visible
- [ ] Gold displayed

## Notes
( filled in during/after implementation )
