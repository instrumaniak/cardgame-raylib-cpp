// src/logic/test_turn_flow.cpp — Unit tests for turn flow orchestrator
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/turn_flow.h"

#include <random>

using namespace game;
using namespace game::logic;

static Card makeCard(CardType type, int value, std::string_view slug) {
  Card card;
  card.slug = std::string(slug);
  card.cardType = type;
  card.value = value;
  card.name = std::string(slug);
  card.description = "";
  return card;
}

static PlayerState makePlayer(int hp, int maxHp, int shield = 0, int gold = 0) {
  PlayerState player;
  player.health = {hp, maxHp};
  player.shield = {shield};
  player.gold = {gold};
  player.athelasUsed = false;
  return player;
}

static GameState makeGame(int rows = 3, int hp = 20, int maxHp = 20) {
  GameState gs;
  gs.player = makePlayer(hp, maxHp);
  gs.board.resize(rows, std::vector<CardSlot>(3));
  gs.status = GameStatus::Ready;
  gs.currentLevel = 1;
  gs.currentRow = 0;
  gs.currentCol = 1;
  gs.goldEarned = 0;
  gs.inChest = false;
  gs.chestRowsRemaining = 0;
  return gs;
}

static void placeCard(GameState& gs, int row, int col, const Card& card) {
  gs.board[row][col] = CardSlot{card, false};
}

TEST_CASE("playCard — validation") {
  std::mt19937 rng(42);

  SUBCASE("Wrong row returns false") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 1, card);
    CHECK_FALSE(playCard(gs, 1, 1, rng));
  }

  SUBCASE("Non-adjacent column returns false") {
    GameState gs = makeGame();
    gs.currentCol = 0;
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 2, card);
    CHECK_FALSE(playCard(gs, 0, 2, rng)); // distance 2 from col 0
  }

  SUBCASE("Same column is valid (col == currentCol)") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
  }

  SUBCASE("Adjacent column is valid (col == currentCol + 1)") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 2, card);
    CHECK(playCard(gs, 0, 2, rng));
  }

  SUBCASE("Consumed slot returns false") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 1, card);
    gs.board[0][1].consumed = true;
    CHECK_FALSE(playCard(gs, 0, 1, rng));
  }
}

TEST_CASE("playCard — shield card") {
  std::mt19937 rng(42);

  SUBCASE("Shield card adds shield") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Shield, 5, Cards::WoodenShield);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.shield.value == 5);
    CHECK(gs.status == GameStatus::Ready);
    CHECK(gs.board[0][1].consumed);
  }
}

TEST_CASE("playCard — heal card") {
  std::mt19937 rng(42);

  SUBCASE("Heal card restores HP") {
    GameState gs = makeGame(3, 10, 20);
    gs.player.health.current = 5;
    Card card = makeCard(CardType::Heal, 3, Cards::MinorHealingPotion);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 8);
  }

  SUBCASE("Heal caps at max HP") {
    GameState gs = makeGame(3, 18, 20);
    Card card = makeCard(CardType::Heal, 5, Cards::MinorHealingPotion);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 20);
  }
}

TEST_CASE("playCard — gold card") {
  std::mt19937 rng(42);

  SUBCASE("Gold card adds gold") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Gold, 7, Cards::OneCoin);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.gold.amount == 7);
  }
}

TEST_CASE("playCard — monster card") {
  std::mt19937 rng(42);

  SUBCASE("Monster card deals damage") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Monster, 3, Cards::Forest);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 17);
  }
}

TEST_CASE("playCard — item card") {
  std::mt19937 rng(42);

  SUBCASE("Item added to bag") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Item, 0, Cards::Apple);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.inventory.items.size() == 1);
    CHECK(gs.player.inventory.items[0].slug == Cards::Apple);
  }
}

TEST_CASE("playCard — drop card") {
  std::mt19937 rng(42);

  SUBCASE("Drop added to bag") {
    GameState gs = makeGame();
    Card card = makeCard(CardType::Drop, 0, Cards::MonsterEgg);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.inventory.items.size() == 1);
  }
}

