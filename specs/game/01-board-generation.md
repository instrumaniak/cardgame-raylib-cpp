# spec: board-generation

## Goal
Define the board structure, card placement algorithm, and weighted random selection for generating game boards.

## Requirements
1. Each board has `rows` rows, each row has exactly 3 slots (positions 0, 1, 2)
2. Cards are placed into slots; empty slots have `card: null`
3. Chests placed first, then items, then special rows, then normal rows
4. First row of main location has no monsters
5. Last row of main location has biome cards (or end card on level 6)
6. Second-to-last row has BOSS monster in center slot
7. Normal rows use weighted random for card count and type

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Must be deterministic with a seed (for testing) or random (for gameplay)

## Pseudocode

### Weighted Random Selection
```
function get_random_number_by_rates(rates):
    total = sum of all rates
    random = random(0, total)
    cumulative = 0
    for each key in rates:
        cumulative += rates[key]
        if random <= cumulative:
            return key
```

### Card Placement Algorithm

```
function generateBoard(level, biome, total_rows):
    board = array of total_rows, each with 3 empty slots
    
    // Step 1: Place chests
    for i in 0..nb_places:
        row = random(0, total_rows - 3)  // avoids last 3 rows
        slot = random(0, 2)
        chest_type = weighted_random({chest: 80, golden_chest: 20})
        board[row].slots[slot] = chest_type
    
    // Step 2: Place items
    for i in 0..nb_items:
        row = random(0, total_rows - 3)
        slot = random(0, 2)
        board[row].slots[slot] = {type: "item"}
    
    // Step 3: First row (no monsters)
    for each empty slot in row 0:
        board[0].slots[slot] = pickCard(types_rates_EXCEPT_monsters)
    
    // Step 4: Last row (biome cards or end)
    if level == 6:
        board[total_rows-1].slots[1] = {type: "end"}
    else:
        biomes = availableBiomes(level)
        if biomes.length == 1:
            board[total_rows-1].slots[1] = {type: "biome", biome: biomes[0]}
        elif biomes.length == 2:
            board[total_rows-1].slots[0] = {type: "biome", biome: biomes[0]}
            board[total_rows-1].slots[2] = {type: "biome", biome: biomes[1]}
        else:
            board[total_rows-1].slots[0] = {type: "biome", biome: biomes[0]}
            board[total_rows-1].slots[1] = {type: "biome", biome: biomes[1]}
            board[total_rows-1].slots[2] = {type: "biome", biome: biomes[2]}
    
    // Step 5: Second-to-last row (BOSS)
    boss = lastMonsterInBiome(biome)
    board[total_rows-2].slots[1] = boss
    
    // Step 6: Normal rows
    for row in 1..total_rows-3:
        nb_cards = weighted_random(rates.nb_cards_by_row)
        positions = randomPositions(nb_cards)  // pick nb_cards of 3
        for each position:
            board[row].slots[position] = pickCard(types_rates)
    
    return board
```

### Card Type Rates (per level)

| Level | Monster | Shield | Heal | Gold | Item |
|-------|---------|--------|------|------|------|
| 1 | 100 | 33 | 33 | 33 | 1 |
| 2 | 107 | 31 | 31 | 31 | 1 |
| 3 | 113 | 29 | 29 | 29 | 1 |
| 4 | 113 | 29 | 29 | 29 | 1 |
| 5 | 100 | 33 | 33 | 33 | 1 |
| 6 | 107 | 31 | 31 | 31 | 1 |

### Cards Per Row Distribution

| Level | 1 card | 2 cards | 3 cards |
|-------|--------|---------|---------|
| 1 | 10% | 15% | 75% |
| 2 | 10% | 15% | 75% |
| 3 | 15% | 20% | 65% |
| 4 | 15% | 20% | 65% |
| 5 | 15% | 20% | 65% |
| 6 | 25% | 25% | 50% |

## Acceptance Criteria
- [ ] Board has correct number of rows and 3 slots per row
- [ ] Chests placed in rows 0 to total_rows-4 (avoids last 3)
- [ ] First row has no monsters
- [ ] Last row has biome cards (or end card on level 6)
- [ ] Second-to-last row has BOSS in center
- [ ] Weighted random returns values proportional to rates
- [ ] Card count distribution matches level config
