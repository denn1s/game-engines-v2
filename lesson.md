# Lesson: 2D Cameras in Game Engines

**Objective:** To understand and implement a 2D camera system from first principles. This will allow us to create game worlds that are larger than the screen and control what the player sees by manually transforming the world's coordinates at render time.

**Prerequisites:** Students should be familiar with the existing ECS architecture of the engine.

---

## 1. The Concept of a Virtual Camera

### Summary

Think of a camera in a 2D game as a virtual window into your game world. If your world is a huge painting, the camera defines the frame you are looking through. The core illusion of a 2D camera is simple but powerful: **to make it look like the camera is moving right, we actually move the entire world left.**

Instead of using a built-in camera object, we will create our own "camera" by designating an entity to represent its position. The rendering systems will then use this camera entity's position to offset the drawing of every other object in the world, creating the camera effect.

This lesson will walk through:
1.  Creating the new components needed to define our camera and world boundaries.
2.  Building a `CameraSystem` to manage the camera's position.
3.  Modifying our rendering systems to use the camera's position.

---

## 2. New Components: `CameraComponent` and `WorldComponent`

### Summary

To manage our camera, we need two new components.
-   `WorldComponent`: This will be attached to a single entity representing the game world. It simply holds the total width and height of our level, defining the boundaries the camera cannot cross.
-   `CameraComponent`: This will be attached to our camera entity. It holds the camera's viewport dimensions (how much of the world it can see at once) and a zoom level.

### Code: `Components.h`

**File:** `src/ECS/Components.h`
```diff
 struct TilemapComponent {
     // ...
 };
 
+struct CameraComponent {
+  int zoom = 1;
+  int vw = 0; // Viewport Width
+  int vh = 0; // Viewport Height
+};
+
+struct WorldComponent {
+  int width = 0;
+  int height = 0;
+};
```

---

## 3. The `CameraSystem`

### Summary

The `CameraSystem` is the brain of our camera's logic. It doesn't render anything. Its sole job is to update the `TransformComponent` of our camera entity each frame.

Its responsibilities are:
1.  **Setup**: Create the `world` and `camera` entities and attach their respective components.
2.  **Update**:
    -   Find the player entity.
    -   Center the camera's position on the player.
    -   **Clamp** the camera's position to the boundaries defined by the `WorldComponent`, ensuring we can't scroll past the edge of the map.

### Code: `CameraSystem.h` (New File)

**File:** `src/Game/Core/Systems/CameraSystem.h`
```cpp
#pragma once

#include "ECS/System.h"

class CameraSystem : public System {
public:
    CameraSystem() = default;
    ~CameraSystem() = default;

    void setup() override;
    void update() override;
};
```

### Code: `CameraSystem.cpp` (New File)

**File:** `src/Game/Core/Systems/CameraSystem.cpp`
```cpp
#include "CameraSystem.h"
#include "Game/Scene/Scene.h"
#include "ECS/Components.h"
#include "Pong/Components.h" // For PlayerComponent
#include "ECS/Entity.h"

void CameraSystem::setup() {
    // Create an entity to hold the world dimensions
    auto world = scene->r.create();
    scene->r.emplace<WorldComponent>(world, 5120, 3840); // Example world size
    scene->world = new Entity(world, scene);

    // Create the camera entity
    auto camera = scene->r.create();
    scene->r.emplace<CameraComponent>(camera, 1, 800, 600); // Viewport matches screen
    scene->r.emplace<TransformComponent>(camera, 0, 0);
    scene->camera = new Entity(camera, scene);
}

void CameraSystem::update() {
    // Find the player
    auto view = scene->r.view<PlayerComponent, TransformComponent, SpriteComponent>();
    
    for (auto entity : view) {
        // Get all the components we need
        auto& playerTransform = view.get<TransformComponent>(entity);
        auto& playerSprite = view.get<SpriteComponent>(entity);
        auto& cameraTransform = scene->camera->get<TransformComponent>();
        auto& cameraComponent = scene->camera->get<CameraComponent>();
        auto& worldComponent = scene->world->get<WorldComponent>();

        // Center camera on player (with an offset for the sprite size)
        cameraTransform.position.x = playerTransform.position.x - (cameraComponent.vw / 2) + (playerSprite.size * 5 / 2);
        cameraTransform.position.y = playerTransform.position.y - (cameraComponent.vh / 2) + (playerSprite.size * 5 / 2);

        // Clamp camera to world boundaries
        if (cameraTransform.position.x < 0) {
            cameraTransform.position.x = 0;
        }
        if (cameraTransform.position.y < 0) {
            cameraTransform.position.y = 0;
        }
        if (cameraTransform.position.x > worldComponent.width - cameraComponent.vw) {
            cameraTransform.position.x = worldComponent.width - cameraComponent.vw;
        }
        if (cameraTransform.position.y > worldComponent.height - cameraComponent.vh) {
            cameraTransform.position.y = worldComponent.height - cameraComponent.vh;
        }
    }
}
```

---

## 4. Integrating the Camera into the Game

