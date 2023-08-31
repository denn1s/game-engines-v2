#include <SDL_render.h>
#include <cstdint>
#include <print.h>
#include <FastNoise.h>
#include <SDL2/SDL.h>
#include <ctime>
#include <cstdlib>
#include <sys/types.h>
#include <bitset>
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
    const auto& c = scene->mainCamera->get<CameraComponent>();
    const auto& cameraTransform = scene->mainCamera->get<TransformComponent>();
    int cx = cameraTransform.x;
    int cy = cameraTransform.y;

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

        texture->render(
            transformComponent.x - cx,
            transformComponent.y - cy,
            spriteComponent.size * c.zoom,
            spriteComponent.size * c.zoom,
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
    const auto player = scene->player->get<TransformComponent>();
    const auto cameraZoom = scene->mainCamera->get<CameraComponent>().zoom;
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

    int centerX = player.x / cameraZoom / tilemapComponent.tileSize;
    int centerY = player.y / cameraZoom / tilemapComponent.tileSize;

    for (int y = 0; y < tilemapComponent.height; y++) {
        for (int x = 0; x < tilemapComponent.width; x++) {
            int index = y * tilemapComponent.width + x;
            Tile& tile = tilemapComponent.tilemap[index];
            if (std::abs(centerX - x) < 5  && std::abs(centerY - y) < 5) {
                tile.up = grass;
                tile.down = water;
                tile.needsAutoTiling = true;
            } else {
                float factor = noise.GetNoise(
                    static_cast<float>(x + offsetX) * zoom, 
                    static_cast<float>(y + offsetY) * zoom
                );

                if (factor < 0.5) {
                    tile.up = grass;
                    tile.down = water;
                    tile.needsAutoTiling = true;
                } else {
                    tile.up = water;
                    tile.needsAutoTiling = false;
                    tile.isWalkable = false;
                }
            }
        } 
    }
}

