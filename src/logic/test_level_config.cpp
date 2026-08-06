// src/logic/test_level_config.cpp — Unit tests for level configuration
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/level_config.h"

using namespace game;
using namespace game::logic;

TEST_CASE("getLevelConfig returns correct configs for all 6 levels") {
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
    CHECK(cfg.monsterValues == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
  }

  SUBCASE("Level 2") {
    LevelConfig cfg = getLevelConfig(2);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 107);
    CHECK(cfg.shieldRate == 31);
    CHECK(cfg.healRate == 31);
    CHECK(cfg.goldRate == 31);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 10);
    CHECK(cfg.cardsPerRow[1] == 15);
    CHECK(cfg.cardsPerRow[2] == 75);
    CHECK(cfg.monsterValues == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
  }

  SUBCASE("Level 3") {
    LevelConfig cfg = getLevelConfig(3);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 113);
    CHECK(cfg.shieldRate == 29);
    CHECK(cfg.healRate == 29);
    CHECK(cfg.goldRate == 29);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 15);
    CHECK(cfg.cardsPerRow[1] == 20);
    CHECK(cfg.cardsPerRow[2] == 65);
    CHECK(cfg.monsterValues == std::vector<int>{1, 2, 3, 4, 5, 6, 7, 8, 9});
  }

  SUBCASE("Level 4 — weighted monster values") {
    LevelConfig cfg = getLevelConfig(4);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 113);
    CHECK(cfg.shieldRate == 29);
    CHECK(cfg.healRate == 29);
    CHECK(cfg.goldRate == 29);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 15);
    CHECK(cfg.cardsPerRow[1] == 20);
    CHECK(cfg.cardsPerRow[2] == 65);
    CHECK(cfg.monsterValues == std::vector<int>{9, 9, 9, 10, 10, 11, 11, 11});
  }

  SUBCASE("Level 5") {
    LevelConfig cfg = getLevelConfig(5);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 100);
    CHECK(cfg.shieldRate == 33);
    CHECK(cfg.healRate == 33);
    CHECK(cfg.goldRate == 33);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 15);
    CHECK(cfg.cardsPerRow[1] == 20);
    CHECK(cfg.cardsPerRow[2] == 65);
    CHECK(cfg.monsterValues == std::vector<int>{3, 4, 5, 6, 7, 8, 9, 10, 11, 12});
  }

  SUBCASE("Level 6") {
    LevelConfig cfg = getLevelConfig(6);
    CHECK(cfg.rows == 15);
    CHECK(cfg.monsterRate == 107);
    CHECK(cfg.shieldRate == 31);
    CHECK(cfg.healRate == 31);
    CHECK(cfg.goldRate == 31);
    CHECK(cfg.itemRate == 1);
    CHECK(cfg.cardsPerRow[0] == 25);
    CHECK(cfg.cardsPerRow[1] == 25);
    CHECK(cfg.cardsPerRow[2] == 50);
    CHECK(cfg.monsterValues == std::vector<int>{5, 6, 7, 8, 9, 10, 11, 12, 13});
  }
}

TEST_CASE("getLevelConfig clamps out-of-range levels") {
  SUBCASE("Level 0 clamps to level 1") {
    LevelConfig cfg = getLevelConfig(0);
    CHECK(cfg.monsterRate == 100);
  }
  SUBCASE("Level 7 clamps to level 6") {
    LevelConfig cfg = getLevelConfig(7);
    CHECK(cfg.cardsPerRow[0] == 25);
  }
}

TEST_CASE("getChestConfig — regular chest") {
  LevelConfig cfg = getChestConfig(Cards::Chest, 1);
  CHECK(cfg.rows == 2);
  CHECK(cfg.itemRate == 70);
  CHECK(cfg.shieldRate == 10);
  CHECK(cfg.healRate == 10);
  CHECK(cfg.goldRate == 10);
  CHECK(cfg.monsterRate == 0);
  CHECK(cfg.cardsPerRow[0] == 5);
  CHECK(cfg.cardsPerRow[1] == 10);
  CHECK(cfg.cardsPerRow[2] == 85);
}

TEST_CASE("getChestConfig — golden chest") {
  LevelConfig cfg = getChestConfig(Cards::GoldenChest, 1);
  CHECK(cfg.rows == 3);
  CHECK(cfg.itemRate == 55);
  CHECK(cfg.shieldRate == 15);
  CHECK(cfg.healRate == 15);
  CHECK(cfg.goldRate == 15);
  CHECK(cfg.monsterRate == 0);
  CHECK(cfg.cardsPerRow[0] == 5);
  CHECK(cfg.cardsPerRow[1] == 10);
  CHECK(cfg.cardsPerRow[2] == 85);
}

TEST_CASE("getChestConfig — level 5 wider rows") {
  SUBCASE("Regular chest at level 5") {
    LevelConfig cfg = getChestConfig(Cards::Chest, 5);
    CHECK(cfg.cardsPerRow[0] == 20);
    CHECK(cfg.cardsPerRow[1] == 30);
    CHECK(cfg.cardsPerRow[2] == 50);
  }
  SUBCASE("Golden chest at level 5") {
    LevelConfig cfg = getChestConfig(Cards::GoldenChest, 5);
    CHECK(cfg.cardsPerRow[0] == 20);
    CHECK(cfg.cardsPerRow[1] == 30);
    CHECK(cfg.cardsPerRow[2] == 50);
  }
}

TEST_CASE("getAvailableBiomes") {
  SUBCASE("Level 1 — no visited") {
    auto biomes = getAvailableBiomes(1, {});
    CHECK(biomes.size() == 4);
    CHECK(biomes[0] == Cards::Forest);
    CHECK(biomes[1] == Cards::Cave);
    CHECK(biomes[2] == Cards::EnchantedLands);
    CHECK(biomes[3] == Cards::Desert);
  }

  SUBCASE("Level 1 — one visited") {
    auto biomes = getAvailableBiomes(1, {"forest"});
    CHECK(biomes.size() == 3);
  }

  SUBCASE("Level 1 — all visited") {
    auto biomes = getAvailableBiomes(1, {"forest", "cave", "enchanted_lands", "desert"});
    CHECK(biomes.empty());
  }

  SUBCASE("Level 2 — no visited") {
    auto biomes = getAvailableBiomes(2, {});
    CHECK(biomes.size() == 1);
    CHECK(biomes[0] == Cards::Abyss);
  }

  SUBCASE("Level 2 — all visited") {
    auto biomes = getAvailableBiomes(2, {"abyss"});
    CHECK(biomes.empty());
  }

  SUBCASE("Level 3 — no visited") {
    auto biomes = getAvailableBiomes(3, {});
    CHECK(biomes.size() == 1);
    CHECK(biomes[0] == Cards::Underworld);
  }

  SUBCASE("Level 3 — all visited") {
    auto biomes = getAvailableBiomes(3, {"underworld"});
    CHECK(biomes.empty());
  }

  SUBCASE("Out-of-range level returns empty") {
    auto biomes = getAvailableBiomes(4, {});
    CHECK(biomes.empty());
  }
}
