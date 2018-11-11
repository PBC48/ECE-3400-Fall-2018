`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144
`define BAR1 48
`define BAR2 72
`define BAR3 96
`define HIGH_THRESH 300
`define TRIANGLE_THRESH 1000
`define DIAMOND_THRESH  25
`define SQUARE_THRESH   50
`define X_THRESH 			30
`define COLOR_THRESH    1
module IMAGE_PROCESSOR (
	PIXEL_IN,
	CLK,
	VGA_PIXEL_X,
	VGA_PIXEL_Y,
	VGA_VSYNC_NEG,
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

output [7:0] RESULT;
output		 RDY;

reg 	[2:0]	 current_state;
reg 	[2:0]	 next_state;
reg	[7:0]	 out;
reg 			 done;

reg 	[15:0] red1;
reg 	[15:0] blue1;

reg 	[15:0] red2;
reg 	[15:0] blue2;

reg 	[15:0] red3;
reg 	[15:0] blue3;

reg   [15:0] redTotal;
reg   [15:0] blueTotal;
reg	[15:0] greenTotal;

/*wire	[2:0]	 red;
wire	[2:0]	 blue;
wire	[1:0]  green;
assign red 	=	PIXEL_IN[7:5];
assign green=  PIXEL_IN[4:3];
assign blue =  PIXEL_IN[2:0];*/

wire	[2:0]	 red;
wire	[1:0]	 blue;
wire	[2:0]  green;
assign red 	=	PIXEL_IN[7:5];
assign green=  PIXEL_IN[4:2];
assign blue =  PIXEL_IN[1:0];

localparam  STATE_IDLE 			= 3'd0;
localparam  STATE_CHECK_BAR1 	= 3'd1;
localparam  STATE_CHECK_BAR2 	= 3'd2;
localparam  STATE_CHECK_BAR3 	= 3'd3;
localparam  STATE_CALC 			= 3'd4;
localparam  STATE_WAIT			= 3'd5;

localparam  RED			= 1'd0;
localparam  BLUE			= 1'd1;
localparam  NONE			= 2'd0;
localparam  TRIANGLE		= 2'd1;
localparam  SQUARE		= 2'd2;
localparam  DIAMOND		= 2'd3;

assign 		RESULT      = out;
assign 		RDY         = done;

always @(posedge CLK) begin
	if (!VGA_VSYNC_NEG) 
		current_state 		= STATE_IDLE;
	else 
		current_state  	= next_state;
end

always @(*) begin
	case (current_state) 
		STATE_IDLE:begin
			if (VGA_PIXEL_Y == `BAR1) 
				next_state 			= STATE_CHECK_BAR1;
			else if (VGA_PIXEL_Y == `BAR2) 
				next_state 			= STATE_CHECK_BAR2;
			else if (VGA_PIXEL_Y == `BAR3) 
				next_state 			= STATE_CHECK_BAR3;
			else
				next_state 		   = STATE_IDLE;
		end
		STATE_CHECK_BAR1: begin
			next_state   			= (VGA_PIXEL_Y == `BAR1) ? STATE_CHECK_BAR1 : STATE_WAIT;
		end
		STATE_CHECK_BAR2: begin
			next_state   			= (VGA_PIXEL_Y == `BAR2) ? STATE_CHECK_BAR2 : STATE_WAIT;
		end
		STATE_CHECK_BAR3: begin
			next_state   			= (VGA_PIXEL_Y == `BAR3) ? STATE_CHECK_BAR3 : STATE_CALC;
		end
		STATE_CALC: begin
			next_state				= STATE_IDLE;
		end
		STATE_WAIT: begin
			if (VGA_PIXEL_Y == `BAR1) 
				next_state 			= STATE_CHECK_BAR1;
			else if (VGA_PIXEL_Y == `BAR2) 
				next_state 			= STATE_CHECK_BAR2;
			else if (VGA_PIXEL_Y == `BAR3) 
				next_state 			= STATE_CHECK_BAR3;
			else
				next_state 		   = STATE_WAIT;
		end
		
		default:begin
			next_state				= STATE_IDLE;
		end
	endcase
end

always @(*) begin
	case (current_state) 
		STATE_IDLE: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= 0;
			blueTotal				= 0;
		end
		STATE_CHECK_BAR1: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= 0;
			blueTotal				= 0;
		end
		STATE_CHECK_BAR2: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= 0;
			blueTotal				= 0;
		end
		STATE_CHECK_BAR3: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= 0;
			blueTotal				= 0;
		end
		STATE_CALC: begin
			redTotal					= red1 + red2 + red3;
			blueTotal				= blue1 + blue2 + blue3;
			if (blueTotal > `COLOR_THRESH || redTotal > `COLOR_THRESH) begin
				if (blueTotal > redTotal)
					out            = {5'b0, BLUE, NONE};
			   else
				   out            = {5'b0, RED,  NONE};
		   end
/*			if (blueTotal > redTotal && (blueTotal > `HIGH_THRESH || (redTotal > `HIGH_THRESH)))  begin
				if (((blue3 > blue2) && (blue2 > blue1)) && ((blue3 - blue2)>`TRIANGLE_THRESH) && ((blue2-blue1)>`TRIANGLE_THRESH))
				   out            = {5'b0, BLUE, TRIANGLE};
				else if (((blue2 > blue1) && (blue2 > blue3)) && ((blue2-blue1) > `DIAMOND_THRESH) && ((blue2-blue3)>`DIAMOND_THRESH))
				   out            = {5'b0, BLUE, DIAMOND};
				else 
					out  				= {5'd0, BLUE, SQUARE};	
			end
			else if (redTotal > blueTotal && (blueTotal > `HIGH_THRESH || (redTotal > `HIGH_THRESH)))begin
			   if (((red3 > red2) && (red2 > red1)) && ((red3 - red2)>`TRIANGLE_THRESH) && ((red2-red1)>`TRIANGLE_THRESH))
				   out            = {5'b0, RED, TRIANGLE};
				else if (((red2 > red1) && (red2 > red3)) && ((red2-red1) > `DIAMOND_THRESH) && ((red2-red3)>`DIAMOND_THRESH))
				   out            = {5'b0, RED, DIAMOND};
				else 
					out  				= {5'd0, RED, SQUARE};	
			end*/
			else 
				out					= 8'd0;
			done						= 1'b1;
		end
		STATE_WAIT: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= redTotal;
			blueTotal				= blueTotal;
		end
		default: begin
			out 						= 8'b0;
			done						= 1'b0;
			redTotal					= 0;
			blueTotal				= 0;
		end
	endcase
end

always @(posedge CLK) begin
	case (current_state)
		STATE_IDLE: begin
			red1						<= 0;
			blue1						<= 0;
			red2						<= 0;
			blue2						<= 0;
			red3						<= 0;
			blue3						<= 0;
		end
		
		STATE_CHECK_BAR1: begin
			red1						<= (VGA_PIXEL_X > `X_THRESH) ? red1 + red*(3-green) : red1;
			blue1						<= (VGA_PIXEL_X > `X_THRESH) ? blue1+ 2*blue*(3-green): blue1;
		end
		STATE_CHECK_BAR2: begin
			red2						<= (VGA_PIXEL_X > `X_THRESH) ? red2 + red*(3-green) : red2;
			blue2						<= (VGA_PIXEL_X > `X_THRESH) ? blue2+ 2*blue*(3-green): blue2;
		end
		STATE_CHECK_BAR3: begin
			red3						<= (VGA_PIXEL_X > `X_THRESH) ? red3 + red*(3-green) : red3;
			blue3						<= (VGA_PIXEL_X > `X_THRESH) ? blue3+ 2*blue*(3-green): blue3;
		end
		
		STATE_WAIT: begin
		end
		
		STATE_CALC: begin
		
		end
		default: begin
			
		end
	endcase
end

endmodule


