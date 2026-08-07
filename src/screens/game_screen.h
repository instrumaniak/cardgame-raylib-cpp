// src/screens/game_screen.h — Game screen: full gameplay loop orchestrator
#pragma once

#include "core/entities.h"
#include "core/screen.h"
#include "core/types.h"

#include <random>

namespace game {

class GameScreen : public Screen {
  GameState _game;
  Hero _hero;
  Account _account;
  std::mt19937 _rng;
  Button _primaryBtn;
  Button _mainMenuBtn;
  float _delayTimer = 0.0f;
  bool _isDelaying = false;

public:
  ScreenID id() const override;
  void onEnter() override;
  void onExit() override;
  void update(float dt) override;
  void draw() override;

  void setSelectedHero(const Hero& hero);

private:
  void initGameState();
  void generateBoard();
  void handleClick(Vector2 mousePos);
  void updateDelay(float dt);
  int calculateScore() const;
};

} // namespace game
