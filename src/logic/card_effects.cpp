// src/logic/card_effects.cpp — Card effect resolution implementations
#include "logic/card_effects.h"

#include "core/slugs.h"
#include "logic/combat.h"
#include "logic/items.h"

namespace game::logic {

CardEffect
resolveCard(const Card& card, PlayerState& player, std::vector<Card>& items, std::mt19937& rng) {
  switch (card.cardType) {
  case CardType::Monster:
    return resolveMonster(card, player, items);
  case CardType::Shield:
    return resolveShield(card, player, items);
  case CardType::Heal:
    return resolveHeal(card, player, items);
  case CardType::Gold:
    return resolveGold(card, player, items, rng);
  case CardType::Item:
    return resolveItem(card);
  case CardType::Drop:
    return resolveDrop(card);
  case CardType::Place:
    return resolvePlace(card);
  case CardType::Biome:
    return resolveBiome(card);
  case CardType::EndCard:
    return resolveEnd();
  default:
    return {};
  }
}

CardEffect resolveMonster(const Card& card, PlayerState& player, std::vector<Card>& items) {
  CombatResult combat = applyDamage(card.value, player, items);

  player.health.current -= combat.damageToPlayer;
  player.shield.value = combat.playerShieldRemaining;
  player.gold.amount += combat.goldGained;

  applyAthelas(player, items);

  CardEffect effect{};
  effect.damage = combat.damageToPlayer;
  effect.goldEarned = combat.goldGained;
  effect.soundEffect = Sfx::Hit;
  if (combat.dodged) {
    effect.message = "Dodged!";
  }
  return effect;
}

CardEffect resolveShield(const Card& card, PlayerState& player, const std::vector<Card>& items) {
  int shieldValue = card.value + getShieldCardModifier(items);
  if (shieldValue < 0) {
    shieldValue = 0;
  }

  CardEffect effect{};
  effect.soundEffect = Sfx::Shield;

  if (hasItem(items, Cards::RingOfVitality)) {
    int previousHp = player.health.current;
    player.health.current += shieldValue;
    int maxHp = calcMaxHp(items, player.health.max);
    if (player.health.current > maxHp) {
      player.health.current = maxHp;
    }
    effect.healed = player.health.current - previousHp;
  } else if (hasItem(items, Cards::CursedShield)) {
    player.shield.value += shieldValue;
    effect.shieldGained = shieldValue;
  } else {
    player.shield.value = shieldValue;
    effect.shieldGained = shieldValue;
  }

  return effect;
}

CardEffect resolveHeal(const Card& card, PlayerState& player, const std::vector<Card>& items) {
  int heal = card.value + getHealCardModifier(items);
  if (heal < 0) {
    heal = 0;
  }

  int previousHp = player.health.current;
  player.health.current += heal;
  int maxHp = calcMaxHp(items, player.health.max);
  if (player.health.current > maxHp) {
    player.health.current = maxHp;
  }

  CardEffect effect{};
  effect.healed = player.health.current - previousHp;
  effect.soundEffect = Sfx::None;
  return effect;
}

CardEffect
resolveGold(const Card& card, PlayerState& player, std::vector<Card>& items, std::mt19937& rng) {
  player.gold.amount += card.value;
  int goldEarned = card.value;

  if (hasItem(items, Cards::Ruby)) {
    std::uniform_int_distribution<int> dist(0, 3);
    if (dist(rng) == 0) {
      player.health.current += 1;
    }
  }

  if (hasItem(items, Cards::Amethyst)) {
    player.gold.amount += card.value;
    goldEarned += card.value;
  }

  if (hasItem(items, Cards::RingOfGreed)) {
    player.health.current -= 1;
  }

  // Remove amethyst after applying its effect
  for (auto it = items.begin(); it != items.end(); ++it) {
    if (it->slug == Cards::Amethyst) {
      items.erase(it);
      break;
    }
  }

  CardEffect effect{};
  effect.goldEarned = goldEarned;
  effect.soundEffect = Sfx::Coins;
  return effect;
}

CardEffect resolveItem(const Card& card) {
  CardEffect effect{};
  effect.message = card.slug;
  effect.soundEffect = Sfx::Equip;
  return effect;
}

CardEffect resolveDrop(const Card& card) {
  CardEffect effect{};
  effect.message = card.slug;
  effect.soundEffect = Sfx::Chest;
  return effect;
}

CardEffect resolvePlace(const Card& card) {
  CardEffect effect{};
  effect.message = card.slug;
  effect.soundEffect = Sfx::Chest;
  return effect;
}

CardEffect resolveBiome(const Card& card) {
  CardEffect effect{};
  effect.message = card.slug;
  effect.soundEffect = Sfx::None;
  return effect;
}

CardEffect resolveEnd() {
  CardEffect effect{};
  effect.message = Cards::End;
  effect.soundEffect = Sfx::Win;
  return effect;
}

} // namespace game::logic
