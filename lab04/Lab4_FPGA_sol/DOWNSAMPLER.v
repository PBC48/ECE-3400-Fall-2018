
module DOWNSAMPLER (
	input           RES,
	input           CLK,
	input  [7:0]    D,
	input           HREF,
	input           VSYNC,
   output [7:0]    PIXEL,
	output          SAMP_RDY,
	output [14:0]   x_out,
	output [14:0]	 y_out,
	input  [14:0]   x_in,
	input  [14:0]   y_in
);
/// I/O ///


reg [15:0]	 TEMP;
reg [7:0]    OUT;
reg          count_bit;
reg 			 reg_valid;

reg [14:0]   x;
reg [14:0]   y;

assign PIXEL = OUT;
assign SAMP_RDY = reg_valid;
assign x_out = x;
assign y_out = y;

reg 			last_vsync;
reg 			last_href;

always @ (posedge CLK) begin
	if (VSYNC && !last_vsync) begin
		x = 0;
		y = 0;
		count_bit = 0;
	end
	else if (!HREF && last_href) begin
		y = y + 1;
		x = 0;
		count_bit = 0;

	end
	else begin
		y = y;
		if(HREF) begin
			if (!count_bit) begin
				TEMP[7:0] = D;
				count_bit = 1'b1;
				reg_valid = 1'b0;
				x = x;
				
			end
			else begin
				TEMP[15:8] = D;
				OUT = {TEMP[15:13], TEMP[10:9], TEMP[4:2]}; //change based on RGB332 or RGB323
				reg_valid = 1'b1;
				count_bit = 1'b0;
				x = x +1;
			end
		end
		else begin 
			x = 0;
		end
	end
	last_vsync = VSYNC;
	last_href = HREF;
end

   
endmodule
