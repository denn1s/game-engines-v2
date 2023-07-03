# Game Loop Tutorial

This tutorial explains the main game loop of a simple game using the following code as an example:

```cpp
#include "Game/Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

Game *game = NULL;

int main( int argc, char* args[] )
{
    game = new Game();

    game->init("Brand new game", SCREEN_WIDTH, SCREEN_HEIGHT);
    game->setup();

    while (game->running())
    {
      game->frameStart();
      game->handleEvents();
      game->update();
      game->render();
      game->frameEnd();
    }

    game->clean();

    return 0;
}
```

## Game Loop Stages
The game loop is the core of any game, and it is responsible for constantly updating the game state and rendering the screen. The main stages of the game loop are:

1. Initialization: This stage sets up the game window and initializes the game's core systems. In our example, this is done with the game->init() and game->setup() calls:
  * game->init(): Initializes the game window with the specified title, width, and height.
  * game->setup(): Sets up game-specific objects, such as loading textures or initializing game entities.

2. Game Loop: This is the main loop that runs continuously while the game is running. It consists of several sub-stages:
  * game->frameStart(): Marks the beginning of a new frame. This is typically used to measure the time since the previous frame, which can be helpful for ensuring consistent game speed across different devices.
  * game->handleEvents(): Processes user input and other events, such as keyboard or mouse events, and updates the game state accordingly.
  * game->update(): Updates the game's internal state, such as moving game entities or detecting collisions. This stage is where the game's logic is implemented.
  * game->render(): Draws the game objects on the screen. This stage is responsible for rendering the game's visuals.
  * game->frameEnd(): Marks the end of the current frame. This can be used to perform cleanup tasks, limit the frame rate, or swap buffers for double buffering.

3. Cleanup: This stage is responsible for releasing any resources used by the game and shutting down the game's systems. In our example, this is done with the game->clean() call:
  * game->clean(): Releases resources, such as textures or fonts, and shuts down any systems that were initialized during the setup stage.

Each of these stages should be tailored to your specific game's requirements, and additional stages can be added as needed. Understanding these stages and their roles will help you create a solid foundation for building your game.