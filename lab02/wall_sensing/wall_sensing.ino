/*
 * How to use the QRE1113 Digital Line Sensor by SparkFun with hardware interrupts
 * https://www.sparkfun.com/products/9454
 *
 * Note: The Arduino Uno is limited to two pins for digital state interrupts: pins 2 and 3.
 * This means you will be limited to two digital line sensors.
 *
 * Refer to this for information on attachInterrupt():
 * https://www.arduino.cc/reference/en/language/functions/external-interrupts/attachinterrupt/
  */
#include <Servo.h>

#define SENSOR0_PIN 2
#define SENSOR1_PIN 3
#define WALL_FRONT A1
#define WALL_LEFT A2
#define RED_LED 5
#define YELLOW_LED 6

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

Servo R;
Servo L;

char map1[] = {0,1,1,1,1,0,0,0};

int i=0; 

void setup() {
  Serial.begin(9600);

  // Tell the compiler which pin to associate with which ISR
  attachInterrupt(digitalPinToInterrupt(SENSOR0_PIN), SENSOR0_ISR, LOW);
  attachInterrupt(digitalPinToInterrupt(SENSOR1_PIN), SENSOR1_ISR, LOW);

  // Setup the sensors
  setup_sensor(SENSOR0_PIN, &SENSOR0_TIMER);
  setup_sensor(SENSOR1_PIN, &SENSOR1_TIMER);

  R.attach(9);
  L.attach(10);
  delay(1000);
  L.write(180);
  R.write(0);

  pinMode(RED_LED,OUTPUT);
  pinMode(YELLOW_LED,OUTPUT);
  digitalWrite(RED_LED,LOW);
  digitalWrite(YELLOW_LED,LOW);
 
}

void turn_left(){
  L.write(90);
  R.write(40);
  Serial.println("left");
}

void turn_right(){
  L.write(140);
  R.write(91);
  Serial.println("right");
}

void forward() {
  L.write(180);
  R.write(0);
  Serial.println("forward");
}


void loop() {
  // These delays are purely for ease of reading.
  //Serial.print("Sensor 0: ");Serial.println(SENSOR0_READING);
  //Serial.print("Sensor Analog: "); Serial.println(analogRead(A0));
  //delay(500);
  //Serial.print("Sensor 1: ");Serial.println(SENSOR1_READING);
  int FRONTWALL = analogRead(WALL_FRONT);
  int LEFTWALL = analogRead(WALL_LEFT);
  Serial.print("Wall Front: ");Serial.println(FRONTWALL);
  Serial.print("Wall Left: ");Serial.println(LEFTWALL);
  if(SENSOR0_READING<400 && SENSOR1_READING<400){
    if(LEFTWALL < 200){
       turn_left();
       digitalWrite(RED_LED,HIGH);
       delay(1200);
       digitalWrite(RED_LED,LOW);
    } else if (FRONTWALL > 170) {
      turn_right();
      digitalWrite(YELLOW_LED,HIGH);
      delay(1200);
      digitalWrite(YELLOW_LED,LOW);
    } else {
      forward();
    }
  }else if(SENSOR1_READING < 400){ //turning right
    turn_right();
  }else if(SENSOR0_READING < 400) { //turning left
    turn_left();
  }else{
    forward();
  }
  

}
