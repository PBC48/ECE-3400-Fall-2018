
module DOWNSAMPLER (
	RES,
	CLK,
	D,
	HREF,
	PIXEL,
	RDY
	//XADDR,
	//YADDR
);
/// I/O ///

input        RES;
input        CLK;
input  [7:0] D;
input 		 HREF;
output [7:0] PIXEL;


reg [7:0]    OUT;
reg [7:0]    inOUT;
reg          count_bit;


//assign PIXEL = OUT;

always @ (negedge CLK) begin
	if(RES) begin
		PIXEL 	 <= 8'b0;
		count_bit <= 0;
	end else if (HREF) begin
		if (count_bit) begin
			OUT <= inOUT || {{5'b0},D[4:2]};
//			count_h <= count_h + 1;
			count_bit <= 1'b0;
//			if (count_h != `SCREEN_WIDTH) begin
//				count_h <= count_h + 1;
//			end else begin
//				count_h <= 14'b0;
//				count_v <= count_v + 1;
//			end
		end else begin
			inOUT <= {D[7:5], D[2:1], {3'b0}};
			count_bit <= 1'b1;
		end
	end
end
   
endmodule
