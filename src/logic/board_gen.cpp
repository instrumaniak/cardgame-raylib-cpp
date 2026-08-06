// src/logic/board_gen.cpp — Board generation implementation
#include "logic/board_gen.h"

#include "core/slugs.h"
#include "logic/items.h"

#include <algorithm>

namespace game::logic {

// Create a minimal card with type and value
static Card makeCard(CardType type, int value, std::string_view slug = "") {
  return Card{std::string(slug), type, value, "", "", 0, 0, 0, 0};
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
    std::string slug = weightedPick<std::string>(
      {{std::string(Cards::Chest), 80}, {std::string(Cards::GoldenChest), 20}}, rng
    );
    board[row][slot] = CardSlot{makeCard(CardType::Place, 0, slug), false};
  }

  // Step 2: Place items in rows 0..totalRows-3 (avoids last 2 rows)
  int itemCount = 1 + (level - 1) / 2;
  for (int i = 0; i < itemCount; ++i) {
    std::uniform_int_distribution<int> rowDist(0, totalRows - 3);
    std::uniform_int_distribution<int> slotDist(0, 2);
    int row = rowDist(rng);
    int slot = slotDist(rng);
    board[row][slot] = CardSlot{makeCard(CardType::Item, 0, Cards::Item), false};
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
    board[totalRows - 1][1] = CardSlot{makeCard(CardType::EndCard, 0, Cards::End), false};
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
  board[bossRow][1] = CardSlot{makeCard(CardType::Monster, bossValue, Cards::Boss), false};

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
      if (board[row][col].card.slug == Cards::Boss) {
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

Card spawnLoot(const LevelConfig& config, const std::vector<Card>& items, std::mt19937& rng) {
  int healRate = config.healRate;
  int shieldRate = config.shieldRate;
  int monsterRate = config.monsterRate;

  if (hasItem(items, Cards::Horn)) {
    healRate += 20;
  }
  if (hasItem(items, Cards::Scales)) {
    shieldRate += 20;
  }
  if (hasItem(items, Cards::Fossil)) {
    monsterRate += 10;
  }

  std::vector<std::pair<CardType, int>> rates = {
    {CardType::Monster, monsterRate},
    {CardType::Shield, shieldRate},
    {CardType::Heal, healRate},
    {CardType::Gold, config.goldRate},
  };

  CardType type = weightedPick<CardType>(rates, rng);
  int value = pickCardValue(type, config, rng);
  return makeCard(type, value);
}

} // namespace game::logic
