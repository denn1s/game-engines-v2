#include "Scene.h"
#include "ECS/Entity.h"
#include "ECS/System.h"
#include <print>

Scene::Scene(const std::string& name)
    : name(name)
{
    std::println("Scene {} constructed!", name);
}

Scene::~Scene()
{
    std::println("Scene Destroyed!");
    // Do not delete systems; user manages their lifetime
}

Entity Scene::createEntity(const std::string& name, int x, int y)
{
    Entity entity = { r.create(), this };
    entity.addComponent<NameComponent>(name);
    entity.addComponent<TransformComponent>(Vector2(x, y));
    return entity;
}

void Scene::addSystem(System* system)
{
    systems.push_back(system);
    system->setScene(this);
}

void Scene::setup()
{
    std::println("Scene Setup");
    for (auto sys : systems) {
        sys->setup();
    }
}

void Scene::update()
{
    std::println("Scene Update");
    for (auto sys : systems) {
        sys->update();
    }
}

void Scene::render()
{
    std::println("Scene Render");
    for (auto sys : systems) {
        sys->render();
    }
}
