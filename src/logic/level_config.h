// src/logic/level_config.h — Level configuration helpers for all 6 levels and chest sub-locations
// Pure game logic layer: no raylib includes
#pragma once

#include "core/types.h"

#include <string>
#include <string_view>
#include <vector>

namespace game::logic {

// Level configuration lookup (levels 1-6)
LevelConfig getLevelConfig(int level);

// Chest sub-location configuration (regular and golden chests)
LevelConfig getChestConfig(std::string_view chestType, int level);

// Available biomes for a level, excluding visited ones
std::vector<std::string> getAvailableBiomes(int level, const std::vector<std::string>& visited);

} // namespace game::logic
