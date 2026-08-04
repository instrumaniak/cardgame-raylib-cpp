#include "input/input.h"

namespace game {

Vector2 virtualMousePosition(const Layout& layout) {
  return layout.screenToVirtual(GetMousePosition());
}

void handleGlobalInput() {
  bool isAltDown = IsKeyDown(KEY_LEFT_ALT) || IsKeyDown(KEY_RIGHT_ALT);

  if (IsKeyPressed(KEY_F11) || (isAltDown && IsKeyPressed(KEY_ENTER))) {
    ToggleFullscreen();
  }
}

} // namespace game
