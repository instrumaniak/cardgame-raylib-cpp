# T008: Main Entry

## Status: pending

## Goal
`main.cpp` that wires together all core systems.

## Depends On
- T003 (Resource Manager)
- T004 (Asset Loading)
- T005 (Layout System)
- T006 (Screen Manager)
- T007 (Input Handling)

## Spec References
Load these before starting:
- `raylib-cpp/01-architecture.md` → "main.cpp Structure" section
- `raylib-cpp/05-coding-style.md` → "Coding Style" section

## Files to Create/Modify
- `src/main.cpp` (MODIFY - replace stub)

## Implementation Steps
1. Include core headers
2. `InitWindow` with `FLAG_WINDOW_RESIZABLE`
3. Create `ResourceManager`, call `loadAll()`
4. Create `Layout`, call `init()`
5. Create `ScreenManager`, call `init()`, `switchTo(ScreenID::Home)`
6. Main loop: `GetFrameTime`, `handleGlobalInput`, `layout.compute`, `screens.update`, `screens.draw`
7. Cleanup: `res.unloadAll()`, `CloseWindow()`

## Constraints
- Thin orchestration only
- No game logic in main
- `game::` namespace usage

## Acceptance Criteria
- [ ] Window opens
- [ ] Screen manager runs
- [ ] Letterbox scales correctly
- [ ] `make run` works
- [ ] `make` succeeds

## Notes
( filled in during/after implementation )
