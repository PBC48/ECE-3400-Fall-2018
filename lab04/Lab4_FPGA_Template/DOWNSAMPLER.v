
module DOWNSAMPLER (
	RES,
	CLK,
	D,
	HREF,
	PIXEL,
	valid
	//XADDR,
	//YADDR
);
/// I/O ///

input        RES;
input        CLK;
input  [7:0] D;
input 		 HREF;
output [7:0] PIXEL;
output       valid;


reg [7:0]    OUT;
reg [7:0]    inOUT;
reg          count_bit;
reg 			 reg_valid;

assign PIXEL = OUT;
assign valid = reg_valid;

always @ (negedge CLK) begin
	if(RES) begin
		OUT 	 <= 8'b0;
		count_bit <= 0;
        reg_valid <= 1'b0;
	end else if (HREF) begin
		if (count_bit) begin
			OUT <= inOUT || {{5'b0},D[4:2]};
			count_bit <= 1'b0;
//			
            reg_valid <= 1'b1;
		end else begin
			inOUT <= {D[7:5], D[2:1], {3'b0}};
			count_bit <= 1'b1;
         reg_valid <= 1'b0;
		end
    end
    else begin
        reg_valid <= 1'b0;
    end
end
   
endmodule
