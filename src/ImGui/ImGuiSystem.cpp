#include "ImGuiSystem.h"
#include "Scene/Scene.h"
#include "rlImGui.h"
#include "imgui.h"

void ImGuiSystem::setup() {
    rlImGuiSetup(true);
}

void ImGuiSystem::render() {
    rlImGuiBegin();

    ImGui::Begin("Components");

    auto view = scene->r.view<NameComponent>();
    for (auto entity : view) {
        auto& name = view.get<NameComponent>(entity);
        if (ImGui::TreeNode(name.tag.c_str())) {
            // TODO: Display component properties
            ImGui::TreePop();
        }
    }

    ImGui::End();

    rlImGuiEnd();
}
