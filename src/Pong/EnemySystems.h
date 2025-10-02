#pragma once

#include "ECS/System.h"

class EnemySpawnSystem : public System {
public:
    EnemySpawnSystem() = default;
    ~EnemySpawnSystem() = default;

    void setup() override;
    void update() override;

private:
    float spawnTimer = 0.0f;
    float spawnInterval = 2.0f;
};
