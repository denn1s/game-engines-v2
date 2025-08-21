#include "TextureManager.h"

std::map<std::string, Texture2D> TextureManager::textures;

Texture2D TextureManager::LoadTexture(const std::string& path) {
    if (textures.find(path) == textures.end()) {
        textures[path] = ::LoadTexture(path.c_str());
    }
    return textures[path];
}

void TextureManager::UnloadTexture(const std::string& path) {
    if (textures.find(path) != textures.end()) {
        ::UnloadTexture(textures[path]);
        textures.erase(path);
    }
}

Texture2D TextureManager::GetTexture(const std::string& path) {
    return textures[path];
}
