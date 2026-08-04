// src/core/layout.cpp — Letterbox scaling implementation
#include "core/layout.h"

namespace game {

void Layout::init() {
  renderTexture = LoadRenderTexture(DESIGN_WIDTH, DESIGN_HEIGHT);
  dstRect.x = 0;
  dstRect.y = 0;
  dstRect.width = 0;
  dstRect.height = 0;
  scaleX = 1.0f;
  scaleY = 1.0f;
}

void Layout::compute(int windowWidth, int windowHeight) {
  float designAspect = static_cast<float>(DESIGN_WIDTH) / DESIGN_HEIGHT;
  float windowAspect = static_cast<float>(windowWidth) / windowHeight;

  if (windowAspect > designAspect) {
    // Window is wider → letterbox horizontally
    float scaledHeight = static_cast<float>(windowHeight);
    float scaledWidth = scaledHeight * designAspect;
    float offsetX = (windowWidth - scaledWidth) / 2.0f;
    dstRect.x = offsetX;
    dstRect.y = 0;
    dstRect.width = scaledWidth;
    dstRect.height = scaledHeight;
    scaleX = scaledWidth / DESIGN_WIDTH;
    scaleY = scaledHeight / DESIGN_HEIGHT;
  } else {
    // Window is taller → pillarbox vertically
    float scaledWidth = static_cast<float>(windowWidth);
    float scaledHeight = scaledWidth / designAspect;
    float offsetY = (windowHeight - scaledHeight) / 2.0f;
    dstRect.x = 0;
    dstRect.y = offsetY;
    dstRect.width = scaledWidth;
    dstRect.height = scaledHeight;
    scaleX = scaledWidth / DESIGN_WIDTH;
    scaleY = scaledHeight / DESIGN_HEIGHT;
  }
}

Vector2 Layout::virtualToScreen(Vector2 virt) const {
  return {
    dstRect.x + virt.x * scaleX,
    dstRect.y + virt.y * scaleY,
  };
}

Vector2 Layout::screenToVirtual(Vector2 screen) const {
  return {
    (screen.x - dstRect.x) / scaleX,
    (screen.y - dstRect.y) / scaleY,
  };
}

} // namespace game
