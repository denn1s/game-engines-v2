-- DirectChase.lua
-- Enemy continuously tracks and chases the player

-- This script demonstrates advanced tracking behavior
-- The enemy constantly updates its direction toward the player's current position

-- Movement speed (pixels per second)
local speed = 120

-- Get current player position (updates every frame)
local player_x = input.player_x or 0
local player_y = input.player_y or 0

-- Calculate direction vector from enemy to player
local dx = player_x - input.current_x
local dy = player_y - input.current_y

-- Calculate distance
local distance = math.sqrt(dx * dx + dy * dy)

-- Normalize direction and apply speed
local velocity_x = 0
local velocity_y = 0

if distance > 0 then
    velocity_x = (dx / distance) * speed
    velocity_y = (dy / distance) * speed
end

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
