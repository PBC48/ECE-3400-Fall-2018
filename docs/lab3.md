# Lab 3: System Integration and Radio Communication

## Objectives
* Design an efficient data scheme to store all maze information on an Arduino
* Familiarize ourselves with Nordic radio transceivers and the RF24 Arduino library in order to communicate between radios on two different Arduinos and send maze information wirelessly
* Communicate with the GUI and update it from a wirelessly connected Arduino to display explored vs unexplored areas of the maze
* Fully ntegrate all components onto the robot so it starts on a 660 Hz tone, explores the entire maze, stops if it sees another robot but ignores decoys, and displays maze information on the GUI via radio communication with the base station


## Introduction
For this lab we integrated all of the robot’s capabilities that we had previously implemented into a cohesive system that communicates wirelessly with the provided GUI.  We first worked on creating an algorithm that would efficiently store all the information our robot detects as it navigates through the maze.  Next, one subteam (Patrick and Chrissy) worked on adding the radio component to the robot, setting up the two Nordic nRF24L01+ transceivers that we would use to communicate wirelessly between two Arduinos, one on the robot and one on a base station connected to the GUI.  Meanwhile the other subteam (Tara and Xiaoyu) integrated all the robot’s other functionalities: starting on detection of a 660 Hz tone, line following, wall detection, and detection of other robots while ignoring decoys. At the end of the lab we combined all of the work so that the robot can autonomously explore the maze and update the GUI in real time.

## Radio
For the radio portion of the lab we implemented two Nordic nRF24l01+ transceivers. To test out the connection between the two radios, we ran the example “GettingStarted” code on two separate Arduinos to which we attached one radio each, and made sure they properly communicated. After that, we integrated othe radios onto the base station (an Arduino connected to a computer) and the Arduino on the robot. While the Arduino does have a 3.3V power supply, it cannot support the current necessary to power the radio. The Arduino's 5V power supply can, however, and therefore we implemented a 3.3V voltage regulator for each radio so that we can step down from the 5V to power the radios. This made the radios more portable since we do not need to rely on the 3.3V from the DC power supply. We tested the radios with these power sources and they worked.

The code for the radio involved setting the correct pipes to send our messages through the radio. Since we are team 7, our pin values are ```const uint64_t pipes[2] = { 0x0000000014LL, 0x0000000015LL };```. The transmitter writes to pipe 0x14 while reading from pipe 0x15; the receiver reads and writes the other way around. The RF24 library abstracts many of the complexities of radio transmission so that we only need to call some already-written functions to send messages with the radio.

To transmit, we call ```radio.write( &buff, sizeof(buff) )```, where the buffer can be an integer. 
After writing, the transmitter has the option to hear back from the receiver, which is done with the code shown below. This allows us to check that the radio has delivered its package correctly, as well as send feedback between the robot and the base station.

```cpp
      radio.startListening();
      uint16_t buffer; 
      radio.read( &buffer, sizeof(buffer) );
      radio.stopListening();
```


### Sending Maze Information Between Arduinos
<iframe width="560" height="315" src="https://www.youtube.com/embed/flzf6dUwdMM" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>


#### Data Structure
The two byte communication message that the robot sends to the base station is structured like this:
```
 15                  9 8     7    6 5       3 2       0
+---------------------+-----+------+---------+---------+
| Reserved            |ROBOT|DIR   |TREASURE | WALLS   |
+---------------------+-----+------+---------+---------+

```
[2:0]  We reserve three bits for detecting walls. The position of the walls 
will be relative to the robot. 

|bits[2:0]| Wall Locations |  
|---|---|
|000|left_wall, front_wall, right_wall = false|
|001|left_wall = true| 
|010|front_wall = true|
|011|left_wall, front_wall = true|
|100|right_wall = true|
|101|left_wall, right_wall = true|
|110|left_wall, front_wall = true|
|111|left_wall, front_wall, right_wall = true|

[5:3]  We reserve three bits for the treasure since there will be three shapes and 
two colors, giving us four options, we also need to define when there is no 
treasure. Thus, we have a total of five options.



[7:6]  The direction the robot has decided to take after hitting the 
intersection

|bits[7:6]|Directions|
|---------|----------|
|0|forward|
|1|right|
|2|left|
|3|reverse|

[8]    Robot detection

[9]    Valid message

[15:10] Reserved for potentially later usage. Allows filled with zeros.


The one byte communication message that the base station sends back to the robot is structured like this:
```
 7           4 3          0
+-------------+------------+
|Reserved     | EXPLORED   |
+-------------+------------+

```

[3:0]  Three bits for whether the relative locations next to the robot has 
been explored or not

[7:4]  Reserved for later usage. Allows filled with zeros

The message sent back to the robot allows the robot to make decisions based on the state of the maze. The robot can make decisions based on whether the locations near it has been explored or not. This can affect which direction the robot turns in an intersection. This comes at a cost of implementing additional decoders for the robot on the system.

