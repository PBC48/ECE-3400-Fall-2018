#include <Servo.h>
#include "line_sensor.h"


char LINE_PIN1;
char LINE_PIN2;
char LINE_PIN3;
char servo_right_pin = 9;
char servo_left_pin = 10;

Servo servo_right;
Servo servo_left;

char ROBOT_SPEED;


void robot_init(){
  pinMode(servo_right_pin,OUTPUT);
  pinMode(servo_left_pin,OUTPUT);
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
}

void robot_forward(){
  servo_right.write(ROBOT_SPEED);
  servo_left.write(abs(ROBOT_SPEED-180));
}

void robot_turn(int dir)
{
  if(dir){
    servo_right.write(0);
    servo_left.write(0);
  }else{
    servo_right.write(180);
    servo_left.write(180);
  }
  
}

int readQD(){
  //Returns value from the QRE1113
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( QRE1113_Pin, OUTPUT );
  digitalWrite( QRE1113_Pin, HIGH );
  delayMicroseconds(10);
  pinMode( QRE1113_Pin, INPUT );

  long time = micros();
  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while (digitalRead(QRE1113_Pin) == HIGH && micros() – time < 3000); 
  int diff = micros() - time; 
  return diff; 
}


void setup() {
  Serial.begin(19200);
  pinMode(LED_BUILTIN,OUTPUT);
  delay(2000);
  digitalWrite(LED_BUILTIN,OUTPUT);
  // put your setup code here, to run once:
  robot_init();
  setup_sensor(LINE_PIN1,&SENSOR0_TIMER,0);
  setup_sensor(LINE_PIN2,&SENSOR1_TIMER,1);
}


void loop() {
  // put your main code here, to run repeatedly:
  int reading = 
  
  if(SENSOR0_READING>0){
    robot_turn(1);
  }else if(SENSOR1_READING>0){
    robot_turn(0);
  }else{
    robot_forward();
  }

  
  
}
