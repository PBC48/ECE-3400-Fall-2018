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
#include "robot.h"
#include "line_sensor.h"
#include "fft_lib.h"

enum states {
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_MOVE,
    ROBOT_SENSE,
    ROBOT_DECTECTED,
};
uint8_t state;

void setup() {
  Serial.begin(115200);
    line_sensor_init();
    robot_init();
    state = START;
 
}

void loop() {
    switch (state){
        case START:
            Serial.println("start");
            state = AUDIO_DECT;
            break;
        
        case AUDIO_DECT:
            microphone_set();
            calculate_FFT();
            if(sum>90){
                turn_led(1);
                Serial.println("660Hz Tone Detected");
               
                turn_led(0);
                state = IR_DECT;
            }else{
                turn_led(0);
                state = AUDIO_DECT;
            }
            sum = 0;
            break;
        
        case IR_DECT:
            ir_set();
            calculate_FFT();
            if(sum > 55){
                turn_led(1);
                Serial.println("Robot Detected");
                
                turn_led(0);
                state = ROBOT_DECTECTED;
            }else{
                turn_led(0);
                state = ROBOT_SENSE;
            }
            sum = 0;
            break;

        case ROBOT_MOVE
    }
}
 /* // These delays are purely for ease of reading.
  Serial.print("Sensor 0: ");Serial.println(SENSOR0_READING);
  //Serial.print("Sensor Analog: "); Serial.println(analogRead(A0));
  //delay(500);
  Serial.print("Sensor 1: ");Serial.println(SENSOR1_READING);
  if(SENSOR0_READING<400 && SENSOR1_READING<400){
    map1[i%8] ? turn_right() : turn_left();
    delay(1200);
    i++;
    Serial.println("straight");
  }else if(SENSOR1_READING < 400){ //turning right
    L.write(140);
    R.write(90);
    Serial.println("right");
  }else if(SENSOR0_READING < 400) { //turning left
    L.write(90);
    R.write(40);
    Serial.println("left");
  }else{
    L.write(180);
    R.write(0);
    Serial.println("forward");
  }
 */ 

}

