// src/core/layout.h — Letterbox scaling from 960x540 design resolution
#pragma once

#include "core/constants.h"
#include "raylib.h"

namespace game {

struct Layout {
  RenderTexture renderTexture;
  Rectangle dstRect;
  float scaleX;
  float scaleY;

  void init();
  void compute(int windowWidth, int windowHeight);
  Vector2 virtualToScreen(Vector2 virt) const;
  Vector2 screenToVirtual(Vector2 screen) const;
};

} // namespace game
