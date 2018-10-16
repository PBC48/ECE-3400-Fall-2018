#include <Servo.h>

#define ROBOT_RIGHT_PIN 9
#define ROBOT_LEFT_PIN 10

Servo R;
Servo L;


enum DIRECTIONS {
    right,
    left,
    forward,
    stop,
    adj_right,
    adj_left,
    back
};

void robot_move(DIRECTIONS dir){
    if(dir != stop && (!R.attached() || !L.attached())){
        Serial.println("in if attach");
        R.attach(ROBOT_RIGHT_PIN);
        L.attach(ROBOT_LEFT_PIN);
    }else if(dir == stop){
        R.detach();
        L.detach();
    }
    switch (dir){ //NEED SOME CALIBRATION
        case right:
            L.write(180);
            R.write(90);
            Serial.println("right");
            break;

        case left:
            L.write(90);
            R.write(0);
            Serial.println("left");
            break;
        case forward:
            L.write(180);
            R.write(0);
            Serial.println("forward");
            break;
        case stop:
            L.write(90);
            R.write(90);
            Serial.println("stopped");
            break;
        case adj_left:
            L.write(95);
            R.write(0);
            Serial.println("adjust left");
            break;
        case adj_right:
            L.write(180);
            R.write(85);
            Serial.println("adjust right");
            break;
        case back:
            L.write(0);
            R.write(180);
            Serial.println("back");
            break;
    }
}


void robot_init(){
  robot_move(stop);
}


void robot_calibrate(){
    robot_move(right);
    delay(10000);
    robot_move(left);
    delay(10000);
    robot_move(forward);
    delay(10000);
    robot_move(stop);
    delay(10000);
    robot_move(back);
    delay(10000);
}
