# spec: level-progression

## Goal
Define the 6 main levels, chest sub-locations, and biome progression system.

## Requirements
1. 6 main levels, each with 15 rows
2. Level 6 ends with "end" card (game completion)
3. Chest sub-locations: 2-3 rows, then return to main map
4. Biome progression: level 1 has 4 biomes, level 2 has abyss, level 3 has underworld
5. Visited biomes cannot be chosen again
6. Each level has different card rates and monster values

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- All values must match original game exactly

## Data Definitions

### Main Map Levels (1-6)

| Level | Rows | Cards/Row (1/2/3) | Monster Rate | Monster Values | Shield Rate | Heal Rate | Gold Rate | Item Rate |
|-------|------|-------------------|-------------|----------------|-------------|-----------|-----------|-----------|
| 1 | 15 | 10/15/75 | 100 | 1-9 (equal) | 33 | 33 | 33 | 1 |
| 2 | 15 | 10/15/75 | 107 | 1-9 (equal) | 31 | 31 | 31 | 1 |
| 3 | 15 | 15/20/65 | 113 | 1-9 (equal) | 29 | 29 | 29 | 1 |
| 4 | 15 | 15/20/65 | 113 | 1-9 (weighted: 9,9,9,10,10,11,11,11) | 29 | 29 | 29 | 1 |
| 5 | 15 | 15/20/65 | 100 | 3-12 (equal) | 33 | 33 | 33 | 1 |
| 6 | 15 | 25/25/50 | 107 | 5-13 (equal) | 31 | 31 | 31 | 1 |

**Cards/Row**: Probability percentages for 1, 2, or 3 cards in a row.

### Chest Sub-Locations

**Regular Chest** (2 rows):
- Rates: item:70, shield:10, heal:10, gold:10
- Cards/Row: 5/10/85

**Golden Chest** (3 rows):
- Rates: item:55, shield:15, heal:15, gold:15
- Cards/Row: 5/10/85

**Level 5 Chest** (wider rows):
- Cards/Row: 20/30/50

### Biome Progression

| Level | Available Biomes |
|-------|-----------------|
| 1 | forest, cave, enchanted_lands, desert (4 options) |
| 2 | abyss (1 option) |
| 3 | underworld (1 option) |

- Each visited biome is added to `biomes.done` and cannot be chosen again
- On last row of main level, game picks from available undisplayed biomes

## Pseudocode

### Level Config
```
function getLevelConfig(level):
    configs = [
        {rows: 15, cardsPerRow: [10,15,75], monsterRate: 100, monsterValues: [1..9], shieldRate: 33, healRate: 33, goldRate: 33, itemRate: 1},
        {rows: 15, cardsPerRow: [10,15,75], monsterRate: 107, monsterValues: [1..9], shieldRate: 31, healRate: 31, goldRate: 31, itemRate: 1},
        {rows: 15, cardsPerRow: [15,20,65], monsterRate: 113, monsterValues: [1..9], shieldRate: 29, healRate: 29, goldRate: 29, itemRate: 1},
        {rows: 15, cardsPerRow: [15,20,65], monsterRate: 113, monsterValues: [9,9,9,10,10,11,11,11], shieldRate: 29, healRate: 29, goldRate: 29, itemRate: 1},
        {rows: 15, cardsPerRow: [15,20,65], monsterRate: 100, monsterValues: [3..12], shieldRate: 33, healRate: 33, goldRate: 33, itemRate: 1},
        {rows: 15, cardsPerRow: [25,25,50], monsterRate: 107, monsterValues: [5..13], shieldRate: 31, healRate: 31, goldRate: 31, itemRate: 1},
    ]
    return configs[min(level-1, 5)]
```

### Chest Config
```
function getChestConfig(chestType, level):
    if chestType == "chest":
        config = {rows: 2, rates: {item:70, shield:10, heal:10, gold:10}, cardsPerRow: [5,10,85]}
    elif chestType == "golden_chest":
        config = {rows: 3, rates: {item:55, shield:15, heal:15, gold:15}, cardsPerRow: [5,10,85]}
    
    // Level 5 chests have wider rows
    if level == 5:
        config.cardsPerRow = [20,30,50]
    
    return config
```

### Biome Selection
```
function getAvailableBiomes(level, doneBiomes):
    allBiomes = {
        1: ["forest", "cave", "enchanted_lands", "desert"],
        2: ["abyss"],
        3: ["underworld"],
    }
    biomes = allBiomes[level] or allBiomes[1]
    return filter(biomes, b => !doneBiomes.includes(b))
```

## Acceptance Criteria
- [ ] 6 levels with correct row counts and rates
- [ ] Level 4 monster values: weighted [9,9,9,10,10,11,11,11]
- [ ] Level 5 monster values: 3-12 (equal)
- [ ] Level 6 monster values: 5-13 (equal)
- [ ] Regular chest: 2 rows, item:70 rates
- [ ] Golden chest: 3 rows, item:55 rates
- [ ] Level 5 chest: wider rows (20/30/50)
- [ ] Level 1: 4 biomes available
- [ ] Level 2: only abyss
- [ ] Level 3: only underworld
- [ ] Visited biomes excluded from selection
