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
- `tileX`, `tileY`: The x and y position of the a in the texture.

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

## Autotiling Techniques

Autotiling is a technique used in 2D games to automatically select the correct tile from a tileset based on its neighbors. This saves artists and level designers a significant amount of time, as they don't have to manually place every single tile variation. There are several methods to achieve this, each with its own strengths and weaknesses.

### Blob Tiles

Blob tiling is a common and straightforward method. It works by looking at the 8 immediate neighbors of a tile and creating a "bitmask" to represent which of those neighbors are of the same type. This bitmask (an 8-bit integer) is then used to look up the correct tile from a tileset that has 256 tiles (2^8).

-   **Advantages:**
    -   Relatively simple to implement.
    -   Can produce very organic and rounded "blob-like" shapes, which is great for terrain like dirt, sand, or water.
-   **Disadvantages:**
    -   Requires a large tileset with 256 tiles to cover all possible combinations. This is a lot of work for an artist.
    -   The standard implementation doesn't handle complex cases like inner corners gracefully without additional logic.

### Wang Tiles

Wang tiling is a more advanced technique that uses a smaller tileset. The most common 2-edge Wang tileset only requires 16 tiles. Each edge of a tile is assigned a color (e.g., "grass" or "water"), and the algorithm places tiles such that the colors of adjacent edges match.

-   **Advantages:**
    -   Requires a much smaller tileset (typically 16 tiles), which is easier for artists to create.
    -   Can produce more structured and varied patterns than blob tiles.
-   **Disadvantages:**
    -   The logic can be more complex to implement than simple blob tiling.
    -   It can sometimes produce repetitive or predictable patterns if not implemented carefully.

### This Project's Implementation: A Hybrid Approach

This project uses a hybrid approach that combines the bitmasking concept of blob tiling with special logic to handle diagonal connections more intelligently, similar to how a more advanced blob or Wang tile system would work.

It uses an 8-way bitmask, but with a crucial addition: **for a diagonal neighbor to be considered "on," its two adjacent cardinal neighbors must also be "on."**

For example, the **top-left** neighbor is only considered part of the group if the **top** and **left** neighbors are also of the same tile type. This prevents the system from tiling against a single, isolated diagonal tile and creates clean, sharp inner corners.

-   **Advantages:**
    -   Produces clean and aesthetically pleasing corners without needing a full 256-tile blob tileset.
    -   More robust and visually appealing than a basic blob implementation.
    -   The logic is contained within the autotiling system, making it easier to understand and modify than some complex Wang tile solvers.
-   **Disadvantages:**
    -   The tileset is more complex than a simple 16-tile Wang set and must be designed specifically for this logic.
    -   The code is slightly more complex than a basic blob implementation due to the extra diagonal checks.

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