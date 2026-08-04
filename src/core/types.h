// src/core/types.h — Core enums and data structs
#pragma once

#include "raylib.h"

#include <array>
#include <string>
#include <vector>

namespace game {

enum class ScreenID { Home, Game, Win, Lose };
enum class GameStatus { NotStarted, NotReady, Ready, HeroDrop, MonsterAttack, Win, Lose };
enum class CardType { Monster, Shield, Heal, Gold, Item, Drop, Place, Biome, EndCard };
enum class Biome { Forest, Cave, EnchantedLands, Desert, Abyss, Underworld };
enum class EffectType { DamageFlash, HealSparkle, CardFlip, GoldCoin, FogFade, ScreenShake };
enum class Sfx { Hit, Shield, Equip, DealCard, Chest, Coins, Lose, Win, None };

struct Card {
  std::string slug;
  CardType cardType;
  int value;
  std::string name;
  std::string description;
  int spriteX, spriteY, spriteW, spriteH;
};

struct Hero {
  std::string slug;
  std::string name;
  std::string description;
  int lifeMax;
  int cost;
  std::vector<std::string> startingItems;
  int spriteX, spriteY;
  bool locked;
};

struct LevelConfig {
  int rows;
  std::array<int, 3> cardsPerRow;
  int monsterRate;
  int shieldRate;
  int healRate;
  int goldRate;
  int itemRate;
  std::vector<int> monsterValues;
};

struct CardEffect {
  int damage;
  int healed;
  int goldEarned;
  int shieldGained;
  std::string message;
  Sfx soundEffect;
};

struct Button {
  Rectangle bounds;
  std::string text;
  bool hovered;
  std::string textureKey;
};

struct HeroSave {
  std::string slug;
  bool locked;
  int life;
  int lifeMax;
  int shield;
  int gold;
  std::vector<std::string> bag;
  int cost;
};

struct Account {
  int gold;
  std::vector<HeroSave> heroes;
  std::string language;
};

} // namespace game
