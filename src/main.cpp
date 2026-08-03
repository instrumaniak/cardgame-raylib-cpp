// main.cpp — The Entry point

#include "raylib.h"
#include "core/constants.h"

int main(void) {
  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(game::DESIGN_WIDTH, game::DESIGN_HEIGHT, "Drop the Volunteer");
  SetTargetFPS(game::TARGET_FPS);

  while (!WindowShouldClose()) {
    BeginDrawing();
    ClearBackground(BLACK);
    DrawText("Drop the Volunteer", 10, 10, 20, LIGHTGRAY);
    DrawText(TextFormat("%i", GetFPS()), 10, 40, 20, DARKGRAY);
    EndDrawing();
  }

  CloseWindow();
  return 0;
}
