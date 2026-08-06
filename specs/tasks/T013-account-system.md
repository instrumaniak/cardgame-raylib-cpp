# T013: Account System

## Status: done
## Phase: 2

## Goal
Account save/load, hero unlock, and gold economy.

## Depends On
- T003 (Core Types)

## Spec References
Load these before starting:
- `game/00-overview.md` → hero table, save schema
- `raylib-cpp/02-gameplay-systems.md` → "Account" section

## Files to Create/Modify
- `src/logic/account.h` (NEW)
- `src/logic/account.cpp` (NEW)
- `src/logic/test_account.cpp` (NEW)

## Implementation Steps
1. Create `account.h` with Account methods and helper functions in `game::logic::`
2. Implement `Account::load()` from JSON using nlohmann/json
3. Implement `Account::save()` to JSON
4. Implement `Account::unlockHero()` (deduct gold, set locked=false)
5. Implement `Account::spendGold()` and `addGold()`
6. Default hero table (from original game): headless-knight (free, shield 0, bag: map+apple), thief (50g, shield 0, bag: invisibility-cloak+monster-eye), harpy (100g, shield 0, bag: ring-of-vitality), warrior (200g, shield 10, bag: shiny-shield+emerald)
7. Write unit tests in `test_account.cpp`: unlock hero, insufficient gold, save/load round-trip

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- Uses nlohmann/json for serialization

## Acceptance Criteria
- [x] Account save/load round-trips correctly
- [x] Hero unlock deducts gold
- [x] Insufficient gold fails gracefully
- [x] Default hero table is correct
- [x] `make test` passes

## Notes
- Fixed inconsistency: T013 originally listed wrong hero names (forest-warden, cave-shadow, desert-nomad). Replaced with correct original game heroes (thief, harpy, warrior).
- Added missing `monster-eye` slug to `slugs.h` and `invisibility-cloak`/`monster-eye` to `03-item-effects.md`.
- Added `shield` field to `Hero` struct in `types.h` (warrior has shield=10).
- Fixed bug: `getNbRowsHidden()` in `items.cpp` was checking `MonsterEgg` instead of `MonsterEye`.
- Extra 4 heroes (dark-knight, mage, spirit, anubis) kept in `slugs.h` for future expansion but not included in default account.
- nlohmann/json used for serialization (header-only, system-wide install).
