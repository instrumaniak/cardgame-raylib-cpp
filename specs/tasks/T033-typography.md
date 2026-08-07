# T033: Typography

## Status: done
## Phase: 4

## Goal
Implement a centralized typography system with typed text styles (like CSS font roles) so all render code uses consistent font/size per role instead of scattered magic numbers.

## Depends On
- T004 (Resource Manager)
- T002 (Asset Acquisition)

## Spec References
Load these before starting:
- `raylib-cpp/03-ui-systems.md` → "Typography" section
- `raylib-cpp/05-coding-style.md` → "Naming Conventions" section

## Files to Create/Modify
- `src/core/typography.h` (NEW)
- `src/core/typography.cpp` (NEW)
- `src/core/res_keys.h` (MODIFY — add `FntKeys` namespace)
- `src/render/card_draw.cpp` (MODIFY — replace `DrawText()` calls)
- `src/render/hero_panel.cpp` (MODIFY — replace `DrawText()` calls)
- `Makefile` (MODIFY — add `src/core/typography.cpp` to SRC)

## Implementation Steps
1. Add `FntKeys` namespace to `src/core/res_keys.h` with `PearSoda` and `Zepto` constants
2. Create `src/core/typography.h` with `TextStyle` enum and `drawText()`/`measureText()` declarations in `game::render::`
3. Create `src/core/typography.cpp` implementing `drawText()` and `measureText()` — map each TextStyle to font key + size, call `DrawTextEx()`/`MeasureTextEx()` via `ResourceManager::fnt()`
4. Add `src/core/typography.cpp` to `Makefile` SRC list
5. Update `src/render/card_draw.cpp` — replace `DrawText()` → `drawText(res, TextStyle::CardStat, ...)`
6. Update `src/render/hero_panel.cpp` — replace `DrawText()` → `drawText(res, TextStyle::Body/Label/CardStat, ...)`
7. Run `make format` and `make` to verify compilation

## Constraints
- Must use `game::render::` namespace for public API
- `#pragma once` header guard
- `drawText()` takes `const ResourceManager&` as first param
- No raw `DrawText()` or `DrawTextEx()` calls in render files — all through typography system
- Colors stay contextual (not baked into TextStyle) — red HP vs green HP is a render concern
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] `src/core/typography.h` and `src/core/typography.cpp` exist and compile
- [x] `FntKeys::PearSoda` and `FntKeys::Zepto` constants added to `res_keys.h`
- [x] `drawText()` renders correct font + size for each `TextStyle`
- [x] `measureText()` returns correct dimensions for each `TextStyle`
- [x] `card_draw.cpp` uses `drawText()` instead of raw `DrawText()`
- [x] `hero_panel.cpp` uses `drawText()` instead of raw `DrawText()`
- [x] `make` succeeds without errors
- [x] `make format-check` passes

## Notes
- Original game CSS uses: PearSoda for `.hero`/`.card` elements, Zepto-Regular as body default
- Size scale from original: 12px (Label) → 16px (Body) → 24px (Heading) → 32px (CardStat) → 64px (Title)
- Text outline for card stats (8-directional parchment outline) can be added as follow-up task
- T019 and T017 notes contain incorrect "no fonts loaded" comments — fonts ARE loaded by ResourceManager but typography system wasn't implemented yet
