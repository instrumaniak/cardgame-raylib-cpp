// src/render/overlay.cpp — Win/Lose overlay drawing implementation

#include "render/overlay.h"

#include "core/colors.h"
#include "core/constants.h"
#include "core/typography.h"
#include "raylib.h"

namespace game::render {

using namespace game;

namespace {

constexpr Color OVERLAY_COLOR = {0, 0, 0, 150};
constexpr Color BTN_BG = {76, 175, 80, 255};
constexpr Color BTN_BG_HOVER = {102, 187, 106, 255};
constexpr float BTN_WIDTH = 160.0f;
constexpr float BTN_HEIGHT = 40.0f;
constexpr float BTN_GAP = 20.0f;
constexpr float STAT_LINE_HEIGHT = 30.0f;

struct OverlayConfig {
  const char* title;
  Color titleColor;
  int displayGoldEarned;
};

void drawOverlay(
  const ResourceManager& res,
  const OverlayConfig& config,
  const std::string& heroName,
  int totalGold,
  int score,
  Button& primaryBtn,
  Button& mainMenuBtn,
  float screenWidth,
  float screenHeight
) {
  DrawRectangleRec({0, 0, screenWidth, screenHeight}, OVERLAY_COLOR);

  Vector2 titleSize = measureText(res, TextStyle::Title, config.title);
  float titleX = (screenWidth - titleSize.x) / 2.0f;
  float titleY = screenHeight * 0.15f;
  drawText(res, TextStyle::Title, config.title, titleX, titleY, config.titleColor);

  float statY = titleY + titleSize.y + 30.0f;

  auto drawStatLine = [&](const char* text, Color color) {
    Vector2 sz = measureText(res, TextStyle::Body, text);
    drawText(res, TextStyle::Body, text, (screenWidth - sz.x) / 2.0f, statY, color);
    statY += STAT_LINE_HEIGHT;
  };

  {
    char buf[64];
    snprintf(buf, sizeof(buf), "Hero: %s", heroName.c_str());
    drawStatLine(buf, WHITE);
  }
  {
    char buf[64];
    snprintf(buf, sizeof(buf), "Gold Earned: %d", config.displayGoldEarned);
    drawStatLine(buf, Colors::GoldBg);
  }
  {
    char buf[64];
    snprintf(buf, sizeof(buf), "Total Gold: %d", totalGold);
    drawStatLine(buf, Colors::GoldBg);
  }
  {
    char buf[64];
    snprintf(buf, sizeof(buf), "Score: %d", score);
    drawStatLine(buf, WHITE);
  }

  float totalBtnWidth = BTN_WIDTH * 2 + BTN_GAP;
  float btnX = (screenWidth - totalBtnWidth) / 2.0f;
  float btnY = statY + 20.0f;

  primaryBtn.bounds = {btnX, btnY, BTN_WIDTH, BTN_HEIGHT};
  mainMenuBtn.bounds = {btnX + BTN_WIDTH + BTN_GAP, btnY, BTN_WIDTH, BTN_HEIGHT};

  auto drawBtn = [&](const Button& btn) {
    Color bg = btn.hovered ? BTN_BG_HOVER : BTN_BG;
    DrawRectangleRec(btn.bounds, bg);
    Vector2 ts = measureText(res, TextStyle::Heading, btn.text.c_str());
    float tx = btn.bounds.x + (btn.bounds.width - ts.x) / 2.0f;
    float ty = btn.bounds.y + (btn.bounds.height - ts.y) / 2.0f;
    drawText(res, TextStyle::Heading, btn.text.c_str(), tx, ty, WHITE);
  };

  drawBtn(primaryBtn);
  drawBtn(mainMenuBtn);
}

} // anonymous namespace

void drawWinOverlay(
  const ResourceManager& res,
  const std::string& heroName,
  int goldEarned,
  int totalGold,
  int score,
  Button& primaryBtn,
  Button& mainMenuBtn,
  float screenWidth,
  float screenHeight
) {
  OverlayConfig config = {"Victory!", Colors::GoldBg, goldEarned};
  drawOverlay(
    res, config, heroName, totalGold, score, primaryBtn, mainMenuBtn, screenWidth, screenHeight
  );
}

void drawLoseOverlay(
  const ResourceManager& res,
  const std::string& heroName,
  int goldEarned,
  int totalGold,
  int score,
  Button& primaryBtn,
  Button& mainMenuBtn,
  float screenWidth,
  float screenHeight
) {
  int displayGold = (goldEarned + 1) / 2;
  OverlayConfig config = {"Defeat", RED, displayGold};
  drawOverlay(
    res, config, heroName, totalGold, score, primaryBtn, mainMenuBtn, screenWidth, screenHeight
  );
}

} // namespace game::render
