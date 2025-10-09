-- SineWave.lua
-- Moves enemy in a sine wave pattern

-- This script demonstrates sine wave movement
-- The enemy moves horizontally while oscillating vertically

-- Horizontal speed (pixels per second)
local horizontal_speed = 80

-- Wave parameters
local wave_amplitude = 50  -- How far up/down the wave goes
local wave_frequency = 2   -- How many waves per second

-- Calculate horizontal movement (constant)
local velocity_x = horizontal_speed

-- Calculate vertical movement (sine wave)
-- Use elapsed_time to create smooth oscillation
local velocity_y = wave_amplitude * wave_frequency * 2 * math.pi * math.cos(2 * math.pi * wave_frequency * input.elapsed_time)

-- Set the output
output = {
    velocity_x = velocity_x,
    velocity_y = velocity_y
}
