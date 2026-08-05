// src/logic/test_combat.cpp — Unit tests for combat system
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/combat.h"

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

static PlayerState makePlayer(int hp, int maxHp, int shield) {
  PlayerState player;
  player.health = {hp, maxHp};
  player.shield = {shield};
  player.gold = {0};
  player.athelasUsed = false;
  return player;
}

TEST_CASE("applyDamage — basic cases") {
  SUBCASE("No shield, damage equals base") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 5);
    CHECK(result.shieldAbsorbed == 0);
    CHECK(result.playerShieldRemaining == 0);
    CHECK(result.dodged == false);
    CHECK(result.goldGained == 0);
  }

  SUBCASE("Shield absorbs all damage") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items;
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 0);
    CHECK(result.shieldAbsorbed == 5);
    CHECK(result.playerShieldRemaining == 5);
  }

  SUBCASE("Shield absorbs partial damage with overflow") {
    PlayerState player = makePlayer(20, 20, 3);
    std::vector<Card> items;
    CombatResult result = applyDamage(10, player, items);
    CHECK(result.damageToPlayer == 7);
    CHECK(result.shieldAbsorbed == 3);
    CHECK(result.playerShieldRemaining == 0);
  }

  SUBCASE("Shield equals damage — no overflow") {
    PlayerState player = makePlayer(20, 20, 5);
    std::vector<Card> items;
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 0);
    CHECK(result.shieldAbsorbed == 5);
    CHECK(result.playerShieldRemaining == 0);
  }
}

TEST_CASE("applyDamage — monster-skull modifier") {
  SUBCASE("One skull adds 1 to damage") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::MonsterSkull)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 6);
  }

  SUBCASE("Three skulls add 3 to damage") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
    };
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 8);
  }

  SUBCASE("Skulls with shield — damage includes modifier") {
    PlayerState player = makePlayer(20, 20, 5);
    std::vector<Card> items = {makeItem(Cards::MonsterSkull)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 1);
    CHECK(result.shieldAbsorbed == 5);
    CHECK(result.playerShieldRemaining == 0);
  }
}

TEST_CASE("applyDamage — dragon-wing") {
  SUBCASE("Dragon-wing replaces shield with count") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items = {makeItem(Cards::DragonWing)};
    CombatResult result = applyDamage(3, player, items);
    CHECK(result.damageToPlayer == 2);
    CHECK(result.shieldAbsorbed == 1);
    CHECK(result.playerShieldRemaining == 0);
  }

  SUBCASE("Two dragon-wings set shield to 2") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::DragonWing),
      makeItem(Cards::DragonWing),
    };
    CombatResult result = applyDamage(1, player, items);
    CHECK(result.damageToPlayer == 0);
    CHECK(result.shieldAbsorbed == 1);
    CHECK(result.playerShieldRemaining == 1);
  }

  SUBCASE("Dragon-wing with no existing shield") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::DragonWing)};
    CombatResult result = applyDamage(1, player, items);
    CHECK(result.damageToPlayer == 0);
    CHECK(result.shieldAbsorbed == 1);
    CHECK(result.playerShieldRemaining == 0);
  }
}

TEST_CASE("applyDamage — emerald gold bonus") {
  SUBCASE("One emerald adds 4 gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Emerald)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.goldGained == 4);
  }

  SUBCASE("Two emeralds add 8 gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::Emerald),
      makeItem(Cards::Emerald),
    };
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.goldGained == 8);
  }

  SUBCASE("Emerald with other items") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::Apple),
      makeItem(Cards::Emerald),
      makeItem(Cards::RingOfProtection),
    };
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.goldGained == 4);
  }
}

TEST_CASE("applyDamage — ring-of-courage") {
  SUBCASE("Life equals 1 after damage — ring adds shield") {
    PlayerState player = makePlayer(6, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfCourage)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 5);
    CHECK(result.playerShieldRemaining == 5);
  }

  SUBCASE("Life not 1 after damage — no ring bonus") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfCourage)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.playerShieldRemaining == 0);
  }

  SUBCASE("Multiple rings stack at 1 HP") {
    PlayerState player = makePlayer(6, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::RingOfCourage),
      makeItem(Cards::RingOfCourage),
    };
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.playerShieldRemaining == 10);
  }

  SUBCASE("Ring with existing shield — adds to remaining") {
    PlayerState player = makePlayer(3, 20, 3);
    std::vector<Card> items = {makeItem(Cards::RingOfCourage)};
    CombatResult result = applyDamage(5, player, items);
    CHECK(result.damageToPlayer == 2);
    CHECK(result.playerShieldRemaining == 5);
  }
}

