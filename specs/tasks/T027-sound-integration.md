# T027: Sound Integration

## Status: pending
## Phase: 6

## Goal
Sound effects triggered at correct game events.

## Depends On
- T004 (ResourceManager)
- T024 (GameScreen)

## Spec References
- `raylib-cpp/04-build-test.md` → "Sound Effects" section

## Files to Create/Modify
- Add playSfx function (to resource.h or new sfx.h)
- Wire into game_screen

## Implementation Steps
1. Create `playSfx(const ResourceManager&, Sfx)` function with static sfxMap in `game::` namespace
2. StopSound before PlaySound to prevent overlap
3. Wire triggers: hit.wav on monster attack, shield.wav on shield card, equip.wav on item/heal, deal-card.wav on card flip, chest.wav on chest entry, coins.wav on gold card, lose.wav on lose status, win.wav on win status
4. Test all 8 SFX

## Constraints
- Uses ResourceManager for sound access
- StopSound before PlaySound
- `game::` namespace
- `#pragma once` header guard
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [ ] All 8 SFX play at correct moments
- [ ] No overlap
- [ ] No audio artifacts

## Notes
(filled in during/after implementation)
