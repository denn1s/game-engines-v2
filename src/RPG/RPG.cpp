#include "RPG.h"

#include "Engine/UI/Systems.h"
#include "Engine/ECS/Entity.h"
#include "Systems.h"
#include "Components.h"

RPG::RPG() : Game("RPG", SCREEN_WIDTH, SCREEN_HEIGHT) {
    std::unique_ptr<Scene> gameplayScene = createGameplayScene();
    setScene(std::move(gameplayScene));
}

RPG::~RPG() {
    // destructor implementation
}

std::unique_ptr<Scene> RPG::createGameplayScene()
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

    return gameplayScene;
}

