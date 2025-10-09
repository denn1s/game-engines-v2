#include "Systems.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include "Game/Scene/Scene.h"
#include "ECS/Entity.h"
#include "Game/Graphics/TextureManager.h"
#include "tilemap.h"
#include <print>
#include <raylib.h>
#include <vector>
#include <map>
#include <sol/sol.hpp>
#include "FastNoiseLite.h"

const float WATER_LEVEL = 0.2f;

void TilemapSetupSystem::setup() {
    Entity tilemapEntity = scene->createEntity("tilemap");
    auto& tilemap = tilemapEntity.addComponent<TilemapComponent>();
    tilemap.width = TILEMAP_WIDTH;
    tilemap.height = TILEMAP_HEIGHT;
    tilemap.tileSize = 16;

    auto& intGrid = tilemapEntity.addComponent<IntGridComponent>();
    intGrid.width = TILEMAP_WIDTH;
    intGrid.height = TILEMAP_HEIGHT;
    intGrid.grid.resize(TILEMAP_WIDTH * TILEMAP_HEIGHT);

    Texture2D waterTexture = TextureManager::LoadTexture("assets/Tilesets/Water.png");
    Texture2D grassTexture = TextureManager::LoadTexture("assets/Tilesets/Grass.png");

    FastNoiseLite noise;
    noise.SetNoiseType(FastNoiseLite::NoiseType_OpenSimplex2);
    noise.SetFrequency(0.05f);

    for (int y = 0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {
            TileComponent tile;
            tile.x = x;
            tile.y = y;
            
            float noiseValue = noise.GetNoise((float)x, (float)y);

            if (noiseValue > WATER_LEVEL) {
                tile.upTexture = grassTexture;
                tile.downTexture = waterTexture;
                tile.needsAutoTiling = true;
                intGrid.grid[y * tilemap.width + x] = 1; // Grass
            } else {
                tile.upTexture = waterTexture;
                intGrid.grid[y * tilemap.width + x] = 0; // Water
            }

            tilemap.tiles.push_back(tile);
        }
    }

    intGrid.grid[0 * tilemap.width + 0] = 2; // Treasure
    intGrid.grid[0 * tilemap.width + 1] = 3; // Teleport
}

void TilemapRenderSystem::render() {
    auto view = scene->r.view<TilemapComponent>();
    auto& cameraTransform = scene->camera->get<TransformComponent>();
    auto& cameraComponent = scene->camera->get<CameraComponent>();
    float cam_vw = (float)cameraComponent.vw;
    float cam_vh = (float)cameraComponent.vh;
    float zoom = cameraComponent.zoom;

    for (auto entity : view) {
        auto& tilemap = view.get<TilemapComponent>(entity);
        for (auto& tile : tilemap.tiles) {
            float worldX = (float)tile.x * tilemap.tileSize * tile.scale;
            float worldY = (float)tile.y * tilemap.tileSize * tile.scale;
            float worldW = (float)tilemap.tileSize * tile.scale;
            float worldH = (float)tilemap.tileSize * tile.scale;

            float screenX = worldX - cameraTransform.position.x;
            float screenY = worldY - cameraTransform.position.y;

            float newScreenX = cam_vw / 2.0f + (screenX - cam_vw / 2.0f) * zoom;
            float newScreenY = cam_vh / 2.0f + (screenY - cam_vh / 2.0f) * zoom;

            if (tile.downTexture.id > 0) {
                DrawTextureEx(tile.downTexture, Vector2{newScreenX, newScreenY}, 0, tile.scale * zoom, WHITE);
            }
            Rectangle sourceRec = {
                (float)tile.tileX,
                (float)tile.tileY,
                (float)tilemap.tileSize,
                (float)tilemap.tileSize
            };
            Rectangle destRec = {
                newScreenX,
                newScreenY,
                worldW * zoom,
                worldH * zoom
            };
            DrawTexturePro(tile.upTexture, sourceRec, destRec, {0, 0}, 0, WHITE);
        }
    }
}

