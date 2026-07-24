# T002: Asset Acquisition

## Status: pending
## Phase: 0

## Goal
Obtain and place all game assets (sprites, fonts, audio) in the correct directories.

## Depends On
- T001 (Project Setup)

## Spec References
Load these before starting:
- `raylib-cpp/00-project-setup.md` → "Asset Organization" section
- `game/02-card-types.md` → "Card Sprites" section
- `game/00-overview.md` → "Heroes and Assets" section

## Files to Create/Modify
- `assets/img/cards.png` (NEW)
- `assets/img/icons.png` (NEW)
- `assets/img/card-back.png` (NEW)
- `assets/img/card-frame-purple.png` (NEW)
- `assets/img/card-frame-red.png` (NEW)
- `assets/img/card-frame-gray.png` (NEW)
- `assets/img/card-frame-yellow.png` (NEW)
- `assets/img/hero-frame.png` (NEW)
- `assets/img/chest-frame.png` (NEW)
- `assets/img/light-frame.png` (NEW)
- `assets/fonts/pearsoda.ttf` (NEW)
- `assets/fonts/zepto.ttf` (NEW)
- `assets/audio/*.wav` (8 files, NEW)

## Implementation Steps
1. Obtain or create the card sprite atlas (`cards.png`) with all card types
2. Obtain or create frame overlays for cards, heroes, chests, and lights
3. Obtain or create hero sprites (if not included in atlas)
4. Download the required fonts (`pearsoda.ttf`, `zepto.ttf`)
5. Download or generate 8 sound effect WAV files for game events
6. Verify all files exist in the correct paths

## Constraints
- Must use `game::` namespace for all game code
- `#pragma once` header guard for all headers
- Follow naming conventions in `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] All referenced asset files exist in correct paths
- [ ] Image files are valid PNG format
- [ ] Font files are valid TTF format
- [ ] Audio files are valid WAV format
- [ ] No missing assets referenced in code

## Notes
( filled in during/after implementation )