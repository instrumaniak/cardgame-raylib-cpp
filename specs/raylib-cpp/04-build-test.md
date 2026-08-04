# spec: raylib-build-test

## Goal
Define testing strategy, build targets, sound integration, animation system, and polish requirements.

## Requirements
1. Unit tests for all pure game logic (board generation, card effects, combat, items, account, fog, level config, turn flow, easing, JSON data)
2. doctest as the test framework (system package)
3. Makefile `test` target compiles and runs tests
4. Sound effects: 8 SFX triggered at correct game events
5. Animations: card flip tween, damage flash, heal sparkle, screen shake
6. Build: release (`make`), debug (`make debug`), run (`make run`), clean (`make clean`)

## Constraints
- Logic tests compile WITHOUT raylib headers (link -lraylib only for main binary)
- Test files live with their tasks — each logic task creates its own `test_*.cpp` (T009→test_board_gen, T010→test_items, T011→test_combat, T012→test_card_effects, T013→test_account, T014→test_fog, T015→test_level_config, T016→test_turn_flow, T028→test_easing, T029→test_game_data)
- Each test_*.cpp uses `#include "doctest/doctest.h"` but only one test main
- No raylib dependency in tests — test pure functions only
- T026 is verification + integration only (no individual unit test creation)
- Sound: LoadSound from .wav files in assets/audio/
- Animations: CPU-side tweening with easing functions, no GPU shaders
- All public APIs must be inside appropriate namespaces

## Build Targets

```makefile
# Test configuration
TEST_CXXFLAGS := -std=c++17 -Wall -Wextra -g -O0 -I src
TEST_SRC      := src/test_main.cpp
TEST_LOGIC_SRC :=   # populated by per-task test files
TEST_TARGET   := build/run_tests

test: $(TEST_TARGET)
	./$(TEST_TARGET)

$(TEST_TARGET): $(TEST_SRC) $(TEST_LOGIC_SRC)
	$(CXX) $(TEST_CXXFLAGS) -o $@ $^ -lm

# Release build
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I src -I vendor/raylib/src
LDFLAGS   := -L vendor/raylib/src -lraylib -lm

# ... rest of build rules per 00-project-setup
```

## Test Structure

### Main Test File (test_main.cpp)

```cpp
#define DOCTEST_CONFIG_IMPLEMENT
#include "doctest/doctest.h"

int main(int argc, char** argv) {
    doctest::Context ctx;
    ctx.applyCommandLine(argc, argv);
    return ctx.run();
}
```

Note: `DOCTEST_CONFIG_IMPLEMENT` is defined in the source file, not via compiler flags, to avoid ODR violations when multiple test files include doctest.

### Test Files

Each logic task creates its own test file alongside the implementation:

**test_board_gen.cpp (T009):**
```cpp
#include "doctest/doctest.h"
#include "logic/board_gen.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("board generation basic dimensions") {
    LevelConfig config;
    config.rows = 5;
    // setup rates...
    std::mt19937 rng(42);  // seeded for deterministic test
    auto board = generateBoard(config, 1, {"Forest"}, rng);
    CHECK(board.slots.size() == 5);
    for (auto& row : board.slots) CHECK(row.size() == 3);
}

TEST_CASE("first row has no monsters") {
    // verify no monster cards in row 0
}

TEST_CASE("last row has biome or end card") {
    // level 6 → EndCard, else Biome
}

TEST_CASE("boss in second-to-last row center") {
    // verify slot[1] of row total-2 is Monster
}

TEST_CASE("chest placed in valid rows") {
    // chests in rows 0..total-4
}

TEST_CASE("weighted card rates per level") {
    // statistical test with large sample
}
```

**test_card_effects.cpp (T012):**
```cpp
#include "doctest/doctest.h"
#include "logic/card_effects.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("monster card deals correct damage") {
    Card monster{"1", CardType::Monster, 5, "", "", 0,0,0,0};
    PlayerState player{HealthData{15,15}, ShieldData{0}, GoldData{0}, InventoryData{{},10}, "headless-knight"};
    auto effect = resolveCard(monster, player, {});
    CHECK(effect.damage == 5);
    CHECK(effect.soundEffect == Sfx::Hit);
}

TEST_CASE("shield card adds shield value")
TEST_CASE("heal card restores HP capped at max")
TEST_CASE("gold card adds correct amount")
TEST_CASE("item card returns item slug")
TEST_CASE("place card returns chest type")
TEST_CASE("end card returns zero effect")
```

**test_combat.cpp (T011):**
```cpp
#include "doctest/doctest.h"
#include "logic/combat.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("shield absorbs damage") {
    ShieldData shield{5};
    auto result = applyToShield(3, shield);
    CHECK(result.value == 2);
}

TEST_CASE("shield overflow bleeds through")
TEST_CASE("invisibility cloak dodges on even HP")
TEST_CASE("monster skull modifier adds +1 per skull")
TEST_CASE("ring of courage gives shield at 1 HP")
TEST_CASE("cursed shield sets max HP to 1")
TEST_CASE("emerald adds gold on monster hit")
```

