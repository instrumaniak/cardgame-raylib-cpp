// src/core/resource.cpp — ResourceManager implementation
// Precondition: InitAudioDevice() must be called before loadAll()
#include "core/resource.h"

#include <cassert>
#include <string>

#include "raylib.h"

namespace game {

static std::string keyFromPath(const std::string& path) {
  // "assets/img/cards.png" → "cards"
  std::string filename = GetFileNameWithoutExt(path.c_str());
  return filename;
}

static std::string extFromPath(const std::string& path) {
  // "assets/img/cards.png" → "png"
  const char* ext = GetFileExtension(path.c_str());
  return ext ? std::string(ext + 1) : "";
}

static bool hasExtension(const std::string& path,
                          const std::string& ext) {
  std::string fileExt = extFromPath(path);
  return fileExt == ext;
}

void ResourceManager::loadAll() {
  // Load textures (.png only)
  FilePathList imgFiles = LoadDirectoryFiles("assets/img");
  for (unsigned int i = 0; i < imgFiles.count; i++) {
    std::string path = imgFiles.paths[i];
    if (!hasExtension(path, "png")) continue;
    std::string key = keyFromPath(path);
    _textures[key] = LoadTexture(path.c_str());
  }
  UnloadDirectoryFiles(imgFiles);

  // Load sounds (.mp3 only)
  FilePathList sndFiles = LoadDirectoryFiles("assets/audio");
  for (unsigned int i = 0; i < sndFiles.count; i++) {
    std::string path = sndFiles.paths[i];
    if (!hasExtension(path, "mp3")) continue;
    std::string key = keyFromPath(path);
    _sounds[key] = LoadSound(path.c_str());
  }
  UnloadDirectoryFiles(sndFiles);

  // Load fonts (.ttf only)
  FilePathList fntFiles = LoadDirectoryFiles("assets/fonts");
  for (unsigned int i = 0; i < fntFiles.count; i++) {
    std::string path = fntFiles.paths[i];
    if (!hasExtension(path, "ttf")) continue;
    std::string key = keyFromPath(path);
    _fonts[key] = LoadFont(path.c_str());
  }
  UnloadDirectoryFiles(fntFiles);
}

void ResourceManager::unloadAll() {
  for (auto& [key, tex] : _textures) {
    UnloadTexture(tex);
  }
  _textures.clear();

  for (auto& [key, snd] : _sounds) {
    UnloadSound(snd);
  }
  _sounds.clear();

  for (auto& [key, fnt] : _fonts) {
    UnloadFont(fnt);
  }
  _fonts.clear();
}

const Texture2D& ResourceManager::tex(const std::string& key) const {
  auto it = _textures.find(key);
  assert(it != _textures.end() && "Texture key not found");
  return it->second;
}

const Sound& ResourceManager::snd(const std::string& key) const {
  auto it = _sounds.find(key);
  assert(it != _sounds.end() && "Sound key not found");
  return it->second;
}

const Font& ResourceManager::fnt(const std::string& key) const {
  auto it = _fonts.find(key);
  assert(it != _fonts.end() && "Font key not found");
  return it->second;
}

bool ResourceManager::hasTex(const std::string& key) const {
  return _textures.find(key) != _textures.end();
}

bool ResourceManager::hasSnd(const std::string& key) const {
  return _sounds.find(key) != _sounds.end();
}

bool ResourceManager::hasFnt(const std::string& key) const {
  return _fonts.find(key) != _fonts.end();
}

} // namespace game
