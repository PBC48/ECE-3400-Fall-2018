// NOTE: This file is very different from Xiao's original maze.h


uint8_t robot_pos; // robot's current position; [7:4] x, [3:0] y
uint8_t next_pos;

byte robot_dir; // absolute direction robot is facing; NESW

bool explored[9][9]; // stores where in maze robot has explored

byte visited_array[9][9];

byte vcount = 0;

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

  byte next_dir = (move_dir + robot_dir)%4; // NESW

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

  Serial.print("Exploring: "); Serial.print(x); Serial.print(", "); Serial.println(y);
  return explored[x][y];
  
}




uint8_t find_next_square() {
  // look at squares adjacent to robot's current position
  // find where visited_array = vcount-1 (most recent previous position)
  // return those coordinates (backtrack)
  // else, return robot's current position
  // (output is used as input to go_to)

  /*uint8_t pos_n = robot_pos + 0x01;
  uint8_t pos_e = robot_pos + 0x10;
  uint8_t pos_s = robot_pos - 0x01;
  uint8_t pos_w = 
  uint8_t square[4] = [pos_n, pos_e, pos_s, pos_w];*/
  
  uint8_t x = (robot_pos>>4) & 0x0f;
  uint8_t y = (robot_pos) & 0x0f;
  uint8_t xs, ys;

  for(byte i=0; i<4; i++) {

    switch(i) {
      case 0: //north
      xs = x;
      if (y!=8)
        ys = y+1;
      else
        ys = y;
      break;
    case 1: //east
      ys = y;
      if (x!=8)
        xs = x+1;
      else
        xs = x;
      break;
    case 2: // south
      xs = x;
      if (y!=0)
        ys = y-1;
      else
        ys = y;
      break;
    case 3: // west
      ys = y;
      if (x!=0)
        xs = x-1;
      else
        xs = x;
      break;
    }
    
    if ( visited_array[xs][ys] == (vcount-1) ) {
      //ncount = visited_array[x][y];
      return (((xs<<4)&0xf0) | ys&0x0f);
    } /*else if ( visited_array[x][y] > ncount ) {
      ncount = visited_array[xs][ys];
      xo = xs; yo = ys;
    } //else {
      
    //}*/
  }
  
  return (((x<<4)&0xf0) | y&0x0f); // or just return robot_pos...
}




byte go_to(uint8_t next_pos){
  // return dir given current location and next location, 
  // and direction the robot is facing

  byte x = (robot_pos>>4) & 0x0f; //bytes? for negative values
  byte y = (robot_pos) & 0x0f;
  byte x_n = (next_pos>>4) & 0x0f;
  byte y_n = (next_pos) & 0x0f;
  byte next_dir; // NESW

  if ( (x_n-x) == 1 ) { //east
    next_dir = 1;
  } else if ( (x-x_n) == 1 ) { //west
    next_dir = 3;
  } else if ( (y_n-y) == 1 ) { //north
    next_dir = 0;
  } else if ( (y-y_n) == 1 ) { //south
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

  Serial.print("move_dir: "); Serial.println(move_dir);
  uint8_t x = (robot_pos>>4) & 0x0f;
  uint8_t y = (robot_pos) & 0x0f;

  Serial.print("robot_dir: "); Serial.println(robot_dir);
  byte next_dir = (4 + move_dir + robot_dir)%4; // NESW
  Serial.print("next_dir: "); Serial.println(next_dir);
  robot_dir = next_dir;

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
  
}



void print_robot_state(){
  
  uint8_t x = (robot_pos>>4) & 0x0f;
  uint8_t y = (robot_pos) & 0x0f;

  Serial.print("x: "); Serial.println(x);
  Serial.print("y: "); Serial.println(y);

  switch(robot_dir){
    case 0:
      Serial.println(F("direction: north"));
      break;
    case 1:
      Serial.println(F("direction: east"));
      break;
    case 2:
      Serial.println(F("direction: south"));
      break;
    case 3:
      Serial.println(F("direction: west"));
      break;
  }
}




void print_explored(){

  for(byte i=0; i<9; i++){
    Serial.print("  ");
    for(byte j=0; j<9; j++){
      Serial.print(explored[i][j]); Serial.print(" ");
    }
    Serial.println("");
  }
  
}




void print_visited_array(){

  for(byte i=0; i<9; i++){
    Serial.print("  ");
    for(byte j=0; j<9; j++){
      Serial.print(visited_array[i][j]); Serial.print(" ");
    }
    Serial.println("");
  }
  
}
