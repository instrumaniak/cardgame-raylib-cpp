// src/core/components.cpp — AnimState method implementations

#include "core/components.h"

#include <algorithm>
#include <cmath>

namespace game {

void AnimState::start(float dur, bool lp) {
  t = 0.0f;
  duration = dur;
  loop = lp;
}

void AnimState::update(float dt) {
  if (!isActive()) {
    return;
  }
  t += dt;
  if (loop && t >= duration) {
    t = std::fmod(t, duration);
  }
}

float AnimState::value() const {
  if (duration <= 0.0f) {
    return 0.0f;
  }
  return std::clamp(t / duration, 0.0f, 1.0f);
}

} // namespace game
