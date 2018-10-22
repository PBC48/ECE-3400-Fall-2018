/**
 * decoder.h
 * 

 * Current bit configuration:
 * [3:0] WALLS; true or false NESW
 * [7:4] TREASURE; can go up four bits. 2 shapes * 2 colors = 4 combs
 * [8]   ROBOT; true or false
*/

/**
 * Takes input number and outputs array of output with labels. Assume
 * size of array works
 * 
 *   15   -------------------  9         8      7     5      2                0
//  +-------------------------+---------+-----+------+------+------------------+
//  | type------------------- |direction|robot| dir  | trea | WALLS            |
//  +-------------------------+---------+-----+------+------+------------------+
*/
int * decoder(uint16_t input){
    bool left  = input & 0x1;
    bool front = (input >> 1) & 0x1;
    bool right  = (input >> 2) & 0x1;
    //bool north = (input >> 3) & 0x1;
    bool robot = (input >> 8) & 0x1;
    int treasure = (input >> 3) & 0x7;
    //int tshape = (input>>4) & 0x1;
    //int tcolor =  (input>>5) & 0x1;
    int direction = (input>>6) & 0x3;
    int output[]={left, front, right, robot, treasure, direction};
    return output; 
}

int encoder(int &in1, int &in2, int &in3, int &in4, int &in5, int &in6){
    
    int output = (in5<<7|in6<<4|in4<<3|in3<<2|in2<<1|in1) & 0xBF;
    return output;
}
