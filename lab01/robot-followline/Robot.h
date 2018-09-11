#include <Servo.h>
// need to fill this out


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
	neither = -1
};

class Robot
{
        
    Servo wheel_R;
    Servo wheel_L;
    int turn_time = 300;
	int turn_direction;

    
    uint8_t ROBOT_SPEED=180;

	uint8_t PIN_WHEEL_R = 9;
	uint8_t PIN_WHEEL_L = 10;
	

public:
	uint8_t curr_state;
  int spins;
	Robot();
    Robot(uint8_t w_pin_R, uint8_t w_pin_L);
	void forward();
	void stop_m();
	void turn(DIRECTIONS dir);
	void turn(DIRECTIONS dir, int r_time, int r_speed);
	// void set_turn_time(int t){turn_time = t;}
	// void set_right_speed(int s){right_speed = s;}
	// void set_left_speed(int s){left_speed = s;} 
	// void set_turn_direction(DIRECTIONS dir){turn_direction = dir;}
	
	// DIRECTIONS get_turn_direction(){return turn_direction;}
	// int get_turn_time(){return turn_time;}
}; 

Robot::Robot(){
	pinMode( PIN_WHEEL_R, OUTPUT );
	pinMode( PIN_WHEEL_L, OUTPUT );
    wheel_R.attach(PIN_WHEEL_R);
    wheel_L.attach(PIN_WHEEL_L);
	//init status
}

Robot::Robot(uint8_t w_pin_R, uint8_t w_pin_L):PIN_WHEEL_L(w_pin_L),
PIN_WHEEL_R(w_pin_R){
    Robot();
}


void Robot::forward(){
  wheel_R.write(ROBOT_SPEED);
  wheel_L.write(abs(ROBOT_SPEED-180));
}

void Robot::turn(DIRECTIONS dir)
{
  if(dir==right){
    wheel_R.write(0);
    wheel_L.write(40);
  }else if(dir==left){
    wheel_R.write(140);
    wheel_L.write(180);
  }  
}