std::map<uint8_t, std::vector<std::pair<int, int>>> m = {
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

std::map<uint8_t, std::pair<int, int>> d_corner = {
  {0, {  1,  1 }},
  {2, { -1,  1 }},
  {5, {  1, -1 }},
  {7, { -1, -1 }},
};

void AutoTilingSetupSystem::setup() {
    auto view = scene->r.view<TilemapComponent>();
    for (auto entity : view) {
        auto& tilemap = view.get<TilemapComponent>(entity);
        int width = tilemap.width;
        int height = tilemap.height;

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                TileComponent& tile = tilemap.tiles[index];

                if (!tile.needsAutoTiling)
                    continue;

                uint8_t surrounding = 0;
                for (int i = 0; i < 8; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) {
                        continue;
                    }

                    if (i == 0 || i == 2 || i == 5 || i == 7) {
                        int nx1 = nx + d_corner[i].first; 
                        int ny1 = ny + 0;
                        int nx2 = nx + 0;
                        int ny2 = ny + d_corner[i].second;

                        if (nx1 < 0 || nx1 >= width || ny1 < 0 || ny1 >= height || 
                            nx2 < 0 || nx2 >= width || ny2 < 0 || ny2 >= height) {
                            continue;
                        }

                        if (tilemap.tiles[ny1 * width + nx1].upTexture.id != tilemap.tiles[ny * width + nx].upTexture.id || 
                            tilemap.tiles[ny2 * width + nx2].upTexture.id != tilemap.tiles[ny * width + nx].upTexture.id)
                        {
                            continue;
                        }
                    }

                    int neighborIndex = ny * width + nx;
                    const TileComponent& neighborTile = tilemap.tiles[neighborIndex];
                    if (tile.upTexture.id == neighborTile.upTexture.id) {
                        surrounding |= 1 << i;          
                    } 
                }
                auto iter = m.find(surrounding);
                
                if (iter == m.end()) {
                    tile.tileX = 0;
                    tile.tileY = 0;
                    std::print("Tile not found", static_cast<int>(surrounding));
                    exit(1);
                } else {
                    auto& pairs = iter->second;
                    
                    if (pairs.size() == 1) {
                        tile.tileX = pairs[0].first;
                        tile.tileY = pairs[0].second;
                    } else {
                        int index = rand() % pairs.size();
                        tile.tileX = pairs[index].first;
                        tile.tileY = pairs[index].second;
                    }
                }
            }
        }
    }
}

void SpriteSetupSystem::setup() {
    auto view = scene->r.view<SpriteComponent>();
    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        sprite.texture = TextureManager::LoadTexture(sprite.name);
    }
}

SpriteSetupSystem::~SpriteSetupSystem() {
    auto view = scene->r.view<SpriteComponent>();
    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        TextureManager::UnloadTexture(sprite.name);
    }
}

void SpriteRenderSystem::render() {
    auto view = scene->r.view<TransformComponent, SpriteComponent>();
    auto& cameraTransform = scene->camera->get<TransformComponent>();
    auto& cameraComponent = scene->camera->get<CameraComponent>();
    float cam_vw = (float)cameraComponent.vw;
    float cam_vh = (float)cameraComponent.vh;
    float zoom = cameraComponent.zoom;

    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);

        float worldX = transform.position.x;
        float worldY = transform.position.y;
        float worldW = (float)sprite.size * 5;
        float worldH = (float)sprite.size * 5;

        float screenX = worldX - cameraTransform.position.x;
        float screenY = worldY - cameraTransform.position.y;

        float newScreenX = cam_vw / 2.0f + (screenX - cam_vw / 2.0f) * zoom;
        float newScreenY = cam_vh / 2.0f + (screenY - cam_vh / 2.0f) * zoom;

        Rectangle sourceRec = {
            (float)sprite.xIndex * sprite.size,
            (float)sprite.yIndex * sprite.size,
            (float)sprite.size,
            (float)sprite.size
        };

        Rectangle destRec = {
            newScreenX,
            newScreenY,
            worldW * zoom,
            worldH * zoom
        };

        DrawTexturePro(sprite.texture, sourceRec, destRec, {0, 0}, 0, WHITE);
    }
}

