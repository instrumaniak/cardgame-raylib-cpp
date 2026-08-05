// src/logic/test_board_gen.cpp — Unit tests for board generation
#include "doctest/doctest.h"
#include "logic/board_gen.h"

#include <map>
#include <set>

using namespace game;
using namespace game::logic;

TEST_CASE("getLevelConfig returns correct configs") {
  SUBCASE("Level 1") {
    LevelConfig cfg = getLevelConfig(1);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 100);
    CHECK(cfg.shieldRate == 33);
    CHECK(cfg.healRate == 33);
    CHECK(cfg.goldRate == 33);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 10);
    CHECK(cfg.cardsPerRow[1] == 15);
    CHECK(cfg.cardsPerRow[2] == 75);
    CHECK(cfg.monsterValues.size() == 9);
  }

  SUBCASE("Level 4") {
    LevelConfig cfg = getLevelConfig(4);
    CHECK(cfg.monsterRate == 113);
    CHECK(cfg.monsterValues == std::vector<int>{9, 9, 9, 10, 10, 11, 11, 11});
  }

  SUBCASE("Level 6") {
    LevelConfig cfg = getLevelConfig(6);
    CHECK(cfg.cardsPerRow[0] == 25);
    CHECK(cfg.cardsPerRow[1] == 25);
    CHECK(cfg.cardsPerRow[2] == 50);
    CHECK(cfg.monsterValues.size() == 9);
  }
}

TEST_CASE("getAvailableBiomes") {
  SUBCASE("Level 1 — no visited") {
    auto biomes = getAvailableBiomes(1, {});
    CHECK(biomes.size() == 4);
    CHECK(biomes[0] == "forest");
    CHECK(biomes[1] == "cave");
    CHECK(biomes[2] == "enchanted_lands");
    CHECK(biomes[3] == "desert");
  }

  SUBCASE("Level 1 — one visited") {
    auto biomes = getAvailableBiomes(1, {"forest"});
    CHECK(biomes.size() == 3);
  }

  SUBCASE("Level 2") {
    auto biomes = getAvailableBiomes(2, {});
    CHECK(biomes.size() == 1);
    CHECK(biomes[0] == "abyss");
  }

  SUBCASE("Level 3 — all visited") {
    auto biomes = getAvailableBiomes(3, {"underworld"});
    CHECK(biomes.empty());
  }
}

TEST_CASE("weightedPick returns values proportional to rates") {
  std::mt19937 rng(42);
  std::map<std::string, int> counts;
  int trials = 10000;

  for (int i = 0; i < trials; ++i) {
    std::string val = weightedPick<std::string>({{"a", 70}, {"b", 20}, {"c", 10}}, rng);
    counts[val]++;
  }

  CHECK(counts["a"] > 6000);
  CHECK(counts["b"] > 1500);
  CHECK(counts["b"] < 2500);
  CHECK(counts["c"] > 500);
  CHECK(counts["c"] < 1500);
}

TEST_CASE("generateBoard — correct dimensions") {
  std::mt19937 rng(42);
  LevelConfig cfg = getLevelConfig(1);
  auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);

  CHECK(board.slots.size() == 15);
  for (const auto& row : board.slots) {
    CHECK(row.size() == 3);
  }
}

TEST_CASE("generateBoard — first row has no monsters") {
  std::mt19937 rng(42);
  LevelConfig cfg = getLevelConfig(1);
  auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);

  for (int col = 0; col < 3; ++col) {
    CHECK(board.slots[0][col].card.cardType != CardType::Monster);
  }
}

TEST_CASE("generateBoard — last row has biome or end card") {
  SUBCASE("Level 1 — biome cards") {
    std::mt19937 rng(42);
    LevelConfig cfg = getLevelConfig(1);
    auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);

    int biomeCount = 0;
    for (int col = 0; col < 3; ++col) {
      if (board.slots[14][col].card.cardType == CardType::Biome) {
        biomeCount++;
      }
    }
    CHECK(biomeCount >= 1);
    CHECK(!board.hasEndCard);
  }

  SUBCASE("Level 6 — end card") {
    std::mt19937 rng(42);
    LevelConfig cfg = getLevelConfig(6);
    auto board = generateBoard(cfg, 6, {}, rng);

    bool hasEnd = false;
    for (int col = 0; col < 3; ++col) {
      if (board.slots[14][col].card.cardType == CardType::EndCard) {
        hasEnd = true;
      }
    }
    CHECK(hasEnd);
    CHECK(board.hasEndCard);
  }
}

