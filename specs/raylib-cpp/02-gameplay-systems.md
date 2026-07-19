# spec: raylib-gameplay-systems

## Goal
Implement the core gameplay systems: board generation, card effects, combat, items, and account management as pure C++ functions operating on plain structs.

## Requirements
1. Board generation from LevelConfig with weighted random card placement
2. Card effect resolution for all card types (monster, shield, heal, gold, item, drop, place, biome, end)
3. Combat damage calculation with shield absorption and item modifiers
4. Item system with 22 items, modifiers, and monster-egg combo
5. Account management (save/load, hero unlock, gold economy)
6. All logic pure — no raylib includes, no I/O except account load/save

## Constraints
- Pure functions only — zero rendering, zero raylib dependency
- Must match spec/game/01 through spec/game/09 exactly
- Weighted random uses C++ <random> (std::mt19937)
- Monster-egg combo: 3 eggs → 1 omelette
- All public APIs must be inside `game::logic::` namespace

## Board Generation

```cpp
// src/logic/board_gen.h
#pragma once
#include <vector>
#include <random>
#include "core/types.h"

namespace game::logic {

struct GeneratedBoard {
    std::vector<std::vector<CardSlot>> slots;
    std::string biome;
    bool hasEndCard;
};

GeneratedBoard generateBoard(const LevelConfig& config, int level,
                             const std::vector<std::string>& availableBiomes,
                             std::mt19937& rng);

} // namespace game::logic
```

### Algorithm (per spec/game/01-board-generation.md)

1. Create rows × 3 empty slots
2. Place chests in rows 0..totalRows-4 (weighted chest/goldenChest)
3. Place items in rows 0..totalRows-3
4. Fill first row (row 0) — no monsters
5. Fill last row (biome cards — 1 or 2 biomes) or end card on level 6
6. Fill second-to-last row (BOSS monster in center slot)
7. Fill remaining normal rows with weighted random card count and type

```cpp
// Weighted selection helper
template<typename T>
T weightedPick(const std::vector<std::pair<T, int>>& rates, std::mt19937& rng);
```

Card rates per level (from spec/game/01):

| Level | Monster | Shield | Heal | Gold | Item |
|-------|---------|--------|------|------|------|
| 1     | 100     | 33     | 33   | 33   | 1    |
| 2     | 107     | 31     | 31   | 31   | 1    |
| 3     | 113     | 29     | 29   | 29   | 1    |
| 4     | 113     | 29     | 29   | 29   | 1    |
| 5     | 100     | 33     | 33   | 33   | 1    |
| 6     | 107     | 31     | 31   | 31   | 1    |

Cards-per-row distribution (probabilities for 1/2/3 cards):

| Level | 1 card | 2 cards | 3 cards |
|-------|--------|---------|---------|
| 1-2   | 10%    | 15%     | 75%     |
| 3-4   | 15%    | 20%     | 65%     |
| 5     | 15%    | 20%     | 65%     |
| 6     | 25%    | 25%     | 50%     |

## Card Effects

```cpp
// src/logic/card_effects.h
#pragma once
#include "core/types.h"

namespace game::logic {

CardEffect resolveCard(const Card& card, const PlayerState& player,
                       const std::vector<Card>& inventoryItems);
CardEffect resolveMonster(const Card& card, const PlayerState& player,
                          const std::vector<Card>& inventoryItems);
CardEffect resolveShield(const Card& card);
CardEffect resolveHeal(const Card& card, const PlayerState& player,
                       const std::vector<Card>& inventoryItems);
CardEffect resolveGold(const Card& card);
CardEffect resolveItem(const Card& card);             // returns item slug as message
CardEffect resolveDrop(const Card& card);             // returns item slug as message
CardEffect resolvePlace(const Card& card);            // chest/goldenChest
CardEffect resolveBiome(const Card& card);            // biome transition
CardEffect resolveEnd();

} // namespace game::logic
```

## Combat

```cpp
// src/logic/combat.h
#pragma once
#include "core/types.h"

namespace game::logic {

struct CombatResult {
    int damageToPlayer;
    int shieldAbsorbed;
    int playerShieldRemaining;
    bool dodged;                    // invisibility cloak
    int goldGained;                 // emerald bonus
};

// Item modifiers
int getMonsterCardModifier(const std::vector<Card>& items);
int getHealCardModifier(const std::vector<Card>& items);
int getShieldCardModifier(const std::vector<Card>& items);
int getHeroLifeMaxModifier(const std::vector<Card>& items);
int getNbRowsHidden(const std::vector<Card>& items);

// Core combat
CombatResult applyDamage(int baseDamage, const PlayerState& player,
                         const std::vector<Card>& items);
ShieldData applyToShield(int damage, const ShieldData& shield);
bool checkInvisibilityCloak(const std::vector<Card>& items, int currentHp);
int calcMaxHp(const std::vector<Card>& items, int baseMax);

} // namespace game::logic
```

