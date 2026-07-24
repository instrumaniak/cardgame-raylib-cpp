# T019: Hero Panel

## Status: pending
## Phase: 4

## Goal
Left-side panel showing hero state (art, HP, shield, gold, inventory).

## Depends On
- T003 (Core Types)
- T004 (Resource Manager)

## Spec References
- `raylib-cpp/03-ui-systems.md` → "Hero Panel" section

## Files to Create/Modify
- `src/render/hero_panel.h` (NEW)
- `src/render/hero_panel.cpp` (NEW)

## Implementation Steps
1. Create `hero_panel.h` with `drawHeroPanel()` declaration in `game::render::`
2. Draw hero card art from spritesheet region
3. Draw hero name
4. Draw health bar: dark gray background, fill width = (current/max) * barWidth, color: green > 60%, yellow > 30%, red ≤ 30%, current/max text
5. Draw shield indicator (blue bar or icon + value)
6. Draw coin icon + gold amount
7. Draw inventory: horizontal scroll of item icons (32x32)

## Constraints
- Must use `game::render::` namespace
- `#pragma once` header guard
- All draw functions take `const ResourceManager& res` as first param
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] Health bar color-coded correctly
- [ ] Shield displayed
- [ ] Gold shown
- [ ] Inventory icons visible

## Notes
( filled in during/after implementation )
