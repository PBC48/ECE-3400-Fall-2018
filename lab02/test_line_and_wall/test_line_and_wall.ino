/*
milestone 2 code with FSMs where we integrated line sensor, wall 
sensor, robot movement, microphone, and ir all in one.
*/

#define WALL_FRONT 2
#define WALL_LEFT 3
#define WAITTIME 800

#define RED_LED 7
#define YELLOW_LED 8

#include "robot.h"
#include "line_sensor.h"
//#include "fft_lib.h"


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
    u32wait = millis();
     R.attach(ROBOT_RIGHT_PIN);
        L.attach(ROBOT_LEFT_PIN);
        robot_move(forward);
}

void loop() {
 
            FRONTWALL = analogRead(WALL_FRONT);
            LEFTWALL = analogRead(WALL_LEFT);
            
    
            if(SENSOR0_READING<55 && SENSOR1_READING<55){
                //Serial.print(F("SENSOR0 READING: "));Serial.println(SENSOR0_READING);
                //Serial.print(F("SENSOR1 READING: "));Serial.println(SENSOR1_READING);
                //Serial.print(F("LEFT WALL READING: "));Serial.println(LEFTWALL);
                //Serial.print(F("FRONT WALL READING: "));Serial.println(FRONTWALL);
                if(LEFTWALL < 200){
                    robot_move(left);
                    
                    //digitalWrite(RED_LED,HIGH);
                    
                    //digitalWrite(RED_LED,LOW);
                } else if (FRONTWALL > 115) {
                    robot_move(right);
                    //digitalWrite(YELLOW_LED,HIGH);
                    
                    //digitalWrite(YELLOW_LED,LOW);
                } else {
                    Serial.println("forward");
                    robot_move(forward);
                    
                }
                //robot_move(left);
                //delay(1000);
            }else if(SENSOR1_READING < 55){ //Sensor1 Threshold: 400   
                robot_move(adj_right);
            }else if(SENSOR0_READING < 55){ //Sensor0 Threshold: 50
                robot_move(adj_left);
            }else{
                //Serial.println("forward");
                robot_move(forward);
            }
}
