// src/logic/combat.cpp — Combat damage calculation with shield absorption
#include "logic/combat.h"

#include "core/slugs.h"
#include "logic/items.h"

#include <algorithm>

namespace game::logic {

CombatResult
applyDamage(int baseDamage, const PlayerState& player, const std::vector<Card>& items) {
  CombatResult result{};

  int damage = baseDamage + getMonsterCardModifier(items);

  int dragonWingCount = 0;
  bool hasDragonWing = false;
  for (const auto& item : items) {
    if (item.slug == Cards::DragonWing) {
      dragonWingCount++;
      hasDragonWing = true;
    }
  }

  int effectiveShield = player.shield.value;
  if (hasDragonWing) {
    effectiveShield = dragonWingCount;
  }

  int overflowDamage = 0;
  if (effectiveShield > 0) {
    int remaining = effectiveShield - damage;
    if (remaining < 0) {
      overflowDamage = -remaining;
      remaining = 0;
    }
    result.shieldAbsorbed = effectiveShield - remaining;
    result.playerShieldRemaining = remaining;
    result.damageToPlayer = overflowDamage;
  } else {
    result.damageToPlayer = damage;
    result.playerShieldRemaining = 0;
    result.shieldAbsorbed = 0;
  }

  result.goldGained = 0;
  for (const auto& item : items) {
    if (item.slug == Cards::Emerald) {
      result.goldGained += 4;
    }
  }

  int lifeAfterDamage = player.health.current - result.damageToPlayer;
  if (lifeAfterDamage == 1) {
    int ringBonus = 0;
    for (const auto& item : items) {
      if (item.slug == Cards::RingOfCourage) {
        ringBonus += 5;
      }
    }
    result.playerShieldRemaining += ringBonus;
  }

  result.dodged = false;

  return result;
}

ShieldData applyToShield(int damage, const ShieldData& shield) {
  ShieldData result{};
  int remaining = shield.value - damage;
  if (remaining < 0) {
    result.value = 0;
  } else {
    result.value = remaining;
  }
  return result;
}

bool checkInvisibilityCloak(const std::vector<Card>& items, int currentHp) {
  if (currentHp <= 0) {
    return false;
  }
  for (const auto& item : items) {
    if (item.slug == Cards::InvisibilityCloak) {
      return currentHp % 2 == 0;
    }
  }
  return false;
}

int calcMaxHp(const std::vector<Card>& items, int baseMax) {
  int modifier = getHeroLifeMaxModifier(items, baseMax);
  return baseMax + modifier;
}

} // namespace game::logic
