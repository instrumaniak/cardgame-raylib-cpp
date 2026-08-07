// src/core/typography.cpp — Typography system implementation

#include "core/typography.h"

#include "core/res_keys.h"
#include "core/resource.h"
#include "raylib.h"

namespace game::render {

namespace {

struct StyleDef {
  std::string_view fontKey;
  float fontSize;
};

StyleDef getStyleDef(TextStyle style) {
  switch (style) {
  case TextStyle::Title:
    return {FntKeys::PearSoda, 64.0f};
  case TextStyle::Heading:
    return {FntKeys::Zepto, 24.0f};
  case TextStyle::Body:
    return {FntKeys::Zepto, 16.0f};
  case TextStyle::CardStat:
    return {FntKeys::PearSoda, 32.0f};
  case TextStyle::Label:
    return {FntKeys::Zepto, 12.0f};
  }
  return {FntKeys::Zepto, 16.0f};
}

} // anonymous namespace

void drawText(
  const ResourceManager& res, TextStyle style, const char* text, float x, float y, Color color
) {
  StyleDef def = getStyleDef(style);
  if (!res.hasFnt(std::string(def.fontKey))) {
    return;
  }
  const Font& font = res.fnt(std::string(def.fontKey));
  DrawTextEx(font, text, {x, y}, def.fontSize, 0.0f, color);
}

Vector2 measureText(const ResourceManager& res, TextStyle style, const char* text) {
  StyleDef def = getStyleDef(style);
  if (!res.hasFnt(std::string(def.fontKey))) {
    return {0.0f, 0.0f};
  }
  const Font& font = res.fnt(std::string(def.fontKey));
  return MeasureTextEx(font, text, def.fontSize, 0.0f);
}

} // namespace game::render
