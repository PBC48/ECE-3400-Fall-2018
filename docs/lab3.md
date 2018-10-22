# Lab 3: System Integration and Radio Communication

## Objectives
* Radio communication between two arduinos.
* Communicate with the GUI to displayed explored vs unexplored areas.
* Integrate all microphone, radio, and IR components into the robot.

## Introduction

## Radio

### Sending Maze Info Between Arduinos

### Simulating the Robot

### Base Station-to-GUI Transmission


## Robot Integration

#### Data Structure
This is the two byte communication message the robot sends to the base station.
```
 15                  9 8     7    6 5       3 2       0
+---------------------+-----+------+---------+---------+
| Reserved            |ROBOT|DIR   |TREASURE | WALLS   |
+---------------------+-----+------+---------+---------+

[2:0]  We reserve three bits for detecting walls. The position of the walls will be relative 
to the robot. 

[5:3]  We reserve three bits for the treasure since there will be two shapes and two colors, 
giving us four options, we also need to define when there is no treasure. Thus, we have a 
total of five options.

[7:6]  The direction the robot has decided to take after hitting the intersection

[8]    Robot detection

[15:9] Reserved for potentially later usage. Allows filled with zeros.

```

This is the one byte communication message the base station sends back to the robot.
```
 7           4 3          0
+-------------+------------+
|Reserved     | EXPLORED   |
+-------------+------------+

[3:0]  Three bits for whether the relative locations next to the robot has been explored 
or not

[7:4]  Reserved for later usage. Allows filled with zeros

```
The message sent back to the robot allows the robot to make decisions based on the state of the maze. The robot can make decisions based on whether the locations near it has been explored or not. This can affect which direction the robot turns in an intersection. This comes at a cost of implementing additional decoders for the robot on the system.

### Robot State Machine

### Integration with Mic

### Robot Navigation through the maze

### Integration with Radio
For the integration with the radio, we decided to do most of the decoding and data processing on the base station and off the robot system because we are already running low on memory and we can delegate some of the maze processing to the base station and have it send back a response. The robot only needs to send the readings from its wall sensors, treasure detection, and direction it intents to move. The base station will take the data and come up with absolute coordinates and wall locations based on robots previous locations. The base station can then update the GUI accordingly. 

## Conclusion 
