# T032: Test Infrastructure Setup

## Status: done
## Phase: 1

## Goal
Set up the test infrastructure: `make test` target, doctest entry point, and the testing strategy for the entire project.

## Depends On
- T001 (Project Scaffolding)

## Phase:
1

## Critical:
false

## Spec References
- `raylib-cpp/04-build-test.md` → "Test Structure" section
- `raylib-cpp/05-coding-style.md` → coding conventions

## Files to Create/Modify
- `src/test_main.cpp` (NEW) — doctest entry point
- `Makefile` (MODIFY) — add `make test` target

## Implementation Steps
1. Create `src/test_main.cpp` with `DOCTEST_CONFIG_IMPLEMENT`
2. Add `make test` target to Makefile
3. Verify `make test` compiles and runs (even with zero tests)

## Testing Strategy (Project-Wide)

### Principle: Tests live with their tasks

Each logic task creates its own `test_*.cpp` alongside implementation. Testing is not deferred to a single Phase 6 task — it's part of every task.

### Task-to-test mapping

| Task | Test File | What to Test |
|------|-----------|-------------|
| T009 (Board Gen) | `src/logic/test_board_gen.cpp` | Dimensions, first row no monsters, boss placement, chest rows |
| T010 (Items) | `src/logic/test_items.cpp` | All 22 item effects, monster egg combo |
| T011 (Combat) | `src/logic/test_combat.cpp` | Shield absorption, overflow, invisibility cloak, ring, cursed shield |
| T012 (Card Effects) | `src/logic/test_card_effects.cpp` | Monster damage, shield, heal, gold, item, place, end |
| T013 (Account) | `src/logic/test_account.cpp` | Unlock hero, insufficient gold, save/load round-trip |
| T014 (Fog) | `src/logic/test_fog.cpp` | Row visibility toggling |
| T015 (Level Config) | `src/logic/test_level_config.cpp` | Level configs match spec tables |
| T016 (Turn Flow) | `src/logic/test_turn_flow.cpp` | State machine transitions, full turn sequence |
| T028 (Easing) | `src/logic/test_easing.cpp` | lerp, easeInOut, easeOut, bounce |
| T029 (JSON Data) | `src/logic/test_game_data.cpp` | JSON parsing, fallback behavior |

### What does NOT get unit tests

- Rendering tasks (T017-T022): visual verification only
- Resource Manager (T004): raylib Load/Unload calls
- Screen Manager (T006): virtual dispatch, manual verification
- Input Handling (T007): runtime-dependent
- Main Entry (T008): integration, `make run` verification
- Sound (T027): auditory verification

### T026 scope (reduced)

T026 becomes verification + integration only:
- Verify all per-task tests pass together
- Add integration tests (combat + items interaction)
- No longer creates individual unit test files

## Constraints
- Tests compile WITHOUT raylib headers
- doctest framework (system package at `/usr/include/doctest/doctest.h`)
- Deterministic seeding: `std::mt19937 rng(42)`
- Use `CHECK(...)` not `REQUIRE(...)`
- Test binary links `-lm` only, no `-lraylib`

## Acceptance Criteria
- [x] `src/test_main.cpp` created with doctest entry point
- [x] `make test` target exists in Makefile
- [x] `make test` compiles and runs successfully
- [x] Test binary has no raylib dependency

## Notes
- doctest installed system-wide at `/usr/include/doctest/doctest.h`
- Test target builds to `build/run_tests`
- Empty test suite is valid (passes with 0 tests)
- `TEST_LOGIC_SRC` variable in Makefile ready for future test files
- All acceptance criteria verified: `make test` passes, no raylib dependency
