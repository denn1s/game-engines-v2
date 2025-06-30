#include "Game.h"
#include <print>
#include <format>

Rectangle ball;
Rectangle paddle;

// Ball speed
float sx = 2.0f;
float sy = 2.0f;

Game::Game(const char* title, int width, int height) {
    InitWindow(width, height, title);
    SetTargetFPS(60);
    std::println("Game Start!");
    isRunning = true;
    screen_width = width;
    screen_height = height;
    counter = 0;
}

void Game::setup() {
    ball = Rectangle{20, 20, 15, 15};
    paddle = Rectangle{
        static_cast<float>(screen_width) / 2.0f - 50.0f,
        static_cast<float>(screen_height) - 20.0f,
        100.0f,
        20.0f
    };
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

    // Paddle movement
    if (IsKeyDown(KEY_LEFT)) {
        paddle.x -= 10.0f;
    }
    if (IsKeyDown(KEY_RIGHT)) {
        paddle.x += 10.0f;
    }
    // Clamp paddle to screen
    if (paddle.x < 0) paddle.x = 0;
    if (paddle.x + paddle.width > screen_width)
        paddle.x = screen_width - paddle.width;
}

void Game::update() {
    std::println("Game Updating...");

    // Ball collision with walls
    if (ball.x <= 0) sx *= -1.0f;
    if (ball.x + ball.width >= screen_width) sx *= -1.0f;
    if (ball.y <= 0) sy *= -1.0f;

    // Ball falls below screen
    if (ball.y + ball.height >= screen_height) {
        isRunning = false;
    }

    // Ball collision with paddle
    if (ball.y + ball.height >= paddle.y &&
        ball.x + ball.width >= paddle.x &&
        ball.x <= paddle.x + paddle.width) {
        sy *= -1.1f;
        sx *= 1.1f;
    }

    // Move ball
    ball.x += sx;
    ball.y += sy;
}

void Game::render() {
    std::println("Game Rendering...");
    BeginDrawing();
    ClearBackground(BLACK);

    // Draw ball and paddle
    DrawRectangleRec(ball, WHITE);
    DrawRectangleRec(paddle, WHITE);

    // Draw FPS
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
