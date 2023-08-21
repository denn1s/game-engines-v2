#include <print.h>
#include <FastNoise.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
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

  Texture* waterTexture = TextureManager::LoadTexture("Tiles/Water.png", renderer);
  Texture* grassTexture = TextureManager::LoadTexture("Tiles/Grass.png", renderer);

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
      tile.up.texture->render(
          x * size * scale,
          y * size * scale,
          size * scale,
          size * scale
      );
    }
  }
}
/*
// run method for the AutoTilingUpdateSystem class
void AutoTilingSetupSystem::run() {
    // Get the TilemapComponent from the scene
    const auto& tilemapComponent = scene->world->get<TilemapComponent>();

    // Define the delta x and y for each of the eight directions (N, NE, E, SE, S, SW, W, NW)
    const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    const int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    // Loop through each tile in the tilemap
    for (int y = 0; y < tilemapComponent.height; y++) {
        for (int x = 0; x < tilemapComponent.width; x++) {

            // Calculate the index of the current tile in the tileEntities vector
            int index = y * tilemap.width + x;

            // Get the Entity and AutoTilingInfo of the current tile
            auto& entity = tilemap.tileEntities[index];
            auto& info = entity.get<AutoTilingInfo>();

            // Reset the surrounding field of the AutoTilingInfo
            info.surrounding = 0;

            // Loop through each of the eight directions
            for (int i = 0; i < 8; i++) {

                // Calculate the coordinates of the neighboring tile
                int nx = x + dx[i];
                int ny = y + dy[i];

                // Check if the coordinates are out of bounds
                if (nx < 0 || nx >= tilemap.width || ny < 0 || ny >= tilemap.height) {
                    continue;  // If out of bounds, skip this iteration
                }

                // Calculate the index of the neighboring tile in the tileEntities vector
                int neighborIndex = ny * tilemap.width + nx;

                // Get the Tile of the neighboring tile
                auto& neighborTile = tilemap.tileEntities[neighborIndex].get<Tile>();

                // Check if the neighboring tile is the same type as the current tile
                if (neighborTile.isWater == entity.get<Tile>().isWater) {

                    // If the neighboring tile is the same type, set the corresponding bit in the surrounding field
                    info.surrounding |= (1 << i);
                }
            }

            // TODO: update the entity's tile based on info.surrounding
        }
    }
}
*/
