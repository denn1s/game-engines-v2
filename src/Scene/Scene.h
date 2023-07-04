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
    std::vector<SetupSystem*> setupSystems;
    std::vector<EventSystem*> eventSystems;
    std::vector<UpdateSystem*> updateSystems;
    std::vector<RenderSystem*> renderSystems;
    std::string name;

  public:
    Scene(const std::string&);
    ~Scene();

    entt::registry r;

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