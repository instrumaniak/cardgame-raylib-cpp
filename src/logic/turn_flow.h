// src/logic/turn_flow.h — Turn sequence orchestrator
// Pure game logic layer: no raylib includes, no I/O
#pragma once

#include "core/entities.h"
#include "core/types.h"

#include <random>

namespace game::logic {

// Main card interaction entry point.
// Validates the drop, resolves the card effect, handles special types
// (biome/place/end), applies carrot, caps/clamps stats.
// Returns true if the drop was valid and processed.
bool playCard(GameState& gameState, int row, int col, std::mt19937& rng);

// Check for passive monster attack from the row below.
// Applies damage, ring-of-courage, spawns loot to replace the monster.
// Sets status to MonsterAttack if a monster was found.
void checkMonsterPlay(GameState& gameState, std::mt19937& rng);

// Advance to next turn after card resolution.
// Checks for death (athelas saves once), advances the row, sets status to Ready.
void nextTurn(GameState& gameState);

// Handle hero death — sets status to Lose.
// Returns the gold reward (hero.gold / 2) to be added to the account.
int die(GameState& gameState);

} // namespace game::logic
