# Game Engine v2

This is a simple game engine built with C++23, CMake, and Raylib.

## Features

- **Entity Component System (ECS):** The engine uses an ECS architecture provided by the `entt` library.
- **2D Rendering:** The engine uses Raylib for 2D rendering.
- **ImGui Integration:** The engine is integrated with ImGui for debugging and UI purposes.
- **Sprite Loading:** The engine supports loading and rendering sprites with animations.
- **Tilemaps:** The engine supports loading and rendering tilemaps.

## Controls

- **Movement:** WASD
- **Run:** Hold Left Shift while moving.
- **Select Tool:**
    - `1`: Shovel
    - `2`: Axe
    - `3`: Water Can
    - `0`: Unequip
- **Use Tool/Attack:** Spacebar

## Tilemaps

A tilemap is a grid of tiles used to create the game world. In this engine, tilemaps are implemented using a `TilemapComponent` and a few systems.

### `TileComponent`

The `TileComponent` stores the following information about a tile:

- `x`, `y`: The position of the tile in the tilemap grid.
- `texture`: The Raylib `Texture2D` object.
- `scale`: The scale of the tile.
- `needsAutoTiling`: A boolean that indicates if the tile needs to be auto-tiled.
- `tileX`, `tileY`: The x and y position of the tile in the texture.

### `TilemapComponent`

The `TilemapComponent` stores the following information about a tilemap:

- `tiles`: A vector of `TileComponent`s.
- `width`, `height`: The dimensions of the tilemap in tiles.
- `tileSize`: The size of a single tile in pixels.

### `TilemapSetupSystem`

The `TilemapSetupSystem` is responsible for creating the tilemap. It runs once when the scene is loaded. It loads the tilemap data from `src/Pong/tilemap.h` and creates the tiles.

### `AutoTilingSetupSystem`

The `AutoTilingSetupSystem` is responsible for auto-tiling the tilemap. It runs once after the `TilemapSetupSystem`. It checks the neighbors of each tile and updates the `tileX` and `tileY` properties of the tile to use the correct sprite from the tileset.

The auto-tiling system uses an 8-way bitmask to determine which tile to use. The bitmask is an 8-bit integer where each bit represents a neighbor of the tile. The neighbors are checked in the following order: top-left, top, top-right, left, right, bottom-left, bottom, bottom-right. If a neighbor is of the same type as the current tile, the corresponding bit is set to 1. The resulting integer is then used as a key in a map to get the correct x and y coordinates of the tile in the tileset. This allows for more complex tile transitions, including diagonal connections.

### `TilemapRenderSystem`

The `TilemapRenderSystem` is responsible for rendering the tilemap. It runs every frame.

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