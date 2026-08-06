// src/core/colors.h — Named color constants for UI and rendering
#pragma once

#include "raylib.h"

namespace game::Colors {

// Card type background colors (from specs/game/02-card-types.md)
inline constexpr Color MonsterBg = {83, 66, 141, 255}; // #53428d
inline constexpr Color HealBg = {217, 71, 67, 255};    // #d94743
inline constexpr Color ShieldBg = {97, 97, 97, 255};   // #616161
inline constexpr Color GoldBg = {251, 192, 45, 255};   // #fbc02d
inline constexpr Color DarkBg = {30, 30, 40, 255};     // Item/Drop/Biome + card-back
inline constexpr Color PlaceBg = {243, 209, 159, 255}; // #f3d19f
inline constexpr Color EndCardBg = {255, 215, 0, 255}; // gold glow

} // namespace game::Colors
