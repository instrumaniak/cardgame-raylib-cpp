# spec: ui-screens

## Goal
Define app-level states, game-level states, screen transitions, and UI component layout.

## Requirements
1. App has two screens: "home" and "game"
2. Game has 7 status states with defined transitions
3. Home screen: hero selection, language toggle, gold display
4. Game screen: board, hero card, inventory, health bar
5. Win/Lose screens show gold earned
6. Card type determines frame color and background

## Constraints
- Pure data definitions — no rendering logic
- No framework dependencies
- Status machine must match original exactly

## Data Definitions

### App-Level States
```
app.screen: "home" | "game"
```

### Game-Level States (current.status)
```
"not-started"    // Initial state before game begins
"not-ready"      // Game initialized but board not drawn yet
"ready"          // Board drawn, waiting for player action
"hero-drop"      // Hero has been dropped on a card, resolving
"monster-attack" // Monster from next row is attacking (animation)
"win"            // Game won (reached end card)
"lose"           // Game lost (hero died)
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

### Screen Transitions
```
home -> (select hero, start game) -> game screen
game -> win -> (new run) -> game / (back) -> home
game -> lose -> (new run) -> game / (back) -> home
```

### UI Components

| Component | Description |
|-----------|-------------|
| Hero | Hero card display (slug, life, shield) |
| Board | Game board with rows/slots |
| BoardSlot | Individual card slot |
| Card | Card face display |
| Inventory | Bag/inventory display |

### Screens

| Screen | Content |
|--------|---------|
| home | Hero selection, language toggle |
| game | Main gameplay |
| win | Victory screen (shows gold earned) |
| lose | Death screen (shows gold/2 earned) |

### Card Type to Frame Color

| Type | Frame Image | BG Color |
|------|-------------|----------|
| monster | card-frame-purple | #53428d |
| heal | card-frame-red | #d94743 |
| shield | card-frame-gray | #616161 |
| gold | card-frame-yellow | #fbc02d |
| item | hero-frame | dark bg |
| drop | hero-frame | dark bg |
| place | chest-frame | #f3d19f |
| biome | hero-frame | dark bg |
| end | light-frame | gold glow |

### Localization
- English (EN) and French (FR)
- Language stored in account config
- All UI strings bilingual

## Acceptance Criteria
- [ ] App screen: "home" or "game"
- [ ] Game status: 7 states with correct transitions
- [ ] Home screen shows hero selection and gold
- [ ] Game screen shows board, hero card, inventory
- [ ] Win screen shows full gold earned
- [ ] Lose screen shows half gold earned
- [ ] Card frame colors match type mapping
- [ ] Status transitions match original exactly
