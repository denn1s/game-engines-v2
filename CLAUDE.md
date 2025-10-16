# Claude Code Session Guide

This document provides context for AI assistants working on this codebase. Read this file along with GEMINI.md to understand the project's structure and priorities.

---

## Core Principles

### 1. Educational Focus
This codebase is designed for **computer science students** learning about game engines and Entity Component Systems (ECS). Every contribution must prioritize **code legibility and educational value** over software engineering best practices.

**What this means:**
- Simple, readable code > clever optimizations
- Raw pointers > smart pointers (if clearer)
- Explicit logic > abstraction
- Meaningful variable names and comments
- One clear responsibility per system

### 2. Architecture: Entity Component System (ECS)

**Framework:** EnTT (header-only library)

**Key Pattern:**
- **Components** = Simple POD structs (no inheritance, public members)
- **Systems** = Classes inheriting from `System` base class
- **Entities** = Integer IDs managed by `entt::registry`, wrapped by `Entity` class
- **Scene** = Container for systems and registry

---

## Project Structure

```
src/
├── ECS/                    # Core ECS framework
│   ├── Components.h        # Base components (Transform, Velocity, Sprite, etc.)
│   ├── Entity.h/.cpp       # Entity wrapper class
│   ├── Scene.h/.cpp        # Scene management
│   └── System.h            # System base class
├── Game/
│   └── Core/
│       └── Systems/        # Core game systems
│           ├── CameraSystem.cpp
│           ├── CameraZoomSystem.cpp
│           ├── ImGuiSystem.cpp
│           └── ScriptingSystem.cpp
└── Pong/                   # Main game implementation
    ├── Components.h        # Game-specific components
    ├── Systems.h/.cpp      # Game-specific systems (17+ systems)
    └── Pong.h/.cpp         # Main game class, scene creation

assets/
├── Sprites/               # Texture files
├── Tiles/                 # Tileset images
└── Scripts/
    └── Enemy/             # Lua movement scripts
```

---

## Component Architecture

### Core Components (`src/ECS/Components.h`)
- `NameComponent` - Entity name/tag
- `TransformComponent` - Position (Vector2)
- `VelocityComponent` - Movement velocity
- `SpriteComponent` - Sprite rendering (texture, animation frames, size)
- `TileComponent` - Individual tile data
- `TilemapComponent` - Collection of tiles
- `CameraComponent` - Camera zoom/viewport
- `WorldComponent` - World dimensions

### Game Components (`src/Pong/Components.h`)
- `SizeComponent` - Width/height for rectangles
- `PlayerComponent` - Player state (running, attacking, tool, direction)
- `EnemyComponent` - Enemy marker
- `EnemyMovementComponent` - Lua script path, movement tracking
- `ColliderComponent` - AABB collision (offset, size, trigger types)
- `ColorComponent` - Rendering color
- `IntGridComponent` - Integer tile grid

**Adding New Components:**
1. Add struct to appropriate header file
2. Use simple public members
3. No inheritance or complex types
4. Add to entities via `entity.addComponent<T>()`

---

## System Architecture

### System Base Class Pattern
```cpp
class MySystem : public System {
public:
    void setup() override;    // Called once at start
    void update() override;   // Called every frame
    void render() override;   // Called after update
};
```

**Access:**
- `scene->r` - EnTT registry (query entities)
- `scene->camera` - Camera entity pointer
- `scene->world` - World entity pointer

### Existing Systems (in order)

**Setup Phase:**
1. SpriteSetupSystem - Load textures
2. TilemapSetupSystem - Generate procedural tilemap
3. AutoTilingSetupSystem - Auto-tile based on neighbors
4. CameraSystem setup - Initialize camera
5. CameraZoomSystem setup - Setup zoom controls

**Update Phase (each frame):**
1. InputSystem - Read keyboard input
2. PlayerActionSystem - Handle tool/attack input
3. MovementSystem - Apply velocity to position
4. EnemyMovementSystem - Execute Lua movement scripts
5. CollisionSystem - Check player-tilemap collision
6. SolidCollisionSystem - Stop movement on solid tiles
7. TreasureSystem - Handle treasure pickup
8. TeleportSystem - Teleport to new locations
9. CameraSystem update - Follow player
10. CameraZoomSystem update - Handle zoom input
11. AnimationSystem - Update sprite animations
12. SpriteUpdateSystem - Advance animation frames
13. ScriptingSystem - Spawn enemies via Lua

