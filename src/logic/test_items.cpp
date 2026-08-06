// src/logic/test_items.cpp — Unit tests for item system
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/items.h"

using namespace game;
using namespace game::logic;

static Card makeItem(std::string_view slug) {
  Card card;
  card.slug = std::string(slug);
  card.cardType = CardType::Item;
  card.value = 0;
  card.name = std::string(slug);
  card.description = "";
  return card;
}

TEST_CASE("getMonsterCardModifier") {
  SUBCASE("No items returns 0") {
    std::vector<Card> items;
    CHECK(getMonsterCardModifier(items) == 0);
  }

  SUBCASE("One monster skull returns 1") {
    std::vector<Card> items = {makeItem(Cards::MonsterSkull)};
    CHECK(getMonsterCardModifier(items) == 1);
  }

  SUBCASE("Three monster skulls returns 3") {
    std::vector<Card> items = {
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
    };
    CHECK(getMonsterCardModifier(items) == 3);
  }

  SUBCASE("Other items ignored") {
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::Omelette),
    };
    CHECK(getMonsterCardModifier(items) == 1);
  }
}

TEST_CASE("getHealCardModifier") {
  SUBCASE("No items returns 0") {
    std::vector<Card> items;
    CHECK(getHealCardModifier(items) == 0);
  }

  SUBCASE("Raw meat adds 1") {
    std::vector<Card> items = {makeItem(Cards::RawMeat)};
    CHECK(getHealCardModifier(items) == 1);
  }

  SUBCASE("Rotten meat subtracts 1") {
    std::vector<Card> items = {makeItem(Cards::RottenMeat)};
    CHECK(getHealCardModifier(items) == -1);
  }

  SUBCASE("Mix cancels out") {
    std::vector<Card> items = {
      makeItem(Cards::RawMeat),
      makeItem(Cards::RawMeat),
      makeItem(Cards::RottenMeat),
    };
    CHECK(getHealCardModifier(items) == 1);
  }
}

TEST_CASE("getShieldCardModifier") {
  SUBCASE("No items returns 0") {
    std::vector<Card> items;
    CHECK(getShieldCardModifier(items) == 0);
  }

  SUBCASE("Shiny shield adds 1") {
    std::vector<Card> items = {makeItem(Cards::ShinyShield)};
    CHECK(getShieldCardModifier(items) == 1);
  }

  SUBCASE("Rusty shield subtracts 1") {
    std::vector<Card> items = {makeItem(Cards::RustyShield)};
    CHECK(getShieldCardModifier(items) == -1);
  }

  SUBCASE("Mix cancels out") {
    std::vector<Card> items = {
      makeItem(Cards::ShinyShield),
      makeItem(Cards::RustyShield),
      makeItem(Cards::RustyShield),
    };
    CHECK(getShieldCardModifier(items) == -1);
  }
}

TEST_CASE("getHeroLifeMaxModifier") {
  SUBCASE("No items returns 0") {
    std::vector<Card> items;
    CHECK(getHeroLifeMaxModifier(items, 100) == 0);
  }

  SUBCASE("Omelette adds 3") {
    std::vector<Card> items = {makeItem(Cards::Omelette)};
    CHECK(getHeroLifeMaxModifier(items, 100) == 3);
  }

  SUBCASE("Ring of vitality adds 5") {
    std::vector<Card> items = {makeItem(Cards::RingOfVitality)};
    CHECK(getHeroLifeMaxModifier(items, 100) == 5);
  }

  SUBCASE("Multiple omelettes stack") {
    std::vector<Card> items = {
      makeItem(Cards::Omelette),
      makeItem(Cards::Omelette),
      makeItem(Cards::Omelette),
    };
    CHECK(getHeroLifeMaxModifier(items, 100) == 9);
  }

  SUBCASE("Cursed shield returns -(heroLifeMax - 1)") {
    std::vector<Card> items = {
      makeItem(Cards::Omelette),
      makeItem(Cards::RingOfVitality),
      makeItem(Cards::CursedShield),
    };
    CHECK(getHeroLifeMaxModifier(items, 100) == -99);
  }

  SUBCASE("Cursed shield with base 20 returns -19") {
    std::vector<Card> items = {makeItem(Cards::CursedShield)};
    CHECK(getHeroLifeMaxModifier(items, 20) == -19);
  }
}

