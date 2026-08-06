// src/render/board_draw.cpp — Board grid drawing with fog behavior

#include "render/board_draw.h"

#include "core/colors.h"
#include "core/constants.h"
#include "logic/fog.h"
#include "logic/items.h"
#include "raylib.h"
#include "render/card_draw.h"

namespace game::render {

using namespace game;

namespace {

constexpr float CARD_GAP = 8.0f;
constexpr float HIGHLIGHT_PAD = 4.0f;

} // anonymous namespace

void drawBoard(
  const ResourceManager& res,
  const std::vector<std::vector<CardSlot>>& board,
  int currentRow,
  float x,
  float y,
  const std::vector<Card>& items
) {
  int nbRowsHidden = logic::getNbRowsHidden(items);
  int totalRows = static_cast<int>(board.size());
  float slotW = static_cast<float>(CARD_WIDTH);
  float slotH = static_cast<float>(CARD_HEIGHT);

  for (int row = 0; row < totalRows; ++row) {
    if (!logic::rowVisible(row, currentRow)) {
      continue;
    }

    bool fogged = logic::rowFog(row, currentRow, nbRowsHidden);
    bool isActive = (row == currentRow);
    float rowY = y + static_cast<float>(row - currentRow) * static_cast<float>(ROW_HEIGHT);

    // Active row highlight
    if (isActive) {
      float totalW = SLOTS_PER_ROW * slotW + (SLOTS_PER_ROW - 1) * CARD_GAP;
      DrawRectangleRec(
        {x - HIGHLIGHT_PAD,
         rowY - HIGHLIGHT_PAD,
         totalW + HIGHLIGHT_PAD * 2,
         slotH + HIGHLIGHT_PAD * 2},
        Fade(Colors::GoldBg, 0.3f)
      );
    }

    int cols = static_cast<int>(board[row].size());
    for (int col = 0; col < cols; ++col) {
      const CardSlot& slot = board[row][col];
      float cardX = x + col * (slotW + CARD_GAP);

      if (slot.consumed) {
        DrawRectangleRec({cardX, rowY, slotW, slotH}, Fade(Colors::DarkBg, 0.5f));
      } else {
        drawCard(res, slot.card, cardX, rowY, slotW, slotH, !fogged);
      }
    }
  }
}

} // namespace game::render
