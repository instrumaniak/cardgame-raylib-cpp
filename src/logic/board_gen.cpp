// src/logic/board_gen.cpp — Board generation implementation
#include "logic/board_gen.h"

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

LevelConfig getChestConfig(const std::string& chestType, int level) {
  if (chestType == "golden_chest") {
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
    {"forest", "cave", "enchanted_lands", "desert"}, // level 1
    {"abyss"},                                       // level 2
    {"underworld"},                                  // level 3
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

// Create a minimal card with type and value
static Card makeCard(CardType type, int value, const std::string& slug = "") {
  return Card{slug, type, value, "", "", 0, 0, 0, 0};
}

// Pick a random card type based on level rates (excluding monsters for first row)
static CardType
pickCardType(const LevelConfig& config, std::mt19937& rng, bool excludeMonsters = false) {
  std::vector<std::pair<CardType, int>> rates = {
    {CardType::Shield, config.shieldRate},
    {CardType::Heal, config.healRate},
    {CardType::Gold, config.goldRate},
    {CardType::Item, config.itemRate},
  };
  if (!excludeMonsters) {
    rates.insert(rates.begin(), {CardType::Monster, config.monsterRate});
  }
  return weightedPick<CardType>(rates, rng);
}

// Pick a random value for a card type
static int pickCardValue(CardType type, const LevelConfig& config, std::mt19937& rng) {
  if (type == CardType::Monster) {
    std::uniform_int_distribution<int> dist(0, static_cast<int>(config.monsterValues.size()) - 1);
    return config.monsterValues[dist(rng)];
  }
  // Shield, Heal, Gold: random 1-9
  std::uniform_int_distribution<int> dist(1, 9);
  return dist(rng);
}

GeneratedBoard generateBoard(
  const LevelConfig& config,
  int level,
  const std::vector<std::string>& availableBiomes,
  std::mt19937& rng
) {
  int totalRows = config.rows;

  // Step 0: Create empty board (rows x 3)
  std::vector<std::vector<CardSlot>> board(totalRows, std::vector<CardSlot>(3));

  // Step 1: Place chests in rows 0..totalRows-4 (avoids last 3 rows)
  int chestCount = 1 + (level - 1) / 2;
  for (int i = 0; i < chestCount; ++i) {
    std::uniform_int_distribution<int> rowDist(0, totalRows - 4);
    std::uniform_int_distribution<int> slotDist(0, 2);
    int row = rowDist(rng);
    int slot = slotDist(rng);
    std::string slug = weightedPick<std::string>({{"chest", 80}, {"golden_chest", 20}}, rng);
    board[row][slot] = CardSlot{makeCard(CardType::Place, 0, slug), false};
  }

  // Step 2: Place items in rows 0..totalRows-3 (avoids last 2 rows)
  int itemCount = 1 + (level - 1) / 2;
  for (int i = 0; i < itemCount; ++i) {
    std::uniform_int_distribution<int> rowDist(0, totalRows - 3);
    std::uniform_int_distribution<int> slotDist(0, 2);
    int row = rowDist(rng);
    int slot = slotDist(rng);
    board[row][slot] = CardSlot{makeCard(CardType::Item, 0, "item"), false};
  }

  // Step 3: Fill first row (row 0) — no monsters
  for (int col = 0; col < 3; ++col) {
    CardType ct = board[0][col].card.cardType;
    if (ct == CardType::Place || ct == CardType::Item) {
      continue; // already placed by chest/item step
    }
    CardType type = pickCardType(config, rng, true);
    int value = pickCardValue(type, config, rng);
    board[0][col] = CardSlot{makeCard(type, value), false};
  }

  // Step 4: Fill last row — biome cards or end card on level 6
  GeneratedBoard result;
  result.hasEndCard = (level == 6);

  if (level == 6) {
    board[totalRows - 1][1] = CardSlot{makeCard(CardType::EndCard, 0, "end"), false};
    result.biome = "";
  } else {
    result.biome = availableBiomes.empty() ? "" : availableBiomes[0];
    int biomeCount = std::min(static_cast<int>(availableBiomes.size()), 3);
    if (biomeCount == 1) {
      board[totalRows - 1][1] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[0]), false};
    } else if (biomeCount == 2) {
      board[totalRows - 1][0] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[0]), false};
      board[totalRows - 1][2] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[1]), false};
    } else {
      board[totalRows - 1][0] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[0]), false};
      board[totalRows - 1][1] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[1]), false};
      board[totalRows - 1][2] = CardSlot{makeCard(CardType::Biome, 0, availableBiomes[2]), false};
    }
  }

  // Step 5: Second-to-last row — BOSS monster in center slot
  int bossRow = totalRows - 2;
  int bossValue = config.monsterValues.back();
  board[bossRow][1] = CardSlot{makeCard(CardType::Monster, bossValue, "boss"), false};

  // Step 6: Fill remaining normal rows with weighted random
  // Rows 1 to totalRows-3 (inclusive), skipping already-filled slots
  for (int row = 1; row <= totalRows - 3; ++row) {
    // Determine number of cards: 1, 2, or 3
    std::vector<std::pair<int, int>> cardCountRates = {
      {1, config.cardsPerRow[0]},
      {2, config.cardsPerRow[1]},
      {3, config.cardsPerRow[2]},
    };
    int cardCount = weightedPick<int>(cardCountRates, rng);

    // Pick positions for cards (only empty/default slots)
    std::vector<int> emptySlots;
    for (int col = 0; col < 3; ++col) {
      auto ct = board[row][col].card.cardType;
      if (ct == CardType::Place || ct == CardType::Item) {
        continue; // don't overwrite chests/items
      }
      if (board[row][col].card.slug == "boss") {
        continue; // don't overwrite boss
      }
      emptySlots.push_back(col);
    }

    // Shuffle and pick up to cardCount positions
    std::shuffle(emptySlots.begin(), emptySlots.end(), rng);
    int toPlace = std::min(cardCount, static_cast<int>(emptySlots.size()));

    for (int i = 0; i < toPlace; ++i) {
      int col = emptySlots[i];
      CardType type = pickCardType(config, rng);
      int value = pickCardValue(type, config, rng);
      board[row][col] = CardSlot{makeCard(type, value), false};
    }
  }

  result.slots = std::move(board);
  return result;
}

} // namespace game::logic
