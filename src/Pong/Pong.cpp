#include "print.h"
#include "Pong.h"
#include "Systems.h"
#include "Components.h"
#include "ECS/Entity.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
    std::unique_ptr<Scene> gameplayScene = createGameplayScene();
    setScene(std::move(gameplayScene));
}

Pong::~Pong() {
    // destructor implementation
}

std::unique_ptr<Scene> Pong::createGameplayScene()
{
    // Create a unique_ptr to hold the created scene
    std::unique_ptr<Scene> gameplayScene = std::make_unique<Scene>("Gameplay");

    Entity ball = gameplayScene->createEntity("ball", 100, 100);
    ball.addComponent<SpeedComponent>(100, 100);
    ball.addComponent<SizeComponent>(20, 20);

    Entity paddle = gameplayScene->createEntity("paddle", (screen_width/2)-50, screen_height-20);
    paddle.addComponent<SpeedComponent>(0, 0);
    paddle.addComponent<SizeComponent>(100, 20);
    paddle.addComponent<PlayerComponent>(200);
 
    // Configure the gameplayScene object as needed
    std::shared_ptr<HelloSystem> helloSystem = std::make_shared<HelloSystem>();
    gameplayScene->addSetupSystem(helloSystem);

    std::shared_ptr<RectRenderSystem> rectSystem = std::make_shared<RectRenderSystem>();
    gameplayScene->addRenderSystem(rectSystem);

    std::shared_ptr<MovementUpdateSystem> movementSystem = std::make_shared<MovementUpdateSystem>(SCREEN_WIDTH, SCREEN_HEIGHT);
    gameplayScene->addUpdateSystem(movementSystem);

    std::shared_ptr<PlayerInputEventSystem> playerInputSystem = std::make_shared<PlayerInputEventSystem>();
    gameplayScene->addEventSystem(playerInputSystem);

    std::shared_ptr<CollisionUpdateSystem> collisionUpdateSystem = std::make_shared<CollisionUpdateSystem>();
    gameplayScene->addUpdateSystem(collisionUpdateSystem);

    return gameplayScene;
}

