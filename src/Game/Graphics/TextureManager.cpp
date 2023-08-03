#include "TextureManager.h"
#include <iostream>

std::map<std::string, Texture*> TextureManager::textures;

Texture* TextureManager::LoadTexture(const std::string& fileName, SDL_Renderer* renderer, SDL_Window* window) {
    if (textures.count(fileName) > 0) {
        return textures[fileName];
    }
    
    Texture* tex = new Texture(renderer, window);
    tex->load(fileName);
    
    textures[fileName] = tex;    
    return tex;
}

void TextureManager::UnloadTexture(const std::string& fileName) {
    if (textures.count(fileName) > 0) {
        delete textures[fileName];
        textures.erase(fileName);
    }
}

Texture* TextureManager::GetTexture(const std::string& fileName) {
    if(textures.count(fileName) > 0) {
        return textures[fileName];
    }
    
    return nullptr;
}
