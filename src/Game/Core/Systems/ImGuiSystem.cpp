#include "ImGuiSystem.h"
#include "Game/Scene/Scene.h"
#include "rlImGui.h"
#include "imgui.h"
#include "ECS/Components.h"
#include "Pong/Components.h"

void ShowComponentProperties(entt::registry& registry, entt::entity entity);

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

    ImGui::Begin("Components");

    auto view = scene->r.view<NameComponent>();
    for (auto entity : view) {
        auto& name = view.get<NameComponent>(entity);
        if (ImGui::TreeNode(name.tag.c_str())) {
            ShowComponentProperties(scene->r, entity);
            ImGui::TreePop();
        }
    }
   
    ImGui::End();

    rlImGuiEnd();
}

void ShowComponentProperties(entt::registry& registry, entt::entity entity) {
    if (registry.all_of<TransformComponent>(entity)) {
        auto& transform = registry.get<TransformComponent>(entity);
    
        ImGui::SliderFloat("pX", &transform.position.x, 0.0f, 1300.0f);
        ImGui::SliderFloat("pY", &transform.position.y, 0.0f, 800.0f);}

    if (registry.all_of<VelocityComponent>(entity)) {
        auto& velocity = registry.get<VelocityComponent>(entity);
        ImGui::SliderFloat("vX", &velocity.velocity.x, -100.0f, 100.0f);
        ImGui::SliderFloat("vY", &velocity.velocity.y, -100.0f, 100.0f);
    }
}
