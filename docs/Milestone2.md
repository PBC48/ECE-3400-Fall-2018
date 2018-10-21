# Milestone 2: Wall and Robot Detection

## Introduction
The goal of milestone 2 was to implement wall-detection functionality and make the robot avoid other robots, while still successfully line-following in order to traverse the maze. To accomplish this task, we added short-range IR sensors to the robot to detect walls and refined our detection of the 6.08kHz IR signal emitted by the robot IR hat. Our final product was a demonstration of the robot traversing an arbitrary maze using a left-wall-following rule, and stopping whenever it detected the IR hat.

## Line-Following Improvements
Line-following functionality was previously implemented in milestone 1, but when we came back to this after working on Lab 2 we discovered that our implementation required some adjustments. Due to the added weight of new components, the power necessary to turn the robot 90 degrees increased; this threw off our initial code, so we re-calibrated the system. We also found that we needed to adjust the height of our line sensors. They were too low to the ground, which led to sensing issues at intersections and caused the robot to occasionally stop inexplicably instead of turning. We added nuts to act as a spacer onto the screw that attaches the sensors to the robot, raising the sensors higher, and re-calibrated our white-black threshold.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone2/line_sensor_spacers2.jpg" width="450"/>
    <font size="3">
    <figcaption> <b>Raised Line Sensors</b>
    </figcaption>
    </font>
</figure>



## Wall Dectection
To avoid walls while navigating the maze, we attached short-range IR sensors to the front and left side of the chassis. These sensors detect objects at distances between 4 cm and 30cm, which is appropriate for our use.


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone2/IMG_0373.jpg" width="600"/>
    <font size="3">
    <figcaption> <b>Updated Robot with Wall Scensors</b>
    </figcaption>
    </font>
</figure>

The wall sensors output an analog value corresponding to the distance of an object from the sensor, and our algorithm sets threshold values to determine whether there is a wall in the grid space directly adjacent to the robot.  We implemented a left-hand wall following rule, defaulting to a left turn at an intersection if no wall is detected to the left. If the robot only detects a wall to the left, it goes straight, and if there is a wall both to the left and in front of the robot, it turns right. The following video shows our robot traversing a small maze:

<iframe width="560" height="315" src="https://www.youtube.com/embed/31nsK4JZrqU?rel=0&amp;controls=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

The core of our algorithm is a finite state machine. It has states that make the robot either turn left, turn right, or move forward, adjust the machine to stay on the line, and sense for other robots.


## Robot Avoidance

The detecting of other robots was already mostly implemented in Lab 2 by the optical team, but we integrated the robot’s response to the IR hat’s signal this week.  
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone2/phototransistor2.jpg" width="450"/>
    <font size="3">
    <figcaption> <b>View of Phototransistor</b>
    </figcaption>
    </font>
</figure>

To increase the sensitivity of the phototransistor, we put the sensor at the head of the robot.


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone2/IMG_0374.jpg" width="600"/>
    <font size="3">
    <figcaption> <b>Front View of Robot</b>
    </figcaption>
    </font>
</figure>
The phototransistor can be seen right in front of the battery.


Placing the phototransistor there improved our ability to sense other robots in front, to the right, and to the left of ours. Our main focus is on detecting robots in front of us, while detection of robots to the side is more limited. We chose to have the robot simply stop when it detects another robot. The following video shows our robot stopping when we hold the IR hat in front of it approximately 5.5 inches above the ground:

<iframe width="560" height="315" src="https://www.youtube.com/embed/r3OLbq3HsRQ?rel=0&amp;controls=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Code Updates

We updated the FSM used in our lab 2 integration with addtional states: 
```cpp
enum states : uint8_t {
    START,
    AUDIO_DECT,
    IR_DECT,
    ROBOT_SENSE,
    ROBOT_DETECTED,
    ROBOT_TURN_LEFT,
    ROBOT_TURN_RIGHT
};

```
We concentrated the use of the FFT for processing of the audio and IR signals into two states. We also have states specifically for the robot turning at intersections because we hope to eventually implement a more robust robot turning algorithm. We have somewhat of a skeleton code for ROBOT_DETECTED, ROBOT_TURN_LEFT, and ROBOT_TURN_RIGHT because we intend to implement them with more complexity than simple delays. The ROBOT_SENSE state is the main state that the robot will be in, where it is following the line and detecting the intersections. ROBOT_DETECTED handles cases where we detect other robots with the IR detection state. The current FSM looks like this:

