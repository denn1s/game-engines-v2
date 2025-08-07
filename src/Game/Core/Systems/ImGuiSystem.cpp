#include "ImGuiSystem.h"
#include "Game/Scene/Scene.h"
#include "rlImGui.h"
#include "imgui.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include <string>
#include <vector>
#include <algorithm>

// Helper to display a single component's properties
void DisplayComponentDetails(entt::registry& registry, entt::entity entity, const std::string& componentName) {
    if (componentName == "NameComponent") {
        if (registry.all_of<NameComponent>(entity)) {
            auto& comp = registry.get<NameComponent>(entity);
            ImGui::Text("Tag: %s", comp.tag.c_str());
        }
    } else if (componentName == "TransformComponent") {
        if (registry.all_of<TransformComponent>(entity)) {
            auto& comp = registry.get<TransformComponent>(entity);
            float pos[2] = { comp.position.x, comp.position.y };
            ImGui::InputFloat2("Position", pos);
            comp.position.x = pos[0];
            comp.position.y = pos[1];
        }
    } else if (componentName == "VelocityComponent") {
        if (registry.all_of<VelocityComponent>(entity)) {
            auto& comp = registry.get<VelocityComponent>(entity);
            float vel[2] = { comp.velocity.x, comp.velocity.y };
            ImGui::InputFloat2("Velocity", vel);
            comp.velocity.x = vel[0];
            comp.velocity.y = vel[1];
        }
    } else if (componentName == "SizeComponent") {
        if (registry.all_of<SizeComponent>(entity)) {
            auto& comp = registry.get<SizeComponent>(entity);
            float size[2] = { comp.width, comp.height };
            ImGui::InputFloat2("Size", size);
            comp.width = size[0];
            comp.height = size[1];
        }
    } else if (componentName == "PlayerComponent") {
        if (registry.all_of<PlayerComponent>(entity)) {
            auto& comp = registry.get<PlayerComponent>(entity);
            ImGui::Checkbox("Running", &comp.isRunning);
            ImGui::Checkbox("Attacking", &comp.isAttacking);
            const char* tool_names[] = { "NONE", "SHOVEL", "AXE", "WATER_CAN" };
            ImGui::Combo("Tool", (int*)&comp.currentTool, tool_names, IM_ARRAYSIZE(tool_names));
        }
    } else if (componentName == "ColliderComponent") {
        if (registry.all_of<ColliderComponent>(entity)) {
            auto& comp = registry.get<ColliderComponent>(entity);
            ImGui::Checkbox("Triggered", &comp.triggered);
        }
    } else if (componentName == "ColorComponent") {
        if (registry.all_of<ColorComponent>(entity)) {
            auto& comp = registry.get<ColorComponent>(entity);
            float color[4] = { comp.color.r / 255.0f, comp.color.g / 255.0f, comp.color.b / 255.0f, comp.color.a / 255.0f };
            ImGui::ColorEdit4("Color", color);
            comp.color.r = static_cast<unsigned char>(color[0] * 255.0f);
            comp.color.g = static_cast<unsigned char>(color[1] * 255.0f);
            comp.color.b = static_cast<unsigned char>(color[2] * 255.0f);
            comp.color.a = static_cast<unsigned char>(color[3] * 255.0f);
        }
    }
}

ImGuiSystem::ImGuiSystem() {
    rlImGuiSetup(true);
}

ImGuiSystem::~ImGuiSystem() {
    rlImGuiShutdown();
}

void ImGuiSystem::setup() {
}

void ImGuiSystem::render() {
    rlImGuiBegin();

    // --- Entities Window ---
    ImGui::Begin("Entities");
    ImGui::InputText("##EntityFilter", entityFilter, IM_ARRAYSIZE(entityFilter));

    std::vector<std::string> entityNames;
    std::vector<entt::entity> entities;

    auto view = scene->r.view<NameComponent>();
    for (auto entity : view) {
        auto& name = view.get<NameComponent>(entity);
        if (strlen(entityFilter) == 0 || name.tag.find(entityFilter) != std::string::npos) {
            entityNames.push_back(name.tag);
            entities.push_back(entity);
        }
    }

    if (ImGui::BeginCombo("##EntitiesCombo", selectedEntity != entt::null ? scene->r.get<NameComponent>(selectedEntity).tag.c_str() : "Select Entity")) {
        for (size_t i = 0; i < entityNames.size(); ++i) {
            bool is_selected = (selectedEntity == entities[i]);
            if (ImGui::Selectable(entityNames[i].c_str(), is_selected)) {
                selectedEntity = entities[i];
                selectedComponentIndex = -1; // Reset component selection
            }
            if (is_selected) {
                ImGui::SetItemDefaultFocus();
            }
        }
        ImGui::EndCombo();
    }
    ImGui::End();

    // --- Components Window ---
    ImGui::Begin("Components");
    if (selectedEntity != entt::null) {
        ImGui::Text("Selected Entity: %s", scene->r.get<NameComponent>(selectedEntity).tag.c_str());
        ImGui::InputText("##ComponentFilter", componentFilter, IM_ARRAYSIZE(componentFilter));

        componentNames.clear();
        if (scene->r.all_of<NameComponent>(selectedEntity)) componentNames.push_back("NameComponent");
        if (scene->r.all_of<TransformComponent>(selectedEntity)) componentNames.push_back("TransformComponent");
        if (scene->r.all_of<VelocityComponent>(selectedEntity)) componentNames.push_back("VelocityComponent");
        if (scene->r.all_of<SizeComponent>(selectedEntity)) componentNames.push_back("SizeComponent");
        if (scene->r.all_of<PlayerComponent>(selectedEntity)) componentNames.push_back("PlayerComponent");
        if (scene->r.all_of<ColliderComponent>(selectedEntity)) componentNames.push_back("ColliderComponent");
        if (scene->r.all_of<ColorComponent>(selectedEntity)) componentNames.push_back("ColorComponent");

        // Filter component names
        std::vector<std::string> filteredComponentNames;
        for (const auto& name : componentNames) {
            if (strlen(componentFilter) == 0 || name.find(componentFilter) != std::string::npos) {
                filteredComponentNames.push_back(name);
            }
        }

        if (ImGui::BeginCombo("##ComponentsCombo", selectedComponentIndex != -1 ? filteredComponentNames[selectedComponentIndex].c_str() : "Select Component")) {
            for (size_t i = 0; i < filteredComponentNames.size(); ++i) {
                bool is_selected = (selectedComponentIndex == i);
                if (ImGui::Selectable(filteredComponentNames[i].c_str(), is_selected)) {
                    selectedComponentIndex = i;
                }
                if (is_selected) {
                    ImGui::SetItemDefaultFocus();
                }
            }
            ImGui::EndCombo();
        }

        if (selectedComponentIndex != -1) {
            ImGui::Separator();
            ImGui::Text("Properties of %s:", filteredComponentNames[selectedComponentIndex].c_str());
            DisplayComponentDetails(scene->r, selectedEntity, filteredComponentNames[selectedComponentIndex]);
        }
    } else {
        ImGui::Text("No entity selected.");
    }
    ImGui::End();

    rlImGuiEnd();
}