// src/render/menu_draw.cpp — HOME screen menu drawing implementation

#include "render/menu_draw.h"

#include "core/colors.h"
#include "core/constants.h"
#include "core/res_keys.h"
#include "core/typography.h"
#include "raylib.h"

namespace game::render {

using namespace game;

namespace {

constexpr float HERO_ART_MARGIN = 8.0f;
constexpr float LOCK_OVERLAY_ALPHA = 0.6f;
constexpr float HIGHLIGHT_BORDER = 3.0f;
constexpr Color HIGHLIGHT_COLOR = {255, 215, 0, 255};
constexpr Color LOCK_OVERLAY = {0, 0, 0, 153};
constexpr Color BTN_BG = {76, 175, 80, 255};
constexpr Color BTN_BG_HOVER = {102, 187, 106, 255};

} // anonymous namespace

void drawTitle(const ResourceManager& res, float screenWidth) {
  const char* title = "Drop the Volunteer";
  Vector2 size = measureText(res, TextStyle::Title, title);
  float x = (screenWidth - size.x) / 2.0f;
  float y = 40.0f;
  drawText(res, TextStyle::Title, title, x, y, WHITE);
}

void drawHeroGrid(
  const ResourceManager& res,
  const std::vector<Hero>& heroes,
  const Account& account,
  float x,
  float y,
  int selectedIndex
) {
  (void)account; // Account available for future unlock logic

  float totalW = HERO_GRID_COLS * HERO_CARD_W + (HERO_GRID_COLS - 1) * HERO_GRID_GAP;
  float startX = x + (DESIGN_WIDTH - totalW) / 2.0f;

  for (int i = 0; i < HERO_GRID_ROWS * HERO_GRID_COLS; ++i) {
    if (i >= static_cast<int>(heroes.size()))
      break;

    const Hero& hero = heroes[i];
    int col = i % HERO_GRID_COLS;
    int row = i / HERO_GRID_COLS;

    float cardX = startX + col * (HERO_CARD_W + HERO_GRID_GAP);
    float cardY = y + row * (HERO_CARD_H + HERO_GRID_GAP);

    bool isLocked = hero.locked;
    bool isSelected = (i == selectedIndex);

    // Card background
    DrawRectangleRec({cardX, cardY, HERO_CARD_W, HERO_CARD_H}, Colors::DarkBg);

    // Hero sprite from cards.png
    if (res.hasTex(std::string(TexKeys::Cards))) {
      const Texture2D& cardsTex = res.tex(std::string(TexKeys::Cards));
      Rectangle srcRect = {
        static_cast<float>(hero.spriteX),
        static_cast<float>(hero.spriteY),
        static_cast<float>(CARD_WIDTH),
        static_cast<float>(CARD_HEIGHT)
      };

      float spriteW = HERO_CARD_W - HERO_ART_MARGIN * 2;
      float spriteH = HERO_CARD_H - HERO_ART_MARGIN * 2;
      float spriteX = cardX + HERO_ART_MARGIN;
      float spriteY = cardY + HERO_ART_MARGIN;

      Rectangle dstRect = {spriteX, spriteY, spriteW, spriteH};
      DrawTexturePro(cardsTex, srcRect, dstRect, {0, 0}, 0.0f, WHITE);
    }

    // Hero name at bottom
    Vector2 nameSize = measureText(res, TextStyle::Label, hero.name.c_str());
    float nameX = cardX + (HERO_CARD_W - nameSize.x) / 2.0f;
    float nameY = cardY + HERO_CARD_H - 18.0f;
    drawText(res, TextStyle::Label, hero.name.c_str(), nameX, nameY, WHITE);

    // Locked overlay
    if (isLocked) {
      DrawRectangleRec({cardX, cardY, HERO_CARD_W, HERO_CARD_H}, LOCK_OVERLAY);

      // Lock icon text
      const char* lockText = "LOCKED";
      Vector2 lockSize = measureText(res, TextStyle::Label, lockText);
      float lockX = cardX + (HERO_CARD_W - lockSize.x) / 2.0f;
      float lockY = cardY + (HERO_CARD_H - lockSize.y) / 2.0f - 10.0f;
      drawText(res, TextStyle::Label, lockText, lockX, lockY, WHITE);

      // Cost text
      char costText[32];
      snprintf(costText, sizeof(costText), "%d G", hero.cost);
      Vector2 costSize = measureText(res, TextStyle::Label, costText);
      float costX = cardX + (HERO_CARD_W - costSize.x) / 2.0f;
      float costY = lockY + 16.0f;
      drawText(res, TextStyle::Label, costText, costX, costY, Colors::GoldBg);
    }

    // Selected highlight border
    if (isSelected) {
      DrawRectangleLinesEx(
        {cardX - HIGHLIGHT_BORDER,
         cardY - HIGHLIGHT_BORDER,
         HERO_CARD_W + HIGHLIGHT_BORDER * 2,
         HERO_CARD_H + HIGHLIGHT_BORDER * 2},
        HIGHLIGHT_BORDER,
        HIGHLIGHT_COLOR
      );
    }
  }
}

void drawStartButton(const ResourceManager& res, const Button& btn) {
  Color bg = btn.hovered ? BTN_BG_HOVER : BTN_BG;
  DrawRectangleRec(btn.bounds, bg);

  Vector2 textSize = measureText(res, TextStyle::Heading, btn.text.c_str());
  float textX = btn.bounds.x + (btn.bounds.width - textSize.x) / 2.0f;
  float textY = btn.bounds.y + (btn.bounds.height - textSize.y) / 2.0f;
  drawText(res, TextStyle::Heading, btn.text.c_str(), textX, textY, WHITE);
}

void drawGoldDisplay(const ResourceManager& res, int gold) {
  char goldText[32];
  snprintf(goldText, sizeof(goldText), "%d G", gold);
  Vector2 size = measureText(res, TextStyle::Body, goldText);
  float x = static_cast<float>(DESIGN_WIDTH) - size.x - 16.0f;
  float y = 16.0f;
  drawText(res, TextStyle::Body, goldText, x, y, Colors::GoldBg);
}

} // namespace game::render
