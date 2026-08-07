// src/render/hero_panel.cpp — Hero panel drawing (left side of game screen)

#include "render/hero_panel.h"

#include "core/colors.h"
#include "core/constants.h"
#include "core/res_keys.h"
#include "core/typography.h"
#include "raylib.h"

namespace game::render {

using namespace game;

namespace {

constexpr float PANEL_WIDTH = 260.0f;
constexpr float HERO_ART_X_OFFSET = 90.0f;
constexpr float HERO_ART_Y_OFFSET = 10.0f;
constexpr float HEALTH_BAR_WIDTH = 200.0f;
constexpr float HEALTH_BAR_HEIGHT = 16.0f;
constexpr float SHIELD_BAR_HEIGHT = 12.0f;
constexpr float ITEM_SIZE = 32.0f;
constexpr float ITEM_GAP = 4.0f;
constexpr float SECTION_GAP = 12.0f;

constexpr Color HEALTH_GREEN = {76, 175, 80, 255};
constexpr Color HEALTH_YELLOW = {255, 193, 7, 255};
constexpr Color HEALTH_RED = {244, 67, 54, 255};
constexpr Color BAR_BG = {50, 50, 60, 255};
constexpr Color SHIELD_BLUE = {66, 165, 245, 255};

Color healthBarColor(float ratio) {
  if (ratio > 0.6f)
    return HEALTH_GREEN;
  if (ratio > 0.3f)
    return HEALTH_YELLOW;
  return HEALTH_RED;
}

} // anonymous namespace

void drawHeroPanel(
  const ResourceManager& res, const PlayerState& player, const Hero& hero, float x, float y
) {
  // Panel background
  DrawRectangleRec({x, y, PANEL_WIDTH, DESIGN_HEIGHT - y}, Colors::DarkBg);

  float cursorY = y + HERO_ART_Y_OFFSET;

  // Hero card art
  if (res.hasTex(std::string(TexKeys::Cards))) {
    const Texture2D& cardsTex = res.tex(std::string(TexKeys::Cards));
    Rectangle srcRect = {
      static_cast<float>(hero.spriteX),
      static_cast<float>(hero.spriteY),
      static_cast<float>(CARD_WIDTH),
      static_cast<float>(CARD_HEIGHT)
    };
    Rectangle destRect = {
      x + HERO_ART_X_OFFSET,
      cursorY,
      static_cast<float>(CARD_WIDTH),
      static_cast<float>(CARD_HEIGHT)
    };
    DrawTexturePro(cardsTex, srcRect, destRect, {0, 0}, 0.0f, WHITE);
  }

  cursorY += CARD_HEIGHT + SECTION_GAP;

  // Hero name
  const char* nameText = hero.name.c_str();
  Vector2 nameSize = measureText(res, TextStyle::Body, nameText);
  drawText(res, TextStyle::Body, nameText, x + (PANEL_WIDTH - nameSize.x) / 2, cursorY, WHITE);
  cursorY += 28 + SECTION_GAP;

  // Health bar
  float healthRatio = (player.health.max > 0) ? static_cast<float>(player.health.current) /
                                                  static_cast<float>(player.health.max)
                                              : 0.0f;
  if (healthRatio < 0.0f)
    healthRatio = 0.0f;
  if (healthRatio > 1.0f)
    healthRatio = 1.0f;

  float barX = x + (PANEL_WIDTH - HEALTH_BAR_WIDTH) / 2;

  DrawRectangleRec({barX, cursorY, HEALTH_BAR_WIDTH, HEALTH_BAR_HEIGHT}, BAR_BG);
  DrawRectangleRec(
    {barX, cursorY, HEALTH_BAR_WIDTH * healthRatio, HEALTH_BAR_HEIGHT}, healthBarColor(healthRatio)
  );

  char healthText[32];
  snprintf(healthText, sizeof(healthText), "%d/%d", player.health.current, player.health.max);
  Vector2 healthSize = measureText(res, TextStyle::Label, healthText);
  drawText(
    res,
    TextStyle::Label,
    healthText,
    barX + (HEALTH_BAR_WIDTH - healthSize.x) / 2,
    cursorY + 1,
    WHITE
  );

  cursorY += HEALTH_BAR_HEIGHT + SECTION_GAP;

  // Shield indicator
  if (player.shield.value > 0) {
    float shieldBarWidth = HEALTH_BAR_WIDTH * 0.6f;
    float barSX = x + (PANEL_WIDTH - shieldBarWidth) / 2;

    DrawRectangleRec({barSX, cursorY, shieldBarWidth, SHIELD_BAR_HEIGHT}, BAR_BG);
    DrawRectangleRec({barSX, cursorY, shieldBarWidth, SHIELD_BAR_HEIGHT}, SHIELD_BLUE);

    char shieldText[32];
    snprintf(shieldText, sizeof(shieldText), "Shield: %d", player.shield.value);
    Vector2 shieldSize = measureText(res, TextStyle::Label, shieldText);
    drawText(
      res,
      TextStyle::Label,
      shieldText,
      barSX + (shieldBarWidth - shieldSize.x) / 2,
      cursorY + 1,
      WHITE
    );

    cursorY += SHIELD_BAR_HEIGHT + SECTION_GAP;
  }

  // Gold display
  char goldText[32];
  snprintf(goldText, sizeof(goldText), "Gold: %d", player.gold.amount);
  Vector2 goldSize = measureText(res, TextStyle::Label, goldText);
  drawText(
    res, TextStyle::Label, goldText, x + (PANEL_WIDTH - goldSize.x) / 2, cursorY, Colors::GoldBg
  );

  cursorY += 22 + SECTION_GAP;

  // Inventory
  const auto& items = player.inventory.items;
  if (!items.empty()) {
    Vector2 invLabelSize = measureText(res, TextStyle::Label, "Inventory");
    drawText(
      res, TextStyle::Label, "Inventory", x + (PANEL_WIDTH - invLabelSize.x) / 2, cursorY, WHITE
    );
    cursorY += 18;

    float totalInvWidth = static_cast<float>(items.size()) * ITEM_SIZE +
                          static_cast<float>(items.size() - 1) * ITEM_GAP;
    float invStartX = x + (PANEL_WIDTH - totalInvWidth) / 2;

    if (res.hasTex(std::string(TexKeys::Cards))) {
      // Clip inventory to panel bounds
      BeginScissorMode(
        static_cast<int>(x),
        static_cast<int>(cursorY),
        static_cast<int>(PANEL_WIDTH),
        static_cast<int>(ITEM_SIZE)
      );
      const Texture2D& cardsTex = res.tex(std::string(TexKeys::Cards));
      for (size_t i = 0; i < items.size(); ++i) {
        const Card& item = items[i];
        Rectangle itemSrc = {
          static_cast<float>(item.spriteX),
          static_cast<float>(item.spriteY),
          static_cast<float>(item.spriteW),
          static_cast<float>(item.spriteH)
        };
        Rectangle itemDest = {
          invStartX + static_cast<float>(i) * (ITEM_SIZE + ITEM_GAP), cursorY, ITEM_SIZE, ITEM_SIZE
        };
        DrawTexturePro(cardsTex, itemSrc, itemDest, {0, 0}, 0.0f, WHITE);
      }
      EndScissorMode();
    }
  }
}

} // namespace game::render
