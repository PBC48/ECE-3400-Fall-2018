
#include "Line_sensor.h"
#include "Robot.h"
/**
Pin definitions 
*/
uint8_t SENSOR3_PIN = 4;

uint8_t servo_right_pin = 9;
uint8_t servo_left_pin = 10;

Robot robot(servo_right_pin,servo_left_pin);

int i=0;
int map1[] = {right,right,left,left,left,left,right,right};


uint8_t state=0;
unsigned long wait;
uint32_t poll_result=0xFFFFFFFF;

void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  delay(2000);
  digitalWrite(LED_BUILTIN,LOW);
  // put your setup code here, to run once:
  setup_sensor(SENSOR0_PIN,&SENSOR0_TIMER,0);
  setup_sensor(SENSOR1_PIN,&SENSOR1_TIMER,1);
  wait = millis()+200;
}


void loop() {
  switch(state){
    case 0:
      robot.forward();
      if(SENSOR0_READING>100){
        state++;
      }else if(SENSOR1_READING>100){
        state = 2;
      }
      if(millis()-wait>=0){
        state = 3;
        wait=millis()+200;
      }else if(poll_result<85){
        state = 4;
      }
      break;
    
    case 1://turn right
      robot.turn(right);
      state = 0;
      break;
    
    case 2: //turn left
      robot.turn(left);
      state = 0;
      break;
    
    case 3:    
      poll_result = pollLineSensor(SENSOR3_PIN);
      state = 0;
      break;
    
    case 4:
      robot.turn(map1[i++]);
      if(i>=8){
        i=0;
      }
      wait=millis()+500;
      if(millis()-wait>=0){
        state = 0;
      }
      poll_result=0xFFFFFFFF;
      break;
  }
  
}


