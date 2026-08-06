// src/logic/account.cpp — Account save/load, hero unlock, gold economy implementations
#include "logic/account.h"

#include "core/json_keys.h"
#include "core/slugs.h"

#include <algorithm>
#include <nlohmann/json.hpp>

using json = nlohmann::json;

namespace game::logic {

using namespace game;

namespace {

json heroToJson(const HeroSave& hero) {
  json j;
  j[std::string(JsonKeys::Slug)] = hero.slug;
  j[std::string(JsonKeys::Locked)] = hero.locked;
  j[std::string(JsonKeys::Life)] = hero.life;
  j[std::string(JsonKeys::LifeMax)] = hero.lifeMax;
  j[std::string(JsonKeys::Shield)] = hero.shield;
  j[std::string(JsonKeys::Gold)] = hero.gold;
  j[std::string(JsonKeys::Bag)] = hero.bag;
  j[std::string(JsonKeys::Cost)] = hero.cost;
  return j;
}

HeroSave jsonToHero(const json& j) {
  HeroSave hero;
  hero.slug = j.value(std::string(JsonKeys::Slug), "");
  hero.locked = j.value(std::string(JsonKeys::Locked), true);
  hero.life = j.value(std::string(JsonKeys::Life), 0);
  hero.lifeMax = j.value(std::string(JsonKeys::LifeMax), 0);
  hero.shield = j.value(std::string(JsonKeys::Shield), 0);
  hero.gold = j.value(std::string(JsonKeys::Gold), 0);
  hero.bag = j.value(std::string(JsonKeys::Bag), std::vector<std::string>{});
  hero.cost = j.value(std::string(JsonKeys::Cost), 0);
  return hero;
}

} // anonymous namespace

Account createDefaultAccount() {
  Account account;
  account.gold = 0;
  account.language = Lang::English;

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
  j[std::string(JsonKeys::Gold)] = account.gold;
  j[std::string(JsonKeys::Config)][std::string(JsonKeys::Lang)] = account.language;
  json heroesArr = json::array();
  for (const auto& hero : account.heroes) {
    heroesArr.push_back(heroToJson(hero));
  }
  j[std::string(JsonKeys::Collection)][std::string(JsonKeys::Heroes)] = heroesArr;
  j[std::string(JsonKeys::Collection)][std::string(JsonKeys::Cards)] = json::array();
  return j.dump(2);
}

Account loadAccount(const std::string& jsonStr) {
  try {
    json j = json::parse(jsonStr);
    Account account;
    account.gold = j.value(std::string(JsonKeys::Gold), 0);
    account.language = j.value(std::string(JsonKeys::Config), json::object())
                         .value(std::string(JsonKeys::Lang), std::string(Lang::English));
    auto heroes = j.value(std::string(JsonKeys::Collection), json::object())
                    .value(std::string(JsonKeys::Heroes), json::array());
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
