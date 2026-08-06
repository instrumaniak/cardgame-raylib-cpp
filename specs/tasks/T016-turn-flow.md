# T016: Turn Flow Orchestrator

## Status: done
## Phase: 3
## Critical: true

## Goal: Complete turn sequence that orchestrates all game logic modules into a playable game loop.

## Depends On
- T009 (board generation)
- T010 (item system)
- T011 (combat system)
- T012 (card effects)
- T013 (account system)
- T014 (fog visibility)
- T015 (level config)

## Spec References
Load these before starting:
- `game/07-turn-flow.md` — Complete turn sequence, status transitions, all sub-operations
- `raylib-cpp/01-architecture.md` → "Game Status Sub-States" section
- `raylib-cpp/05-coding-style.md` — Naming and style conventions

## Files to Create/Modify
- `src/logic/turn_flow.h` (NEW)
- `src/logic/turn_flow.cpp` (NEW)
- `src/logic/test_turn_flow.cpp` (NEW)

## Implementation Steps
1. Create turn_flow.h with turn flow functions in `game::logic::` namespace
2. Implement playCard(gameState, row, col) — main card interaction entry point
3. Implement checkMonsterPlay(gameState) — passive monster attack from row below
4. Implement nextTurn(gameState) — advance to next state after card resolution
5. Implement die(gameState) — handle hero death, check athelas, transition to Lose
6. Wire up complete status machine: NotStarted → NotReady → Ready → HeroDrop → MonsterAttack → Ready
7. Handle special transitions: HeroDrop → Win (end card), HeroDrop → Lose (death)
8. Handle chest sub-location entry/exit
9. Handle biome card transitions (new level)
10. Handle carrot effect (+1 HP on position change)
11. Write unit tests in `test_turn_flow.cpp`: state machine transitions, full turn sequence, integration tests

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- This is pure logic — no rendering, no I/O

## Acceptance Criteria
- [x] Full turn sequence works end-to-end in pure logic
- [x] Status transitions match game/07-turn-flow.md exactly
- [x] Monster passive attack triggers correctly from row below
- [x] Athelas prevents death once and removes from bag
- [x] Chest sub-location entry/exit works
- [x] Biome card transitions to new level
- [x] Death transitions to Lose status
- [x] End card transitions to Win status
- [x] `make test` passes with integration tests

## Notes
- Added `consumed` field to `CardSlot` in core/entities.h to track played cards
- Added public `hasItem` and `removeFirstItem` utilities to items.h (refactored from static in card_effects.cpp)
- Added `spawnLoot` to board_gen for monster-kill loot generation
- Ring-of-courage is handled inside `applyDamage` (combat layer), not duplicated in turn_flow
- `resolveMonster` calls `applyAthelas` internally; `nextTurn` has a safety-net check for non-combat deaths
