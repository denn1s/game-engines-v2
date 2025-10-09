-- AdvancedTrackerSpawn.lua
-- Spawns enemies with various advanced tracking behaviors

-- This script demonstrates spawning enemies with different movement patterns
-- Enemies spawn in a circle around the player with randomized tracking behaviors

local diameter = 300

-- Calculate a random angle for spawn position
local angle = input.rng * 2 * math.pi

-- Calculate spawn coordinates
local x = input.player_x + (diameter / 2) * math.cos(angle)
local y = input.player_y + (diameter / 2) * math.sin(angle)

-- List of available advanced movement patterns
local movement_patterns = {
    "assets/Scripts/Enemy/Movement/DirectChase.lua",
    "assets/Scripts/Enemy/Movement/ZigzagChase.lua",
    "assets/Scripts/Enemy/Movement/CircleStrafe.lua",
    "assets/Scripts/Enemy/Movement/LeapChase.lua",
    "assets/Scripts/Enemy/Movement/ApproachRetreat.lua"
}

-- Select a random movement pattern
local pattern_index = math.floor(input.rng * #movement_patterns) + 1
local selected_pattern = movement_patterns[pattern_index]

-- Set the output table
output = {
    type = "tracker",
    x = x,
    y = y,
    movement_script = selected_pattern
}
