// src/logic/test_card_effects.cpp — Unit tests for card effect resolution
#include "core/slugs.h"
#include "doctest/doctest.h"
#include "logic/card_effects.h"

#include <random>

using namespace game;
using namespace game::logic;

using game::Cards::End;

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

static Card makeMonster(int value) {
  Card card;
  card.slug = std::string(Cards::Forest);
  card.cardType = CardType::Monster;
  card.value = value;
  card.name = "Forest Monster";
  card.description = "";
  return card;
}

static Card makeShield(int value) {
  Card card;
  card.slug = std::string(Cards::WoodenShield);
  card.cardType = CardType::Shield;
  card.value = value;
  card.name = "Wooden Shield";
  card.description = "";
  return card;
}

static Card makeHeal(int value) {
  Card card;
  card.slug = std::string(Cards::MinorHealingPotion);
  card.cardType = CardType::Heal;
  card.value = value;
  card.name = "Minor Healing Potion";
  card.description = "";
  return card;
}

static Card makeGold(int value) {
  Card card;
  card.slug = std::string(Cards::OneCoin);
  card.cardType = CardType::Gold;
  card.value = value;
  card.name = "Coin";
  card.description = "";
  return card;
}

TEST_CASE("resolveMonster — basic cases") {
  SUBCASE("No items, damage equals base") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 5);
    CHECK(player.health.current == 15);
    CHECK(player.shield.value == 0);
    CHECK(effect.goldEarned == 0);
  }

  SUBCASE("Shield absorbs all damage") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items;
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 0);
    CHECK(player.health.current == 20);
    CHECK(player.shield.value == 5);
  }

  SUBCASE("Shield absorbs partial damage with overflow") {
    PlayerState player = makePlayer(20, 20, 3);
    std::vector<Card> items;
    Card monster = makeMonster(10);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 7);
    CHECK(player.health.current == 13);
    CHECK(player.shield.value == 0);
  }
}

TEST_CASE("resolveMonster — monster-skull modifier") {
  SUBCASE("One skull adds 1 to damage") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::MonsterSkull)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 6);
    CHECK(player.health.current == 14);
  }

  SUBCASE("Three skulls add 3 to damage") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
      makeItem(Cards::MonsterSkull),
    };
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 8);
    CHECK(player.health.current == 12);
  }
}

TEST_CASE("resolveMonster — dragon-wing") {
  SUBCASE("Dragon-wing replaces shield with count") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items = {makeItem(Cards::DragonWing)};
    Card monster = makeMonster(3);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 2);
    CHECK(player.shield.value == 0);
  }

  SUBCASE("Two dragon-wings set shield to 2") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::DragonWing),
      makeItem(Cards::DragonWing),
    };
    Card monster = makeMonster(1);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 0);
    CHECK(player.shield.value == 1);
  }
}

TEST_CASE("resolveMonster — emerald gold bonus") {
  SUBCASE("One emerald adds 4 gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Emerald)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.goldEarned == 4);
    CHECK(player.gold.amount == 4);
  }

  SUBCASE("Two emeralds add 8 gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::Emerald),
      makeItem(Cards::Emerald),
    };
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.goldEarned == 8);
    CHECK(player.gold.amount == 8);
  }
}

TEST_CASE("resolveMonster — ring-of-courage") {
  SUBCASE("Life equals 1 after damage — ring adds shield") {
    PlayerState player = makePlayer(6, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfCourage)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(effect.damage == 5);
    CHECK(player.health.current == 1);
    CHECK(player.shield.value == 5);
  }

  SUBCASE("Life not 1 after damage — no ring bonus") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfCourage)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(player.shield.value == 0);
  }
}

TEST_CASE("resolveMonster — athelas death prevention") {
  SUBCASE("Athelas prevents death and is consumed") {
    PlayerState player = makePlayer(1, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Athelas)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(player.health.current == 1);
    CHECK(player.athelasUsed == true);
    CHECK(items.empty());
  }

  SUBCASE("Athelas not used if HP stays above 0") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Athelas)};
    Card monster = makeMonster(5);
    CardEffect effect = resolveMonster(monster, player, items);
    CHECK(player.health.current == 5);
    CHECK(player.athelasUsed == false);
    CHECK(items.size() == 1);
  }
}

TEST_CASE("resolveShield — basic cases") {
  SUBCASE("Shield replaces existing shield") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items;
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 5);
    CHECK(effect.shieldGained == 5);
  }

  SUBCASE("Shield value floors at 0") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    Card shield = makeShield(0);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 0);
  }
}

