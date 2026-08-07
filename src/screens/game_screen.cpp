// src/screens/game_screen.cpp — Game screen implementation
#include "screens/game_screen.h"

#include "core/constants.h"
#include "core/slugs.h"
#include "input/input.h"
#include "logic/account.h"
#include "logic/board_gen.h"
#include "logic/combat.h"
#include "logic/items.h"
#include "logic/level_config.h"
#include "logic/turn_flow.h"
#include "raylib.h"
#include "render/board_draw.h"
#include "render/hero_panel.h"
#include "render/hud_draw.h"
#include "render/overlay.h"

#include <algorithm>
#include <cmath>
#include <fstream>
#include <sstream>

namespace game {

namespace {

constexpr float BOARD_X = 280.0f;
constexpr float BOARD_Y = 80.0f;
constexpr float CARD_GAP = 8.0f;
constexpr float CLICK_PAD = 4.0f;

} // anonymous namespace

ScreenID GameScreen::id() const {
  return ScreenID::Game;
}

void GameScreen::setSelectedHero(const Hero& hero) {
  _hero = hero;
}

void GameScreen::onEnter() {
  std::ifstream file("save.json");
  if (file.is_open()) {
    std::stringstream ss;
    ss << file.rdbuf();
    _account = logic::loadAccount(ss.str());
  } else {
    _account = logic::createDefaultAccount();
  }

  _delayTimer = 0.0f;
  _isDelaying = false;

  initGameState();
}

void GameScreen::onExit() {}

void GameScreen::initGameState() {
  _game = GameState{};

  _game.player.heroSlug = _hero.slug;
  _game.player.health.max = logic::calcMaxHp(_game.player.inventory.items, _hero.lifeMax);
  _game.player.health.current = _game.player.health.max;
  _game.player.shield.value = _hero.shield;
  _game.player.gold.amount = 0;
  _game.player.athelasUsed = false;

  for (const auto& slug : _hero.startingItems) {
    Card item;
    item.slug = slug;
    item.cardType = CardType::Item;
    item.value = 0;
    item.name = slug;
    item.description = "";
    item.spriteX = 0;
    item.spriteY = 0;
    item.spriteW = 0;
    item.spriteH = 0;
    _game.player.inventory.items.push_back(item);
  }

  _game.currentLevel = 1;
  _game.currentRow = 0;
  _game.currentCol = 1;
  _game.goldEarned = 0;
  _game.visitedBiomes.clear();
  _game.currentBiome = "";
  _game.inChest = false;
  _game.chestRowsRemaining = 0;
  _game.chestType = "";
  _game.effects.clear();

  _game.status = GameStatus::NotStarted;
  generateBoard();
}

void GameScreen::generateBoard() {
  LevelConfig config;
  if (_game.inChest) {
    config = logic::getChestConfig(_game.chestType, _game.currentLevel);
  } else {
    config = logic::getLevelConfig(_game.currentLevel);
  }

  auto biomes = logic::getAvailableBiomes(_game.currentLevel, _game.visitedBiomes);
  auto result = logic::generateBoard(config, _game.currentLevel, biomes, _rng);

  _game.board = std::move(result.slots);
  _game.currentBiome = result.biome;
  _game.currentRow = 0;
  _game.currentCol = 1;
  _game.status = GameStatus::Ready;
}

void GameScreen::update(float dt) {
  if (!_layout) {
    return;
  }

  _game.effects.erase(
    std::remove_if(
      _game.effects.begin(),
      _game.effects.end(),
      [](const VisualEffect& e) { return e.anim.isDone(); }
    ),
    _game.effects.end()
  );

  for (auto& effect : _game.effects) {
    effect.anim.update(dt);
  }

  if (_isDelaying) {
    updateDelay(dt);
    return;
  }

  switch (_game.status) {
  case GameStatus::NotStarted:
    generateBoard();
    break;

  case GameStatus::NotReady:
    generateBoard();
    break;

  case GameStatus::Ready: {
    Vector2 mousePos = virtualMousePosition(*_layout);
    _primaryBtn.hovered = CheckCollisionPointRec(mousePos, _primaryBtn.bounds);
    _mainMenuBtn.hovered = CheckCollisionPointRec(mousePos, _mainMenuBtn.bounds);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      handleClick(mousePos);
    }
    break;
  }

  case GameStatus::HeroDrop:
  case GameStatus::MonsterAttack:
    _game.status = GameStatus::Ready;
    break;

  case GameStatus::Win:
  case GameStatus::Lose: {
    Vector2 mousePos = virtualMousePosition(*_layout);
    _primaryBtn.hovered = CheckCollisionPointRec(mousePos, _primaryBtn.bounds);
    _mainMenuBtn.hovered = CheckCollisionPointRec(mousePos, _mainMenuBtn.bounds);

    if (IsMouseButtonPressed(MOUSE_LEFT_BUTTON)) {
      if (CheckCollisionPointRec(mousePos, _primaryBtn.bounds)) {
        logic::addGold(_account, _game.goldEarned);
        std::ofstream saveFile("save.json");
        if (saveFile.is_open()) {
          saveFile << logic::saveAccount(_account);
        }
        initGameState();
      } else if (CheckCollisionPointRec(mousePos, _mainMenuBtn.bounds)) {
        logic::addGold(_account, _game.goldEarned);
        std::ofstream saveFile("save.json");
        if (saveFile.is_open()) {
          saveFile << logic::saveAccount(_account);
        }
        _screenManager->switchTo(ScreenID::Home);
      }
    }
    break;
  }
  }
}

