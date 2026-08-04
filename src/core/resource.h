// src/core/resource.h — Flyweight resource cache for textures, sounds, and fonts
#pragma once

#include "raylib.h"

#include <string>
#include <unordered_map>

namespace game {

class ResourceManager {
  std::unordered_map<std::string, Texture2D> _textures;
  std::unordered_map<std::string, Sound> _sounds;
  std::unordered_map<std::string, Font> _fonts;

public:
  void loadAll();
  void unloadAll();

  const Texture2D& tex(const std::string& key) const;
  const Sound& snd(const std::string& key) const;
  const Font& fnt(const std::string& key) const;

  bool hasTex(const std::string& key) const;
  bool hasSnd(const std::string& key) const;
  bool hasFnt(const std::string& key) const;
};

} // namespace game
