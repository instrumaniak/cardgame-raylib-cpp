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
- `src/core/types.h` (NEW)
- `src/core/components.h` (NEW)
- `src/core/entities.h` (NEW)

## Implementation Steps
1. Create `types.h` with all enum classes (ScreenID, GameStatus, CardType, Biome, EffectType, Sfx) and data structs (Card, Hero, LevelConfig, CardEffect, Button, HeroSave, Account, GameState)
2. Create `components.h` with HealthData, ShieldData, GoldData, InventoryData, PositionData, AnimState
3. Create `entities.h` with PlayerState, CardSlot, VisualEffect
4. Ensure all code is inside `game::` namespace
5. Verify compilation with stub includes

## Constraints
- Header-only files, no .cpp needed
- `#pragma once` header guard
- `game::` namespace for all game types
- camelCase for functions, PascalCase for types
- `_camelCase` for private members

## Acceptance Criteria
- [ ] All headers compile without errors
- [ ] Structs are aggregate-initializable
- [ ] `make` succeeds
- [ ] No naming convention violations

## Notes
( filled in during/after implementation )