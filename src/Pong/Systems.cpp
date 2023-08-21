#include <cstdint>
#include <print.h>
#include <FastNoise.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>
#include "Systems.h"
#include "Components.h"

#include "ECS/Entity.h"
#include "ECS/Components.h"
#include "Game/Graphics/TextureManager.h"

SpriteSetupSystem::SpriteSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) { }

SpriteSetupSystem::~SpriteSetupSystem() {
    auto view = scene->r.view<SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        TextureManager::UnloadTexture(spriteComponent.name, spriteComponent.shader.name);
    }
}

void SpriteSetupSystem::run() {
    auto view = scene->r.view<SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        TextureManager::LoadTexture(spriteComponent.name, renderer, spriteComponent.shader);
    }
}

void SpriteRenderSystem::run(SDL_Renderer* renderer) {
    auto view = scene->r.view<TransformComponent, SpriteComponent>();

    for(auto entity : view) {
        const auto spriteComponent = view.get<SpriteComponent>(entity);
        const auto transformComponent = view.get<TransformComponent>(entity);
  
        Texture* texture = TextureManager::GetTexture(spriteComponent.name, spriteComponent.shader.name);
  
        SDL_Rect clip = {
            spriteComponent.xIndex * spriteComponent.size,
            spriteComponent.yIndex * spriteComponent.size,
            spriteComponent.size,
            spriteComponent.size
        };

        int scale = 5;

        texture->render(
            transformComponent.position.x * scale,
            transformComponent.position.y * scale,
            48 * scale,
            48 * scale,
            &clip
        );
    }
}

void SpriteUpdateSystem::run(double dT) {
    auto view = scene->r.view<SpriteComponent>();

    Uint32 now = SDL_GetTicks();

    for(auto entity : view) {
        auto& spriteComponent = view.get<SpriteComponent>(entity);

        if (spriteComponent.animationFrames > 0) {
            float timeSinceLastUpdate = now - spriteComponent.lastUpdate;

            int framesToUpdate = static_cast<int>(
                timeSinceLastUpdate / 
                spriteComponent.animationDuration * spriteComponent.animationFrames
            );

            if (framesToUpdate > 0) {
                spriteComponent.xIndex += framesToUpdate;
                spriteComponent.xIndex %= spriteComponent.animationFrames;
                spriteComponent.lastUpdate = now;            
            }
        }
    }
}

TilemapSetupSystem::TilemapSetupSystem(SDL_Renderer* renderer)
    : renderer(renderer) { }

TilemapSetupSystem::~TilemapSetupSystem() {
}

void TilemapSetupSystem::run() {
  auto& tilemapComponent = scene->world->get<TilemapComponent>();
  tilemapComponent.width = 50;
  tilemapComponent.height = 38;
  tilemapComponent.tileSize = 16;
  tilemapComponent.tilemap.resize(tilemapComponent.width * tilemapComponent.height);

  Texture* waterTexture = TextureManager::LoadTexture("Tilesets/Water.png", renderer);
  Texture* grassTexture = TextureManager::LoadTexture("Tilesets/Grass.png", renderer);

  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

  std::srand(std::time(nullptr));
  float offsetX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  float offsetY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  float zoom = 20.0f;

  Terrain grass{grassTexture};
  Terrain water{waterTexture};  

  for (int y = 0; y < tilemapComponent.height; y++) {
      for (int x = 0; x < tilemapComponent.width; x++) {
          float factor = noise.GetNoise(
              static_cast<float>(x + offsetX) * zoom, 
              static_cast<float>(y + offsetY) * zoom
          );

          int index = y * tilemapComponent.width + x;
          Tile& tile = tilemapComponent.tilemap[index];

          if (factor < 0.5) {
            tile.up = grass;
            tile.down = water;
            tile.needsAutoTiling = true;
          } else{
            tile.up = water;
            tile.needsAutoTiling = false;
          }
        }
    }
}

