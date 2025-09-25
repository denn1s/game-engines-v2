#pragma once
#include "ECS/Components.h"
#include <raylib.h>
#include <vector>

enum ToolType {
    NONE,
    SHOVEL,
    AXE,
    WATER_CAN
};

struct SizeComponent {
    float width, height;
};

struct PlayerComponent {
    bool isRunning = false;
    bool isAttacking = false;
    ToolType currentTool = NONE;
    Vector2 lastDirection = {0, 1}; // Default to facing down
};

struct ColliderComponent {
    float width;
    float height;
    float offsetX;
    float offsetY;
    bool triggered = false;

    enum TriggerType {
        NONE,
        SOLID,
        TREASURE,
        TELEPORT
    };

    TriggerType triggerType = NONE;

    union {
        struct {
            int x, y;
        } teleport;
    } data;
};

struct ColorComponent {
    Color color;
};

struct IntGridComponent {
    std::vector<int> grid;
    int width, height;
};
