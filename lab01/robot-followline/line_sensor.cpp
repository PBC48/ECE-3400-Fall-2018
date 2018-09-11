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

 #include "line_sensor.h"

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

void SENSOR0_ISR() {
  // The sensor light reading is inversely proportional to the time taken
  // for the pin to fall from high to low. Lower values mean lighter colors.
  SENSOR0_READING = micros() - SENSOR0_TIMER;
  // Reset the sensor for another reading
  Serial.print("sensor2 reading: ");Serial.println(SENSOR2_READING);
  setup_sensor(LINE_PIN1,&SENSOR0_TIMER,0);
}

void SENSOR1_ISR() {
  SENSOR1_READING = micros() - SENSOR1_TIMER;
  Serial.print("sensor1 reading: ");Serial.println(SENSOR1_READING);
  setup_sensor(LINE_PIN2,&SENSOR1_TIMER,1);
}
