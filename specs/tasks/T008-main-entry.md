# T008: Main Entry

## Status: done
## Phase: 1

## Goal
`main.cpp` that wires together all core systems.

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)
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
- [x] Minimized or otherwise 0x0 windows skip layout recompute and draw safely
- [ ] `make run` works
- [x] `make` succeeds

## Notes
- `main.cpp` now initializes the window, audio device, resource manager, layout, and screen manager.
- The main loop handles global input and updates screens each frame, then computes layout and composites the render texture with letterbox scaling when the window has a non-zero size.
- Minimized or otherwise 0x0 windows now skip the layout recompute and draw path to avoid invalid letterbox math.
