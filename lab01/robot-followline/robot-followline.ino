#include <Servo.h>

#include <cmath>

int servo_right_pin = 9;
int servo_left_pin = 10;

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

void setup() {
  pinMode(LED_BUILTIN,OUTPUT);
  delay(2000);
  digitalWrite(LED_BUILTIN,OUTPUT);
  // put your setup code here, to run once:
  robot_init();
}


void loop() {
  // put your main code here, to run repeatedly:
  //delay(1000);
}
