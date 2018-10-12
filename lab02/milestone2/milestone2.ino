/*
milestone 2 code with FSMs where we integrated line sensor, wall 
sensor, robot movement, microphone, and ir all in one.
*/

#define WALL_FRONT A1
#define WALL_LEFT A2

#include "robot.h"
#include "line_sensor.h"
#include "fft_lib.h"

enum states {
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_MOVE,
    ROBOT_SENSE,
    ROBOT_DETECTED
};

uint8_t STATE;
uint32_t u32wait;
uint32_t WAITTIME = 800;

void setup() {
  Serial.begin(115200);
    line_sensor_init();
    robot_init();
    STATE = START;
    u32wait = millis();
}

void loop() {
    Serial.print("State: ");Serial.println(STATE);
    switch (STATE){
        case START:
            Serial.println("start");
            STATE = AUDIO_DECT;
            break;
        
        case AUDIO_DECT:
            microphone_set();
            calculate_FFT();
            if(sum>90){
                Serial.println("660Hz Tone Detected");
                STATE = IR_DECT;
            }else{
                STATE = AUDIO_DECT;
            }
            sum = 0;
            break;
        
        case IR_DECT:
            ir_set();
            calculate_FFT();
            if(sum > 55){
                Serial.println("Robot Detected");
                STATE = ROBOT_DETECTED;
            }else{
                STATE = ROBOT_SENSE;
            }
            sum = 0;
            u32wait = millis();
            break;

        case ROBOT_SENSE:
        {
            uint16_t FRONTWALL = analogRead(WALL_FRONT);
            uint16_t LEFTWALL = analogRead(WALL_LEFT);
            if(SENSOR0_READING<400 && SENSOR1_READING<400){
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
            }else if(SENSOR1_READING < 400){ //turning right
                robot_move(adj_right);
            }else if(SENSOR0_READING < 400){
                robot_move(adj_left);
            }else{
                robot_move(forward);
                STATE = (millis()-u32wait) > WAITTIME ? IR_DECT : ROBOT_SENSE; 
            }
            break;
        }
        
        case ROBOT_DETECTED:
            robot_move(stop);
            STATE = IR_DECT;
            break;
    }
}


