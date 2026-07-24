# T017: Card Rendering

## Status: pending
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
- [ ] Can draw all card types
- [ ] Frame colors match spec
- [ ] Face-up shows name + value
- [ ] Face-down shows card-back

## Notes
( filled in during/after implementation )
