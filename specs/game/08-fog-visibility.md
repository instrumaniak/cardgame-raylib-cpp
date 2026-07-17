# spec: fog-visibility

## Goal
Define the row visibility and fog system that controls how many rows the player can see.

## Requirements
1. Player always sees 3 rows: current, next, and row after that
2. Default: no fog (all 3 rows visible)
3. 1 monster-eye: fog hides the 3rd row (see current + next only)
4. 2+ monster-eyes: fog hides rows 2 and 3 (see current row only)
5. Monster-eye max is 2 (more than 2 has no additional effect)

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies

## Pseudocode

### Row Visibility
```
function row_visible(rowId, currentRow):
    return rowId >= currentRow AND rowId < currentRow + 3
```

### Row Fog
```
function row_fog(rowId, currentRow, nbRowsHidden):
    if NOT row_visible(rowId, currentRow):
        return false  // not visible at all
    return rowId > currentRow + (2 - nbRowsHidden)
```

### Nb Rows Hidden
```
function get_nb_rows_hidden(bag):
    count = 0
    for each item in bag:
        if item.slug == "monster-eye": count++
    if count > 2: count = 2
    return count
```

### Visibility Examples

| Monster Eyes | Current Row | Row+1 | Row+2 | Row+3 |
|-------------|-------------|-------|-------|-------|
| 0 | Visible | Visible | Visible | Hidden |
| 1 | Visible | Visible | Fogged | Hidden |
| 2+ | Visible | Fogged | Fogged | Hidden |

## Acceptance Criteria
- [ ] row_visible: true for currentRow, currentRow+1, currentRow+2
- [ ] row_visible: false for currentRow+3 and beyond
- [ ] 0 monster-eyes: no fog on any visible row
- [ ] 1 monster-eye: fog on row currentRow+2
- [ ] 2 monster-eyes: fog on row currentRow+1 and currentRow+2
- [ ] 3+ monster-eyes: same as 2 (capped at 2)
- [ ] Fogged rows show card backs (not face-up cards)
