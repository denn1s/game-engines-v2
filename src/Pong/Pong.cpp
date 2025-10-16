#include "Pong.h"
#include "Components.h"

#include "ECS/Entity.h"
#include "Systems.h"
#include "Game/Core/Systems/ImGuiSystem.h"
#include "Game/Core/Systems/CameraSystem.h"
#include "Game/Core/Systems/CameraZoomSystem.h"
#include "Game/Core/Systems/ScriptingSystem.h"

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

    // Add HP component to player
    auto& hp = white.addComponent<HPComponent>();
    hp.maxHP = 100;
    hp.currentHP = 100;

    // Add damage cooldown component
    auto& cooldown = white.addComponent<DamageCooldownComponent>();
    cooldown.cooldownDuration = 1.0f;

    // Add systems
    gameplayScene->addSystem(new ScriptingSystem("assets/Scripts/Enemy/OneTimeSpawn.lua"));
    gameplayScene->addSystem(new CameraSystem());
    gameplayScene->addSystem(new TilemapSetupSystem());
    gameplayScene->addSystem(new AutoTilingSetupSystem());
    gameplayScene->addSystem(new TilemapRenderSystem());
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new SolidCollisionSystem());
    gameplayScene->addSystem(new TreasureSystem());
    gameplayScene->addSystem(new TeleportSystem());
    gameplayScene->addSystem(new PlayerActionSystem());
    gameplayScene->addSystem(new EnemyMovementSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new PlayerEnemyCollisionSystem());  // Check player-enemy collisions
    gameplayScene->addSystem(new DeathSystem());                 // Remove dead entities
    gameplayScene->addSystem(new RenderSystem());
    gameplayScene->addSystem(new ImGuiSystem());
    gameplayScene->addSystem(new SpriteSetupSystem());
    gameplayScene->addSystem(new SpriteRenderSystem());
    gameplayScene->addSystem(new HPRenderSystem());              // Render health bars
    gameplayScene->addSystem(new ColliderRenderSystem());
    gameplayScene->addSystem(new IntGridRenderSystem());
    gameplayScene->addSystem(new SpriteUpdateSystem());
    gameplayScene->addSystem(new AnimationSystem());
    gameplayScene->addSystem(new CameraZoomSystem());


    return gameplayScene;
}
