-- CircleStrafe.lua
-- Enemy circles around the player while maintaining distance

-- This script demonstrates orbiting behavior around the player
-- The enemy tries to maintain a specific distance while circling

-- Desired orbit distance from player
local orbit_radius = 150

-- Angular speed (how fast to circle)
local angular_speed = 1.5  -- radians per second

-- Speed to move toward/away from desired radius
local radius_correction_speed = 80

-- Get current player position
local player_x = input.player_x or 0
local player_y = input.player_y or 0

-- Calculate current distance and direction to player
local dx = input.current_x - player_x
local dy = input.current_y - player_y
local current_distance = math.sqrt(dx * dx + dy * dy)

local velocity_x = 0
local velocity_y = 0

if current_distance > 0 then
    -- Normalized direction from player to enemy
    local norm_x = dx / current_distance
    local norm_y = dy / current_distance

    -- Calculate tangent direction (perpendicular, for circling)
    local tangent_x = -norm_y
    local tangent_y = norm_x

    -- Calculate radial velocity (move toward/away to maintain orbit_radius)
    local distance_error = current_distance - orbit_radius
    local radial_speed = -distance_error * 0.5  -- Proportional control

    -- Limit radial correction speed
    if radial_speed > radius_correction_speed then
        radial_speed = radius_correction_speed
    elseif radial_speed < -radius_correction_speed then
        radial_speed = -radius_correction_speed
    end

    -- Calculate tangential velocity (circling)
    local tangent_speed = angular_speed * current_distance

    -- Combine radial and tangential movement
    velocity_x = norm_x * radial_speed + tangent_x * tangent_speed
    velocity_y = norm_y * radial_speed + tangent_y * tangent_speed
end

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