TEST_CASE("playCard — carrot effect") {
  std::mt19937 rng(42);

  SUBCASE("Carrot gives +1 HP when moving to different position") {
    GameState gs = makeGame();
    gs.player.health.current = 10;
    gs.player.inventory.items.push_back(makeCard(CardType::Item, 0, Cards::Carrot));
    Card card = makeCard(CardType::Shield, 0, Cards::WoodenShield);
    placeCard(gs, 0, 2, card); // move from col 1 to col 2
    CHECK(playCard(gs, 0, 2, rng));
    CHECK(gs.player.health.current == 11); // 10 + 1 carrot
  }

  SUBCASE("Carrot does NOT trigger when staying at same position") {
    GameState gs = makeGame();
    gs.player.health.current = 10;
    gs.player.inventory.items.push_back(makeCard(CardType::Item, 0, Cards::Carrot));
    Card card = makeCard(CardType::Shield, 0, Cards::WoodenShield);
    placeCard(gs, 0, 1, card); // stay at col 1
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 10); // no carrot bonus
  }
}

TEST_CASE("playCard — HP cap and clamp") {
  std::mt19937 rng(42);

  SUBCASE("HP capped at max after heal") {
    GameState gs = makeGame(3, 19, 20);
    Card card = makeCard(CardType::Heal, 5, Cards::MinorHealingPotion);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 20);
  }

  SUBCASE("HP clamped to 0 minimum") {
    GameState gs = makeGame(3, 1, 20);
    Card card = makeCard(CardType::Monster, 5, Cards::Forest);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.health.current == 0);
  }

  SUBCASE("Shield clamped to 0 minimum") {
    GameState gs = makeGame();
    gs.player.shield.value = 0;
    Card card = makeCard(CardType::Monster, 5, Cards::Forest);
    placeCard(gs, 0, 1, card);
    CHECK(playCard(gs, 0, 1, rng));
    CHECK(gs.player.shield.value >= 0);
  }
}

TEST_CASE("playCard — biome card transitions to NotReady") {
  std::mt19937 rng(42);

  GameState gs = makeGame();
  Card card = makeCard(CardType::Biome, 0, Cards::Forest);
  placeCard(gs, 0, 1, card);
  int prevLevel = gs.currentLevel;
  CHECK(playCard(gs, 0, 1, rng));
  CHECK(gs.status == GameStatus::NotReady);
  CHECK(gs.currentLevel == prevLevel + 1);
}

TEST_CASE("playCard — place card enters chest") {
  std::mt19937 rng(42);

  GameState gs = makeGame();
  Card card = makeCard(CardType::Place, 0, Cards::Chest);
  placeCard(gs, 0, 1, card);
  CHECK(playCard(gs, 0, 1, rng));
  CHECK(gs.status == GameStatus::NotReady);
  CHECK(gs.inChest);
  CHECK(gs.chestType == Cards::Chest);
}

TEST_CASE("playCard — end card triggers win") {
  std::mt19937 rng(42);

  GameState gs = makeGame();
  gs.player.gold.amount = 100;
  Card card = makeCard(CardType::EndCard, 0, Cards::End);
  placeCard(gs, 0, 1, card);
  CHECK(playCard(gs, 0, 1, rng));
  CHECK(gs.status == GameStatus::Win);
  CHECK(gs.goldEarned == 100);
}

TEST_CASE("checkMonsterPlay — no monster below") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  // Row 1 is empty (default)
  gs.currentRow = 0;
  gs.currentCol = 1;
  gs.status = GameStatus::HeroDrop;
  checkMonsterPlay(gs, rng);
  CHECK(gs.status == GameStatus::Ready);
  CHECK(gs.currentRow == 1);
}

TEST_CASE("checkMonsterPlay — monster in row below") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  gs.player.health.current = 20;
  // Place monster in row 1, col 1 (below hero at row 0, col 1)
  Card monster = makeCard(CardType::Monster, 3, Cards::Forest);
  placeCard(gs, 1, 1, monster);
  gs.currentRow = 0;
  gs.currentCol = 1;
  gs.status = GameStatus::HeroDrop;

  checkMonsterPlay(gs, rng);

  CHECK(gs.status == GameStatus::Ready);
  CHECK(gs.currentRow == 1);
  CHECK(gs.player.health.current == 17); // 20 - 3
  // Monster slot should have been replaced with loot (card changed)
  CHECK(gs.board[1][1].card.slug != Cards::Forest);
}

TEST_CASE("checkMonsterPlay — invisibility cloak dodges") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  gs.player.health.current = 10; // even HP
  gs.player.inventory.items.push_back(makeCard(CardType::Item, 0, Cards::InvisibilityCloak));
  Card monster = makeCard(CardType::Monster, 5, Cards::Forest);
  placeCard(gs, 1, 1, monster);
  gs.currentRow = 0;
  gs.currentCol = 1;
  gs.status = GameStatus::HeroDrop;

  checkMonsterPlay(gs, rng);

  CHECK(gs.player.health.current == 10); // no damage taken
  CHECK(gs.status == GameStatus::Ready);
}

