// src/core/typography.h — Centralized typography system with typed text styles
#pragma once

#include "raylib.h"

namespace game {

class ResourceManager;

namespace render {

enum class TextStyle {
  Title,    // PearSoda 64px — end-game titles
  Heading,  // Zepto 24px — subtitles, section headers
  Body,     // Zepto 16px — default text
  CardStat, // PearSoda 32px — card values, hero stats
  Label,    // Zepto 12px — small UI text
};

void drawText(
  const ResourceManager& res, TextStyle style, const char* text, float x, float y, Color color
);

Vector2 measureText(const ResourceManager& res, TextStyle style, const char* text);

} // namespace render
} // namespace game
