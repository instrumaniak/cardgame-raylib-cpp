# T025: Win / Lose Screens

## Status: pending
## Phase: 5

## Goal
WinScreen and LoseScreen that show results and handle play again / main menu.

## Depends On
- T006 (ScreenManager)
- T013 (account)
- T022 (win/lose overlays)

## Spec References
- `raylib-cpp/01-architecture.md` → "Screen Lifecycle" table (Win/Lose rows)

## Files to Create/Modify
- `src/screens/win_screen.h` (NEW)
- `src/screens/win_screen.cpp` (NEW)
- `src/screens/lose_screen.h` (NEW)
- `src/screens/lose_screen.cpp` (NEW)

## Implementation Steps
1. Create WinScreen and LoseScreen classes derived from Screen in `game::`
2. WinScreen::onEnter(): calculate full gold reward, save account
3. LoseScreen::onEnter(): calculate half gold reward, save account
4. update(dt): handle "Play Again" button (switchTo Game), handle "Main Menu" button (switchTo Home)
5. draw(): BeginTextureMode, call render::drawWinOverlay/drawLoseOverlay, EndTextureMode, DrawTexturePro

## Constraints
- Must use `game::` namespace
- `#pragma once` header guard
- Derived from Screen base class with `override` on all virtual methods
- Follow `specs/raylib-cpp/05-coding-style.md`
- Uses Screen base, saves account on enter

## Acceptance Criteria
- [ ] Win shows full gold
- [ ] Lose shows half gold
- [ ] Play again starts new game
- [ ] Main menu returns home

## Notes
( filled in during/after implementation )