TEST_CASE("resolveShield — shiny-shield and rusty-shield") {
  SUBCASE("Shiny-shield adds +1") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::ShinyShield)};
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 6);
  }

  SUBCASE("Rusty-shield subtracts -1") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RustyShield)};
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 4);
  }

  SUBCASE("Negative shield floors at 0") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RustyShield)};
    Card shield = makeShield(0);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 0);
  }
}

TEST_CASE("resolveShield — ring-of-vitality converts to heal") {
  SUBCASE("Shield value added to life instead") {
    PlayerState player = makePlayer(15, 20, 10);
    std::vector<Card> items = {makeItem(Cards::RingOfVitality)};
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.health.current == 20);
    CHECK(player.shield.value == 10);
    CHECK(effect.healed == 5);
  }

  SUBCASE("Heal capped at max HP") {
    // Ring-of-vitality adds +5 max HP, so effective max = 25
    PlayerState player = makePlayer(23, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfVitality)};
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.health.current == 25);
    CHECK(effect.healed == 2);
  }
}

TEST_CASE("resolveShield — cursed-shield makes additive") {
  SUBCASE("Shield added to existing shield") {
    PlayerState player = makePlayer(20, 20, 10);
    std::vector<Card> items = {makeItem(Cards::CursedShield)};
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.shield.value == 15);
    CHECK(effect.shieldGained == 5);
  }
}

TEST_CASE("resolveShield — ring-of-vitality takes priority over cursed-shield") {
  SUBCASE("Ring-of-vitality converts to heal even with cursed-shield") {
    // Cursed-shield sets max HP to 1, so heal is capped at 1
    PlayerState player = makePlayer(0, 20, 10);
    std::vector<Card> items = {
      makeItem(Cards::RingOfVitality),
      makeItem(Cards::CursedShield),
    };
    Card shield = makeShield(5);
    CardEffect effect = resolveShield(shield, player, items);
    CHECK(player.health.current == 1);
    CHECK(player.shield.value == 10);
    CHECK(effect.healed == 1);
  }
}

TEST_CASE("resolveHeal — basic cases") {
  SUBCASE("Heal adds to current HP") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items;
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 15);
    CHECK(effect.healed == 5);
  }

  SUBCASE("Heal capped at max HP") {
    PlayerState player = makePlayer(18, 20, 0);
    std::vector<Card> items;
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 20);
    CHECK(effect.healed == 2);
  }

  SUBCASE("Already at max HP — no heal") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 20);
    CHECK(effect.healed == 0);
  }
}

TEST_CASE("resolveHeal — raw-meat and rotten-meat modifiers") {
  SUBCASE("Raw-meat adds +1 heal") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RawMeat)};
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 16);
    CHECK(effect.healed == 6);
  }

  SUBCASE("Rotten-meat subtracts -1 heal") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RottenMeat)};
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 14);
    CHECK(effect.healed == 4);
  }

  SUBCASE("Multiple raw-meat stack") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::RawMeat),
      makeItem(Cards::RawMeat),
    };
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 17);
    CHECK(effect.healed == 7);
  }

  SUBCASE("Mixed meat — net +1") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::RawMeat),
      makeItem(Cards::RawMeat),
      makeItem(Cards::RottenMeat),
    };
    Card heal = makeHeal(5);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 16);
    CHECK(effect.healed == 6);
  }
}

TEST_CASE("resolveHeal — heal minimum is 0") {
  SUBCASE("Negative heal floors at 0") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::RottenMeat),
      makeItem(Cards::RottenMeat),
    };
    Card heal = makeHeal(1);
    CardEffect effect = resolveHeal(heal, player, items);
    CHECK(player.health.current == 10);
    CHECK(effect.healed == 0);
  }
}

TEST_CASE("resolveGold — basic cases") {
  SUBCASE("Gold adds to player gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 5);
    CHECK(effect.goldEarned == 5);
  }
}

TEST_CASE("resolveGold — ruby 25% HP") {
  SUBCASE("Ruby procs — HP increases") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Ruby)};
    // Seed 5 produces dist(rng) == 0 (25% proc)
    std::mt19937 rng(5);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 5);
    CHECK(player.health.current == 21);
  }

  SUBCASE("Ruby does not proc — HP unchanged") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Ruby)};
    std::mt19937 rng(1);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 5);
    CHECK(player.health.current == 20);
  }
}

TEST_CASE("resolveGold — amethyst doubles gold") {
  SUBCASE("Amethyst doubles gold and is consumed") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::Amethyst)};
    std::mt19937 rng(42);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 10);
    CHECK(effect.goldEarned == 10);
    CHECK(items.empty());
  }
}

TEST_CASE("resolveGold — ring-of-greed") {
  SUBCASE("Ring-of-greed costs 1 HP") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {makeItem(Cards::RingOfGreed)};
    std::mt19937 rng(42);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 5);
    CHECK(player.health.current == 19);
  }
}

