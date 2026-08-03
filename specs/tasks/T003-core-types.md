# T003: Core Types

## Status: pending
## Phase: 1
## Critical: true

## Goal
Define all core type definitions, enums, components, and entities for the game.

## Depends On
- T001 (Project Setup)

## Spec References
Load these before starting:
- `game/00-overview.md` → "Heroes" section
- `game/02-card-types.md` → "Card Types" section
- `game/10-ui-screens.md` → "State Machine" section
- `raylib-cpp/01-architecture.md` → "C++ Type Signatures" section
- `raylib-cpp/05-coding-style.md` → "Naming Conventions" section

## Files to Create/Modify
- `src/core/constants.h` (already created in T001 — immutable compile-time constants, `SCREAMING_SNAKE` naming)
- `src/core/types.h` (NEW)
- `src/core/components.h` (NEW)
- `src/core/entities.h` (NEW)

## Implementation Steps
1. Verify `constants.h` exists with DESIGN_WIDTH, DESIGN_HEIGHT, INITIAL_WINDOW_WIDTH, INITIAL_WINDOW_HEIGHT, TARGET_FPS, CARD_WIDTH, CARD_HEIGHT, ROW_HEIGHT, SLOTS_PER_ROW
2. Create `types.h` with all enum classes (ScreenID, GameStatus, CardType, Biome, EffectType, Sfx) and data structs (Card, Hero, LevelConfig, CardEffect, Button, HeroSave, Account, GameState)
3. Create `components.h` with HealthData, ShieldData, GoldData, InventoryData, PositionData, AnimState
4. Create `entities.h` with PlayerState, CardSlot, VisualEffect
5. Ensure all code is inside `game::` namespace
6. Verify compilation with stub includes

## Constraints
- Header-only files, no .cpp needed
- `#pragma once` header guard
- `game::` namespace for all game types
- camelCase for functions, PascalCase for types
- `_camelCase` for private members
- Immutable constants use `SCREAMING_SNAKE` (see constants.h)
- Configurable values use `camelCase`

## Acceptance Criteria
- [ ] All headers compile without errors
- [ ] Structs are aggregate-initializable
- [ ] `make` succeeds
- [ ] No naming convention violations

## Notes
- `constants.h` was created during T001 to centralize game-wide constants (960×540 design resolution, etc.)
- See `raylib-cpp/01-architecture.md` → "Core Constants" section for the full list