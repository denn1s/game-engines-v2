old_player_x = nil
old_player_y = nil

function update_enemies(speed, player_x, player_y)
    for _, enemy in ipairs(enemies) do
        local dx, dy = 0, 0  -- Default to no movement
        if enemy.kind == "blinky" then
            dx = (enemy.x < player_x) and speed or (enemy.x > player_x) and -speed or 0
            dy = (enemy.y < player_y) and speed or (enemy.y > player_y) and -speed or 0
        elseif enemy.kind == "pinky" then
            if old_player_x == nil then
              old_player_x = player_x
            end
            if old_player_y == nil then
              old_player_y = player_y
            end

            local target_x = old_player_x + 16 * 3 * 4
            local target_y = old_player_y + 16 * 3 * 4
            -- Calculate raw differences between enemy and target positions
            local diff_x = target_x - enemy.x
            local diff_y = target_y - enemy.y
            -- Determine movement amounts based on the sign of the differences
            dx = (diff_x < 0) and -speed or (diff_x > 0) and speed or 0
            dy = (diff_y < 0) and -speed or (diff_y > 0) and speed or 0
            -- Decide to move along the axis where the enemy and target are furthest apart
            if math.abs(diff_x) > math.abs(diff_y) then
                dy = 0  -- Move only along x if x difference is greater
            else
                dx = 0  -- Move only along y if y difference is greater or equal
            end
        else  -- random enemy
          local target_x = player_x + 16 * 4
          local target_y = player_y + 16 * 4
          -- Calculate raw differences between enemy and target positions
          local diff_x = target_x - enemy.x
          local diff_y = target_y - enemy.y
          -- Determine movement amounts based on the sign of the differences
          dx = (diff_x < 0) and -speed or (diff_x > 0) and speed or 0
          dy = (diff_y < 0) and -speed or (diff_y > 0) and speed or 0
          -- Randomly decide to move along one of the axes
          if math.random() > 0.5 then
            dy = 0  -- Move only along x
          else
            dx = 0  -- Move only along y
          end
        end
    enemy.x = enemy.x + dx
    enemy.y = enemy.y + dy
  end
end