TEST_CASE("getNbRowsHidden") {
  SUBCASE("No items returns 0") {
    std::vector<Card> items;
    CHECK(getNbRowsHidden(items) == 0);
  }

  SUBCASE("One monster-eye returns 1") {
    std::vector<Card> items = {makeItem(Cards::MonsterEye)};
    CHECK(getNbRowsHidden(items) == 1);
  }

  SUBCASE("Capped at 2") {
    std::vector<Card> items = {
      makeItem(Cards::MonsterEye),
      makeItem(Cards::MonsterEye),
      makeItem(Cards::MonsterEye),
    };
    CHECK(getNbRowsHidden(items) == 2);
  }
}

TEST_CASE("checkMonsterEggCombo") {
  SUBCASE("No eggs — no combo") {
    std::vector<Card> items = {makeItem(Cards::Apple), makeItem(Cards::Apple)};
    ComboResult result = checkMonsterEggCombo(items);
    CHECK(result.triggered == false);
    CHECK(result.newItems.size() == 2);
  }

  SUBCASE("Two eggs — no combo") {
    std::vector<Card> items = {
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
    };
    ComboResult result = checkMonsterEggCombo(items);
    CHECK(result.triggered == false);
  }

  SUBCASE("Three eggs — triggers combo") {
    std::vector<Card> items = {
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
    };
    ComboResult result = checkMonsterEggCombo(items);
    CHECK(result.triggered == true);
    CHECK(result.newItems.size() == 1);
    CHECK(result.newItems[0].slug == Cards::Omelette);
  }

  SUBCASE("Four eggs — removes 3, keeps 1") {
    std::vector<Card> items = {
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
    };
    ComboResult result = checkMonsterEggCombo(items);
    CHECK(result.triggered == true);
    CHECK(result.newItems.size() == 2);
    int eggCount = 0;
    for (const auto& item : result.newItems) {
      if (item.slug == Cards::MonsterEgg) {
        eggCount++;
      }
    }
    CHECK(eggCount == 1);
    CHECK(result.newItems.back().slug == Cards::Omelette);
  }

  SUBCASE("Eggs with other items preserved") {
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::MonsterEgg),
      makeItem(Cards::RingOfProtection),
    };
    ComboResult result = checkMonsterEggCombo(items);
    CHECK(result.triggered == true);
    CHECK(result.newItems.size() == 3);
    CHECK(result.newItems[0].slug == Cards::Apple);
    CHECK(result.newItems[1].slug == Cards::RingOfProtection);
    CHECK(result.newItems[2].slug == Cards::Omelette);
  }
}

TEST_CASE("applyAppleEffect") {
  SUBCASE("No apples — no change") {
    HealthData health = {50, 100};
    std::vector<Card> items;
    applyAppleEffect(health, items);
    CHECK(health.current == 50);
  }

  SUBCASE("One apple — +10 HP") {
    HealthData health = {50, 100};
    std::vector<Card> items = {makeItem(Cards::Apple)};
    applyAppleEffect(health, items);
    CHECK(health.current == 60);
  }

  SUBCASE("Capped at max") {
    HealthData health = {95, 100};
    std::vector<Card> items = {makeItem(Cards::Apple)};
    applyAppleEffect(health, items);
    CHECK(health.current == 100);
  }

  SUBCASE("Multiple apples stack") {
    HealthData health = {30, 100};
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::Apple),
      makeItem(Cards::Apple),
    };
    applyAppleEffect(health, items);
    CHECK(health.current == 60);
  }

  SUBCASE("Cursed shield caps at effective max 1") {
    HealthData health = {0, 20};
    std::vector<Card> items = {
      makeItem(Cards::CursedShield),
      makeItem(Cards::Apple),
    };
    applyAppleEffect(health, items);
    CHECK(health.current == 1);
  }
}

TEST_CASE("applyRingOfProtection") {
  SUBCASE("No rings — no change") {
    ShieldData shield = {0};
    std::vector<Card> items;
    applyRingOfProtection(shield, items);
    CHECK(shield.value == 0);
  }

  SUBCASE("One ring — +5 shield") {
    ShieldData shield = {0};
    std::vector<Card> items = {makeItem(Cards::RingOfProtection)};
    applyRingOfProtection(shield, items);
    CHECK(shield.value == 5);
  }

  SUBCASE("Multiple rings stack") {
    ShieldData shield = {0};
    std::vector<Card> items = {
      makeItem(Cards::RingOfProtection),
      makeItem(Cards::RingOfProtection),
    };
    applyRingOfProtection(shield, items);
    CHECK(shield.value == 10);
  }
}

