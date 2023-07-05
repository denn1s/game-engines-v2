#pragma once

#include <SDL2/SDL.h>
#include <string>
#include <entt/entt.hpp>

class Entity;
class SetupSystem;
class EventSystem;
class UpdateSystem;
class RenderSystem;

class Scene {
  private:
    std::vector<std::shared_ptr<SetupSystem>> setupSystems;
    std::vector<std::shared_ptr<EventSystem>> eventSystems;
    std::vector<std::shared_ptr<UpdateSystem>> updateSystems;
    std::vector<std::shared_ptr<RenderSystem>> renderSystems;

  public:
    Scene(const std::string&);
    ~Scene();

    entt::registry r;
    std::string name;

    Entity createEntity(
      const std::string& name = "NO NAME",
      int x = 0,
      int y = 0
    );
    void addSetupSystem(std::shared_ptr<SetupSystem> s);
    void addEventSystem(std::shared_ptr<EventSystem> s);
    void addUpdateSystem(std::shared_ptr<UpdateSystem> s);
    void addRenderSystem(std::shared_ptr<RenderSystem> s);

    void setup();
    void update(double dT);
    void render(SDL_Renderer* renderer);
    void event(SDL_Event renderer);
};