void SpriteUpdateSystem::update() {
    auto view = scene->r.view<SpriteComponent>();
    long now = GetTime() * 1000;

    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);

        if (sprite.animationFrames > 0) {
            float timeSinceLastUpdate = now - sprite.lastUpdate;

            int framesToUpdate = static_cast<int>(
                timeSinceLastUpdate /
                sprite.animationDuration * sprite.animationFrames
            );

            if (framesToUpdate > 0) {
                int oldXIndex = sprite.xIndex;
                sprite.xIndex += framesToUpdate;
                sprite.xIndex %= sprite.animationFrames;

                if (scene->r.all_of<PlayerComponent>(entity)) {
                    auto& player = scene->r.get<PlayerComponent>(entity);
                    if (player.isAttacking && sprite.xIndex < oldXIndex) {
                        player.isAttacking = false;
                    }
                }
                sprite.lastUpdate = now;
            }
        }
    }
}

void AnimationSystem::update() {
    auto view = scene->r.view<SpriteComponent, VelocityComponent>();
    for (auto entity : view) {
        if (scene->r.all_of<PlayerComponent>(entity)) {
            auto& sprite = view.get<SpriteComponent>(entity);
            auto& vel = view.get<VelocityComponent>(entity);
            auto& player = scene->r.get<PlayerComponent>(entity);

            if (player.isAttacking) {
                std::println("Animation: Attacking with tool {}", (int)player.currentTool);
                sprite.animationDuration = 500;
                switch (player.currentTool) {
                    case SHOVEL:
                        if (player.lastDirection.y > 0) sprite.yIndex = 12;
                        else if (player.lastDirection.y < 0) sprite.yIndex = 13;
                        else if (player.lastDirection.x < 0) sprite.yIndex = 14;
                        else if (player.lastDirection.x > 0) sprite.yIndex = 15;
                        break;
                    case AXE:
                        if (player.lastDirection.y > 0) sprite.yIndex = 16;
                        else if (player.lastDirection.y < 0) sprite.yIndex = 17;
                        else if (player.lastDirection.x < 0) sprite.yIndex = 18;
                        else if (player.lastDirection.x > 0) sprite.yIndex = 19;
                        break;
                    case WATER_CAN:
                        if (player.lastDirection.y > 0) sprite.yIndex = 20;
                        else if (player.lastDirection.y < 0) sprite.yIndex = 21;
                        else if (player.lastDirection.x < 0) sprite.yIndex = 22;
                        else if (player.lastDirection.x > 0) sprite.yIndex = 23;
                        break;
                    case NONE:
                        std::println("Animation: Attacking with no tool");
                        break;
                }
            } else if (vel.velocity.x != 0 || vel.velocity.y != 0) {
                std::println("Animation: Moving");
                sprite.animationDuration = 1000;
                player.lastDirection = vel.velocity;

                if (player.isRunning) {
                    if (vel.velocity.y > 0) sprite.yIndex = 8;
                    else if (vel.velocity.y < 0) sprite.yIndex = 9;
                    else if (vel.velocity.x > 0) sprite.yIndex = 10;
                    else if (vel.velocity.x < 0) sprite.yIndex = 11;
                } else {
                    if (vel.velocity.y > 0) sprite.yIndex = 4;
                    else if (vel.velocity.y < 0) sprite.yIndex = 5;
                    else if (vel.velocity.x > 0) sprite.yIndex = 6;
                    else if (vel.velocity.x < 0) sprite.yIndex = 7;
                }
            } else {
                std::println("Animation: Idle");
                sprite.animationDuration = 1000;
                if (player.lastDirection.y > 0) sprite.yIndex = 0;
                else if (player.lastDirection.y < 0) sprite.yIndex = 1;
                else if (player.lastDirection.x < 0) sprite.yIndex = 2;
                else if (player.lastDirection.x > 0) sprite.yIndex = 3;
            }
        } else if (scene->r.all_of<EnemyComponent>(entity)) {
            auto& sprite = view.get<SpriteComponent>(entity);
            auto& vel = view.get<VelocityComponent>(entity);

            if (vel.velocity.x != 0 || vel.velocity.y != 0) {
                // Moving animation
                sprite.yIndex = 4; // Assuming row 4 is move animation
            } else {
                // Idle
                sprite.yIndex = 0; // Assuming row 0 is idle
            }
        }
    }
}

