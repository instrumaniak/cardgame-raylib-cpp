# T001: Project Setup

## Status: pending
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
- [ ] `make` compiles without errors
- [ ] `make run` opens a window
- [ ] `make clean` removes build artifacts
- [ ] Directory tree exists: `src/core/`, `src/logic/`, `src/render/`, `src/screens/`, `src/data/`, `src/anim/`, `src/input/`, `assets/audio/`, `assets/fonts/`, `assets/img/`, `assets/data/`

## Notes
( filled in during/after implementation )