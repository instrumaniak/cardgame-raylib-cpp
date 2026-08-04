# T031: Clang-Format & Pre-Commit Hook

## Status: done
## Phase: 0
## Critical: false

## Goal
Set up automatic code formatting with clang-format and a git pre-commit hook to enforce consistent style before every commit.

## Depends On
- T001 (Project Setup — Makefile exists)

## Spec References
- `raylib-cpp/05-coding-style.md` → Section 15 (`.clang-format` configuration)
- `raylib-cpp/05-coding-style.md` → Section 3 (Formatting rules)

## Files to Create/Modify
- `.clang-format` (MODIFY — replace stub with full config from spec §15)
- `.pre-commit-config.yaml` (NEW)
- `Makefile` (MODIFY — add `format` and `format-check` targets)

## Implementation Steps

### 1. Install pre-commit framework
```bash
pip install pre-commit
```
Note: clang-format binary is bundled via the pre-commit hook — no `apt install` needed.

### 2. Update `.clang-format` with full spec config
Replace the current stub (`BasedOnStyle: LLVM`) with the complete configuration from `specs/raylib-cpp/05-coding-style.md` §15. Key settings:
- `IndentWidth: 2`
- `BreakBeforeBraces: Attach` (K&R)
- `ColumnLimit: 100`
- `SortIncludes: CaseSensitive` with include category regrouping
- `PointerAlignment: Left`

### 3. Create `.pre-commit-config.yaml`
```yaml
repos:
  - repo: https://github.com/pre-commit/mirrors-clang-format
    rev: v19.1.7
    hooks:
      - id: clang-format
        types_or: [c++, c]
```

### 4. Install the pre-commit hook
```bash
pre-commit install
```

### 5. Add Makefile targets
```makefile
format:
	clang-format -i src/**/*.cpp src/**/*.h

format-check:
	clang-format --dry-run --Werror src/**/*.cpp src/**/*.h
```

### 6. Run initial full-repo format pass
```bash
clang-format -i src/**/*.cpp src/**/*.h
```
Commit this as a standalone formatting commit (no logic changes mixed in).

## Constraints
- Must use the `.clang-format` config from spec §15 (not a custom config)
- pre-commit framework only (no raw `.git/hooks` scripts)
- clang-format version pinned via `rev:` tag in `.pre-commit-config.yaml`
- Initial format pass must be a dedicated commit (not mixed with code changes)
## Acceptance Criteria

- [x] `.clang-format` matches spec §15 exactly
- [x] `.pre-commit-config.yaml` exists with `mirrors-clang-format` hook
- [x] `pre-commit install` succeeds and hook is active
- [x] `pre-commit run --all-files` passes cleanly
- [x] `make format` formats all source files
- [x] `make format-check` passes without errors
- [x] Committing a poorly-formatted `.cpp` file is blocked by the hook
- [x] Initial full-repo format commit is clean (no logic changes)

## Notes
- Installed `pre-commit` v4.6.1 and `clang-format` v22.1.8 via pip
- Pinned `mirrors-clang-format` to `v22.1.8` in `.pre-commit-config.yaml`
- `.clang-format` updated from stub (`BasedOnStyle: LLVM`) to full spec §15 config
- Makefile targets `format` and `format-check` added
- Initial format pass applied to all source files (7 files reformatted)
- Build verified: `make clean && make` passes
