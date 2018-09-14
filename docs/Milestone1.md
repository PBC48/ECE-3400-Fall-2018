# Milestone 1: Line Follower

## Introduction

## Hardware Upgrades

### Chassis upgrade
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213436.jpg" width="800"/>
    <font size="2">
    <figcaption> Front view of Robot version 1
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213510.jpg" width="800"/>
    <font size="2">
    <figcaption> Top view of updated Chassis
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180913_204701.jpg" width="800"/>
    <font size="2">
    <figcaption> Front view of Robot version 2
    </figcaption>
    </font>
</figure>

### Circuit
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-RobotLineSensing.PNG" width="800"/>
    <font size="2">
    <figcaption> Schematic of Updated Robot
    </figcaption>
    </font>
</figure>


## Software Design

### Follow the Line

```cpp
if(SENSOR1_READING < 400){ //turning right
    turn_right();
    Serial.println("right");
  }else if(SENSOR0_READING < 400) { //turning left
    turn _left();
    Serial.println("left");
  }else{
    forward();
    Serial.println("forward");
  }
```

<a><img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/gif-followLine.gif" width = "800" height = "auto"></a>

### Figure 8
<iframe width="800" height="450" src="https://www.youtube.com/embed/mZf0CTAzZvA" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

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