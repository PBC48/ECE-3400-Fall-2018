#include <Vector.h>
#include "frontier.h"

//dir: 00 is north, 01 is south, 10 is east, 11 is west

uint8_t maze[9][9];
  // [4] explored/unexplored, [3:0] walls NSEW


byte conv_x(byte x, int i){
  switch (i){
    case 0:
      if(x==0)
        return x;
      else
        return x-1;
      break;
    case 1:
      if(x==8)
        return x;
      else
        return x+1;
      break;
    case 2:
      return x;
      break;
    case 3:
      return x;
      break;
  }
}

byte conv_y(byte y, int i){
  switch (i){
    case 2:
      if(y==0)
        return y;
      else
        return y-1;
      break;
    case 3:
      if(y==8)
        return y;
      else
        return y+1;
      break;
    case 0:
      return y;
      break;
    case 1:
      return y;
      break;
  }
}

int compute_cost(byte dir, int action){
  switch(dir + action){
    case 0:
    return 0;
    case 1:
    return 3;
    case 2:
    return 2;
    case 3:
    return 1;
    case 4:
    return 0;
    case 5:
    return 3;
    case 6:
    return 2;
    default:
    break;
  }
}
bool wall_present(byte xi, byte yi, int i){
  return (maze[xi][yi]>>i)&&0x1;
}

void find_path(uint8_t pos, byte dir) {
  //pos: [7:6] empty, [5:3] x, [2:0] y
  //dir: convert moving direction to absolute direction
  
  frontier* front = init_frontier(byte(pos)>>4,byte(pos),dir,0); //figure this out
    // [15:13] xpos, [12:10] ypos, [9:8] dir, [7:0] cost
  bool visited[9][9];
  Vector<byte> path; //somehow return list of directions to go
  uint8_t n = 0;

  for (byte i = 0; i < 9; i++) {
    for (byte j = 0; j < 9; j++) {
      visited[i][j] = 0;
    }
  }
  visited[pos>>3][pos&00000111] = 1; //or cost
  frontier *next = front;
  int l = 0;
  while (l<100)
    {
      uint16_t n_state = next; //pull n from frontier: 
          // somehow pull x coordinates out of vector / store n from previous loop
      
      byte x = (n_state&0xe000)>>13;
      byte y = (n_state&0x1c00)>>10;
      byte xi;
      byte yi;
      byte cost_i;
      if( !((maze[x][y])>>4) ) {
        return path;
      } else {
        for(byte i = 0; i < 4; i++) {
          xi = conv_x(x,i); yi = conv_y(y,i);
          if(!wall_present(xi,yi,i)){
            cost_i = compute_cost(dir,i);
            // find dir
          }
          if(visited[xi][yi]){
            frontier* find_node = get_cost(find_frontier(next,xi,yi));
            if (find_node != NULL){
              if (cost_i < (get_cost(find_node))){
                set_cost(find_node,cost_i);
              }
            }
            else{
              add_frontier(next,i,false,get_cost(next) + cost_i);
            }
          }
        }
      }
    l = l+1;
    } 
 destroy_frontier(front);
}
