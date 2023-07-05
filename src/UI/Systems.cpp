#include "Systems.h"

#include "print.h"
#include "imgui.h"
#include "imgui/imgui_impl_sdl.h"
#include "imgui/imgui_impl_sdlrenderer.h"

EngineUISystem::EngineUISystem(SDL_Renderer* r, SDL_Window* w) : renderer(r), window(w) {}

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

  ImGui::Begin("Hello, ImGui!");
  ImGui::Text("This is a simple example using ImGui with SDL2 and SDL_Renderer.");
  ImGui::End();

  ImGui::Render();
}

void EngineUISystem::run(SDL_Renderer* r) {
  print("UI engine render");

  ImGui_ImplSDLRenderer_RenderDrawData(ImGui::GetDrawData());
}