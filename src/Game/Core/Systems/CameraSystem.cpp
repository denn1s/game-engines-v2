#include "CameraSystem.h"
#include "Game/Scene/Scene.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include "ECS/Entity.h"
#include <print>

void CameraSystem::setup() {
    auto world = scene->r.create();
    scene->r.emplace<WorldComponent>(world, 5120, 3840);
    scene->world = new Entity(world, scene);

    auto camera = scene->r.create();
    scene->r.emplace<CameraComponent>(camera, 1, 800, 600);
    scene->r.emplace<TransformComponent>(camera, 0, 0);
    scene->camera = new Entity(camera, scene);
}

void CameraSystem::update() {
    auto view = scene->r.view<PlayerComponent, TransformComponent, SpriteComponent>();
    
    for (auto entity : view) {
        auto& playerTransform = view.get<TransformComponent>(entity);
        auto& cameraTransform = scene->camera->get<TransformComponent>();
        auto& cameraComponent = scene->camera->get<CameraComponent>();
        auto& worldComponent = scene->world->get<WorldComponent>();
        auto& playerSprite = view.get<SpriteComponent>(entity);

        cameraTransform.position.x = playerTransform.position.x - (cameraComponent.vw / 2) + (playerSprite.size * 5 / 2);
        cameraTransform.position.y = playerTransform.position.y - (cameraComponent.vh / 2) + (playerSprite.size * 5 / 2);

        if (cameraTransform.position.x < 0) {
            cameraTransform.position.x = 0;
        }
        if (cameraTransform.position.y < 0) {
            cameraTransform.position.y = 0;
        }
        if (cameraTransform.position.x > worldComponent.width - cameraComponent.vw) {
            cameraTransform.position.x = worldComponent.width - cameraComponent.vw;
        }
        if (cameraTransform.position.y > worldComponent.height - cameraComponent.vh) {
            cameraTransform.position.y = worldComponent.height - cameraComponent.vh;
        }
    }
}
