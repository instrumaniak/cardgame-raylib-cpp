# Task Execution Order

"Drop the Volunteer" — Roguelike Card Dungeon Crawler

## How to Use

1. Pick the next uncompleted task from the current phase
2. Read the task file fully before starting
3. Load the referenced spec sections
4. Implement, test, verify acceptance criteria
5. Mark status as `done` and move to the next task

## Phase 0: Scaffolding

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T001](T001-project-setup.md) | Project structure & Makefile | — | pending |
| [T002](T002-asset-acquisition.md) | Asset acquisition & placement | T001 | pending |

## Phase 1: Foundation Types

All tasks in this phase can run in parallel after T001 completes.

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T003](T003-core-types.md) | Core types, enums, components, entities | T001 | pending |
| [T004](T004-resource-manager.md) | ResourceManager | T001, T002 | pending |
| [T005](T005-layout-system.md) | Layout system | T001 | pending |
| [T006](T006-screen-manager.md) | Screen base + ScreenManager | T001 | pending |
| [T007](T007-input-handling.md) | Input handling | T005 | pending |
| [T008](T008-main-entry.md) | main.cpp entry point | T003-T007 | pending |

## Phase 2: Pure Logic

All tasks in this phase can run in parallel after T003 completes (T011 depends on T010).

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T009](T009-board-generation.md) | Board generation | T003 | pending |
| [T010](T010-item-system.md) | Item system & modifiers | T003 | pending |
| [T011](T011-combat-system.md) | Combat system | T003, T010 | pending |
| [T012](T012-card-effects.md) | Card effects (heal/shield/gold) | T003, T010, T011 | pending |
| [T013](T013-account-system.md) | Account system | T003 | pending |
| [T014](T014-fog-visibility.md) | Fog visibility logic | T010 | pending |
| [T015](T015-level-config.md) | Level config helpers | T003 | pending |

## Phase 3: Integration

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T016](T016-turn-flow.md) | Turn flow orchestrator | T009-T015 | pending |

## Phase 4: Rendering

All tasks in this phase can run in parallel after T003 + T004 complete.

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T017](T017-card-rendering.md) | Card rendering | T003, T004 | pending |
| [T018](T018-board-rendering.md) | Board rendering + fog | T017, T014 | pending |
| [T019](T019-hero-panel.md) | Hero panel rendering | T003, T004 | pending |
| [T020](T020-hud-rendering.md) | HUD rendering | T003, T004 | pending |
| [T021](T021-menu-rendering.md) | Menu rendering | T003, T004 | pending |
| [T022](T022-win-lose-overlays.md) | Win/Lose overlays | T003, T004 | pending |

## Phase 5: Screens

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T023](T023-home-screen.md) | HomeScreen | T006, T013, T021 | pending |
| [T024](T024-game-screen.md) | GameScreen | T006, T009, T016, T018-T020 | pending |
| [T025](T025-win-lose-screens.md) | WinScreen / LoseScreen | T006, T013, T022 | pending |

## Phase 6: Polish

| Task | Name | Depends On | Status |
|------|------|------------|--------|
| [T026](T026-unit-tests.md) | Unit tests | T009-T013 | pending |
| [T027](T027-sound-integration.md) | Sound integration | T004, T024 | pending |
| [T028](T028-animation-system.md) | Animation system | T003, T024 | pending |
| [T029](T029-json-data-loading.md) | JSON data loading | T003, T002 | pending |
| [T030](T030-final-polish.md) | Final build & polish | All | pending |

## Critical Path

The fastest path to a playable game:

```
T001 → T003 → T010 → T011 → T012 → T016 → T024
```

## Parallelization Groups

After T001 + T003 complete, these groups can run simultaneously:

- **Group A** (Logic): T009, T010, T013, T015
- **Group B** (Rendering): T017, T019, T020, T021, T022
- **Group C** (depends on A/B): T011, T012, T014, T018
