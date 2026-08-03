# spec: raylib-architecture

## Goal
Define the core architecture: Screen Manager (State Pattern), Resource Manager (Flyweight Pattern), entity composition via plain structs, layout system, input handling, and screen/game state machines.

## Requirements
1. Screen Manager: base class with lifecycle (onEnter/onExit/update/draw), ScreenManager dispatches
2. Resource Manager: string-keyed cache for textures/sounds/fonts (Flyweight)
3. Entity composition: plain structs that embed reusable data types (has-a)
4. Layout: RenderTexture at 960x540 design resolution, letterbox scaling to window
5. Input: virtual mouse coordinates, F11/Alt+Enter fullscreen toggle
6. Screen states: Home, Game, Win, Lose via ScreenID enum
7. Game status: 7 sub-states inside Game screen (notStarted → lose)
8. Account: save/load via nlohmann/json (./save.json)

## Constraints
- No ECS, no dynamic_cast, no virtual dispatch for entities
- ResourceManager owns all loaded resources (no raw pointers to raylib resources scattered)
- Screen class uses virtual dispatch (only 4 screens — trivial overhead)
- Game logic is pure — no raylib dependency in logic/ directory
- All rendering uses BeginTextureMode(renderTexture) → DrawTexturePro for letterbox
- All public APIs must be inside namespaces (no global-scope types or functions)

## Namespace Structure

```
game::           — core types, enums, components, entities (types.h, components.h, entities.h, screen.h, resource.h, layout.h, input.h)
game::logic::    — pure game logic (board_gen.h, card_effects.h, combat.h, items.h, account.h)
game::render::   — drawing functions (card_draw.h, board_draw.h, hero_panel.h, hud_draw.h, menu_draw.h)
game::anim::     — easing and animation (easing.h, anim.h)
game::data::     — JSON deserialization (game_data.h)
```

## Screen Manager (State Pattern)

```cpp
// src/core/screen.h
#pragma once
#include <array>
#include <memory>

namespace game {

enum class ScreenID { Home, Game, Win, Lose };

class Screen {
public:
    virtual ~Screen() = default;
    virtual ScreenID id() const = 0;
    virtual void onEnter() {}
    virtual void onExit() {}
    virtual void update(float dt) = 0;
    virtual void draw() = 0;
};

class ScreenManager {
    std::array<std::unique_ptr<Screen>, 4> _screens;
    Screen* _current = nullptr;   // non-owning observer, owned by _screens
    Screen* _next = nullptr;      // non-owning observer
public:
    void init();                    // construct all screens
    void switchTo(ScreenID id);     // queue transition for next frame
    void update(float dt);
    void draw();
    ScreenID currentId() const;
};

} // namespace game
```

**Derived screen classes must use `override` on all virtual function overrides:**

```cpp
class HomeScreen : public Screen {
public:
    ScreenID id() const override;
    void onEnter() override;
    void update(float dt) override;
    void draw() override;
};
```

Transition behavior: `switchTo()` sets `_next`, at start of next `update()` it calls `_current->onExit()`, sets `_current = _next`, calls `_current->onEnter()`.

## Screen Lifecycle

| Screen | onEnter | onExit | update | draw |
|--------|---------|--------|--------|------|
| Home | Build hero grid UI state | None | Check button clicks | menuDraw title, heroes, gold |
| Game | Generate board, init player | None | GameStatus machine step | boardDraw, heroPanel, hud |
| Win | Calculate rewards | Save account | Check Play Again input | Win overlay with stats |
| Lose | Calculate half rewards | Save account | Check Try Again input | Lose overlay with stats |

## Game Status Sub-States (inside Game screen)

```
NotStarted → (init game) → NotReady → (draw board) → Ready
Ready → (drop hero) → HeroDrop → (check) → MonsterAttack (if monster next row)
                                             → Ready (no monster)
HeroDrop → (biome card) → new level → NotReady
HeroDrop → (place card) → enter chest → NotReady
HeroDrop → (end card) → Win
HeroDrop → (death) → Lose
```

## Resource Manager (Flyweight Pattern)

