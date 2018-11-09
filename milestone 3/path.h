#include <Vector.h>


struct dir{ //do we still need this?
  bool dir1;
  bool dir2;
}dir; //00 is north, 01 is south, 10 is east, 11 is west


/*typedef struct Fourtree {
  bool is_comp_visit;
  uint8_t pos;//[7:6] empty, [5:3] x, [2:0] y
  uint8_t state;
  struct Fourtree* front;
  struct Fourtree* left;
  struct Fourtree* right;
  struct Fourtree* back;
}Fourtree;

void find_path(Fourtree tree, uint8_t state){
  Fourtree init = {,state,NULL,NULL,NULL,NULL}
}*/

void find_path(uint8_t pos, byte dir) {
  //pos: [7:6] empty, [5:3] x, [2:0] y
  //dir: convert moving direction to absolute direction
  
  Vector<uint16_t> frontier; //figure this out
  bool visited[9][9];
  Vector<byte> path; //somehow return list of directions to go

  for (byte i = 0; i < 9; i++) {
    for (byte j = 0; j < 9; j++) {
      visited[i][j] = 0;
    }
  }

  frontier.push_back(pos<<10 | dir<<8 | cost);
  visited[pos>>3][pos&00000111] = 1 //or cost

  while (//frontier not empty)
    {
      //pull n from frontier: somehow pull x coordinates out of vector
    }
  

  
}
