// src/logic/board_gen.h — Board generation from LevelConfig
// Pure game logic layer: no raylib includes
#pragma once

#include "core/entities.h"
#include "core/types.h"

#include <random>
#include <string>
#include <utility>
#include <vector>

namespace game::logic {

struct GeneratedBoard {
  std::vector<std::vector<CardSlot>> slots;
  std::string biome;
  bool hasEndCard = false;
};

// Weighted random selection using cumulative distribution
template <typename T>
T weightedPick(const std::vector<std::pair<T, int>>& rates, std::mt19937& rng) {
  int total = 0;
  for (const auto& [val, weight] : rates) {
    total += weight;
  }
  std::uniform_int_distribution<int> dist(0, total - 1);
  int roll = dist(rng);
  int cumulative = 0;
  for (const auto& [val, weight] : rates) {
    cumulative += weight;
    if (roll < cumulative) {
      return val;
    }
  }
  return rates.back().first; // unreachable — dist is [0, total-1]
}

// Level configuration lookup
LevelConfig getLevelConfig(int level);

// Chest sub-location configuration
LevelConfig getChestConfig(const std::string& chestType, int level);

// Available biomes for a level, excluding visited ones
std::vector<std::string> getAvailableBiomes(int level, const std::vector<std::string>& visited);

// Generate a full board for a level
GeneratedBoard generateBoard(
  const LevelConfig& config,
  int level,
  const std::vector<std::string>& availableBiomes,
  std::mt19937& rng
);

} // namespace game::logic
