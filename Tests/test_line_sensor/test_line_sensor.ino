#include "line_sensor.h"

void setup() {
  // put your setup code here, to run once:
  line_sensor_init();
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  for(int i=0;i<100;i++);
  Serial.print("SL: ");Serial.print(AVERAGE_L);
  Serial.print("--------SR: ");Serial.println(AVERAGE_R);
}