TEST_CASE("resolveGold — amethyst + ring-of-greed") {
  SUBCASE("Both apply — double gold and lose 1 HP") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::Amethyst),
      makeItem(Cards::RingOfGreed),
    };
    std::mt19937 rng(42);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 10);
    CHECK(player.health.current == 19);
    CHECK(items.size() == 1);
    CHECK(items[0].slug == Cards::RingOfGreed);
  }
}

TEST_CASE("resolveGold — ruby + amethyst") {
  SUBCASE("Both apply — 25% HP and double gold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items = {
      makeItem(Cards::Ruby),
      makeItem(Cards::Amethyst),
    };
    std::mt19937 rng(5);
    Card gold = makeGold(5);
    CardEffect effect = resolveGold(gold, player, items, rng);
    CHECK(player.gold.amount == 10);
    CHECK(player.health.current == 21);
    CHECK(items.size() == 1);
    CHECK(items[0].slug == Cards::Ruby);
  }
}

TEST_CASE("resolveItem — returns slug in message") {
  Card item;
  item.slug = std::string(Cards::Ruby);
  item.cardType = CardType::Item;
  item.value = 0;
  item.name = "Ruby";
  item.description = "";
  CardEffect effect = resolveItem(item);
  CHECK(effect.message == Cards::Ruby);
  CHECK(effect.soundEffect == Sfx::Equip);
}

TEST_CASE("resolveDrop — returns slug in message") {
  Card drop;
  drop.slug = std::string(Cards::MonsterEgg);
  drop.cardType = CardType::Drop;
  drop.value = 0;
  drop.name = "Monster Egg";
  drop.description = "";
  CardEffect effect = resolveDrop(drop);
  CHECK(effect.message == Cards::MonsterEgg);
  CHECK(effect.soundEffect == Sfx::Chest);
}

TEST_CASE("resolvePlace — returns slug in message") {
  Card place;
  place.slug = std::string(Cards::Chest);
  place.cardType = CardType::Place;
  place.value = 0;
  place.name = "Chest";
  place.description = "";
  CardEffect effect = resolvePlace(place);
  CHECK(effect.message == Cards::Chest);
  CHECK(effect.soundEffect == Sfx::Chest);
}

TEST_CASE("resolveBiome — returns slug in message") {
  Card biome;
  biome.slug = std::string(Cards::Forest);
  biome.cardType = CardType::Biome;
  biome.value = 0;
  biome.name = "Forest";
  biome.description = "";
  CardEffect effect = resolveBiome(biome);
  CHECK(effect.message == Cards::Forest);
  CHECK(effect.soundEffect == Sfx::None);
}

TEST_CASE("resolveEnd — returns end message") {
  CardEffect effect = resolveEnd();
  CHECK(effect.message == End);
  CHECK(effect.soundEffect == Sfx::Win);
}

TEST_CASE("resolveCard — dispatch by CardType") {
  SUBCASE("Monster dispatches to resolveMonster") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card monster = makeMonster(5);
    CardEffect effect = resolveCard(monster, player, items, rng);
    CHECK(effect.damage == 5);
    CHECK(player.health.current == 15);
  }

  SUBCASE("Shield dispatches to resolveShield") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card shield = makeShield(5);
    CardEffect effect = resolveCard(shield, player, items, rng);
    CHECK(player.shield.value == 5);
  }

  SUBCASE("Heal dispatches to resolveHeal") {
    PlayerState player = makePlayer(10, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card heal = makeHeal(5);
    CardEffect effect = resolveCard(heal, player, items, rng);
    CHECK(player.health.current == 15);
  }

  SUBCASE("Gold dispatches to resolveGold") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card gold = makeGold(5);
    CardEffect effect = resolveCard(gold, player, items, rng);
    CHECK(player.gold.amount == 5);
  }

  SUBCASE("Item dispatches to resolveItem") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card item;
    item.slug = std::string(Cards::Ruby);
    item.cardType = CardType::Item;
    CardEffect effect = resolveCard(item, player, items, rng);
    CHECK(effect.message == Cards::Ruby);
  }

  SUBCASE("EndCard dispatches to resolveEnd") {
    PlayerState player = makePlayer(20, 20, 0);
    std::vector<Card> items;
    std::mt19937 rng(42);
    Card end;
    end.slug = std::string(Cards::End);
    end.cardType = CardType::EndCard;
    CardEffect effect = resolveCard(end, player, items, rng);
    CHECK(effect.message == Cards::End);
    CHECK(effect.soundEffect == Sfx::Win);
  }
}
