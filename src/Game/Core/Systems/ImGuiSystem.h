#pragma once

#include "ECS/System.h"

class ImGuiSystem : public System {
public:
    ImGuiSystem();
    ~ImGuiSystem();
    void setup() override;
    void render() override;
};
