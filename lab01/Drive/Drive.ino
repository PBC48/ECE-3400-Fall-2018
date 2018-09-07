#include <Servo.h>

int servo_right_pin = 9;
int servo_left_pin = 10;

Servo servo_right;
Servo servo_left;

void setup() {
  delay(2000);
  // put your setup code here, to run once:
  pinMode(servo_right_pin,OUTPUT);
  pinMode(servo_left_pin,OUTPUT);
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
}

void loop() {
  // put your main code here, to run repeatedly:
      servo_right.write(80);
      servo_left.write(80);
      delay(667);
      servo_right.write(100);
      servo_left.write(80);
      delay(333);
  
  //delay(1000);
}
