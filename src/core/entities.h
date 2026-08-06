// src/core/entities.h — Entity structs that compose components
#pragma once

#include "core/components.h"
#include "core/types.h"

#include <string>
#include <vector>

namespace game {

struct PlayerState {
  HealthData health;
  ShieldData shield;
  GoldData gold;
  InventoryData inventory;
  std::string heroSlug;
  bool athelasUsed = false;
};

struct CardSlot {
  Card card;
  bool revealed;
  bool consumed = false;
};

struct VisualEffect {
  EffectType type;
  AnimState anim;
  float x, y;
  float value;
};

struct GameState {
  PlayerState player;
  std::vector<std::vector<CardSlot>> board;
  GameStatus status;
  int currentLevel;
  int currentRow;
  int currentCol;
  int goldEarned;
  std::vector<std::string> visitedBiomes;
  std::string currentBiome;
  bool inChest;
  int chestRowsRemaining;
  std::string chestType;
  std::vector<VisualEffect> effects;
};

} // namespace game
