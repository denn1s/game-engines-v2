#include "EnemySystems.h"
#include "Game/Scene/Scene.h"
#include "ECS/Entity.h"
#include "Components.h"

void EnemySpawnSystem::setup() {
    Scene* scene = this->scene;
    Entity enemy = scene->createEntity("enemy", 100, 100);
    enemy.addComponent<VelocityComponent>(Vector2{0, 0});
    enemy.addComponent<EnemyComponent>();
    auto& s = enemy.addComponent<SpriteComponent>();
    s.name = "assets/Sprites/EvilCat/1.png";
    s.xIndex = 0;
    s.yIndex = 0;
    s.size = 48;
    s.animationFrames = 8;
    s.animationDuration = 1000;
    s.lastUpdate = GetTime() * 1000;
}
