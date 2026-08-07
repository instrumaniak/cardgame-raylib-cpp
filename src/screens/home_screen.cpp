// src/screens/home_screen.cpp — Home screen implementation
#include "screens/home_screen.h"

#include "core/constants.h"
#include "input/input.h"
#include "logic/account.h"
#include "raylib.h"
#include "render/menu_draw.h"

#include <fstream>
#include <sstream>

namespace game {

namespace {

constexpr float START_BTN_W = 196.0f;
constexpr float START_BTN_H = 50.0f;

std::string slugToDisplayName(const std::string& slug) {
  std::string result;
  bool capitalizeNext = true;
  for (char c : slug) {
    if (c == '-') {
      result += ' ';
      capitalizeNext = true;
    } else if (capitalizeNext) {
      result += static_cast<char>(toupper(static_cast<unsigned char>(c)));
      capitalizeNext = false;
    } else {
      result += c;
    }
  }
  return result;
}

} // anonymous namespace

ScreenID HomeScreen::id() const {
  return ScreenID::Home;
}

void HomeScreen::onEnter() {
  // Load saved account or create default
  std::ifstream file("save.json");
  if (file.is_open()) {
    std::stringstream ss;
    ss << file.rdbuf();
    _account = logic::loadAccount(ss.str());
  } else {
    _account = logic::createDefaultAccount();
  }

  _selectedIndex = -1;

  buildHeroGrid();

  float btnX = (DESIGN_WIDTH - START_BTN_W) / 2.0f;
  float btnY = HERO_GRID_Y + HERO_GRID_ROWS * (HERO_CARD_H + HERO_GRID_GAP) + 20.0f;
  _startButton = Button{{btnX, btnY, START_BTN_W, START_BTN_H}, "Start Game", false, ""};
}

void HomeScreen::update(float dt) {
  (void)dt;

  if (!_layout) {
    return;
  }

  Vector2 mousePos = virtualMousePosition(*_layout);

  _startButton.hovered = CheckCollisionPointRec(mousePos, _startButton.bounds);

  if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
    handleClick(mousePos);
  }
}

void HomeScreen::draw() {
  if (!_res) {
    return;
  }

  ClearBackground(BLACK);

  render::drawTitle(*_res, DESIGN_WIDTH);
  render::drawHeroGrid(*_res, _heroes, _account, 0, HERO_GRID_Y, _selectedIndex);
  render::drawStartButton(*_res, _startButton);
  render::drawGoldDisplay(*_res, _account.gold);
}

void HomeScreen::buildHeroGrid() {
  _heroes.clear();

  for (const auto& save : _account.heroes) {
    Hero hero;
    hero.slug = save.slug;
    hero.name = slugToDisplayName(save.slug);
    hero.description = "";
    hero.lifeMax = save.lifeMax;
    hero.shield = save.shield;
    hero.cost = save.cost;
    hero.startingItems = save.bag;
    hero.spriteX = 0;
    hero.spriteY = 0;
    hero.locked = save.locked;
    _heroes.push_back(hero);
  }
}

void HomeScreen::handleClick(Vector2 mousePos) {
  if (CheckCollisionPointRec(mousePos, _startButton.bounds)) {
    if (_selectedIndex >= 0 && !_heroes[_selectedIndex].locked) {
      _screenManager->setSelectedHero(ScreenID::Game, _heroes[_selectedIndex]);
      _screenManager->switchTo(ScreenID::Game);
    }
    return;
  }

  float totalW = HERO_GRID_COLS * HERO_CARD_W + (HERO_GRID_COLS - 1) * HERO_GRID_GAP;
  float startX = (DESIGN_WIDTH - totalW) / 2.0f;

  for (int i = 0; i < HERO_GRID_ROWS * HERO_GRID_COLS; ++i) {
    if (i >= static_cast<int>(_heroes.size())) {
      break;
    }

    int col = i % HERO_GRID_COLS;
    int row = i / HERO_GRID_COLS;

    float cardX = startX + col * (HERO_CARD_W + HERO_GRID_GAP);
    float cardY = HERO_GRID_Y + row * (HERO_CARD_H + HERO_GRID_GAP);

    Rectangle cardRect = {cardX, cardY, HERO_CARD_W, HERO_CARD_H};

    if (CheckCollisionPointRec(mousePos, cardRect)) {
      if (_heroes[i].locked) {
        // Find matching HeroSave and attempt unlock
        for (auto& save : _account.heroes) {
          if (save.slug == _heroes[i].slug) {
            if (logic::unlockHero(save, _account)) {
              _heroes[i].locked = false;
              std::ofstream file("save.json");
              if (file.is_open()) {
                file << logic::saveAccount(_account);
              }
            }
            break;
          }
        }
      } else {
        _selectedIndex = i;
      }
      return;
    }
  }
}

} // namespace game