void TilemapRenderSystem::run(SDL_Renderer* renderer) {
  auto& tilemapComponent = scene->world->get<TilemapComponent>();
  int width = tilemapComponent.width;
  int height = tilemapComponent.height;
  int size = tilemapComponent.tileSize;
  const auto& c = scene->mainCamera->get<CameraComponent>();
  const auto& cameraTransform = scene->mainCamera->get<TransformComponent>();
  int cx = cameraTransform.x;
  int cy = cameraTransform.y;

  // Calculate the range of tiles that should be visible
  int startX = std::max(0, cx / (size * c.zoom));
  int startY = std::max(0, cy / (size * c.zoom));
  int endX = std::min(width, (cx + c.vw) / (size * c.zoom) + 1);
  int endY = std::min(height, (cy + c.vh) / (size * c.zoom) + 1);


  for (int y = startY; y < endY; y++) {
    for (int x = startX; x < endX; x++) {
      Tile& tile = tilemapComponent.tilemap[y * width + x];
      if (tile.down.texture) {
        SDL_Rect downClip = {
          tile.down.x,
          tile.down.y,
          size,
          size
        };

        tile.down.texture->render(
          x * size * c.zoom - cx,
          y * size * c.zoom - cy,
          size * c.zoom,
          size * c.zoom,
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
        x * size * c.zoom - cx,
        y * size * c.zoom - cy,
        size * c.zoom,
        size * c.zoom,
        &upClip
      );
    }
  }
}



std::map<u_int8_t, std::vector<std::pair<int, int>>> m = {
    {  2, {{   0,  80 }} },
    {  8, {{  48,  96 }} },
    { 10, {{  80, 112 }} },
    { 11, {{  48,  80 }} },
    { 16, {{   0,  96 }} },
    { 18, {{  64, 112 }} },
    { 22, {{  16,  80 }} },
    { 24, {{  16,  96 }, {  32,  96 }} },
    { 26, {{ 144,  32 }} },
    { 27, {{ 144,  80 }} },
    { 30, {{  96,  80 }} },
    { 31, {{  32,  80 }} },
    { 64, {{   0,  32 }} },
    { 66, {{   0,  48 }, {   0,  64 }} },
    { 72, {{  80,  96 }} },
    { 74, {{ 128,  32 }} },
    { 75, {{ 112,  80 }} },
    { 80, {{  64,  96 }} },
    { 82, {{ 144,  48 }} },
    { 86, {{ 128,  80 }} },
    { 88, {{ 128,  48 }} },
    { 90, {{   0, 112 }, {  16, 112 }} },
    { 91, {{  32, 112 }} },
    { 94, {{  96,  48 }} },
    { 95, {{  96, 112 }} },
    {104, {{  48,  48 }} },
    {106, {{ 144,  64 }} },
    {107, {{  48,  64 }} },
    {120, {{ 112,  64 }} },
    {122, {{  48, 112 }} },
    {123, {{ 112, 112 }} },
    {126, {{  48, 112 }} },
    {127, {{  64,  64 }} },
    {208, {{  16,  48 }} },
    {210, {{  96,  64 }} },
    {214, {{  16,  64 }} },
    {216, {{ 128,  64 }} },
    {218, {{  96,  32 }} },
    {219, {{  32, 112 }} },
    {222, {{  96,  96 }} },
    {223, {{  80,  64 }} },
    {248, {{  32,  48 }} },
    {250, {{ 112,  96 }} },
    {251, {{  64,  80 }} },
    {254, {{  80,  80 }} },
    {255, {
            {   0,   0 }, {  16,   0 }, {  32,   0 }, {  48,   0 }, {  64,   0 }, {  80,   0 },
            {   0,  16 }, {  16,  16 }, {  32,  16 }, {  48,  16 }, {  64,  16 }, {  80,  16 },
            {  32,  64 }
          }
    },
    {  0, {
            { 16,  32 }, {  32,  32 }, {  48,  32 },
            { 64,  32 }, {  80,  32 }, {  64,  48 }, {  80,  48 }
          }
    }
};


const int dx[8] = { -1,  0,  1, -1, 1, -1, 0, 1 }; 
const int dy[8] = { -1, -1, -1,  0, 0,  1, 1, 1 };

std::map<u_int8_t, std::pair<int, int>> d_corner = {
  {0, {  1,  1 }},
  {2, { -1,  1 }},
  {5, {  1, -1 }},
  {7, { -1, -1 }},
};

// run method for the AutoTilingUpdateSystem class
void AutoTilingSetupSystem::run() {
  auto& tilemapComponent = scene->world->get<TilemapComponent>();
  int width = tilemapComponent.width;
  int height = tilemapComponent.height;
  int size = tilemapComponent.tileSize;
  // Loop through each tile in the tilemap
  for (int y = 0; y < tilemapComponent.height; y++) {
    for (int x = 0; x < tilemapComponent.width; x++) {
      int index = y * width + x;
      Tile& tile = tilemapComponent.tilemap[index];

      if (!tile.needsAutoTiling)
        continue;


      uint8_t surrounding = 0;
      // Loop through each of the eight directions
      for (int i = 0; i < 8; i++) {
        // Calculate the coordinates of the neighboring tile
        int nx = x + dx[i];
        int ny = y + dy[i];

        // Check if the coordinates are out of bounds
        if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
          continue;  // If out of bounds, skip this iteration
        }

      // For corner tiles, check the two adjacent cardinal directions
      if (i == 0 || i == 2 || i == 5 || i == 7) {  // i is 0, 2, 5, 7 for corner tiles
          int nx1 = nx + d_corner[i].first; 
          int ny1 = ny + 0;  // This is for legibility
          int nx2 = nx + 0;
          int ny2 = ny + d_corner[i].second;

          // If either of the cardinal tiles are missing, skip this iteration
          if (nx1 < 0 || nx1 >= width || ny1 < 0 || ny1 >= height || 
              nx2 < 0 || nx2 >= width || ny2 < 0 || ny2 >= height) {
              continue;
          }

          if (tilemapComponent.tilemap[ny1 * width + nx1].up.texture != tilemapComponent.tilemap[ny * width + nx].up.texture || 
              tilemapComponent.tilemap[ny2 * width + nx2].up.texture != tilemapComponent.tilemap[ny * width + nx].up.texture)
          {
              continue;
          }
      }

        int neighborIndex = ny * width + nx;
        // Get the Tile of the neighboring tile
        const Tile& neighborTile = tilemapComponent.tilemap[neighborIndex];
        if (tile.up.texture == neighborTile.up.texture) {
            surrounding |= 1 << i;          
        } 
      }
      auto iter = m.find(surrounding);
      
      if (iter == m.end()) {
          tile.up.x = 0;
          tile.up.y = 0;
          print("Tile not found", static_cast<int>(surrounding));
          exit(1);
      } else {
        auto& pairs = iter->second;
        
        if (pairs.size() == 1) {
          tile.up.x = pairs[0].first;
          tile.up.y = pairs[0].second;
        } else {
          int index = rand() % pairs.size();
          tile.up.x = pairs[index].first;
          tile.up.y = pairs[index].second;
        }
      }
    }
  }
}

