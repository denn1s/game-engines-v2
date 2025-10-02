-- OneTimeSpawn.lua

if not spawned and input.total_time > 5 then
    -- Hardcoded diameter for the spawn circle
    local diameter = 100

    -- Calculate a random angle
    local angle = input.rng * 2 * math.pi

    -- Calculate spawn coordinates
    local x = input.player_x + (diameter / 2) * math.cos(angle)
    local y = input.player_y + (diameter / 2) * math.sin(angle)

    -- Set the output table
    output = {
        type = "default",
        x = x,
        y = y
    }
    spawned = true
else
    output = {
        type = "NONE"
    }
end
