// src/screens/home_screen.h — Home screen: hero selection, account, game start
#pragma once

#include "core/screen.h"
#include "core/types.h"

#include <vector>

namespace game {

class HomeScreen : public Screen {
  Account _account;
  std::vector<Hero> _heroes;
  int _selectedIndex = -1;
  Button _startButton;

public:
  ScreenID id() const override;
  void onEnter() override;
  void update(float dt) override;
  void draw() override;

private:
  void buildHeroGrid();
  void handleClick(Vector2 mousePos);
};

} // namespace game
