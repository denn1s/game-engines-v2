#include <iostream>
#include <SDL2/SDL.h>
#include "../print.h"
#include "Game.h"

int screen_width;
int screen_height;

SDL_Rect ball;
SDL_Rect paddle;


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

    screen_width = width;
    screen_height = height;

    isRunning = true;
  } else {
    isRunning = false;
  }
  counter = 0;
}


void Game::setup()
{
  ball.x = 20;
  ball.y = 20;
  ball.w = 15;
  ball.h = 15;

  paddle.x = (screen_width / 2) - 50;
  paddle.y = screen_height - 20;
  paddle.w = 100;
  paddle.h = 20;
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

    if (event.type == SDL_KEYDOWN)
    {
      switch (event.key.keysym.sym)
      {
        case SDLK_LEFT:
          paddle.x -= 10;
          break;
        case SDLK_RIGHT:
          paddle.x += 10;
          break;
      }
    }
  }
}

int sx = 2;
int sy = 2;

void Game::update()
{
  print("Game Updating...");

  // collisions 
  if (ball.x <= 0)
  {
    sx *= -1;
  }

  if (ball.x + ball.w >= screen_width)
  {
    sx *= -1;
  }

  if (ball.y <= 0)
  {
    sy *= -1;
  }

  if (ball.y + ball.h >= screen_height)
  {
    isRunning = false;
  }

  if (ball.y + ball.h >= paddle.y &&
      ball.x + ball.w >= paddle.x &&
      ball.x <= paddle.x + paddle.w)
  {
    sy *= -1.1;
    sx *= 1.1;
  }

  ball.x += sx;
  ball.y += sy;
}

void Game::render()
{
  print("Game Rendering...");

  SDL_SetRenderDrawColor(renderer, 0, 0, 0, 1);
  SDL_RenderClear(renderer);
  // actually render stuff
  SDL_SetRenderDrawColor(renderer, 255, 255 ,255, 1);
  SDL_RenderFillRect(renderer, &ball);
  SDL_RenderFillRect(renderer, &paddle);


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
