# T001: Project Setup

## Status: done
## Phase: 0
## Critical: true

## Goal
Set up project structure, Makefile, directory tree, .gitignore, and a stub main.cpp that opens a window.

## Depends On
(None)

## Spec References
Load these before starting:
- `raylib-cpp/00-project-setup.md` → "Project Structure" and "Makefile" sections
- `raylib-cpp/05-coding-style.md` → "General Rules" section

## Files to Create/Modify
- `Makefile` (MODIFY or CREATE)
- `.gitignore` (MODIFY or CREATE)
- Directory structure: `src/core/`, `src/logic/`, `src/render/`, `src/screens/`, `src/data/`, `src/anim/`, `src/input/`, `assets/audio/`, `assets/fonts/`, `assets/img/`, `assets/data/`
- `src/main.cpp` (NEW or MODIFY)

## Implementation Steps
1. Create the directory tree under the project root
2. Write a Makefile with debug, release, run, and clean targets following `raylib-cpp/00-project-setup.md`
3. Create or update `.gitignore` to exclude build artifacts and IDE files
4. Create a stub `src/main.cpp` that initializes a raylib window and runs a basic game loop
5. Verify `make` compiles successfully

## Constraints
- Must use `game::` namespace for all game code
- `#pragma once` header guard for all headers
- Follow naming conventions in `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] `make` compiles without errors
- [x] `make run` opens a window
- [x] `make clean` removes build artifacts
- [x] Directory tree exists: `src/core/`, `src/logic/`, `src/render/`, `src/screens/`, `src/data/`, `src/anim/`, `src/input/`, `assets/audio/`, `assets/fonts/`, `assets/img/`, `assets/data/`
- [x] Makefile outputs to `build/` directory
- [x] `.gitignore` excludes `build/`, `assets/`, `vendor/`, `save.json`
- [x] `src/core/constants.h` created with DESIGN_WIDTH/HEIGHT, INITIAL_WINDOW_WIDTH/HEIGHT, TARGET_FPS, CARD_WIDTH/HEIGHT, ROW_HEIGHT, SLOTS_PER_ROW

## Notes
- `src/game.cpp` was renamed to `src/main.cpp` (per spec)
- Vendor raylib headers are in `vendor/raylib/` (not `vendor/raylib/src/`), so Makefile uses `-I vendor/raylib -L vendor/raylib`
- System link flags: `-lm -lpthread -ldl -lrt -lX11`
- `make debug` sets `-g -O0 -DDEBUG` for release vs debug builds
- `src/core/constants.h` was added to centralize immutable compile-time constants using `SCREAMING_SNAKE` naming