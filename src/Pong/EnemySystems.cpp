#include "EnemySystems.h"
#include "Game/Scene/Scene.h"
#include "ECS/Entity.h"
#include "Components.h"
#include <random>

void EnemySpawnSystem::setup() {
    // The logic from the previous implementation is now in update.
}

void EnemySpawnSystem::update() {
    spawnTimer += GetFrameTime();

    if (spawnTimer >= spawnInterval) {
        spawnTimer = 0.0f;

        // Get player position
        auto view = scene->r.view<PlayerComponent, TransformComponent>();
        for (auto entity : view) {
            auto& playerTransform = view.get<TransformComponent>(entity);
            float playerX = playerTransform.position.x;
            float playerY = playerTransform.position.y;

            // Random position near player
            std::random_device rd;
            std::mt19937 gen(rd());
            std::uniform_real_distribution<> dis(-200.0, 200.0);
            float spawnX = playerX + dis(gen);
            float spawnY = playerY + dis(gen);

            // Random non-diagonal velocity
            std::uniform_int_distribution<> velDis(0, 3);
            int randomDir = velDis(gen);
            Vector2 velocity = {0, 0};
            float speed = 100.0f;
            if (randomDir == 0) velocity.y = -speed; // Up
            else if (randomDir == 1) velocity.y = speed; // Down
            else if (randomDir == 2) velocity.x = -speed; // Left
            else if (randomDir == 3) velocity.x = speed; // Right


            Entity enemy = scene->createEntity("enemy", spawnX, spawnY);
            enemy.addComponent<VelocityComponent>(velocity);
            enemy.addComponent<EnemyComponent>();
            auto& s = enemy.addComponent<SpriteComponent>();
            s.name = "assets/Sprites/EvilCat/1.png";
            s.xIndex = 0;
            s.yIndex = 0;
            s.size = 48;
            s.animationFrames = 8;
            s.animationDuration = 1000;
            s.lastUpdate = GetTime() * 1000;
        }
    }
}
