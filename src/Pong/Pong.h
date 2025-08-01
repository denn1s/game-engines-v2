#pragma once

#include "Game/Game.h"

const int SCREEN_WIDTH = 1300;
const int SCREEN_HEIGHT = 800;

class Pong : public Game {
public:
    Pong();
    ~Pong();

private:
    Scene* createGameplayScene();
};
