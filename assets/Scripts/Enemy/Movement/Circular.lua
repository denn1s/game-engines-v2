-- Circular.lua
-- Moves enemy in a circular pattern around its spawn point

-- This script demonstrates circular movement
-- The enemy orbits around its starting position

-- Orbital parameters
local orbit_radius = 100    -- Distance from center
local angular_speed = 1.0   -- Radians per second (1.0 = ~6 seconds per circle)

-- Calculate current angle based on elapsed time
local angle = input.elapsed_time * angular_speed

-- Calculate target position on the circle
local target_x = input.start_x + orbit_radius * math.cos(angle)
local target_y = input.start_y + orbit_radius * math.sin(angle)

-- Calculate velocity needed to reach target position
-- We calculate the tangent velocity for smooth circular motion
local velocity_x = -orbit_radius * angular_speed * math.sin(angle)
local velocity_y = orbit_radius * angular_speed * math.cos(angle)

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