### Simulating the Robot
To simulate the robot on the base station, we take the information that the robot sends and structure it into a format that the GUI can pick up.

### Base Station-to-GUI Transmission
We simulated the base station to GUI transmission by using print statements in the base station. The base station receives messages from the robot and decodes the message with the absolute directions in North, West, East, and South. The base station also prints out any detected treasures. The base station uses both the robot's wall sensor values and the direction the robot decides to go to deduce the next square the robot will be. Then it uses the robot's facing direction to figure out the absolute position on the maze. We always start our maze with the robot facing 'right'. As the robot moves through the maze, we update the GUI one intersection at a time. 
``` cpp
byte * decoder(uint16_t input){
    byte wforward  = (input>>1) & 0x0001;
    byte wright = (input >> 2) & 0x0001;
    byte wleft = input & 0x0001;
    byte robot = (input >> 8) & 0x0001;
    byte dir = ((unsigned)input >> 6) & 0x0003;
    byte treasure = (input >> 3) & 0x0007;
    byte output[]={wleft, wright, wforward, robot, treasure, dir};
    return output; 
}
```
This is the decoder we used to get all the necessary values. We use the wall directions in combination with the direction the robot decided to take to update the robot's position on the maze for the GUI.

To update the robot's absolute position in the maze, we use the direction the robot is facing and where the robot decide go towards in the intersection to up date the robot.

Code snippet for updating the maze locations.
```cpp
        case forward: //forward : robot decided to go forward
            if(robot_direction==right){
                x++;
            }else if(robot_direction==left){
                x--;
            }else if(robot_direction==up){
                y--;
            }else if(robot_direction==down){
                y++;
            }
            break;
```

Once the logic is set in place, we print the required values to the monitor so that the GUI can pick it up. We call ```python main.py COM# --rows 3 --cols 2``` for a 3x2 maze

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab03/maze.PNG" width="400"/>
    <font size="2">
    <figcaption> <b> GUI of the 3x2 Maze </b>
    </figcaption>
    </font>
</figure>
There is a missing wall because the robot haven't explored that wall yet since the robot starts facing away from the wall.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab03/mazeResp.PNG" width="800"/>
    <font size="2">
    <figcaption> <b> Printout for GUI</b>
    </figcaption>
    </font>
</figure>
The base station prints these values to the GUI for interpretation.

## Robot Integration
The robot subteam’s task was to integrate all the pieces we’ve been implementing into one cohesive system. A fair amount of this had already been completed for Milestone 2. What remained was to add the functionality of starting at a 660 Hz tone, as well as sending signals between radios to the base station. We also had to make sure all the necessary hardware was integrated on the robot.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab03/IMG_0386.jpg" width="800"/>
    <font size="2">
    <figcaption> <b> Adding the Radio to the Robot </b>
    </figcaption>
    </font>
</figure>


### Robot State Machine
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab03/FSM.PNG" width="800"/>
    <font size="2">
    <figcaption> <b> State Machine for the Robot </b>
    </figcaption>
    </font>
</figure>

### Integration with Mic
In order to make the robot start exploring the maze when we play the 660Hz tone, as it will in the actual competition, we added the audio portion of lab 2 onto the robot. We moved the circuitry and microphone onto the robot’s breadboard and connected the output signal to the Arduino.
We had previously been having trouble with the range of our 660Hz tone detection; the microphone was only able to distinguish it from background noise when we played the tone from or phones about an inch away. We learned that if we unplugged the power from our wall sensors our results improved significantly, presumably because this gave more power to the microphone and allowed it to better pick up audio signals. To solve this problem, we decided to move the microphone power to the arduino, while keeping the wall sensor power on the main breadboard circuit powered by the battery.

### Robot Navigation through the maze
Since we had already implemented a left-wall-following rule in milestone 2, our navigation implementation was already complete, and we did not have to change any of our code involving our wall and line sensors or turning conditionals. 
<iframe width="560" height="315" src="https://www.youtube.com/embed/zy4pa3PNozo" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

### Integration with Radio
For the integration with the radio, we decided to do most of the decoding and data processing on the base station and off the robot system because we are already running low on memory and we can delegate some of the maze processing to the base station and have it send back a response. The robot only needs to send the readings from its wall sensors, treasure detection, and direction it intents to move. The base station will take the data and come up with absolute coordinates and wall locations based on robots previous locations. The base station can then update the GUI accordingly. 

## Conclusion 
Overall, Lab 3 proved to be quite a challenge. We learned a lot about wireless communication, bit optimization, and integration and testing. With the addition of new components, we needed to better optimize the circuit layout on our robot and the power supplies for each component. Through the *frustration* of component faults and other issues, we’ve learned how to better debug our problems.

