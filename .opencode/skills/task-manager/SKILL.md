---
name: task-manager
description: Manage implementation tasks in specs/tasks/. Use when the user says /task, wants to check task status, start/pause/complete a task, see what to work on next, create new tasks, or manage git worktrees for parallel task work. Also use when asking about task dependencies or what tasks are available.
---

# Task Manager

Manage the implementation task queue in `specs/tasks/`.

## Commands

Parse the user's message for these patterns:

### `/task status`
Show all tasks with their current status.

1. Read `specs/tasks/README.md` for the task list
2. For each T*.md file in `specs/tasks/`, extract the `## Status:` line
3. Display as a grouped table by phase:

```
Phase 0: Scaffolding
  T001  Project setup         [done]
  T002  Asset acquisition     [in_progress]

Phase 1: Foundation
  T003  Core types            [pending]
  T004  ResourceManager       [pending]
  ...

Summary: 5 done, 2 in_progress, 23 pending
```

### `/task do TXXX`
Start working on a task.

1. Read `specs/tasks/TXXX-*.md`
2. Check that all dependencies listed under `## Depends On` are `done` — warn the user if any are not
3. Set `## Status: in_progress` in the task file
4. Optionally create a git worktree for parallel work:
   ```bash
   git worktree add ../cardgame-TXXX -b task/TXXX
   ```
   Skip if the worktree already exists. Ask the user if they want one.
5. Display the task's goal, implementation steps, and acceptance criteria so the user can begin

### `/task done TXXX`
Mark a task as complete.

1. Read `specs/tasks/TXXX-*.md`
2. Set `## Status: done`
3. Update the status column in `specs/tasks/README.md`
4. Show what tasks are now unblocked (tasks whose dependencies are all `done`)

### `/task pause TXXX`
Pause a task (revert to pending).

1. Read `specs/tasks/TXXX-*.md`
2. Set `## Status: pending`
3. Ask if the user wants to remove the git worktree:
   ```bash
   git worktree remove ../cardgame-TXXX
   ```

### `/task next`
Find the next task to work on.

1. Read all task files and their statuses
2. Find all `pending` tasks where every dependency is `done`
3. Prioritize using this order:
   - Critical path first: T001 → T003 → T010 → T011 → T012 → T016 → T024
   - Then by phase number (lower first)
   - Then by task number (lower first)
4. Display the recommended next task with its goal and first implementation step

### `/task deps TXXX`
Show dependency information for a task.

1. Read `specs/tasks/TXXX-*.md` for `## Depends On`
2. Scan all other task files to find what depends on TXXX
3. Display:
   ```
   T017-card-rendering depends on:
     T003-core-types           [done] ✓
     T004-resource-manager     [done] ✓

   Tasks waiting on T017:
     T018-board-rendering      [pending]
   ```

### `/task worktree TXXX`
Create or manage a git worktree for parallel task work.

1. Check if worktree `../cardgame-TXXX` exists:
   ```bash
   git worktree list
   ```
2. If not, create it:
   ```bash
   git worktree add ../cardgame-TXXX -b task/TXXX
   ```
3. Display the worktree path and branch name
4. To remove later: `git worktree remove ../cardgame-TXXX`

### `/task create "Task Name"`
Create a new task from the standard template.

1. Determine the next task number (find highest TXXX, add 1)
2. Convert the name to kebab-case for the filename
3. Create `specs/tasks/TXXX-kebab-case-name.md` with:
   ```markdown
   # TXXX: Task Name

   ## Status: pending

   ## Goal
   (one sentence describing what this task produces)

   ## Depends On
   - (list dependencies or "None")

   ## Spec References
   - (reference relevant spec sections)

   ## Files to Create/Modify
   - (list files)

   ## Implementation Steps
   1. (step)

   ## Constraints
   - Must use `game::` namespace
   - `#pragma once` header guard
   - Follow `specs/raylib-cpp/05-coding-style.md`

   ## Acceptance Criteria
   - [ ] (criterion)

   ## Notes
   ( filled in during/after implementation )
   ```
4. Update `specs/tasks/README.md` to include the new task in the appropriate phase

## Implementation Notes

All task operations are file manipulation:
- Read/parse Markdown task files
- Update the `## Status:` line using the edit tool
- Update README.md status table
- Git worktree commands via bash

The task status is always stored in the task file itself — no external database or state files.
