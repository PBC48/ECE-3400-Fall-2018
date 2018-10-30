`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144
`define NUM_BARS 3
`define BAR_HEIGHT 48

module IMAGE_PROCESSOR (
	PIXEL_IN,
	CLK,
	VGA_PIXEL_X,
	VGA_PIXEL_Y,
	VGA_VSYNC_NEG,
	RESULT
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

reg [9:0] pixel_count;
reg [9:0] line_count;
reg [15:0] top_r;
reg [15:0] top_b;
reg [15:0] mid_r;
reg [15:0] mid_b;
reg [15:0] bot_r;
reg [15:0] bot_b;
reg [8:0] result;

assign RESULT = result;
always @(posedge CLK) begin
  if (VGA_PIXEL_X == 0 & VGA_PIXEL_Y == 0) begin
		top_r = 0;
		top_b = 0;
		mid_r = 0;
		mid_b = 0;
		bot_r = 0;
		bot_b = 0;
  end else if (VGA_PIXEL_Y < 48) begin
		top_r = top_r + (PIXEL_IN >> 5);
		top_b = top_b + PIXEL_IN & 3'b111;
  end else if (VGA_PIXEL_Y < 96) begin
      mid_r = mid_r + (PIXEL_IN >> 5);
		mid_b = mid_b + PIXEL_IN & 3'b111;
  end else if (VGA_PIXEL_Y < 144 & VGA_PIXEL_X == 143) begin
      bot_r = bot_r + (PIXEL_IN >> 5);
		bot_b = bot_b + PIXEL_IN & 3'b111;
  end else begin
      if (top_r + mid_r + bot_r >= top_r + mid_r + bot_r) begin
		   if (top_r + mid_r + bot_r < 40000) begin
			result = 9'b000000000;
			end else if((mid_r > top_r + 2000) & (bot_r > mid_r + 2000)) begin
			result = 9'b000001000; //red triangle
			end else if ((mid_r > top_r + 2000) & (mid_r > bot_r + 2000)) begin
			result = 9'b000010000; //red rhombus
			end else
			result = 9'b000011000; // red square
			end
		else begin
			if (top_b + mid_b + bot_b < 40000) begin
			result = 9'b000000000;
			end else if((mid_b > top_b + 2000) & (bot_b > mid_b + 2000)) begin
			result = 9'b000101000; //blue triangle
			end else if ((mid_b > top_b + 2000) & (mid_b > bot_b + 2000)) begin
			result = 9'b000110000; //blue rhombus
			end else
			result = 9'b000111000; //blue square
			end
  end
end

endmodule