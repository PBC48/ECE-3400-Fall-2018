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
    LINE_SENSOR
};

uint8_t STATE;
uint32_t u32wait;
uint16_t FRONTWALL;
uint16_t LEFTWALL;

void toggle_LED(uint8_t &pin){
    
    int set = digitalRead(pin) ? LOW : HIGH;
    digitalWrite(pin, set);
    pinMode(WALL_FRONT, INPUT_PULLUP);
    pinMode(WALL_LEFT, INPUT_PULLUP);
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
            FFT_device_init(MIC);
            calculate_FFT(MIC);
            Serial.print(F("AUDIO SUM: "));Serial.println(sum);
            if(sum>40){ //originally at 90
                Serial.println(F("660Hz Tone Detected"));
                STATE = IR_DECT;
            }else{
                STATE = AUDIO_DECT;
            }
            
            break;
        
        case IR_DECT:
            FFT_device_init(IR);
            calculate_FFT(IR);
            Serial.print(F("IR SUM: ")); Serial.println(sum);
            if(sum > 55){
                Serial.println(F("Robot Detected"));
                STATE = ROBOT_DETECTED;
            }else{
                STATE = LINE_SENSOR;//ROBOT_SENSE;
            }
            u32wait = millis();
            break;

        case ROBOT_SENSE:
        
            Serial.println(F("IN ROBOT_SENSE"));
        
            Serial.print(F("SENSOR0 READING: "));Serial.println(SENSOR0_READING);
            Serial.print(F("SENSOR1 READING: "));Serial.println(SENSOR1_READING);
            if(SENSOR0_READING<400 && SENSOR1_READING<400){ //Sensor0 Threshold: 50; Sensor1 Threshold: 400
                if(LEFTWALL < 200){
                    robot_move(left);
                    // digitalWrite(RED_LED,HIGH);
                    delay(1200);
                    // digitalWrite(RED_LED,LOW);
                } else if (FRONTWALL > 170) {
                    robot_move(right);
                    // digitalWrite(YELLOW_LED,HIGH);
                    delay(1200);
                    // digitalWrite(YELLOW_LED,LOW);
                } else {
                    robot_move(forward);
                }
            }else if(SENSOR1_READING < 400){ //Sensor1 Threshold: 400   
                robot_move(adj_right);
            }else if(SENSOR0_READING < 400){ //Sensor0 Threshold: 50
                robot_move(adj_left);
            }else{
                robot_move(forward);
            }
            if((millis()-u32wait) > WAITTIME){
                robot_move(stop);
                STATE = IR_DECT;
            }else{
                STATE = ROBOT_SENSE;
            } 
            break;
        
        
        case ROBOT_DETECTED:
            robot_move(stop);
            STATE = IR_DECT;
            break;

        case LINE_SENSOR:
        
            FRONTWALL = OverAnalogRead(WALL_FRONT);
            //LEFTWALL = analogRead(WALL_LEFT);
            //Serial.print(F("FRONTWALL: "));Serial.println(FRONTWALL);
            Serial.print(F("SENSOR0: "));Serial.println(SENSOR0_READING);
            Serial.print(F("SENSOR1: "));Serial.println(SENSOR1_READING);
            break;
            
        default:
            STATE = ROBOT_SENSE;
            Serial.println(F("state broken"));
            break;
    }
}


