-- OneTimeSpawn.lua

if not spawned and input.total_time > 5 then
    -- Hardcoded diameter for the spawn circle
    local diameter = 100

    -- Calculate a random angle
    local angle = input.rng * 2 * math.pi

    -- Calculate spawn coordinates
    local x = input.player_x + (diameter / 2) * math.cos(angle)
    local y = input.player_y + (diameter / 2) * math.sin(angle)

    -- Select a random movement pattern (including advanced tracking)
    local movement_scripts = {
        "assets/Scripts/Enemy/Movement/LinearTowardsPlayer.lua",
        "assets/Scripts/Enemy/Movement/SineWave.lua",
        "assets/Scripts/Enemy/Movement/Circular.lua",
        "assets/Scripts/Enemy/Movement/DirectChase.lua",
        "assets/Scripts/Enemy/Movement/ZigzagChase.lua",
        "assets/Scripts/Enemy/Movement/CircleStrafe.lua",
        "assets/Scripts/Enemy/Movement/LeapChase.lua",
        "assets/Scripts/Enemy/Movement/ApproachRetreat.lua"
    }

    local script_index = math.floor(input.rng * #movement_scripts) + 1
    local selected_movement = movement_scripts[script_index]

    -- Set the output table
    output = {
        type = "default",
        x = x,
        y = y,
        movement_script = selected_movement
    }
    spawned = true
else
    output = {
        type = "NONE"
    }
end
