#include "print.h"
#include "Pong.h"
#include "Systems.h"
#include "Components.h"
#include "UI/Systems.h"
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
    ball.addComponent<MovementComponent>(MovementComponent{glm::vec2(10, 100)});
    ball.addComponent<SizeComponent>(SizeComponent{glm::vec2(20, 20)});
 
    Entity ball2 = gameplayScene->createEntity("second ball", 300, 300);
    ball2.addComponent<MovementComponent>(MovementComponent{glm::vec2(5, 5)});
    ball2.addComponent<SizeComponent>(SizeComponent{glm::vec2(30, 30)});

    // Configure the gameplayScene object as needed
    std::shared_ptr<HelloSystem> helloSystem = std::make_shared<HelloSystem>();
    gameplayScene->addSetupSystem(helloSystem);

    std::shared_ptr<RectRenderSystem> rectSystem = std::make_shared<RectRenderSystem>();
    gameplayScene->addRenderSystem(rectSystem);

    std::shared_ptr<MovementUpdateSystem> movementSystem = std::make_shared<MovementUpdateSystem>(SCREEN_WIDTH, SCREEN_HEIGHT);
    gameplayScene->addUpdateSystem(movementSystem);

    std::shared_ptr<EngineUISystem> ui = std::make_shared<EngineUISystem>(renderer, window);
    gameplayScene->addSetupSystem(ui);
    gameplayScene->addEventSystem(ui);
    gameplayScene->addUpdateSystem(ui);
    gameplayScene->addRenderSystem(ui);

    return gameplayScene;
}

