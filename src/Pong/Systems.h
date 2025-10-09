#pragma once

#include "ECS/System.h"

class TilemapSetupSystem : public System {
public:
    void setup() override;
};

class TilemapRenderSystem : public System {
public:
    void render() override;
};

class MovementSystem : public System {
  public:
    void update() override;
};

class CollisionSystem : public System {
    public:
    void update() override;
};

class ColliderRenderSystem : public System {
    public:
    void render() override;
};

class IntGridRenderSystem : public System {
    public:
    void render() override;
};

class SpriteSetupSystem : public System {
  public:
    void setup() override;
    ~SpriteSetupSystem();
};

class SpriteRenderSystem : public System {
public:
    void render() override;
};

class SpriteUpdateSystem : public System {
public:
    void update() override;
};

class AnimationSystem : public System {
public:
    void update() override;
};

class PlayerActionSystem : public System {
public:
    void update() override;
};

class AutoTilingSetupSystem : public System {
  public:
    void setup() override;
};

class HelloSystem : public System {
public:
    void setup() override;
};

class InputSystem : public System {
public:
    void update() override;
};

class RenderSystem : public System {
public:
    void render() override;
};

class SolidCollisionSystem : public System {
public:
    void update() override;
};

class TreasureSystem : public System {
public:
    void update() override;
};

class TeleportSystem : public System {
public:
    void update() override;
};

class EnemyMovementSystem : public System {
public:
    void update() override;
};


