// src/render/hud_draw.cpp — HUD drawing implementation

#include "render/hud_draw.h"

#include "core/constants.h"
#include "core/entities.h"
#include "core/typography.h"
#include "raylib.h"

#include <cmath>

namespace game::render {

using namespace game;

namespace {

constexpr float HUD_PADDING = 16.0f;
constexpr float SCREEN_SHAKE_INTENSITY = 4.0f;

} // anonymous namespace

void drawHud(
  const ResourceManager& res, int level, int goldEarned, const std::vector<VisualEffect>& effects
) {
  float shakeX = 0.0f;
  float shakeY = 0.0f;

  for (const auto& effect : effects) {
    if (effect.type == EffectType::ScreenShake && effect.anim.isActive()) {
      float intensity = SCREEN_SHAKE_INTENSITY * (1.0f - effect.anim.value());
      int minVal = static_cast<int>(std::round(-intensity));
      int maxVal = static_cast<int>(std::round(intensity));
      shakeX = static_cast<float>(GetRandomValue(minVal, maxVal));
      shakeY = static_cast<float>(GetRandomValue(minVal, maxVal));
      break;
    }
  }

  float levelX = HUD_PADDING + shakeX;
  float levelY = HUD_PADDING + shakeY;

  char levelText[32];
  snprintf(levelText, sizeof(levelText), "Level %d", level);
  drawText(res, TextStyle::Heading, levelText, levelX, levelY, WHITE);

  char goldText[32];
  snprintf(goldText, sizeof(goldText), "%d G", goldEarned);
  Vector2 goldSize = measureText(res, TextStyle::Body, goldText);
  float goldX = static_cast<float>(DESIGN_WIDTH) - goldSize.x - HUD_PADDING + shakeX;
  float goldY = HUD_PADDING + shakeY;
  drawText(res, TextStyle::Body, goldText, goldX, goldY, GOLD);
}

} // namespace game::render
