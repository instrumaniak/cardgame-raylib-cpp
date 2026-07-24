# T030: Final Polish

## Status: pending
## Phase: 6

## Goal
Final build verification, bug fixes, and polish.

## Depends On
- All previous tasks

## Spec References
- `raylib-cpp/04-build-test.md` → "Polish" acceptance criteria

## Files to Create/Modify
- Various (bug fixes)

## Implementation Steps
1. Verify `make` compiles release with -O2
2. Verify `make debug` compiles with -g -O0 -DDEBUG
3. Verify `make test` passes all tests
4. Verify no entity leaks on screen transitions
5. Verify window resizes and letterboxes correctly
6. Verify fullscreen toggle (F11 / Alt+Enter)
7. Set window title "Drop the Volunteer"
8. Fix any remaining bugs
9. Run full gameplay session end-to-end

## Constraints
- No new features
- Only fixes and polish

## Acceptance Criteria
- [ ] All acceptance criteria from all specs met
- [ ] No crashes
- [ ] No memory leaks
- [ ] Smooth gameplay

## Notes
(filled in during/after implementation)
