# Milestone 1: Line Follower

## Introduction

The goal of this milestone was to add functionality enabling our robot to both follow a straight line and run in a figure eight pattern.  To accomplish this task, we added line sensors to the robot and implemented code to follow the lines and turn as directed at intersections.

## Hardware Upgrades

### Chassis upgrade

We added a second level to the robot’s chassis to support our battery, the arduino, and a breadboard for our circuits. The breadboard allowed us to connect more components requiring more ground and power sources than are available directly on the Arduino.  With a common ground and source on the breadboard, we were able to connect the line sensors to the Arduino.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213436.jpg" width="800"/>
    <font size="3">
    <figcaption> Front View of Robot Version 1
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


![alt text](https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204623.jpg | width=400x "")

![alt text](https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204628.jpg | width=400x "")



### Line Sensor Update

We used QRE1113 line sensors, which work by transmitting and then detecting IR light that reflects back to the phototransistor on the sensor. We used digital pins to detect how long it takes to charge the phototransistor -- the return values indicate a light or dark surface. These sensors are mounted at the front of the robot facing downward, with the sensors only a few centimeters above the ground to maximize sensor accuracy. 

Our first design idea used three line sensors -- two for staying on the line and a third for detecting intersections.  In our final implementation, we were able to use software to perform all the required tasks using only two sensors.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204605.jpg" width="800"/>
    <font size="3">
    <figcaption> Front view of Robot Version 2
    </figcaption>
    </font>
</figure>

### Circuit
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-RobotLineSensing.PNG" width="800"/>
    <font size="3">
    <figcaption> Schematic of Updated Robot with 2 Line Sensors
    </figcaption>
    </font>
</figure>


## Software Design

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
    SENSOR0_READING = micros() - SENSOR0_TIMER;
    SENSOR0_TIMER = micros();
    pinMode(SENSOR0_PIN, OUTPUT);
    digitalWrite(SENSOR0_PIN, HIGH);
    pinMode(SENSOR0_PIN, INPUT);  
}


void setup(){
    attachInterrupt(digitalPinToInterrupt(SENSOR0_PIN), SENSOR0_ISR, LOW);
}


```



### Obstacles

We tried to use the analogRead() function to read input values from the sensors, but the numbers we read were illogical and unuseable. Instead, we used a combination of digital interrupts and polling to gather data from the line sensors. The Arduino supports two digital state change interrupts which trigger an interrupt whenever the state of a digital pin changes. We used the interrupts for sensors with time sensitive operations such as following the line. We were going to use polling for our third sensor to obtain a digital reading to detect intersections, but we discovered we could accomplish all our functionality with just two. As our implementation progressed, we enabled the robot to first follow a straight line, then a curved line, and finally drive in a figure 8. 

### Follow the Line

To follow the line, we used simple if statements to adjust the robot’s path. The two line sensors detected when the robot moved too far off-center of the white line, and we used a conditional to move the robot left or right when the line sensors’ values pass a certain threshold.

#### Code snippet for following the line
```cpp
if(SENSOR1_READING < 400){ //turning right
    turn_right();git pulu
}else if(SENSOR0_READING < 400) { //turning left
    turn _left();
}else{
    forward();
}
```

<a><img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/gif-followLine.gif" width = "800" height = "auto"></a>

### Figure-8

To implement a figure-8 path, we built off the line-following code and added a conditional checking when both sensors hit a white line -- an intersection.  Using an array, we created a map of where the robot should turn at an intersection to create a figure-8, with each element of the array indicating the direction to turn. The array is repeatable such that the robot will continuously move in a figure-8 formation.

#### Video of robot Figure-8
<iframe width="800" height="450" src="https://www.youtube.com/embed/mZf0CTAzZvA" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

#### Code snippet for figure-8 logic
```cpp
char map1[] = {right,left,left,left,left,right,right,right};
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
