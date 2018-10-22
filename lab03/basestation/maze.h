/**
 * decoder.h
 * 

 * Current bit configuration:
 * [2:0] WALLS; true or false / left, front, right
 * [5:3] TREASURE; y/n, circle/square, red/blue
 * [7:6] ROBOT DIRECTION; forward, right, left
 * [8]   ROBOT; true or false
*/

/**
 * Takes input number and outputs array of output with labels. Assume
 * size of array works
 * 0000000|R|LRF|TSC|LRF
15                   9 8     7    6 5       3 2       0
+---------------------+-----+------+---------+---------+
| Reserved            |ROBOT|DIR   |TREASURE | WALLS   |
+---------------------+-----+------+---------+---------+
*/
int * decoder(uint16_t input){
    bool wleft  = input & 0x1;
    bool wright = (input >> 1) & 0x1;
    bool wforward  = (input >> 2) & 0x1;
    bool robot = (input >> 7) & 0x1;
    bool left = 
    bool right =
    bool forward =
    int treasure = (input >> 4) & 0x7;
    int tshape = (input>>4) & 0x1;
    int tcolor =  (input>>5) & 0x1;
    int direction = (input>>8) & 0x3;
    int output[]={wleft, wright, wforward, robot, left, right, forward, tshape, tcolor, direction};
    return output; 
}

int encoder(int &in1, int &in2, int &in3, int &in4, int &in5, int &in6){
    
    int output = (in5<<7|in6<<4|in4<<3|in3<<2|in2<<1|in1) & 0xBF;
    return output;
}