TEST_CASE("generateBoard — boss in second-to-last row center") {
  std::mt19937 rng(42);
  LevelConfig cfg = getLevelConfig(1);
  auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);

  CHECK(board.slots[13][1].card.cardType == CardType::Monster);
  CHECK(board.slots[13][1].card.value == 9); // max of level 1 monsterValues
}

TEST_CASE("generateBoard — chest placement in valid rows") {
  std::mt19937 rng(42);
  LevelConfig cfg = getLevelConfig(2);
  auto board = generateBoard(cfg, 2, {"abyss"}, rng);

  for (int row = 0; row < 15; ++row) {
    for (int col = 0; col < 3; ++col) {
      if (board.slots[row][col].card.cardType == CardType::Place) {
        CHECK(row <= 11); // rows 0..11 (totalRows-4 = 11)
      }
    }
  }
}

TEST_CASE("generateBoard — no monster in first row across many seeds") {
  for (int seed = 0; seed < 100; ++seed) {
    std::mt19937 rng(seed);
    LevelConfig cfg = getLevelConfig(1);
    auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);

    for (int col = 0; col < 3; ++col) {
      CHECK(board.slots[0][col].card.cardType != CardType::Monster);
    }
  }
}

TEST_CASE("generateBoard — last row biome/end across many seeds") {
  for (int seed = 0; seed < 100; ++seed) {
    std::mt19937 rng(seed);

    SUBCASE("Level 1") {
      LevelConfig cfg = getLevelConfig(1);
      auto board = generateBoard(cfg, 1, {"forest", "cave"}, rng);
      bool hasSpecial = false;
      for (int col = 0; col < 3; ++col) {
        auto type = board.slots[14][col].card.cardType;
        if (type == CardType::Biome || type == CardType::EndCard) {
          hasSpecial = true;
        }
      }
      CHECK(hasSpecial);
    }

    SUBCASE("Level 6") {
      LevelConfig cfg = getLevelConfig(6);
      auto board = generateBoard(cfg, 6, {}, rng);
      bool hasEnd = false;
      for (int col = 0; col < 3; ++col) {
        if (board.slots[14][col].card.cardType == CardType::EndCard) {
          hasEnd = true;
        }
      }
      CHECK(hasEnd);
    }
  }
}

TEST_CASE("generateBoard — boss always in center of second-to-last row") {
  for (int seed = 0; seed < 100; ++seed) {
    std::mt19937 rng(seed);
    LevelConfig cfg = getLevelConfig(3);
    auto board = generateBoard(cfg, 3, {"underworld"}, rng);

    CHECK(board.slots[13][1].card.cardType == CardType::Monster);
    CHECK(board.slots[13][1].card.value >= 1);
  }
}

TEST_CASE("getChestConfig — regular chest") {
  LevelConfig cfg = getChestConfig("chest", 1);
  CHECK(cfg.rows == 2);
  CHECK(cfg.itemRate == 70);
  CHECK(cfg.shieldRate == 10);
  CHECK(cfg.healRate == 10);
  CHECK(cfg.goldRate == 10);
}

TEST_CASE("getChestConfig — golden chest") {
  LevelConfig cfg = getChestConfig("golden_chest", 1);
  CHECK(cfg.rows == 3);
  CHECK(cfg.itemRate == 55);
}

TEST_CASE("getChestConfig — level 5 wider rows") {
  LevelConfig cfg = getChestConfig("chest", 5);
  CHECK(cfg.cardsPerRow[0] == 20);
  CHECK(cfg.cardsPerRow[1] == 30);
  CHECK(cfg.cardsPerRow[2] == 50);
}
