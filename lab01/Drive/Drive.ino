#include <Servo.h>

int servo_right_pin = 5;
int servo_left_pin = 6;

Servo servo_right;
Servo servo_left;

void setup() {
  delay(2000);
  Serial.begin(9600);
  // put your setup code here, to run once:
  pinMode(servo_right_pin,OUTPUT);
  pinMode(servo_left_pin,OUTPUT);
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
  servo_right.write(180);
  servo_left.write(180);
  Serial.println("Done setup");
}

void loop() {
  // put your main code here, to run repeatedly:
      
//      for(int i =90;i<180;i++){
//        Serial.println(i);
//        servo_left.write(i);
//        delay(5000);
//        
//      }
        
  
  delay(1000);
}
