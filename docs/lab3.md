# Lab 3: System Integration and Radio Communication

## Objectives
* Radio communication between two arduinos.
* Communicate with the GUI to displayed explored vs unexplored areas.
* Integrate all microphone, radio, and IR components into the robot.

## Introduction

## Radio

### Sending Maze Info Between Arduinos

### Simulating the Robot

#### Data Structure
```
 15                  9 8     7    6 5       3 2       0
+---------------------+-----+------+---------+---------+
| Reserved            |ROBOT|DIR   |TREASURE | WALLS   |
+---------------------+-----+------+---------+---------+

[2:0]  We reserve three bits for detecting walls. The position of the walls will be relative to the robot. 

[5:3]  We reserve three bits for the treasure since there will be two shapes and two colors, giving us four options, we also need to define when there is no treasure. Thus, we have a total of five options.

[7:6]  The direction the robot has decided to take after hitting the intersection

[8]    Robot detection

[15:9] Reserved for potentially later usage. Allows filled with 0's.

```


### Base Station-to-GUI Transmission


## Robot Integration

### Robot State Machine

### Integration with Mic

### Robot Navigation through the maze

### Integration with Radio

## Conclusion 
