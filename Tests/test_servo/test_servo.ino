#include "robot.h"


void setup(){
    Serial.begin(9600);
    robot_init();
}


void loop(){
    robot_calibrate();
}