TEST_CASE("applyDragonWing") {
  SUBCASE("Adds +1 shield") {
    ShieldData shield = {0};
    applyDragonWing(shield);
    CHECK(shield.value == 1);
  }

  SUBCASE("Adds to existing shield") {
    ShieldData shield = {5};
    applyDragonWing(shield);
    CHECK(shield.value == 6);
  }
}

TEST_CASE("applyAthelas") {
  SUBCASE("No athelas in bag — no effect") {
    PlayerState player;
    player.health = {0, 100};
    player.athelasUsed = false;
    std::vector<Card> items = {makeItem(Cards::Apple)};
    applyAthelas(player, items);
    CHECK(player.health.current == 0);
    CHECK(items.size() == 1);
  }

  SUBCASE("Athelas prevents death — sets life to 1") {
    PlayerState player;
    player.health = {0, 100};
    player.athelasUsed = false;
    std::vector<Card> items = {makeItem(Cards::Athelas)};
    applyAthelas(player, items);
    CHECK(player.health.current == 1);
    CHECK(player.athelasUsed == true);
    CHECK(items.empty());
  }

  SUBCASE("Athelas already used — no effect") {
    PlayerState player;
    player.health = {0, 100};
    player.athelasUsed = true;
    std::vector<Card> items = {makeItem(Cards::Athelas)};
    applyAthelas(player, items);
    CHECK(player.health.current == 0);
    CHECK(items.size() == 1);
  }

  SUBCASE("Athelas with other items — only removes athelas") {
    PlayerState player;
    player.health = {0, 100};
    player.athelasUsed = false;
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::Athelas),
      makeItem(Cards::RingOfProtection),
    };
    applyAthelas(player, items);
    CHECK(player.health.current == 1);
    CHECK(items.size() == 2);
    CHECK(items[0].slug == Cards::Apple);
    CHECK(items[1].slug == Cards::RingOfProtection);
  }
}

TEST_CASE("applyCursedShield") {
  SUBCASE("No cursed shield — returns false") {
    PlayerState player;
    player.health = {50, 100};
    std::vector<Card> items = {makeItem(Cards::Apple)};
    bool applied = applyCursedShield(player, items);
    CHECK(applied == false);
    CHECK(player.health.max == 100);
  }

  SUBCASE("Cursed shield — caps current HP to 1") {
    PlayerState player;
    player.health = {50, 100};
    std::vector<Card> items = {makeItem(Cards::CursedShield)};
    bool applied = applyCursedShield(player, items);
    CHECK(applied == true);
    CHECK(player.health.max == 100);
    CHECK(player.health.current == 1);
  }

  SUBCASE("Cursed shield — current HP already 1 stays 1") {
    PlayerState player;
    player.health = {1, 100};
    std::vector<Card> items = {makeItem(Cards::CursedShield)};
    bool applied = applyCursedShield(player, items);
    CHECK(applied == true);
    CHECK(player.health.max == 100);
    CHECK(player.health.current == 1);
  }
}

TEST_CASE("applyAmethyst") {
  SUBCASE("No amethyst — no change") {
    GoldData gold = {100};
    std::vector<Card> items = {makeItem(Cards::Apple)};
    applyAmethyst(gold, items);
    CHECK(gold.amount == 100);
    CHECK(items.size() == 1);
  }

  SUBCASE("Amethyst — doubles gold and removes self") {
    GoldData gold = {50};
    std::vector<Card> items = {makeItem(Cards::Amethyst)};
    applyAmethyst(gold, items);
    CHECK(gold.amount == 100);
    CHECK(items.empty());
  }

  SUBCASE("Amethyst with other items — only removes amethyst") {
    GoldData gold = {25};
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::Amethyst),
      makeItem(Cards::RingOfProtection),
    };
    applyAmethyst(gold, items);
    CHECK(gold.amount == 50);
    CHECK(items.size() == 2);
    CHECK(items[0].slug == Cards::Apple);
    CHECK(items[1].slug == Cards::RingOfProtection);
  }
}
