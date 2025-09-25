#include "Pong.h"
#include "Components.h"

#include "ECS/Entity.h"
#include "Systems.h"
#include "Game/Core/Systems/ImGuiSystem.h"
#include "Game/Core/Systems/CameraSystem.h"
#include "Game/Core/Systems/CameraZoomSystem.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createGameplayScene();
    setScene(gameplayScene);
}


Pong::~Pong() {
}

Scene* Pong::createGameplayScene() {
    Scene* gameplayScene = new Scene("Gameplay");

    Entity white = gameplayScene->createEntity("cat1", 0, 0);
    white.addComponent<VelocityComponent>(Vector2{0, 0});
    white.addComponent<PlayerComponent>();
    auto& s = white.addComponent<SpriteComponent>();
    s.name = "assets/Sprites/Cat/SpriteSheet.png";
    s.xIndex = 0;
    s.yIndex = 0;
    s.size = 48;
    s.animationFrames = 8;
    s.animationDuration = 1000;
    s.lastUpdate = GetTime() * 1000;
    auto& collider = white.addComponent<ColliderComponent>();
    collider.width = 20;
    collider.height = 20;
    collider.offsetX = 14;
    collider.offsetY = 20;

    // Add systems
    gameplayScene->addSystem(new CameraSystem());
    gameplayScene->addSystem(new TilemapSetupSystem());
    gameplayScene->addSystem(new AutoTilingSetupSystem());
    gameplayScene->addSystem(new TilemapRenderSystem());
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new PlayerActionSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new RenderSystem());
    gameplayScene->addSystem(new ImGuiSystem());
    gameplayScene->addSystem(new SpriteSetupSystem());
    gameplayScene->addSystem(new SpriteRenderSystem());
    gameplayScene->addSystem(new ColliderRenderSystem());
    gameplayScene->addSystem(new IntGridRenderSystem());
    gameplayScene->addSystem(new SpriteUpdateSystem());
    gameplayScene->addSystem(new SpriteAnimationSystem());
    gameplayScene->addSystem(new CameraZoomSystem());


    return gameplayScene;
}
