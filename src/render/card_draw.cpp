// src/render/card_draw.cpp — Card face-up and face-down drawing functions

#include "render/card_draw.h"

#include "core/colors.h"
#include "core/res_keys.h"
#include "core/typography.h"
#include "raylib.h"

#include <algorithm>

namespace game::render {

using namespace game;

namespace {

Color getBgColor(CardType type) {
  switch (type) {
  case CardType::Monster:
    return Colors::MonsterBg;
  case CardType::Heal:
    return Colors::HealBg;
  case CardType::Shield:
    return Colors::ShieldBg;
  case CardType::Gold:
    return Colors::GoldBg;
  case CardType::Item:
  case CardType::Drop:
  case CardType::Biome:
    return Colors::DarkBg;
  case CardType::Place:
    return Colors::PlaceBg;
  case CardType::EndCard:
    return Colors::EndCardBg;
  default:
    return Colors::DarkBg;
  }
}

std::string_view getFrameKey(CardType type) {
  switch (type) {
  case CardType::Monster:
    return TexKeys::CardFramePurple;
  case CardType::Heal:
    return TexKeys::CardFrameRed;
  case CardType::Shield:
    return TexKeys::CardFrameGray;
  case CardType::Gold:
    return TexKeys::CardFrameYellow;
  case CardType::Item:
  case CardType::Drop:
  case CardType::Biome:
    return TexKeys::HeroFrame;
  case CardType::Place:
    return TexKeys::ChestFrame;
  case CardType::EndCard:
    return TexKeys::LightFrame;
  default:
    return TexKeys::HeroFrame;
  }
}

} // anonymous namespace

void drawCard(
  const ResourceManager& res, const Card& card, float x, float y, float w, float h, bool revealed
) {
  if (!revealed) {
    drawCardBack(res, x, y, w, h);
    return;
  }

  // 1. Background fill
  DrawRectangleRec({x, y, w, h}, getBgColor(card.cardType));

  // 2. Card sprite from cards.png
  if (res.hasTex(std::string(TexKeys::Cards)) && card.spriteW > 0 && card.spriteH > 0) {
    const Texture2D& cardsTex = res.tex(std::string(TexKeys::Cards));
    Rectangle srcRect = {
      static_cast<float>(card.spriteX),
      static_cast<float>(card.spriteY),
      static_cast<float>(card.spriteW),
      static_cast<float>(card.spriteH)
    };

    // Center sprite within card with margin for frame
    float margin = 8.0f;
    float spriteAreaW = w - margin * 2;
    float spriteAreaH = h - margin * 2;
    float scale = std::min(spriteAreaW / card.spriteW, spriteAreaH / card.spriteH);
    float spriteW = card.spriteW * scale;
    float spriteH = card.spriteH * scale;
    float spriteX = x + (w - spriteW) / 2.0f;
    float spriteY = y + (h - spriteH) / 2.0f;

    Rectangle dstRect = {spriteX, spriteY, spriteW, spriteH};
    DrawTexturePro(cardsTex, srcRect, dstRect, {0, 0}, 0.0f, WHITE);
  }

  // 3. Frame overlay
  std::string_view frameKey = getFrameKey(card.cardType);
  if (res.hasTex(std::string(frameKey))) {
    const Texture2D& frameTex = res.tex(std::string(frameKey));
    Rectangle srcRect = {
      0, 0, static_cast<float>(frameTex.width), static_cast<float>(frameTex.height)
    };
    Rectangle dstRect = {x, y, w, h};
    DrawTexturePro(frameTex, srcRect, dstRect, {0, 0}, 0.0f, WHITE);
  }

  // 4. Card name at top
  if (card.name.size() > 0) {
    drawText(res, TextStyle::Label, card.name.c_str(), x + 4, y + 4, WHITE);
  }

  // 5. Card value at bottom
  const char* valueText = TextFormat("%d", card.value);
  Vector2 valueSize = measureText(res, TextStyle::CardStat, valueText);
  drawText(res, TextStyle::CardStat, valueText, x + (w - valueSize.x) / 2, y + h - 18, WHITE);
}

void drawCardBack(const ResourceManager& res, float x, float y, float w, float h) {
  // Dark background
  DrawRectangleRec({x, y, w, h}, Colors::DarkBg);

  // Card-back overlay
  if (res.hasTex(std::string(TexKeys::CardBack))) {
    const Texture2D& backTex = res.tex(std::string(TexKeys::CardBack));
    Rectangle srcRect = {
      0, 0, static_cast<float>(backTex.width), static_cast<float>(backTex.height)
    };
    Rectangle dstRect = {x, y, w, h};
    DrawTexturePro(backTex, srcRect, dstRect, {0, 0}, 0.0f, WHITE);
  }
}

} // namespace game::render
