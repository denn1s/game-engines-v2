-- ZigzagChase.lua
-- Enemy chases player while zigzagging left and right

-- This script demonstrates advanced tracking with oscillating movement
-- The enemy moves toward the player but weaves side-to-side

-- Base speed toward player
local chase_speed = 100

-- Zigzag parameters
local zigzag_amplitude = 80  -- How far to the side it zigzags
local zigzag_frequency = 2   -- How fast it zigzags

-- Get current player position
local player_x = input.player_x or 0
local player_y = input.player_y or 0

-- Calculate direction vector from enemy to player
local dx = player_x - input.current_x
local dy = player_y - input.current_y

-- Calculate distance
local distance = math.sqrt(dx * dx + dy * dy)

-- Base velocity toward player
local velocity_x = 0
local velocity_y = 0

if distance > 0 then
    -- Normalized direction to player
    local norm_x = dx / distance
    local norm_y = dy / distance

    -- Calculate perpendicular direction (for zigzag)
    local perp_x = -norm_y
    local perp_y = norm_x

    -- Calculate zigzag offset using sine wave
    local zigzag_offset = math.sin(input.elapsed_time * zigzag_frequency * 2 * math.pi) * zigzag_amplitude

    -- Combine chase movement with zigzag
    velocity_x = norm_x * chase_speed + perp_x * zigzag_offset
    velocity_y = norm_y * chase_speed + perp_y * zigzag_offset
end

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
