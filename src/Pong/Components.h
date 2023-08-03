#include "ECS/Components.h"
#include "Game/Graphics/Texture.h"

struct SizeComponent {
  int w;
  int h;
};

struct ColliderComponent {
  bool triggered;
};

struct PlayerComponent {
  short moveSpeed;
};

struct SpriteComponent {
  std::string textureName;
};