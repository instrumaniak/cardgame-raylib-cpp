// src/render/hero_panel.h — Hero panel drawing (left side of game screen)
#pragma once

#include "core/entities.h"
#include "core/resource.h"
#include "core/types.h"

namespace game::render {

void drawHeroPanel(
  const ResourceManager& res, const PlayerState& player, const Hero& hero, float x, float y
);

} // namespace game::render
