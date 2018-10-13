
#define SENSOR0_PIN 2 //right sensor
#define SENSOR1_PIN 3 //left sensor


// These variables are marked 'volatile' to inform the compiler that they can change
// at any time (as they are set by hardware interrupts).
volatile uint32_t SENSOR0_TIMER;
volatile uint32_t SENSOR1_TIMER;

// Consider smoothing this value with your favorite smoothing technique (exponential moving average?)
volatile uint32_t SENSOR0_READING;
volatile uint32_t SENSOR1_READING;


void SENSOR0_ISR();
void SENSOR1_ISR();
void setup_sensor(int pin, long *sensor_timer);
void line_sensor_init();

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
  setup_sensor(SENSOR0_PIN,&SENSOR0_TIMER);
}

void SENSOR1_ISR() {
  SENSOR1_READING = micros() - SENSOR1_TIMER;
  setup_sensor(SENSOR1_PIN,&SENSOR1_TIMER);
}


/**
 * @brief
 *  initializes a sensor with an interrupt. There are 2 interrupts available
 * for digital state changes. 
 * @param
 *  pin: Arduino digital pin
 *  sensor_timer: timer for sensor that times how long it takes for state 
 * change
*/
// A digital write is required to trigger a sensor reading.
void setup_sensor(int pin, long *sensor_timer) {
  *sensor_timer = micros();
  pinMode(pin, OUTPUT);
  digitalWrite(pin, HIGH);
  pinMode(pin, INPUT);
}

/**
 * Initializes the two line sensors
*/
void line_sensor_init(){
    // Tell the compiler which pin to associate with which ISR
  attachInterrupt(digitalPinToInterrupt(SENSOR0_PIN), SENSOR0_ISR, LOW);
  attachInterrupt(digitalPinToInterrupt(SENSOR1_PIN), SENSOR1_ISR, LOW);

  // Setup the sensors
  setup_sensor(SENSOR0_PIN, &SENSOR0_TIMER);
  setup_sensor(SENSOR1_PIN, &SENSOR1_TIMER);

}
