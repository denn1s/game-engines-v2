#pragma once
#include "ECS/Components.h"
#include <raylib.h>

struct SizeComponent {
    float width, height;
};

struct PlayerComponent {
    float moveSpeed;
};

struct ColliderComponent {
    bool triggered = false;
};

struct ColorComponent {
    Color color;
};
