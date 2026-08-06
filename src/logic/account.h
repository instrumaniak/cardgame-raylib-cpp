// src/logic/account.h — Account save/load, hero unlock, gold economy
// Pure game logic layer: no raylib includes
#pragma once

#include "core/types.h"

#include <string>

namespace game::logic {

// Create a default account with 4 heroes from the original game
Account createDefaultAccount();

// Serialize account to JSON string
std::string saveAccount(const Account& account);

// Deserialize account from JSON string
// Returns empty Account on parse failure
Account loadAccount(const std::string& json);

// Unlock a hero: deducts gold and sets locked=false
// Returns true on success, false if hero already unlocked or insufficient gold
bool unlockHero(HeroSave& hero, Account& account);

// Spend gold from account
// Returns true on success, false if insufficient gold
bool spendGold(Account& account, int amount);

// Add gold to account
void addGold(Account& account, int amount);

} // namespace game::logic
