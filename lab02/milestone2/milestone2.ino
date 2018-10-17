/*
milestone 2 code with FSMs where we integrated line sensor, wall 
sensor, robot movement, microphone, and ir all in one.
*/

#define WALL_FRONT 2
#define WALL_LEFT 3
#define WAITTIME 800

#include "robot.h"
#include "line_sensor.h"
#include "fft_lib.h"

enum states : uint8_t {
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_SENSE,
    ROBOT_DETECTED,
    LINE_SENSOR,
    ROBOT_TURN_LEFT,
    ROBOT_TURN_RIGHT
};

uint8_t STATE;
uint32_t u32wait;
uint32_t u32wait_ir;
uint16_t FRONTWALL;
uint16_t LEFTWALL;
bool turn_var = false;

void toggle_LED(uint8_t &pin){
    
    int set = digitalRead(pin) ? LOW : HIGH;
    digitalWrite(pin, set);
}

void setup() {
  Serial.begin(115200);
    line_sensor_init();
    robot_init();
    STATE = START;
    u32wait = millis();
}

void loop() {
    Serial.print(F("State: "));Serial.println(STATE);
    switch (STATE){
        case START:
            Serial.println(F("start"));
            STATE = AUDIO_DECT;
            
            break;
        
        case AUDIO_DECT:
            /*
            calculate_FFT(MIC);
            Serial.print(F("AUDIO SUM: "));Serial.println(sum);
            if(sum>40){ //originally at 90
                Serial.println(F("660Hz Tone Detected"));
                STATE = IR_DECT;
            }else{
                STATE = AUDIO_DECT;
            }*/
            STATE = IR_DECT;
            break;
        
        case IR_DECT:
            calculate_FFT(IR);
            Serial.println("IN IR_DECT");
            Serial.print(F("IR SUM: ")); Serial.println(sum);
            if(sum > 55){
                Serial.println(F("Robot Detected"));
                STATE = ROBOT_DETECTED;
            }else{
                STATE = ROBOT_SENSE;//LINE_SENSOR;
            }
            STATE = ROBOT_SENSE;
            break;

        case ROBOT_SENSE:
            FRONTWALL = analogRead(WALL_FRONT);
            LEFTWALL = analogRead(WALL_LEFT);
            //Serial.println(F("IN ROBOT_SENSE"));
            //Serial.print(F("SENSOR_R READING: "));Serial.println(SENSOR_R_READING);
            //Serial.print(F("SENSOR_L READING: "));Serial.println(SENSOR_L_READING);
            if(SENSOR_R_READING<200 && SENSOR_L_READING<200){ //Sensor_R Threshold: 50; Sensor_L Threshold: 400
                if(LEFTWALL < 200){
                    u32wait = millis();
                    STATE = ROBOT_TURN_LEFT;
                } else if (FRONTWALL > 115) {
                    u32wait = millis();
                    STATE = ROBOT_TURN_RIGHT;
                } else {
                    robot_move(forward);
                }
            }else if(SENSOR_L_READING < 200){ //Sensor_L Threshold: 400   
                robot_move(adj_right);
            }else if(SENSOR_R_READING < 200){ //Sensor_R Threshold: 50
                robot_move(adj_left);
            }else{
              robot_move(forward);
            }
                
            if((millis()-u32wait_ir) > WAITTIME){
                    robot_move(rstop);
                    u32wait_ir = millis();
                    STATE = IR_DECT;
            }else{
                    STATE = ROBOT_SENSE;
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