**test_items.cpp (T010):**
```cpp
#include "doctest/doctest.h"
#include "logic/items.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("monster egg combo triggers at 3 eggs") {
    std::vector<Card> items;
    for (int i = 0; i < 3; i++)
        items.push_back({"monster-egg", CardType::Drop, 0, "", "", 0,0,0,0});
    auto result = checkMonsterEggCombo(items);
    CHECK(result.triggered);
    CHECK(result.newItems.size() == 1);
    CHECK(result.newItems[0].slug == "omelette");
}

TEST_CASE("apple heals +10 on level change")
TEST_CASE("ring of protection adds +5 shield")
TEST_CASE("dragon wing regenerates shield after fight")
TEST_CASE("athelas prevents death once")
TEST_CASE("amethyst doubles gold and removes self")
```

**test_account.cpp (T013):**
```cpp
#include "doctest/doctest.h"
#include "logic/account.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("unlock hero deducts gold") {
    Account acc{100, {{"headless-knight", false, 15,15,0,0,{},0}}, "en"};
    CHECK(acc.spendGold(50));
    CHECK(acc.gold == 50);
}

TEST_CASE("unlock fails if insufficient gold")
TEST_CASE("save/load round-trip preserves data") {
    // write to stringstream, read back
}
```

**test_fog.cpp (T014):**
```cpp
#include "doctest/doctest.h"
#include "logic/board_gen.h"

using namespace game;
using namespace game::logic;

TEST_CASE("0 monster-eyes: all 3 rows visible") {
    CHECK(getNbRowsHidden(0) == 0);
}

TEST_CASE("1 monster-eye: 1 row fogged") {
    CHECK(getNbRowsHidden(1) == 1);
}

TEST_CASE("2+ monster-eyes: 2 rows fogged") {
    CHECK(getNbRowsHidden(2) == 2);
    CHECK(getNbRowsHidden(3) == 2); // capped at 2
}
```

**test_level_config.cpp (T015):**
```cpp
#include "doctest/doctest.h"
#include "logic/level_config.h"

using namespace game;
using namespace game::logic;

TEST_CASE("level 1 config matches spec") {
    auto config = getLevelConfig(1);
    CHECK(config.rows == 5);
    // verify rates, monster values...
}

TEST_CASE("all 6 levels have valid configs") {
    for (int level = 1; level <= 6; level++) {
        auto config = getLevelConfig(level);
        CHECK(config.rows > 0);
    }
}

TEST_CASE("chest configs match spec")
TEST_CASE("biome exclusion works correctly")
```

**test_turn_flow.cpp (T016):**
```cpp
#include "doctest/doctest.h"
#include "logic/turn_flow.h"
#include "core/types.h"

using namespace game;
using namespace game::logic;

TEST_CASE("status transitions: NotStarted → NotReady → Ready") {
    GameState state;
    // verify initial state machine progression
}

TEST_CASE("hero death transitions to Lose") {
    GameState state;
    state.player.health.current = 0;
    die(state);
    CHECK(state.status == GameStatus::Lose);
}

TEST_CASE("end card transitions to Win")
TEST_CASE("monster passive attack from row below")
TEST_CASE("chest sub-location entry/exit")
TEST_CASE("biome card transitions to new level")
```

**test_easing.cpp (T028):**
```cpp
#include "doctest/doctest.h"
#include "anim/easing.h"

using namespace game::anim;

TEST_CASE("lerp endpoints") {
    CHECK(lerp(0.f, 10.f, 0.f) == doctest::Approx(0.f));
    CHECK(lerp(0.f, 10.f, 1.f) == doctest::Approx(10.f));
    CHECK(lerp(0.f, 10.f, 0.5f) == doctest::Approx(5.f));
}

TEST_CASE("easeInOut boundary") {
    CHECK(easeInOut(0.f) == doctest::Approx(0.f));
    CHECK(easeInOut(1.f) == doctest::Approx(1.f));
}

TEST_CASE("easeOut boundary")
TEST_CASE("bounce returns 0 at t=0 and 1 at t=1")
```

