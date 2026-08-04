// src/core/components.h — Reusable data types for entity composition
#pragma once

#include "core/types.h"

#include <vector>

namespace game {

struct HealthData {
  int current;
  int max;
};

struct ShieldData {
  int value;
};

struct GoldData {
  int amount;
};

struct InventoryData {
  std::vector<Card> items;
  int capacity;
};

struct PositionData {
  int row;
  int col;
};

struct AnimState {
  float t = 0.f;
  float duration = 0.f;
  bool loop = false;

  void start(float dur, bool lp = false);
  void update(float dt);
  [[nodiscard]] float value() const;
  [[nodiscard]] bool isActive() const {
    return t < duration;
  }
  [[nodiscard]] bool isDone() const {
    return t >= duration;
  }
};

} // namespace game
