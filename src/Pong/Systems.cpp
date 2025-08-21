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
#include <bitset>

void TilemapSetupSystem::setup() {
    Entity tilemapEntity = scene->createEntity("tilemap");
    auto& tilemap = tilemapEntity.addComponent<TilemapComponent>();
    tilemap.width = TILEMAP_WIDTH;
    tilemap.height = TILEMAP_HEIGHT;
    tilemap.tileSize = 16;

    Texture2D waterTexture = TextureManager::LoadTexture("assets/Tilesets/Water.png");
    Texture2D grassTexture = TextureManager::LoadTexture("assets/Tilesets/Grass.png");

    for (int y = 0; y < tilemap.height; y++) {
        for (int x = 0; x < tilemap.width; x++) {
            TileComponent tile;
            tile.x = x;
            tile.y = y;
            
            switch (TILEMAP_DATA[y][x]) {
                case 0:
                    tile.upTexture = grassTexture;
                    tile.downTexture = waterTexture;
                    tile.needsAutoTiling = true;
                    break;
                case 1:
                    tile.upTexture = waterTexture;
                    break;
            }

            tilemap.tiles.push_back(tile);
        }
    }
}

void TilemapRenderSystem::render() {
    auto view = scene->r.view<TilemapComponent>();
    for (auto entity : view) {
        auto& tilemap = view.get<TilemapComponent>(entity);
        for (auto& tile : tilemap.tiles) {
            if (tile.downTexture.id > 0) {
                DrawTextureEx(tile.downTexture, {(float)tile.x * tilemap.tileSize * tile.scale, (float)tile.y * tilemap.tileSize * tile.scale}, 0, tile.scale, WHITE);
            }
            Rectangle sourceRec = {
                (float)tile.tileX,
                (float)tile.tileY,
                (float)tilemap.tileSize,
                (float)tilemap.tileSize
            };
            Rectangle destRec = {
                (float)tile.x * tilemap.tileSize * tile.scale,
                (float)tile.y * tilemap.tileSize * tile.scale,
                (float)tilemap.tileSize * tile.scale,
                (float)tilemap.tileSize * tile.scale
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
    for (auto entity : view) {
        const auto& transform = view.get<TransformComponent>(entity);
        const auto& sprite = view.get<SpriteComponent>(entity);

        Rectangle sourceRec = {
            (float)sprite.xIndex * sprite.size,
            (float)sprite.yIndex * sprite.size,
            (float)sprite.size,
            (float)sprite.size
        };

        Rectangle destRec = {
            transform.position.x,
            transform.position.y,
            (float)sprite.size * 5,
            (float)sprite.size * 5
        };

        DrawTexturePro(sprite.texture, sourceRec, destRec, {0, 0}, 0, WHITE);
    }
}

void SpriteUpdateSystem::update() {
    auto view = scene->r.view<SpriteComponent, PlayerComponent>();
    long now = GetTime() * 1000;

    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& player = view.get<PlayerComponent>(entity);

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

                if (player.isAttacking && sprite.xIndex < oldXIndex) {
                    player.isAttacking = false;
                }
                sprite.lastUpdate = now;
            }
        }
    }
}

void SpriteAnimationSystem::update() {
    auto view = scene->r.view<SpriteComponent, VelocityComponent, PlayerComponent>();
    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& vel = view.get<VelocityComponent>(entity);
        auto& player = view.get<PlayerComponent>(entity);

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

void PlayerInputEventSystem::update() {
    auto view = scene->r.view<PlayerComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& vel = view.get<VelocityComponent>(entity);
        int speed = 200;

        if (IsKeyDown(KEY_LEFT)) vel.velocity.x = -speed;
        if (IsKeyDown(KEY_RIGHT)) vel.velocity.x = speed;
        if (IsKeyDown(KEY_UP)) vel.velocity.y = -speed;
        if (IsKeyDown(KEY_DOWN)) vel.velocity.y = speed;

        if (IsKeyUp(KEY_LEFT) && IsKeyUp(KEY_RIGHT)) vel.velocity.x = 0;
        if (IsKeyUp(KEY_UP) && IsKeyUp(KEY_DOWN)) vel.velocity.y = 0;
    }
}

void PlayerSpriteUpdateSystem::update() {
    auto view = scene->r.view<PlayerComponent, SpriteComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& sprite = view.get<SpriteComponent>(entity);
        auto& vel = view.get<VelocityComponent>(entity);

        if (vel.velocity.x < 0) {
            sprite.yIndex = 7;
        }
        else if (vel.velocity.x > 0) {
            sprite.yIndex = 6;
        }
        else if (vel.velocity.y < 0) {
            sprite.yIndex = 5;
        }
        else if (vel.velocity.y > 0) {
            sprite.yIndex = 4;
        }
        else {
            if (sprite.yIndex == 7) {
                sprite.yIndex = 2;
            }
            else if (sprite.yIndex == 6) {
                sprite.yIndex = 3;
            }
            else if (sprite.yIndex == 5) {
                sprite.yIndex = 1;
            }
            else if (sprite.yIndex == 4) {
                sprite.yIndex = 0;
            }
        }
    }
}

void CameraFollowUpdateSystem::update() {
    auto cameraView = scene->r.view<CameraComponent, TransformComponent>();
    auto playerView = scene->r.view<PlayerComponent, TransformComponent>();
    auto worldView = scene->r.view<WorldComponent>();

    for (auto cameraEntity : cameraView) {
        auto& camera = cameraView.get<CameraComponent>(cameraEntity);
        auto& cameraTransform = cameraView.get<TransformComponent>(cameraEntity);

        for (auto playerEntity : playerView) {
            auto& playerTransform = playerView.get<TransformComponent>(playerEntity);

            for (auto worldEntity : worldView) {
                auto& world = worldView.get<WorldComponent>(worldEntity);

                int px = playerTransform.position.x - camera.vw / 2 + (48 / 2) * camera.zoom;
                int py = playerTransform.position.y - camera.vh / 2 + (48 / 2) * camera.zoom;

                if (px > 0 && px < world.width - camera.vw) {
                    cameraTransform.position.x = px;
                }

                if (py > 0 && py < world.height - camera.vh) {
                    cameraTransform.position.y = py;
                }
            }
        }
    }
}

