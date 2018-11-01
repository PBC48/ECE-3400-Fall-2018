`define SCREEN_WIDTH 176

module DOWNSAMPLER (
	RES,
	CLK,
	D,
	HREF,
	VSYNC,
	RDY,
	PIXEL
);
/// I/O ///

input        RESET;
input        CLOCK;
input  [7:0] D;
input 	 	 HREF;
input        VSYNC;
output       RDY;
output [7:0] PIXEL;

reg [7:0] OUT;
reg       count_bit;
reg [7:0]	 count_h;
reg [7:0]    count_v;

always @ (negedge CLK) begin
	if(RESET) begin
		OUT <= 8'b0;
		RDY <= 1'b0;
		count_bit<= 0;
		count_h <= 0;
		count_v <= 0;
	end else if (HREF) begin
		if (count_bit) begin
			PIXEL <= PIXEL && {{3'b1}}
		end else 
			PIXEL <= {{D[]}}
		
		end
   end else if ()