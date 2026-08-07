// src/render/menu_draw.h — HOME screen menu drawing (title, hero grid, start button, gold)
#pragma once

#include "core/resource.h"
#include "core/types.h"

#include <vector>

namespace game::render {

void drawTitle(const ResourceManager& res, float screenWidth);

void drawHeroGrid(
  const ResourceManager& res,
  const std::vector<Hero>& heroes,
  const Account& account,
  float x,
  float y,
  int selectedIndex
);

void drawStartButton(const ResourceManager& res, const Button& btn);

void drawGoldDisplay(const ResourceManager& res, int gold);

} // namespace game::render
