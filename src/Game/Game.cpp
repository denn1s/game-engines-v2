#include "Game.h"
#include <print>
#include <format>
#include "Game/Scene/Scene.h"
#include "ECS/Components.h"

Game::Game(const char* title, int width, int height)
    : screen_width(width), screen_height(height)
{
    InitWindow(width, height, title);
    SetTargetFPS(60);
    std::println("Game Start!");
    isRunning = true;
    frameCount = 0;
    dT = 0.0f;
    FPS = 0.0f;

    camera.target = { 0.0f, 0.0f };
    camera.offset = { (float)screen_width / 2.0f, (float)screen_height / 2.0f };
    camera.rotation = 0.0f;
    camera.zoom = 1.0f;
}

Game::~Game() {
    clean();
}

void Game::setup() {
    if (currentScene) {
        currentScene->setup();
    }
}

void Game::frameStart() {
    std::println("---- Frame: {} ----", frameCount);
    dT = GetFrameTime(); // seconds
}

void Game::frameEnd() {
    frameCount++;
    FPS = static_cast<float>(GetFPS());
    std::println();
}

void Game::handleEvents() {
    if (WindowShouldClose()) {
        isRunning = false;
    }
}

void Game::update() {
    if (currentScene) {
        currentScene->update();

        auto cameraView = currentScene->r.view<CameraComponent, TransformComponent>();
        for (auto entity : cameraView) {
            auto& cam = cameraView.get<CameraComponent>(entity);
            auto& trans = cameraView.get<TransformComponent>(entity);
            camera.target = trans.position;
            camera.zoom = cam.zoom;
        }
    }
}

void Game::render() {
    BeginDrawing();
    ClearBackground(BLACK);

    if (currentScene) {
        BeginMode2D(camera);
        currentScene->render();
        EndMode2D();
    }

    DrawText(std::format("FPS: {:.2f}", FPS).c_str(), 10, 10, 20, DARKGRAY);

    EndDrawing();
}

void Game::clean() {
    if (!WindowShouldClose()) {
        CloseWindow();
    }
    std::println("Game Over.");
}

bool Game::running() const {
    return isRunning;
}

void Game::run() {
    setup();

    while (running()) {
        frameStart();
        handleEvents();
        update();
        render();
        frameEnd();
    }

    clean();
}

void Game::setScene(Scene* newScene) {
    currentScene = std::move(newScene);
}

Scene* Game::getCurrentScene() const {
    return currentScene;
}
