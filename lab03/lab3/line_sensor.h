
#define SENSOR_R_PIN 2 //right sensor
#define SENSOR_L_PIN 3 //left sensor

#define maxC 16
// These variables are marked 'volatile' to inform the compiler that they can change
// at any time (as they are set by hardware interrupts).
volatile uint32_t SENSOR_R_TIMER;
volatile uint32_t SENSOR_L_TIMER;

// Consider smoothing this value with your favorite smoothing technique (exponential moving average?)
volatile uint32_t SENSOR_R_READING;
volatile uint32_t SENSOR_L_READING;
volatile bool VALID_R=false;
volatile bool VALID_L=false;
volatile uint8_t COUNT_R = 0, COUNT_L = 0;
volatile uint32_t SUM_R=0,SUM_L=0,AVERAGE_L=0,AVERAGE_R=0;

void SENSOR_R_ISR();
void SENSOR_L_ISR();
void setup_sensor(int pin, long *sensor_timer);
void line_sensor_init();

/**
 * @brief
 *  Interrupt handler for sensor 0. Triggered as a result of digital signal
 * changing states from high to low.
*/
void SENSOR_R_ISR()
{
    // The sensor light reading is inversely proportional to the time taken
    // for the pin to fall from high to low. Lower values mean lighter colors.
    SENSOR_R_READING = micros() - SENSOR_R_TIMER;
    // Reset the sensor for another reading
    setup_sensor(SENSOR_R_PIN, &SENSOR_R_TIMER);
    COUNT_R++;
    SUM_R += SENSOR_R_READING;
    if (COUNT_R >= maxC)
    {
        VALID_R = true;
        AVERAGE_R = SUM_R >> (COUNT_R/4);
        AVERAGE_R = 0;
        COUNT_R = 0;
    }
}

void SENSOR_L_ISR()
{
    SENSOR_L_READING = micros() - SENSOR_L_TIMER;
    setup_sensor(SENSOR_L_PIN, &SENSOR_L_TIMER);
    COUNT_L++;
    SUM_L += SENSOR_L_READING;
    if (COUNT_L >= maxC)
    {
        VALID_L = true;
        AVERAGE_L = SUM_L >> (COUNT_L/4);
        SUM_L = 0;
        COUNT_L = 0;
    }
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
void setup_sensor(int pin, long *sensor_timer)
{
    *sensor_timer = micros();
    pinMode(pin, OUTPUT);
    digitalWrite(pin, HIGH);
    pinMode(pin, INPUT);
}

/**
 * Initializes the two line sensors
*/
void line_sensor_init()
{
    // Tell the compiler which pin to associate with which ISR
    attachInterrupt(digitalPinToInterrupt(SENSOR_R_PIN), SENSOR_R_ISR, LOW);
    attachInterrupt(digitalPinToInterrupt(SENSOR_L_PIN), SENSOR_L_ISR, LOW);
    VALID_L = false;
    VALID_R = false;
    // Setup the sensors
    setup_sensor(SENSOR_R_PIN, &SENSOR_R_TIMER);
    setup_sensor(SENSOR_L_PIN, &SENSOR_L_TIMER);
}
