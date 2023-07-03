#include <iostream>
#include <SDL2/SDL.h>
#include "../print.h"
#include "Game.h"

Game::Game()
{
  int maxFPS = 60;
  frameDuration = (1.0f / maxFPS) * 1000.0f;  // how many mili seconds in one frame

  // initial frame count variables
  frameCount = 0;
  lastFPSUpdateTime = 0;
  FPS = 0;
}

Game::~Game()
{}

void Game::init(const char* title, int width, int height)
{
  if(SDL_Init(SDL_INIT_EVERYTHING) == 0)
  {
    window = SDL_CreateWindow(title, 0, 0, width, height, 0);
    renderer = SDL_CreateRenderer(window, -1, 0);
    SDL_SetRenderDrawColor(renderer, 200, 255, 255, 1);
    print("Game Start!");

    isRunning = true;
  } else {
    isRunning = false;
  }
  counter = 0;
}


void Game::setup()
{
}

void Game::frameStart()
{
  print("---- Frame: ", counter, " ----");
  frameStartTimestamp = SDL_GetTicks();
  dT = frameEndTimestamp - frameStartTimestamp;
}

void Game::frameEnd()
{
  frameEndTimestamp = SDL_GetTicks();

  float actualFrameDuration = frameEndTimestamp - frameStartTimestamp;

  if (actualFrameDuration < frameDuration)
  {
    SDL_Delay(frameDuration - actualFrameDuration);
  }

  counter++;
  
  frameCount++;
  // Update FPS counter every second
  Uint32 currentTime = SDL_GetTicks();
  if (currentTime - lastFPSUpdateTime > 1000) // 1000 milliseconds in 1 second
  {
    FPS = frameCount / ((currentTime - lastFPSUpdateTime) / 1000.0f);
    lastFPSUpdateTime = currentTime;
    frameCount = 0;
  }

  print();
}

void Game::handleEvents()
{
  print("Game Handling events...");

  SDL_Event event;
  while (SDL_PollEvent(&event) != 0)
  {
    if (event.type == SDL_QUIT)
    {
      isRunning = false;
    }
  }
}

void Game::update()
{
  print("Game Updating...");

}

void Game::render()
{
  print("Game Rendering...");

  SDL_RenderClear(renderer);
  // actually render stuff
  SDL_RenderPresent(renderer);

  vprint(FPS);
}

void Game::clean()
{
  SDL_DestroyWindow(window);
  SDL_DestroyRenderer(renderer);
  SDL_Quit();
  print("Game Over.");
}

bool Game::running()
{
  return isRunning;
}
