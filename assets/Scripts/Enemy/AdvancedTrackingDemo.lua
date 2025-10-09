-- AdvancedTrackingDemo.lua
-- Demonstrates advanced enemy tracking behaviors

-- This script spawns multiple enemies with different tracking patterns
-- to showcase the various movement behaviors

if not initialized then
    spawn_count = 0
    last_spawn_time = 0
    spawn_interval = 3.0  -- Spawn every 3 seconds
    max_spawns = 5        -- Spawn up to 5 enemies
    initialized = true
end

-- Check if it's time to spawn
if spawn_count < max_spawns and (input.total_time - last_spawn_time) >= spawn_interval then
    local diameter = 300

    -- Calculate spawn position in a circle around player
    local angle = input.rng * 2 * math.pi
    local x = input.player_x + (diameter / 2) * math.cos(angle)
    local y = input.player_y + (diameter / 2) * math.sin(angle)

    -- List of all advanced tracking movement patterns
    local advanced_movements = {
        "assets/Scripts/Enemy/Movement/DirectChase.lua",
        "assets/Scripts/Enemy/Movement/ZigzagChase.lua",
        "assets/Scripts/Enemy/Movement/CircleStrafe.lua",
        "assets/Scripts/Enemy/Movement/LeapChase.lua",
        "assets/Scripts/Enemy/Movement/ApproachRetreat.lua"
    }

    -- Cycle through patterns or pick randomly
    -- Using modulo to cycle through all patterns
    local pattern_index = (spawn_count % #advanced_movements) + 1
    local selected_pattern = advanced_movements[pattern_index]

    -- Set output to spawn enemy
    output = {
        type = "tracker",
        x = x,
        y = y,
        movement_script = selected_pattern
    }

    spawn_count = spawn_count + 1
    last_spawn_time = input.total_time
else
    -- Don't spawn anything
    output = {
        type = "NONE"
    }
end
