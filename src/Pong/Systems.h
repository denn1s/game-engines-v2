#pragma once

#include "ECS/System.h"

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

class SpriteAnimationSystem : public System {
public:
    void update() override;
};

class PlayerActionSystem : public System {
public:
    void update() override;
};

class HelloSystem : public System {
public:
    void setup() override;
};

class InputSystem : public System {
public:
    void update() override;
};

class MovementSystem : public System {
public:
    void update() override;
};

class RenderSystem : public System {
public:
    void render() override;
};
