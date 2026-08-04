# T028: Animation System

## Status: pending
## Phase: 6

## Goal
Easing functions, AnimState, and visual effect animations.

## Depends On
- T003 (types)
- T024 (GameScreen)

## Spec References
- `raylib-cpp/04-build-test.md` → "Animation System" section

## Files to Create/Modify
- `src/anim/easing.h` (NEW)
- `src/anim/anim.cpp` (NEW)
- `src/logic/test_easing.cpp` (NEW)

## Implementation Steps
1. Create easing.h with constexpr lerp, easeInOut, easeOut, bounce in `game::anim::`
2. Implement AnimState::start(), update(), value() methods (AnimState defined in components.h)
3. Implement 6 animations: card flip (0.3s, easeOut, scale Y 0→1), damage flash (0.2s, lerp, red tint), heal sparkle (0.5s, easeOut, particles), gold coin (0.4s, bounce, scale up), fog fade (0.5s, easeInOut, alpha 0→255), screen shake (0.3s, easeOut, random offset)
4. Integrate VisualEffect in game_screen
5. Write unit tests in `test_easing.cpp`: lerp, easeInOut, easeOut, bounce functions

## Constraints
- AnimState methods
- constexpr easing
- `game::anim::` namespace

## Acceptance Criteria
- [ ] All 6 animations work smoothly
- [ ] AnimState correctly tracks time

## Notes
(filled in during/after implementation)
