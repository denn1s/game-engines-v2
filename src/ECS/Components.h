#pragma once

#include <string>
#include <vector>
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

struct TileComponent {
    int x;
    int y;
    Texture2D upTexture;
    Texture2D downTexture;
    float scale = 5.0f;
    bool needsAutoTiling = false;
    int tileX = 0;
    int tileY = 0;
};

struct TilemapComponent {
  std::vector<TileComponent> tiles;
  int width;
  int height;
  int tileSize;
};

