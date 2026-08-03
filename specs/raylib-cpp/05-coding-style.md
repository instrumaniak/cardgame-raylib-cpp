# spec: C++ Coding Style Guide

## 1. Overview and Philosophy

### Why This Style Exists

This codebase is written in C++17, but its style is intentionally borrowed from JavaScript/TypeScript conventions. The goal is **zero cognitive overhead** for a JS/TS developer reading C++ code.

### Core Principle

**Write C++ that looks like TypeScript.** If a JS/TS pattern has a direct C++ equivalent, use it. If not, prefer the simplest possible C++ construct.

### What This Guide Is Not

- Not trying to be idiomatic C++ (we diverge from Google/LLVM style on function naming)
- Not trying to be "maximum performance" style
- Readability first, performance second

### The JS/TS Developer Contract

Every JS/TS convention that maps to C++ is documented in this guide. Every C++ feature that has no JS/TS equivalent is explicitly called out with a comparison.

### Decision Rule

When in doubt, choose the option that a JS/TS developer would immediately recognize.

---

## 2. Naming Conventions

### Full Mapping Table

| C++ Construct | This Project | JS/TS Equivalent | Example |
|---|---|---|---|
| Files | `snake_case` | `camelCase.ts` or `kebab-case.ts` | `board_gen.cpp` |
| Directories | `snake_case` | `camelCase/` or `kebab-case/` | `src/logic/` |
| Classes | `PascalCase` | `PascalCase` | `ScreenManager` |
| Structs | `PascalCase` | `interface` / `type` | `GameState` |
| Functions | `camelCase` | `camelCase` | `generateBoard()` |
| Methods | `camelCase` | `camelCase` | `switchTo()` |
| Variables | `camelCase` | `camelCase` | `currentRow` |
| Parameters | `camelCase` | `camelCase` | `availableBiomes` |
| Immutable constants | `SCREAMING_SNAKE` | `SCREAMING_SNAKE` | `DESIGN_WIDTH`, `MAX_HAND_SIZE` |
| Configurable constants | `camelCase` | `camelCase` | `maxHp`, `defaultSpeed` |
| Private members | `_camelCase` | `_camelCase` | `_screens` |
| Boolean vars | `is`/`has`/`can` prefix | `is`/`has`/`can` prefix | `isActive`, `hasEndCard` |
| Enum type | `PascalCase` | `PascalCase` | `GameStatus` |
| Enum values | `PascalCase` | `PascalCase` | `NotStarted`, `HeroDrop` |
| Macros | `SCREAMING_SNAKE` | `SCREAMING_SNAKE` | `FLAG_WINDOW_RESIZABLE` |
| Template params | `PascalCase` | n/a | `typename T` |

### Rules

- **Functions and methods:** Always `camelCase`. This is the most visible convention change from typical C++.
- **Private member variables:** Leading underscore `_camelCase`. This mirrors JS/TS private naming convention.
- **Boolean getters:** Use `is`/`has`/`can`/`should` prefix. A function like `active()` becomes `isActive()`.
- **File names:** `snake_case.cpp` / `snake_case.h` (avoids case-sensitivity issues across OS).
- **Abbreviations:** Avoid. Write `screenWidth` not `screenW`. Write `ResourceManager` not `ResMgr`.
- **Constants:** Immutable compile-time constants (design resolution, max values, fixed dimensions) use `SCREAMING_SNAKE`. Configurable or context-dependent values use `camelCase`.

### Good Examples

```cpp
// Functions: camelCase
CardEffect resolveCard(const Card& card, const PlayerState& player);
void drawCard(const ResourceManager& res, const Card& card, float x, float y);

// Variables: camelCase
int currentRow = 0;
bool hasEndCard = false;
int baseDamage = 5;

// Private members: _camelCase
class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;
  Screen* _current = nullptr;
  Screen* _next = nullptr;
};

// Boolean prefix
bool isActive() const;
bool hasEndCard() const;
```

### Bad Examples (Anti-patterns)

```cpp
// snake_case function name
CardEffect resolve_card(const Card& card);  // wrong naming

// PascalCase function name
CardEffect ResolveCard(const Card& card);   // wrong naming

// Trailing underscore (deprecated)
std::unique_ptr<Screen> screens_;   // deprecated trailing underscore

// No boolean prefix
bool active() const;         // unclear - is it a state or action?
bool endCard() const;        // ambiguous
```

---

## 3. Formatting

### Rules

| Rule | Setting | JS/TS Comparison |
|---|---|---|
| Indentation | **2 spaces** (never tabs) | Same as JS/TS standard |
| Braces | **K&R / attached** (opening `{` on same line) | Same as JS/TS |
| Line length | **100 characters** (soft limit) | Same as JS/TS (Prettier default is 80) |
| Trailing commas | **Yes** | Same as JS/TS best practice |
| Semicolons | **Yes** (C++ requires them) | Same as JS/TS (with semicolons) |
| Spaces around operators | Yes: `a = b + c` | Same as JS/TS |
| Spaces after commas | Yes: `f(a, b, c)` | Same as JS/TS |
| Blank line after class opening | Yes | Common JS/TS style |
| Blank line between functions | Yes (in .cpp) | Same as JS/TS |

### Brace Style Detail

```cpp
// K&R: opening brace on same line (JS/TS standard)
if (condition) {
  doSomething();
} else {
  doOther();
}

// Functions in .cpp: same line
void resolveCard(const Card& card) {
  if (card.value > 0) {
    _currentHP -= card.value;
  }
}

// Classes/structs: same line
class ResourceManager {
public:
  void loadAll();
};
```

### Blank Line Rules

- One blank line between function definitions in `.cpp` files.
- One blank line after the opening `{` of a class body.
- No blank line between a function's opening brace and its first statement.
- One blank line between member variable groups (e.g., between private sections).

### Good Example

```cpp
// 2-space indent, K&R braces, trailing comma
void drawCard(const ResourceManager& res, const Card& card,
              float x, float y, float w, float h) {
  if (!isActive()) {
    return;
  }
  const auto& tex = res.tex("cards");
  DrawTextureRec(tex, card.spriteRect(), {x, y}, WHITE);
}
```

### Bad Example

```cpp
// 4-space indent (wrong)
void drawCard(const ResourceManager& res, const Card& card,
    float x, float y) {          // 4-space indent
    if (!isActive()) {           // 4-space indent
        return;
    }
}

// Allman braces (wrong - not JS/TS style)
void drawCard(const ResourceManager& res)
{                                // opening brace on new line
  // ...
}
```

