// src/logic/test_board_gen.cpp — Unit tests for board generation
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/board_gen.h"

#include <map>
#include <set>

using namespace game;
using namespace game::logic;

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
