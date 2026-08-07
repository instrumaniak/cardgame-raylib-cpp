# T029: JSON Data Loading

## Status: pending
## Phase: 6

## Goal
Load game data (heroes, cards, items, levels, biomes) from JSON files and provide
registries that enrich Card/Hero structs with sprite coordinates and metadata.

## Depends On
- T003 (types)
- T002 (assets)
- T009 (board gen — for integration)
- T024 (game screen — for integration)

## Spec References
- `game/00-overview.md` (hero table)
- `game/02-card-types.md`
- `game/09-level-progression.md`
- `raylib-cpp/01-architecture.md` (data/ layer)
- Original game JS source (card registry `lt`, `get_random_card()`)

## JSON Data Model (verified against original game)

### cards.json
Structure: `{ "monsters": { "<biome>": [...] }, "shields": [...], "heals": [...], "golds": [...] }`

**Monsters** — one entry per (biome, value). Each entry:
```json
{ "value": 5, "name": "Forest Monster 5", "description": "Deals 5 damage",
  "sprite": { "sheet": "cards", "x": 8, "y": 863, "w": 168, "h": 171 } }
```
Biomes: forest (1-9), cave (1-9), enchanted_lands (1-9), desert (1-9), abyss (3-13), underworld (5-14).

**Shields** — flat array, 9 entries (one per value 1-9). Each entry:
```json
{ "slug": "wooden-shield", "type": "shield", "value": 1, "name": "Wooden Shield",
  "description": "Basic protection",
  "sprite": { "sheet": "cards", "x": 1504, "y": 170, "w": 32, "h": 32 } }
```
Value→slug mapping: wooden-shield(1,2), shield(3,4), iron-shield(5,6), reinforced-shield(7,8), legendary-shield(9).

**Heals** — flat array, 9 entries. Same structure. Value→slug: minor-healing-potion(1,2,3), healing-potion(4,5,6), major-healing-potion(7,8), holy-healing-potion(9).

**Golds** — flat array, 9 entries. Same structure. Value→slug: one-coin(1), two-coins(2), three-coins(3,4,5), many-coins(6,7,8), lot-of-coins(9).

**Spawning mechanism**: Type is chosen via weighted random from `types_rates`. For shields/heals/golds, `_.sample(array)` picks uniformly from all 9 entries (equal probability per value). For monsters, value is chosen first via `monster_values` rates, then looked up by `(biome, value)`.

### items.json
Flat array, 23 items. Each entry:
```json
{ "slug": "apple", "name": "Apple", "description": "+10 HP on level change",
  "allow_multiple": true,
  "sprite": { "sheet": "cards", "x": 1504, "y": 314, "w": 32, "h": 32 } }
```
Must include all 23 items from original: map, horn, scales, omelette, athelas, raw-meat, rotten-meat, rusty-shield, shiny-shield, apple, dragon-wing, monster-skull, carrot, fossil, ring-of-vitality, ring-of-protection, ring-of-courage, ring-of-greed, cursed-shield, ruby, emerald, amethyst, monster-egg, **invisibility-cloak**, **monster-eye**.

Note: `monster-egg` type is `"drop"` (not `"item"`) — it's a collectible that combos into omelette.

### heroes.json
Array, 4 original heroes + 4 custom (locked). Each entry:
```json
{ "slug": "headless-knight", "name": "Headless Knight",
  "description": "A glorious fighter who fought one too many battles.",
  "life_max": 15, "shield": 0, "cost": 0, "locked": false,
  "starting_items": ["map", "apple"],
  "sprite": { "sheet": "cards", "x": 1216, "y": 828, "w": 111, "h": 180 } }
```
Original heroes: headless-knight(0g, 15hp, 0shield, [map,apple]), thief(50g, 12hp, 0shield, [invisibility-cloak,monster-eye]), harpy(100g, 15hp, 0shield, [ring-of-vitality]), warrior(200g, 10hp, **10shield**, [shiny-shield,emerald]).
Custom heroes (locked:true): dark-knight, mage, spirit, anubis.