**Render Phase:**
1. TilemapRenderSystem - Draw tilemap layers
2. SpriteRenderSystem - Draw all sprites
3. ColliderRenderSystem - Debug collision visualization
4. IntGridRenderSystem - Debug tile type visualization
5. RenderSystem - Draw colored rectangles
6. ImGuiSystem - Debug UI

**System Registration:**
Add systems in `Pong::createGameplayScene()` using `scene->addSystem(new MySystem());`

---

## Entity Creation Patterns

### Player Entity
**Location:** `src/Pong/Pong.cpp::createGameplayScene()` (lines 23-38)

**Components:**
- NameComponent("cat1")
- TransformComponent (initial position)
- VelocityComponent
- PlayerComponent (state flags)
- SpriteComponent (48x48, 8 frames, "assets/Sprites/Cat.png")
- ColliderComponent (20x20 with offset 14,20)

### Enemy Entity
**Location:** Created by `ScriptingSystem` via Lua scripts

**Components:**
- NameComponent("enemy")
- TransformComponent (from spawn position)
- VelocityComponent
- EnemyComponent
- EnemyMovementComponent (Lua script path)
- SpriteComponent (48x48, 8 frames, "assets/Sprites/EvilCat.png")

**Note:** Enemies currently lack ColliderComponent - add when implementing collision!

---

## Collision System

### Current Implementation
**Type:** AABB (Axis-Aligned Bounding Box) with tilemap only

**How it works:**
1. `CollisionSystem` queries entities with `TransformComponent` + `ColliderComponent`
2. Calculates future position based on velocity
3. Samples 4 corners of collider against tilemap IntGrid
4. Sets `ColliderComponent::triggered` and `triggerType`
5. Other systems react to trigger states

**Trigger Types:**
- `NONE` - No collision
- `SOLID` - Blocks movement (water tiles)
- `TREASURE` - Collectible
- `TELEPORT` - Teleports player

**Important:** Entity-to-entity collision (player-enemy, enemy-enemy) is **NOT** implemented yet!

### Rectangle Collision Helper
Use Raylib's built-in: `CheckCollisionRecs(Rectangle a, Rectangle b)`

---

## Lua Scripting System

### Enemy Spawning
Scripts in `assets/Scripts/Enemy/` define spawn timing and positions.

**Example:** `OneTimeSpawn.lua`
```lua
function spawn_enemies(input)
    if input.elapsed_time >= 2.0 then
        return {
            {x = 500, y = 500, movement_script = "DirectChase"}
        }
    end
    return {}
end
```

### Enemy Movement
Scripts in movement system define AI patterns.

**Input Table:**
- `elapsed_time`, `delta_time`
- `start_x`, `start_y` (spawn position)
- `current_x`, `current_y` (current position)
- `player_x`, `player_y` (target position)

**Output Table:**
- `velocity_x`, `velocity_y` (movement direction)

**Available Patterns:**
DirectChase, CircleStrafe, PredictiveChase, LeapChase, AdvancedTracking, etc.

---

## Rendering Pipeline

**Order:**
1. Clear screen (black)
2. TilemapRenderSystem (bottom + top layers)
3. SpriteRenderSystem (entities with camera transform)
4. Debug overlays (colliders, grid)
5. ImGuiSystem (debug UI)
6. FPS counter

**Camera Transform:**
- World coordinates → Screen coordinates via camera position and zoom
- Camera follows player with boundary clamping
- Zoom controlled by Q/E keys, toggle with M

---

## Common Patterns

### Query Entities with Components
```cpp
auto view = scene->r.view<TransformComponent, VelocityComponent>();
for (auto entity : view) {
    auto& transform = view.get<TransformComponent>(entity);
    auto& velocity = view.get<VelocityComponent>(entity);
    // Process...
}
```

