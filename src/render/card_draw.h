// src/render/card_draw.h — Card face-up and face-down drawing functions
#pragma once

#include "core/resource.h"
#include "core/types.h"

namespace game::render {

void drawCard(
  const ResourceManager& res, const Card& card, float x, float y, float w, float h, bool revealed
);
void drawCardBack(const ResourceManager& res, float x, float y, float w, float h);

} // namespace game::render
