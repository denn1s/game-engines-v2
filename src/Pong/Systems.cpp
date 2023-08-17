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
  auto& tilemap = scene->world->get<TilemapComponent>();
  tilemap.width = 50;
  tilemap.height = 38;
  tilemap.tileSize = 16;

  Texture* waterTexture = TextureManager::LoadTexture("Tiles/Water.png", renderer);
  Texture* grassTexture = TextureManager::LoadTexture("Tiles/Grass.png", renderer);

  FastNoiseLite noise;
  noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);

  std::srand(std::time(nullptr));
  float offsetX = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  float offsetY = static_cast<float>(rand()) / static_cast<float>(RAND_MAX);
  float zoom = 100.0f;

  for (int y = 0; y < tilemap.height; y++) {
      for (int x = 0; x < tilemap.width; x++) {
          float factor = noise.GetNoise(
              static_cast<float>(x + offsetX) * zoom, 
              static_cast<float>(y + offsetY) * zoom
          );

          Entity tile = scene->createEntity(
            "TILE",
            x * tilemap.tileSize,
            y * tilemap.tileSize
          );

          print("Tile");
          print(x * tilemap.tileSize);
          print(y * tilemap.tileSize);
          print("---");

          auto& tileComponent = tile.get<TileComponent>();
          if (factor < 0.5) {
              tileComponent.texture = grassTexture;
              tileComponent.isWater = false;
          } else {
              tileComponent.texture = waterTexture;
              tileComponent.isWater = true;
          }

          tile.addComponent<AutoTilingInfo>();
        }
    }
}

void TilemapRenderSystem::run(SDL_Renderer* renderer) {
  auto view = scene->r.view<TileComponent, TransformComponent>();

  int size = 16;

  for (auto e : view) {
    const auto transform = view.get<TransformComponent>(e);
    const auto tile = view.get<TileComponent>(e);
    const auto pos = transform.position;

    tile.texture->render(
      pos.x * size,
      pos.y * size,
      16,
      16
    );
  }    
}

// Include the necessary header files
#include "AutoTilingUpdateSystem.h"

// Define the structure of a Tile
struct Tile {
    Texture* texture;
    bool isWater;
};

// Define the structure of AutoTilingInfo
struct AutoTilingInfo {
    bool needsUpdate;
    uint8_t surrounding;  // Bitfield of surrounding tiles (N, NE, E, SE, S, SW, W, NW)
};

// Constructor for the AutoTilingUpdateSystem class
AutoTilingUpdateSystem::AutoTilingUpdateSystem(Scene* scene)
    : scene(scene) { }

// run method for the AutoTilingUpdateSystem class
void AutoTilingUpdateSystem::run() {

    // Get the TilemapComponent from the scene
    auto& tilemap = scene->r.ctx<TilemapComponent>();

    // Define the delta x and y for each of the eight directions (N, NE, E, SE, S, SW, W, NW)
    const int dx[8] = { 0, 1, 1, 1, 0, -1, -1, -1 };
    const int dy[8] = { -1, -1, 0, 1, 1, 1, 0, -1 };

    // Loop through each tile in the tilemap
    for (int y = 0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {

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

