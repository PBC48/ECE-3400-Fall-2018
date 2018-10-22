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
#include "maze.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9,10);

//
// Topology
//
int rows = 2;
int cols = 3;
// Starting coordinates
int x = 0;
int y = 0;
// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = { 0x0000000014LL, 0x0000000015LL };
// The map of the stage
//int map1[2][3];
enum r_dir{
    up,
    right,
    left,
    down
};

r_dir robot_direction;
// The debug-friendly names of those roles
const char* role_friendly_name[] = { "invalid", "Ping out", "Pong back"};



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
  radio.setPALevel(RF24_PA_MIN);
  //RF24_250KBPS for 250kbs, RF24_1MBPS for 1Mbps, or RF24_2MBPS for 2Mbps
  radio.setDataRate(RF24_250KBPS);

  // optionally, reduce the payload size.  seems to
  // improve reliability
  //radio.setPayloadSize(2);

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
  uint16_t *buff;
  //
  // Pong back role.  Receive each packet, dump it out, and send it back
  //

    // if there is data ready
    if ( radio.available() )
    {
      // Dump the payloads until we've gotten everything
      
      bool done = false;
      while (!done)
      {
        // Fetch the payload, and see if this was the last one.
        done = radio.read( buff, sizeof(uint16_t) );
      
        // Spew it
        printf("Got payload %lu...",*buff);
        // Delay just a little bit to let the other unit
        // make the transition to receiver
        delay(20);

      }
      
      
      // First, stop listening so we can talk and process
      radio.stopListening();

      // Decode message
      //map[x][y] =       
      // Send the final one back.
      radio.write( &buff, sizeof(uint16_t) );
     
      // Now, resume listening so we catch the next packets.
      radio.startListening();
    }
  

    
    int * output = decoder(*buff);
    bool north, east,west,south,robot;
    int direction,tcolor,tshape;
    west = output[0];
    north  = output[1];
    east = output[2];
    south = output[3];
    robot = output[4];
    tshape = output[5];
    tcolor = output[6];
    direction = output[7];

    Serial.print(x);Serial.print(",");Serial.print(y);
    Serial.print(",west=");Serial.print(west);
    Serial.print(",east=");Serial.print(east);
    Serial.print(",north=");Serial.print(north);
    Serial.print(",south=");Serial.println(south);
    Serial.print(",tshape=");
    if(tshape)
        Serial.print("circle");
    else
        Serial.print("square");
    Serial.print(",tcolor=");
    if(tcolor)
        Serial.print("red");
    else
        Serial.print("blue");
    Serial.println("");

    switch (direction){
        case 0: //forward : robot decided to go forward
            if(robot_direction==right){
                x++;
            }else if(robot_direction==left){
                x--;
            }else if(robot_direction==up){
                y++;
            }else if(robot_direction==down){
                y--;
            }
            break;
        case 1: //right : robot decided to turn right
            if(robot_direction==right){
                y--;
                robot_direction = down;
            }else if(robot_direction==left){
                y++;
                robot_direction = up;
            }else if(robot_direction==up){
                x++;
                robot_direction = right;
            }else if(robot_direction==down){
                x--;
                robot_direction = left;
            }
            break;
        case 2: //left : robot decides to turn left
            if(robot_direction==right){
                y++;
                robot_direction = up;
            }else if(robot_direction==left){
                y--;
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

  
  //added extra delay: but remove when integrate
  delay(500);
}