void PlayerActionSystem::update() {
    auto view = scene->r.view<PlayerComponent, SpriteComponent>();
    for (auto entity : view) {
        auto& player = view.get<PlayerComponent>(entity);
        auto& sprite = view.get<SpriteComponent>(entity);

        player.isRunning = IsKeyDown(KEY_LEFT_SHIFT);

        if (IsKeyPressed(KEY_ONE)) {
            player.currentTool = SHOVEL;
            std::println("Action: Switched to SHOVEL");
        }
        if (IsKeyPressed(KEY_TWO)) {
            player.currentTool = AXE;
            std::println("Action: Switched to AXE");
        }
        if (IsKeyPressed(KEY_THREE)) {
            player.currentTool = WATER_CAN;
            std::println("Action: Switched to WATER_CAN");
        }
        if (IsKeyPressed(KEY_ZERO)) {
            player.currentTool = NONE;
            std::println("Action: Switched to NONE");
        }

        if (IsKeyPressed(KEY_SPACE) && !player.isAttacking) {
            player.isAttacking = true;
            sprite.xIndex = 0;
            std::println("Action: Attack started");
        }
    }
}

void HelloSystem::setup() {
    std::println("Hello, Pong ECS World!");
}

void InputSystem::update() {
    auto view = scene->r.view<PlayerComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& player = view.get<PlayerComponent>(entity);
        auto& vel = view.get<VelocityComponent>(entity);
        vel.velocity = {0, 0};
        
        float currentSpeed = player.isRunning ? 200.0f : 100.0f;

        if (IsKeyDown(KEY_W)) vel.velocity.y = -currentSpeed;
        if (IsKeyDown(KEY_S)) vel.velocity.y =  currentSpeed;
        if (IsKeyDown(KEY_A)) vel.velocity.x = -currentSpeed;
        if (IsKeyDown(KEY_D)) vel.velocity.x =  currentSpeed;
    }
}

void MovementSystem::update() {
    float dT = GetFrameTime();
    auto view = scene->r.view<TransformComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& pos = view.get<TransformComponent>(entity);
        auto& vel = view.get<VelocityComponent>(entity);
        pos.position.x += vel.velocity.x * dT;
        pos.position.y += vel.velocity.y * dT;
    }
}

void RenderSystem::render() {
    auto view = scene->r.view<TransformComponent, SizeComponent, ColorComponent>();
    for (auto entity : view) {
        const auto& pos = view.get<TransformComponent>(entity).position;
        const auto& size = view.get<SizeComponent>(entity);
        const auto& color = view.get<ColorComponent>(entity).color;

        DrawRectangle(
            static_cast<int>(pos.x),
            static_cast<int>(pos.y),
            static_cast<int>(size.width),
            static_cast<int>(size.height),
            color
        );
    }
}

void CollisionSystem::update() {
    auto playerView = scene->r.view<PlayerComponent, TransformComponent, VelocityComponent, ColliderComponent, SpriteComponent>();
    auto tilemapView = scene->r.view<TilemapComponent, IntGridComponent>();

    for (auto playerEntity : playerView) {
        auto& playerCollider = playerView.get<ColliderComponent>(playerEntity);
        playerCollider.triggered = false;
        playerCollider.triggerType = ColliderComponent::NONE;

        auto& playerTransform = playerView.get<TransformComponent>(playerEntity);
        auto& playerVelocity = playerView.get<VelocityComponent>(playerEntity);
        auto& sprite = playerView.get<SpriteComponent>(playerEntity);

        if (playerVelocity.velocity.x == 0 && playerVelocity.velocity.y == 0) {
            continue;
        }

        for (auto tilemapEntity : tilemapView) {
            auto& tilemap = tilemapView.get<TilemapComponent>(tilemapEntity);
            auto& intGrid = tilemapView.get<IntGridComponent>(tilemapEntity);

            float tileScale = tilemap.tiles[0].scale;
            float scaledTileSize = tilemap.tileSize * tileScale;

            float spriteScale = 5.0f;
            
            // Future position
            Vector2 nextPos = {
                playerTransform.position.x + playerVelocity.velocity.x * GetFrameTime(),
                playerTransform.position.y + playerVelocity.velocity.y * GetFrameTime()
            };

            // Collider corners in next position
            float nextColliderX = nextPos.x + playerCollider.offsetX * spriteScale;
            float nextColliderY = nextPos.y + playerCollider.offsetY * spriteScale;
            float colliderWidth = playerCollider.width * spriteScale;
            float colliderHeight = playerCollider.height * spriteScale;

            Vector2 topLeft = {nextColliderX, nextColliderY};
            Vector2 topRight = {nextColliderX + colliderWidth, nextColliderY};
            Vector2 bottomLeft = {nextColliderX, nextColliderY + colliderHeight};
            Vector2 bottomRight = {nextColliderX + colliderWidth, nextColliderY + colliderHeight};

            auto handleTileInteraction = [&](int tileX, int tileY) {
                if (tileX >= 0 && tileX < intGrid.width && tileY >= 0 && tileY < intGrid.height) {
                    int tileType = intGrid.grid[tileY * intGrid.width + tileX];
                    if (tileType != 1) { // Not grass
                        playerCollider.triggered = true;
                        switch (tileType) {
                            case 0: // Water
                                playerCollider.triggerType = ColliderComponent::SOLID;
                                break;
                            case 2: // Treasure
                                playerCollider.triggerType = ColliderComponent::TREASURE;
                                break;
                            case 3: // Teleport
                                playerCollider.triggerType = ColliderComponent::TELEPORT;
                                playerCollider.data.teleport = {10, 10};
                                break;
                        }
                        return true;
                    }
                }
                return false;
            };

            // Check X and Y movement
            if (playerVelocity.velocity.x != 0 || playerVelocity.velocity.y != 0) {
                Vector2 corners[] = {topLeft, topRight, bottomLeft, bottomRight};
                for (const auto& corner : corners) {
                    int tileX = static_cast<int>(corner.x / scaledTileSize);
                    int tileY = static_cast<int>(corner.y / scaledTileSize);
                    if (handleTileInteraction(tileX, tileY)) {
                        return; // Stop after first trigger
                    }
                }
            }
        }
    }
}

