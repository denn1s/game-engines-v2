#include "ScriptingSystem.h"
#include "Game/Scene/Scene.h"
#include "ECS/Components.h"
#include "Pong/Components.h"
#include "ECS/Entity.h"
#include "Game/Graphics/TextureManager.h"
#include <random>

ScriptingSystem::ScriptingSystem(const std::string& script_path) : script_path(script_path) {
    lua.open_libraries(sol::lib::base, sol::lib::math);
}

void ScriptingSystem::update() {
    total_time += GetFrameTime();

    // Get player position
    auto view = scene->r.view<PlayerComponent, TransformComponent>();
    for (auto entity : view) {
        auto& playerTransform = view.get<TransformComponent>(entity);

        // Create input table
        sol::table input = lua.create_table();
        input["player_x"] = playerTransform.position.x;
        input["player_y"] = playerTransform.position.y;
        input["delta_time"] = GetFrameTime();
        input["total_time"] = total_time;

        // Add random number to input
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0.0, 1.0);
        input["rng"] = dis(gen);

        lua["input"] = input;

        // Run the script
        lua.script_file(script_path);

        // Get output table
        sol::table output = lua["output"];
        if (output) {
            std::string type = output["type"];
            if (type != "NONE") {
                float x = output["x"];
                float y = output["y"];

                // Spawn enemy
                Entity enemy = scene->createEntity("enemy", x, y);
                enemy.addComponent<VelocityComponent>(Vector2{0, 0});
                enemy.addComponent<EnemyComponent>();

                // Add movement component with script path if provided
                if (output["movement_script"].valid()) {
                    std::string movement_script = output["movement_script"];
                    auto& movement = enemy.addComponent<EnemyMovementComponent>();
                    movement.movementScript = movement_script;
                }

                auto& s = enemy.addComponent<SpriteComponent>();
                s.name = "assets/Sprites/EvilCat/1.png";
                s.texture = TextureManager::LoadTexture(s.name);
                s.xIndex = 0;
                s.yIndex = 0;
                s.size = 48;
                s.animationFrames = 8;
                s.animationDuration = 1000;
                s.lastUpdate = GetTime() * 1000;

                // Add collider for enemy (similar to player)
                auto& collider = enemy.addComponent<ColliderComponent>();
                collider.width = 20;
                collider.height = 20;
                collider.offsetX = 14;
                collider.offsetY = 20;

                // Add HP component
                auto& hp = enemy.addComponent<HPComponent>();
                hp.maxHP = 30;
                hp.currentHP = 30;
            }
        }
    }
}
