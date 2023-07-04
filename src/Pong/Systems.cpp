#include "print.h"
#include "Systems.h"
#include "Components.h"

HelloSystem::HelloSystem() {
    std::cout << "Hello System Constructor" << std::endl;
}

HelloSystem::~HelloSystem() {
    std::cout << "Hello System Destructor" << std::endl;
}

void HelloSystem::run() {
    std::cout << "Hello System!" << std::endl;
}

void RectRenderSystem::run(SDL_Renderer* renderer) {
  SDL_SetRenderDrawColor(renderer, 255, 100, 100, 1);

  const auto view = scene->r.view<TransformComponent, SizeComponent>();
  for (const entt::entity e : view) {
    const TransformComponent& t = view.get<TransformComponent>(e);
    const SizeComponent& c = view.get<SizeComponent>(e);
    const int x = t.position.x;
    const int y = t.position.y;
    const int w = c.size.x;
    const int h = c.size.y;

    SDL_Rect rect = { x, y, w, h };    
    SDL_RenderFillRect(renderer, &rect);
  }
}

MovementUpdateSystem::MovementUpdateSystem(int screen_width, int screen_height)
  : screen_width(screen_width), screen_height(screen_height) { }

void MovementUpdateSystem::run(double dT) {
  const auto view = scene->r.view<TransformComponent, MovementComponent>();
  for (const entt::entity e : view) {
    TransformComponent& t = view.get<TransformComponent>(e);
    MovementComponent& m = view.get<MovementComponent>(e);

    if (t.position.x <= 0)
    {
      m.velocity.x *= -1;
    }

    if (t.position.x >= screen_width)
    {
      m.velocity.x *= -1;
    }

    if (t.position.y <= 0)
    {
      m.velocity.y *= -1;
    }

    if (t.position.y >= screen_height)
    {
      m.velocity.y *= -1;
    }
  
    t.position.x += m.velocity.x * dT;
    t.position.y += m.velocity.y * dT;
  }
}