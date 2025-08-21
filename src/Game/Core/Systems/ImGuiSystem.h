#pragma once

#include "ECS/System.h"
#include <string>
#include <vector>
#include <entt/entt.hpp>

class ImGuiSystem : public System {
private:
    entt::entity selectedEntity = entt::null;
    char entityFilter[256] = "";
    char componentFilter[256] = "";
    int selectedComponentIndex = -1;
    std::vector<std::string> componentNames;

public:
    ImGuiSystem();
    ~ImGuiSystem();
    void setup() override;
    void render() override;
};