TEST_CASE("checkMonsterPlay — invisibility cloak does NOT dodge with odd HP") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  gs.player.health.current = 11; // odd HP
  gs.player.inventory.items.push_back(makeCard(CardType::Item, 0, Cards::InvisibilityCloak));
  Card monster = makeCard(CardType::Monster, 5, Cards::Forest);
  placeCard(gs, 1, 1, monster);
  gs.currentRow = 0;
  gs.currentCol = 1;
  gs.status = GameStatus::HeroDrop;

  checkMonsterPlay(gs, rng);

  CHECK(gs.player.health.current == 6); // 11 - 5 damage taken
}

TEST_CASE("nextTurn — death without athelas") {
  GameState gs = makeGame();
  gs.player.health.current = 0;
  nextTurn(gs);
  CHECK(gs.status == GameStatus::Lose);
}

TEST_CASE("nextTurn — athelas saves from death") {
  GameState gs = makeGame();
  gs.player.health.current = 0;
  gs.player.inventory.items.push_back(makeCard(CardType::Item, 0, Cards::Athelas));
  nextTurn(gs);
  CHECK(gs.player.health.current == 1);
  CHECK(gs.status == GameStatus::Ready);
  CHECK(gs.currentRow == 1);
  // Athelas should be removed from bag
  bool found = false;
  for (const auto& item : gs.player.inventory.items) {
    if (item.slug == Cards::Athelas) {
      found = true;
    }
  }
  CHECK_FALSE(found);
}

TEST_CASE("nextTurn — normal advance") {
  GameState gs = makeGame();
  gs.player.health.current = 10;
  gs.currentRow = 0;
  nextTurn(gs);
  CHECK(gs.status == GameStatus::Ready);
  CHECK(gs.currentRow == 1);
}

TEST_CASE("die — sets Lose status and returns gold reward") {
  GameState gs = makeGame();
  gs.player.gold.amount = 50;
  int reward = die(gs);
  CHECK(gs.status == GameStatus::Lose);
  CHECK(reward == 25);
  CHECK(gs.goldEarned == 25);
}

TEST_CASE("Full turn sequence — shield card then ready") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  Card shield = makeCard(CardType::Shield, 4, Cards::WoodenShield);
  placeCard(gs, 0, 1, shield);

  bool ok = playCard(gs, 0, 1, rng);
  CHECK(ok);
  CHECK(gs.status == GameStatus::Ready); // no monster below → ready
  CHECK(gs.currentRow == 1);
  CHECK(gs.player.shield.value == 4);
}

TEST_CASE("Full turn sequence — monster kill then loot") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  gs.player.health.current = 20;
  Card shield = makeCard(CardType::Shield, 5, Cards::WoodenShield);
  placeCard(gs, 0, 1, shield);
  Card monster = makeCard(CardType::Monster, 3, Cards::Forest);
  placeCard(gs, 1, 1, monster);

  playCard(gs, 0, 1, rng);
  CHECK(gs.status == GameStatus::Ready);
  CHECK(gs.currentRow == 1);
  // Shield absorbs the 3 damage → health stays 20, shield drops to 2
  CHECK(gs.player.health.current == 20);
  CHECK(gs.player.shield.value == 2);
}

TEST_CASE("Full turn sequence — loot is valid type") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  gs.player.health.current = 20;
  Card monster = makeCard(CardType::Monster, 3, Cards::Forest);
  placeCard(gs, 1, 1, monster);
  gs.currentRow = 0;
  gs.currentCol = 1;

  checkMonsterPlay(gs, rng);

  CardType lootType = gs.board[1][1].card.cardType;
  CHECK(lootType != CardType::Item);
  CHECK(lootType != CardType::Place);
  CHECK(lootType != CardType::EndCard);
}

TEST_CASE("Full turn sequence — egg combo triggers through playCard") {
  std::mt19937 rng(42);

  GameState gs = makeGame(3);
  // Preload 2 eggs into bag
  gs.player.inventory.items.push_back(makeCard(CardType::Drop, 0, Cards::MonsterEgg));
  gs.player.inventory.items.push_back(makeCard(CardType::Drop, 0, Cards::MonsterEgg));

  // Drop a 3rd egg
  Card egg = makeCard(CardType::Drop, 0, Cards::MonsterEgg);
  placeCard(gs, 0, 1, egg);
  CHECK(playCard(gs, 0, 1, rng));

  // Combo should have fired: 3 eggs → 1 omelette
  CHECK(gs.player.inventory.items.size() == 1);
  CHECK(gs.player.inventory.items[0].slug == Cards::Omelette);
}
