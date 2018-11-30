#include <StackArray.h>


uint8_t robot_pos; // robot's current position; [7:4] x, [3:0] y

byte robot_dir; // absolute direction robot is facing; NESW

bool explored[9][9]; // stores where in maze robot has explored

StackArray <uint8_t> visited;

// dir and move_dir are the direction the robot is turning; FRBL
// next_dir is the absolute direction the robot is going to go; NESW
// robot_dir is the absolute direction the robot is currently facing; NESW




bool is_explored(byte move_dir){
  // return whether an adjacent square is explored or not
  // given the direction the robot is proposing to turn
  // and the direction the robot is currently facing
  // move_dir: FRBL

  uint8_t x = (robot_pos>>4) & 0x0f;
  uint8_t y = (robot_pos) & 0x0f;

  byte next_dir = (4 + move_dir - robot_dir)%4; // NESW

  switch (next_dir) {
    case 0: //north
      if (y!=8)
        y++;
      break;
    case 1: //east
      if (x!=8)
        x++;
      break;
    case 2: // south
      if (y!=0)
        y--;
      break;
    case 3: // west
      if (x!=0)
        x--;
      break;
  }

  return explored[x][y];
  
}




byte go_to(uint8_t next_pos){
  // return dir given current location and next location, 
  // and direction the robot is facing

  uint8_t x = (robot_pos>>4) & 0x0f; //bytes? for negative values
  uint8_t y = (robot_pos) & 0x0f;
  uint8_t x_n = (next_pos>>4) & 0x0f;
  uint8_t y_n = (next_pos) & 0x0f;
  byte next_dir; // NESW

  if ( (x_n-x) == 1 ) { //east
    next_dir = 1;
  } else if ( (x-x_n) == 1 ) { //west
    next_dir = 3;
  } else if ( (y_n-y) == 1 ) { //north
    next_dir = 1;
  } else if ( (y-y_n) ==1 ) { //south
    next_dir = 2;
  } else {
    next_dir = 4; // just turn around
  }

  if (next_dir == 4) {
    return 2;
  } else {
    return (4 + next_dir - robot_dir)%4; // = move_dir
  }
 
}




void update_robot_state(byte move_dir){
  // update robot_pos and robot_dir
  
  uint8_t x = (robot_pos>>4) & 0x0f;
  uint8_t y = (robot_pos) & 0x0f;

  byte next_dir = (4 + move_dir - robot_dir)%4; // NESW

  switch (next_dir) {
    case 0: //north
      if (y!=8)
        y++;
      break;
    case 1: //east
      if (x!=8)
        x++;
      break;
    case 2: // south
      if (y!=0)
        y--;
      break;
    case 3: // west
      if (x!=0)
        x--;
      break;
  }

  robot_pos = ( ((x<<4)&0xf0) | (y&0x0f) );
  robot_dir = next_dir;
}
