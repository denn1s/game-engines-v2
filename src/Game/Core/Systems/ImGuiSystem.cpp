#include "ImGuiSystem.h"
#include "Scene/Scene.h"
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
        ImGui::Text("TransformComponent");
        ImGui::Text("  Position: %.2f, %.2f", transform.position.x, transform.position.y);
    }

    if (registry.all_of<VelocityComponent>(entity)) {
        auto& velocity = registry.get<VelocityComponent>(entity);
        ImGui::Text("VelocityComponent");
        ImGui::Text("  Velocity: %.2f, %.2f", velocity.velocity.x, velocity.velocity.y);
    }
}
