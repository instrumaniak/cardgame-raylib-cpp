// src/core/json_keys.h — JSON serialization field name constants
#pragma once

#include <string_view>

namespace game::JsonKeys {
inline constexpr std::string_view Slug = "slug";
inline constexpr std::string_view Locked = "locked";
inline constexpr std::string_view Life = "life";
inline constexpr std::string_view LifeMax = "life_max";
inline constexpr std::string_view Shield = "shield";
inline constexpr std::string_view Gold = "gold";
inline constexpr std::string_view Bag = "bag";
inline constexpr std::string_view Cost = "cost";
inline constexpr std::string_view Config = "config";
inline constexpr std::string_view Lang = "lang";
inline constexpr std::string_view Collection = "collection";
inline constexpr std::string_view Heroes = "heroes";
inline constexpr std::string_view Cards = "cards";
} // namespace game::JsonKeys
