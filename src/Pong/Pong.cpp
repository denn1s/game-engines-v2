#include "Pong.h"
#include "Components.h"

#include "ECS/Entity.h"
#include "Systems.h"
#include "Game/Core/Systems/ImGuiSystem.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
    Scene* gameplayScene = createGameplayScene();
    setScene(gameplayScene);
}

Pong::~Pong() {
}

Scene* Pong::createGameplayScene() {
    Scene* gameplayScene = new Scene("Gameplay");

    // Create ball entity
    Entity ball = gameplayScene->createEntity("ball", 100, 100);
    ball.addComponent<SizeComponent>(30, 30);
    ball.addComponent<ColliderComponent>(false);
    ball.addComponent<VelocityComponent>(Vector2{100, 100});
    ball.addComponent<ColorComponent>(ORANGE);


    Entity ball2 = gameplayScene->createEntity("ball2", 200, 100);
    ball2.addComponent<SizeComponent>(30, 30);
    ball2.addComponent<ColliderComponent>(false);
    ball2.addComponent<VelocityComponent>(Vector2{-200, 200});
    ball2.addComponent<ColorComponent>(YELLOW);

    // Create paddle entity
    Entity paddle = gameplayScene->createEntity("paddle", (screen_width / 2) - 50, screen_height - 20);
    paddle.addComponent<SizeComponent>(100, 20);
    paddle.addComponent<PlayerComponent>(200);
    paddle.addComponent<VelocityComponent>(Vector2{0, 0});
    paddle.addComponent<ColorComponent>(GREEN);

    Entity white = gameplayScene->createEntity("cat1", 0, 0);
    white.addComponent<VelocityComponent>(Vector2{0, 0});
    auto& s = white.addComponent<SpriteComponent>();
    s.name = "assets/Sprites/Cat/SpriteSheet.png";
    s.xIndex = 0;
    s.yIndex = 0;
    s.size = 48;
    s.animationFrames = 8;
    s.animationDuration = 1000;
    s.lastUpdate = GetTime() * 1000;


    // Add systems
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new RenderSystem());
    gameplayScene->addSystem(new ImGuiSystem());
    gameplayScene->addSystem(new SpriteSetupSystem());
    gameplayScene->addSystem(new SpriteRenderSystem());
    gameplayScene->addSystem(new SpriteUpdateSystem());
    gameplayScene->addSystem(new SpriteAnimationSystem());


    return gameplayScene;
}
