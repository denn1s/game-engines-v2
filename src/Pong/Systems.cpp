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

void HelloSystem::setup() {
    std::println("Hello, Pong ECS World!");
}

void InputSystem::update() {
    auto view = scene->r.view<PlayerComponent, VelocityComponent>();
    for (auto entity : view) {
        auto& player = view.get<PlayerComponent>(entity);
        auto& vel = view.get<VelocityComponent>(entity);

        vel.velocity.x = 0.0f;
        if (IsKeyDown(KEY_LEFT))  vel.velocity.x = -player.moveSpeed;
        if (IsKeyDown(KEY_RIGHT)) vel.velocity.x =  player.moveSpeed;
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

void CollisionSystem::update() {
    auto ballView = scene->r.view<NameComponent, TransformComponent, SizeComponent, VelocityComponent, ColliderComponent>();
    auto paddleView = scene->r.view<PlayerComponent, TransformComponent, SizeComponent>();

    for (auto ball : ballView) {
        auto& ballPos = ballView.get<TransformComponent>(ball).position;
        auto& ballSize = ballView.get<SizeComponent>(ball);
        auto& ballVel  = ballView.get<VelocityComponent>(ball).velocity;
        auto& ballCol  = ballView.get<ColliderComponent>(ball);

        // Wall collision (left/right)
        if (ballPos.x <= 0 || ballPos.x + ballSize.width >= GetScreenWidth()) {
            ballVel.x *= -1.0f;
        }
        // Ceiling collision
        if (ballPos.y <= 0) {
            ballVel.y *= -1.0f;
        }
        // Floor (lose)
        if (ballPos.y + ballSize.height >= GetScreenHeight()) {
            std::println("Game Over!");
            ballVel = {0, 0};
        }

        // Paddle collision
        for (auto paddle : paddleView) {
            auto& padPos = paddleView.get<TransformComponent>(paddle).position;
            auto& padSize = paddleView.get<SizeComponent>(paddle);

            bool overlapX = ballPos.x < padPos.x + padSize.width && ballPos.x + ballSize.width > padPos.x;
            bool overlapY = ballPos.y + ballSize.height > padPos.y && ballPos.y < padPos.y + padSize.height;

            if (overlapX && overlapY) {
                ballVel.y *= -1.1f; // bounce and speed up
                ballVel.x *= 1.05f;
                ballCol.triggered = true;
            }
        }
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
