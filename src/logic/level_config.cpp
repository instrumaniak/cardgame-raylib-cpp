// src/logic/level_config.cpp — Level configuration implementation
// Pure game logic layer: no raylib includes
#include "logic/level_config.h"

#include "core/slugs.h"

#include <algorithm>

namespace game::logic {

LevelConfig getLevelConfig(int level) {
  // Level configs from spec/game/09-level-progression.md
  static const LevelConfig configs[6] = {
    // Level 1
    {15, {10, 15, 75}, 100, 33, 33, 33, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9}},
    // Level 2
    {15, {10, 15, 75}, 107, 31, 31, 31, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9}},
    // Level 3
    {15, {15, 20, 65}, 113, 29, 29, 29, 1, {1, 2, 3, 4, 5, 6, 7, 8, 9}},
    // Level 4
    {15, {15, 20, 65}, 113, 29, 29, 29, 1, {9, 9, 9, 10, 10, 11, 11, 11}},
    // Level 5
    {15, {15, 20, 65}, 100, 33, 33, 33, 1, {3, 4, 5, 6, 7, 8, 9, 10, 11, 12}},
    // Level 6
    {15, {25, 25, 50}, 107, 31, 31, 31, 1, {5, 6, 7, 8, 9, 10, 11, 12, 13}},
  };
  int idx = std::clamp(level - 1, 0, 5);
  return configs[idx];
}

LevelConfig getChestConfig(std::string_view chestType, int level) {
  if (chestType == Cards::GoldenChest) {
    LevelConfig cfg{3, {5, 10, 85}, 0, 15, 15, 15, 55, {}};
    if (level == 5) {
      cfg.cardsPerRow = {20, 30, 50};
    }
    return cfg;
  }
  // Regular chest (default)
  LevelConfig cfg{2, {5, 10, 85}, 0, 10, 10, 10, 70, {}};
  if (level == 5) {
    cfg.cardsPerRow = {20, 30, 50};
  }
  return cfg;
}

std::vector<std::string> getAvailableBiomes(int level, const std::vector<std::string>& visited) {
  static const std::vector<std::string> biomesByLevel[3] = {
    {std::string(Cards::Forest),
     std::string(Cards::Cave),
     std::string(Cards::EnchantedLands),
     std::string(Cards::Desert)},     // level 1
    {std::string(Cards::Abyss)},      // level 2
    {std::string(Cards::Underworld)}, // level 3
  };

  if (level < 1 || level > 3) {
    return {};
  }

  const auto& all = biomesByLevel[level - 1];

  std::vector<std::string> available;
  for (const auto& b : all) {
    if (std::find(visited.begin(), visited.end(), b) == visited.end()) {
      available.push_back(b);
    }
  }
  return available;
}

} // namespace game::logic