---

## 4. Header Files

### Rules

- Use `#pragma once` (no `#ifndef` include guards).
- Headers contain: declarations only, struct/class definitions, function declarations, `using` type aliases, `constexpr` values.
- Headers do NOT contain: function bodies (except `constexpr` and inline trivial), `static` variables (except `constexpr`), `using namespace`.
- Include what you use. Every `.cpp` must include its own dependencies.

### Include Order

```cpp
// 1. Own header (if .cpp file)
#include "screen_manager.h"

// 2. Project headers
#include "core/types.h"
#include "core/resource.h"

// 3. Standard library
#include <string>
#include <vector>
#include <memory>

// 4. Vendor (raylib)
#include "raylib.h"
```

### Forward Declarations

- Prefer forward declarations in headers when possible (reduces compilation dependencies).
- Use forward declarations for pointer/reference parameters only.

### Good Example

```cpp
// card_effects.h
#pragma once

#include <string>
#include <vector>
#include "core/types.h"

CardEffect resolveCard(const Card& card, const PlayerState& player,
                       const std::vector<Card>& inventoryItems);
```

### Bad Example

```cpp
// card_effects.h
#ifndef CARD_EFFECTS_H     // old-style include guard
#define CARD_EFFECTS_H
// ...
#endif

#include "core/types.h"
#include <string>           // wrong order: system before project
#include <vector>
```

---

## 5. Classes and Structs

### Rule: Structs are interfaces, classes have behavior

- **`struct`**: All public members by default. Used for data-only types (the JS/TS `interface` equivalent). No methods except trivial getters.
- **`class`**: Has private state + public interface. Used for anything with behavior or encapsulation (the JS/TS `class` equivalent).

### JS/TS `interface` → C++ `struct`

```typescript
// JS/TS
interface GameState {
  currentLevel: number;
  currentRow: number;
  goldEarned: number;
}
```

```cpp
// C++ equivalent
struct GameState {
  int currentLevel;
  int currentRow;
  int goldEarned;
};
```

### JS/TS `class` → C++ `class`

```typescript
// JS/TS
class ScreenManager {
  private screens: Screen[] = [];
  private current: Screen | null = null;

  constructor() { /* ... */ }
  switchTo(id: ScreenID): void { /* ... */ }
  update(dt: number): void { /* ... */ }
  draw(): void { /* ... */ }
}
```

```cpp
// C++ equivalent
class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;
  Screen* _current = nullptr;
  Screen* _next = nullptr;

public:
  ScreenManager();
  void switchTo(ScreenID id);
  void update(float dt);
  void draw();
};
```

### Class Layout Order

```
class Foo {
public:
  // 1. Public types and aliases
  // 2. Constructor(s) / destructor
  // 3. Public methods (interface)

protected:
  // 4. Protected methods

private:
  // 5. Private methods
  // 6. Private data members (_camelCase)
};
```

### Struct Layout Order

```
struct Foo {
  // All public. Group by logical purpose.
  // 1. Core data
  // 2. Nested structs / enums
  // 3. Trivial const methods (getters)
};
```

### Rule: No Virtual Dispatch on Data Structs

Entity structs (`PlayerState`, `Card`, `GameState`) are plain data. No virtual methods, no inheritance. Only `Screen` uses virtual dispatch (only 4 screens, trivial overhead).

### Composition Over Inheritance

Use embedded structs (has-a) instead of class inheritance for entities. `PlayerState` has `HealthData`, `ShieldData`, `GoldData` — not inheriting from them.

### Good Example

```cpp
// Data struct (JS/TS interface equivalent)
struct HealthData {
  int current;
  int max;
};

// Class with behavior (JS/TS class equivalent)
class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;
  Screen* _current = nullptr;
  Screen* _next = nullptr;

public:
  ScreenManager();
  void switchTo(ScreenID id);
  void update(float dt);
  void draw();
};
```

### Bad Example

```cpp
// Data struct with methods (confusing - struct or class?)
struct ScreenManager {
  Screen* current;
  void switchTo(ScreenID id);  // should be a class
};

// Inheritance for entities (wrong - use composition)
class PlayerState : public HealthData, public ShieldData { ... };
```

---

## 6. Enums

### Rule: Always `enum class` (TS-style `enum`)

Never use C-style `enum`. Always `enum class` for type safety. Enum type: `PascalCase`. Enum values: `PascalCase` (matching TypeScript).

### JS/TS enum → C++ enum class Mapping

```typescript
// JS/TS
enum GameStatus {
  NotStarted,
  NotReady,
  Ready,
  HeroDrop,
  MonsterAttack,
  Win,
  Lose,
}
```

```cpp
// C++ equivalent
enum class GameStatus {
  NotStarted,
  NotReady,
  Ready,
  HeroDrop,
  MonsterAttack,
  Win,
  Lose,
};
```

### Access Pattern

```cpp
// Always fully qualified: GameStatus::NotStarted (not just NotStarted)
if (status == GameStatus::NotStarted) { /* ... */ }
```

### String Conversion Helper

For debugging/logging, provide a `toString()` function for important enums:

```cpp
const char* toString(GameStatus status);
// Returns: "NotStarted", "NotReady", "Ready", etc.
```

### Good Example

```cpp
enum class CardType {
  Monster,
  Shield,
  Heal,
  Gold,
  Item,
  Drop,
  Place,
  Biome,
  EndCard,
};

// Usage
if (card.cardType == CardType::Monster) {
  applyDamage(card.value);
}
```

### Bad Example

```cpp
enum CardType { MONSTER, SHIELD, HEAL };   // C-style enum - wrong
enum class CardType { MONSTER, SHIELD };   // SCREAMING_SNAKE values - wrong
enum class CardType { monster, shield };   // camelCase values - wrong
```

---

## 7. Functions

### Rules

- All functions: `camelCase` (matching JS/TS).
- Method names: `camelCase` (matching JS/TS).
- Free functions: `camelCase` (matching JS/TS).
- Const methods: append `const` (C++ specific, no JS/TS equivalent).
- Parameter passing: `const&` for read-only objects, `&` for output, by value for small types.

### Parameter Conventions

