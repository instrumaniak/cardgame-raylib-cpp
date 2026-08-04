# T026: Unit Tests

## Status: pending
## Phase: 6

## Goal
Verify all per-task tests pass together. Add integration tests (combat + items interaction).

## Depends On
- T009 (board generation)
- T010 (item system)
- T011 (combat)
- T012 (card effects)
- T013 (account)
- T014 (fog visibility)
- T015 (level config)
- T016 (turn flow)
- T028 (animation)
- T029 (JSON data)

## Spec References
- `raylib-cpp/04-build-test.md` → "Test Structure" section

## Files to Create/Modify
- `src/logic/test_integration.cpp` (NEW) — combat + items interaction tests

## Implementation Steps
1. Verify all per-task tests pass together (`make test`)
2. Create test_integration.cpp: combat + items interaction tests (e.g., cursed shield + damage calc, athelas + lethal damage, ring-of-courage + 1 HP trigger)
3. Verify full test suite passes

## Constraints
- Tests compile WITHOUT raylib
- Deterministic seeding (rng(42))
- CHECK not REQUIRE
- Follow `specs/raylib-cpp/05-coding-style.md`
- Individual unit tests live in their respective task files (T009–T016, T028, T029)

## Acceptance Criteria
- [ ] All per-task tests pass together
- [ ] Integration tests cover combat + items interaction
- [ ] `make test` works with full suite

## Notes
- T032 established the testing strategy: each logic task owns its own test file
- This task is verification + integration only, not individual unit test creation
- Test files are created by: T009 (test_board_gen), T010 (test_items), T011 (test_combat), T012 (test_card_effects), T013 (test_account), T014 (test_fog), T015 (test_level_config), T016 (test_turn_flow), T028 (test_easing), T029 (test_game_data)
