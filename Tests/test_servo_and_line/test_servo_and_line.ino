#include "robot.h"
#include "line_sensor.h"

void setup(){
    Serial.begin(9600);
    robot_init();
    line_sensor_init();
}


void loop(){
  Serial.print("SL: ");Serial.print(AVERAGE_L);
  Serial.print("--------SR: ");Serial.println(AVERAGE_R);
    robot_calibrate();
}
