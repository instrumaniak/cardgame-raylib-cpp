// src/logic/items.cpp — Item effects and modifier calculation implementations
#include "logic/items.h"

#include "core/slugs.h"

#include <algorithm>

namespace game::logic {

int getMonsterCardModifier(const std::vector<Card>& items) {
  int modifier = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::MonsterSkull) {
      modifier += 1;
    }
  }
  return modifier;
}

int getHealCardModifier(const std::vector<Card>& items) {
  int modifier = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::RawMeat) {
      modifier += 1;
    } else if (item.slug == Cards::RottenMeat) {
      modifier -= 1;
    }
  }
  return modifier;
}

int getShieldCardModifier(const std::vector<Card>& items) {
  int modifier = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::ShinyShield) {
      modifier += 1;
    } else if (item.slug == Cards::RustyShield) {
      modifier -= 1;
    }
  }
  return modifier;
}

int getHeroLifeMaxModifier(const std::vector<Card>& items, int heroLifeMax) {
  int modifier = 0;
  bool hasCursedShield = false;
  for (const auto& item : items) {
    if (item.slug == Cards::Omelette) {
      modifier += 3;
    } else if (item.slug == Cards::RingOfVitality) {
      modifier += 5;
    } else if (item.slug == Cards::CursedShield) {
      hasCursedShield = true;
    }
  }
  return hasCursedShield ? -(heroLifeMax - 1) : modifier;
}

int getNbRowsHidden(const std::vector<Card>& items) {
  int count = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::MonsterEye) {
      count++;
    }
  }
  return std::min(count, 2);
}

ComboResult checkMonsterEggCombo(const std::vector<Card>& items) {
  int eggCount = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::MonsterEgg) {
      eggCount++;
    }
  }
  if (eggCount >= 3) {
    int removed = 0;
    std::vector<Card> newItems;
    for (const auto& item : items) {
      if (item.slug == Cards::MonsterEgg && removed < 3) {
        removed++;
      } else {
        newItems.push_back(item);
      }
    }
    Card omelette;
    omelette.slug = Cards::Omelette;
    omelette.cardType = CardType::Item;
    omelette.value = 0;
    omelette.name = "Omelette";
    omelette.description = "+3 max HP";
    newItems.push_back(omelette);
    return {true, newItems};
  }
  return {false, items};
}

void applyAppleEffect(HealthData& health, const std::vector<Card>& items) {
  int modifier = getHeroLifeMaxModifier(items, health.max);
  int effectiveMax = health.max + modifier;
  for (const auto& item : items) {
    if (item.slug == Cards::Apple) {
      health.current += 10;
      if (health.current > effectiveMax) {
        health.current = effectiveMax;
      }
    }
  }
}

void applyRingOfProtection(ShieldData& shield, const std::vector<Card>& items) {
  for (const auto& item : items) {
    if (item.slug == Cards::RingOfProtection) {
      shield.value += 5;
    }
  }
}

void applyDragonWing(ShieldData& shield) {
  shield.value += 1;
}

void applyAthelas(PlayerState& player, std::vector<Card>& items) {
  if (player.athelasUsed) {
    return;
  }
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->slug == Cards::Athelas) {
      if (player.health.current <= 0) {
        player.health.current = 1;
        player.athelasUsed = true;
        items.erase(it);
      }
      break;
    }
  }
}

bool applyCursedShield(PlayerState& player, const std::vector<Card>& items) {
  for (const auto& item : items) {
    if (item.slug == Cards::CursedShield) {
      if (player.health.current > 1) {
        player.health.current = 1;
      }
      return true;
    }
  }
  return false;
}

void applyAmethyst(GoldData& gold, std::vector<Card>& items) {
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->slug == Cards::Amethyst) {
      gold.amount *= 2;
      items.erase(it);
      break;
    }
  }
}

} // namespace game::logic
