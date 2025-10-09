-- CircleStraferSpawn.lua
-- Spawns enemies that circle around the player

local diameter = 250

-- Calculate a random angle
local angle = input.rng * 2 * math.pi

-- Calculate spawn coordinates
local x = input.player_x + (diameter / 2) * math.cos(angle)
local y = input.player_y + (diameter / 2) * math.sin(angle)

-- Set the output table with CircleStrafe movement
output = {
    type = "strafer",
    x = x,
    y = y,
    movement_script = "assets/Scripts/Enemy/Movement/CircleStrafe.lua"
}
