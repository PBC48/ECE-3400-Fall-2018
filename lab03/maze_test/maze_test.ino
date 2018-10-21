void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
}

void loop() {
<<<<<<< HEAD
  // Run through a 2x3 maze
  Serial.println("reset");
  delay(1000);
  Serial.println("0,0,north=true,west=true");
  delay(1000);
  Serial.println("0,1,north=true");
  delay(1000);
  Serial.println("0,2,north=true,east=true");
  delay(1000);
  Serial.println("1,2,south=true,east=true");
  delay(5000);
  Serial.println("1,1,south=true");
  delay(1000);
  Serial.println("1,0,south=true,west=true");
=======
  // put your main code here, to run repeatedly:
  Serial.begin(115200);
  Serial.println("0,0,west=true,north=true,south=true");
  delay(1000);
  Serial.println("1,0,north=true,south=true");
  delay(1000);
  Serial.println("2,0,north=true,east=true");
  delay(1000);
  Serial.println("2,1,east=true,south=true");
  delay(1000);
  Serial.println("1,1,north=true,south=true");
  delay(1000);
  Serial.println("0,1,north=true,south=true,west=true");
>>>>>>> 894d39f623e1b1caadb324d92554557e52bba4e8
  delay(1000);
}