### Summary

Now that the system is built, we need to add it to our scene and give the scene direct access to the `camera` and `world` entities for our rendering systems to use.

### Code: `Scene.h`
**File:** `src/Game/Scene/Scene.h`
```diff
 class Scene {
 // ...
 public:
     entt::registry r;
+    Entity* camera;
+    Entity* world;
 // ...
 };
```

### Code: `Pong.cpp`
**File:** `src/Pong/Pong.cpp`
```diff
 #include "Systems.h"
 #include "Game/Core/Systems/ImGuiSystem.h"
+#include "Game/Core/Systems/CameraSystem.h"
 
 // ...
 Scene* Pong::createGameplayScene() {
     // ...
 
     // Add systems
+    gameplayScene->addSystem(new CameraSystem());
     gameplayScene->addSystem(new TilemapSetupSystem());
     // ...
 }
```

---

## 5. Rendering with the Camera

### Summary

This is where the concept of moving the world instead of the camera is put into practice. We will modify our two main rendering systems: `TilemapRenderSystem` and `SpriteRenderSystem`.

In each system, before drawing a tile or a sprite, we will get the camera's current position. Then, we will subtract the camera's X and Y coordinates from the object's X and Y coordinates.

`renderX = object.x - camera.x`
`renderY = object.y - camera.y`

This ensures that if the camera moves right (increasing `camera.x`), all objects are drawn further to the left, creating the perfect illusion of a moving camera.

### Code: `Systems.cpp`

**File:** `src/Pong/Systems.cpp`
```diff
 // In TilemapRenderSystem::render()
 void TilemapRenderSystem::render() {
     auto view = scene->r.view<TilemapComponent>();
+    auto& cameraTransform = scene->camera->get<TransformComponent>();
     for (auto entity : view) {
         auto& tilemap = view.get<TilemapComponent>(entity);
         for (auto& tile : tilemap.tiles) {
             if (tile.downTexture.id > 0) {
-                DrawTextureEx(tile.downTexture, {(float)tile.x * tilemap.tileSize * tile.scale, (float)tile.y * tilemap.tileSize * tile.scale}, 0, tile.scale, WHITE);
+                DrawTextureEx(tile.downTexture, Vector2{(float)tile.x * tilemap.tileSize * tile.scale - cameraTransform.position.x, (float)tile.y * tilemap.tileSize * tile.scale - cameraTransform.position.y}, 0, tile.scale, WHITE);
             }
             // ...
             Rectangle destRec = {
-                (float)tile.x * tilemap.tileSize * tile.scale,
-                (float)tile.y * tilemap.tileSize * tile.scale,
+                (float)tile.x * tilemap.tileSize * tile.scale - cameraTransform.position.x,
+                (float)tile.y * tilemap.tileSize * tile.scale - cameraTransform.position.y,
                 (float)tilemap.tileSize * tile.scale,
                 (float)tilemap.tileSize * tile.scale
             };
             // ...
         }
     }
 }
 
 // In SpriteRenderSystem::render()
 void SpriteRenderSystem::render() {
     auto view = scene->r.view<TransformComponent, SpriteComponent>();
+    auto& cameraTransform = scene->camera->get<TransformComponent>();
     for (auto entity : view) {
         const auto& transform = view.get<TransformComponent>(entity);
         const auto& sprite = view.get<SpriteComponent>(entity);
         // ...
         Rectangle destRec = {
-            transform.position.x,
-            transform.position.y,
+            transform.position.x - cameraTransform.position.x,
+            transform.position.y - cameraTransform.position.y,
             (float)sprite.size * 5,
             (float)sprite.size * 5
         };
         // ...
     }
 }
```

---

## 6. A Note on Raylib's Built-in Camera

For educational purposes, building a camera from scratch is invaluable. However, it's important to know that Raylib (and most game libraries) provides a built-in solution that handles this for you.

Raylib has a `Camera2D` struct that stores position, zoom, and rotation. You can wrap your entire rendering loop inside `BeginMode2D(myCamera)` and `EndMode2D()` calls. Raylib will then automatically perform the coordinate transformations we just did manually on every single drawing function called between them.

This is much more convenient for larger projects, but by implementing it ourselves, we now understand exactly what's happening under the hood.

---

## 7. Further Exploration

-   **Vector Math:** The movement and positioning of the camera and game objects rely heavily on vector mathematics. A lecture on vector addition, subtraction, and scalar multiplication would provide a strong foundation for more advanced movement logic.
-   **Camera Shake:** A common effect used to add impact to events like explosions or collisions. This can be implemented by briefly applying small, random offsets to the camera's position or rotation.
-   **Parallax Scrolling:** This is a technique used in 2D games to create an illusion of depth. It involves moving background layers at a slower speed than foreground layers. This can be achieved by subtracting a *fraction* of the camera's position from background layers (e.g., `background.x - camera.x * 0.5`).
-   **Delta Time (`dt`):** Using "delta time" – the time elapsed since the last frame – to modify positions ensures movement is smooth and consistent regardless of the frame rate.