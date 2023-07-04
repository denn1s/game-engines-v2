#include "Game/Game.h"

//Screen dimension constants
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;

int main( int argc, char* args[] )
{
    Game game = Game("Brand new game", SCREEN_WIDTH, SCREEN_HEIGHT);
    game.run();

    return 0;
}