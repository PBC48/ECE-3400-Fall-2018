

int pin = A0;

void setup(){
    Serial.begin(115200);
}

void loop(){
    int reading = analogRead(pin);
    Serial.println(reading);
}
