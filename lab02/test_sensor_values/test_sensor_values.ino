

#define SENSOR0_PIN 2
#define SENSOR1_PIN 3

#define WALL_FRONT A2
#define WALL_LEFT A3

uint32_t u32wait;
uint16_t FRONTWALL;
uint16_t LEFTWALL;

// These variables are marked 'volatile' to inform the compiler that they can change
// at any time (as they are set by hardware interrupts).
volatile long SENSOR0_TIMER;
volatile long SENSOR1_TIMER;

// Consider smoothing this value with your favorite smoothing technique (exponential moving average?)
volatile int SENSOR0_READING;
volatile int SENSOR1_READING;



// A digital write is required to trigger a sensor reading.
void setup_sensor(int pin, long *sensor_timer) {
  *sensor_timer = micros();
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  pinMode(pin, INPUT);
}



void SENSOR0_ISR() {
  // The sensor light reading is inversely proportional to the time taken
  // for the pin to fall from high to low. Lower values mean lighter colors.
  SENSOR0_READING = micros() - SENSOR0_TIMER;
  // Reset the sensor for another reading
  setup_sensor(SENSOR0_PIN, &SENSOR0_TIMER);
}



void SENSOR1_ISR() {
  SENSOR1_READING = micros() - SENSOR1_TIMER;
  setup_sensor(SENSOR1_PIN, &SENSOR1_TIMER);
}


uint16_t OverAnalogRead(uint8_t pin){
  byte TIMSK0_temp = TIMSK0;
    byte ADCSRA_temp = ADCSRA;
    byte ADMUX_temp = ADMUX;
    byte DIDR0_temp = DIDR0;
ADCSRA = 0xe7; // set the adc to free running mode
    DIDR0 = 0x30; 
  ADMUX = 0x40|pin; // use adc pin
  Serial.println("START");
  while(!(ADCSRA & 0x10)); // wait for adc to be ready
  ADCSRA = 0xf7; // restart adc
  byte m = ADCL; // fetch adc data
  byte j = ADCH;
  uint16_t k = (j << 8) | m; // form into an int
  k -= 0x0200; // form into a signed int
  k <<= 6; // form into a 16b signed int
  return k;

  TIMSK0 = TIMSK0_temp;
    ADCSRA = ADCSRA_temp;
    ADMUX = ADMUX_temp;
    DIDR0 = DIDR0_temp;
}


void setup() {
  Serial.begin(115200);

  // Tell the compiler which pin to associate with which ISR
  //attachInterrupt(digitalPinToInterrupt(SENSOR0_PIN), SENSOR0_ISR, LOW);
  //attachInterrupt(digitalPinToInterrupt(SENSOR1_PIN), SENSOR1_ISR, LOW);

  // Setup the sensors
  //setup_sensor(SENSOR0_PIN, &SENSOR0_TIMER);
  //setup_sensor(SENSOR1_PIN, &SENSOR1_TIMER);

}

void loop() {
    
    //FRONTWALL = analogRead(WALL_FRONT);
    //LEFTWALL = analogRead(WALL_LEFT);
    
    uint16_t SENSOR_LEFT = OverAnalogRead(5);
    uint16_t SENSOR_RIGHT = OverAnalogRead(4);
    //Serial.print(F("LEFT WALL READING: "));Serial.println(LEFTWALL);
    //Serial.print(F("FRONT WALL READING: "));Serial.println(FRONTWALL);
    Serial.print(F("SENSOR_LEFT READING: "));Serial.println(SENSOR_LEFT);
    Serial.print(F("SENSOR_RIGHT READING: "));Serial.println(SENSOR_RIGHT);
    //Serial.print(F("SENSOR0 READING: "));Serial.println(SENSOR0_READING);
    //Serial.print(F("SENSOR1 READING: "));Serial.println(SENSOR1_READING);
    Serial.println(" ");
    delay(500);
    
}
