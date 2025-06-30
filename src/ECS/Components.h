#pragma once

#include <string>
#include <raylib.h>

struct NameComponent {
    std::string tag;
};

struct TransformComponent {
    Vector2 position;
    // Vector2 scale;
    // float rotation;
};

struct VelocityComponent {
    Vector2 velocity;
};
