# T026: Unit Tests

## Status: pending
## Phase: 6

## Goal
Comprehensive unit tests for all pure logic modules using doctest.

## Depends On
- T009 (board generation)
- T010 (card effects)
- T011 (combat)
- T012 (items)
- T013 (account)

## Spec References
- `raylib-cpp/04-build-test.md` → "Test Structure" section

## Files to Create/Modify
- `src/test_main.cpp` (NEW)
- `src/logic/test_board_gen.cpp` (NEW)
- `src/logic/test_card_effects.cpp` (NEW)
- `src/logic/test_combat.cpp` (NEW)
- `src/logic/test_items.cpp` (NEW)
- `src/logic/test_account.cpp` (NEW)

## Implementation Steps
1. Create test_main.cpp with DOCTEST_CONFIG_IMPLEMENT
2. Create test_board_gen.cpp: dimensions, first row no monsters, boss placement, chest rows, weighted rates
3. Create test_card_effects.cpp: monster damage, shield, heal, gold, item, place, end
4. Create test_combat.cpp: shield absorption, overflow bleed, invisibility cloak, ring of courage, cursed shield, emerald
5. Create test_items.cpp: monster egg combo, apple, ring of protection, dragon wing, athelas, amethyst
6. Create test_account.cpp: unlock hero, insufficient gold, save/load round-trip
7. Add `make test` target

## Constraints
- Tests compile WITHOUT raylib
- Deterministic seeding (rng(42))
- CHECK not REQUIRE
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] All tests pass
- [ ] `make test` works
- [ ] No raylib dependency in test binary

## Notes
(filled in during/after implementation)