void TilemapRenderSystem::run(SDL_Renderer* renderer) {
  auto& tilemapComponent = scene->world->get<TilemapComponent>();
  int width = tilemapComponent.width;
  int height = tilemapComponent.height;
  int size = tilemapComponent.tileSize;
  int scale = 5;

  for (int y = 0; y < height; y++) {
    for (int x = 0; x < width; x++) {
      Tile& tile = tilemapComponent.tilemap[y * width + x];
      if (tile.down.texture) {
        SDL_Rect downClip = {
          tile.down.x,
          tile.down.y,
          size,
          size
        };

        tile.down.texture->render(
          x * size * scale,
          y * size * scale,
          size * scale,
          size * scale,
          &downClip
        );
      }

      SDL_Rect upClip = {
        tile.up.x,
        tile.up.y,
        size,
        size
      };

      tile.up.texture->render(
        x * size * scale,
        y * size * scale,
        size * scale,
        size * scale,
        &upClip
      );
    }
  }
}

std::map<u_int8_t, std::pair<int, int>> m = {
  {  0, {16, 32} },
  {  1, { 0, 80} },
  {  2, {48, 96} },
  {  3, {48, 80} },
  {  4, { 0, 96} },
  {  5, {16, 80} },
  {  6, {16, 96} },
  {  7, {32, 80} },
  {  8, { 0, 32} },
  {  9, { 0, 48} },
  { 10, {48, 48} },
  { 11, {48, 64} },
  { 12, {16, 48} },
  { 13, {16, 64} },
  { 14, {32, 48} },
  { 15, { 0,  0} },
};

// run method for the AutoTilingUpdateSystem class
void AutoTilingSetupSystem::run() {
  auto& tilemapComponent = scene->world->get<TilemapComponent>();
  int width = tilemapComponent.width;
  int height = tilemapComponent.height;
  int size = tilemapComponent.tileSize;

  // Define the delta x and y for each of the eight directions (N, NE, E, SE, S, SW, W, NW)
  // const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
  // const int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };
  const int dx[4] = { 0, -1, 1, 0 };
  const int dy[4] = { -1, 0, 0, 1 };

  // Loop through each tile in the tilemap
  for (int y = 0; y < tilemapComponent.height; y++) {
    for (int x = 0; x < tilemapComponent.width; x++) {
      int index = y * width + x;
      Tile& tile = tilemapComponent.tilemap[index];

      if (!tile.needsAutoTiling)
        continue;

      uint8_t surrounding = 0;
      // Loop through each of the eight directions
      for (int i = 0; i < 4; i++) {

        // Calculate the coordinates of the neighboring tile
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Check if the coordinates are out of bounds
        if (nx < 0 || nx >= width || ny < 0 || ny >= height) 
          continue;  // If out of bounds, skip this iteration

          // Calculate the index of the neighboring tile in the tileEntities vector
        int neighborIndex = ny * width + nx;


        // Get the Tile of the neighboring tile
        const Tile& neighborTile = tilemapComponent.tilemap[neighborIndex];
        
        if (tile.up.texture == neighborTile.up.texture) { 
          /* The line surrounding |= 1 << i; is using bit manipulation to set a specific bit in the surrounding variable to 1. Let's break it down piece by piece:
          1 << i: This is a bit shift operation. It takes the binary number 1 (which is 00000001 in 8 bits) and shifts it to the left i times. For example, if i is 2, 1 << 2 would be 00000100. This gives us a binary number where only the i-th bit is set to 1.
          surrounding |= ...: This is a bitwise OR assignment. It takes the current value of surrounding, performs a bitwise OR with the value on the right-hand side, and assigns the result back to surrounding. In a bitwise OR, each bit in the result is 1 if at least one of the corresponding bits in the operands is 1. So, surrounding |= 1 << i; has the effect of setting the i-th bit of surrounding to 1, while leaving all other bits unchanged.
          */
          surrounding |= 1 << i;          
        }
      }

      tile.up.x = m[surrounding].first;
      tile.up.y = m[surrounding].second;

    }
  }
}