void PlayerInputEventSystem::run(SDL_Event event) {
    auto& playerMovement = scene->player->get<SpeedComponent>();
    const auto cameraZoom = scene->mainCamera->get<CameraComponent>().zoom;
    int speed = 200;

    if (event.type == SDL_KEYDOWN) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
                playerMovement.x = -speed;
                break;
            case SDLK_RIGHT:
                playerMovement.x = speed;
                break;
            case SDLK_UP:
                playerMovement.y = -speed;
                break;
            case SDLK_DOWN:
                playerMovement.y = speed;
                break;
        }
   }
    else if (event.type == SDL_KEYUP) {
        switch (event.key.keysym.sym) {
            case SDLK_LEFT:
            case SDLK_RIGHT:
                playerMovement.x = 0;
                break;
            case SDLK_UP:
            case SDLK_DOWN:
                playerMovement.y = 0;
                break;
        }
    }
}

void PlayerSpriteUpdateSystem::run(double dT) {
    auto& playerMovement = scene->player->get<SpeedComponent>();
    auto& playerSprite = scene->player->get<SpriteComponent>();

    if (playerMovement.x < 0) {
        playerSprite.yIndex = 7;
    }
    else if (playerMovement.x > 0) {
        playerSprite.yIndex = 6;
    }
    else if (playerMovement.y < 0) {
        playerSprite.yIndex = 5;
    }
    else if (playerMovement.y > 0) {
        playerSprite.yIndex = 4;
    }
    else {
        if (playerSprite.yIndex == 7) {
            playerSprite.yIndex = 2;
        }
        else if (playerSprite.yIndex == 6) {
            playerSprite.yIndex = 3;
        }
        else if (playerSprite.yIndex == 5) {
            playerSprite.yIndex = 1;
        }
        else if (playerSprite.yIndex == 4) {
            playerSprite.yIndex = 0;
        }
    }
}

void MovementUpdateSystem::run(double dT) {
  const auto view = scene->r.view<TransformComponent, SpeedComponent>();
  for (const entt::entity e : view) {
    auto& pos = view.get<TransformComponent>(e);
    const auto vel = view.get<SpeedComponent>(e);

    pos.x += vel.x * dT;
    pos.y += vel.y * dT;
  }
}

void CameraFollowUpdateSystem::run(double dT) {
    const int spriteSize = 48;
    auto playerTransform = scene->player->get<TransformComponent>();
    auto cameraComponent = scene->mainCamera->get<CameraComponent>();
    auto& cameraTransform = scene->mainCamera->get<TransformComponent>();
    auto worldComponent = scene->world->get<WorldComponent>();

    int px = playerTransform.x - cameraComponent.vw / 2 + (spriteSize / 2) * cameraComponent.zoom;
    int py = playerTransform.y - cameraComponent.vh / 2 + (spriteSize / 2) * cameraComponent.zoom;

    if (px > 0 && px < worldComponent.width - cameraComponent.vw) {
        cameraTransform.x = px;
    }

    if (py > 0 && py < worldComponent.height - cameraComponent.vh) {
        cameraTransform.y = py;
    }
}

