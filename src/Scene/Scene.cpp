#include <string>
#include <glm/glm.hpp>
#include <SDL2/SDL.h>
#include "print.h"

#include "Scene.h"

#include "Entities.hpp"
#include "Components.hpp"
#include "System.h"


Scene::Scene(const std::string& name)
  : name(name)
{
  print("Scene ", name, "constructed!");
}

Scene::~Scene()
{
  print("Scene Destroyed!");
}

Entity Scene::createEntity(const std::string& name, int x, int y)
{
  Entity entity = { r.create(), this };
  entity.addComponent<NameComponent>(name);
  entity.addComponent<TransformComponent>(
    glm::vec2(x, y),
    glm::vec2(1, 1),
    0.0d
  );

  return entity;
}

void Scene::addSetupSystem(std::shared_ptr<SetupSystem> s)
{
  s->setScene(this);
  setupSystems.push_back(s);
}

void Scene::addEventSystem(std::shared_ptr<EventSystem> s)
{
  s->setScene(this);
  eventSystems.push_back(s);
}

void Scene::addUpdateSystem(std::shared_ptr<UpdateSystem> s)
{
  s->setScene(this);
  updateSystems.push_back(s);
}

void Scene::addRenderSystem(std::shared_ptr<RenderSystem> s)
{
  s->setScene(this);
  renderSystems.push_back(s);
}

void Scene::setup()
{
  print("Scene Setup");
  
  for (SetupSystem* sys: setupSystems)
  {
    sys->run();
  }
}

void Scene::update(double dT)
{
  print("Scene Update");
  
  for (UpdateSystem* sys: updateSystems)
  {
    sys->run(dT);
  }
}

void Scene::render(SDL_Renderer* renderer)
{
  print("Scene Render");
  
  for (RenderSystem* sys: renderSystems)
  {
    sys->run(renderer);
  }
}

void Scene::event(SDL_Event event)
{
  print("Scene Events");
  
  for (EventSystem* sys: eventSystems)
  {
    sys->run(event);
  }
}