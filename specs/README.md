# Specs

Spec-driven development for Forward (Game Jam Version) "Drop the Volunteer" — a roguelike card-dungeon crawler.

## Structure

```
specs/
├── game/              # Tech-agnostic game logic (10 specs)
│   ├── 00-overview.md
│   ├── 01-board-generation.md
│   ├── 02-card-types.md
│   ├── 03-item-effects.md
│   ├── 04-combat-system.md
│   ├── 05-healing-shields.md
│   ├── 06-gold-scoring.md
│   ├── 07-turn-flow.md
│   ├── 08-fog-visibility.md
│   └── 09-level-progression.md
│
└── raylib-cpp/              # Raylib/C++ specific implementation (5 specs)
    ├── 00-project-setup.md
    ├── 01-architecture.md
    ├── 02-gameplay-systems.md
    ├── 03-ui-systems.md
    └── 04-build-test.md
```

## How to Use

### For AI Agents

1. **Read `game/` specs** to understand game mechanics
2. **Read `raylib-cpp/` specs** for Raylib/C++ specific implementation
3. **Implement against specs** — each spec has acceptance criteria
4. **Verify** — run ` ` against acceptance criteria

### For Humans

1. Start with `game/00-overview.md` for game architecture
2. Read specific mechanic specs as needed
3. Follow `raylib-cpp/` specs for Raylib/C++ implementation order

## Spec Format

Each spec follows:

```markdown
# spec: <name>

## Goal
What this spec covers.

## Requirements
Numbered list of verifiable requirements.

## Constraints
Pure functions, no I/O, no framework dependencies.

## Pseudocode / Data Definitions
Algorithms and data tables.

## Acceptance Criteria
Checklist of testable conditions.
```

## Relationship to RECREATE Files

The `RECREATE_*.md` files in the parent directory contain detailed implementation plans with code examples. These specs are concise, context-friendly versions optimized for AI agent consumption.
