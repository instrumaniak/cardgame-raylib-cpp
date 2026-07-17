# spec: item-effects

## Goal
Define all item effects, the bag/inventory system, and item interactions.

## Requirements
1. All items go into the hero's bag (inventory)
2. Effects are passive and apply automatically
3. Some items allow multiples, some don't
4. Item effects modify game mechanics (damage, healing, shield, gold, spawn rates)
5. Monster-egg combo: 3 eggs → 1 omelette (+3 HP per omelette)

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Must handle all edge cases (stacking, capping, removal)

## Data Definitions

### Bag Items

| Slug | Allow Multiple | Effect |
|------|---------------|--------|
| `map` | No | Reveals the map sidebar (shows board layout) |
| `horn` | Yes | +20 to loot_types.heal spawn rate |
| `scales` | Yes | +20 to loot_types.shield spawn rate |
| `omelette` | Yes | +3 to hero_life_max_modifier per omelette |
| `athelas` | No | Prevents death once: sets life=1, removes from bag |
| `raw-meat` | Yes | +1 to heal_card_modifier (healing cards heal more) |
| `rotten-meat` | Yes | -1 to heal_card_modifier (healing cards heal less) |
| `rusty-shield` | Yes | -1 to shield_card_modifier, +20 to shield spawn rate |
| `shiny-shield` | Yes | +1 to shield_card_modifier, -20 to shield spawn rate |
| `apple` | Yes | +10 HP on level change, capped at max |
| `dragon-wing` | Yes | +1 shield after each fight when shield was depleted |
| `monster-skull` | Yes | +1 to monster_card_modifier (monsters deal MORE damage) |
| `carrot` | Yes | +1 HP when moving to different position |
| `fossil` | Yes | +10 to loot_types.monster spawn rate |
| `ring-of-vitality` | No | +5 to hero_life_max_modifier; shields become healing instead |
| `ring-of-protection` | Yes | +5 shield on level change, stackable |
| `ring-of-courage` | Yes | +5 shield when life == 1 after fight, stackable |
| `ring-of-greed` | Yes | -1 HP per gold card picked |
| `cursed-shield` | No | Sets hero_life_max_modifier to -(life_max - 1) (max HP = 1); shields become additive |
| `ruby` | Yes | On gold card: 25% chance of +1 HP |
| `emerald` | Yes | On monster hit: +4 gold per emerald in bag |
| `amethyst` | Yes | On gold card: doubles gold value; then removes itself from bag |

### Drop Items

| Slug | Effect |
|------|--------|
| `monster-egg` | Collect 3 to create `omelette` (special combo) |

## Pseudocode

### Modifier Calculations
```
function getMonsterCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "monster-skull": modifier += 1
    return modifier

function getHealCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "raw-meat": modifier += 1
        if item.slug == "rotten-meat": modifier -= 1
    return modifier

function getShieldCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "shiny-shield": modifier += 1
        if item.slug == "rusty-shield": modifier -= 1
    return modifier

function getNbRowsHidden(bag):
    count = 0
    for each item in bag:
        if item.slug == "monster-eye": count++
    return min(count, 2)

function getHeroLifeMaxModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "omelette": modifier += 3
        if item.slug == "ring-of-vitality": modifier += 5
    if hasItem(bag, "cursed-shield"):
        modifier = -(hero.life_max - 1)
    return modifier
```

### Monster-Egg Combo
```
function checkMonsterEggCombo(bag):
    eggCount = count(bag, "monster-egg")
    if eggCount >= 3:
        // Remove 3 eggs
        removed = 0
        newBag = filter(bag, item => {
            if item.slug == "monster-egg" AND removed < 3:
                removed++
                return false
            return true
        })
        // Add omelette
        newBag.push({slug: "omelette", type: "item", value: 0})
        return {combo: true, newBag: newBag}
    return {combo: false, newBag: bag}
```

### Apple Effect (on level change)
```
function applyAppleEffect(hero, bag):
    for each item in bag:
        if item.slug == "apple":
            hero.life += 10
            maxHP = hero.life_max + getHeroLifeMaxModifier(bag)
            hero.life = min(hero.life, maxHP)
```

### Ring-of-Protection Effect (on level change)
```
function applyRingOfProtection(hero, bag):
    for each item in bag:
        if item.slug == "ring-of-protection":
            hero.shield += 5
```

## Acceptance Criteria
- [ ] All 22 items have correct effects
- [ ] Allow-multiple flag respected (ring-of-vitality, athelas = single)
- [ ] Monster-card-modifier = monster-skull count
- [ ] Heal-card-modifier = raw-meat count - rotten-meat count
- [ ] Shield-card-modifier = shiny-shield count - rusty-shield count
- [ ] Nb-rows-hidden capped at 2
- [ ] Cursed-shield sets max HP to 1
- [ ] Monster-egg combo removes 3 eggs, adds omelette
- [ ] Apple heals +10 on level change, capped at max
- [ ] Ring-of-protection adds +5 shield on level change
