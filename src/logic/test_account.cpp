// src/logic/test_account.cpp — Unit tests for account system
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/account.h"

using namespace game;
using namespace game::logic;

TEST_CASE("createDefaultAccount") {
  Account account = createDefaultAccount();

  SUBCASE("Gold starts at 0") {
    CHECK(account.gold == 0);
  }

  SUBCASE("Language defaults to English") {
    CHECK(account.language == "en");
  }

  SUBCASE("Has 4 heroes") {
    CHECK(account.heroes.size() == 4);
  }

  SUBCASE("Headless Knight is default hero") {
    CHECK(account.heroes[0].slug == Heroes::HeadlessKnight);
    CHECK(account.heroes[0].locked == false);
    CHECK(account.heroes[0].cost == 0);
    CHECK(account.heroes[0].lifeMax == 15);
    CHECK(account.heroes[0].shield == 0);
    CHECK(account.heroes[0].bag.size() == 2);
  }

  SUBCASE("Thief starts locked") {
    CHECK(account.heroes[1].slug == Heroes::Thief);
    CHECK(account.heroes[1].locked == true);
    CHECK(account.heroes[1].cost == 50);
    CHECK(account.heroes[1].lifeMax == 12);
  }

  SUBCASE("Harpy starts locked") {
    CHECK(account.heroes[2].slug == Heroes::Harpy);
    CHECK(account.heroes[2].locked == true);
    CHECK(account.heroes[2].cost == 100);
  }

  SUBCASE("Warrior starts locked with shield") {
    CHECK(account.heroes[3].slug == Heroes::Warrior);
    CHECK(account.heroes[3].locked == true);
    CHECK(account.heroes[3].cost == 200);
    CHECK(account.heroes[3].shield == 10);
  }
}

TEST_CASE("unlockHero") {
  Account account = createDefaultAccount();
  account.gold = 100;

  SUBCASE("Unlock hero with enough gold") {
    bool result = unlockHero(account.heroes[1], account);
    CHECK(result == true);
    CHECK(account.heroes[1].locked == false);
    CHECK(account.gold == 50);
  }

  SUBCASE("Unlock hero with exactly enough gold") {
    account.gold = 50;
    bool result = unlockHero(account.heroes[1], account);
    CHECK(result == true);
    CHECK(account.gold == 0);
  }

  SUBCASE("Fail with insufficient gold") {
    account.gold = 30;
    bool result = unlockHero(account.heroes[1], account);
    CHECK(result == false);
    CHECK(account.heroes[1].locked == true);
    CHECK(account.gold == 30);
  }

  SUBCASE("Fail when hero already unlocked") {
    bool result = unlockHero(account.heroes[0], account);
    CHECK(result == false);
    CHECK(account.gold == 100);
  }

  SUBCASE("Unlock multiple heroes sequentially") {
    account.gold = 350;
    unlockHero(account.heroes[1], account);
    CHECK(account.gold == 300);
    unlockHero(account.heroes[2], account);
    CHECK(account.gold == 200);
    unlockHero(account.heroes[3], account);
    CHECK(account.gold == 0);
  }
}

TEST_CASE("spendGold") {
  Account account = createDefaultAccount();
  account.gold = 100;

  SUBCASE("Spend valid amount") {
    bool result = spendGold(account, 30);
    CHECK(result == true);
    CHECK(account.gold == 70);
  }

  SUBCASE("Spend exact amount") {
    bool result = spendGold(account, 100);
    CHECK(result == true);
    CHECK(account.gold == 0);
  }

  SUBCASE("Fail with insufficient gold") {
    bool result = spendGold(account, 200);
    CHECK(result == false);
    CHECK(account.gold == 100);
  }

  SUBCASE("Fail with zero amount") {
    bool result = spendGold(account, 0);
    CHECK(result == false);
    CHECK(account.gold == 100);
  }

  SUBCASE("Fail with negative amount") {
    bool result = spendGold(account, -10);
    CHECK(result == false);
    CHECK(account.gold == 100);
  }
}

TEST_CASE("addGold") {
  Account account = createDefaultAccount();

  SUBCASE("Add positive amount") {
    addGold(account, 50);
    CHECK(account.gold == 50);
  }

  SUBCASE("Add zero does nothing") {
    addGold(account, 0);
    CHECK(account.gold == 0);
  }

  SUBCASE("Add negative does nothing") {
    addGold(account, -10);
    CHECK(account.gold == 0);
  }

  SUBCASE("Gold accumulates") {
    addGold(account, 30);
    addGold(account, 20);
    CHECK(account.gold == 50);
  }
}

TEST_CASE("save/load round-trip") {
  SUBCASE("Default account round-trips") {
    Account original = createDefaultAccount();
    original.gold = 250;
    std::string json = saveAccount(original);
    Account loaded = loadAccount(json);

    CHECK(loaded.gold == 250);
    CHECK(loaded.language == "en");
    CHECK(loaded.heroes.size() == 4);
    CHECK(loaded.heroes[0].slug == original.heroes[0].slug);
    CHECK(loaded.heroes[0].locked == original.heroes[0].locked);
    CHECK(loaded.heroes[0].lifeMax == original.heroes[0].lifeMax);
    CHECK(loaded.heroes[0].shield == original.heroes[0].shield);
    CHECK(loaded.heroes[0].bag == original.heroes[0].bag);
    CHECK(loaded.heroes[1].slug == original.heroes[1].slug);
    CHECK(loaded.heroes[1].locked == original.heroes[1].locked);
    CHECK(loaded.heroes[1].cost == original.heroes[1].cost);
  }

  SUBCASE("Modified account round-trips") {
    Account account = createDefaultAccount();
    account.gold = 500;
    account.language = "fr";
    unlockHero(account.heroes[1], account);
    account.heroes[0].life = 10;
    account.heroes[0].gold = 25;

    std::string json = saveAccount(account);
    Account loaded = loadAccount(json);

    CHECK(loaded.gold == 450);
    CHECK(loaded.language == "fr");
    CHECK(loaded.heroes[0].life == 10);
    CHECK(loaded.heroes[0].gold == 25);
    CHECK(loaded.heroes[1].locked == false);
  }

  SUBCASE("Invalid JSON returns default account") {
    Account loaded = loadAccount("not valid json {{{");
    CHECK(loaded.gold == 0);
    CHECK(loaded.heroes.size() == 4);
    CHECK(loaded.language == "en");
  }

  SUBCASE("Empty JSON returns default account") {
    Account loaded = loadAccount("");
    CHECK(loaded.gold == 0);
    CHECK(loaded.heroes.size() == 4);
  }
}
