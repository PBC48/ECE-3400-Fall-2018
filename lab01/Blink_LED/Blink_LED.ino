/*
  Blink

  Turns an external LED on for one second, then off for one second, repeatedly.

  ECE 3400, Team 7
*/

// Define digital pin
#define DPIN 13


void setup() {
  // initialize digital pin as an output
  pinMode(DPIN, OUTPUT);
}


void loop() {
  digitalWrite(DPIN, HIGH);   // turn the LED on (HIGH is the voltage level)
  delay(1000);                       // wait for a second
  digitalWrite(DPIN, LOW);    // turn the LED off by making the voltage LOW
  delay(1000);                       // wait for a second
}
