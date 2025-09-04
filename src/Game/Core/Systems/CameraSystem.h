#pragma once

#include "ECS/System.h"

class CameraSystem : public System {
public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    void setup() override;
    void update() override;
};
