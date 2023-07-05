#pragma once
#include <entt/entt.hpp>
#include "ECS/System.h"

class EngineUISystem :
  public SetupSystem,
  public UpdateSystem,
  public EventSystem,
  public RenderSystem {

  public:
    EngineUISystem(SDL_Renderer* r, SDL_Window* w);
    ~EngineUISystem();

    void run() override;
    void run(double dT) override;
    void run(SDL_Event event) override;
    void run(SDL_Renderer* r) override;

  private:
    SDL_Renderer* renderer;
    SDL_Window* window;
    entt::entity selectedEntity;
};