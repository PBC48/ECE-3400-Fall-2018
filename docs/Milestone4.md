# Milestone 4: Finishing the Robot with Treasure and Color Detection

## Introduction
The goal of milestone 4 are as follows:
- Build off our results from lab 4 and implement treasure shape detection.
- Integrate our camera and FPGA system into our robot. 

Treasure shape detection requires a more complicated algorithm then color detection because it deals directly with image processing. We must distinguish between three shapes: square, triangle, and diamond. For our algorithm to be robust, we must successfully threshold each shape.

Integration is not a trivial task. The challenges are in both hardware and software. On the hardware side, we must figure out the ideal place for the camera and FPGA in the robot chasis and route power to both these devices. The camera placement is important as it determines where the the robot can detect the treasure and on which wall. On the software side, we must integrate the treasure detection into the Robot control flow. We need to create new FSM states to account for the fact that treasures can exist at every intersection; this also means that there is more processing to do at every interestion. We must change the state transitions properly such that treasure detection does not interfere with the rest of the robot's functionality. The radio from lab 3 must also be updated because the radio only handles wall mapping and not treasure mapping. This applies for both basestation and robot.

## Updated Robot
We decided to update the robot chassis to fit the camera. The Robot will now have three layers. We will add a new layer under the current first layer of the robot where the servos are. This will be used to store the batteries. We will need two batteries, one for the Arduino and one for servo and sensors. Previously, we used the second layer of the robot to hold the battery. However, we decided to use the second chassis layer of the robot to hold the FPGA and camera hardware. The third layer will host the Arduino and sensor circuitry.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/milestone4/IMG_1187.jpg" width="500"/>
    <font size="3">
    <figcaption> <b>Camera and FPGA Mounted</b>
    </figcaption>
    </font>
</figure>

In the image above, we mounted the Camera and FPGA on a a new chasis. We also soldered some of the intermediate connectors onto a protoboard such as the resistors required for I2C. This will be the new second level of the robot. We decided to have the camera face only one direction and only have the robot detect one of the walls at each intersection. We will miss some of the walls but this allows the robot movement algorithm to be simpler.

## Treasure Color Detection

<iframe width="560" height="315" src="https://www.youtube.com/embed/nbeuPRgot2Y?controls=0" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

We already have color detection from lab4 algorithm which was reliable if the camera can reliably detect the colors. We debug the camera by using VGA display connected to a monitor to see what the camera sees by displaying the QCIF image on the monitor. The camera clearly has trouble capturing color on the image if the lighting is poor. This leads to misdetections which our algorithms can't solve since the problem lies in the quality of the image. We can only hope that the lighting during the competition is better than in the lab. We may also consider adding a light pointing in the same direction as the camera so that it can better capture the image.

Another strategy we used to improve reliability was to use averages. This method allows us to avoid outliers in detection. 

```cpp
for (int i = 0; i<500; i++){
    treasure1 = digitalRead(T1);
    treasure2 = digitalRead(T2);
    color1     = digitalRead(C1);
    color2     = digitalRead(C2);
    treasure  = (treasure2 << 1)|treasure1;
    color   =   (color2<<1)|(color1);
    if(color==BLUE) blueCount += 1;
    else if(color==RED)   redCount += 1;
    else nonCount += 1;
    if (treasure == SQUARE) sqCount +=1;
    else if( treasure == TRIANGLE) triCount += 1;
    else if( treasure == DIAMOND)  diaCount += 1;
    else nonTreasure += 1;
    delay(2);
}
```

The code above shows us processing averages on the Arduino where we would take multiple readings over a second and then count the predictions. We would then choose the prediction with the highest count as our answer. To execute this well, we would need to be wary of timing constraints of the camera. 

The camera completes transmission for a pixel of data every two clock cycles. An image is 176 x 144 in size in terms of pixels and our clock is 25 MHz. This translates to two milliseconds per image for processing our one image. We count the predictions for 500 images.

## Treasure Shape Detection

Treasure shape detection is much more difficult than color detection. We needed to detect the differences in color concentration between different parts of the image. Our design is to insert three different bars in the image and only count the color values within those bars. In theory, the bars will have differences in values for different shapes and we can compare the bars and distinguish between the shape of the treasure. 

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
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < 
    `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARfirstTop && VGA_PIXEL_Y < 
    `Y_BARfirstBot) begin
		red1 = red1 + red;
		blue1 = blue1 + blue;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < 
    `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARsecondTop && VGA_PIXEL_Y < 
    `Y_BARsecondBot) begin
		red2 = red2 + red;
		blue2 = blue2 + blue;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < 
    `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_BARthirdTop && VGA_PIXEL_Y <
    `Y_BARthirdBot) begin
		red3 = red3 + red;
		blue3 = blue3 + blue;
	end
	if ((VGA_PIXEL_Y == `SCREEN_HEIGHT) && done_color && !done_treasure) 
    begin
	
		if (color == RED) begin
			if ((diffr3>diffr2) && (diffr3>diffr1) && diffr3>50)
				//mid > top; bot > mid
				treasure = TRIANGLE;
			else if((diffr1<100) && (diffr2<100) && (diffr3<100))
				//mid ~= top; mid ~= bot
				treasure = SQUARE;
			else
				treasure = DIAMOND;
		end
		else if (color == BLUE) begin
			if ((diffb3>diffb2) && (diffb3>diffb1) && diffb3>50)
				treasure = TRIANGLE;
			else if((diffb1<100) && (diffb2<100) && (diffb3<100))
				treasure = SQUARE;
			else
				treasure = DIAMOND;
		end
		else begin
			treasure = NONE;
		end
		done_treasure = 1'b1;
	end
end
```

In the code above, g1-g6 shows which bar has greater color concentration while diffb and diffr are the actual color differences.

## Conclusion

We were able to detect both color and shape detection on the FPGA and mounted the camera onto the robot chassis. Moving forward, we can explore implementing the averaging on the FPGA instead of the Arduino for more timely image processing averages. We can also improve reliability by increasing the number of bars. This will increase code density but will likely improve accuracy. The greatest challenge to accuracy of the camera remains that the camera needs good lighting to capture reliable images.   