### levels.json
Array, 6 levels. Each entry:
```json
{ "rows": 15, "monster_rate": 100, "shield_rate": 33, "heal_rate": 33,
  "gold_rate": 33, "item_rate": 1,
  "nb_cards_by_row": { "1": 10, "2": 15, "3": 75 },
  "monster_values": { "1": 10, "2": 10, "3": 10, "4": 10, "5": 10, "6": 10 } }
```
Level 4 monster_values uses weighted distribution: `{ "1": 9, "2": 9, "3": 9, "4": 10, "5": 10, "6": 11, "7": 11, "8": 11 }`.
Level 5 monster_values: `{ "3": 10, "4": 10, "5": 10, "6": 10, "7": 10, "8": 10, "9": 10, "10": 10, "11": 10, "12": 10 }`.
Level 6 monster_values: `{ "5": 10, "6": 10, "7": 10, "8": 10, "9": 10, "10": 10, "11": 10, "12": 10, "13": 10 }`.

### biomes.json
Array, 6 biomes. Each entry:
```json
{ "slug": "forest", "name": "Forest", "level": 1, "color": "#2d5a27" }
```
Level mapping: forest/cave/enchanted_lands/desert = level 1, abyss = level 2, underworld = level 3.

## Card Registry Design

### Types
```cpp
struct CardEntry {
  std::string slug;
  CardType type;
  int value;
  std::string name;
  std::string description;
  int spriteX, spriteY, spriteW, spriteH;
};

struct MonsterEntry {
  int value;
  std::string name;
  std::string description;
  int spriteX, spriteY, spriteW, spriteH;
};

struct ItemEntry {
  std::string slug;
  std::string name;
  std::string description;
  bool allowMultiple;
  int spriteX, spriteY, spriteW, spriteH;
};

struct HeroData {
  std::string slug;
  std::string name;
  std::string description;
  int lifeMax;
  int shield;
  int cost;
  bool locked;
  std::vector<std::string> startingItems;
  int spriteX, spriteY;
};

struct LevelData {
  int rows;
  int monsterRate, shieldRate, healRate, goldRate, itemRate;
  std::unordered_map<int, int> nbCardsByRow;   // cards_count → weight
  std::unordered_map<int, int> monsterValues;   // value → weight
};

struct BiomeData {
  std::string slug;
  std::string name;
  int level;
};
```

### Registries
```cpp
class CardRegistry {
  // shields/heals/golds: type+value → CardEntry (flat lookup)
  std::unordered_map<int, std::unordered_map<int, CardEntry>> _byTypeValue;
  // monsters: biome+value → MonsterEntry
  std::unordered_map<std::string, std::unordered_map<int, MonsterEntry>> _monsters;
public:
  void loadFromJson(const std::string& jsonStr);
  Card getCard(CardType type, int value) const;         // shields/heals/golds
  Card getMonster(const std::string& biome, int value) const;
  bool hasCard(CardType type, int value) const;
  bool hasMonster(const std::string& biome, int value) const;
};

class ItemRegistry {
  std::unordered_map<std::string, ItemEntry> _bySlug;
public:
  void loadFromJson(const std::string& jsonStr);
  Card getItem(const std::string& slug) const;
  bool hasItem(const std::string& slug) const;
};

class HeroRegistry {
  std::unordered_map<std::string, HeroData> _bySlug;
public:
  void loadFromJson(const std::string& jsonStr);
  HeroData getHero(const std::string& slug) const;
  std::vector<HeroData> allHeroes() const;
  bool hasHero(const std::string& slug) const;
};

class LevelRegistry {
  std::vector<LevelData> _levels;  // indexed by level-1
public:
  void loadFromJson(const std::string& jsonStr);
  LevelData getLevel(int level) const;
};

class BiomeRegistry {
  std::unordered_map<std::string, BiomeData> _bySlug;
public:
  void loadFromJson(const std::string& jsonStr);
  BiomeData getBiome(const std::string& slug) const;
  std::vector<std::string> getBiomesForLevel(int level) const;
};
```

## Files to Create/Modify

### New files
- `src/data/game_data.h` — All registry types and load functions
- `src/data/game_data.cpp` — Implementation
- `src/data/test_game_data.cpp` — Unit tests

