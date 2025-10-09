# Enemy Movement Patterns

This directory contains Lua scripts that define different enemy movement behaviors. Each script receives input from the game engine and outputs velocity values to control enemy movement.

## Basic Movement Patterns

### LinearTowardsPlayer.lua
- **Behavior**: Moves in a straight line toward the player's initial position
- **Use Case**: Simple, predictable enemies
- **Parameters**:
  - Speed: 100 pixels/second

### SineWave.lua
- **Behavior**: Moves horizontally while oscillating vertically in a sine wave pattern
- **Use Case**: Flying or swimming enemies with wavy movement
- **Parameters**:
  - Horizontal speed: 80 pixels/second
  - Wave amplitude: 50 pixels
  - Wave frequency: 2 waves/second

### Circular.lua
- **Behavior**: Orbits around its spawn point
- **Use Case**: Stationary enemies that guard an area
- **Parameters**:
  - Orbit radius: 100 pixels
  - Angular speed: 1.0 radians/second

## Advanced Tracking Patterns

### DirectChase.lua
- **Behavior**: Continuously tracks and chases the player's current position
- **Use Case**: Persistent enemies that never give up
- **Parameters**:
  - Speed: 120 pixels/second
- **Key Features**: Updates direction every frame to follow player

### ZigzagChase.lua
- **Behavior**: Chases the player while weaving side-to-side
- **Use Case**: Enemies that are harder to predict and dodge
- **Parameters**:
  - Chase speed: 100 pixels/second
  - Zigzag amplitude: 80 pixels
  - Zigzag frequency: 2 oscillations/second
- **Key Features**: Combines direct pursuit with perpendicular oscillation

### CircleStrafe.lua
- **Behavior**: Circles around the player while maintaining a specific distance
- **Use Case**: Ranged enemies that keep their distance
- **Parameters**:
  - Orbit radius: 150 pixels
  - Angular speed: 1.5 radians/second
  - Radius correction speed: 80 pixels/second
- **Key Features**: Uses proportional control to maintain target distance

### LeapChase.lua
- **Behavior**: Alternates between rapid movement toward player and brief pauses
- **Use Case**: Enemies with burst movement like frogs or insects
- **Parameters**:
  - Leap speed: 250 pixels/second
  - Leap duration: 0.4 seconds
  - Pause duration: 0.8 seconds
- **Key Features**: Creates a stop-and-go pattern

### ApproachRetreat.lua
- **Behavior**: Approaches when far, retreats when close, circles at medium range
- **Use Case**: Cautious enemies that maintain tactical positioning
- **Parameters**:
  - Speed: 100 pixels/second
  - Min distance: 80 pixels (retreat threshold)
  - Max distance: 200 pixels (approach threshold)
- **Key Features**: Three distinct behaviors based on distance

### PredictiveChase.lua
- **Behavior**: Aims ahead of the player based on their velocity
- **Use Case**: Smart enemies that intercept the player's path
- **Parameters**:
  - Speed: 110 pixels/second
  - Prediction time: 0.5 seconds
- **Key Features**: Estimates player velocity and predicts future position

## Input Variables

All movement scripts receive an `input` table with the following fields:

- `elapsed_time`: Time since enemy spawned (seconds)
- `delta_time`: Time since last frame (seconds)
- `start_x`, `start_y`: Enemy's spawn position
- `current_x`, `current_y`: Enemy's current position
- `player_x`, `player_y`: Player's current position

## Output Format

All movement scripts must set an `output` table with:

- `velocity_x`: Horizontal velocity (pixels/second)
- `velocity_y`: Vertical velocity (pixels/second)

## Creating Custom Movement Patterns

To create a new movement pattern:

1. Create a new `.lua` file in this directory
2. Read values from the `input` table
3. Calculate desired velocities based on your logic
4. Set the `output` table with `velocity_x` and `velocity_y`

Example template:

```lua
-- CustomPattern.lua
-- Description of the movement pattern

local speed = 100

-- Your movement logic here
local velocity_x = 0
local velocity_y = 0

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
```

## Teaching Notes

This lesson demonstrates:
- **Player tracking**: Using player position to create responsive AI
- **State-based behavior**: Different actions based on distance/time
- **Predictive movement**: Anticipating player movement
- **Pattern combination**: Mixing multiple movement types (chase + oscillation)
- **Mathematical foundations**: Trigonometry, vector math, and proportional control
