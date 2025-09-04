#pragma once

#include <raylib.h>
#include "Game/Scene/Scene.h"

class Game {
public:
    Game(const char* title, int width, int height);
    ~Game();

    void setup();
    void frameStart();
    void frameEnd();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running() const;

    void run();

    void setScene(Scene* newScene);
    Scene* getCurrentScene() const;

protected:
    bool isRunning = false;
    int screen_width = 0;
    int screen_height = 0;

    float dT = 0.0f;
    float FPS = 0.0f;
    int frameCount = 0;

    Scene* currentScene;
};
