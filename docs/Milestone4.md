# Milestone 4: Finishing the Robot with Treasure and Color Detection

## Introduction
The goal of milestone 4 are as follows:
- Build off our results from lab 4 and implement treasure shape detection.
- Integrate our camera and FPGA system into our robot. 

Treasure shape detection requires a more complicated algorithm then color detection because it deals directly with image processing. We must distinguish between three shapes: square, triangle, and diamond. For our algorithm to be robust, we must successfully threshold each shape.

Integration is not a trivial task. The challenges are in both hardware and software. On the hardware side, we must figure out the ideal place for the camera and FPGA in the robot chasis and route power to both these devices. The camera placement is important as it determines where the the robot can detect the treasure and on which wall. On the software side, we must integrate the treasure detection into the Robot control flow. We need to create new FSM states to account for the fact that treasures can exist at every intersection; this also means that there are more processing to do at every interestion. We must change the state transitions properly such that treasure detection does not interfere with the rest of the robot's functionality. The radio from lab 3 must also be updated because the radio only handles wall mapping and not treasure mapping. This applies for both basestation and robot.

## Updated Robot

## Treasure Shape Detection

