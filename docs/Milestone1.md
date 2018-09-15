# Milestone 1: Line Follower

## Introduction

The goal of this milestone was to implement line-following functionality to enable our robot to both follow a straight line and trace a figure-eight pattern. To accomplish this task, we added IR sensors to the robot and implemented code for it to follow the lines and turn as directed at intersections.

## Hardware Upgrades

We spent a large amount of lab time changing the structure of the robot chassis. The goal is to build a good foundation for the robot structure so that we have enough space and organization later on as the robot becomes more complex.

### Chassis upgrade

We added a second level to the robot’s chassis to support our battery, the arduino, and a breadboard for our circuits. 
This second level allows us to mount the battery in a secure place and creates room for additions. The spaces between the two levels can also be used to store wiring for future additional sensors and circuitry. 

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213436.jpg" width="800"/>
    <font size="3">
    <figcaption> Front View of Robot after Adding a Second Level
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213510.jpg" width="800"/>
    <font size="3">
    <figcaption> Top View of Updated Chassis
    </figcaption>
    </font>
</figure>

#### Robot with Breadboard on top 
 <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204623.jpg" width = "340"/><img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204628.jpg" width = "340"/>


Adding the breadboard allowed us to create common power and ground lines and connect more components than there are power and ground pins on the Arduino. We were then able to connect the line sensors to the Arduino. The current breadboard is temporary, as smaller breadboards were unavailable during the lab. We plan to attach a smaller breadboard or potentially solder breadboards in order to securely and reliably attach electrical components to the board. We also plan to attach the breadboard more securely to the chassis, and will organize the wiring better especially as we add more components.

### Line Sensor Update

We used QRE1113 line sensors, which work by transmitting and then detecting IR light that reflects back to a phototransistor on the sensor. These sensors are mounted at the front of the robot facing downward, with the sensors only a few centimeters above the ground to maximize sensor accuracy. 

Our first design idea used three line sensors: two for staying on the line, and a third for detecting intersections.  In our final implementation, we were able to use software to perform all the required tasks using only two sensors. We then positioned the sensors further apart than they were in our initial design so that they wouldn't trigger as often and the robot would make fewer adjustments to stay on the line. This hardware update helped improve the robot's navigation speed and smoothness.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204620.jpg" width="800"/>
    <font size="3">
    <figcaption> Front view of Robot, Final Version
    </figcaption>
    </font>
</figure>

### Circuit
Our final circuit with the sensors looked like this:
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-RobotLineSensing.PNG" width="800"/>
    <font size="3">
    <figcaption> Schematic of Updated Robot with 2 Line Sensors
    </figcaption>
    </font>
</figure>


## Software Design

We used a simple software algorithm for performing these tasks. However, we also began the ground work for abstracting away some of the robot functions such as servo speed for robot forward movement and turning, which will be useful as we develop more complex algorithms to solve difficult problems later on. 


### The Line Sensor
We tried to use the ```analogRead()``` function to read input values from the sensors corresponding to the darkness of the surface, but the numbers we read were illogical and unuseable. Instead, we used digital pins to detect how long it takes to charge the sensor's phototransistor; the return values indicate the darkness of the surface. We used digital interrupts to gather data from the line sensors. The Arduino supports two digital state change interrupts which trigger an interrupt whenever the state of a digital pin changes. We used the interrupts for sensors with time sensitive operations such as following the line.

#### Code snippet for reading from line sensor using digital pins
```cpp
volatile int SENSOR0_READING;
volatile int SENSOR0_TIMER;
int SENSOR0_PIN=2;


/**
 * Triggers when the state of digital pin goes from high to low.
 * Updates SENSOR0_READING variable.
*/
void SENSOR0_ISR(){
    SENSOR0_READING = micros() - SENSOR0_TIMER; //time difference as a result 
    //of digital signal changing states from high to low. 
    SENSOR0_TIMER = micros(); 
    pinMode(SENSOR0_PIN, OUTPUT); //reset the digital pin
    digitalWrite(SENSOR0_PIN, HIGH); //put back to high for next reading when low
    pinMode(SENSOR0_PIN, INPUT);  
}


void setup(){
    attachInterrupt(digitalPinToInterrupt(SENSOR0_PIN), SENSOR0_ISR, LOW);
}

```



### Follow the Line

To follow the line, we used simple ```if``` statements to adjust the robot’s path. The two line sensors detected when the robot moved too far off-center of the white line. We used a conditional to move the robot left or right when the line sensors’ values passed a certain threshold indicating they were over the white tape, and had it move straight forward when both sensors detected white tape at an intersection.

#### Code snippet for following the line
```cpp
if(SENSOR1_READING < 400){ //turning right
    turn_right();
}else if(SENSOR0_READING < 400) { //turning left
    turn _left();
}else{
    forward();
}
```

#### Demonstration of Robot Following the Line
<a><img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/gif-followLine.gif" width = "800" height = "auto"></a>

The robot reliably follows the line. The distance between the line sensors was good enough for the robot to be "following" the line but not constantly repositioning itself; we wanted to maximize the forward moving time while turning only occasionally, when the robot moved off the line.

Currently, the robot turns very sharply to reposition itself because we stop one wheel and move the other to create a turning motion. Moving forward, we want to smooth the robot's readjustment so that it will not lose too much speed when readjusting.

### Figure-8

To implement a figure-8 path, we built off the line-following code and added a conditional checking when both sensors hit a white line at an intersection.  Using an array, we created a map of the sequence of turns the robot should take at each intersection to create a figure-8, with each element of the array indicating the direction to turn. The array is repeatable such that the robot will continuously move in a figure-8 formation.

#### Code snippet for figure-8 logic
```cpp
char map1[] = {right,left,left,left,left,right,right,right}; //dependent on 
//where robot starts
int i=0; 
void loop() {
  if(SENSOR0_READING<400 && SENSOR1_READING<400){
    map1[i%8]==right ? turn_right() : turn_left();
    delay(1200);
    i++;
  }else if(SENSOR1_READING < 400){ //turning right
    turn_right();
  }else if(SENSOR0_READING < 400) { //turning left
    turn _left();
  }else{
    forward();
  }
}
```

#### Video of Robot Performing Figure-8
<iframe width="800" height="450" src="https://www.youtube.com/embed/mZf0CTAzZvA" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

Currently the robot doesn't turn as well as we would like; it doesn't turn for long enough and relies on the line sensors to reposition itself on the line after the turn. There is more room to optimize the robot's turn configuration either by optimizing the turn or the robot's shifting. 






 <!--<img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204644.jpg" width = "340"/><img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204557.jpg" width = "340"/>--> 