```cpp
// Read-only object: const reference (JS passes objects by reference implicitly)
void resolveCard(const Card& card, const PlayerState& player);

// Output parameter: non-const reference (JS would return an object)
void applyDamage(int damage, PlayerState& player);

// Small types: by value (JS passes everything by value for primitives)
int calculateScore(int totalCards, int gold, bool won);

// Transfer ownership: move semantics (JS has no equivalent)
void addCard(Card&& card);
```

### Lambda Style (JS Arrow Function Equivalent)

```cpp
// JS: const sorted = items.sort((a, b) => a.value - b.value);

// C++ equivalent
std::sort(items.begin(), items.end(),
  [](const Card& a, const Card& b) { return a.value < b.value; });

// Named lambda for complex operations (like JS helper functions)
auto applyShieldDamage = [](int damage, ShieldData& shield) -> ShieldData {
  int absorbed = std::min(damage, shield.value);
  return {shield.value - absorbed};
};
```

### Function Length Guideline

Functions should be short: ideally under 30 lines. If longer, extract helper functions (just like in JS/TS). One function, one responsibility.

### Void vs Return Value

Prefer returning values over output parameters when practical. Use output parameters (`&`) only for multiple return values where structured bindings would be awkward.

```cpp
// Good: return a struct (like JS returning an object)
CombatResult applyDamage(int baseDamage, const PlayerState& player,
                         const std::vector<Card>& items);

// OK: output parameter for complex state mutation
void applyItemEffects(PlayerState& player, const Card& item);
```

### Good Example

```cpp
CardEffect resolveCard(const Card& card, const PlayerState& player,
                       const std::vector<Card>& inventoryItems) {
  switch (card.cardType) {
    case CardType::Monster:
      return resolveMonster(card, player, inventoryItems);
    case CardType::Shield:
      return resolveShield(card);
    default:
      return {};
  }
}
```

### Bad Example

```cpp
// snake_case function name
CardEffect resolve_card(const Card& card);  // wrong naming

// Missing const on read-only parameters
void drawCard(ResourceManager& res, Card& card);  // should be const&
```

---

## 8. Memory Management

### Rule: Smart pointers everywhere

- `std::unique_ptr` for owning pointers (like JS object references — JS GC handles this, C++ uses unique_ptr).
- Raw pointers for non-owning observers (like JS variable references to existing objects).
- `std::shared_ptr` only when genuinely shared ownership is needed (rare).
- No `new`/`delete` in application code.

### JS/TS Mental Model

```typescript
// JS: objects are automatically managed by GC
class ScreenManager {
  private screens: Screen[] = [];  // GC manages lifetime
  private current: Screen | null = null;  // reference, not owned
}
```

```cpp
// C++ equivalent: unique_ptr replaces GC ownership
class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;  // owns screens
  Screen* _current = nullptr;  // non-owning observer (like JS ref)
  Screen* _next = nullptr;    // non-owning observer
};
```

### Ownership Rules

| JS/TS Pattern | C++ Equivalent | Example |
|---|---|---|
| Object in array | `unique_ptr` in container | `std::vector<std::unique_ptr<Screen>>` |
| Reference to existing object | Raw pointer | `Screen* _current` |
| Shared between owners | `shared_ptr` (rare) | `std::shared_ptr<Texture>` |
| Optional value | `std::optional` | `std::optional<int> maybeValue` |

### Factory Pattern (JS/TS `new` Equivalent)

```cpp
// Instead of: const screen = new HomeScreen()
// Use:
auto screen = std::make_unique<HomeScreen>();
```

### No Raw new/delete

```cpp
// WRONG
Screen* s = new HomeScreen();
delete s;

// CORRECT
auto s = std::make_unique<HomeScreen>();
// Automatically deleted when s goes out of scope
```

### Good Example

```cpp
class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;
  Screen* _current = nullptr;

public:
  void init() {
    _screens[0] = std::make_unique<HomeScreen>();
    _screens[1] = std::make_unique<GameScreen>();
    _screens[2] = std::make_unique<WinScreen>();
    _screens[3] = std::make_unique<LoseScreen>();
    _current = _screens[0].get();  // raw pointer = observer
  }
};
```

### Bad Example

```cpp
Screen* screens[4];              // raw owning pointers - wrong
screens[0] = new HomeScreen();  // raw new - wrong
// ... where is the delete?
```

---

## 9. Error Handling

### Rule: No exceptions. Result pattern or error codes.

This project uses no exceptions (C++ `throw`/`catch` are forbidden). No RTTI (`dynamic_cast`, `typeid`). For functions that can fail: return a result struct with a success flag. For initialization: assert in debug, graceful fallback in release.

### Result Pattern (like JS/TS returning `{ ok, value, error }`)

```cpp
// JS/TS
// function loadData(path): { ok: boolean; data?: GameData; error?: string }

// C++ equivalent
struct LoadResult {
  bool ok;
  GameData data;
  std::string error;
};

LoadResult loadData(const std::string& path);
```

### Generic `Result<T>` Template

For reusable error handling, use a generic template. Add to `core/result.h`:

```cpp
// core/result.h — Lightweight error handling (no exceptions)
#pragma once

#include <string>

template <typename T>
struct Result {
  bool ok;
  T value;
  std::string error;

  // Convenience constructors
  static Result success(T val) { return {true, std::move(val), ""}; }
  static Result failure(std::string msg) { return {false, T{}, std::move(msg)}; }
};

// Specialization for void-returning functions
struct ResultVoid {
  bool ok;
  std::string error;

  static ResultVoid success() { return {true, ""}; }
  static ResultVoid failure(std::string msg) { return {false, std::move(msg)}; }
};
```

**Usage pattern:**
```cpp
#include "core/result.h"

Result<int> parseInt(const std::string& str);
ResultVoid saveToFile(const std::string& path, const Data& data);

// Caller checks .ok before accessing .value
auto result = parseInt("42");
if (result.ok) {
  use(result.value);
} else {
  TraceLog(LOG_ERROR, "Parse failed: %s", result.error.c_str());
}
```

**When to use:**
- File I/O (may fail)
- Parsing (may fail)
- Network operations (may fail)
- Any operation with expected failure modes

**When NOT to use:**
- Pure computations that cannot fail
- Functions that always succeed (most internal logic)
- Use `std::optional` for nullable returns, not `Result`

### std::optional (like JS `?.` optional chaining)

