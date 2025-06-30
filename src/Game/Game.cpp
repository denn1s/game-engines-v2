#include "Game.h"
#include <print>

Game::Game(const char* title, int width, int height) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
    std::println("Game Start!");
    isRunning = true;
    counter = 0;
}

void Game::setup() {
    // Any setup logic here
}

void Game::frameStart() {
    std::println("---- Frame: {} ----", counter);
    dT = GetFrameTime(); // seconds
}

void Game::frameEnd() {
    counter++;
    FPS = static_cast<float>(GetFPS());
    std::println();
}

void Game::handleEvents() {
    std::println("Game Handling events...");
    if (WindowShouldClose()) {
        isRunning = false;
    }
}

void Game::update() {
    std::println("Game Updating...");
    // Game logic here
}

void Game::render() {
    std::println("Game Rendering...");
    BeginDrawing();
    ClearBackground(RAYWHITE);
    // Draw your game here
    DrawText(std::format("FPS: {:.2f}", FPS).c_str(), 10, 10, 20, DARKGRAY);
    EndDrawing();
}

void Game::clean() {
    CloseWindow();
    std::println("Game Over.");
}

bool Game::running() {
    return isRunning;
}
