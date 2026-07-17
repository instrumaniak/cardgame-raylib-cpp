# spec: combat-system

## Goal
Define damage resolution for active monster flips and passive monster attacks.

## Requirements
1. Active flip: damage = card.value + monster_card_modifier
2. Dragon-wing grants shield before absorption
3. Shield absorbs damage first, overflow hits life
4. Emerald grants +4 gold per emerald in bag
5. Ring-of-courage: +5 shield when life == 1
6. Passive attack: check row below for monster at same position
7. Invisibility-cloak: dodge when life is even
8. All item modifiers compose correctly

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Must be testable with any unit test framework

## Pseudocode

### Active Monster Flip
```
function resolveActiveMonster(card, hero):
    // Step 1: Calculate base damage
    damage = card.value + getMonsterCardModifier(hero.bag)
    
    // Step 2: Apply dragon-wing effect (gives shield)
    if hasItem(hero.bag, "dragon-wing"):
        hero.shield = 0
        for each item in hero.bag:
            if item.slug == "dragon-wing": hero.shield++
    
    // Step 3: Apply shield absorption
    if hero.shield > 0:
        remaining = hero.shield - damage
        if remaining < 0:
            hero.life = hero.life + remaining  // remaining is negative
            remaining = 0
        hero.shield = remaining
    else:
        hero.life = hero.life - damage
    
    // Step 4: Apply emerald effect
    if hasItem(hero.bag, "emerald"):
        for each item in hero.bag:
            if item.slug == "emerald": hero.gold += 4
    
    // Step 5: Apply ring-of-courage (last resort)
    if hero.life == 1 AND hasItem(hero.bag, "ring-of-courage"):
        bonus = 0
        for each item in hero.bag:
            if item.slug == "ring-of-courage": bonus += 5
        hero.shield += bonus
```

### Passive Monster Attack
```
function checkPassiveAttack(hero, rowBelow, position):
    // Step 1: Check invisibility-cloak
    if hasItem(hero.bag, "invisibility-cloak"):
        if hero.life % 2 == 0:  // dodge if life is EVEN
            return  // no attack
    
    // Step 2: Find monster in row below
    monster = rowBelow.slots[position].card
    if monster == null OR monster.type != "monster":
        return  // no attack
    
    // Step 3: Apply same damage as active flip
    resolveActiveMonster(monster, hero)
```

### Monster Card Modifier
```
function getMonsterCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "monster-skull": modifier += 1
    return modifier
```

## Acceptance Criteria
- [ ] damage = card.value + monster-skull count
- [ ] dragon-wing sets shield = dragon-wing count before absorption
- [ ] shield > damage: shield reduced, life unchanged
- [ ] shield < damage: shield = 0, life reduced by overflow (negative remaining)
- [ ] shield == 0: life reduced by full damage
- [ ] emerald grants +4 gold per emerald in bag
- [ ] ring-of-courage grants +5 shield per ring when life == 1
- [ ] passive attack checks row below at same position
- [ ] invisibility-cloak dodges passive attack when life is even
- [ ] invisibility-cloak does NOT affect active flips
- [ ] all modifiers compose (multiple skulls, multiple emeralds, etc.)
