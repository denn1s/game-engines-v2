#include <print.h>
#include "Game/Graphics/PixelShader.h"
#include "Pong.h"
#include "Systems.h"
#include "Components.h"

#include "ECS/Entity.h"

Pong::Pong() : Game("Pong", SCREEN_WIDTH, SCREEN_HEIGHT) {
  Scene* gameplayScene = createGameplayScene();
  setScene(gameplayScene);
}

Pong::~Pong() {
    // destructor implementation
}

Scene* Pong::createGameplayScene()
{
  Scene* scene = new Scene("GAMEPLAY SCENE");

  scene->player->addComponent<SpriteComponent>(
    "Sprites/Cat/SpriteSheet.png",
    0, 0,
    48,
    8,
    1000,
    PixelShader{nullptr, ""},
    SDL_GetTicks()
  );
  scene->addEventSystem<PlayerInputEventSystem>();
  scene->addUpdateSystem<PlayerSpriteUpdateSystem>();
  scene->addUpdateSystem<TileCollisionUpdateSystem>();
  scene->addUpdateSystem<MovementUpdateSystem>();
  scene->addUpdateSystem<CameraFollowUpdateSystem>();

  scene->addSetupSystem<TilemapSetupSystem>(renderer);
  scene->addSetupSystem<AutoTilingSetupSystem>();
  scene->addRenderSystem<TilemapRenderSystem>();

  scene->addSetupSystem<EnemySpawnSystem>();
  scene->addSetupSystem<SpriteSetupSystem>(renderer);
  scene->addRenderSystem<SpriteRenderSystem>();
  scene->addUpdateSystem<SpriteUpdateSystem>();
  scene->addRenderSystem<BoxColliderRenderSystem>();

  scene->addUpdateSystem<EnemyMoveSystem>();
  /* scene->addRenderSystem<TileColliderRenderSystem>(); */
  return scene;
}

