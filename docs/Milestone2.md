# Milestone 2: Wall and Robot Detection

## Introduction


## Line Tracking


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone1/20180910_213436.jpg" width="800"/>
    <font size="3">
    <figcaption> <b>Front View of Robot after Adding a Second Level</b>
    </figcaption>
    </font>
</figure>


## Wall Dectection


## Avoidance

### Coding
For the coding section, we updated the FSM used in lab 2 integration with addtional states: 
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
We concentrated the FFT and processing of the audio and IR signals into two states. We also had states specifically for robot turning at intersections because we hope to eventually implemnet a more robust robot turning algorithm. The ROBOT_SENSE is main state that the robot will be in where it is following the line and detecting the intersections. ROBOT_DETECTED handles cases when we detect other robots with the IR.

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
We have somewhat of a skeleton code for ROBOT_DETECTED, ROBOT_TURN_LEFT, and ROBOT_TURN_RIGHT because we intended to implement them with more complexity than simple waits. However, the current code do the job for this milestone. 

For ROBOT_SENSE, we read from the wall sensors and also the line sensors to tell if we need to either re-adjust the robot to stay on the line or turn. Just like milestone 1, we only consider turning if both sensors detect white line, but this time, we turn based on the readings of the wall sensors. 

We currently perform left wall following so if there is no wall to the left, then we automatically turn left, otherwise we will consider right if we appoarching a wall. 

The IR sensing occurs every 800 ms, which can be adjusted later based on computation needs. Every 800 ms, we switch the state to IR_DECT and run the FFT with a processing algorithm. Currently we only do averages of N FFT cycles but may consider moving averages.   

## Conclusion

