#pragma once

#include <raylib.h>
#include <string>
#include <map>

class TextureManager {
public:
    static Texture2D LoadTexture(const std::string& path);
    static void UnloadTexture(const std::string& path);
    static Texture2D GetTexture(const std::string& path);

private:
    static std::map<std::string, Texture2D> textures;
};