```cpp
// JS: const hero = heroes.find(h => h.slug === slug);
// C++ equivalent
std::optional<Hero> findHero(const std::string& slug);

// Usage (like optional chaining)
if (auto hero = findHero(slug)) {
  // hero->name is accessible (like hero?.name)
}
```

### std::optional + value_or (like JS `??` nullish coalescing)

```cpp
// JS: const name = hero?.name ?? "Unknown";
// C++ equivalent
auto hero = findHero(slug);
std::string name = hero.has_value() ? hero->name : "Unknown";
// or with a helper:
std::string name = hero ? hero->name : "Unknown";
```

### Assert for Invariant Violations

```cpp
// Debug-only assertion (removed in release builds)
assert(card.cardType != CardType::EndCard && "EndCard should be handled separately");
```

### Good Example

```cpp
std::optional<Card> findCard(const std::vector<CardSlot>& row, int col) {
  if (col < 0 || col >= static_cast<int>(row.size())) {
    return std::nullopt;  // like returning null in JS
  }
  return row[col].card;
}

// Usage
if (auto card = findCard(row, 2)) {
  drawCard(res, *card, x, y, w, h);
}
```

### Bad Example

```cpp
// Throwing exceptions (forbidden in this project)
Card& findCard(...) {
  throw std::runtime_error("out of bounds");  // WRONG
}

// Returning nullptr for non-pointer types (confusing)
Card findCard(...) {
  return nullptr;  // WRONG - not a pointer
}
```

---

## 10. Modern C++ Features (JS/TS Parallels)

### Feature Mapping Table

| C++ Feature | JS/TS Equivalent | Usage |
|---|---|---|
| `const` / `constexpr` | `const` | Immutable values |
| `auto` | `const` / inferred types | Type inference (**strongly discouraged**, see §10) |
| `std::optional` | `?.` / `??` | Nullable values |
| Structured bindings | Destructuring | `auto [key, value] = ...` (**discouraged**, prefer explicit) |
| `enum class` | `enum` | Type-safe enums |
| `using` alias | `type` / `interface` | Type aliases |
| Lambdas | Arrow functions | `[]() { ... }` |
| Range-based for | `for...of` | `for (const auto& x : vec)` (acceptable) |
| `std::string_view` | n/a (JS strings are immutable) | Read-only string refs |
| `[[nodiscard]]` | n/a (implicit in JS) | Warn on unused return |

### `const` / `constexpr` (JS `const`)

```cpp
// JS: const MAX_HP = 100;
constexpr int maxHp = 100;

// JS: const name = "hero";
const std::string name = "hero";

// JS: const fn = (x) => x * 2;
constexpr int doubleValue(int x) { return x * 2; }
```

### `auto` — Strongly Discouraged (with exceptions)

The `auto` keyword should be **strongly discouraged** in production code and tests. Explicit types improve readability, enable IDE tooling, and help both humans and agents infer types at a glance.

**Exceptions where `auto` is acceptable or preferred:**

| Case | Rule | Rationale |
|---|---|---|
| Lambda variables | `auto` required | Lambda types are unnameable; `std::function` adds heap alloc + type erasure |
| Range-for variables | `const auto&` recommended | Writing explicit type like `const std::pair<const std::string, Task>&` is verbose with no safety benefit |
| Iterator from `.find()` | `auto` preferred | Type is fully determined by container; spelling it out adds verbosity |

```cpp
// ✅ Required — lambda types are unnameable
auto formatCard = [](const Card& c) { return c.name; };

// ✅ Recommended — guaranteed correct, no copies
for (const auto& kv : textureMap) { ... }

// ✅ Preferred — type determined by container
auto it = cards.find(targetId);

// ❌ Avoid — explicit type is clearer
auto result = computeDamage(base, modifier);  // what type is result?
int result = computeDamage(base, modifier);   // explicit = clear
```

**Note:** This rule is stricter than typical C++ game projects (which often use `auto` liberally). The deliberate choice here is to maximize readability for a mixed human+agent codebase.

### Structured Bindings (JS Destructuring)

```cpp
// JS: const [x, y] = position;
auto [x, y] = position;

// JS: const { name, hp } = hero;
auto [name, hp] = std::tie(hero.name, hero.health.current);

// In a map iteration (JS: Object.entries)
for (const auto& [key, value] : textureMap) {
  // key is std::string, value is Texture2D
}
```

### Range-based For (JS `for...of`)

```cpp
// JS: for (const card of hand) { draw(card); }
for (const auto& card : hand) {
  drawCard(card);
}

// JS: hand.forEach((card, i) => { ... })
for (size_t i = 0; i < hand.size(); ++i) {
  // use i and hand[i]
}
```

### `[[nodiscard]]` (C++ specific, no JS equivalent)

```cpp
// Warn if caller ignores the return value
[[nodiscard]] CombatResult applyDamage(int damage, const PlayerState& player);
```

### Good Example

```cpp
// Modern C++ with explicit types (preferred)
std::pair<int, int> position = layout.virtualToScreen({cardX, cardY});
int x = position.first;
int y = position.second;

constexpr int maxHp = 100;
const std::string defaultHero = "headless-knight";

// Range-for with const auto& (acceptable exception)
for (const auto& [key, texture] : _textures) {
  if (key == targetKey) {
    return texture;
  }
}
```

### Bad Example

```cpp
// Old-style C++ that doesn't look like JS/TS
typedef std::map<std::string, Texture2D> TextureMap;  // use 'using'
TextureMap::iterator it = _textures.find(key);        // use 'auto' (iterator exception)
if (it != _textures.end()) {                          // use range-for or structured binding
  return it->second;
}
```

---

## 11. Comments and Documentation

### Rules

- Use `//` for single-line comments (not `/* */`)
- Comment **why**, not **what** (code should be self-documenting for "what")
- No commented-out code (use git history)
- TODO format: `// TODO(username): description`

### File-Level Comments

Every `.h` file should have a brief purpose comment at the top:

```cpp
// combat.h — Damage calculation and shield absorption logic
// Pure game logic layer: no raylib includes, no I/O
#pragma once
```

### Function-Level Comments

Add comments when:
- Algorithm is non-obvious (e.g., weighted random pick)
- Side effects exist (e.g., modifies global state)
- Performance critical path (e.g., hot loop)

```cpp
// Weighted random selection using cumulative distribution
// Returns index into `weights` array, or -1 if all weights are 0
int weightedPick(const std::vector<int>& weights, std::mt19937& rng);
```

