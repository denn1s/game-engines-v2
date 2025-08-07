# Game Engine v2

This is a simple game engine built with C++23, CMake, and Raylib.

## Features

- **Entity Component System (ECS):** The engine uses an ECS architecture provided by the `entt` library.
- **2D Rendering:** The engine uses Raylib for 2D rendering.
- **ImGui Integration:** The engine is integrated with ImGui for debugging and UI purposes.
- **Sprite Loading:** The engine supports loading and rendering sprites with animations.

## Controls

- **Movement:** WASD
- **Run:** Hold Left Shift while moving.
- **Select Tool:**
    - `1`: Shovel
    - `2`: Axe
    - `3`: Water Can
    - `0`: Unequip
- **Use Tool/Attack:** Spacebar

## Sprite Loading

The sprite loading system is implemented using a `SpriteComponent` and a few systems.

### `SpriteComponent`

The `SpriteComponent` stores the following information about a sprite:

- `name`: The path to the sprite sheet.
- `xIndex`, `yIndex`: The index of the sprite in the sprite sheet.
- `size`: The size of a single sprite in the sprite sheet.
- `animationFrames`: The number of frames in the animation.
- `animationDuration`: The duration of the animation in milliseconds.
- `texture`: The Raylib `Texture2D` object.
- `lastUpdate`: The time of the last animation frame update.

### `SpriteSetupSystem`

The `SpriteSetupSystem` is responsible for loading the sprite textures. It runs once when the scene is loaded.

### `SpriteRenderSystem`

The `SpriteRenderSystem` is responsible for rendering the sprites. It runs every frame.

### `SpriteUpdateSystem`

The `SpriteUpdateSystem` is responsible for updating the animation frames of the sprites. It runs every frame.

### `TextureManager`

The `TextureManager` is a simple singleton that loads and unloads textures. This is to avoid loading the same texture multiple times.

## How to Build

1. **Configure the project:**
   ```bash
   ./configure.sh
   ```
2. **Build the project:**
    ```bash
    ./build.sh
    ```
3. **Run the game:**
    ```bash
    ./run.sh
    ```