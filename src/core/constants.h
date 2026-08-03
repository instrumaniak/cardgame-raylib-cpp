// src/core/constants.h — immutable game-wide compile-time constants
#pragma once

namespace game {

// Display
constexpr int DESIGN_WIDTH = 960;
constexpr int DESIGN_HEIGHT = 540;
constexpr int INITIAL_WINDOW_WIDTH = 1280;
constexpr int INITIAL_WINDOW_HEIGHT = 720;
constexpr int TARGET_FPS = 60;

// Board layout (virtual pixels)
constexpr int CARD_WIDTH = 80;
constexpr int CARD_HEIGHT = 112;
constexpr int ROW_HEIGHT = 120;
constexpr int SLOTS_PER_ROW = 3;

} // namespace game