### Parameter Documentation

For complex functions, document parameters inline or with `///` comments:

```cpp
/// @param baseDamage - Raw damage before modifiers
/// @param player - Current player state (modified in-place)
/// @param items - Inventory cards that may modify damage
CombatResult applyDamage(int baseDamage, PlayerState& player,
                         const std::vector<Card>& items);
```

### Agent-Readable Code

This codebase is read by both humans and AI agents. To support both:
- Explicit types (not `auto`) help agents infer types and relationships
- Consistent naming (camelCase functions, PascalCase types) enables pattern matching
- Comments explain intent, not syntax — agents can read code, but need context for "why"

### Balance

Documentation should be practical, not verbose:
- Game code evolves rapidly — avoid docs that become stale
- Focus on **intent** and **constraints**, not obvious behavior
- One comment per non-obvious decision, not one per line

### Good Example

```cpp
// Shuffle board rows so player doesn't know exact order
// (original game randomizes row positions after generation)
std::shuffle(rows.begin(), rows.end(), rng);

// TODO(raziur): Add animation delay between card flips
// (currently instant — feels jarring)

int screenWidth = 800;  // design resolution width (virtual pixels)
```

### Bad Example

```cpp
// Increment counter by 1 (obvious from code)
counter++;

/* Multi-line comment block that describes what the code does
   rather than why it exists. */
int x = computePosition();

// int oldCode = deprecated();   // commented-out code — use git
```

---

## 11B. Performance Guidelines

### Philosophy

This project follows lightweight Data-Oriented Design (DOD) principles. We don't need full ECS or Structure-of-Arrays — just cache-friendly, predictable code.

### Core Principles

| Principle | Rule | Why |
|---|---|---|
| Stack-first | Prefer stack allocation over heap | Cache locality, automatic cleanup, no allocator overhead |
| Contiguous storage | `std::vector<T>` over `std::vector<T*>` | Elements side-by-side in memory → fewer cache misses |
| Value semantics | Store by value, not pointer | Avoids indirection, enables compiler optimizations |
| Const references | `const T&` for read-only params | Avoid copies without ownership transfer |
| Compile-time computation | `constexpr` for constants and simple functions | Zero runtime cost |

### Anti-patterns

```cpp
// ❌ Pointer indirection — scattered memory, cache misses
std::vector<Card*> cards;
cards.push_back(new Card());

// ✅ Contiguous values — cache-friendly iteration
std::vector<Card> cards;
cards.push_back(Card());

// ❌ Virtual dispatch on hot path (per-frame)
class Card { virtual void render(); };

// ✅ Data struct + free function (no vtable)
struct Card { CardType type; int value; };
void renderCard(const Card& card, float x, float y);
```

### When to Break Rules

- `std::unique_ptr` for polymorphic screens (only 4 screens, cold path)
- Raw pointers for non-owning observers (existing pattern in `ScreenManager`)
- Virtual dispatch acceptable for screen transitions (rare, not per-frame)

### Reference

This aligns with Mike Acton's Data-Oriented Design principles and the BitSquid/Stingray engine architecture. For small/medium games, just avoiding pointer indirection in hot loops is sufficient — full ECS is overkill.

---

## 12. File Organization

### Directory Structure

```
src/
├── main.cpp                 # Entry point
├── core/                    # Framework: types, screen mgr, resource mgr, layout
│   ├── types.h              # Core enums and data structs
│   ├── components.h         # Reusable data types (HealthData, ShieldData, etc.)
│   ├── entities.h           # Entity structs (PlayerState, CardSlot, VisualEffect)
│   ├── screen.h / .cpp      # Screen base class + ScreenManager
│   ├── resource.h / .cpp    # ResourceManager
│   ├── layout.h / .cpp      # Layout (letterbox scaling)
│   └── input.h / .cpp       # Global input handling
├── screens/                 # Screen implementations
│   ├── home_screen.h / .cpp
│   ├── game_screen.h / .cpp
│   ├── win_screen.h / .cpp
│   └── lose_screen.h / .cpp
├── logic/                   # Pure game logic (NO raylib includes)
│   ├── board_gen.h / .cpp
│   ├── card_effects.h / .cpp
│   ├── combat.h / .cpp
│   ├── items.h / .cpp
│   └── account.h / .cpp
├── render/                  # Drawing functions (NO logic)
│   ├── card_draw.h / .cpp
│   ├── board_draw.h / .cpp
│   ├── hero_panel.h / .cpp
│   ├── hud_draw.h / .cpp
│   └── menu_draw.h / .cpp
├── data/                    # JSON deserialization
│   └── game_data.h / .cpp
├── input/                   # Input abstraction
│   └── input.h / .cpp
└── anim/                    # Animation + easing
    ├── easing.h
    ├── anim.h / .cpp
```

### File Naming Convention

- `snake_case.cpp` / `snake_case.h` (matching directory convention).
- Header + implementation pairs: `foo.h` + `foo.cpp`.
- One class/struct per file (when practical).
- Test files: `test_foo.cpp` alongside implementation.

### Include Graph Rule

- `logic/` files NEVER include raylib headers. This is the pure game logic layer.
- `render/` files include raylib but never call logic functions directly (receive data via parameters).
- `core/` is the shared layer.

### Good Example

```
src/logic/combat.h      # Pure logic, no raylib
src/logic/combat.cpp    # Implementation
src/render/card_draw.h  # Rendering, depends on raylib
src/render/card_draw.cpp
src/logic/test_combat.cpp  # Test, no raylib
```

### Bad Example

```
src/combat.h            # unclear which layer
src/helpers.h           # grab bag, unclear purpose
src/util.h              # same
```

---

## 13. Namespaces

### Rule: All public APIs must be inside namespaces

No type, function, or variable should be in the global namespace. Use nested namespaces matching the directory structure.

### Namespace Structure

```
game::           — core types, enums, components, entities
game::logic::    — pure game logic (no raylib)
game::render::   — drawing functions (raylib-dependent)
game::anim::     — easing and animation
game::data::     — JSON deserialization
```

### Declaration Pattern

```cpp
// src/logic/combat.h
#pragma once
#include "core/types.h"

namespace game::logic {

struct CombatResult {
    int damageToPlayer;
    int shieldAbsorbed;
    // ...
};

CombatResult applyDamage(int baseDamage, const PlayerState& player,
                         const std::vector<Card>& items);

} // namespace game::logic
```

