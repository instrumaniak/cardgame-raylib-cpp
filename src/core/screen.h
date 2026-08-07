#pragma once

#include "core/types.h"

#include <array>
#include <memory>

namespace game {

class ResourceManager;
class Layout;
class ScreenManager;

class Screen {
protected:
  ResourceManager* _res = nullptr;
  Layout* _layout = nullptr;
  ScreenManager* _screenManager = nullptr;

public:
  virtual ~Screen() = default;

  virtual ScreenID id() const = 0;
  virtual void onEnter() {}
  virtual void onExit() {}
  virtual void update(float dt) = 0;
  virtual void draw() = 0;
  virtual void setSelectedHero(const Hero&) {}

  void setResources(ResourceManager* res, Layout* layout, ScreenManager* sm) {
    _res = res;
    _layout = layout;
    _screenManager = sm;
  }
};

class ScreenManager {
  std::array<std::unique_ptr<Screen>, 4> _screens;
  Screen* _current = nullptr;
  Screen* _next = nullptr;

public:
  void init(ResourceManager& res, Layout& layout);
  void switchTo(ScreenID id);
  void update(float dt);
  void draw();
  ScreenID currentId() const;
  Screen* getScreen(ScreenID id);
  void setSelectedHero(ScreenID id, const Hero& hero);
};

} // namespace game
