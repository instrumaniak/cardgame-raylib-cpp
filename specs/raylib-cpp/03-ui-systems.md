# spec: raylib-ui-systems

## Goal
Implement all drawing functions: HOME screen (hero selection), GAME screen (board + hero panel + HUD), WIN and LOSE overlays. Each screen's draw() uses the ResourceManager and Layout to render correctly at any window size.

## Requirements
1. HOME screen: title, hero selection grid, start button, gold display
2. GAME screen: board grid with fog, hero panel, health bar, HUD, inventory
3. Card rendering: correct frame color/sprite per type, face-up/down states
4. WIN/LOSE screens: title, stats (gold earned, total cards), action buttons
5. All drawing uses BeginTextureMode(renderTexture) at 960x540, then DrawTexturePro to window
6. Button hit-testing uses virtual mouse coordinates
7. Health bar: color-coded (green > 60%, yellow > 30%, red)

## Constraints
- Drawing functions take ResourceManager by const ref for texture/sound access
- No game logic in render files — drawing only
- Card sprites come from cards.png atlas, sliced by src rect
- Frame overlays are separate PNGs drawn on top of card sprite
- Fog rows show card-back.png with dark overlay
- All public APIs must be inside `game::render::` namespace
- Button struct is defined in `core/types.h` (inside `game::` namespace)

## Typography

All text rendering goes through a centralized typography system. No raw `DrawText()` or `DrawTextEx()` calls in render code — use `render::drawText()` and `render::measureText()` instead. This matches the original game's CSS where fonts and sizes are defined once and inherited.

### Font Assignment (matches original game CSS)

| Font | Role | Source |
|------|------|--------|
| PearSoda | Display/decorative: card stats, hero names, end-game titles | `assets/fonts/PearSoda.ttf` |
| Zepto-Regular | Body/UI: all other text (labels, descriptions, buttons) | `assets/fonts/Zepto-Regular.ttf` |

### Text Styles

| TextStyle | Font | Size | Use Cases |
|-----------|------|------|-----------|
| `Title` | PearSoda | 64px | End-game titles ("Victory!", "Defeat") |
| `Heading` | Zepto | 24px | Subtitles, section headers, unlock button text |
| `Body` | Zepto | 16px | Default text, descriptions, hero name, tooltips |
| `CardStat` | PearSoda | 32px | Card values (HP, shield, damage), hero stats |
| `Label` | Zepto | 12px | Small UI text, inventory labels, effect text |

### API

```cpp
// src/core/typography.h
#pragma once
#include "core/resource.h"
#include "raylib.h"

namespace game::render {

enum class TextStyle {
  Title,     // PearSoda 64px — end-game titles
  Heading,   // Zepto 24px — subtitles, section headers
  Body,      // Zepto 16px — default text
  CardStat,  // PearSoda 32px — card values, hero stats
  Label,     // Zepto 12px — small UI text
};

void drawText(const ResourceManager& res, TextStyle style,
              const char* text, float x, float y, Color color);

Vector2 measureText(const ResourceManager& res, TextStyle style,
                    const char* text);

} // namespace game::render
```

### Usage Pattern

```cpp
// WRONG — raw DrawText with magic numbers:
DrawText(hero.name.c_str(), x, y, 20, WHITE);
DrawText("Inventory", x, y, 12, WHITE);

// CORRECT — typed, consistent:
render::drawText(res, TextStyle::Body, hero.name.c_str(), x, y, WHITE);
render::drawText(res, TextStyle::Label, "Inventory", x, y, WHITE);
```

### Text Outline (for card stats)

Card stat numbers use an 8-directional outline effect for legibility over varied card backgrounds. This matches the original game's CSS text-shadow on `.card .card-front .value` (8-directional parchment outline in `#f4e3cb`). Implement as a separate `drawTextOutlined()` function or handled internally by `drawText()` when style is `CardStat`.

## Card Rendering

```cpp
// src/render/card_draw.h
#pragma once
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawCard(const ResourceManager& res, const Card& card,
              float x, float y, float w, float h, bool revealed);
void drawCardBack(const ResourceManager& res, float x, float y, float w, float h);

} // namespace game::render
```

### Card Layout (per slot, ~80x112 virtual pixels)

1. Background fill (Color based on cardType)
2. Card frame PNG on top (card-frame-*.png or hero-frame/chest-frame/light-frame)
3. Card sprite region from cards.png clipped inside frame
4. If revealed: card name text at top, value text at bottom
5. If face-down: card-back.png overlay

### Frame Colors

| Type    | Frame            | BG Color  |
|---------|------------------|-----------|
| Monster | card-frame-purple | #53428d  |
| Heal    | card-frame-red    | #d94743  |
| Shield  | card-frame-gray   | #616161  |
| Gold    | card-frame-yellow | #fbc02d  |
| Item    | hero-frame        | dark      |
| Drop    | hero-frame        | dark      |
| Place   | chest-frame       | #f3d19f  |
| Biome   | hero-frame        | dark      |
| EndCard | light-frame       | gold glow |

## Board Drawing

