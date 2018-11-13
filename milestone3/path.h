#include <Vector.h>
#include <frontier.h>


struct Frontier{
  bool is_parent;
  uint16_t info;
  frontier north;
  frontier west;
  frontier east;
  frontier south; 
};
//dir: 00 is north, 01 is south, 10 is east, 11 is west

uint8_t maze[9][9];
  // [4] explored/unexplored, [3:0] walls NSEW

  

void find_path(uint8_t pos, byte dir) {
  //pos: [7:6] empty, [5:3] x, [2:0] y
  //dir: convert moving direction to absolute direction
  
  Vector<uint16_t> frontier; //figure this out
    // [15:13] xpos, [12:10] ypos, [9:8] dir, [7:0] cost
  bool visited[9][9];
  Vector<byte> path; //somehow return list of directions to go
  uint8_t n = 0;

  for (byte i = 0; i < 9; i++) {
    for (byte j = 0; j < 9; j++) {
      visited[i][j] = 0;
    }
  }

  frontier.push_back(pos<<10 | dir<<8 | cost);
  visited[pos>>3][pos&00000111] = 1 //or cost

  while (//frontier not empty)
    {
      uint16_t n_state = frontier[n] //pull n from frontier: 
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
            cost_i = compute_cost(dir);
            // find dir
          }
          if(visited[xi][yi]){
            if (cost_i < ( )){
              
            }
          }
          else{
            
          }
        }
      }
    }
  

  
}

byte conv_x(byte x, i){
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

byte conv_y(byte y, i){
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


bool wall_present(){
  
}
