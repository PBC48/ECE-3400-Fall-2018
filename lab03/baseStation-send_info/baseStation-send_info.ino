//int map1[2][3];
int map1[2][3] = {0};

/**
 *  |0,0,0|
 *  |0,0,0|
*/
//
// Topology
//
int rows = 2; //need to adjust based on robot position
int cols = 3;
// Starting coordinates
int x = 0;
int y = 0;
enum r_dir
{
    up,
    right,
    left,
    down
};

r_dir robot_direction;
// The debug-friendly na
uint16_t buff;
int i=0;

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    ; // wait for serial port to connect. Needed for native USB port only
  }
  i = 0;
  buff = 0;
  robot_direction = right;
}


void loop() {

//   // Run through a 2x3 maze
//   Serial.println("reset");
//   delay(1000);
//   Serial.println("0,0,north=true,west=true");
//   delay(1000);
//   Serial.println("0,1,north=true");
//   delay(1000);
//   Serial.println("0,2,north=true,east=true");
//   delay(1000);
//   Serial.println("1,2,south=true,east=true");
//   delay(1000);
//   Serial.println("1,1,south=true");
//   delay(1000);
//   Serial.println("1,0,south=true,west=true");
//   delay(1000);
//000000vb ddtttrfl
//dir = 0:for, 1:right, 2:left
delay(1000);
    if(i==0){
        buff = 579;
    }else if(i == 1){
        buff = 646;
    }else if(i==2){
        buff = 642;
    }else if(i==3){
        buff = 579;
    }else if( i==4){
        buff = 579;
    }else if( i==5){
        buff = 517;
    }else if(i>5){
        while(1){};
    }
  i++;
  //Serial.print("buffer: ");Serial.println(buff);
          bool wall_left, wall_right, wall_front, robot, west, east, north, south;
        uint8_t dir, treasure;

        wall_left = buff & 0x0001;
        wall_front = (buff >> 1) & 0x0001;
        wall_right = (buff >> 2) & 0x0001;
        treasure = (buff >> 3) & 0x0007;
        dir = (buff >> 6) & 0x0003;
        robot = (buff >> 8) & 0x0001;

/*        
    Serial.print("direction=");Serial.print(dir);
    Serial.print(",wall_left=");Serial.print(wall_left);
    Serial.print(",wall_front=");Serial.print(wall_front);
    Serial.print(",wall_right=");Serial.print(wall_right);
    Serial.print(",robot_direction=");Serial.print(robot_direction);
    
        /*Serial.print(",treasure=");Serial.print(treasure);
    Serial.print(",tshape=");Serial.print(tshape);
    Serial.print(",tcolor=");Serial.print(tcolor);
    Serial.print(",robot=");Serial.println(robot);*/
        //Serial.println("");
        //decode the absolute directions base on the robot's direction and input
        switch (robot_direction)
        {
        case right: //facing right, then left is north, right is south
            north = wall_left;
            south = wall_right;
            west = false; //No wall behind robot
            east = wall_front;
            break;
        case left:
            north = wall_right;
            south = wall_left;
            west = wall_front;
            east = false;
            break;
        case up:
            north = wall_front;
            south = false;
            west = wall_left;
            east = wall_right;
            break;
        case down:
            north = false;
            south = wall_front;
            west = wall_right;
            east = wall_left;
            break;
        default:
            Serial.println("robot_direction not recognized");
        }
        Serial.print(y);
        Serial.print(",");
        Serial.print(x);
        if(west)
        Serial.print(",west=true");
        if(east)
        Serial.print(",east=true");
        if(north)
        Serial.print(",north=true");
        if(south)
        Serial.print(",south=true");
        /*if(treasure!=0){
        Serial.print(",tshape=");
        if(treasure>=0 && treasure <3)
            Serial.print("circle");
        else
            Serial.print("square");
        Serial.print(",tcolor=");
        if(tcolor)
            Serial.print("red");
        else
            Serial.print("blue");
    }*/
        Serial.println("");
        switch (dir)
        {
        case 0: //forward : robot decided to go forward
            if (robot_direction == right)
            {
                x++;
            }
            else if (robot_direction == left)
            {
                x--;
            }
            else if (robot_direction == up)
            {
                y--;
            }
            else if (robot_direction == down)
            {
                y++;
            }
            break;
        case 1: //right : robot decided to turn right
            if (robot_direction == right)
            {
                y++;
                robot_direction = down;
            }
            else if (robot_direction == left)
            {
                y--;
                robot_direction = up;
            }
            else if (robot_direction == up)
            {
                x++;
                robot_direction = right;
            }
            else if (robot_direction == down)
            {
                x--;
                robot_direction = left;
            }
            break;
        case 2: //left : robot decides to turn left
            if (robot_direction == right)
            {
                y--;
                robot_direction = up;
            }
            else if (robot_direction == left)
            {
                y++;
                robot_direction = down;
            }
            else if (robot_direction == up)
            {
                x--;
                robot_direction = left;
            }
            else if (robot_direction == down)
            {
                x++;
                robot_direction = right;
            }
            break;

        default:
            Serial.println("ROBOT DIRECTION NOT RECOGNIZED");
        }
}

