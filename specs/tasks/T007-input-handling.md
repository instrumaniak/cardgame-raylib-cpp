# T007: Input Handling

## Status: done
## Phase: 1

## Goal
Virtual mouse coordinate conversion and fullscreen toggle.

## Depends On
- T005 (Layout System)

## Spec References
Load these before starting:
- `raylib-cpp/01-architecture.md` → "Input" section
- `raylib-cpp/05-coding-style.md` → "Coding Style" section

## Files to Create/Modify
- `src/input/input.h` (NEW)
- `src/input/input.cpp` (NEW)

## Implementation Steps
1. Create `input.h` with free functions in `game::` namespace
2. `virtualMousePosition()` that converts screen coords to 960x540 design space
3. `handleGlobalInput()` for F11/Alt+Enter fullscreen toggle
4. Implement using Layout's `screenToVirtual`
5. Verify mouse coords map correctly

## Constraints
- Free functions in `game::` namespace
- Uses raylib input functions
- Depends on `Layout` for coord conversion

## Acceptance Criteria
- [x] Mouse coords correctly mapped to design resolution
- [x] F11 toggles fullscreen
- [x] `make` succeeds

## Notes
- Added `game::virtualMousePosition(const Layout&)` and `game::handleGlobalInput()`.
- Input module is wired into the build and compiles cleanly.
- Verified with `make` and `make format-check`.
