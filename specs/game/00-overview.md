# spec: game-overview

## Goal
Define the core game architecture, hero system, save/load mechanism, and gold economy for "Drop the Volunteer" — a browser-based roguelike card-dungeon crawler.

## Requirements
1. Player drops a hero card into an abyss and navigates through rows of face-down cards
2. Flipping cards reveals monsters, shields, healing, gold, items, or special cards
3. Goal: survive and reach the bottom (level 6 end card)
4. 4 heroes with unique stats, unlockable with gold
5. Persistent account data (gold, unlocked heroes, language) saved to localStorage
6. Current game progress is NOT persisted — new game always starts fresh
7. English and French (bilingual), stored in account config

## Constraints
- Pure data definitions — no rendering logic
- No framework dependencies
- All values must match the original game exactly

## Data Definitions

### Heroes

| Hero | Slug | Life Max | Shield | Starting Bag | Cost | Unlock |
|------|------|----------|--------|-------------|------|--------|
| Headless Knight | `headless-knight` | 15 | 0 | `map`, `apple` | 0 | Default |
| Thief | `thief` | 12 | 0 | `invisibility-cloak`, `monster-eye` | 50 gold | Locked |
| Harpy | `harpy` | 15 | 0 | `ring-of-vitality` | 100 gold | Locked |
| Warrior | `warrior` | 10 | 10 | `shiny-shield`, `emerald` | 200 gold | Locked |

### Unlock Logic
```
function unlockHero(hero, account):
    if hero.cost <= account.gold:
        account.gold -= hero.cost
        hero.locked = false
        return true
    return false
```

### Save Data Structure
```json
{
    "gold": 0,
    "config": { "lang": "en" },
    "collection": {
        "heroes": [
            {
                "slug": "headless-knight",
                "life": 15,
                "life_max": 15,
                "shield": 0,
                "gold": 0,
                "bag": [],
                "cost": 0,
                "locked": false,
                "en": { "name": "...", "description": "..." },
                "fr": { "name": "...", "description": "..." }
            }
        ],
        "cards": []
    }
}
```

### Storage
- **Key**: `"drop-the-volunteer"` in localStorage
- **Format**: JSON string of account object
- **When saved**: After hero unlock, after game ends, after language change, on first load

### Gold Economy

| Event | Reward |
|-------|--------|
| Victory | `hero.gold` (full amount) |
| Defeat | `Math.round(hero.gold / 2)` (half) |

### Score Calculation
```
function calculateScore(stats, won):
    multiplier = won ? 1.5 : 1.0
    totalCards = sum of all stats.cards.values
    return Math.round((totalCards + stats.cards.gold / 2) * multiplier)
```

## Acceptance Criteria
- [ ] Hero table matches original exactly (slugs, stats, costs)
- [ ] Unlock logic deducts gold and sets locked=false
- [ ] Save data structure matches specification
- [ ] Game progress NOT persisted between sessions
- [ ] Victory awards full gold, defeat awards half (rounded)
- [ ] Score calculation uses correct multiplier and formula