### Usage Pattern

```cpp
// In .cpp files or tests — use using declarations
using namespace game;
using namespace game::logic;

auto result = applyDamage(5, player, items);

// Or fully qualified
auto result = game::logic::applyDamage(5, player, items);
```

### Rules

- Use **nested namespace syntax** (`namespace game::logic { }`) — not nested braces
- Closing brace comment: `} // namespace game::logic`
- Never `using namespace` in header files (only in .cpp files)
- `using namespace game;` and `using namespace game::logic;` are acceptable in .cpp and test files
- Third-party code (raylib, doctest, nlohmann) stays in its own namespace — do not wrap it

### Good Example

```cpp
// header
namespace game::render {
void drawCard(const ResourceManager& res, const Card& card, float x, float y);
}

// .cpp
#include "render/card_draw.h"

namespace game::render {

void drawCard(const ResourceManager& res, const Card& card, float x, float y) {
    // implementation
}

} // namespace game::render
```

### Bad Example

```cpp
// WRONG: everything in global namespace
void drawCard(const ResourceManager& res, const Card& card, float x, float y);

// WRONG: using namespace in header
using namespace std;
namespace game {
    // ...
}

// WRONG: inconsistent namespace nesting
namespace game { namespace logic {
    // ...
}}
```

---

## 14. Anti-patterns

### Forbidden Patterns

| Anti-pattern | Why | Correct Approach |
|---|---|---|
| `new`/`delete` | Manual memory management | `std::make_unique` / RAII |
| `throw`/`catch` | Exceptions forbidden | `Result<T>`, `std::optional` |
| `dynamic_cast` | RTTI forbidden | `enum class` + `switch` |
| `printf`/`cout` | Use raylib `TraceLog` for logging | `TraceLog(LOG_INFO, ...)` |
| `using namespace std;` | Pollutes global namespace | `std::` prefix everywhere |
| `#define` macros | Not type-safe | `constexpr`, `inline`, `enum class` |
| C-style arrays | Not bounds-checked | `std::array` or `std::vector` |
| Trailing underscore `_` on private | Deprecated in this project | Leading underscore `_camelCase` |
| snake_case functions | Not JS/TS readable | `camelCase` |
| SCREAMING_SNAKE enum values | Not TypeScript convention | `PascalCase` |
| Global namespace types/functions | Name collisions, poor organization | `namespace game { }` |
| Struct + free functions (C pattern) | Not idiomatic C++ | Methods on the struct |
| `using namespace` in headers | Pollutes includer's namespace | Only in .cpp files |
| `auto x = ...` (general case) | Reduces readability for humans/agents | Explicit type annotation (see §10) |
| Per-function error structs | Code duplication, inconsistent APIs | `Result<T>` template (see §9) |

### Specific C++ Anti-patterns That Look Like JS Mistakes

```cpp
// WRONG: Accidental copy (JS devs think objects are references)
void processCard(Card card) { ... }  // copies card! Use const&

// WRONG: Shadowed variable (JS devs do this accidentally)
int x = 5;
{
  int x = 10;  // shadows outer x - confusing
}

// WRONG: Missing const on read-only parameters
std::string getName(PlayerState player) { return player.heroSlug; }  // copies!
```

### Good Example

```cpp
// Always const& for read-only parameters
void drawCard(const ResourceManager& res, const Card& card,
              float x, float y);

// Use std::array, not C arrays
std::array<CardSlot, 3> row;  // good
CardSlot row[3];              // bad

// Use enum class, not #defines
enum class CardType { Monster, Shield, Heal };
// NOT: #define CARD_MONSTER 0
```

### Bad Example

```cpp
#include <iostream>
using namespace std;    // never do this

void processCards(vector<Card> cards) {  // copies entire vector!
  // ...
}

// #define MAX_CARDS 10  // use constexpr instead
constexpr int maxCards = 10;
```

---

## 15. .clang-format Configuration

Place this file in the project root:

```yaml
---
Language: Cpp
BasedOnStyle: LLVM

# 2-space indent (JS/TS standard)
IndentWidth: 2
ContinuationIndentWidth: 2
NamespaceIndentation: None
AccessModifierOffset: -2

# K&R braces (JS/TS standard)
BreakBeforeBraces: Attach

# Line length
ColumnLimit: 100

# Trailing commas
Cpp11BracedListStyle: true
AllowShortFunctionsOnASingleLine: Empty
AllowShortIfStatementsOnASingleLine: Never
AllowShortLoopsOnASingleLine: false

# Alignment
AlignAfterOpenBracket: Align
AlignConsecutiveAssignments: false
AlignConsecutiveDeclarations: false
AlignOperands: Align
AlignTrailingComments: true

# Spacing
SpaceAfterCStyleCast: false
SpaceAfterTemplateKeyword: true
SpaceBeforeAssignmentOperators: true
SpaceBeforeParens: ControlStatements
SpaceInEmptyParentheses: false
SpacesInAngles: Never
SpacesInContainerLiterals: true

# Includes
SortIncludes: CaseSensitive
IncludeBlocks: Regroup
IncludeCategories:
  # Own header first
  - Regex: '^"\\.\\.\\/|^"'
    Priority: 1
  # Project headers
  - Regex: '^"core/|^"screens/|^"logic/|^"render/|^"data/|^"input/|^"anim/'
    Priority: 2
  # System headers
  - Regex: '^<.*>'
    Priority: 3
  # Vendor headers
  - Regex: '^"raylib\\.h"'
    Priority: 4

# Pointer alignment (left = type belongs to name)
PointerAlignment: Left
ReferenceAlignment: Left

# Penalty settings (encourage short functions)
PenaltyBreakAssignment: 2
PenaltyBreakBeforeFirstCallParameter: 19
PenaltyBreakComment: 300
PenaltyBreakFirstLessLess: 120
PenaltyBreakString: 1000
```

---

## 16. JS/TS → C++ Cheat Sheet

### Comprehensive Mapping Table

