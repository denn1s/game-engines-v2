-- DirectChaserSpawn.lua
-- Spawns enemies that directly chase the player

local diameter = 350

-- Calculate a random angle
local angle = input.rng * 2 * math.pi

-- Calculate spawn coordinates
local x = input.player_x + (diameter / 2) * math.cos(angle)
local y = input.player_y + (diameter / 2) * math.sin(angle)

-- Set the output table with DirectChase movement
output = {
    type = "chaser",
    x = x,
    y = y,
    movement_script = "assets/Scripts/Enemy/Movement/DirectChase.lua"
}
