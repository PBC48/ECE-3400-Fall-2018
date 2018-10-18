# Milestone 2: Wall and Robot Detection

## Introduction
The goal of milestone 2 was to implement wall-detection functionality and make the robot avoid other robots, while still successfully line-following in order to traverse the maze. To accomplish this task, we added short-range IR sensors to the robot to detect walls and refined our detection of the 6.08kHz IR signal emitted by the robot IR hat. Our final product was a demonstration of the robot traversing an arbitrary maze using a left-wall-following rule, and stopping whenever it detected the IR hat.

## Line-Following Improvements
Line-following functionality was previously implemented in milestone 1, but when we came back to this after working on Lab 2 we discovered that our implementation required some adjustments. Due to the added weight of new components, the power necessary to turn the robot 90 degrees increased; this threw off our initial code, so we re-calibrated the system. We also found that we needed to adjust the height of our line sensors. They were too low to the ground, which (somehow) led to sensing issues at intersections and caused the robot to occasionally stop inexplicably instead of turning. We added a spacer onto the screw that attaches the sensors to the robot, raising the sensors higher, and re-calibrated our white-black threshold.

## Wall Dectection
To avoid walls while navigating the maze, we attached short-range IR sensors to the front and left side of the chassis. These sensors detect objects at distances between 4 cm and 30cm, which is appropriate for our use.

The wall sensors output an analog value corresponding to the distance of an object from the sensor, and our algorithm sets threshold values to determine whether there is a wall in the grid space directly adjacent to the robot.  We implemented a left-hand wall following rule, defaulting to a left turn at an intersection if no wall is detected to the left. If the robot only detects a wall to the left, it goes straight, and if there is a wall both to the left and in front of the robot, it turns right.

<iframe width="560" height="315" src="https://www.youtube.com/embed/iyFj-MwOoxo" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

The core of our algorithm is a finite state machine. It has states that make the robot either turn left, turn right, or move forward, adjust the machine to stay on the line, and sense for other robots.


## Robot Avoidance

<iframe width="560" height="315" src="https://youtube.com/embed/DLj0mfdm-ms" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

### Coding Updates
We updated the FSM used in lab 2 integration with addtional states: 
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
We concentrated the FFT and processing of the audio and IR signals into two states. We also had states specifically for robot turning at intersections because we hope to eventually implement a more robust robot turning algorithm. The ROBOT_SENSE is main state that the robot will be in where it is following the line and detecting the intersections. ROBOT_DETECTED handles cases when we detect other robots with the IR.

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
We have somewhat of a skeleton code for ROBOT_DETECTED, ROBOT_TURN_LEFT, and ROBOT_TURN_RIGHT because we intended to implement them with more complexity than simple waits. However, the current code does the job for this milestone. 

In ROBOT_SENSE, we read from the wall sensors and also the line sensors to tell if we need to either re-adjust the robot to stay on the line or turn. Just like milestone 1, we only consider turning if both sensors detect white line, but this time, we turn based on the readings of the wall sensors. 

We currently perform left wall following so if there is no wall to the left, then we automatically turn left, otherwise we will consider right if we appoarching a wall. 

The IR sensing occurs every 800 ms, which can be adjusted later based on computation needs. Every 800 ms, we switch the state to IR_DECT and run the FFT with a processing algorithm. Currently, we only do averages of N FFT cycles but may consider moving averages.   

## Conclusion
We have now made further progress toward our robot being able to successfully navigate the final competition maze. In the future we intend to add a third short-range IR sensor to the right side of the robot to allow it to detect walls on all side for the purpose of mapping the maze. In addition, we would like to implement a more robust turning algorithm that does not rely on hard-coded delays.
