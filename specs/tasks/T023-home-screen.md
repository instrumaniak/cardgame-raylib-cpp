# T023: Home Screen

## Status: pending

## Goal
HomeScreen that handles hero selection, account display, and game start.

## Depends On
- T006 (ScreenManager)
- T013 (account)
- T021 (menu rendering)

## Spec References
- `raylib-cpp/01-architecture.md` → "Screen Lifecycle" table (Home row)
- `game/10-ui-screens.md`

## Files to Create/Modify
- `src/screens/home_screen.h` (NEW)
- `src/screens/home_screen.cpp` (NEW)

## Implementation Steps
1. Create HomeScreen class derived from Screen in `game::`
2. onEnter(): load account, build hero grid state (8 heroes with lock/status)
3. update(dt): handle hero selection clicks (highlight selected), handle start button (if hero selected and unlocked), handle locked hero click (prompt unlock if enough gold)
4. draw(): BeginTextureMode, ClearBackground, call render::drawTitle, render::drawHeroGrid, render::drawStartButton, render::drawGoldDisplay, EndTextureMode, DrawTexturePro

## Constraints
- Must use `game::` namespace
- `#pragma once` header guard
- Derived from Screen base class with `override` on all virtual methods
- Follow `specs/raylib-cpp/05-coding-style.md`
- Uses Screen base, override on all virtuals, calls render:: functions

## Acceptance Criteria
- [ ] Hero grid displays
- [ ] Selection works
- [ ] Start button launches game
- [ ] Locked hero unlock works

## Notes
( filled in during/after implementation )
