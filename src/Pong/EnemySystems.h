#pragma once

#include "ECS/System.h"

class EnemySpawnSystem : public System {
public:
    EnemySpawnSystem() = default;
    ~EnemySpawnSystem() = default;

    void setup() override;
};
