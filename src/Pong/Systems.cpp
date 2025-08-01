#include "ECS/System.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include "Game/Scene/Scene.h"
#include <print>

class HelloSystem : public System {
public:
    void setup() override {
        std::println("Hello, Pong ECS World!");
    }
};

class InputSystem : public System {
public:
    void update() override {
        auto view = scene->r.view<PlayerComponent, VelocityComponent>();
        for (auto entity : view) {
            auto& player = view.get<PlayerComponent>(entity);
            auto& vel = view.get<VelocityComponent>(entity);

            vel.velocity.x = 0.0f;
            if (IsKeyDown(KEY_LEFT))  vel.velocity.x = -player.moveSpeed;
            if (IsKeyDown(KEY_RIGHT)) vel.velocity.x =  player.moveSpeed;
        }
    }
};

class MovementSystem : public System {
public:
    void update() override {
        float dT = GetFrameTime();
        auto view = scene->r.view<TransformComponent, VelocityComponent>();
        for (auto entity : view) {
            auto& pos = view.get<TransformComponent>(entity);
            auto& vel = view.get<VelocityComponent>(entity);
            pos.position.x += vel.velocity.x * dT;
            pos.position.y += vel.velocity.y * dT;
        }
    }
};

class CollisionSystem : public System {
public:
    void update() override {
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
                ballVel = {-20.0f, -20.0f};
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
};


class RenderSystem : public System {
public:
    void render() override {
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
};
