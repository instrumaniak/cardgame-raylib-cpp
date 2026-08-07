// src/render/overlay.h — Win/Lose overlay drawing
#pragma once

#include "core/resource.h"
#include "core/types.h"

#include <string>

namespace game::render {

void drawWinOverlay(
  const ResourceManager& res,
  const std::string& heroName,
  int goldEarned,
  int totalGold,
  int score,
  Button& primaryBtn,
  Button& mainMenuBtn,
  float screenWidth,
  float screenHeight
);

void drawLoseOverlay(
  const ResourceManager& res,
  const std::string& heroName,
  int goldEarned,
  int totalGold,
  int score,
  Button& primaryBtn,
  Button& mainMenuBtn,
  float screenWidth,
  float screenHeight
);

} // namespace game::render
