`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144

`define Y_BARfirstTop 25
`define Y_BARfirstBot 45 

`define Y_BARsecondTop 65
`define Y_BARsecondBot 85

`define Y_BARthirdTop 105
`define Y_BARthirdBot 125

`define X_HIGH_THRESH 110
`define X_LOW_THRESH  50
`define Y_LOW_THRESH  20
`define Y_HIGH_THRESH  125

`define LOW_THR 10
`define HIGH_THR 20

module IMAGE_PROCESSOR (
	PIXEL_IN,
	CLK,
	VGA_PIXEL_X,
	VGA_PIXEL_Y,
	VGA_VSYNC_NEG,
	HREF,
	RESULT,
	RDY
);


//=======================================================
//  PORT declarations
//=======================================================
input	[7:0]	PIXEL_IN;
input 		CLK;

input [9:0] VGA_PIXEL_X;
input [9:0] VGA_PIXEL_Y;
input			VGA_VSYNC_NEG;
input 		HREF;

output [3:0] RESULT;
output		 RDY;

reg 	[2:0]	 current_state;
reg 	[2:0]	 next_state;

reg 			 done_color;
reg 			 done_treasure;

reg 	[15:0] red1;
reg 	[15:0] blue1;
reg 	[15:0] green1;

reg 	[15:0] red2;
reg 	[15:0] blue2;
reg 	[15:0] green2;

reg 	[15:0] red3;
reg 	[15:0] blue3;
reg 	[15:0] green3;


wire	[3:0]	 red;
wire	[3:0]	 blue;
wire	[2:0]  green;

assign red 	=	PIXEL_IN[7:5];
assign green=  PIXEL_IN[4:3];
assign blue =  PIXEL_IN[2:0];
//
//wire	[2:0]	 red;
//wire	[1:0]	 blue;
//wire	[2:0]  green;
//assign red 	=	PIXEL_IN[7:5];
//assign green=  PIXEL_IN[4:2];
//assign blue =  PIXEL_IN[1:0];

localparam  STATE_IDLE 			= 3'd0;
localparam  STATE_CHECK_BAR1 	= 3'd1;
localparam  STATE_CHECK_BAR2 	= 3'd2;
localparam  STATE_CHECK_BAR3 	= 3'd3;
localparam  STATE_CALC 			= 3'd4;
localparam  STATE_WAIT			= 3'd5;

localparam  RED			= 2'd2;
localparam  BLUE			= 2'd1;
localparam  NONE			= 2'd0;
localparam  TRIANGLE		= 2'd1;
localparam  SQUARE		= 2'd2;
localparam  DIAMOND		= 2'd3;

assign 		RESULT      = {color, treasure};
assign 		RDY         = done_color && done_treasure;



reg 	[15:0]  blueCount;
reg 	[15:0]  redCount;
reg 	[15:0]  greenCount;

reg [1:0]	color;
reg [1:0]   treasure;


///////* Color Detection *///////
always @(posedge CLK) begin

	if (VGA_PIXEL_Y <=`Y_LOW_THRESH) begin
		blueCount  = 0;
		redCount   = 0;
		greenCount = 0;
		done_color = 1'b0;
	end
	else if (VGA_PIXEL_X > `X_LOW_THRESH && VGA_PIXEL_X < `X_HIGH_THRESH && VGA_PIXEL_Y > `Y_LOW_THRESH && VGA_PIXEL_Y < `Y_HIGH_THRESH) begin
		blueCount = blueCount + blue;
		redCount  = redCount  + red;
		greenCount = greenCount + green;
	end
	else if(VGA_PIXEL_Y >= `Y_HIGH_THRESH && !done_color) begin
		if (greenCount > 15'd10000 && blueCount > 15'd20000 && redCount > 15'd20000) begin
			color = NONE;
		end
		else if (blueCount > redCount && (blueCount - redCount) > 30 && blueCount > 15'd10000) begin
			color = BLUE;
		end
		else if (redCount > blueCount && (redCount - blueCount) > 30 && redCount > 15'd10000) begin
			color = RED;
		end
		else begin
			color = NONE;
		end
		blueCount = 0;
		redCount  = 0;
		greenCount = 0;
		done_color = 1'b1;
	end
end

reg done_subtract;
///////* SHAPE Detection *///////
always @(posedge CLK) begin
	if (VGA_PIXEL_Y == 0) begin
		red1 = 0;
		red2 = 0;
		red3 = 0;
		blue1 = 0;
		blue2 = 0;
		blue3 = 0;
		done_treasure = 0;
		//treasure = NONE;
		done_subtract = 0;
	end
	if ((VGA_PIXEL_X > `X_LOW_THRESH) && (VGA_PIXEL_X < `X_HIGH_THRESH) && (VGA_PIXEL_Y > `Y_BARfirstTop) && (VGA_PIXEL_Y < `Y_BARfirstBot)) begin
		red1 = red1 + red;
		blue1 = blue1 + blue;
		green1 = green1 + green;
	end
	if ((VGA_PIXEL_X > `X_LOW_THRESH) && (VGA_PIXEL_X < `X_HIGH_THRESH) && (VGA_PIXEL_Y > `Y_BARsecondTop) && (VGA_PIXEL_Y < `Y_BARsecondBot)) begin
		red2 = red2 + red;
		blue2 = blue2 + blue;
		green2 = green2 + green;
	end
	if ((VGA_PIXEL_X > `X_LOW_THRESH) && (VGA_PIXEL_X < `X_HIGH_THRESH) && (VGA_PIXEL_Y > `Y_BARthirdTop) && (VGA_PIXEL_Y < `Y_BARthirdBot)) begin
		red3 = red3 + red;
		blue3 = blue3 + blue;
		green3 = green3 + green;
	end
	/*if(VGA_PIXEL_Y==`SCREEN_HEIGHT-5 && !done_subtract) begin
		red3 = red3 - green3;
		blue3 = blue3 - green3;
		red2 = red2 - green2;
		blue2 = blue2 - green2;
		red1 = red1 - green1;
		blue1 = blue1 - green1;
		done_subtract = 1'b1;
	end*/
	if ((VGA_PIXEL_Y == `SCREEN_HEIGHT) && done_color && !done_treasure) begin
	
		if (color == RED) begin
			if ((diffr3>diffr2) && (diffr3>diffr1) && diffr3>50)//g3 && g2 && g1 && diffr3>diffr2 && diffr3>diffr2)//diffr3 > `HIGH_THR && diffr2 > `LOW_THR && diffr1 > `LOW_THR)
				//mid > top; bot > mid
				treasure = TRIANGLE;
			else if((diffr1<100) && (diffr2<100) && (diffr3<100))//g1 && diffr1 > `LOW_THR && !g2 && diffr2 > `LOW_THR && diffr3 < `LOW_THR)
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

DIFF(.A(red1), .B(red2), .C(diffr1), .D(g1));
DIFF(.A(red2), .B(red3), .C(diffr2), .D(g2));
DIFF(.A(red1), .B(red3), .C(diffr3), .D(g3));

DIFF(.A(blue1), .B(blue2), .C(diffb1), .D(g4));
DIFF(.A(blue2), .B(blue3), .C(diffb2), .D(g5));
DIFF(.A(blue1), .B(blue3), .C(diffb3), .D(g6));


wire   [15:0]  diffr1;
wire   [15:0]  diffr2;
wire   [15:0]  diffr3;
wire   [15:0]  diffb1;
wire   [15:0]  diffb2;
wire   [15:0]  diffb3;
wire				g1;
wire				g2;
wire				g3;
wire				g4;
wire				g5;
wire				g6;

endmodule


module DIFF (
	input  [15:0] A,
	input  [15:0] B,
	output [15:0] C,
	output 		  D
);
assign C 	= A>B ? A-B: B-A;
assign D 	= A>B ? 0:1;

endmodule






