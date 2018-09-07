#include <Servo.h>

Servo servo;
int POTEN_PIN = A1;

void setup() {
  Serial.begin(9600);
  pinMode(3,OUTPUT);
  servo.attach(3);
  // put your setup code here, to run once:
  
}

void loop() {
  int amount = analogRead(POTEN_PIN);
  Serial.println(amount);
  servo.write(amount/6);
}
