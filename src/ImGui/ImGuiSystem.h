#pragma once

#include "ECS/System.h"

class ImGuiSystem : public System {
public:
    void setup() override;
    void render() override;
};
