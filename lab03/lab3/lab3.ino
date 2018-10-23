/*
milestone 2 code with FSMs where we integrated line sensor, wall 
sensor, robot movement, microphone, and ir all in one.
*/

#define WALL_FRONT A2
#define WALL_LEFT A3
#define WAITTIME 800
#define BUTTON 7 //Digital pin for button

#include "robot.h"
#include "line_sensor.h"
#include "fft_lib.h"
//#include "radio.h"
//#include "maze.h"

enum states : uint8_t {
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_SENSE,
    ROBOT_DETECTED,
    ROBOT_TURN_LEFT,
    ROBOT_TURN_RIGHT
};

uint8_t STATE;
uint32_t u32wait;
uint32_t u32wait_ir;
uint16_t radio_msg = 0;
uint16_t FRONTWALL;
uint16_t LEFTWALL;

void toggle_LED(uint8_t &pin){
    
    int set = digitalRead(pin) ? LOW : HIGH;
    digitalWrite(pin, set);
}

void setup() {
  Serial.begin(57600); // Not 115200?
    line_sensor_init();
    robot_init();
    //radio_init(role_ping_out);
    STATE = START;
    u32wait = millis();
}
uint32_t R_READING;
uint32_t L_READING;
void loop() {
    //Serial.print(F("State: "));Serial.println(STATE);
    switch (STATE){
        case START:
            Serial.println(F("start"));
            //radio_msg = 0;
            //radio_msg = 0x1FF & (1<<6|0<<2);
            //radio_transmit(radio_msg);
            
            delay(1000);
            STATE = AUDIO_DECT;
            
            break;
        
        case AUDIO_DECT:
            /*calculate_FFT(MIC);
            Serial.print(F("AUDIO SUM: "));Serial.println(sum);
            if(sum>85) { //|| digitalRead(BUTTON)){ //originally at 90
                Serial.println(F("660Hz Tone Detected"));
                STATE = IR_DECT;
            }else{
                STATE = AUDIO_DECT;
            }
            //radio_msg = 0x1FF & (1<<6|0<<2);
            //radio_transmit(radio_msg);*/
            STATE = IR_DECT;
            break;
        
        case IR_DECT:
            calculate_FFT(IR);
            //Serial.println("IN IR_DECT");
            //Serial.print(F("IR SUM: ")); Serial.println(sum);
            if(sum > 50){
                Serial.println(F("Robot Detected"));
                STATE = ROBOT_DETECTED;
            }else{
                STATE = ROBOT_SENSE;//LINE_SENSOR;
            }
            u32wait_ir = millis();
            break;

        case ROBOT_SENSE:
            radio_msg = 0;
                //structure: 0000000b ddtttrfl
                // d = direction robot will travel; t = treasure; b = robot
                // f = front wall; r = right wall; l = left wall
            FRONTWALL = analogRead(WALL_FRONT);
            LEFTWALL = analogRead(WALL_LEFT);
            //Serial.println(F("IN ROBOT_SENSE"));
            //Serial.print(F("SENSOR_R READING: "));Serial.println(SENSOR_R_READING);
            //Serial.print(F("SENSOR_L READING: "));Serial.println(SENSOR_L_READING);
            //Serial.print(F("FRONTWALL: "));Serial.println(FRONTWALL);
            //Serial.print(F("LEFTWALL: "));Serial.println(LEFTWALL);
            
             R_READING = SENSOR_R_READING;
             L_READING = SENSOR_L_READING;
            if(SENSOR_R_READING<200 && SENSOR_R_READING<200){ //Sensor_R Threshold: 50; Sensor_L Threshold: 400
                Serial.println("In intersection");
                //Serial.print(F("SENSOR_R READING: "));Serial.println(SENSOR_R_READING);
                //Serial.print(F("SENSOR_L READING: "));Serial.println(SENSOR_L_READING);
                byte dir;
                if(LEFTWALL < 200){
                    dir = 2;
                    STATE = ROBOT_TURN_LEFT;
                } else if (FRONTWALL > 115) {
                    dir = 1;
                    STATE = ROBOT_TURN_RIGHT;
                } else {
                    dir = 0;
                    robot_move(forward);
                    //delay(200);
                }
                
                radio_msg = radio_msg | (dir << 6) | ((LEFTWALL > 200)<<2) | (FRONTWALL > 115); //| ((RIGHTWALL > ###)<<1);

                //robot_move(rstop);
                //radio_transmit(radio_msg);
                u32wait = millis();
            }   
            else if(SENSOR_L_READING < 200){ //Sensor_L Threshold: 400   
                robot_move(adj_right);
            }else if(SENSOR_R_READING < 200){ //Sensor_R Threshold: 50
                robot_move(adj_left);
            }else{
                robot_move(forward);
            }
                
            if((millis()-u32wait_ir) > WAITTIME){
                    u32wait_ir = millis();
                    STATE = IR_DECT;
            }
            break;
              
        case ROBOT_DETECTED:
            robot_move(rstop);
            STATE = IR_DECT;
            break;

        case ROBOT_TURN_LEFT:
            robot_move(left);
            if(millis()-u32wait>700){
                STATE = ROBOT_SENSE;
            }
            break;

        case ROBOT_TURN_RIGHT:
            robot_move(right);
            if(millis()-u32wait>700){
                STATE = ROBOT_SENSE;
            }
            break;
            
        default:
            STATE = ROBOT_SENSE;
            Serial.println(F("state broken"));
            break;
    }
}

