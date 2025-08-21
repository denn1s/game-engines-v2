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

std::map<uint8_t, std::pair<int, int>> m = {
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

void AutoTilingSetupSystem::setup() {
    auto view = scene->r.view<TilemapComponent>();
    for (auto entity : view) {
        auto& tilemap = view.get<TilemapComponent>(entity);
        int width = tilemap.width;
        int height = tilemap.height;

        const int dx[4] = { 0, -1, 1, 0 };
        const int dy[4] = { -1, 0, 0, 1 };

        for (int y = 0; y < height; y++) {
            for (int x = 0; x < width; x++) {
                int index = y * width + x;
                TileComponent& tile = tilemap.tiles[index];

                if (!tile.needsAutoTiling)
                    continue;

                uint8_t surrounding = 0;
                for (int i = 0; i < 4; i++) {
                    int nx = x + dx[i];
                    int ny = y + dy[i];

                    if (nx < 0 || nx >= width || ny < 0 || ny >= height) 
                        continue;

                    int neighborIndex = ny * width + nx;
                    const TileComponent& neighborTile = tilemap.tiles[neighborIndex];
                    
                    if (tile.upTexture.id == neighborTile.upTexture.id) { 
                        // The line `surrounding |= 1 << i;` is using bit manipulation to set a specific bit in the surrounding variable to 1.
                        // Let's break it down piece by piece:
                        // 1 << i: This is a bit shift operation. It takes the binary number 1 (which is 00000001 in 8 bits) and shifts it to the left i times.
                        // For example, if i is 2, 1 << 2 would be 00000100. This gives us a binary number where only the i-th bit is set to 1.
                        // surrounding |= ...: This is a bitwise OR assignment. It takes the current value of surrounding, performs a bitwise OR with the value on the right-hand side, and assigns the result back to surrounding.
                        // In a bitwise OR, each bit in the result is 1 if at least one of the corresponding bits in the operands is 1.
                        // So, `surrounding |= 1 << i;` has the effect of setting the i-th bit of surrounding to 1, while leaving all other bits unchanged.
                        surrounding |= 1 << i;          
                    }
                }

                tile.tileX = m[surrounding].first;
                tile.tileY = m[surrounding].second;
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