```cpp
// src/core/resource.h
#pragma once
#include <string>
#include <unordered_map>
#include "raylib.h"

namespace game {

class ResourceManager {
    std::unordered_map<std::string, Texture2D> _textures;
    std::unordered_map<std::string, Sound> _sounds;
    std::unordered_map<std::string, Font> _fonts;
public:
    void loadAll();                 // load from assets/img/, assets/audio/, assets/fonts/
    void unloadAll();               // UnloadTexture/UnloadSound/UnloadFont on all

    const Texture2D& tex(const std::string& key) const;
    const Sound& snd(const std::string& key) const;
    const Font& fnt(const std::string& key) const;

    bool hasTex(const std::string& key) const;
};

} // namespace game
```

Keys match filenames without extension (e.g. `"cards"`, `"hit"`, `"pear-soda"`, `"card-frame-purple"`).

## Layout System

```cpp
// src/core/layout.h
#pragma once
#include "raylib.h"

namespace game {

struct Layout {
    RenderTexture renderTexture;   // 960x540
    Rectangle dstRect;             // computed letterbox rect in window coords
    float scaleX;
    float scaleY;

    void init();
    void compute(int windowWidth, int windowHeight);
    Vector2 virtualToScreen(Vector2 virt) const;    // virtual → window
    Vector2 screenToVirtual(Vector2 screen) const;  // window → virtual
};

} // namespace game
```

## Entity Composition (Plain Structs)

```cpp
// src/core/components.h — reusable data types
#pragma once
#include <vector>
#include <string>

namespace game {

struct HealthData    { int current; int max; };
struct ShieldData    { int value; };
struct GoldData      { int amount; };
struct InventoryData { std::vector<Card> items; int capacity; };
struct PositionData  { int row; int col; };

struct AnimState {
    float t = 0.f;           // current time (0..duration)
    float duration = 0.f;
    bool loop = false;

    void start(float dur, bool lp = false);
    void update(float dt);
    [[nodiscard]] float value() const;  // normalized 0..1
    [[nodiscard]] bool isActive() const { return t < duration; }
    [[nodiscard]] bool isDone() const { return t >= duration; }
};

} // namespace game
```

## Entity Structs

```cpp
// src/core/entities.h — entity structs that compose components
#pragma once
#include "core/components.h"
#include "core/types.h"

namespace game {

struct PlayerState {
    HealthData health;
    ShieldData shield;
    GoldData gold;
    InventoryData inventory;
    std::string heroSlug;
    bool athelasUsed = false;
};

struct CardSlot {
    Card card;
    bool revealed;
};

struct VisualEffect {
    EffectType type;
    AnimState anim;
    float x, y;
    float value;             // for numeric display (e.g. "+5 HP")
};

} // namespace game
```

## Core Constants

```cpp
// src/core/constants.h — immutable game-wide compile-time constants
#pragma once

namespace game {

// Display
constexpr int DESIGN_WIDTH = 960;
constexpr int DESIGN_HEIGHT = 540;
constexpr int INITIAL_WINDOW_WIDTH = 1280;
constexpr int INITIAL_WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

// Board layout (virtual pixels)
constexpr int CARD_WIDTH = 80;
constexpr int CARD_HEIGHT = 112;
constexpr int ROW_HEIGHT = 120;
constexpr int SLOTS_PER_ROW = 3;

} // namespace game
```

## Core Types

