/*
  LED Analog

  Light an external LED with "analog" voltage values (using PWM)

  ECE 3400, Team 7
*/


// Define analog input and output pins
#define PWMPIN 10
#define APIN A5


void setup() {
  // initialize digital pin as an output
  pinMode(PWMPIN, OUTPUT);

  // establish serial connection with board
  Serial.begin(115200);
}


void loop() {
  int value = analogRead(APIN); // Read and store input value from analog pin
  int writeVal = value / 4; // Adjust the value (to write to LED)
  analogWrite(PWMPIN, writeVal);   // turn the LED on to analog voltage value
  Serial.println(value);      // print value written to LED to serial monitor
  
  delay(500);                       // wait for half a second
}