TEST_CASE("applyDamage — all modifiers compose") {
  SUBCASE("Skull + emerald + ring-of-courage + dragon-wing") {
    PlayerState player = makePlayer(4, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::Emerald),
      makeItem(Cards::RingOfCourage),
      makeItem(Cards::DragonWing),
    };
    CombatResult result = applyDamage(3, player, items);
    CHECK(result.damageToPlayer == 3);
    CHECK(result.shieldAbsorbed == 1);
    CHECK(result.playerShieldRemaining == 5);
    CHECK(result.goldGained == 4);
  }

  SUBCASE("Multiple of each modifier") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::Emerald),
      makeItem(Cards::Emerald),
      makeItem(Cards::DragonWing),
      makeItem(Cards::DragonWing),
    };
    CombatResult result = applyDamage(3, player, items);
    CHECK(result.damageToPlayer == 3);
    CHECK(result.shieldAbsorbed == 2);
    CHECK(result.playerShieldRemaining == 0);
    CHECK(result.goldGained == 8);
  }
}

TEST_CASE("applyToShield") {
  SUBCASE("Damage less than shield") {
    ShieldData shield = {10};
    ShieldData result = applyToShield(3, shield);
    CHECK(result.value == 7);
  }

  SUBCASE("Damage equals shield") {
    ShieldData shield = {5};
    ShieldData result = applyToShield(5, shield);
    CHECK(result.value == 0);
  }

  SUBCASE("Damage greater than shield") {
    ShieldData shield = {3};
    ShieldData result = applyToShield(10, shield);
    CHECK(result.value == 0);
  }

  SUBCASE("Zero shield") {
    ShieldData shield = {0};
    ShieldData result = applyToShield(5, shield);
    CHECK(result.value == 0);
  }
}

TEST_CASE("checkInvisibilityCloak") {
  SUBCASE("No cloak — always false") {
    std::vector<Card> items;
    CHECK(checkInvisibilityCloak(items, 10) == false);
    CHECK(checkInvisibilityCloak(items, 7) == false);
  }

  SUBCASE("Cloak with even life — dodge") {
    std::vector<Card> items = {makeItem(Cards::InvisibilityCloak)};
    CHECK(checkInvisibilityCloak(items, 10) == true);
    CHECK(checkInvisibilityCloak(items, 2) == true);
  }

  SUBCASE("Cloak with 0 HP — no dodge") {
    std::vector<Card> items = {makeItem(Cards::InvisibilityCloak)};
    CHECK(checkInvisibilityCloak(items, 0) == false);
  }

  SUBCASE("Cloak with odd life — no dodge") {
    std::vector<Card> items = {makeItem(Cards::InvisibilityCloak)};
    CHECK(checkInvisibilityCloak(items, 7) == false);
    CHECK(checkInvisibilityCloak(items, 1) == false);
    CHECK(checkInvisibilityCloak(items, 3) == false);
  }
}

TEST_CASE("calcMaxHp") {
  SUBCASE("No items — returns base") {
    std::vector<Card> items;
    CHECK(calcMaxHp(items, 20) == 20);
  }

  SUBCASE("Cursed shield — returns 1") {
    std::vector<Card> items = {makeItem(Cards::CursedShield)};
    CHECK(calcMaxHp(items, 20) == 1);
  }

  SUBCASE("Ring of vitality — +5") {
    std::vector<Card> items = {makeItem(Cards::RingOfVitality)};
    CHECK(calcMaxHp(items, 20) == 25);
  }

  SUBCASE("Omelette — +3") {
    std::vector<Card> items = {makeItem(Cards::Omelette)};
    CHECK(calcMaxHp(items, 20) == 23);
  }

  SUBCASE("Multiple omelettes stack") {
    std::vector<Card> items = {
      makeItem(Cards::Omelette),
      makeItem(Cards::Omelette),
    };
    CHECK(calcMaxHp(items, 20) == 26);
  }

  SUBCASE("Cursed shield overrides other bonuses") {
    std::vector<Card> items = {
      makeItem(Cards::Omelette),
      makeItem(Cards::RingOfVitality),
      makeItem(Cards::CursedShield),
    };
    CHECK(calcMaxHp(items, 20) == 1);
  }
}
