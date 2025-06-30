#include "Pong.h"
#include "Components.h"
#include "ECS/Entity.h"
#include "Systems.cpp"

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
    ball.addComponent<TransformComponent>(Vector2{100, 100});
    ball.addComponent<VelocityComponent>(Vector2{200, 200});
    ball.addComponent<SizeComponent>(30, 30);
    ball.addComponent<ColliderComponent>(false);

    // Create paddle entity
    Entity paddle = gameplayScene->createEntity("paddle", (screen_width / 2) - 50, screen_height - 20);
    paddle.addComponent<TransformComponent>(Vector2{static_cast<float>((screen_width / 2.0) - 50), static_cast<float>(screen_height - 20)});
    paddle.addComponent<VelocityComponent>(Vector2{0, 0});
    paddle.addComponent<SizeComponent>(100, 20);
    paddle.addComponent<PlayerComponent>(200);

    // Add systems
    gameplayScene->addSystem(new HelloSystem());
    gameplayScene->addSystem(new InputSystem());
    gameplayScene->addSystem(new MovementSystem());
    gameplayScene->addSystem(new CollisionSystem());
    gameplayScene->addSystem(new RenderSystem());

    return gameplayScene;
}
