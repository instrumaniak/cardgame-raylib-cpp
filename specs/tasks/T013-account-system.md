# T013: Account System

## Status: pending
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
6. Default hero table: headless-knight (free), forest-warden (100), cave-shadow (200), desert-nomad (300)
7. Write unit tests in `test_account.cpp`: unlock hero, insufficient gold, save/load round-trip

## Constraints
- Must use `game::logic::` namespace
- `#pragma once` header guard
- No raylib includes in logic layer
- Follow `specs/raylib-cpp/05-coding-style.md`
- Uses nlohmann/json for serialization

## Acceptance Criteria
- [ ] Account save/load round-trips correctly
- [ ] Hero unlock deducts gold
- [ ] Insufficient gold fails gracefully
- [ ] Default hero table is correct
- [ ] `make test` passes

## Notes
( filled in during/after implementation )
