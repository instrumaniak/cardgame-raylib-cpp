// src/logic/items.h — Item effects and modifier calculation functions
// Pure game logic layer: no raylib includes
#pragma once

#include "core/components.h"
#include "core/entities.h"
#include "core/types.h"

#include <vector>

namespace game::logic {

struct ComboResult {
  bool triggered;
  std::vector<Card> newItems;
};

// Modifier calculations
int getMonsterCardModifier(const std::vector<Card>& items);
int getHealCardModifier(const std::vector<Card>& items);
int getShieldCardModifier(const std::vector<Card>& items);
int getHeroLifeMaxModifier(const std::vector<Card>& items, int heroLifeMax);
int getNbRowsHidden(const std::vector<Card>& items);

// Item combos and effects
ComboResult checkMonsterEggCombo(const std::vector<Card>& items);
void applyAppleEffect(HealthData& health, const std::vector<Card>& items);
void applyRingOfProtection(ShieldData& shield, const std::vector<Card>& items);
// Preconditions: caller must verify shield was depleted before fight
void applyDragonWing(ShieldData& shield);
void applyAthelas(PlayerState& player, std::vector<Card>& items);
bool applyCursedShield(PlayerState& player, const std::vector<Card>& items);
void applyAmethyst(GoldData& gold, std::vector<Card>& items);

} // namespace game::logic
