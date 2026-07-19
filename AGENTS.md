# AGENTS.md — Drop the Volunteer

## Project Overview
Roguelike card-dungeon crawler built with C++17 and raylib.
Player drops a hero through a grid of cards, fighting monsters,
collecting items, and reaching the end card to win.

## Build Commands
- `make` — Release build (C++17, -O2)
- `make debug` — Debug build (-g -O0 -DDEBUG)
- `make run` — Build and run
- `make test` — Run unit tests (doctest)
- `make clean` — Remove build artifacts

## Code Style
Follow `specs/raylib-cpp/05-coding-style.md`. Key rules:
- Functions: `camelCase`
- Types/enums: `PascalCase`
- Private members: `_camelCase` (leading underscore)
- 2-space indent, K&R braces
- `#pragma once` for headers
- No exceptions, no RTTI, no raw new/delete
- `enum class` always, PascalCase values

## Namespaces
- `game::` — core types, enums, components, entities
- `game::logic::` — pure game logic (no raylib)
- `game::render::` — drawing functions
- `game::anim::` — easing and animation
- `game::data::` — JSON deserialization

## Directory Structure
```
src/
├── core/       # ScreenManager, ResourceManager, Layout, types
├── screens/    # Screen implementations
├── logic/      # Pure game logic (zero raylib)
├── render/     # Drawing functions (zero logic)
├── data/       # JSON deserialization
├── anim/       # Easing and animation
└── input/      # Input abstraction
```

## Task System
Tasks live in `specs/tasks/`. Each task file (T001-T030) defines:
- Goal, dependencies, spec references
- Files to create/modify
- Implementation steps
- Acceptance criteria

### Working on a Task
1. Read the task file fully
2. Load referenced spec sections
3. Implement following code style
4. Verify acceptance criteria (`make test`)
5. Mark status as `done` in the task file

### Task Status Values
- `pending` — not started
- `in_progress` — actively working
- `done` — completed and verified

## Specs
- `specs/game/` — Tech-agnostic game logic (10 specs)
- `specs/raylib-cpp/` — C++ implementation specs (6 specs)
- `specs/tasks/` — Implementation task breakdown (30 tasks)

## Testing
- Unit tests in `src/logic/test_*.cpp`
- Tests compile WITHOUT raylib (pure logic only)
- Use doctest framework
- Deterministic seeding: `std::mt19937 rng(42)`
- Use `CHECK(...)` not `REQUIRE(...)`

## Key Conventions
- All structs use composition (has-a), not inheritance
- Virtual dispatch only on Screen hierarchy (4 classes)
- ResourceManager owns all loaded resources (const ref access)
- GameState passed by mutable reference
- Logic layer has zero raylib includes
- Render layer has zero game logic
