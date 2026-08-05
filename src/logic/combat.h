#pragma once

#include "core/components.h"
#include "core/entities.h"
#include "core/types.h"

#include <vector>

namespace game::logic {

struct CombatResult {
  int damageToPlayer;
  int shieldAbsorbed;
  int playerShieldRemaining;
  bool dodged;
  int goldGained;
};

CombatResult applyDamage(int baseDamage, const PlayerState& player, const std::vector<Card>& items);

ShieldData applyToShield(int damage, const ShieldData& shield);

bool checkInvisibilityCloak(const std::vector<Card>& items, int currentHp);

int calcMaxHp(const std::vector<Card>& items, int baseMax);

} // namespace game::logic