### Ring of Courage
+5 shield when HP == 1 after a fight, per ring in inventory.

### Ring of Vitality
+5 max HP; shields become healing instead (shield card → heal card).

### Cursed Shield
Sets max HP to 1; shields become additive (shield card value adds to shield directly).

### Monster Skull
+1 to monsterCardModifier per skull (monsters deal more damage).

### Emerald
On monster hit: +4 gold per emerald.

### Ruby
On gold card: 25% chance of +1 HP.

## Items

```cpp
// src/logic/items.h
#pragma once
#include "core/types.h"

namespace game::logic {

struct ComboResult {
    bool triggered;
    std::vector<Card> newItems;
};

// All 22 items (see spec/game/03-item-effects.md)
ComboResult checkMonsterEggCombo(std::vector<Card>& items);
void applyAppleEffect(HealthData& health, const std::vector<Card>& items);
void applyRingOfProtection(ShieldData& shield, const std::vector<Card>& items);
void applyDragonWing(ShieldData& shield);
void applyAthelas(PlayerState& player, std::vector<Card>& items);
bool applyCursedShield(PlayerState& player, const std::vector<Card>& items);
void applyAmethyst(GoldData& gold, std::vector<Card>& items);

} // namespace game::logic
```

### Key Items (all 22 from spec/game/03)

| Slug | Allow Multiple | Effect |
|------|---------------|--------|
| map | No | Reveals board sidebar |
| horn | Yes | +20 to heal spawn rate |
| scales | Yes | +20 to shield spawn rate |
| omelette | Yes | +3 max HP per omelette |
| athelas | No | Prevents death once (life=1), removes from bag |
| raw-meat | Yes | +1 heal modifier |
| rotten-meat | Yes | -1 heal modifier |
| rusty-shield | Yes | -1 shield mod, +20 shield spawn |
| shiny-shield | Yes | +1 shield mod, -20 shield spawn |
| apple | Yes | +10 HP on level change |
| dragon-wing | Yes | +1 shield after fight if shield was 0 |
| monster-skull | Yes | +1 monster damage mod |
| carrot | Yes | +1 HP on position change |
| fossil | Yes | +10 to monster spawn rate |
| ring-of-vitality | No | +5 max HP, shields→heal |
| ring-of-protection | Yes | +5 shield on level change |
| ring-of-courage | Yes | +5 shield if HP==1 after fight |
| ring-of-greed | Yes | -1 HP per gold card |
| cursed-shield | No | Max HP=1, shields additive |
| ruby | Yes | 25% +1 HP on gold card |
| emerald | Yes | +4 gold per monster hit |
| amethyst | Yes | Double gold, remove self |
| monster-egg | N/A | Collect 3 → omelette |

## Account

```cpp
// src/logic/account.h
#pragma once
#include <string>
#include "core/types.h"

namespace game::logic {

// Account methods are defined in core/types.h (see 01-architecture.md)
// This file provides additional account utility functions:

LevelConfig getLevelConfig(int level);     // levels 1-6
LevelConfig getChestConfig(const std::string& chestType, int level);
std::vector<std::string> getAvailableBiomes(int level,
                                            const std::vector<std::string>& visited);

} // namespace game::logic
```

### Gold Economy

| Event | Reward |
|-------|--------|
| Victory | `hero.gold` (full amount) |
| Defeat | `round(hero.gold / 2)` |

### Score

```
score = won ? round((totalCards + gold/2) * 1.5)
           : round(totalCards + gold/2)

where totalCards = sum of all card type counts
```

## Acceptance Criteria
- [ ] All logic functions are inside `game::logic::` namespace
- [ ] Board generation matches spec/game/01-board-generation.md exactly
- [ ] Card effects match spec/game/04-combat-system.md, 05-healing-shields.md, 06-gold-scoring.md
- [ ] Item effects match spec/game/03-item-effects.md (all 22 items)
- [ ] Monster-egg combo (3→1 omelette) works correctly
- [ ] Level configs match spec/game/09-level-progression.md exactly
- [ ] Account load/save produces valid JSON
- [ ] All pure functions compile without raylib includes
- [ ] Gold economy (full/half) matches spec/game/06-gold-scoring.md
