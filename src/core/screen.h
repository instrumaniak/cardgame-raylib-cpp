#pragma once

#include "core/types.h"

#include <array>
#include <memory>

namespace game {

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
  Screen* _current = nullptr;
  Screen* _next = nullptr;

public:
  void init();
  void switchTo(ScreenID id);
  void update(float dt);
  void draw();
  ScreenID currentId() const;
};

} // namespace game
