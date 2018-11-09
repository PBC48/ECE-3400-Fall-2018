`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144
`define NUM_BARS 3
`define BAR1 48
`define BAR2 72
`define BAR3 96

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

output [8:0] RESULT;
output		 RDY;

reg 	[2:0]	 current_state;
reg 	[2:0]	 next_state;
reg	[8:0]	 out;
reg 			 done;
reg 	[15:0] red1;
reg 	[15:0] blue1;

reg 	[15:0] red2;
reg 	[15:0] blue2;

reg 	[15:0] red3;
reg 	[15:0] blue3;

reg   [15:0] redTotal;
reg   [15:0] blueTotal;

wire	[2:0]	 red;
wire	[2:0]	 blue;
assign red 	=	PIXEL_IN[7:5];
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
			if (!(VGA_PIXEL_Y == `BAR1 || VGA_PIXEL_Y == `BAR2 ||VGA_PIXEL_Y == `BAR3))
				next_state 			= STATE_WAIT;
		end
		STATE_CHECK_BAR2: begin
			if (!(VGA_PIXEL_Y == `BAR1 || VGA_PIXEL_Y == `BAR2 ||VGA_PIXEL_Y == `BAR3))
				next_state 			= STATE_WAIT;
		end
		STATE_CHECK_BAR3: begin
			if (!(VGA_PIXEL_Y == `BAR1 || VGA_PIXEL_Y == `BAR2 ||VGA_PIXEL_Y == `BAR3))
				next_state 			= STATE_CALC;
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
		end
	endcase
end

always @(*) begin
	case (current_state) 
		STATE_IDLE: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
		STATE_CHECK_BAR1: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
		STATE_CHECK_BAR2: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
		STATE_CHECK_BAR3: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
		STATE_CALC: begin
			redTotal					= red1 + red2 + red3;
			blueTotal				= blue1+blue2+blue3;
			if (blueTotal > redTotal) begin
				out 						= 1'b1;
				done						= 1'b1;
			end
		end
		STATE_WAIT: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
		default: begin
			out 						= 1'b0;
			done						= 1'b0;
		end
	endcase
end

always @(posedge CLK) begin
	case (current_state)
		STATE_IDLE: begin
			red1						= 0;
			blue1						= 0;
			red2						= 0;
			blue2						= 0;
			red3						= 0;
			blue3						= 0;
		end
		
		STATE_CHECK_BAR1: begin
			red1						= red1 + red;
			blue1						= blue1+ blue;
		end
		STATE_CHECK_BAR2: begin
			red2						= red2 + red;
			blue2						= blue2+ blue;
		end
		STATE_CHECK_BAR3: begin
			red3						= red3 + red;
			blue3						= blue3+ blue;
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


