# T012: Card Effect Resolution

## Status: pending
## Phase: 2
## Critical: true

## Goal
Card effect resolution for all card types (monster, shield, heal, gold, item, drop, place, biome, end).

## Depends On
- T003 (Core Types)
- T010 (Item System)
- T011 (Combat System)

## Spec References
Load these before starting:
- `game/04-combat-system.md` → combat mechanics
- `game/05-healing-shields.md` → healing and shield modifiers
- `game/06-gold-scoring.md` → gold and scoring rules
- `raylib-cpp/02-gameplay-systems.md` → "Card Effects" section

## Files to Create/Modify
- `src/logic/card_effects.h` (NEW)
- `src/logic/card_effects.cpp` (NEW)

## Implementation Steps
1. Create `card_effects.h` with `resolveCard()` dispatcher and individual resolvers in `game::logic::`
2. Implement `resolveCard()` that dispatches by `CardType`
3. Implement `resolveMonster()` using combat system
4. Implement `resolveShield()` with ring-of-vitality and cursed-shield modifiers
5. Implement `resolveHeal()` with heal modifiers capped at max HP
6. Implement `resolveGold()` with ruby 25% HP, amethyst double+consume, ring-of-greed -1 HP
7. Implement `resolveItem()`, `resolveDrop()`, `resolvePlace()`, `resolveBiome()`, `resolveEnd()`

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- Depends on combat.h and items.h

## Acceptance Criteria
- [ ] Monster deals correct damage
- [ ] Shield adds shield value
- [ ] Heal restores HP capped at max
- [ ] Gold adds correct amount
- [ ] All card types resolve correctly
- [ ] `make test` passes

## Notes
( filled in during/after implementation )
