# Milestone 3: Search Algorithm for Maze Exploration

## Introduction
The goal of this milestone was to implement a search algorithm that would allow our robot to fully explore a maze of any configuration. In order to visit every square in a maze with an arbitrary configuration of walls, the robot’s next move is decided with a more intelligent algorithm than our initial left-hand-following rule. We incorporated a Depth-First Search algorithm into our Arduino code to guide the robot’s exploration the maze. Additionally, we updated our basestation code and demonstrated that we are able to display the information the robot collects on the GUI.

## Search Algorithm
After playing around with Dijkstra’s Algorithm and realizing how difficult it would be to implement in reality on the Arduino, we decided to write a simple Depth First Search algorithm to determine our robot’s path when exploring a maze. This type of search algorithm traverses as far as there is to go down a single path before backtracking to explore the next path, with a predetermined preference for the order in which the path options are explored. For the purpose of exploring a maze, this means turning around to backtrack only when all surrounding squares are either a wall or previously explored. The algorithm takes into account the spaces the robot has already encountered in order to allow it to explore more paths in as short a time as possible. In addition, the search algorithm keeps the robot from traversing in an endless loop. Although we at first tried to implement this algorithm with the robot virtually exploring the maze to find an unexplored square at every intersection, we eventually went the much simpler route of exploring the maze one square at a time as the robot traveled through it. Our logic took the following form:

```cpp
  if( (no_front_wall) && (unexplored(forward_space)) ) {
    go_forward;
  }
```
Our search order preference for the algorithm is forward, right, left, then backward. The robot keeps a list of the squares it has visited and the order in which it has visited them in order to allow it to backtrack to the next unexplored path. It also keeps track of its position and direction. Once the algorithm was implemented into our code, we successfully tested the it in several different maze configurations of various sizes and confirmed that it explored as we wanted it to. The following video is a demonstration of one of these tests:

### Regular Speed:
<iframe width="560" height="315" src="https://www.youtube.com/embed/p1IaieuAFX0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

### 3x Speed:
<iframe width="560" height="315" src="https://www.youtube.com/embed/JbzAFhrltYo" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Basestation Modification
In order to reflect the modifications made to the robot’s code, we also had to update our basestation code to better receive and process the information the robot collects as it explores the maze. We had not touched the code since we initially wrote it for Lab 3, so we had to make sure it would correctly interpret the radio messages from the robot and update the GUI to correctly display what the robot discovers. Since we had to keep track of the robot’s position and direction for the algorithm, we were able to simplify the basestation code by sending this information directly from the robot rather than having the basestation calculate and keep track of it. This decreases the probability of discrepancies between the robot’s exploration and the GUI. We therefore slightly changed the structure of our radio messages, so they now look like this: 
```
 15        12 11         8 7    6 5       3 2       0
+------------+------------+------+---------+---------+
| X-POS      | Y-POS      | DIR  |TREASURE | WALLS   |
+------------+------------+------+---------+---------+

```
With the first 8 bits dedicated to the x- and y-coordinates of the robot in the maze, the next 2 bits dedicated to the robot's direction, the following 3 bits dedicated to any treasures detected, and the last 3 bits dedicated to the readings from the three wall sensors.

Additionally, we coded the basestation to display a mirrored version of the maze that the robot explores due to the fact that our algorithm considers the point (0,0) to be the bottom-left corner, and the GUI treats (0,0) as the top-left corner.

## Final Implementation and Demonstration
Once the basestation was fixed, we tested the search algorithm with the radios and a fully operational basestation. We confirmed that the basestation was aligned with the radio messages sent by the robot, and able to decode them to output the correct information. Finally, we ran the GUI to display the output from the basestation. In the end, we were able to correctly represent the entire maze with the GUI on the basestation’s screen. The following video is a demonstration of the robot exploring an entire maze and updating the GUI to display it correctly:

<iframe width="560" height="315" src="https://www.youtube.com/embed/cKP1L6YakC4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

We also demonstrated that the robot was able to start at a 660Hz tone and explore an entire maze with the algorithm, although we were unable to get complete video evidence of this because the GUI kept crashing around 5 or 6 squares into the maze exploration.

<iframe width="560" height="315" src="https://www.youtube.com/embed/gVXUPPQBTW0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