| JS/TS | C++ | Notes |
|---|---|---|
| `const x = 5;` | `const int x = 5;` or `constexpr int x = 5;` | Use `constexpr` for compile-time constants |
| `let x = 5;` | `int x = 5;` | Mutable variable |
| `const fn = () => { ... }` | `constexpr auto fn = []() { ... };` or regular function | Lambdas for callbacks |
| `function foo() { }` | `void foo() { }` | Free function |
| `class Foo { private x = 5; }` | `class Foo { int _x = 5; };` | Leading underscore for private |
| `interface Foo { bar: string; }` | `struct Foo { std::string bar; };` | Struct = interface |
| `type ID = string` | `using Id = std::string;` | Type alias |
| `enum Foo { A, B }` | `enum class Foo { A, B };` | Always enum class |
| `array.map(fn)` | `std::transform(begin, end, out, fn)` or range-for | |
| `array.filter(fn)` | `std::copy_if(begin, end, out, fn)` or loop | |
| `array.find(fn)` | `std::find_if(begin, end, fn)` or range-for with return | |
| `array.reduce(fn, init)` | `std::accumulate(begin, end, init, fn)` | |
| `obj?.prop` | `optional<T>` + `.value()` or `*` | |
| `a ?? b` | `a ? *a : b` or ternary | |
| `const [a, b] = arr` | `auto [a, b] = arr;` | Structured bindings |
| `try { } catch (e) { }` | Forbidden | Use optional/result pattern |
| `typeof x` | `typeid(x)` | Forbidden (no RTTI) |
| `instanceof` | `dynamic_cast` | Forbidden (no RTTI) |
| `for...of` | `for (const auto& x : vec)` | Range-based for |
| `Object.keys(obj)` | `std::views::keys(map)` (C++20) or loop | |
| `JSON.parse(str)` | `nlohmann::json::parse(str)` | Using nlohmann/json |
| `localStorage` | File I/O (`save.json`) | |
| `console.log(msg)` | `TraceLog(LOG_INFO, msg)` | raylib logging |
| Arrow function in callback | Lambda `[]() { ... }` | |
| Spread operator `[...arr]` | `std::vector<T> copy = arr;` | Copy constructor |
| `readonly` | `const` | |
| `as const` | `constexpr` or `const auto&` | |

### Code Conversion Examples

**Example 1: Board generation**

```typescript
// JS/TS
function generateBoard(config: LevelConfig, level: number): CardSlot[][] {
  const board: CardSlot[][] = [];
  for (let row = 0; row < config.rows; row++) {
    board.push(new Array(3).fill(null));
  }
  return board;
}
```

```cpp
// C++ (this project's style)
std::vector<std::vector<CardSlot>> generateBoard(const LevelConfig& config,
                                                  int level) {
  std::vector<std::vector<CardSlot>> board(config.rows,
                                           std::vector<CardSlot>(3));
  return board;
}
```

**Example 2: Finding a hero**

```typescript
// JS/TS
function findHero(slug: string): Hero | undefined {
  return heroes.find(h => h.slug === slug);
}
```

```cpp
// C++
std::optional<Hero> findHero(const std::string& slug) {
  for (const auto& hero : _heroes) {
    if (hero.slug == slug) {
      return hero;
    }
  }
  return std::nullopt;
}
```

**Example 3: Card effect resolution**

```typescript
// JS/TS
function resolveCard(card: Card, player: PlayerState): CardEffect {
  switch (card.cardType) {
    case "monster": return resolveMonster(card, player);
    case "shield": return resolveShield(card);
    default: return { damage: 0, healed: 0 };
  }
}
```

```cpp
// C++
CardEffect resolveCard(const Card& card, const PlayerState& player,
                       const std::vector<Card>& inventoryItems) {
  switch (card.cardType) {
    case CardType::Monster:
      return resolveMonster(card, player, inventoryItems);
    case CardType::Shield:
      return resolveShield(card);
    default:
      return {};
  }
}
```

---

## 17. Migration Note

### snake_case → camelCase Renames

The existing specs (01-architecture.md through 04-build-test.md) use `snake_case` for some function names and member accessors. This section documents all required renames.

### Function Renames

#### From `core/` (ScreenManager, ResourceManager, Layout)

| Current (spec) | Target (style guide) | File |
|---|---|---|
| `on_enter()` | `onEnter()` | screen.h |
| `on_exit()` | `onExit()` | screen.h |
| `switch_to()` | `switchTo()` | screen.h |
| `current_id()` | `currentId()` | screen.h |
| `load_all()` | `loadAll()` | resource.h |
| `unload_all()` | `unloadAll()` | resource.h |
| `has_tex()` | `hasTex()` | resource.h |
| `virtual_to_screen()` | `virtualToScreen()` | layout.h |
| `screen_to_virtual()` | `screenToVirtual()` | layout.h |

#### From `logic/`

| Current (spec) | Target | File |
|---|---|---|
| `generate_board()` | `generateBoard()` | board_gen.h |
| `weighted_pick()` | `weightedPick()` | board_gen.h |
| `resolve_card()` | `resolveCard()` | card_effects.h |
| `resolve_monster()` | `resolveMonster()` | card_effects.h |
| `resolve_shield()` | `resolveShield()` | card_effects.h |
| `resolve_heal()` | `resolveHeal()` | card_effects.h |
| `resolve_gold()` | `resolveGold()` | card_effects.h |
| `resolve_item()` | `resolveItem()` | card_effects.h |
| `resolve_drop()` | `resolveDrop()` | card_effects.h |
| `resolve_place()` | `resolvePlace()` | card_effects.h |
| `resolve_biome()` | `resolveBiome()` | card_effects.h |
| `resolve_end()` | `resolveEnd()` | card_effects.h |
| `apply_damage()` | `applyDamage()` | combat.h |
| `apply_to_shield()` | `applyToShield()` | combat.h |
| `check_invisibility_cloak()` | `checkInvisibilityCloak()` | combat.h |
| `calc_max_hp()` | `calcMaxHp()` | combat.h |
| `get_monster_card_modifier()` | `getMonsterCardModifier()` | combat.h |
| `get_heal_card_modifier()` | `getHealCardModifier()` | combat.h |
| `get_shield_card_modifier()` | `getShieldCardModifier()` | combat.h |
| `get_hero_life_max_modifier()` | `getHeroLifeMaxModifier()` | combat.h |
| `get_nb_rows_hidden()` | `getNbRowsHidden()` | combat.h |
| `check_monster_egg_combo()` | `checkMonsterEggCombo()` | items.h |
| `apply_apple_effect()` | `applyAppleEffect()` | items.h |
| `apply_ring_of_protection()` | `applyRingOfProtection()` | items.h |
| `apply_dragon_wing()` | `applyDragonWing()` | items.h |
| `apply_athelas()` | `applyAthelas()` | items.h |
| `apply_cursed_shield()` | `applyCursedShield()` | items.h |
| `apply_amethyst()` | `applyAmethyst()` | items.h |
| `load_account()` | `loadAccount()` | account.h |
| `save_account()` | `saveAccount()` | account.h |
| `get_level_config()` | `getLevelConfig()` | board_gen.h |
| `get_chest_config()` | `getChestConfig()` | board_gen.h |
| `get_available_biomes()` | `getAvailableBiomes()` | board_gen.h |

