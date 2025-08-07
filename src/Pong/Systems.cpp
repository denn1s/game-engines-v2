#include "Systems.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include "Game/Scene/Scene.h"
#include "Game/Graphics/TextureManager.h"
#include <print>
#include <raylib.h>

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
                sprite.xIndex += framesToUpdate;
                sprite.xIndex %= sprite.animationFrames;
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
            sprite.animationDuration = 500; // Faster animation for attack
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
                    // Do nothing
                    break;
            }
        } else if (vel.velocity.x != 0 || vel.velocity.y != 0) {
            sprite.animationDuration = 1000;
            // Update last direction
            player.lastDirection = vel.velocity;

            if (player.isRunning) {
                if (vel.velocity.y > 0) sprite.yIndex = 8;      // Running down
                else if (vel.velocity.y < 0) sprite.yIndex = 9; // Running up
                else if (vel.velocity.x > 0) sprite.yIndex = 10; // Running right
                else if (vel.velocity.x < 0) sprite.yIndex = 11; // Running left
            } else {
                if (vel.velocity.y > 0) sprite.yIndex = 4;      // Walking down
                else if (vel.velocity.y < 0) sprite.yIndex = 5; // Walking up
                else if (vel.velocity.x > 0) sprite.yIndex = 6; // Walking right
                else if (vel.velocity.x < 0) sprite.yIndex = 7; // Walking left
            }
        } else { // Idle
            sprite.animationDuration = 1000;
            if (player.lastDirection.y > 0) sprite.yIndex = 0;      // Idle down
            else if (player.lastDirection.y < 0) sprite.yIndex = 1; // Idle up
            else if (player.lastDirection.x < 0) sprite.yIndex = 2; // Idle left
            else if (player.lastDirection.x > 0) sprite.yIndex = 3; // Idle right
        }
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

                // If animation looped and we were attacking, stop attacking
                if (player.isAttacking && sprite.xIndex < oldXIndex) {
                    player.isAttacking = false;
                }
                sprite.lastUpdate = now;
            }
        }
    }
}

void PlayerActionSystem::update() {
    auto view = scene->r.view<PlayerComponent, SpriteComponent>();
    for (auto entity : view) {
        auto& player = view.get<PlayerComponent>(entity);
        auto& sprite = view.get<SpriteComponent>(entity);

        // Running
        player.isRunning = IsKeyDown(KEY_LEFT_SHIFT);

        // Tool selection
        if (IsKeyPressed(KEY_ONE)) player.currentTool = SHOVEL;
        if (IsKeyPressed(KEY_TWO)) player.currentTool = AXE;
        if (IsKeyPressed(KEY_THREE)) player.currentTool = WATER_CAN;
        if (IsKeyPressed(KEY_ZERO)) player.currentTool = NONE;

        // Attacking
        if (IsKeyPressed(KEY_SPACE) && !player.isAttacking) {
            player.isAttacking = true;
            sprite.xIndex = 0; // Reset animation to the first frame
        }
    }
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

void HelloSystem::setup() {
    std::println("Hello, Pong ECS World!");
}

void InputSystem::update() {
    auto view = scene->r.view<PlayerComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& vel = view.get<VelocityComponent>(entity);
        vel.velocity = {0, 0};
        float speed = 100.0f;

        if (IsKeyDown(KEY_W)) vel.velocity.y = -speed;
        if (IsKeyDown(KEY_S)) vel.velocity.y =  speed;
        if (IsKeyDown(KEY_A)) vel.velocity.x = -speed;
        if (IsKeyDown(KEY_D)) vel.velocity.x =  speed;
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