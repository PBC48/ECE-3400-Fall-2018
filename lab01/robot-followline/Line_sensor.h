



#define SENSOR0_PIN 2
#define SENSOR1_PIN 3


// These variables are marked 'volatile' to inform the compiler that they can change
// at any time (as they are set by hardware interrupts).
volatile long SENSOR0_TIMER;
volatile long SENSOR1_TIMER;

// Consider smoothing this value with your favorite smoothing technique (exponential moving average?)
volatile int SENSOR0_READING;
volatile int SENSOR1_READING;


void SENSOR0_ISR();
void SENSOR1_ISR();


/**
 * @brief
 *  initializes a sensor with an interrupt. There are 2 interrupts available
 * for digital state changes. 
 * @param
 *  pin: Arduino digital pin
 *  sensor_timer: timer for sensor that times how long it takes for state 
 * change
 *  type: sensor 0 or sensor 1 (supports up to 2 sensors)
*/
void setup_sensor(uint8_t pin, long *sensor_timer, uint8_t type) {
  *sensor_timer = micros();
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  pinMode(pin, INPUT);
  if(type==0){
    attachInterrupt(digitalPinToInterrupt(pin), SENSOR0_ISR,LOW);
  }else if (type==1){
    attachInterrupt(digitalPinToInterrupt(pin), SENSOR1_ISR,LOW);
  }
}

void SENSOR1_ISR() {
  SENSOR1_READING = micros() - SENSOR1_TIMER;
  Serial.print("sensor1 reading: ");Serial.println(SENSOR1_READING);
  setup_sensor(SENSOR1_PIN,&SENSOR1_TIMER,1);
}

/**
 * @brief 
 *  DIGITAL reading of QRE1113 line sensor
 * Polls the digital pin for line sensor value. The line sensor has a 
 * capacitor that gets charged at a rate based on the light reflected
 * from the surface it faces. The typical charge time is around 10uS to
 * 2.5ms. Thus we poll for a max of that long.
 * @param
 *  pin: Arduino digital pin
 * @return 
 *  time to charge cap (raw value of reflected surface)
 */
int pollLineSensor(uint8_t pin){
  pinMode( pin, OUTPUT );
  digitalWrite( pin, HIGH );
  delayMicroseconds(10);
  pinMode( pin, INPUT );

  long time = micros();
  while(digitalRead(pin )==HIGH && micros()-time < 3000);
  
  int diff = micros() - time; 
  return diff; 
}



/**
 * @brief
 *  Interrupt handler for sensor 0. Triggered as a result of digital signal
 * changing states from high to low.
*/
void SENSOR0_ISR() {
  // The sensor light reading is inversely proportional to the time taken
  // for the pin to fall from high to low. Lower values mean lighter colors.
  SENSOR0_READING = micros() - SENSOR0_TIMER;
  // Reset the sensor for another reading
  Serial.print("sensor0 reading: ");Serial.println(SENSOR0_READING);
  setup_sensor(SENSOR0_PIN,&SENSOR0_TIMER,0);
}