```cpp
// src/render/board_draw.h
#pragma once
#include <vector>
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawBoard(const ResourceManager& res, const std::vector<std::vector<CardSlot>>& board,
               int currentRow, float x, float y,
               const std::vector<Card>& items);

} // namespace game::render
```

### Board Layout

- Centered in the right ~70% of the screen
- Each row: 3 card slots equally spaced
- Row height: ~120 virtual pixels
- Fog rule (spec/game/08):
  - `currentRow` and below: revealed (card face visible)
  - `currentRow + 1` to `currentRow + 1 + nbRowsHidden`: show card-back
  - Below that: not drawn (black)
  - Active row (where hero is dropping): highlight glow
- Chest sub-locations: draw as overlays with "chest" header

## Hero Panel

```cpp
// src/render/hero_panel.h
#pragma once
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawHeroPanel(const ResourceManager& res, const PlayerState& player,
                   float x, float y);

} // namespace game::render
```

### Panel Layout (left ~30% of screen)

- Hero card art (from hero sprite region in cards.png)
- Hero name
- Health bar (horizontal, color-coded):
  - Full bar background (dark gray)
  - Fill width = (current/max) * barWidth
  - Color: green > 60%, yellow > 30%, red ≤ 30%
  - Current/ Max text
- Shield indicator (blue bar or icon + value)
- Coin icon + gold amount
- Inventory: horizontal scroll of item icons (small 32x32)

## HUD Drawing

```cpp
// src/render/hud_draw.h
#pragma once
#include <vector>
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawHud(const ResourceManager& res, int level, int goldEarned,
             const std::vector<VisualEffect>& effects, float dt);

} // namespace game::render
```

### HUD Elements

- Level indicator: top-left "Level X"
- Gold earned this run: top-right
- Screen shake: offset all draw calls by random(-intensity, +intensity) for duration

## Menu Drawing (HOME Screen)

```cpp
// src/render/menu_draw.h
#pragma once
#include <vector>
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawTitle(const ResourceManager& res, float screenWidth);
void drawHeroGrid(const ResourceManager& res, const std::vector<Hero>& heroes,
                  const Account& account, float x, float y,
                  int selectedIndex);
void drawStartButton(const ResourceManager& res, const Button& btn);
void drawGoldDisplay(const ResourceManager& res, int gold);

} // namespace game::render
```

### HOME Screen Layout

- Title "Drop the Volunteer" centered, big (logo image or PearSoda Title style)
- 8 hero cards in 2 rows × 4 columns grid
  - Each shows hero art, name, stats (HP, shield)
  - Locked heroes show lock icon + cost
  - Selected hero gets highlight border
- Gold display: top-right coin icon + amount
- Start button centered below hero grid
- Locked hero click → prompt unlock (if enough gold)

## WIN/LOSE Screens (drawn as overlays in game_screen::draw)

```cpp
// src/screens/win_screen.h and lose_screen.h
#pragma once
#include "core/types.h"
#include "core/resource.h"

namespace game::render {

void drawWinOverlay(const ResourceManager& res, int goldEarned, int totalGold,
                    int score, float screenW, float screenH);
void drawLoseOverlay(const ResourceManager& res, int goldEarned, int totalGold,
                     int score, float screenW, float screenH);

} // namespace game::render
```

### WIN Screen

- Semi-transparent dark overlay over game screen
- "Victory!" title (large, gold, TextStyle::Title)
- Stats: Hero name, Gold earned, Total gold, Score
- "Play Again" button → new game (same hero)
- "Main Menu" button → HOME screen

### LOSE Screen

- Same layout as WIN
- "Defeat" title (large, red, TextStyle::Title)
- Gold earned shown as half
- "Try Again" button → new game (same hero)
- "Main Menu" button → HOME screen

## Screen Drawing Flow

```
Screen::draw() for each screen:

HomeScreen::draw():
    BeginTextureMode(renderTexture)
        ClearBackground(darkBlue)
        render::drawTitle()
        render::drawHeroGrid()
        render::drawStartButton()
        render::drawGoldDisplay()
    EndTextureMode()
    DrawTexturePro(renderTexture.texture, dstRect, white)

GameScreen::draw():
    BeginTextureMode(renderTexture)
        ClearBackground(darkBg)
        render::drawBoard()
        render::drawHeroPanel()
        render::drawHud()
        // Win/Lose overlay if status == Win/Lose
        if status == Win: render::drawWinOverlay()
        if status == Lose: render::drawLoseOverlay()
    EndTextureMode()
    DrawTexturePro(...)
```

## Acceptance Criteria
- [ ] All render functions are inside `game::render::` namespace
- [ ] HOME screen renders title, hero grid (8 heroes), gold, start button
- [ ] Hero grid shows locked state (lock icon + cost for locked heroes)
- [ ] GAME screen renders board with correct card face-down/face-up by fog rules
- [ ] Card frames match type-to-color mapping
- [ ] Health bar is color-coded (green/yellow/red)
- [ ] Inventory shows item icons
- [ ] WIN overlay shows full gold; LOSE shows half gold
- [ ] Buttons respond to virtual mouse clicks
- [ ] All rendering is inside BeginTextureMode/EndTextureMode pair
- [ ] Screen shake offsets draw calls correctly
