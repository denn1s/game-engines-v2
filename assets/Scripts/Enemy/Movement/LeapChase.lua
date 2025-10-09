-- LeapChase.lua
-- Enemy leaps toward player in bursts with pauses

-- This script demonstrates burst movement pattern
-- The enemy alternates between rapid movement and brief pauses

-- Leap parameters
local leap_speed = 250       -- Speed during leap
local leap_duration = 0.4    -- How long each leap lasts
local pause_duration = 0.8   -- How long to pause between leaps

-- Calculate phase in the leap cycle
local cycle_duration = leap_duration + pause_duration
local cycle_time = math.fmod(input.elapsed_time, cycle_duration)

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

-- Only move during leap phase
if cycle_time < leap_duration then
    if distance > 0 then
        velocity_x = (dx / distance) * leap_speed
        velocity_y = (dy / distance) * leap_speed
    end
end
-- Otherwise remain still (pause phase)

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
