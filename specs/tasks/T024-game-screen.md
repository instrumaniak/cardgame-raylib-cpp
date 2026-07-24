# T024: Game Screen

## Status: pending
## Phase: 5
## Critical: true

## Goal
GameScreen that orchestrates the full gameplay loop.

## Depends On
- T006 (ScreenManager)
- T009 (board gen)
- T016 (turn flow)
- T018 (board render)
- T019 (hero panel)
- T020 (hud)

## Spec References
- `raylib-cpp/01-architecture.md` → "Screen Lifecycle" table (Game row)
- `game/07-turn-flow.md`
- `game/10-ui-screens.md`

## Files to Create/Modify
- `src/screens/game_screen.h` (NEW)
- `src/screens/game_screen.cpp` (NEW)

## Implementation Steps
1. Create GameScreen class derived from Screen in `game::`
2. GameState member holding all runtime state
3. onEnter(): generate board, init player state, set status to NotStarted
4. update(dt): GameStatus state machine step — handle drag/drop input, call turnFlow functions, check win/lose conditions
5. draw(): BeginTextureMode, ClearBackground, call render::drawBoard, render::drawHeroPanel, render::drawHud, if win/lose call overlay, EndTextureMode, DrawTexturePro

## Constraints
- Must use `game::` namespace
- `#pragma once` header guard
- Derived from Screen base class with `override` on all virtual methods
- Follow `specs/raylib-cpp/05-coding-style.md`
- Uses Screen base, integrates all logic and render modules

## Acceptance Criteria
- [ ] Full gameplay loop works
- [ ] Status transitions correct
- [ ] Board renders with fog
- [ ] Hero panel shows state

## Notes
( filled in during/after implementation )
