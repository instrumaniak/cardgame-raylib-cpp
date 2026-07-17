# spec: healing-shields

## Goal
Define healing card resolution and shield card resolution with all item interactions.

## Requirements
1. Healing: heal = card.value + heal_card_modifier
2. Heal capped at max HP (life_max + hero_life_max_modifier)
3. Shield card value = card.value + shield_card_modifier
4. Ring-of-vitality: shields become healing instead
5. Cursed-shield: shields become additive (not replace)
6. Default: shield replaces existing shield value

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Shield value must never go negative

## Pseudocode

### Healing Resolution
```
function resolveHealing(card, hero, bag):
    // Step 1: Calculate heal amount
    heal = card.value + getHealCardModifier(bag)
    if heal < 0: heal = 0
    
    // Step 2: Apply heal
    hero.life = hero.life + heal
    
    // Step 3: Cap at max
    maxHP = hero.life_max + getHeroLifeMaxModifier(bag)
    if hero.life > maxHP:
        hero.life = maxHP
```

### Shield Card Resolution
```
function resolveShieldCard(card, hero, bag):
    // Step 1: Calculate shield value
    shield_value = card.value + getShieldCardModifier(bag)
    if shield_value < 0: shield_value = 0
    
    // Step 2: Apply based on items
    if hasItem(bag, "ring-of-vitality"):
        hero.life += shield_value  // shields become healing!
    elif hasItem(bag, "cursed-shield"):
        hero.shield = hero.shield + shield_value  // ADDITIVE (not replace)
    else:
        hero.shield = shield_value  // SET (replaces existing shield)
```

### Heal Card Modifier
```
function getHealCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "raw-meat": modifier += 1
        if item.slug == "rotten-meat": modifier -= 1
    return modifier
```

### Shield Card Modifier
```
function getShieldCardModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "shiny-shield": modifier += 1
        if item.slug == "rusty-shield": modifier -= 1
    return modifier
```

### Hero Life Max Modifier
```
function getHeroLifeMaxModifier(bag):
    modifier = 0
    for each item in bag:
        if item.slug == "omelette": modifier += 3
        if item.slug == "ring-of-vitality": modifier += 5
    if hasItem(bag, "cursed-shield"):
        modifier = -(hero.life_max - 1)  // max HP = 1
    return modifier
```

## Acceptance Criteria
- [ ] Heal = card.value + raw-meat count - rotten-meat count
- [ ] Heal minimum is 0 (never negative healing)
- [ ] Heal capped at life_max + hero_life_max_modifier
- [ ] Shield value = card.value + shiny-shield count - rusty-shield count
- [ ] Shield minimum is 0
- [ ] Ring-of-vitality: shield value added to life instead of shield
- [ ] Cursed-shield: shield added to existing shield (additive)
- [ ] Default: shield replaces existing shield
- [ ] Ring-of-vitality + cursed-shield interaction: ring-of-vitality takes priority
