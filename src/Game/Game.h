#include <raylib.h>
#include <print>

class Game {
public:
    Game();
    ~Game();

    void init(const char* title, int width, int height);
    void setup();
    void frameStart();
    void frameEnd();
    void handleEvents();
    void update();
    void render();
    void clean();
    bool running();

private:
    int counter = 0;
    bool isRunning = false;
    float dT = 0.0f; // Delta time in seconds
    float FPS = 0.0f;
};
