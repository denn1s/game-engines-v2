#include "Texture.h"
#include <map>
#include <string>

class TextureManager {
  public:
    static Texture* LoadTexture(const std::string& fileName, SDL_Renderer* renderer, SDL_Window* window);
    static void UnloadTexture(const std::string& fileName);
    static Texture* GetTexture(const std::string& fileName);
  private:
    static std::map<std::string, Texture*> textures;
};