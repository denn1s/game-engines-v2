#pragma once

#include <string>
#include <raylib.h>

struct NameComponent {
    std::string tag;
};

struct TransformComponent {
    Vector2 position;
};

struct VelocityComponent {
    Vector2 velocity;
};

struct SpriteComponent {
  std::string name;
  int xIndex = 0;
  int yIndex = 0;
  int size = 48;
  int animationFrames = 0;
  int animationDuration = 0;
  Texture2D texture;

  long lastUpdate = 0;
};

