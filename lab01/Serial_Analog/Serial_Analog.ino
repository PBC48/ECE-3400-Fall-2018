/*
  Serial Analog

  Reads a value from an analog input pin and prints it on the serial monitor

  ECE 3400, Team 7
*/

// define analog pin
#define APIN A5


void setup() {
  // initialize analog pin as an input
  pinMode(APIN, INPUT);

  // establish serial connection with board
  Serial.begin(115200);
}


void loop() {
  int value = analogRead(APIN);   // Read the input value from the analog pin
  Serial.println(value);          // Display the value on the serial monitor
  
  delay(500);                     // Wait for half a second
}
