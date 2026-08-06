// src/render/board_draw.h — Board grid drawing with fog behavior
#pragma once

#include "core/entities.h"
#include "core/resource.h"

#include <vector>

namespace game::render {

void drawBoard(
  const ResourceManager& res,
  const std::vector<std::vector<CardSlot>>& board,
  int currentRow,
  float x,
  float y,
  const std::vector<Card>& items
);

} // namespace game::render
