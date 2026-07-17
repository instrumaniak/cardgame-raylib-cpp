# spec: turn-flow

## Goal
Define the complete turn sequence from hero drop to next turn, including all status transitions.

## Requirements
1. Player drops hero onto a slot (drag-and-drop)
2. Validates: slot has card, slot is on current row, position difference <= 1
3. Resolve card effect based on type
4. Apply carrot effect (+1 HP if moved to different position)
5. Cap HP at max, clamp life/shield to >= 0
6. Check for monster in row below (passive attack)
7. If monster found: wait 960ms, spawn loot, replace monster card
8. If no monster: wait 500ms, advance to next turn
9. Handle death (athelas or lose)

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Status machine must match original exactly

## Pseudocode

### Complete Turn Sequence
```
function play_card(hero, board, row, position):
    // Step 1: Validate drop
    slot = board[row].slots[position]
    if slot.card == null: return error
    if row != current.row: return error
    if abs(position - current.position) > 1: return error
    
    // Step 2: Resolve card effect
    card = slot.card
    switch card.type:
        case "monster": resolveActiveMonster(card, hero)
        case "shield": resolveShieldCard(card, hero, hero.bag)
        case "heal": resolveHealing(card, hero, hero.bag)
        case "gold": resolveGoldCard(card, hero, hero.bag)
        case "item":
            if card.allow_multiple OR !hasItem(hero.bag, card.slug):
                hero.bag.push(card)
            checkMonsterEggCombo(hero.bag)
        case "drop":
            hero.bag.push(card)
            checkMonsterEggCombo(hero.bag)
    
    // Step 3: Apply carrot effect
    if hasItem(hero.bag, "carrot") AND position != current.position:
        hero.life += 1
    
    // Step 4: Cap and clamp
    maxHP = hero.life_max + getHeroLifeMaxModifier(hero.bag)
    hero.life = min(hero.life, maxHP)
    hero.life = max(hero.life, 0)
    hero.shield = max(hero.shield, 0)
    
    // Step 5: Card type branching
    switch card.type:
        case "biome": next_level()  // advance to next level/biome
        case "place": enter_place()  // enter chest sub-location
        case "end":
            status = "win"
            addGoldToAccount(hero.gold)
            return
        default: check_monster_play()
    
    // Step 6: Check monster in row below
    check_monster_play(hero, board, row, position)
```

### Check Monster Play
```
function check_monster_play(hero, board, row, position):
    nextRow = row + 1
    if nextRow >= board.length:
        next_turn()
        return
    
    monster = board[nextRow].slots[position].card
    if monster != null AND monster.type == "monster":
        // Check invisibility-cloak
        if hasItem(hero.bag, "invisibility-cloak"):
            if hero.life % 2 == 0:
                next_turn()
                return
        
        // Passive monster attack
        resolveActiveMonster(monster, hero)
        
        // Apply ring-of-courage AFTER passive attack
        if hero.life == 1 AND hasItem(hero.bag, "ring-of-courage"):
            for each item in hero.bag:
                if item.slug == "ring-of-courage": hero.shield += 5
        
        status = "monster-attack"
        
        // After animation (960ms), spawn loot
        // loot_rates = base rates + horn(+20 heal) + scales(+20 shield) + fossil(+10 monster)
        loot = spawnLoot(loot_rates)
        board[nextRow].slots[position].card = loot  // loot replaces monster
        
        next_turn()
    else:
        next_turn()
```

### Next Turn
```
function next_turn(hero):
    // Check if life <= 0
    if hero.life <= 0:
        if hasItem(hero.bag, "athelas"):
            hero.life = 1
            removeFirst(hero.bag, "athelas")
        else:
            die()
            return
    
    // Advance row
    current.row += 1
    status = "ready"  // after 500ms delay
```

### Die
```
function die(hero, account):
    status = "lose"
    reward = Math.round(hero.gold / 2)
    account.gold += reward
    saveAccount(account)
```

### Status Transitions
```
not-started -> (init_game) -> not-ready -> (init_draw) -> ready
ready -> (drop hero) -> hero-drop -> (check_monster_play)
  -> monster-attack -> (resolve monster) -> ready
  -> ready (no monster)
hero-drop -> (biome card) -> next_level -> ready
hero-drop -> (place card) -> enter_place -> ready (in sub-location)
hero-drop -> (end card) -> win
hero-drop -> (death) -> die() -> lose
```

## Acceptance Criteria
- [ ] Drop validates: slot has card, correct row, position diff <= 1
- [ ] Card effect resolved based on type
- [ ] Carrot: +1 HP when moving to different position
- [ ] Ring-of-courage: +5 shield when life == 1 (after passive monster attack)
- [ ] HP capped at max, life/shield clamped to >= 0
- [ ] Biome card triggers next_level
- [ ] Place card triggers enter_place
- [ ] End card triggers win
- [ ] Monster in row below triggers passive attack
- [ ] Invisibility-cloak dodges passive attack when life even
- [ ] Athelas prevents death once (life=1, remove from bag)
- [ ] Death: status="lose", reward = gold/2
- [ ] Status machine matches original exactly
