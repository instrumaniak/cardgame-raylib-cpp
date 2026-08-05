# T011: Combat System

## Status: done
## Phase: 2
## Critical: true

## Goal
Combat damage calculation with shield absorption and item modifiers.

## Depends On
- T003 (Core Types)
- T010 (Item System)

## Spec References
Load these before starting:
- `game/04-combat-system.md` → relevant section
- `raylib-cpp/02-gameplay-systems.md` → "Combat" section

## Files to Create/Modify
- `src/logic/combat.h` (NEW)
- `src/logic/combat.cpp` (NEW)
- `src/logic/test_combat.cpp` (NEW)

## Implementation Steps
1. Create `combat.h` with `CombatResult` struct and combat functions in `game::logic::`
2. Implement `applyDamage()` with shield absorption and overflow bleed
3. Implement `applyToShield()` for shield damage
4. Implement `checkInvisibilityCloak()` (dodge on even HP)
5. Implement `calcMaxHp()` with item modifiers
6. Handle dragon-wing (shield after fight if was 0), emerald (+4 gold per monster hit), ring-of-courage (+5 shield at 1 HP)
7. Write unit tests in `test_combat.cpp`: shield absorption, overflow, invisibility cloak, ring of courage, cursed shield, emerald

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Depends on `items.h` for modifier functions
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Shield absorbs damage correctly
- [x] Overflow bleeds through
- [x] Invisibility cloak dodges on even HP
- [x] Ring-of-courage gives shield at 1 HP
- [x] `make test` passes
- [x] No warnings with `-Wall -Wextra`

## Notes
- Implemented `applyDamage()` with full item modifier support (monster-skull, dragon-wing, emerald, ring-of-courage)
- Implemented `applyToShield()` for shield damage absorption
- Implemented `checkInvisibilityCloak()` for dodge on even HP
- Implemented `calcMaxHp()` using existing `getHeroLifeMaxModifier()` from items.h
- All 35 test cases pass (704 assertions)
- Format check passes