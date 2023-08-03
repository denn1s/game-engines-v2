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
    ball.addComponent<SpriteComponent>("sprites/2.png");

    Entity ball2 = gameplayScene->createEntity("ball2", 300, 100);
    ball2.addComponent<SpriteComponent>(
        "sprites/2.png", 
        PixelShader{ [](Uint32 pixel) -> Uint32 { return (pixel == 0xF3F2C0) ? 0xD2B48C : pixel; }, "sampleShader" }
    );

    Entity paddle = gameplayScene->createEntity("paddle", (screen_width/2)-50, screen_height-20);
    paddle.addComponent<SpeedComponent>(0, 0);
    paddle.addComponent<SizeComponent>(100, 20);
    paddle.addComponent<PlayerComponent>(200);

    gameplayScene->addSetupSystem<HelloSystem>();
    gameplayScene->addRenderSystem<RectRenderSystem>();
    gameplayScene->addUpdateSystem<MovementUpdateSystem>(SCREEN_WIDTH, SCREEN_HEIGHT);
    gameplayScene->addEventSystem<PlayerInputEventSystem>();
    gameplayScene->addUpdateSystem<CollisionDetectionUpdateSystem>();
    gameplayScene->addUpdateSystem<BounceUpdateSystem>();

    gameplayScene->addSetupSystem<SimpleSpriteSetupSystem>(renderer, window);
    gameplayScene->addRenderSystem<SimpleSpriteRenderSystem>();

    return gameplayScene;
}

