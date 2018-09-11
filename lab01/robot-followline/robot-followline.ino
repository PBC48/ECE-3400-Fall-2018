#include <Servo.h>

#define SENSOR0_PIN 2
#define SENSOR1_PIN 3


/**
Pin definitions 
*/
int SENSOR3_PIN = 4;

char servo_right_pin = 9;
char servo_left_pin = 10;


// These variables are marked 'volatile' to inform the compiler that they can change
// at any time (as they are set by hardware interrupts).
volatile long SENSOR0_TIMER;
volatile long SENSOR1_TIMER;

// Consider smoothing this value with your favorite smoothing technique (exponential moving average?)
volatile int SENSOR0_READING;
volatile int SENSOR1_READING;

Servo servo_right;
Servo servo_left;

char ROBOT_SPEED=180;

int i=0;
int map1[] = {1,1,0,0,0,0,1,1};


void robot_init(){
  pinMode(servo_right_pin,OUTPUT);
  pinMode(servo_left_pin,OUTPUT);
  servo_right.attach(servo_right_pin);
  servo_left.attach(servo_left_pin);
}

void robot_forward(){
  servo_right.write(ROBOT_SPEED);
  servo_left.write(abs(ROBOT_SPEED-180));
}

void robot_turn(int dir)
{
  if(dir){
    servo_right.write(0);
    servo_left.write(40);
  }else{
    servo_right.write(140);
    servo_left.write(180);
  }
  
}


void SENSOR0_ISR() {
  // The sensor light reading is inversely proportional to the time taken
  // for the pin to fall from high to low. Lower values mean lighter colors.
  SENSOR0_READING = micros() - SENSOR0_TIMER;
  // Reset the sensor for another reading
  Serial.print("sensor0 reading: ");Serial.println(SENSOR0_READING);
  setup_sensor(SENSOR0_PIN,&SENSOR0_TIMER,0);
}

void SENSOR1_ISR() {
  SENSOR1_READING = micros() - SENSOR1_TIMER;
  Serial.print("sensor1 reading: ");Serial.println(SENSOR1_READING);
  setup_sensor(SENSOR1_PIN,&SENSOR1_TIMER,1);
}


int readLineSensor(){
  //Returns value from the QRE1113
  //Lower numbers mean more refleacive
  //More than 3000 means nothing was reflected.
  pinMode( SENSOR3_PIN, OUTPUT );
  digitalWrite( SENSOR3_PIN, HIGH );
  delayMicroseconds(10);
  pinMode( SENSOR3_PIN, INPUT );

  long time = micros();
  //time how long the input is HIGH, but quit after 3ms as nothing happens after that
  while(digitalRead(SENSOR3_PIN)==HIGH && micros()-time < 3000);
  
  int diff = micros() - time; 
  return diff; 
}


// A digital write is required to trigger a sensor reading.
void setup_sensor(int pin, long *sensor_timer, int type) {
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


void setup() {
  Serial.begin(9600);
  pinMode(LED_BUILTIN,OUTPUT);
  delay(2000);
  digitalWrite(LED_BUILTIN,LOW);
  // put your setup code here, to run once:
  robot_init();
  setup_sensor(SENSOR0_PIN,&SENSOR0_TIMER,0);
  setup_sensor(SENSOR1_PIN,&SENSOR1_TIMER,1);
}


void loop() {
  // put your main code here, to run repeatedly:
  int reading = readLineSensor();
  
  if(SENSOR0_READING>100){ //farthest left pin
    robot_turn(1);
  }else if(SENSOR1_READING>100){
    robot_turn(0);
  }else{
    robot_forward();
  }
  if(reading<85){
    robot_turn(map1[i++]);
    if(i>=8){
      i=0;
    }
    delay(500);
  }
  
  
}