**test_game_data.cpp (T029):**
```cpp
#include "doctest/doctest.h"
#include "data/game_data.h"

using namespace game::data;

TEST_CASE("loadHeroes from JSON") {
    auto heroes = loadHeroes("assets/data/heroes.json");
    CHECK(heroes.size() == 4);
}

TEST_CASE("fallback when JSON missing") {
    auto heroes = loadHeroes("nonexistent.json");
    CHECK(heroes.size() == 4); // hardcoded defaults
}

TEST_CASE("data matches spec tables")

## Sound Effects

| Sound File | Trigger | Notes |
|------------|---------|-------|
| hit.wav    | Monster attack resolved | Play at full volume |
| shield.wav | Shield card collected | |
| equip.wav  | Item equip / heal card | |
| deal-card.wav | Card flip animation starts | |
| chest.wav  | Chest sub-location entered | |
| coins.wav  | Gold card collected | |
| lose.wav   | Game status → Lose | One-shot, don't overlap |
| win.wav    | Game status → Win | One-shot, don't overlap |

```cpp
// Trigger pattern (inside game::render or game:: namespace)
void playSfx(const ResourceManager& res, Sfx sfx) {
    static const std::unordered_map<Sfx, std::string> sfxMap = {
        {Sfx::Hit, "hit"}, {Sfx::Shield, "shield"}, {Sfx::Equip, "equip"},
        {Sfx::DealCard, "deal-card"}, {Sfx::Chest, "chest"}, {Sfx::Coins, "coins"},
        {Sfx::Lose, "lose"}, {Sfx::Win, "win"}
    };
    const Sound& snd = res.snd(sfxMap.at(sfx));
    if (IsSoundPlaying(snd)) StopSound(snd);
    PlaySound(snd);
}
```

No BGM for v1.

## Animation System

```cpp
// src/anim/easing.h
#pragma once

namespace game::anim {

constexpr float lerp(float a, float b, float t) { return a + (b - a) * t; }
constexpr float easeInOut(float t) { return t < 0.5f ? 2*t*t : 1 - (-2*t+2)*(-2*t+2)/2; }
constexpr float easeOut(float t) { return 1 - (1-t)*(1-t); }
float bounce(float t);  // non-trivial, defined in .cpp

} // namespace game::anim
```

**AnimState** is defined in `core/components.h` (see 01-architecture.md) with methods:

```cpp
struct AnimState {
    float t = 0.f;
    float duration = 0.f;
    bool loop = false;

    void start(float dur, bool lp = false);   // start or restart animation
    void update(float dt);                     // advance time, handle loop
    [[nodiscard]] float value() const;         // normalized 0..1
    [[nodiscard]] bool isActive() const;       // t < duration
    [[nodiscard]] bool isDone() const;         // t >= duration
};
```

### Animations

| Animation | Duration | Easing | Implementation |
|-----------|----------|--------|----------------|
| Card flip | 0.3s | easeOut | Scale Y from 0→1 (scale X mid-flip to swap texture) |
| Damage flash | 0.2s | lerp | Tint hero panel red, fade to normal |
| Heal sparkle | 0.5s | easeOut | Particles emit from hero position (4-6 small circles) |
| Gold coin | 0.4s | bounce | Coin icon scales up then bounces to gold counter |
| Fog fade | 0.5s | easeInOut | Alpha of fog overlay from 0→255 |
| Screen shake | 0.3s | easeOut | Random offset (-3..+3 px), amplitude decays |

VisualEffect is defined in `core/entities.h` (see 01-architecture.md). It stores an `EffectType` enum, `AnimState`, position, and optional numeric value for display.

## Acceptance Criteria

### Build
- [ ] `make` compiles release binary with -O2
- [ ] `make debug` compiles with -g -O0 -DDEBUG
- [ ] `make run` launches the game
- [ ] `make clean` removes .o files and binary
- [ ] `make test` compiles and runs all doctest tests

### Tests
- [ ] All board generation tests pass (dimensions, first row no monster, boss, chests)
- [ ] All card effect tests pass (monster, shield, heal, gold, item, place, end)
- [ ] All combat tests pass (shield absorption, items, invisibility, rings)
- [ ] All item tests pass (22 items, monster-egg combo)
- [ ] All account tests pass (unlock, spend, save/load)
- [ ] All fog tests pass (0/1/2+ monster-eyes visibility)
- [ ] All level config tests pass (6 levels, chest configs, biome exclusion)
- [ ] All turn flow tests pass (status transitions, death, win, chest, biome)
- [ ] All easing tests pass (lerp, easeInOut, easeOut, bounce)
- [ ] All JSON data tests pass (parsing, fallback, spec match)
- [ ] Integration tests pass (combat + items interaction)
- [ ] Test compilation does not require -lraylib

### Sound
- [ ] All 8 SFX play at correct game events
- [ ] Sound does not overlap itself (StopSound before PlaySound)
- [ ] No audio artifacts on rapid triggers

### Animation
- [ ] Card flip animation visible and smooth
- [ ] Damage flash applies red tint for 0.2s
- [ ] Heal sparkle emits particles
- [ ] Screen shake offsets rendering for 0.3s
- [ ] Fog fades in/out smoothly
- [ ] All animations complete without visual glitches

### Polish
- [ ] No entity leaks on screen transitions
- [ ] Window resizes and letterboxes correctly
- [ ] Fullscreen toggle works (F11 / Alt+Enter)
- [ ] FPS counter optional (debug mode only)
- [ ] Window title shows "Drop the Volunteer"
