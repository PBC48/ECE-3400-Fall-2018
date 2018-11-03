
module DOWNSAMPLER (
	input           RES,
	input           CLK,
	input  [7:0]    D,
	input           HREF,
	input           VSYNC,
    output [7:0]    PIXEL,
	output          SAMP_RDY
);
/// I/O ///



reg [7:0]    OUT = 0;
reg          count_bit;
reg 			 reg_valid;
reg 			 fsm;

assign PIXEL = OUT;
assign SAMP_RDY = reg_valid;

localparam IDLE = 0;
localparam READ = 1;

always @ (negedge CLK) begin
	case (fsm)
		IDLE: begin
 
            fsm <= !VSYNC && !RES ? READ : IDLE;
            count_bit <= 0;
            reg_valid <= 1'b0;

 		end
		READ: begin
			fsm <= VSYNC || RES ? IDLE : READ;
			reg_valid <= (HREF && count_bit) ? 1: 0;
			if(HREF) begin
				count_bit = ~count_bit;
				if(count_bit) OUT[7:3] = {D[7:5], D[2:1]};
				else OUT[2:0] = D[4:2];
			end
		end
	endcase
//		if(RES || !vsync) begin
//			OUT 	 <= 8'b1;
//			count_bit <= 0;
//			reg_valid <= 1'b0;
//		end else if (HREF) begin
//			if (count_bit) begin
//				OUT[2:0] <= {{5'b0},D[4:2]};
//				count_bit <= 1'b0;
//				reg_valid = 1'b1;
//			end else begin
//				OUT[7:3] <= {D[7:5], D[2:1]};
//				count_bit <= 1'b1;
//				reg_valid <= 1'b0;
//			end
//		 end
//		 else begin
//			  reg_valid <= 1'b0;
//		 end
	end
   
endmodule
