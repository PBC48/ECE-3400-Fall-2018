% Initialize current location maze array
% 1 means unvisited
% 0.5 means visited
% 0 is the robot's current location

curr_loc = [...
    1 1 1 1 
    1 1 1 1
    1 1 1 1
    1 1 1 1 
    1 1 1 0];  

% Set maze walls
% For each grid space, wall locations are represented with 4 bits:
% [bit3 bit2 bit1 bit0] ==> [W E S N]
% N (north wall) = 0001 in binary or 1 in decimal
% S (south wall) = 0010 in binary or 2 in decimal
% E (east wall)  = 0100 in binary or 4 in decimal
% W (west wall)  = 1000 in binary or 8 in decimal
% For example: A grid that has a wall to the north and east would have a
% wall location of 0101 in binary or 5 in decimal

% Two wall configurations, for testing
% You can uncomment any of the wall_loc arrays or write your own mazes

% wall_loc = [...
%     9 1 3 5
%     8 4 11 4
%     12 10 5 12
%     8 5 14 12
%     10 2 3 6];

wall_loc = [...
    9 1 3 5
    8 6 13 12
    12 11 6 12
    8 3 7 14
    10 3 3 7];

% User-defined color map
% 0   -> black
% 0.5 -> green
% 1   ->
map = [...
       0, 0, 0 
       0, 1, 0
       1, 1, 1];

% Draw initial grid
colormap(map);
imagesc(curr_loc);
caxis([0 1])
draw_walls(wall_loc);