#### From `render/`

| Current (spec) | Target | File |
|---|---|---|
| `draw_card()` | `drawCard()` | card_draw.h |
| `draw_card_back()` | `drawCardBack()` | card_draw.h |
| `draw_board()` | `drawBoard()` | board_draw.h |
| `draw_hero_panel()` | `drawHeroPanel()` | hero_panel.h |
| `draw_hud()` | `drawHud()` | hud_draw.h |
| `draw_title()` | `drawTitle()` | menu_draw.h |
| `draw_hero_grid()` | `drawHeroGrid()` | menu_draw.h |
| `draw_start_button()` | `drawStartButton()` | menu_draw.h |
| `draw_gold_display()` | `drawGoldDisplay()` | menu_draw.h |
| `draw_win_overlay()` | `drawWinOverlay()` | win_screen.h |
| `draw_lose_overlay()` | `drawLoseOverlay()` | lose_screen.h |

#### From `anim/`

| Current (spec) | Target | File |
|---|---|---|
| `anim_start()` | `animStart()` | anim.h |
| `anim_update()` | `animUpdate()` | anim.h |
| `anim_value()` | `animValue()` | anim.h |

### Private Member Renames (Trailing → Leading Underscore)

| Spec (trailing _) | Style Guide (leading _) | Class |
|---|---|---|
| `screens_` | `_screens` | ScreenManager |
| `current_` | `_current` | ScreenManager |
| `next_` | `_next` | ScreenManager |
| `textures_` | `_textures` | ResourceManager |
| `sounds_` | `_sounds` | ResourceManager |
| `fonts_` | `_fonts` | ResourceManager |

### Enum Value Renames (SCREAMING_SNAKE → PascalCase)

| Current (SCREAMING_SNAKE) | Target (PascalCase) | Enum |
|---|---|---|
| `NOT_STARTED` | `NotStarted` | GameStatus |
| `NOT_READY` | `NotReady` | GameStatus |
| `READY` | `Ready` | GameStatus |
| `HERO_DROP` | `HeroDrop` | GameStatus |
| `MONSTER_ATTACK` | `MonsterAttack` | GameStatus |
| `WIN` | `Win` | GameStatus |
| `LOSE` | `Lose` | GameStatus |
| `HOME` | `Home` | ScreenID |
| `GAME` | `Game` | ScreenID |
| `MONSTER` | `Monster` | CardType |
| `SHIELD` | `Shield` | CardType |
| `HEAL` | `Heal` | CardType |
| `GOLD` | `Gold` | CardType |
| `ITEM` | `Item` | CardType |
| `DROP` | `Drop` | CardType |
| `PLACE` | `Place` | CardType |
| `BIOME` | `Biome` | CardType |
| `END` | `EndCard` | CardType (avoids keyword collision) |
| `FOREST` | `Forest` | Biome |
| `CAVE` | `Cave` | Biome |
| `ENCHANTED_LANDS` | `EnchantedLands` | Biome |
| `DESERT` | `Desert` | Biome |
| `ABYSS` | `Abyss` | Biome |
| `UNDERWORLD` | `Underworld` | Biome |

### Namespace Migration

| File | Current (no namespace) | Target (namespaced) |
|---|---|---|
| `core/types.h` | Global `Card`, `Hero`, `GameState`, enums | `namespace game { ... }` |
| `core/components.h` | Global `HealthData`, `AnimState`, etc. | `namespace game { ... }` |
| `core/entities.h` | Global `PlayerState`, `CardSlot`, etc. | `namespace game { ... }` |
| `core/screen.h` | Global `Screen`, `ScreenManager` | `namespace game { ... }` |
| `core/resource.h` | Global `ResourceManager` | `namespace game { ... }` |
| `core/layout.h` | Global `Layout` | `namespace game { ... }` |
| `logic/board_gen.h` | Global `GeneratedBoard`, `generateBoard()` | `namespace game::logic { ... }` |
| `logic/card_effects.h` | Global `resolveCard()`, etc. | `namespace game::logic { ... }` |
| `logic/combat.h` | Global `CombatResult`, `applyDamage()`, etc. | `namespace game::logic { ... }` |
| `logic/items.h` | Global `ComboResult`, `checkMonsterEggCombo()`, etc. | `namespace game::logic { ... }` |
| `logic/account.h` | Global `getLevelConfig()`, etc. | `namespace game::logic { ... }` |
| `render/card_draw.h` | Global `drawCard()`, `drawCardBack()` | `namespace game::render { ... }` |
| `render/board_draw.h` | Global `drawBoard()` | `namespace game::render { ... }` |
| `render/hero_panel.h` | Global `drawHeroPanel()` | `namespace game::render { ... }` |
| `render/hud_draw.h` | Global `drawHud()` | `namespace game::render { ... }` |
| `render/menu_draw.h` | Global `drawTitle()`, etc. | `namespace game::render { ... }` |
| `anim/easing.h` | Global `lerp()`, `easeOut()`, etc. | `namespace game::anim { ... }` |

### Migration Process

1. Add namespace wrappers to all header files (core first, then logic, render, anim).
2. Add `using namespace game;` / `using namespace game::logic;` etc. to all .cpp files.
3. Search-replace all snake_case function names in headers first (these define the API).
4. Update all call sites in .cpp files.
5. Update private member names in class definitions.
6. Update enum values (most tedious — requires updating all usage sites).
7. Move `animStart`/`animUpdate`/`animValue` from free functions to AnimState methods.
8. Move `Button` struct from `render/menu_draw.h` to `core/types.h`.
9. Remove duplicate `loadAccount`/`saveAccount` free functions (keep Account methods only).
10. Run `make test` to verify nothing broke.
11. Update spec files to reference the new naming.
