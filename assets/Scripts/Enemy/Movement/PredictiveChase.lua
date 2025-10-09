-- PredictiveChase.lua
-- Enemy predicts where player will be and moves there

-- This script demonstrates predictive tracking
-- The enemy aims ahead of the player based on their movement

-- Movement speed
local speed = 110

-- Prediction time (how far ahead to aim)
local prediction_time = 0.5  -- seconds

-- Get current player position
local player_x = input.player_x or 0
local player_y = input.player_y or 0

-- Store previous player position to calculate velocity
-- Using a simple estimation based on time
local prev_player_x = input.prev_player_x or player_x
local prev_player_y = input.prev_player_y or player_y

-- Estimate player velocity
local player_vx = (player_x - prev_player_x) / input.delta_time
local player_vy = (player_y - prev_player_y) / input.delta_time

-- Predict future player position
local predicted_x = player_x + player_vx * prediction_time
local predicted_y = player_y + player_vy * prediction_time

-- Calculate direction to predicted position
local dx = predicted_x - input.current_x
local dy = predicted_y - input.current_y

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
    velocity_y = velocity_y,
    -- Store current player position for next frame
    prev_player_x = player_x,
    prev_player_y = player_y
}