void GameScreen::updateDelay(float dt) {
  _delayTimer -= dt;
  if (_delayTimer <= 0.0f) {
    _isDelaying = false;
  }
}

void GameScreen::handleClick(Vector2 mousePos) {
  float slotW = static_cast<float>(CARD_WIDTH);
  float slotH = static_cast<float>(CARD_HEIGHT);

  float rowY = BOARD_Y;

  for (int row = 0; row < static_cast<int>(_game.board.size()); ++row) {
    if (row < _game.currentRow || row >= _game.currentRow + 3) {
      rowY += static_cast<float>(ROW_HEIGHT);
      continue;
    }

    for (int col = 0; col < static_cast<int>(_game.board[row].size()); ++col) {
      float cardX = BOARD_X + col * (slotW + CARD_GAP);

      Rectangle cardRect = {
        cardX - CLICK_PAD, rowY - CLICK_PAD, slotW + CLICK_PAD * 2, slotH + CLICK_PAD * 2
      };

      if (CheckCollisionPointRec(mousePos, cardRect)) {
        logic::playCard(_game, row, col, _rng);
        return;
      }
    }

    rowY += static_cast<float>(ROW_HEIGHT);
  }
}

int GameScreen::calculateScore() const {
  return _game.goldEarned + _game.currentLevel * 10;
}

void GameScreen::draw() {
  if (!_res) {
    return;
  }

  ClearBackground(BLACK);

  render::drawBoard(
    *_res, _game.board, _game.currentRow, BOARD_X, BOARD_Y, _game.player.inventory.items
  );
  render::drawHeroPanel(*_res, _game.player, _hero, 0.0f, 0.0f);
  render::drawHud(*_res, _game.currentLevel, _game.goldEarned, _game.effects);

  if (_game.status == GameStatus::Win) {
    int score = calculateScore();
    int totalGold = _account.gold + _game.goldEarned;
    render::drawWinOverlay(
      *_res,
      _hero.name,
      _game.goldEarned,
      totalGold,
      score,
      _primaryBtn,
      _mainMenuBtn,
      static_cast<float>(DESIGN_WIDTH),
      static_cast<float>(DESIGN_HEIGHT)
    );
  } else if (_game.status == GameStatus::Lose) {
    int score = calculateScore();
    int totalGold = _account.gold + _game.goldEarned;
    render::drawLoseOverlay(
      *_res,
      _hero.name,
      _game.goldEarned,
      totalGold,
      score,
      _primaryBtn,
      _mainMenuBtn,
      static_cast<float>(DESIGN_WIDTH),
      static_cast<float>(DESIGN_HEIGHT)
    );
  }
}

} // namespace game
