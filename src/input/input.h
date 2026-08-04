#pragma once

#include "core/layout.h"
#include "raylib.h"

namespace game {

Vector2 virtualMousePosition(const Layout& layout);
void handleGlobalInput();

} // namespace game
