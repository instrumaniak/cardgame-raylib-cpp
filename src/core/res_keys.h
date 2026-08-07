// src/core/res_keys.h — ResourceManager asset key constants
#pragma once

#include <string_view>

namespace game::TexKeys {
inline constexpr std::string_view Cards = "cards";
inline constexpr std::string_view CardBack = "card-back";
inline constexpr std::string_view CardFramePurple = "card-frame-purple";
inline constexpr std::string_view CardFrameRed = "card-frame-red";
inline constexpr std::string_view CardFrameGray = "card-frame-gray";
inline constexpr std::string_view CardFrameYellow = "card-frame-yellow";
inline constexpr std::string_view HeroFrame = "hero-frame";
inline constexpr std::string_view ChestFrame = "chest-frame";
inline constexpr std::string_view LightFrame = "light-frame";
} // namespace game::TexKeys

namespace game::FntKeys {
inline constexpr std::string_view PearSoda = "PearSoda";
inline constexpr std::string_view Zepto = "Zepto-Regular";
} // namespace game::FntKeys