void BoxColliderRenderSystem::run(SDL_Renderer* r) {
    const auto cameraZoom = scene->mainCamera->get<CameraComponent>().zoom;
    const auto cameraTransform = scene->mainCamera->get<TransformComponent>();
    const int cx = cameraTransform.x;
    const int cy = cameraTransform.y;

    const auto view = scene->r.view<TransformComponent, BoxColliderComponent>();

    for (const entt::entity e : view) {
        const auto box = view.get<BoxColliderComponent>(e);
        const auto t = view.get<TransformComponent>(e);
        const int xo = box.xo * cameraZoom;
        const int yo = box.yo * cameraZoom;

        SDL_SetRenderDrawColor(r, box.color.r, box.color.g, box.color.b, 255);

        SDL_Rect rect = {
            static_cast<int>(t.x - cx + xo), 
            static_cast<int>(t.y - cy + yo),
            box.w * cameraZoom,
            box.h * cameraZoom
        };

        SDL_RenderDrawRect(r, &rect);
    }
}

void TileColliderRenderSystem::run(SDL_Renderer* r) {
    auto& tilemapComponent = scene->world->get<TilemapComponent>();
    int width = tilemapComponent.width;
    int height = tilemapComponent.height;
    int size = tilemapComponent.tileSize;
    const auto& c = scene->mainCamera->get<CameraComponent>();
    const auto& cameraTransform = scene->mainCamera->get<TransformComponent>();
    int cx = cameraTransform.x;
    int cy = cameraTransform.y;

    // Calculate the range of tiles that should be visible
    int startX = std::max(0, cx / (size * c.zoom));
    int startY = std::max(0, cy / (size * c.zoom));
    int endX = std::min(width, (cx + c.vw) / (size * c.zoom) + 1);
    int endY = std::min(height, (cy + c.vh) / (size * c.zoom) + 1);

    for (int y = startY; y < endY; y++) {
        for (int x = startX; x < endX; x++) {
            Tile& tile = tilemapComponent.tilemap[y * width + x];

            SDL_Rect rect = {
                x * size * c.zoom - cx,
                y * size * c.zoom - cy,
                size * c.zoom,
                size * c.zoom
            };

            if (tile.isWalkable) {
                SDL_SetRenderDrawColor(r, 0, 0, 255, 255);
                SDL_RenderDrawRect(r, &rect);
            } else {
                SDL_SetRenderDrawColor(r, 0, 0, 255, 55);
                SDL_RenderFillRect(r, &rect);
            }
        }
    }
}

void TileCollisionUpdateSystem::run(double dT) {
    const auto playerPosition = scene->player->get<TransformComponent>();
    const auto playerCollider = scene->player->get<BoxColliderComponent>();
    const auto world = scene->world->get<WorldComponent>();
    const auto cameraZoom = scene->mainCamera->get<CameraComponent>().zoom;
    auto& playerMovement = scene->player->get<SpeedComponent>();
    const auto tilemapComponent = scene->world->get<TilemapComponent>();

    if (playerMovement.x == 0 && playerMovement.y == 0) {
        return;
    }
    const int colliderPositionX = playerPosition.x + playerCollider.xo * cameraZoom;
    const int colliderPositionY = playerPosition.y + playerCollider.yo * cameraZoom;
    const int colliderSize = playerCollider.w * cameraZoom; // squared
    const int tileSize = tilemapComponent.tileSize * cameraZoom;


    // Calculate future position based on current position and speed.
    int futureX = colliderPositionX + playerMovement.x * dT;
    int futureY = colliderPositionY + playerMovement.y * dT;
    int futureRightX = futureX + colliderSize;
    int futureBottomY = futureY + colliderSize;

    if (futureX <= 0 || futureY <= 0 || futureRightX >= world.width || futureBottomY >= world.height) {
        playerMovement.x = 0;
        playerMovement.y = 0;
        return;
    }

    // Convert the future positions from pixels to tile coordinates.
    std::vector<std::pair<int, int>> futureTiles = {
        {futureX / tileSize, futureY / tileSize}, // top left corner
        {futureRightX / tileSize, futureY / tileSize}, // top right corner
        {futureX / tileSize, futureBottomY / tileSize}, // bottom left corner
        {futureRightX / tileSize, futureBottomY / tileSize}  // bottom right corner
    };

    for (const auto& [tileX, tileY] : futureTiles) {
        // Get the tile at the future position.
        const Tile& tile = tilemapComponent.tilemap[tileY * tilemapComponent.width + tileX];

        // If the tile is not walkable, set the speed to 0.
        if (!tile.isWalkable) {
            playerMovement.x = 0;
            playerMovement.y = 0;
            return;
        }
    }
}