### Get Delta Time
```cpp
float dt = GetFrameTime();  // Raylib function
```

### Get Elapsed Time
```cpp
float time = GetTime();  // Raylib function, seconds since start
```

### Draw with Camera Transform
```cpp
auto& cam = scene->camera->get<CameraComponent>();
auto& camPos = scene->camera->get<TransformComponent>();

Rectangle destRect = {
    (position.x - camPos.position.x) * cam.zoom,
    (position.y - camPos.position.y) * cam.zoom,
    width * cam.zoom,
    height * cam.zoom
};
DrawRectangleRec(destRect, color);
```

---

## Adding New Features: Best Practices

### 1. Break Down into Atomic Systems
Each system should do **ONE** clear thing:
- Good: `PlayerEnemyCollisionSystem` (checks collisions, applies damage)
- Good: `DeathSystem` (removes dead entities)
- Bad: `CombatSystem` (does collision, damage, death, and animations)

### 2. Follow Existing Patterns
- Look at similar systems for reference
- Use the same query patterns
- Follow the same rendering approaches
- Maintain consistent variable naming

### 3. Keep It Simple
- Avoid complex algorithms or data structures
- Use Raylib built-in functions when possible
- Add comments for non-obvious logic
- Prefer explicit code over clever tricks

### 4. Test Incrementally
- Add one component at a time
- Test each system independently
- Use debug rendering to visualize state
- Use ImGuiSystem to inspect entity components

### 5. Component Design
- Keep components simple (POD structs)
- Use clear, descriptive member names
- Add components to `src/Pong/Components.h` (game-specific) or `src/ECS/Components.h` (reusable)
- Initialize all members explicitly when adding to entity

### 6. System Ordering Matters
- Input systems first
- Logic systems in middle
- Rendering systems last
- Consider dependencies (collision before death, movement before rendering)

---

## Branch Strategy

**Main Branch:** `main` - Most up-to-date engine version

**Feature Branches:** Each branch teaches a new concept
- Example: `12-Enemies-7-AdvancedTracking`
- Example: `13-HP-System-Player-Enemy-Interaction`

**Workflow:**
1. Branch from `main`
2. Implement feature with clear, educational code
3. Test thoroughly
4. Merge back to `main`
5. Students can check out branches to learn incrementally

---

## Common Tasks Reference

### Add a New Component
1. Define struct in `src/Pong/Components.h`
2. Add to entities in creation code
3. Access in systems via `entity.get<MyComponent>()`

### Add a New System
1. Declare class in `src/Pong/Systems.h`
2. Implement in `src/Pong/Systems.cpp`
3. Inherit from `System`, override `setup()`/`update()`/`render()`
4. Register in `Pong::createGameplayScene()`

### Debug Entity State
- Use ImGuiSystem (shows all entities and their components)
- Add debug rendering (follow `ColliderRenderSystem` pattern)
- Add print statements in system update

### Load a Texture
```cpp
Texture2D texture = TextureManager::LoadTexture("assets/Sprites/MySprite.png");
// Automatically unloaded by TextureManager
```

---

## Quick Reference: Key Files

| Purpose | File Path |
|---------|-----------|
| Game entry point | `src/Pong/Pong.cpp` |
| Entity creation | `src/Pong/Pong.cpp::createGameplayScene()` |
| Core components | `src/ECS/Components.h` |
| Game components | `src/Pong/Components.h` |
| All gameplay systems | `src/Pong/Systems.h` and `.cpp` |
| Enemy spawning | `src/Game/Core/Systems/ScriptingSystem.cpp` |
| System base class | `src/ECS/System.h` |
| Entity wrapper | `src/ECS/Entity.h` |

---

## Technology Stack

- **Language:** C++17
- **Graphics Library:** Raylib
- **ECS Framework:** EnTT
- **Scripting:** Lua with sol2 bindings
- **Noise Generation:** FastNoiseLite
- **Build System:** CMake

---

## Remember

This codebase exists to **teach students**. Every line of code should be:
- Easy to read
- Easy to understand
- Easy to modify
- Well-organized
- Following clear patterns

When in doubt, choose clarity over cleverness!
