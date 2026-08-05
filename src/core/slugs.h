// src/core/slugs.h — Compile-time slug constants for all card and hero identifiers
#pragma once

#include <string_view>

namespace game::Cards {
// Items
inline constexpr std::string_view Apple = "apple";
inline constexpr std::string_view MonsterSkull = "monster-skull";
inline constexpr std::string_view RawMeat = "raw-meat";
inline constexpr std::string_view RottenMeat = "rotten-meat";
inline constexpr std::string_view ShinyShield = "shiny-shield";
inline constexpr std::string_view RustyShield = "rusty-shield";
inline constexpr std::string_view Omelette = "omelette";
inline constexpr std::string_view RingOfVitality = "ring-of-vitality";
inline constexpr std::string_view RingOfProtection = "ring-of-protection";
inline constexpr std::string_view RingOfCourage = "ring-of-courage";
inline constexpr std::string_view RingOfGreed = "ring-of-greed";
inline constexpr std::string_view CursedShield = "cursed-shield";
inline constexpr std::string_view Athelas = "athelas";
inline constexpr std::string_view Amethyst = "amethyst";
inline constexpr std::string_view InvisibilityCloak = "invisibility-cloak";
inline constexpr std::string_view DragonWing = "dragon-wing";
inline constexpr std::string_view Carrot = "carrot";
inline constexpr std::string_view Fossil = "fossil";
inline constexpr std::string_view Horn = "horn";
inline constexpr std::string_view Scales = "scales";
inline constexpr std::string_view Map = "map";
inline constexpr std::string_view Ruby = "ruby";
inline constexpr std::string_view Emerald = "emerald";
inline constexpr std::string_view MonsterEgg = "monster-egg";

// Shields
inline constexpr std::string_view WoodenShield = "wooden-shield";
inline constexpr std::string_view IronShield = "iron-shield";
inline constexpr std::string_view ReinforcedShield = "reinforced-shield";
inline constexpr std::string_view LegendaryShield = "legendary-shield";

// Heals
inline constexpr std::string_view MinorHealingPotion = "minor-healing-potion";
inline constexpr std::string_view HealingPotion = "healing-potion";
inline constexpr std::string_view MajorHealingPotion = "major-healing-potion";
inline constexpr std::string_view HolyHealingPotion = "holy-healing-potion";

// Gold
inline constexpr std::string_view OneCoin = "one-coin";
inline constexpr std::string_view TwoCoins = "two-coins";
inline constexpr std::string_view ThreeCoins = "three-coins";
inline constexpr std::string_view ManyCoins = "many-coins";
inline constexpr std::string_view LotOfCoins = "lot-of-coins";

// Place
inline constexpr std::string_view Chest = "chest";
inline constexpr std::string_view GoldenChest = "golden_chest";

// Special
inline constexpr std::string_view End = "end";
inline constexpr std::string_view Boss = "boss";
inline constexpr std::string_view Item = "item";

// Biomes
inline constexpr std::string_view Forest = "forest";
inline constexpr std::string_view Cave = "cave";
inline constexpr std::string_view EnchantedLands = "enchanted_lands";
inline constexpr std::string_view Desert = "desert";
inline constexpr std::string_view Abyss = "abyss";
inline constexpr std::string_view Underworld = "underworld";
} // namespace game::Cards

namespace game::Heroes {
inline constexpr std::string_view HeadlessKnight = "headless-knight";
inline constexpr std::string_view Thief = "thief";
inline constexpr std::string_view Harpy = "harpy";
inline constexpr std::string_view Warrior = "warrior";
inline constexpr std::string_view DarkKnight = "dark-knight";
inline constexpr std::string_view Mage = "mage";
inline constexpr std::string_view Spirit = "spirit";
inline constexpr std::string_view Anubis = "anubis";
} // namespace game::Heroes
