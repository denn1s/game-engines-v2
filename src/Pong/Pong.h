#pragma once
#include "Game/Game.h"


//Screen dimension constants
const int SCREEN_WIDTH = 800;
const int SCREEN_HEIGHT = 600;

class Pong : public Game {
  public:
    Pong();
    ~Pong();

  private:
    Scene* createGameplayScene();
};
