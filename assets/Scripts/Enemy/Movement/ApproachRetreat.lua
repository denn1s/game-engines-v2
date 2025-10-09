-- ApproachRetreat.lua
-- Enemy approaches player then retreats, creating a yo-yo effect

-- This script demonstrates approach/retreat pattern
-- The enemy moves close to the player then backs away repeatedly

-- Movement speed
local speed = 100

-- Distance thresholds
local min_distance = 80   -- Start retreating when closer than this
local max_distance = 200  -- Start approaching when farther than this

-- Get current player position
local player_x = input.player_x or 0
local player_y = input.player_y or 0

-- Calculate direction vector from enemy to player
local dx = player_x - input.current_x
local dy = player_y - input.current_y

-- Calculate distance
local distance = math.sqrt(dx * dx + dy * dy)

local velocity_x = 0
local velocity_y = 0

if distance > 0 then
    local norm_x = dx / distance
    local norm_y = dy / distance

    if distance > max_distance then
        -- Too far: approach the player
        velocity_x = norm_x * speed
        velocity_y = norm_y * speed
    elseif distance < min_distance then
        -- Too close: retreat from the player
        velocity_x = -norm_x * speed
        velocity_y = -norm_y * speed
    else
        -- In the sweet spot: circle around
        local tangent_x = -norm_y
        local tangent_y = norm_x
        velocity_x = tangent_x * speed * 0.7
        velocity_y = tangent_y * speed * 0.7
    end
end

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