### JSON fixes (in `assets/data/`)
- `cards.json` — Expand shields to 9 entries, heals to 9, golds to 9; fix major-healing-potion value (6→7)
- `heroes.json` — Add `shield`, `starting_items`, `locked` fields; update descriptions to match original; keep 4 custom heroes as locked
- `items.json` — Add `invisibility-cloak` and `monster-eye`; fix `monster-egg` type to `"drop"`
- `levels.json` — Add `heal_rate`, `nb_cards_by_row`, `monster_values`; remove `cols`
- `biomes.json` — Add `level` field

### Integration (in existing files)
- `src/logic/board_gen.cpp` — After `makeCard()`, no changes needed (registry enriches externally)
- `src/screens/game_screen.cpp` — Call `enrichBoard()` after `generateBoard()` to fill sprite data on all cards
- `src/screens/home_screen.cpp` — Use `HeroRegistry` to populate hero sprite coords in `buildHeroGrid()`
- `Makefile` — Add `src/data/game_data.cpp` to SRC

## Implementation Steps

### Step 1: Fix JSON data files
1. Update `cards.json` — expand shields/heals/golds to 9 entries each, fix major-healing-potion
2. Update `heroes.json` — add shield/starting_items/locked, fix descriptions, keep custom heroes locked
3. Update `items.json` — add invisibility-cloak and monster-eye, fix monster-egg type
4. Update `levels.json` — add heal_rate, nb_cards_by_row, monster_values; remove cols
5. Update `biomes.json` — add level field

### Step 2: Create data layer
1. Create `src/data/game_data.h` with registry types and free functions
2. Create `src/data/game_data.cpp` with JSON parsing using nlohmann/json
3. Implement `CardRegistry`, `ItemRegistry`, `HeroRegistry`, `LevelRegistry`, `BiomeRegistry`
4. Implement `loadAllRegistries()` convenience function
5. Implement `enrichBoard()` — post-processes board to fill sprite data on all Card slots

### Step 3: Integrate with existing code
1. `game_screen.cpp` — Load registries in `onEnter()`, call `enrichBoard()` after `generateBoard()`
2. `home_screen.cpp` — Use `HeroRegistry` for hero sprite data in `buildHeroGrid()`
3. `Makefile` — Add `src/data/game_data.cpp`

### Step 4: Write tests
1. Test JSON parsing for each registry
2. Test fallback behavior (missing JSON → defaults)
3. Test card lookup: `getCard(CardType::Shield, 7)` returns reinforced-shield with correct sprite
4. Test monster lookup: `getMonster("forest", 5)` returns correct entry
5. Test hero lookup: `getHero("warrior")` returns shield=10, starting_items=[shiny-shield,emerald]

## Constraints
- `game::data::` namespace
- `#pragma once` header guard
- nlohmann/json (system package: `nlohmann-json3-dev`)
- Graceful fallback to hardcoded defaults if JSON missing or malformed
- Follow `specs/raylib-cpp/05-coding-style.md`
- Logic layer (`game::logic::`) must not depend on data layer — enrichment happens at screen level

## Acceptance Criteria
- [ ] Card sprites render from cards.png atlas (shields, heals, golds, monsters)
- [ ] Hero sprites render in hero panel and home grid
- [ ] Item sprites display correctly in inventory
- [ ] Shield/heal/gold cards have uniform value distribution (1-9 equally likely)
- [ ] Monster cards respect biome + value lookup
- [ ] 4 original heroes have correct stats (warrior shield=10, starting items)
- [ ] 4 custom heroes are locked by default
- [ ] Fallback works: missing JSON → hardcoded defaults
- [ ] All unit tests pass

## Notes
- Verified card spawning mechanism against original JS: shields/heals/golds use `_.sample()` (uniform random across all 9 entries). Monsters use value-first lookup by biome+value.
- The data layer enriches cards AFTER board generation — `board_gen.cpp` creates skeleton cards with slug+type+value, then `enrichBoard()` fills sprite data.
- nlohmann/json 3.10.5 is installed as system package on Ubuntu 22.04.
