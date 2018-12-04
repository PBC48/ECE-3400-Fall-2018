/**
 * Updated basestation code to keep track of treasures and dynamic 
 * direction
 */

#include <SPI.h>
#include "nRF24L01.h"
#include "RF24.h"
#include "printf.h"
//
// Hardware configuration
//

// Set up nRF24L01 radio on SPI bus plus pins 9 & 10

RF24 radio(9, 10);

//
// Topology
//
int rows = 9; //need to adjust based on robot position
int cols = 9;
// Starting coordinates
//int x = 0;
//int y = 0;

// Radio pipe addresses for the 2 nodes to communicate.
const uint64_t pipes[2] = {0x0000000014LL, 0x0000000015LL};


// ROBOT MESSAGE
// [15:8] robot position; [7:6] robot direction; [5:3] treasure; [2:0] walls



enum r_dir
{
    unknown,
    up,
    right,
    left,
    down
};

r_dir robot_direction;
// The debug-friendly names of those roles
const char *role_friendly_name[] = {"invalid", "Ping out", "Pong back"};

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
    radio.setRetries(15, 15);
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
    radio.openReadingPipe(1, pipes[0]);
    //
    // Start listening
    //
    radio.startListening();
    //
    // Dump the configuration of the rf unit for debugging
    //
    radio.printDetails();
    Serial.println("reset");
}

void loop(void)
{
    uint16_t buff;
    // if there is data ready
    if (radio.available())
    {
        // Dump the payloads until we've gotten everything
        bool done = false;
        bool received = false;
        while (!done)
        {
            // Fetch the payload, and see if this was the last one.
            done = radio.read(&buff, sizeof(uint16_t));

            // Spew it
            printf("Got payload %d...\n", buff);
            GUI_com(buff);
        }
        
    }
    
}

void GUI_com(int buff){
    /*
    Current bit configuration:
 * [2:0] WALLS; true or false / right,front, left
 * [5:3] TREASURE; y/n, circle/square, red/blue
 * [7:6] ROBOT DIRECTION; forward, right, left
 * [8]   ROBOT; true or false
 * 
 * */
    bool wall_left, wall_right, wall_front, robot, west, east, north, south;
    uint8_t dir, treasure, x, y;

    wall_left = buff & 0x0001;
    wall_front = (buff >> 1) & 0x0001;
    wall_right = (buff >> 2) & 0x0001;
    treasure = (buff >> 3) & 0x0007;
    dir = (buff >> 6) & 0x0003;
    x = (buff>>12) & 0x0f;
    y = ((buff>>8) & 0x0f);
    
    
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
        //Serial.println("");
    //decode the absolute directions base on the robot's direction and input
    
    
    switch (dir)
    {
    case 1: //facing east, then left is north, right is south
        north = wall_right ;
        south = wall_left;
        west = false; //No wall behind robot
        east = wall_front;
        break;
    case 3: //facing west
        north = wall_left;
        south = wall_right;
        west = wall_front;
        east = false;
        break;
    case 0: //north
        north = false;
        south = wall_front ;
        west = wall_left;
        east = wall_right;
        break;
    case 2: //south
        north = wall_front;
        south = false;
        west = wall_right;
        east = wall_left;
        break;
    default:
        Serial.println("robot_direction not recognized");
    }

    Serial.print(y);
    Serial.print(",");
    Serial.print(x);
    if(west)
    Serial.print(",west=true");
    if(east)
    Serial.print(",east=true");
    if(north)
    Serial.print(",north=true");
    if(south)
    Serial.print(",south=true");
    

    /***
     * treasure detection. TODO need some conversions
     * */
    switch(treasure)
    {
        case 0: break;
        case 1: Serial.print(",tshape=triangle,tcolor=red");break;
        case 2: Serial.print(",tshape=square,tcolor=red");break;
        case 3: Serial.print(",tshape=diamond,tcolor=red");break;
        case 4: Serial.print(",tshape=triangle,tcolor=blue");break;
        case 5: Serial.print(",tshape=square,tcolor=blue");break;
        case 6: Serial.print(",tshape=diamond,tcolor=blue");break;
        default: break;
    }


    Serial.println("");
    /*switch (dir)
    {
    case 0: //forward : robot decided to go forward
        if (robot_direction == right)
        {
            x++;
        }
        else if (robot_direction == left)
        {
            x--;
        }
        else if (robot_direction == up)
        {
            y--;
        }
        else if (robot_direction == down)
        {
            y++;
        }
        break;
    case 1: //right : robot decided to turn right
        if (robot_direction == right)
        {
            y++;
            robot_direction = down;
        }
        else if (robot_direction == left)
        {
            y--;
            robot_direction = up;
        }
        else if (robot_direction == up)
        {
            x++;
            robot_direction = right;
        }
        else if (robot_direction == down)
        {
            x--;
            robot_direction = left;
        }
        break;
    case 2: //left : robot decides to turn left
        if (robot_direction == right)
        {
            y--;
            robot_direction = up;
        }
        else if (robot_direction == left)
        {
            y++;
            robot_direction = down;
        }
        else if (robot_direction == up)
        {
            x--;
            robot_direction = left;
        }
        else if (robot_direction == down)
        {
            x++;
            robot_direction = right;
        }
        break;
    case 3: //Uturn: robot reverses
        if (robot_direction == right)
        {
            x--;
            robot_direction = left;
        }
        else if (robot_direction == left)
        {
            x++;
            robot_direction = right;
        }
        else if (robot_direction == up)
        {
            y++;
            robot_direction = down;
        }
        else if (robot_direction == down)
        {
            y--;
            robot_direction = up;
        }

    default:
        Serial.println("ROBOT DIRECTION NOT RECOGNIZED");
    }*/
        
    
}
