#include <Servo.h>
// need to fill this out

#define ROBOT_RIGHT_PIN 9
#define ROBOT_LEFT_PIN 10

Servo R;
Servo L;


enum ROBOT_STATES {
    ROBOT_START = 0,
//    ROBOT_HIT = 1,
//    ROBOT_FOUND = 2,
    ROBOT_FORWARD = 3,
    ROBOT_RIGHT = 4,
    ROBOT_LEFT = 5,
//	ROBOT_REVERSE = 6,
//	ROBOT_TURN = 7,
    ROBOT_STOP = 8,
};


enum DIRECTIONS {
	right = 0,
	left = 1,
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
    switch (dir){
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
            break;

        case adj_right:
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