```cpp
#define WALL_FRONT 2
#define WALL_LEFT 3
#define WAITTIME 800
switch (STATE){
    case START:
        STATE = AUDIO_DECT;
        break;
    
    case AUDIO_DECT:
        calculate_FFT(MIC);
        if(pass_thresh_hold) 
            STATE = IR_DECT;
        else
            STATE = AUDIO_DECT;
        break;
    
    case IR_DECT:
        calculate_FFT(IR);
        if(pass_thresh_hold)
            STATE = ROBOT_DETECTED;
        else
            STATE = ROBOT_SENSE;
        u32wait_ir = millis();
        break;

    case ROBOT_SENSE:
        FRONTWALL = analogRead(WALL_FRONT);
        LEFTWALL = analogRead(WALL_LEFT);
        if(SENSOR_R_READING<200 && SENSOR_L_READING<200){ 
            if(LEFTWALL < 200){
                u32wait = millis();
                STATE = ROBOT_TURN_LEFT;
            } else if (FRONTWALL > 115) {
                u32wait = millis();
                STATE = ROBOT_TURN_RIGHT;
            } else 
                robot_move(forward);
        }else if(SENSOR_L_READING < 200)
            robot_move(adj_right);
        else if(SENSOR_R_READING < 200)
            robot_move(adj_left);
        else
            robot_move(forward);
        if((millis()-u32wait_ir) > WAITTIME){
            u32wait_ir = millis();
            STATE = IR_DECT;
        }
        break;
            
    case ROBOT_DETECTED:
        robot_move(rstop); //stop till we don't detect IR
        STATE = IR_DECT;
        break;

    case ROBOT_TURN_LEFT:
        robot_move(left);
        if(millis()-u32wait>700) //same as a delay
            STATE = ROBOT_SENSE;
        break;

    case ROBOT_TURN_RIGHT:
        robot_move(right);
        if(millis()-u32wait>700) //same as a delay
            STATE = ROBOT_SENSE;
        break
    }
```

In ROBOT_SENSE, we read from the wall sensors and the line sensors to tell if we need to either turn or adjust the robot to stay on the line. Just like in milestone 1, we only consider turning if both sensors detect a white line; this time, however, we turn based on the readings of the wall sensors rather than a pre-determined pattern.

The IR sensing occurs every 800 ms, which can be adjusted later based on computation needs. Every 800 ms, we switch the state to IR_DECT and run the FFT with a processing algorithm. Currently, we only do averages of N FFT cycles but may consider moving averages.   

## Demonstration
We show what our robot is “thinking” by flashing LEDs indicating what outside stimuli the robot has picked up and will be reacting to according to the FSM.  As it navigates the maze, it stays with the wall on its left.  The red LED indicates that the left path is clear, so the robot will turn left.  If the yellow LED has turned on, this indicates that both the front and left wall sensors detect walls, so the robot will respond by turning right. The green LED turns on when the IR sensor has detected a signal from the IR hat (i.e., detected another robot), and the robot will halt to avoid the other robot.

<iframe width="560" height="315" src="https://www.youtube.com/embed/aWahZUqSUfI?rel=0&amp;controls=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Conclusion
We have progressed further toward our robot being able to successfully navigate the final competition maze. As we continue to optomize our design, we intend to add a third wall sensor to the right side of the robot to allow it to detect walls on all side for the purpose of mapping the maze. We also need to tune the IR sensor to better  detect robots coming from the sides. This will likely involve re-adusting the battery's positioning since its USB blocks some of the signals to the IR. The FSM design for our robot control is a robust and modular system that allows the robot to multitask by checking states per cycle and allow us to easily add more tasks if needed. The robot can switch its current task every clock cycle of the Arudino. In the future, we might build a complex scheduler that decides what state the robot is in based on current conditions which can adapt to the various different activities the robot will be performing in the competition. In terms of implementation, we would like to write a more robust turning algorithm that will not rely on hard-coded delays, which would allow the robot to  adapt to different conditions as it turns. Our current method assumes that the robot will always be in the same position when it starts turning, but that may not be the case in the long term.   
