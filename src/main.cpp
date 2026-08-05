// src/main.cpp — application entry point

#include "core/constants.h"
#include "core/layout.h"
#include "core/resource.h"
#include "core/screen.h"
#include "input/input.h"
#include "raylib.h"

int main() {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(game::DESIGN_WIDTH, game::DESIGN_HEIGHT, "Drop the Volunteer");
  if (!IsWindowReady()) {
    return 1;
  }

  InitAudioDevice();
  SetTargetFPS(game::TARGET_FPS);

  game::ResourceManager res;
  res.loadAll();

  game::Layout layout;
  layout.init();

  game::ScreenManager screens;
  screens.init();
  screens.switchTo(game::ScreenID::Home);

  while (!WindowShouldClose()) {
    float dt = GetFrameTime();
    int windowWidth = GetScreenWidth();
    int windowHeight = GetScreenHeight();

    game::handleGlobalInput();

    if (windowWidth <= 0 || windowHeight <= 0) {
      screens.update(dt);
      continue;
    }

    layout.compute(windowWidth, windowHeight);

    screens.update(dt);

    BeginTextureMode(layout.renderTexture);
    ClearBackground(BLACK);
    screens.draw();
    EndTextureMode();

    BeginDrawing();
    ClearBackground(BLACK);
    DrawTexturePro(
      layout.renderTexture.texture,
      {
        0.0f,
        0.0f,
        static_cast<float>(layout.renderTexture.texture.width),
        -static_cast<float>(layout.renderTexture.texture.height),
      },
      layout.dstRect,
      {0.0f, 0.0f},
      0.0f,
      WHITE
    );
    EndDrawing();
  }

  res.unloadAll();
  UnloadRenderTexture(layout.renderTexture);
  CloseAudioDevice();
  CloseWindow();
  return 0;
}