void ColliderRenderSystem::render() {
    auto view = scene->r.view<TransformComponent, ColliderComponent, SpriteComponent>();
    auto& cameraTransform = scene->camera->get<TransformComponent>();
    auto& cameraComponent = scene->camera->get<CameraComponent>();
    float cam_vw = (float)cameraComponent.vw;
    float cam_vh = (float)cameraComponent.vh;
    float zoom = cameraComponent.zoom;

    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& collider = view.get<ColliderComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);

        float spriteScale = 5.0f; // This seems to be hardcoded in SpriteRenderSystem

        float worldX = transform.position.x + collider.offsetX * spriteScale;
        float worldY = transform.position.y + collider.offsetY * spriteScale;
        float worldW = collider.width * spriteScale;
        float worldH = collider.height * spriteScale;

        float screenX = worldX - cameraTransform.position.x;
        float screenY = worldY - cameraTransform.position.y;

        float newScreenX = cam_vw / 2.0f + (screenX - cam_vw / 2.0f) * zoom;
        float newScreenY = cam_vh / 2.0f + (screenY - cam_vh / 2.0f) * zoom;

        DrawRectangleLines(
            newScreenX,
            newScreenY,
            worldW * zoom,
            worldH * zoom,
            RED
        );
    }
}

void IntGridRenderSystem::render() {
    auto view = scene->r.view<TilemapComponent, IntGridComponent>();
    auto& cameraTransform = scene->camera->get<TransformComponent>();
    auto& cameraComponent = scene->camera->get<CameraComponent>();
    float cam_vw = (float)cameraComponent.vw;
    float cam_vh = (float)cameraComponent.vh;
    float zoom = cameraComponent.zoom;

    for (auto entity : view) {
        auto& tilemap = view.get<TilemapComponent>(entity);
        auto& intGrid = view.get<IntGridComponent>(entity);

        float tileScale = tilemap.tiles[0].scale;
        float scaledTileSize = tilemap.tileSize * tileScale;

        for (int y = 0; y < intGrid.height; y++) {
            for (int x = 0; x < intGrid.width; x++) {
                int tileType = intGrid.grid[y * intGrid.width + x];
                Color color;
                bool shouldDraw = true;

                switch (tileType) {
                    case 0: // Water
                        color = Fade(RED, 0.5f);
                        break;
                    case 2: // Treasure
                        color = Fade(YELLOW, 0.5f);
                        break;
                    case 3: // Teleport
                        color = Fade(PURPLE, 0.5f);
                        break;
                    default:
                        shouldDraw = false;
                        break;
                }

                if (shouldDraw) {
                    float worldX = x * scaledTileSize;
                    float worldY = y * scaledTileSize;

                    float screenX = worldX - cameraTransform.position.x;
                    float screenY = worldY - cameraTransform.position.y;

                    float newScreenX = cam_vw / 2.0f + (screenX - cam_vw / 2.0f) * zoom;
                    float newScreenY = cam_vh / 2.0f + (screenY - cam_vh / 2.0f) * zoom;

                    DrawRectangle(
                        newScreenX,
                        newScreenY,
                        scaledTileSize * zoom,
                        scaledTileSize * zoom,
                        color
                    );
                }
            }
        }
    }
}

