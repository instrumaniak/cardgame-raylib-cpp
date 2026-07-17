# spec: gold-scoring

## Goal
Define gold card resolution, item interactions with gold, and score calculation.

## Requirements
1. Gold card: hero.gold += card.value
2. Ruby: 25% chance of +1 HP per gold card
3. Amethyst: doubles gold value, then removes itself from bag
4. Ring-of-greed: -1 HP per gold card picked
5. Victory: full gold earned
6. Defeat: Math.round(gold / 2)
7. Score based on total cards flipped and gold

## Constraints
- Pure functions only — no I/O, no side effects
- No framework dependencies
- Amethyst removal is one-time (consumed on use)

## Pseudocode

### Gold Card Resolution
```
function resolveGoldCard(card, hero, bag):
    // Step 1: Add gold
    hero.gold += card.value
    
    // Step 2: Apply ruby effect
    if hasItem(bag, "ruby"):
        if random(0, 4) == 0:  // 25% chance
            hero.life += 1
    
    // Step 3: Apply amethyst effect (double gold)
    if hasItem(bag, "amethyst"):
        hero.gold += card.value  // double gold
    
    // Step 4: Apply ring-of-greed effect
    if hasItem(bag, "ring-of-greed"):
        hero.life -= 1
    
    // Step 5: Remove amethyst if present
    if hasItem(bag, "amethyst"):
        removeFirst(bag, "amethyst")
```

### Win/Lose Rewards
```
function calculateReward(gold, won):
    if won:
        return gold  // full gold
    else:
        return Math.round(gold / 2)  // half gold
```

### Score Calculation
```
function calculateScore(stats, won):
    multiplier = won ? 1.5 : 1.0
    totalCards = sum of all stats.cards.values
    return Math.round((totalCards + stats.cards.gold / 2) * multiplier)
```

## Acceptance Criteria
- [ ] Gold added = card.value
- [ ] Ruby: 25% chance of +1 HP (1 in 4 chance)
- [ ] Amethyst: gold doubled, then amethyst removed from bag
- [ ] Ring-of-greed: -1 HP per gold card
- [ ] Amethyst + ring-of-greed: both apply (double gold, lose 1 HP)
- [ ] Ruby + amethyst: both apply (25% +1 HP, double gold)
- [ ] Victory reward = full gold
- [ ] Defeat reward = Math.round(gold / 2)
- [ ] Score = (totalCards + gold/2) * multiplier
- [ ] Score multiplier = 1.5 for win, 1.0 for loss
