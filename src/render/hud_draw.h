// src/render/hud_draw.h — HUD drawing (level indicator, gold earned, screen shake)
#pragma once

#include "core/entities.h"
#include "core/resource.h"
#include "core/types.h"

#include <vector>

namespace game::render {

void drawHud(
  const ResourceManager& res, int level, int goldEarned, const std::vector<VisualEffect>& effects
);

} // namespace game::render
