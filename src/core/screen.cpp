// src/core/screen.cpp — Screen base implementation and ScreenManager state transitions
#include "core/screen.h"

#include <memory>

namespace game {

namespace {

constexpr std::size_t screenIndex(ScreenID id) {
  return static_cast<std::size_t>(id);
}

class PlaceholderScreen : public Screen {
  ScreenID _id;

public:
  explicit PlaceholderScreen(ScreenID id) : _id(id) {}

  ScreenID id() const override {
    return _id;
  }
  void update(float dt) override {
    (void)dt;
  }
  void draw() override {}
};

} // namespace

void ScreenManager::init() {
  _screens[screenIndex(ScreenID::Home)] = std::make_unique<PlaceholderScreen>(ScreenID::Home);
  _screens[screenIndex(ScreenID::Game)] = std::make_unique<PlaceholderScreen>(ScreenID::Game);
  _screens[screenIndex(ScreenID::Win)] = std::make_unique<PlaceholderScreen>(ScreenID::Win);
  _screens[screenIndex(ScreenID::Lose)] = std::make_unique<PlaceholderScreen>(ScreenID::Lose);

  _current = nullptr;
  _next = nullptr;
}

void ScreenManager::switchTo(ScreenID id) {
  _next = _screens[screenIndex(id)].get();
}

void ScreenManager::update(float dt) {
  if (_current != _next) {
    if (_current != nullptr) {
      _current->onExit();
    }

    _current = _next;

    if (_current != nullptr) {
      _current->onEnter();
    }
  }

  if (_current != nullptr) {
    _current->update(dt);
  }
}

void ScreenManager::draw() {
  if (_current != nullptr) {
    _current->draw();
  }
}

ScreenID ScreenManager::currentId() const {
  if (_current != nullptr) {
    return _current->id();
  }

  return ScreenID::Home;
}

} // namespace game
