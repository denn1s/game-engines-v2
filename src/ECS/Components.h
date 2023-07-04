#pragma once

#include <glm/glm.hpp>
#include <string>

struct NameComponent {
  std::string tag;
};

struct TransformComponent {
  glm::vec2 position;
  glm::vec2 scale;
  double rotation;
};

struct MovementComponent {
  glm::vec2 velocity;
};
