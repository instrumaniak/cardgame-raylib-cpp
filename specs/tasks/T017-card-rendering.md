# T017: Card Rendering

## Status: done
## Phase: 4

## Goal
Draw any card type at any position with correct frame, sprite, and text.

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "Card Rendering" section
- `game/02-card-types.md` → frame colors

## Files to Create/Modify
- `src/render/card_draw.h` (NEW)
- `src/render/card_draw.cpp` (NEW)

## Implementation Steps
1. Create `card_draw.h` with `drawCard()` and `drawCardBack()` declarations in `game::render::`
2. Implement `drawCard()`: background fill by `CardType` color, card frame PNG overlay, card sprite region from `cards.png`, name/value text if revealed
3. Implement `drawCardBack()`: `card-back.png` overlay
4. Frame color mapping: Monster=purple, Heal=red, Shield=gray, Gold=yellow, Item/Drop/Biome=hero-frame, Place=chest-frame, EndCard=light-frame

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Can draw all card types
- [x] Frame colors match spec
- [x] Face-up shows name + value
- [x] Face-down shows card-back

## Notes
- Implemented `drawCard()` and `drawCardBack()` in `game::render::` namespace
- Background colors match spec: Monster=#53428d, Heal=#d94743, Shield=#616161, Gold=#fbc02d, Item/Drop/Biome=dark, Place=#f3d19f, EndCard=gold glow
- Frame textures: card-frame-purple/red/gray/yellow, hero-frame, chest-frame, light-frame
- Sprite rendered centered within card with 8px margin for frame overlay
- Text uses default raylib font (can be refined with specific font later)
