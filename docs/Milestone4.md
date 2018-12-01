# Milestone 4: Finishing the Robot with Treasure and Color Detection

## Introduction
The goal of milestone 4 are as follows:
- Build off our results from lab 4 and implement treasure shape detection.
- Integrate our camera and FPGA system into our robot. 

Treasure shape detection requires a more complicated algorithm then color detection because it deals directly with image processing. We must distinguish between three shapes: square, triangle, and diamond. For our algorithm to be robust, we must successfully threshold each shape.

Integration is not a trivial task. The challenges are in both hardware and software. On the hardware side, we must figure out the ideal place for the camera and FPGA in the robot chasis and route power to both these devices. The camera placement is important as it determines where the the robot can detect the treasure and on which wall. On the software side, we must integrate the treasure detection into the Robot control flow. We need to create new FSM states to account for the fact that treasures can exist at every intersection; this also means that there are more processing to do at every interestion. We must change the state transitions properly such that treasure detection does not interfere with the rest of the robot's functionality. The radio from lab 3 must also be updated because the radio only handles wall mapping and not treasure mapping. This applies for both basestation and robot.

## Updated Robot
We decided to update the robot chassis to fit the camera. The Robot will now have three layers. Previously, we used the second layer of the robot to hold the battery. However, we decided to use the second chassis layer of the robot to hold the FPGA and camera hardware. 

## Treasure Color Detection

<iframe width="560" height="315" src="https://www.youtube.com/embed/nbeuPRgot2Y?controls=0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

We already have color detection from lab4 algorithm which was reliable given the camera's

## Treasure Shape Detection

Treasure shape detection is 
```vhdl
always @(posedge CLK) begin
	if (!VGA_VSYNC_NEG) begin
		top = 0;
		mid = 0;
		bot = 0;
		red1 = 0;
		red2 = 0;
		red3 = 0;
		blue1 = 0;
		blue2 = 0;
		blue3 = 0;
		done_treasure = 0;
		treasure = NONE;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARfirstTop && VGA_PIXEL_Y < `Y_BARfirstBot) begin
		red1 = red1 + red;
		blue1 = blue1 + blue;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARsecondTop && VGA_PIXEL_Y < `Y_BARsecondBot) begin
		red2 = red2 + red;
		blue2 = blue2 + blue;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARthirdTop && VGA_PIXEL_Y < `Y_BARthirdBot) begin
		red3 = red3 + red;
		blue3 = blue3 + blue;
	end
	if (VGA_PIXEL_Y == `SCREEN_HEIGHT-3 && done_color && !done_treasure) begin
		if (color == RED) begin
			if (g3 && g2 && g1 && diffr3 > `HIGH_THR && diffr2 > `LOW_THR && diffr1 > `LOW_THR)
				//mid > top; bot > mid
				treasure = TRIANGLE;
			else if(g1 && diffr1 > `LOW_THR && !g2 && diffr2 > `LOW_THR && diffr3 < `LOW_THR)
				//mid > top; mid > bot
				treasure = DIAMOND;
			else
				treasure = SQUARE;
		end
		else if (color == BLUE) begin
			if (g4 && g5 && g6 && diffb3 > `HIGH_THR && diffb2 > `LOW_THR && diffb1 > `LOW_THR)
				treasure = TRIANGLE;
			else if(g4 && !g5 && diffb1 > `LOW_THR && diffb2 > `LOW_THR && diffb3 < `LOW_THR)
				treasure = DIAMOND;
			else
				treasure = SQUARE;
		end
		else begin
			treasure = NONE;
		end
		done_treasure = 1'b1;
	end
end
```

