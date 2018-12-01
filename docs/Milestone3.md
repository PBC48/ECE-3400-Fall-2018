# Milestone 3: Search Algorithm (For Maze Exploration)

## Introduction
The goal of this milestone was to implement a search algorithm that would allow our robot to fully explore a maze of any configuration. In order to visit every square in a maze with an arbitrary configuration of walls,  the robot’s next move is decided with a more intelligent algorithm than our initial left-hand-following rule. We incorporated a Depth-First Search algorithm into our Arduino code to guide the robot’s exploration the maze. Additionally, we updated our basestation code to better receive and process the information the robot collects as it explores the maze. We then demonstrated that our robot was capable of displaying that information to the GUI.

## Search Algorithm
After playing around with Dijkstra’s Algorithm and realizing how difficult it would be to implement in reality on the Arduino, we decided to write a simple Depth First Search algorithm to determine our robot’s path when exploring a maze.  This type of search algorithm traverses as far as there is to go down a single path before backtracking to explore the next path, with a predetermined preference for the order in which the path options are explored. This algorithm  For the purpose of exploring a maze, this means turning around to backtrack only when all surrounding squares are either a wall or previously explored. The algorithm takes into account the spaces the robot has already encountered as to allow it to explore more paths in as little amount of time as possible. In addition, the search algorithm keeps the robot from traversing in an endless loop.

### Regular Speed:
<iframe width="560" height="315" src="https://www.youtube.com/embed/p1IaieuAFX0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

### 3x Speed:
<iframe width="560" height="315" src="https://www.youtube.com/embed/JbzAFhrltYo" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

## Basestation Modification
In order to reflect the modifications to the robot exploration code, we also had to update our basestation code to better receive and process the information the robot collects as it explores the maze. 

## Final implementation / Demonstration
Finally, we (added GUI)... In the end, we were able to correctly represent the entire maze with the GUI on the basestation’s screen.

<iframe width="560" height="315" src="https://www.youtube.com/embed/cKP1L6YakC4" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>
