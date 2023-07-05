#pragma once
#include "Engine/Game/Game.h"

const int SCREEN_WIDTH = 1200;
const int SCREEN_HEIGHT = 900;

class RPG : public Game {
  public:
    RPG();
    ~RPG();

  private:
    std::unique_ptr<Scene> createGameplayScene();
};
