// src/logic/account.cpp — Account save/load, hero unlock, gold economy implementations
#include "logic/account.h"

#include "core/slugs.h"

#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace game::logic {

namespace {

json heroToJson(const HeroSave& hero) {
  json j;
  j["slug"] = hero.slug;
  j["locked"] = hero.locked;
  j["life"] = hero.life;
  j["life_max"] = hero.lifeMax;
  j["shield"] = hero.shield;
  j["gold"] = hero.gold;
  j["bag"] = hero.bag;
  j["cost"] = hero.cost;
  return j;
}

HeroSave jsonToHero(const json& j) {
  HeroSave hero;
  hero.slug = j.value("slug", "");
  hero.locked = j.value("locked", true);
  hero.life = j.value("life", 0);
  hero.lifeMax = j.value("life_max", 0);
  hero.shield = j.value("shield", 0);
  hero.gold = j.value("gold", 0);
  hero.bag = j.value("bag", std::vector<std::string>{});
  hero.cost = j.value("cost", 0);
  return hero;
}

} // anonymous namespace

Account createDefaultAccount() {
  Account account;
  account.gold = 0;
  account.language = "en";

  auto makeHero = [](
                    std::string_view slug,
                    int lifeMax,
                    int shield,
                    int cost,
                    bool locked,
                    std::vector<std::string> bag
                  ) {
    HeroSave h;
    h.slug = std::string(slug);
    h.lifeMax = lifeMax;
    h.life = lifeMax;
    h.shield = shield;
    h.cost = cost;
    h.locked = locked;
    h.gold = 0;
    h.bag = std::move(bag);
    return h;
  };

  account.heroes.push_back(makeHero(
    Heroes::HeadlessKnight, 15, 0, 0, false, {std::string(Cards::Map), std::string(Cards::Apple)}
  ));
  account.heroes.push_back(makeHero(
    Heroes::Thief,
    12,
    0,
    50,
    true,
    {std::string(Cards::InvisibilityCloak), std::string(Cards::MonsterEye)}
  ));
  account.heroes.push_back(
    makeHero(Heroes::Harpy, 15, 0, 100, true, {std::string(Cards::RingOfVitality)})
  );
  account.heroes.push_back(makeHero(
    Heroes::Warrior,
    10,
    10,
    200,
    true,
    {std::string(Cards::ShinyShield), std::string(Cards::Emerald)}
  ));

  return account;
}

std::string saveAccount(const Account& account) {
  json j;
  j["gold"] = account.gold;
  j["config"]["lang"] = account.language;
  json heroesArr = json::array();
  for (const auto& hero : account.heroes) {
    heroesArr.push_back(heroToJson(hero));
  }
  j["collection"]["heroes"] = heroesArr;
  j["collection"]["cards"] = json::array();
  return j.dump(2);
}

Account loadAccount(const std::string& jsonStr) {
  try {
    json j = json::parse(jsonStr);
    Account account;
    account.gold = j.value("gold", 0);
    account.language = j.value("config", json::object()).value("lang", "en");
    auto heroes = j.value("collection", json::object()).value("heroes", json::array());
    for (const auto& h : heroes) {
      account.heroes.push_back(jsonToHero(h));
    }
    return account;
  } catch (...) {
    return createDefaultAccount();
  }
}

bool unlockHero(HeroSave& hero, Account& account) {
  if (!hero.locked) {
    return false;
  }
  if (hero.cost > account.gold) {
    return false;
  }
  account.gold -= hero.cost;
  hero.locked = false;
  return true;
}

bool spendGold(Account& account, int amount) {
  if (amount <= 0 || amount > account.gold) {
    return false;
  }
  account.gold -= amount;
  return true;
}

void addGold(Account& account, int amount) {
  if (amount > 0) {
    account.gold += amount;
  }
}

} // namespace game::logic
