#include <iostream>
#include <SDL2/SDL.h>
#include "print.h"
#include "Game.h"

SDL_Rect ball;
SDL_Rect paddle;

Game::Game(const char* title, int width, int height)
{
  int maxFPS = 60;
  frameDuration = (1.0f / maxFPS) * 1000.0f;  // how many mili seconds in one frame

  // initial frame count variables
  frameCount = 0;
  lastFPSUpdateTime = 0;
  FPS = 0;

  SDL_Init(SDL_INIT_EVERYTHING);
  
  window = SDL_CreateWindow(title, 0, 0, width, height, 0);
  renderer = SDL_CreateRenderer(window, -1, 0);
  
  SDL_SetRenderDrawColor(renderer, 200, 255, 255, 1);
  print("Game Start!");

  screen_width = width;
  screen_height = height;

  isRunning = true;
}

Game::~Game()
{}

void Game::frameStart()
{
  print("---- Frame: ", frameCount, " ----");
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

    currentScene->event(event);
  }
}

void Game::update()
{
  print("Game Updating...");

  currentScene->update(dT);
}

void Game::render()
{
  print("Game Rendering...");

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);
  
  currentScene->render(renderer);

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

void Game::run()
{
    setup();

    while (running())
    {
      frameStart();
      handleEvents();
      update();
      render();
      frameEnd();
    }

    clean();
}

void Game::setScene(std::unique_ptr<Scene> newScene) {
  currentScene = std::move(newScene);
}

Scene* Game::getCurrentScene() const {
  return currentScene.get();
}