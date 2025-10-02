-- SquareSpawn.lua

-- Hardcoded side length for the spawn square
local side = 400

-- Calculate a random point on the perimeter of the square
local perimeter = 4 * side
local random_point = input.rng * perimeter

local x, y

if random_point < side then
    -- Top edge
    x = input.player_x - side / 2 + random_point
    y = input.player_y - side / 2
elseif random_point < 2 * side then
    -- Right edge
    x = input.player_x + side / 2
    y = input.player_y - side / 2 + (random_point - side)
elseif random_point < 3 * side then
    -- Bottom edge
    x = input.player_x + side / 2 - (random_point - 2 * side)
    y = input.player_y + side / 2
else
    -- Left edge
    x = input.player_x - side / 2
    y = input.player_y + side / 2 - (random_point - 3 * side)
end

-- Set the output table
output = {
    type = "default",
    x = x,
    y = y
}
