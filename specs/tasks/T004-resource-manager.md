# T004: Resource Manager

## Status: done
## Phase: 1

## Goal
Implement a ResourceManager class that loads and caches all game assets.

## Depends On
- T001 (Project Setup)
- T002 (Asset Acquisition)

## Spec References
Load these before starting:
- `raylib-cpp/01-architecture.md` → "Resource Manager" section
- `raylib-cpp/05-coding-style.md` → "Class Design" section

## Files to Create/Modify
- `src/core/resource.h` (NEW)
- `src/core/resource.cpp` (NEW)

## Implementation Steps
1. Create `resource.h` with ResourceManager class in `game::` namespace
2. Define private members: `_textures`, `_sounds`, `_fonts` as unordered_maps
3. Implement public methods: `loadAll()`, `unloadAll()`, `tex(key)`, `snd(key)`, `fnt(key)`, `hasTex(key)` with const correctness
4. Implement `loadAll()` to scan assets directories and load all resources
5. Implement accessor methods returning const references
6. Verify compilation and basic functionality

## Constraints
- Class (not struct) design
- Private members with `_camelCase` naming
- Const references for all accessor methods
- Must use `game::` namespace
- Follow `specs/raylib-cpp/05-coding-style.md`

## Acceptance Criteria
- [x] Compiles without errors
- [x] `loadAll()` successfully loads assets
- [x] Accessors return const references
- [x] `make` succeeds
- [x] No memory leaks on unload

## Notes
- Implemented using raylib's `LoadDirectoryFiles` for directory scanning
- Keys derived via `GetFileNameWithoutExt()` — filename without extension
- Makefile updated with `mkdir -p $(dir $@)` for subdirectory build targets
- 20 textures, 8 sounds, 2 fonts loaded from assets/img, assets/audio, assets/fonts