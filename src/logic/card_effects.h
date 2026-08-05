// src/logic/card_effects.h — Card effect resolution for all card types
// Pure game logic layer: no raylib includes, no I/O
#pragma once

#include "core/entities.h"
#include "core/types.h"

#include <random>
#include <vector>

namespace game::logic {

CardEffect
resolveCard(const Card& card, PlayerState& player, std::vector<Card>& items, std::mt19937& rng);

CardEffect resolveMonster(const Card& card, PlayerState& player, std::vector<Card>& items);
CardEffect resolveShield(const Card& card, PlayerState& player, const std::vector<Card>& items);
CardEffect resolveHeal(const Card& card, PlayerState& player, const std::vector<Card>& items);
CardEffect
resolveGold(const Card& card, PlayerState& player, std::vector<Card>& items, std::mt19937& rng);
CardEffect resolveItem(const Card& card);
CardEffect resolveDrop(const Card& card);
CardEffect resolvePlace(const Card& card);
CardEffect resolveBiome(const Card& card);
CardEffect resolveEnd();

} // namespace game::logic
