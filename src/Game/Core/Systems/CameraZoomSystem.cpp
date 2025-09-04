#include "CameraZoomSystem.h"
#include "Game/Scene/Scene.h"
#include "ECS/Components.h"
#include "ECS/Entity.h"
#include <raylib.h>

void CameraZoomSystem::update() {
    if (scene->camera == nullptr) {
        return;
    }

    auto& cameraComponent = scene->camera->get<CameraComponent>();

    if (IsKeyDown(KEY_Q)) {
        cameraComponent.zoom += 0.1f;
    }
    if (IsKeyDown(KEY_E)) {
        cameraComponent.zoom -= 0.1f;
    }

    if (cameraComponent.zoom < 0.1f) {
        cameraComponent.zoom = 0.1f;
    }
}