```cpp
// src/core/types.h — core enums and data structs
#pragma once
#include <array>
#include <vector>
#include <string>

namespace game {

enum class ScreenID   { Home, Game, Win, Lose };
enum class GameStatus { NotStarted, NotReady, Ready, HeroDrop, MonsterAttack, Win, Lose };
enum class CardType   { Monster, Shield, Heal, Gold, Item, Drop, Place, Biome, EndCard };
enum class Biome      { Forest, Cave, EnchantedLands, Desert, Abyss, Underworld };
enum class EffectType { DamageFlash, HealSparkle, CardFlip, GoldCoin, FogFade, ScreenShake };
enum class Sfx        { Hit, Shield, Equip, DealCard, Chest, Coins, Lose, Win, None };

struct Card {
    std::string slug;
    CardType cardType;
    int value;
    std::string name;
    std::string description;
    int spriteX, spriteY, spriteW, spriteH;  // region in cards.png
};

struct Hero {
    std::string slug;
    std::string name;
    std::string description;
    int lifeMax;
    int cost;
    // Starting inventory slugs
    std::vector<std::string> startingItems;
    int spriteX, spriteY;
    bool locked;
};

struct LevelConfig {
    int rows;
    std::array<int, 3> cardsPerRow;   // probability for 1/2/3 cards
    int monsterRate;
    int shieldRate;
    int healRate;
    int goldRate;
    int itemRate;
    std::vector<int> monsterValues;   // possible monster damage values
};

struct CardEffect {
    int damage;
    int healed;
    int goldEarned;
    int shieldGained;
    std::string message;
    Sfx soundEffect;
};

struct Button {
    Rectangle bounds;       // in virtual coords
    std::string text;
    bool hovered;
    std::string textureKey;
};

} // namespace game
```

## Game State (global, passed by reference)

```cpp
// Aggregates all runtime state for the game screen
struct GameState {
    PlayerState player;
    std::vector<std::vector<CardSlot>> board;
    GameStatus status;
    int currentLevel;
    int currentRow;
    int currentCol;
    int goldEarned;
    std::vector<std::string> visitedBiomes;
    std::string currentBiome;
    // Chest sub-location
    bool inChest;
    int chestRowsRemaining;
    std::string chestType;     // "chest" or "goldenChest"
    // Visual effects
    std::vector<VisualEffect> effects;
};
```

## Account Data

```cpp
// Persisted to save.json
struct HeroSave {
    std::string slug;
    bool locked;
    int life;
    int lifeMax;
    int shield;
    int gold;
    std::vector<std::string> bag;   // item slugs
    int cost;
};

struct Account {
    int gold;
    std::vector<HeroSave> heroes;
    std::string language;           // "en" for v1

    bool unlockHero(const std::string& slug);
    bool spendGold(int amount);
    void addGold(int amount);
    void load(const std::string& path);
    void save(const std::string& path) const;
};
```

## main.cpp Structure

```cpp
#include "core/resource.h"
#include "core/layout.h"
#include "core/screen.h"

int main() {
    SetConfigFlags(FLAG_WINDOW_RESIZABLE);
    InitWindow(1280, 720, "Drop the Volunteer");
    SetTargetFPS(60);

    game::ResourceManager res;
    res.loadAll();

    game::Layout layout;
    layout.init();

    game::ScreenManager screens;
    screens.init();     // constructs all 4 screens, passing res/layout refs
    screens.switchTo(game::ScreenID::Home);

    while (!WindowShouldClose()) {
        float dt = GetFrameTime();
        handleGlobalInput();      // F11 fullscreen
        layout.compute(GetScreenWidth(), GetScreenHeight());

        screens.update(dt);
        screens.draw();
    }

    res.unloadAll();
    CloseWindow();
    return 0;
}
```

## Directory Responsibilities

| Directory | Responsibility |
|-----------|---------------|
| src/core/ | Screen Mgr, Resource Mgr, Layout, Input, types, components, entities |
| src/screens/ | Screen implementations (one per screen) |
| src/data/ | JSON deserialization (nlohmann/json) |
| src/logic/ | Pure game logic — no raylib includes |
| src/render/ | Drawing functions — take ResourceManager by const ref |
| src/input/ | Input abstraction |
| src/anim/ | Animation interpolation + easing functions |

## Acceptance Criteria
- [ ] All public APIs are inside `game::` or sub-namespaces
- [ ] ScreenManager transitions correctly (exit → enter) on switch
- [ ] Each screen draws only during its active period
- [ ] ResourceManager loads all assets and unloads on exit
- [ ] ResourceManager returns const refs to shared resources (Flyweight)
- [ ] Entity structs use composition (has-a) for HealthData, ShieldData, etc.
- [ ] Layout computes correct letterbox dstRect for any aspect ratio
- [ ] virtualToScreen / screenToVirtual round-trip correctly
- [ ] GameStatus transitions match spec/game/10-ui-screens.md exactly
- [ ] Account loads/saves from save.json correctly
- [ ] main.cpp follows the pattern above
