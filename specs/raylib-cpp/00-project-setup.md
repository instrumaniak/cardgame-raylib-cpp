# spec: raylib-project-setup

## Goal
Set up the raylib C++17 project structure, dependencies, assets, build configuration, and directory layout.

## Requirements
1. C++17 project with g++, Make, raylib C API (vendor/raylib/)
2. Asset directory structure: audio/, fonts/, img/, data/
3. All game assets included locally in assets/
4. Configure Makefile for desktop build (debug, release, run, test targets)
5. Set up .gitignore for raylib/C++ project
6. System dependencies via apt (nlohmann-json3-dev, doctest-dev)
7. Screen management at 960x540 design resolution with letterbox scaling

## Constraints
- raylib C API only (vendor/raylib/ with libraylib.a), no raylib-cpp wrapper
- Desktop only for v1 (no WASM)
- English only (no i18n in v1)
- All dependencies resolved via apt or local vendor/ (no CMake, no FetchContent)

## Dependencies

```bash
# Required system packages
sudo apt install nlohmann-json3-dev doctest-dev

# raylib (build from vendor)
cd vendor/raylib/src
make
# produces libraylib.a in vendor/raylib/src/
```

## Project Structure

```
cardgame-raylib/
├── Makefile
├── .gitignore
├── README.md
├── assets/
│   ├── audio/          # 8 WAV sound effects (converted from MP3)
│   ├── fonts/          # 2 TTF fonts (PearSoda, Zepto)
│   ├── img/            # All images
│   │   ├── cards.png   # Main sprite sheet (all card/hero/monster illustrations)
│   │   ├── icons.png   # Icon sprite sheet (hearts, coins, arrows)
│   │   ├── hit.png     # Hit animation strip
│   │   ├── shards.png  # Shards animation strip
│   │   ├── card-back.png
│   │   ├── card-frame-*.png  # Card frame overlays (per type)
│   │   ├── card-bg-deco*.png # Card background decorations
│   │   ├── hero-frame.png
│   │   ├── chest-frame.png
│   │   ├── light-frame.png
│   │   ├── biome-frame.png
│   │   ├── click.png
│   │   └── pointer.png
│   └── data/           # JSON data files
│       ├── heroes.json
│       ├── cards.json
│       ├── items.json
│       ├── levels.json
│       └── biomes.json
├── specs/              # Implementation specs
├── vendor/
│   └── raylib/          # raylib source (submodule or extracted)
│       ├── src/
│       │   ├── raylib.h
│       │   ├── libraylib.a  # pre-built
│       │   └── ...          # raylib source files
│       └── CHANGELOG
└── src/
    ├── main.cpp
    ├── core/
    │   ├── constants.h
    │   ├── types.h
    │   ├── components.h
    │   ├── entities.h
    │   ├── screen.h
    │   ├── screen.cpp
    │   ├── resource.h
    │   ├── resource.cpp
    │   ├── layout.h
    │   ├── layout.cpp
    │   ├── input.h
    │   └── input.cpp
    ├── screens/
    │   ├── home_screen.h
    │   ├── home_screen.cpp
    │   ├── game_screen.h
    │   ├── game_screen.cpp
    │   ├── win_screen.h
    │   ├── win_screen.cpp
    │   ├── lose_screen.h
    │   └── lose_screen.cpp
    ├── data/
    │   ├── game_data.h
    │   └── game_data.cpp
    ├── logic/
    │   ├── board_gen.h
    │   ├── board_gen.cpp
    │   ├── card_effects.h
    │   ├── card_effects.cpp
    │   ├── combat.h
    │   ├── combat.cpp
    │   ├── items.h
    │   ├── items.cpp
    │   ├── account.h
    │   └── account.cpp
    ├── render/
    │   ├── card_draw.h
    │   ├── card_draw.cpp
    │   ├── board_draw.h
    │   ├── board_draw.cpp
    │   ├── hero_panel.h
    │   ├── hero_panel.cpp
    │   ├── hud_draw.h
    │   ├── hud_draw.cpp
    │   ├── menu_draw.h
    │   └── menu_draw.cpp
    ├── input/
    │   ├── input.h
    │   └── input.cpp
    └── anim/
        ├── easing.h
        ├── anim.h
        └── anim.cpp
```

## Makefile

```makefile
CXX       := g++
CXXFLAGS  := -std=c++17 -Wall -Wextra -O2 -I vendor/raylib/src -I src
LDFLAGS   := -L vendor/raylib/src -lraylib -lm
SRC       := src/main.cpp \
             src/core/screen.cpp src/core/resource.cpp \
             src/core/layout.cpp src/core/input.cpp \
             src/screens/home_screen.cpp src/screens/game_screen.cpp \
             src/screens/win_screen.cpp src/screens/lose_screen.cpp \
             src/data/game_data.cpp \
             src/logic/board_gen.cpp src/logic/card_effects.cpp \
             src/logic/combat.cpp src/logic/items.cpp src/logic/account.cpp \
             src/render/card_draw.cpp src/render/board_draw.cpp \
             src/render/hero_panel.cpp src/render/hud_draw.cpp \
             src/render/menu_draw.cpp \
             src/input/input.cpp src/anim/anim.cpp
OBJ       := $(SRC:.cpp=.o)
TARGET    := cardgame

all: $(TARGET)

$(TARGET): $(OBJ)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $<

run: $(TARGET)
	./$(TARGET)

debug: CXXFLAGS += -g -O0 -DDEBUG
debug: $(TARGET)

clean:
	rm -f $(OBJ) $(TARGET)

.PHONY: all run debug clean
```

## .gitignore

```
# Build
build/
*.o
cardgame

# Dependencies
vendor/raylib/src/*.o
vendor/raylib/src/libraylib.a

# Runtime data
save.json

# Editor
*.swp
*.swo
*~
.DS_Store
```

## Acceptance Criteria
- [ ] `make` compiles without errors
- [ ] `make run` launches the game window at 960x540 (letterboxed)
- [ ] All assets listed in assets/ exist on disk
- [ ] Makefile has debug, run, clean targets
- [ ] .gitignore covers all temporary and build artifacts
- [ ] System packages installable via apt
- [ ] raylib builds from vendor/ without errors
