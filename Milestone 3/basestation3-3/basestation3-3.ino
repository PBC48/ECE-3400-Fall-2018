  /*
 Copyright (C) 2011 J. Coliz <maniacbug@ymail.com>
 This program is free software; you can redistribute it and/or
 modify it under the terms of the GNU General Public License
 version 2 as published by the Free Software Foundation.
 */

/**
 * Example for Getting Started with nRF24L01+ radios.
 *
 * This is an example of how to use the RF24 class.  Write this sketch to two
 * different nodes.  Put one of the nodes into 'transmit' mode by connecting
 * with the serial monitor and sending a 'T'.  The ping node sends the current
 * time to the pong node, which responds by sending the value back.  The ping
 * node can then see how long the whole cycle took.
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//
int rows = 2; //need to adjust based on robot position
int cols = 3; 
// Starting coordinates
int x = 0;
int y = 0;
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000014LL, 0x0000000015LL };
// The map of the stage

//int map1[2][3];
int map1[2][3] = {0};

/**
 *  |0,0,0|
 *  |0,0,0|
*/

enum r_dir{
    up,
    right,
    left,
    down
};

r_dir robot_direction;
// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};

/**
 * 4 bits used: left,right,up,down
*/
uint8_t getResp(){
    uint8_t up,down,left,right;
    up = (y>0) ? map1[x][y-1] : 1;
    down = (y<(rows-1)) ? map1[x][y+1] : 1;
    left = (x>0) ? map1[x-1][y] : 1;
    right = (y<(cols-1)) ? map1[x+1][y] : 1;
    return ((left<<3)|(right<<2)|(up<<1)|(down)) & 0x0F;
}


void setup(void)
{
  //
  // Print preamble
  //

  Serial.begin(9600);
  printf_begin();
  //
  // Setup and configure rf radio
  //

  radio.begin();

  // optionally, increase the delay between retries & # of retries
  radio.setRetries(15,15);
  radio.setAutoAck(true);
  // set the channel
  radio.setChannel(0x50);
  // set the power
  // RF24_PA_MIN=-18dBm, RF24_PA_LOW=-12dBm, RF24_PA_MED=-6dBM, and RF24_PA_HIGH=0dBm.
  radio.setPALevel(RF24_PA_HIGH);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(8);

  //
  // Open pipes to other nodes for communication
  //

  // This simple sketch opens two pipes for these two nodes to communicate
  // back and forth.
  // Open 'our' pipe for writing
  // Open the 'other' pipe for reading, in position #1 (we can have up to 5 pipes open for reading)
    radio.openWritingPipe(pipes[1]);
    radio.openReadingPipe(1,pipes[0]);
  //
  // Start listening
  //
  radio.startListening();
  //
  // Dump the configuration of the rf unit for debugging
  //
  radio.printDetails();
  robot_direction = right;
}



void loop(void)
{
  //
  // Ping out role.  Repeatedly send the current time
  //
  uint16_t buff;
  uint8_t * resp;
  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      map1[x][y]=1; //Explored robot
      bool done = false;
      bool received = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( &buff, sizeof(uint16_t) );
      
        // Spew it
        printf("Got payload %d...\n",buff);
        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);

      }
      
      *resp = getResp();
      *resp = *resp & 0x000F;
      // First, stop listening so we can talk and process
      radio.stopListening();
        
      // Decode message
      //map[x][y] =       
      // Send the final one back.
      //while(!received){
        //received = radio.write( &buff, sizeof(uint16_t) );
        received = radio.write( &resp, sizeof(uint8_t) );
        delay(20);
      //}
      
      // Now, resume listening so we catch the next packets.
      radio.startListening();
    
    
    //byte * output = decoder(buff);
    /*for(int i =0;i<8;i++){
      Serial.print(",");Serial.print(output[i]);
    }
    Serial.println("");*/
    //Decoding
    bool wall_left,wall_right, wall_front, robot,treasure, west,east,north,south;
    uint8_t dir,tcolor,tshape;
    wall_left   = (buff >> 2) & 0x0001;
    wall_front  = buff & 0x0001;
    wall_right  = (buff >> 1) & 0x0001;
    treasure = (buff >> 3) & 0x0007;
    dir = ((unsigned)buff >> 6) & 0x0003;
    robot  = (buff >> 8) & 0x0001;

/*
    Serial.print("direction=");Serial.print(dir);
    Serial.print(",wall_left=");Serial.print(wall_left);
    Serial.print(",wall_front=");Serial.print(wall_front);
    Serial.print(",wall_right=");Serial.print(wall_right);
    Serial.print(",robot_direction=");Serial.print(robot_direction);
    */
    /*Serial.print(",treasure=");Serial.print(treasure);
    Serial.print(",tshape=");Serial.print(tshape);
    Serial.print(",tcolor=");Serial.print(tcolor);
    Serial.print(",robot=");Serial.println(robot);*/
    Serial.println("");
    //decode the absolute directions base on the robot's direction and input
    switch(robot_direction){
        case right: //facing right, then left is north, right is south
            north = wall_left;
            south = wall_right;
            west  = false; //No wall behind robot
            east  = wall_front;
            break;
        case left:
            north = wall_right;
            south = wall_left;
            west  = wall_front;
            east  = false;
            break;
        case up:
            north = wall_front;
            south = false;
            west  = wall_left;
            east  = wall_right;
            break;
        case down:
            north = false;
            south = wall_front;
            west  = wall_right;
            east  = wall_left;
            break;
        default:
            Serial.println("robot_direction not recognized");
    }

    Serial.print(x);Serial.print(",");Serial.print(y);
    Serial.print(",west=");Serial.print(west);
    Serial.print(",east=");Serial.print(east);
    Serial.print(",north=");Serial.print(north);
    Serial.print(",south=");Serial.print(south);
    /*if(treasure!=0){
        Serial.print(",tshape=");
        if(treasure>=0 && treasure <3)
            Serial.print("circle");
        else
            Serial.print("square");
        Serial.print(",tcolor=");
        if(tcolor)
            Serial.print("red");
        else
            Serial.print("blue");
    }*/
    Serial.println("");
    switch (dir){
        case 0: //forward : robot decided to go forward
            if(robot_direction==right){
                x++;
            }else if(robot_direction==left){
                x--;
            }else if(robot_direction==up){
                y--;
            }else if(robot_direction==down){
                y++;
            }
            break;
        case 1: //right : robot decided to turn right
            if(robot_direction==right){
                y++;
                robot_direction = down;
            }else if(robot_direction==left){
                y--;
                robot_direction = up;
            }else if(robot_direction==up){
                x++;
                robot_direction = right;
            }else if(robot_direction==down){
                x--;
                robot_direction = left;
            }
            break;
        case 2: //back : robot decides to turn around
        // WORK THIS OUT
            if(robot_direction==right){
              x--;
              robot_direction = left;
            }else if(robot_direction==left){
              x++;
              robot_direction = right;
            }else if(robot_direction==up){
              y++;
              robot_direction = down;
            }else if(robot_direction==down){
              y--;
              robot_direction = up;
            }
            break;
        case 3: //left : robot decides to turn left
            if(robot_direction==right){
                y--;
                robot_direction = up;
            }else if(robot_direction==left){
                y++;
                robot_direction = down;
            }else if(robot_direction==up){
                x--;
                robot_direction = left;
            }else if(robot_direction==down){
                x++;
                robot_direction = right;
            }
            break;

        default:
            Serial.println("ROBOT DIRECTION NOT RECOGNIZED");
        
    }
    //free(output);
    }
  //added extra delay: but remove when integrate
  //delay(500);
}
