#include "Systems.h"

#include "print.h"
#include "imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

#include "ECS/Components.h"

EngineUISystem::EngineUISystem(SDL_Renderer* r, SDL_Window* w)
  : renderer(r), window(w) {
    selectedEntity = entt::null;
  }

EngineUISystem::~EngineUISystem() {}

void EngineUISystem::run() {
  print("UI engine setup");

  // Setup Dear ImGui context
  IMGUI_CHECKVERSION();
  ImGui::CreateContext();
  ImGuiIO& io = ImGui::GetIO(); (void)io;
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
  //io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

  // Setup Dear ImGui style
  ImGui::StyleColorsDark();
  //ImGui::StyleColorsLight();

  // Setup Platform/Renderer backends
  ImGui_ImplSDL2_InitForSDLRenderer(window, renderer);
  ImGui_ImplSDLRenderer_Init(renderer);
}

void EngineUISystem::run(SDL_Event event) {
  print("UI engine events");
  ImGui_ImplSDL2_ProcessEvent(&event);
}

void EngineUISystem::run(double dT) {
  print("UI engine update");

  // Start the Dear ImGui frame
  ImGui_ImplSDLRenderer_NewFrame();
  ImGui_ImplSDL2_NewFrame();
  ImGui::NewFrame();

  ImGui::Begin("Entities", nullptr);
  // ImGui::Text("Test");

  ImGui::SetNextItemOpen(true, ImGuiCond_Once);
  if (ImGui::TreeNode(UpdateSystem::scene->name.c_str())) {
      const auto view = UpdateSystem::scene->r.view<NameComponent>();
      for (const auto& entity : view) {
        const NameComponent n = view.get<NameComponent>(entity);

        ImGui::TreeNodeEx(n.name.c_str(), ImGuiTreeNodeFlags_Leaf);
        if (ImGui::IsItemClicked()) {
            selectedEntity = entity;
        }
        ImGui::TreePop();
      }
      ImGui::TreePop();
  }
  ImGui::End();
  
  if (selectedEntity != entt::null) {
    auto& entityTransform =  UpdateSystem::scene->r.get<TransformComponent>(selectedEntity);
    auto& entityName =  UpdateSystem::scene->r.get<NameComponent>(selectedEntity);

    ImGui::Begin(("Components for " + entityName.name).c_str(), nullptr);
      if (ImGui::Button("Close")) {
        selectedEntity = entt::null;
      }
      ImGui::SetNextItemOpen(true, ImGuiCond_Once);
      if (ImGui::CollapsingHeader("Transform")) {
        ImGui::InputFloat2("Position", &entityTransform.position[0]);
        ImGui::InputFloat2("Scale", &entityTransform.scale[0]);
        ImGui::InputDouble("Rotation", &entityTransform.rotation);
      }
    ImGui::End();
  }

  ImGui::Render();
}

void EngineUISystem::run(SDL_Renderer* r) {
  print("UI engine render");

  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}