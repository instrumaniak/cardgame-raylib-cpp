# T006: Screen Manager

## Status: done
## Phase: 1

## Goal
Screen base class with virtual lifecycle and ScreenManager for state transitions.

## Depends On
- T001 (Project Scaffolding)

## Spec References
Load these before starting:
- `raylib-cpp/01-architecture.md` → "Screen Manager" section
- `game/10-ui-screens.md`
- `raylib-cpp/05-coding-style.md` → "Coding Style" section

## Files to Create/Modify
- `src/core/screen.h` (NEW)
- `src/core/screen.cpp` (NEW)

## Implementation Steps
1. Create `screen.h` with `Screen` abstract base class in `game::` namespace
2. Virtual methods: `id()`, `onEnter()`, `onExit()`, `update(dt)`, `draw()` with `override` on derived
3. `ScreenManager` class with `_screens` array of `unique_ptr<Screen>`, `_current` and `_next` raw pointers
4. Methods: `init()`, `switchTo(ScreenID)`, `update(dt)`, `draw()`, `currentId()`
5. Implement transition logic: `switchTo` sets `_next`, `update` checks `_current != _next` and calls `onExit`/`onEnter`

## Constraints
- `game::` namespace
- `std::unique_ptr` for ownership
- Raw ptr for observers
- Virtual destructor
- `override` on derived

## Acceptance Criteria
- [x] Can switch between 2 placeholder screens
- [x] Transitions call `onExit` / `onEnter` correctly
- [x] `make` succeeds

## Notes
- Added `game::Screen` and `game::ScreenManager` in `src/core/screen.h/.cpp`.
- `ScreenManager::init()` installs placeholder screens for all four `ScreenID` values so later screen tasks can replace them.
- Verified with `make` and `make test`.
