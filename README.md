# Fordard Game Jam Version — Raylib Recreation For Study

A roguelike card-dungeon crawler.

## Overview

"Forward" (Game Jam version) is a roguelike card-dungeon crawler. The player drops a hero card into an abyss and navigates through rows of face-down cards, flipping them to reveal monsters, shields, healing, gold, or special items. The goal is to survive and reach the bottom.

This project recreates the original game jam version using Raylib & C++.

## Quick Start

```bash
make run
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

## Assets

Currently not included.

```
$ tree assets

assets
├── audio
│   ├── chest.mp3
│   ├── coins.mp3
│   ├── deal-card.mp3
│   ├── equip.mp3
│   ├── hit.mp3
│   ├── lose.mp3
│   ├── shield.mp3
│   └── win.mp3
├── data
│   ├── biomes.json
│   ├── cards.json
│   ├── heroes.json
│   ├── items.json
│   └── levels.json
├── fonts
│   ├── PearSoda.ttf
│   └── Zepto-Regular.ttf
└── img
    ├── biome-frame.png
    ├── card-back.png
    ├── card-bg-deco-no-value-dark.png
    ├── card-bg-deco-no-value.png
    ├── card-bg-deco.png
    ├── card-frame-brown.png
    ├── card-frame-gray.png
    ├── card-frame-orange.png
    ├── card-frame-purple.png
    ├── card-frame-red.png
    ├── card-frame-yellow.png
    ├── cards.png
    ├── chest-frame.png
    ├── click.png
    ├── hero-frame.png
    ├── hit.png
    ├── icons.png
    ├── light-frame.png
    ├── pointer.png
    └── shards.png

4 directories, 35 files

```

## Vendor

Not included. Download raylib and copy the required files.

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

- Original game: https://twotinydice.itch.io/forward ( https://html-classic.itch.zone/html/1314748/index.html )
- Recreation For Study purposes only
