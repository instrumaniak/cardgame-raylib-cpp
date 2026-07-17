#include "raylib.h"

int main(void) {
  int screenWidth = 800;
  int screenHeight = 450;
  const char *windowTitle = "Raylib Card Game";
  const char *greetingText = "Hello World! Forward!";
  const int fontSize = 20;

  SetConfigFlags(FLAG_WINDOW_RESIZABLE);
  InitWindow(screenWidth, screenHeight, windowTitle);
  SetTargetFPS(60);

  while (!WindowShouldClose()) {
    screenWidth = GetScreenWidth();
    screenHeight = GetScreenHeight();

    int textLength = MeasureText(greetingText, fontSize);
    int px = (screenWidth - textLength) / 2;
    int py = (screenHeight - fontSize) / 2;

    BeginDrawing();
    ClearBackground(BLACK);
    DrawText(greetingText, px, py, fontSize, LIGHTGRAY);
    DrawText(TextFormat("%i", GetFPS()), 10, 10, fontSize, DARKGRAY);
    EndDrawing();
  }

  CloseWindow();

  return 0;
}