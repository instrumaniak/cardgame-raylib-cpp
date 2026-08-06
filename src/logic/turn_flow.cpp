// src/logic/turn_flow.cpp — Turn sequence orchestrator implementation
// Pure game logic layer: no raylib includes, no I/O
#include "logic/turn_flow.h"

#include "core/slugs.h"
#include "logic/board_gen.h"
#include "logic/card_effects.h"
#include "logic/combat.h"
#include "logic/items.h"
#include "logic/level_config.h"

#include <algorithm>
#include <cmath>

namespace game::logic {

bool playCard(GameState& gameState, int row, int col, std::mt19937& rng) {
  auto& player = gameState.player;
  auto& board = gameState.board;

  if (board.empty()) {
    return false;
  }
  if (row < 0 || row >= static_cast<int>(board.size())) {
    return false;
  }
  if (col < 0 || col >= static_cast<int>(board[row].size())) {
    return false;
  }
  if (row != gameState.currentRow) {
    return false;
  }
  if (std::abs(col - gameState.currentCol) > 1) {
    return false;
  }

  auto& slot = board[row][col];
  if (slot.consumed) {
    return false;
  }

  const Card& card = slot.card;

  // Resolve card effect — handles Monster/Shield/Heal/Gold player mutation
  resolveCard(card, player, player.inventory.items, rng);

  // Handle Item/Drop: add card to bag and check combos
  if (card.cardType == CardType::Item || card.cardType == CardType::Drop) {
    player.inventory.items.push_back(card);
    ComboResult combo = checkMonsterEggCombo(player.inventory.items);
    if (combo.triggered) {
      player.inventory.items = combo.newItems;
    }
  }

  // Carrot: +1 HP when moving to a different position
  if (hasItem(player.inventory.items, Cards::Carrot) && col != gameState.currentCol) {
    player.health.current += 1;
  }

  // Cap HP at effective max, clamp life and shield to >= 0
  int maxHp = calcMaxHp(player.inventory.items, player.health.max);
  player.health.current = std::min(player.health.current, maxHp);
  player.health.current = std::max(player.health.current, 0);
  player.shield.value = std::max(player.shield.value, 0);

  // Update hero position
  gameState.currentRow = row;
  gameState.currentCol = col;

  // Mark slot consumed
  slot.consumed = true;

  // Set HeroDrop status — special types override below
  gameState.status = GameStatus::HeroDrop;

  // Handle special card types that end the turn immediately
  switch (card.cardType) {
  case CardType::Biome:
    gameState.currentLevel++;
    gameState.status = GameStatus::NotReady;
    return true;
  case CardType::Place:
    gameState.inChest = true;
    gameState.chestType = card.slug;
    gameState.status = GameStatus::NotReady;
    return true;
  case CardType::EndCard:
    gameState.status = GameStatus::Win;
    gameState.goldEarned = player.gold.amount;
    return true;
  default:
    break;
  }

  // Check for passive monster attack from the row below
  checkMonsterPlay(gameState, rng);

  return true;
}

void checkMonsterPlay(GameState& gameState, std::mt19937& rng) {
  auto& player = gameState.player;
  auto& board = gameState.board;

  int nextRow = gameState.currentRow + 1;
  if (nextRow >= static_cast<int>(board.size())) {
    nextTurn(gameState);
    return;
  }

  auto& nextSlot = board[nextRow][gameState.currentCol];
  if (nextSlot.consumed || nextSlot.card.cardType != CardType::Monster) {
    nextTurn(gameState);
    return;
  }

  // Invisibility cloak: dodge passive attack when life is even
  if (checkInvisibilityCloak(player.inventory.items, player.health.current)) {
    nextTurn(gameState);
    return;
  }

  // Passive monster attack — resolveMonster calls applyDamage + applyAthelas
  // Ring-of-courage is handled inside applyDamage (adds shield when lifeAfterDamage == 1)
  resolveMonster(nextSlot.card, player, player.inventory.items);

  gameState.status = GameStatus::MonsterAttack;

  // Replace killed monster with loot
  LevelConfig levelConfig = getLevelConfig(gameState.currentLevel);
  Card loot = spawnLoot(levelConfig, player.inventory.items, rng);
  nextSlot.card = loot;

  nextTurn(gameState);
}

void nextTurn(GameState& gameState) {
  auto& player = gameState.player;

  // Death check — defensive safety-net. In practice, resolveMonster already
  // calls applyAthelas which handles non-combat deaths. This catches any
  // edge case where health reaches 0 through an unanticipated path.
  if (player.health.current <= 0) {
    if (hasItem(player.inventory.items, Cards::Athelas)) {
      player.health.current = 1;
      removeFirstItem(player.inventory.items, Cards::Athelas);
    } else {
      die(gameState);
      return;
    }
  }

  // Advance to next row and set Ready
  gameState.currentRow++;
  gameState.status = GameStatus::Ready;
}

int die(GameState& gameState) {
  gameState.status = GameStatus::Lose;
  int reward = gameState.player.gold.amount / 2;
  gameState.goldEarned = reward;
  return reward;
}

} // namespace game::logic
