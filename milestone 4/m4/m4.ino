/*
milestone 2 code with FSMs where we integrated line sensor, wall 
sensor, robot movement, microphone, and ir all in one.
*/

#define WAITTIME 800
#define BUTTON 7 //Digital pin for button

#include "robot.h"
#include "line_sensor.h"
//#include "fft_lib.h"
#include "radio.h"
#include "mux.h"
#include "maze.h"
#include "treasure.h"

byte dir; // move_dir

enum states : uint8_t
{
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_SENSE,
    ROBOT_DETECTED,
    ROBOT_TURN_LEFT,
    ROBOT_TURN_RIGHT,
    ROBOT_FORWARD,
    ROBOT_U_TURN,
    TRANSMIT,
    TREASURE_DECT
};

bool radio_msg_valid;
uint8_t STATE;
uint32_t u32wait;
uint32_t u32wait_ir;

uint16_t radio_msg = 0;
uint16_t FRONTWALL;
uint16_t LEFTWALL;
uint16_t RIGHTWALL;


void setup()
{
    Serial.begin(115200);
    line_sensor_init();
    radio_init(role_ping_out);
    STATE = START;
    u32wait = millis();
    mux_init();
    pinMode(BUTTON, INPUT_PULLUP);
    for (int i = 0; i < 9; i++)
    {
        for (int j = 0; j < 9; j++)
        {
            explored[i][j] = false;
            visited_array[i][j] = 0;
        }
    }
    robot_pos = 0x00; //x=0,y=8
    robot_dir = 1;    //down
    Serial.println(F("INITIAL STATE"));
    // print_robot_state;
    radio_msg_valid = false;
    //treasure_setup();
}

