
[num_row, num_col] = size(wall_loc);

    % Draw all north walls on image
    for r = 1:num_row
        for c = 1:num_col
            wall_bin = de2bi(wall_loc(r,c), 4, 'right-msb');

            % Draw all walls
            if (wall_bin(1) == 1) % NORTH wall
                line([c-0.5,c+0.5],[r-0.5,r-0.5],'color','r','linewidth', 3);
            end
        end
    end