void SolidCollisionSystem::update() {
    auto view = scene->r.view<PlayerComponent, VelocityComponent, ColliderComponent>();
    for (auto entity : view) {
        auto& collider = view.get<ColliderComponent>(entity);
        if (collider.triggered && collider.triggerType == ColliderComponent::SOLID) {
            auto& vel = view.get<VelocityComponent>(entity);
            vel.velocity = {0, 0};
        }
    }
}

void TreasureSystem::update() {
    auto view = scene->r.view<PlayerComponent, ColliderComponent>();
    for (auto entity : view) {
        auto& collider = view.get<ColliderComponent>(entity);
        if (collider.triggered && collider.triggerType == ColliderComponent::TREASURE) {
            std::println("You found a treasure!");
        }
    }
}

void TeleportSystem::update() {
    auto view = scene->r.view<PlayerComponent, TransformComponent, ColliderComponent>();
    auto tilemapView = scene->r.view<TilemapComponent>();
    for (auto entity : view) {
        auto& collider = view.get<ColliderComponent>(entity);
        if (collider.triggered && collider.triggerType == ColliderComponent::TELEPORT) {
            auto& transform = view.get<TransformComponent>(entity);
            for (auto tilemapEntity : tilemapView) {
                auto& tilemap = tilemapView.get<TilemapComponent>(tilemapEntity);
                float tileScale = tilemap.tiles[0].scale;
                float scaledTileSize = tilemap.tileSize * tileScale;
                transform.position.x = collider.data.teleport.x * scaledTileSize;
                transform.position.y = collider.data.teleport.y * scaledTileSize;
            }
        }
    }
}

void EnemyMovementSystem::update() {
    auto view = scene->r.view<EnemyComponent, EnemyMovementComponent, TransformComponent, VelocityComponent>();

    // Get player position
    auto playerView = scene->r.view<PlayerComponent, TransformComponent>();
    float player_x = 0.0f;
    float player_y = 0.0f;
    for (auto playerEntity : playerView) {
        auto& playerTransform = playerView.get<TransformComponent>(playerEntity);
        player_x = playerTransform.position.x;
        player_y = playerTransform.position.y;
        break; // Only need one player
    }

    for (auto entity : view) {
        auto& movement = view.get<EnemyMovementComponent>(entity);
        auto& transform = view.get<TransformComponent>(entity);
        auto& velocity = view.get<VelocityComponent>(entity);

        // Initialize start time if not set
        if (movement.startTime == 0.0f) {
            movement.startTime = GetTime();
            movement.startPosition = transform.position;
        }

        // Calculate elapsed time since spawn
        float elapsed = GetTime() - movement.startTime;

        // Load and execute the Lua script
        if (!movement.movementScript.empty()) {
            sol::state lua;
            lua.open_libraries(sol::lib::base, sol::lib::math);

            // Create input table for Lua
            sol::table input = lua.create_table();
            input["elapsed_time"] = elapsed;
            input["delta_time"] = GetFrameTime();
            input["start_x"] = movement.startPosition.x;
            input["start_y"] = movement.startPosition.y;
            input["current_x"] = transform.position.x;
            input["current_y"] = transform.position.y;
            input["player_x"] = player_x;
            input["player_y"] = player_y;

            lua["input"] = input;

            // Run the script
            try {
                lua.script_file(movement.movementScript);

                // Get output from Lua
                sol::table output = lua["output"];
                if (output) {
                    // Check if output contains velocity
                    if (output["velocity_x"].valid() && output["velocity_y"].valid()) {
                        velocity.velocity.x = output["velocity_x"];
                        velocity.velocity.y = output["velocity_y"];
                    }
                }
            } catch (const std::exception& e) {
                std::println("Error executing movement script {}: {}", movement.movementScript, e.what());
            }
        }
    }
}
