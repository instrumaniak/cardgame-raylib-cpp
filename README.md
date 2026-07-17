# Fordard Game Jam Version — Raylib Recreation For Study

A roguelike card-dungeon crawler.

## Overview

"Forward" (Game Jam version) is a roguelike card-dungeon crawler. The player drops a hero card into an abyss and navigates through rows of face-down cards, flipping them to reveal monsters, shields, healing, gold, or special items. The goal is to survive and reach the bottom.

This project recreates the original game jam version using Raylib & C++.

## Quick Start

```bash

```

## Project Structure

```
cardgame-study-bevy/
├── specs/              # Game and implementation specs
├── assets/             # Game assets (images, audio, fonts, data)
├── src/                # C++ source code
├── tests/              # Integration tests
├── vendor/             # libraries (raylib)
└── Makefile
```

## Specs

See `specs/README.md` for the spec-driven development workflow.

- **Game logic**: `specs/game/` — tech-agnostic game mechanics
- **Raylib/C++ implementation**: `specs/raylib-cpp/` — Raylib/C++ specific architecture and systems

## Vendor

```
$ tree vendor
vendor
└── raylib
    ├── libraylib.a
    ├── raylib.h
    ├── raymath.h
    ├── rcamera.h
    ├── rgestures.h
    └── rlgl.h

1 directory, 6 files

```

## Target Platforms

| Platform | Build | Status |
|----------|-------|--------|
| Linux Desktop | `make run` | Primary |
| Web (WASM) | ` ` | Supported |
| Android | ` ` | Later |

## Credits

- Original game: https://html-classic.itch.zone/html/1314748/index.html
- Assets included locally in `assets/`
