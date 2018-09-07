#include <Servo.h>

/*
  Servo Analog

  Writes an analog output voltage to a servo based on an input analog voltage 

  ECE 3400, Team 7
*/

// Define analog output and input pins
#define SERVO 10
#define APIN A5

// Define servo
Servo myservo;


void setup() {
  // initialize digital pin as an output
  pinMode(SERVO, OUTPUT);

  // establish serial connection with board
  Serial.begin(115200);

  // attach servo to pin
  myservo.attach(10);
}


void loop() {
  float value = analogRead(APIN); // Read and store input value from analog pin
  float writeVal = value / 1023 * 180; // Adjust analog value (for writing to servo)
  myservo.write(writeVal);   // Write adjusted analog value to servo
  Serial.println(writeVal); // print value written to servo to serial monitor
  
  delay(10);                       // wait for ten milliseconds
}
