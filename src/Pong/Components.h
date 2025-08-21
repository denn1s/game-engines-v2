#pragma once
#include "ECS/Components.h"
#include <raylib.h>

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
    bool triggered = false;
};

struct ColorComponent {
    Color color;
};