void loop()
{
    //Serial.print(F("State: "));Serial.println(STATE);
    switch (STATE)
    {
    case START:
        Serial.println(F("start"));
        //radio_msg = 0;
        //radio_msg = millis();
        //radio_transmit(radio_msg);

        delay(1000);
        STATE = AUDIO_DECT;

        break;

    case AUDIO_DECT:
//        calculate_FFT(MIC);
//        Serial.print(F("AUDIO SUM: "));
//        Serial.println(sum);
        if (!digitalRead(BUTTON)){
          STATE = IR_DECT;
          robot_init();
        }else
        {
            STATE = AUDIO_DECT;
        }
//        if ((sum > 150) || (!digitalRead(BUTTON)))
//        {
//            Serial.println(F("660Hz Tone Detected"));
//            STATE = IR_DECT;
//            robot_init();
//        }
//        else
//        {
//            STATE = AUDIO_DECT;
//        }
        //STATE = IR_DECT;
        //radio_msg = millis();
        //radio_transmit(radio_msg);
        break;

    case IR_DECT:
        //set_mux_select(1);
        //calculate_FFT(IR);
        //Serial.println("IN IR_DECT");
        //Serial.print(F("IR SUM: ")); Serial.println(sum);
        set_mux_select(2);
        FRONTWALL = analogRead(MUX_OUT);
        
//        if(FRONTWALL>300){
//            STATE=ROBOT_DETECTED;
//        }
//        else {
//            STATE = ROBOT_SENSE;
//        }
        STATE = ROBOT_SENSE;
        u32wait_ir = millis();
        // if (sum > 100)
        // {
        //     Serial.println(F("Robot Detected"));
        //     STATE = ROBOT_DETECTED;
        // }
        // else
        // {
        //     STATE = ROBOT_SENSE; //LINE_SENSOR;
        // }
        // u32wait_ir = millis();
        break;

    case ROBOT_SENSE:

        //structure: 000000vb ddtttrfl
        // d = direction robot will travel; t = treasure; b = robot
        // f = front wall; r = right wall; l = left wall; v = valid
        // we add a valid bit so that the robot wouldn't stall at intersection.
        // May consider queue.
        //Serial.println(F("IN ROBOT_SENSE"));
       // Serial.print(F("SENSOR_R READING: "));Serial.print(SENSOR_R_READING);
       // Serial.print(F("-----SENSOR_L READING: "));Serial.println(SENSOR_L_READING);

        if (AVERAGE_L < 200 && AVERAGE_R < 200 && VALID_L && VALID_R)
        {
            Serial.print(F("SENSOR_R READING: "));
            Serial.print(AVERAGE_R);
            Serial.print(F("-----SENSOR_L READING: "));
            Serial.println(AVERAGE_L);
            Serial.println(F("In intersection"));

            radio_msg = 0;

            // READ WALL SENSORS
            set_mux_select(3);
            LEFTWALL = analogRead(MUX_OUT);
            ///////////////// START TREASURE DETECTION CODE ////////////////////
            if (LEFTWALL > 250  )
            {
                robot_move(forward);
                //delay(50);
                int treasure = treasure_decode();
                radio_msg |= (treasure & 0x3) << 3;
            }
            ///////////////// END TREASURE DETECTION CODE ////////////////////
            robot_rest();
            set_mux_select(2);
            FRONTWALL = analogRead(MUX_OUT);
            Serial.print(F("FRONTWALL: "));
            Serial.print(FRONTWALL) ;
            set_mux_select(3);
            LEFTWALL = analogRead(MUX_OUT);
            Serial.print(F("---LEFTWALL: "));
            Serial.print(LEFTWALL);
            set_mux_select(4);
            RIGHTWALL = analogRead(MUX_OUT);
            Serial.print(F("---RIGHTWALL: "));
            Serial.println(RIGHTWALL);

            explored[(robot_pos >> 4) & 0x0f][(robot_pos)&0x0f] = true;
            visited_array[(robot_pos >> 4) & 0x0f][(robot_pos)&0x0f] = vcount;
            print_explored();
            print_robot_state();
            print_visited_array();

            // DECIDE WHICH WAY TO GO
            // DFS: search order forward, right, left, back
            if ((FRONTWALL < 100) && (!is_explored(0)))
            {
                dir = 0;
                Serial.println(F("Go Forward"));
                vcount++;
            }
            else if ((RIGHTWALL < 100) && (!is_explored(1)))
            {
                dir = 1;
                Serial.println(F("Turn Right"));
                vcount++;
            }
            else if ((LEFTWALL < 100) && (!is_explored(3)))
            {
                dir = 3;
                Serial.println(("Turn Left"));
                vcount++;
                //visited.push(robot_pos); // add robot_pos to stack
            }
            else
            {
                next_pos = find_next_square();
                dir = go_to(next_pos);
                //dir = 2;
                //Serial.println("Turn Around");
                Serial.println(F("Backtrack"));
                vcount = visited_array[(next_pos >> 4) & 0x0f][(next_pos)&0x0f];
            }
            // Check when and how vcount is set
            // Check assumptions made about vcount in functions that use it
            // Check when and how explored is set
            // Check when and how visited_array is set
            // Make sure the equation for move_dir in go_to is correct (subtraction)

            // CHANGE FSM STATE
            switch (dir)
            {       // switch statement for dir to update fsm state
            case 0: //forward
                STATE = ROBOT_FORWARD;
                break;
            case 1: //right
                STATE = ROBOT_TURN_RIGHT;
                break;
            case 2: //back
                STATE = ROBOT_U_TURN;
                break;
            case 3: //left
                STATE = ROBOT_TURN_LEFT;
                break;
            }

            // TRANSMIT TO BASESTATION
            //radio_msg = millis();
            
            radio_msg = 0xFFFF &radio_msg | (robot_pos << 8 | //setting valid bit.
                                  ((uint8_t)robot_dir << 6) | ((LEFTWALL > 115)) | (FRONTWALL > 115) << 1) |
                        ((RIGHTWALL > 115) << 2);
            /*completely re-work basestation communication?
              make sure dir in basestation is FRBL */

            // UPDATE ROBOT INFO VARIABLES
            //explored[(robot_pos>>4) & 0x0f][(robot_pos) & 0x0f] = true; --- was here
            update_robot_state(dir); // update robot_pos and robot_dir

            // RESET VARIABLES
            u32wait = millis();
            VALID_L = false;
            VALID_R = false;
            radio_msg_valid = true;
            robot_wake();
        }
        else
        {
            if (SENSOR_L_READING < 220)
            {
                robot_move(adj_right);
                //Serial.println("Adjust right");
            }
            else if (SENSOR_R_READING < 220)
            {
                robot_move(adj_left);
                //Serial.println("Adjust left");
            }
            else
            {
                robot_move(forward);
                //Serial.println("Moving");
            }
            if ((millis() - u32wait_ir) > WAITTIME)
            {
                STATE = IR_DECT;
            }
            else if (radio_msg_valid)
            {
                STATE = TRANSMIT;
            }
        }

        break;

    case ROBOT_DETECTED:
        robot_move(rstop);
        STATE = IR_DECT;
        break;

    case ROBOT_TURN_LEFT:
        robot_move(left);
        //Serial.println(F("TURNING LEFT"));
        if (millis() - u32wait > 750)
        {
            STATE = ROBOT_SENSE;
            // robot_rest(); //stop all functions
            // set_mux_select(3); //Check left wall
            // int reading = analogRead(MUX_OUT);
            // if(reading<100){
            //     treasure_decode();
            // }
            // robot_wake();
        }
        break;

    case ROBOT_TURN_RIGHT:
        robot_move(right);
        //Serial.println(F("TURNING RIGHT"));
        if (millis() - u32wait > 750)
        {
            STATE = ROBOT_SENSE;
        }
        break;

    case ROBOT_FORWARD:
        robot_move(forward);
        if (millis() - u32wait > 200)
        {
            STATE = ROBOT_SENSE;
        }
        break;

    case ROBOT_U_TURN:
        robot_move(right);
        if (millis() - u32wait > 1650)
        {
            STATE = ROBOT_SENSE;
        }
        break;

    case TRANSMIT:
        send_to_baseStation();

        STATE = ROBOT_SENSE;
        break;

    default:
        STATE = ROBOT_SENSE;
        Serial.println(F("state broken"));
        break;
    }
}


void send_to_baseStation()
{
    robot_move(rstop);
    servo_detach();
    line_sensor_detach();
    while (!radio_transmit(radio_msg))
        ;
    line_sensor_init();
    radio_msg_valid = false;
}

void robot_rest()
{
    robot_move(rstop);
    servo_detach();
    line_sensor_detach();
}
void robot_wake()
{
    line_sensor_init();
}
