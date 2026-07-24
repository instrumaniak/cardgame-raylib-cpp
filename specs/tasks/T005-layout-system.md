# T005: Layout System

## Status: pending
## Phase: 1

## Goal
Layout struct that handles letterbox scaling from 960x540 design resolution.

## Depends On
- T001 (Project Scaffolding)

## Spec References
Load these before starting:
- `raylib-cpp/01-architecture.md` → "Layout System" section
- `raylib-cpp/05-coding-style.md` → "Coding Style" section

## Files to Create/Modify
- `src/core/layout.h` (NEW)
- `src/core/layout.cpp` (NEW)

## Implementation Steps
1. Create `layout.h` with `Layout` struct in `game::` namespace
2. Members: `renderTexture` (RenderTexture), `dstRect` (Rectangle), `scaleX`, `scaleY`
3. Methods: `init()`, `compute(windowWidth, windowHeight)`, `virtualToScreen(Vector2)`, `screenToVirtual(Vector2)` all const-correct
4. Implement `init()` to create 960x540 RenderTexture
5. Implement `compute()` for letterbox calculation
6. Implement coordinate conversion methods

## Constraints
- Uses raylib types (`RenderTexture`, `Rectangle`, `Vector2`)
- `game::` namespace
- Const methods where appropriate

## Acceptance Criteria
- [ ] `compute(1280, 720)` produces correct `dstRect`
- [ ] `virtualToScreen` / `screenToVirtual` round-trip correctly
- [ ] `make` succeeds

## Notes
( filled in during/after implementation )
