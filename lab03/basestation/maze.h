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
 * 0000000|R|DD|TSC|LRF
15                   9 8     7    6 5       3 2       0
+---------------------+-----+------+---------+---------+
| Reserved            |ROBOT|DIR   |TREASURE | WALLS   |
+---------------------+-----+------+---------+---------+
*/
byte * decoder(uint16_t input){
  //Serial.print("INPUT: ");Serial.println(input);
    byte wforward  = input & 0x0001;
    byte wright = (input >> 1) & 0x0001;
    byte wleft = (input >> 2) & 0x0001;
    byte robot = (input >> 8) & 0x0001;
    byte dir = ((unsigned)input >> 6) & 0x0003;
    byte treasure = (input >> 5) & 0x0001;
    byte tshape = (input >> 4) & 0x0001;
    byte tcolor =  (input >> 3) & 0x0001;
    byte output[]={wleft, wright, wforward, robot, treasure, tshape, tcolor, dir};
    for(int i =0;i<8;i++){
      Serial.print(",");Serial.print(output[i]);
    }
    Serial.println("");
    
    return output; 
}

int encoder(int &in1, int &in2, int &in3, int &in4, int &in5, int &in6){
    
    int output = (in5<<7|in6<<4|in4<<3|in3<<2|in2<<1|in1) & 0xBF;
    return output;
}
