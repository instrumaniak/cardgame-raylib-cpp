# spec: card-types

## Goal
Define all card types, their slugs, values, and visual properties for the game.

## Requirements
1. Each card has: slug, type, value, name, description, image
2. Card types: monster, shield, heal, gold, item, drop, place, biome, end
3. Monster values are per-biome
4. Shield/heal/gold values are tiered by slug
5. Place cards have value 0

## Constraints
- Pure data definitions — no logic
- No framework dependencies
- All values must match original game exactly

## Data Definitions

### Monster Cards

Monsters are per-biome. Slug is the biome name, value is the damage.

| Biome | Monster Values |
|-------|---------------|
| forest | 1, 2, 3, 4, 5, 6, 7, 8, 9 |
| cave | 1, 2, 3, 4, 5, 6, 7, 8, 9 |
| enchanted_lands | 1, 2, 3, 4, 5, 6, 7, 8, 9 |
| desert | 1, 2, 3, 4, 5, 6, 7, 8, 9 |
| abyss | 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13 |
| underworld | 5, 6, 7, 8, 9, 10, 11, 12, 13, 14 |

### Shield Cards

| Slug | Values |
|------|--------|
| `wooden-shield` | 1, 2 |
| `shield` | 3, 4 |
| `iron-shield` | 5, 6 |
| `reinforced-shield` | 7, 8 |
| `legendary-shield` | 9 |

### Healing Cards

| Slug | Values |
|------|--------|
| `minor-healing-potion` | 1, 2, 3 |
| `healing-potion` | 4, 5, 6 |
| `major-healing-potion` | 7, 8 |
| `holy-healing-potion` | 9 |

### Gold Cards

| Slug | Values |
|------|--------|
| `one-coin` | 1 |
| `two-coins` | 2 |
| `three-coins` | 3, 4, 5 |
| `many-coins` | 6, 7, 8 |
| `lot-of-coins` | 9 |

### Place Cards

| Slug | Value |
|------|-------|
| `chest` | 0 |
| `golden_chest` | 0 |

### Biome Cards

Available biomes per level:

| Level | Available Biomes |
|-------|-----------------|
| 1 | forest, cave, enchanted_lands, desert |
| 2 | abyss |
| 3 | underworld |

### Special Cards

- `end` — Generated in code on level 6, marks game completion

### Card Type to Frame Color

| Type | Frame | BG Color |
|------|-------|----------|
| monster | card-frame-purple | #53428d |
| heal | card-frame-red | #d94743 |
| shield | card-frame-gray | #616161 |
| gold | card-frame-yellow | #fbc02d |
| item | hero-frame | dark bg |
| drop | hero-frame | dark bg |
| place | chest-frame | #f3d19f |
| biome | hero-frame | dark bg |
| end | light-frame | gold glow |

## Acceptance Criteria
- [ ] Monster values match per-biome tables exactly
- [ ] Shield/heal/gold slugs and value ranges match tables
- [ ] Place cards always have value 0
- [ ] Biome availability matches level progression
- [ ] End card only generated on level 6
- [ ] Frame color mapping is complete for all card types
