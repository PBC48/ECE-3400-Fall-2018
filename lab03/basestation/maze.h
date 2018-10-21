/**
 * decoder.h
 * 

 * Current bit configuration:
 * [3:0] WALLS; true or false NESW
 * [5:4] TREASURE; can go up four bits. 2 shapes * 2 colors = 4 combs
 * [8]   ROBOT; true or false
*/

/**
 * Takes input number and outputs array of output with labels. Assume
 * size of array works
*/
int * decoder(int input){
    bool west  = input & 0x1;
    bool south = (input >> 1) & 0x1;
    bool east  = (input >> 2) & 0x1;
    bool north = (input >> 3) & 0x1;
    bool robot = (input >> 7) & 0x1;
    int treasure = (input >> 4) & 0x3;
    int tshape = (treasure>>1) & 1;
    int tcolor =  treasure & 1;
    int output[]={west, north, east, south, robot, tshape, tcolor};
    return output; 
}

int encoder(int &in1, int &in2, int &in3, int &in4, int &in5, int &in6){
    
    int output = (in5<<7|in6<<4|in4<<3|in3<<2|in2<<1|in1) & 0xBF;
    return output;
}
