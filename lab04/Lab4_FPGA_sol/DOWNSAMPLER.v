
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
reg 			 fsm;

reg [14:0]   x;
reg [14:0]   y;

assign PIXEL = OUT;
assign SAMP_RDY = reg_valid;
assign x_out = x;
assign y_out = y;

localparam IDLE = 0;
localparam READ = 1;

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

/*
always @ (posedge CLK) begin
	case (fsm)
		IDLE: begin
			fsm <= (RES && !HREF) ? IDLE : READ;
			count_bit 				= 1'b0;
			reg_valid 				= 1'b0;
			OUT 		 				= 8'd0;
 		end		
		READ: begin
			fsm 					= (RES || VSYNC) ? IDLE : READ;
			if(HREF) begin
				count_bit 		= ~count_bit;
				if(!count_bit) begin
					reg_valid		<= 1'b1;
					OUT[7:2]		 	<= {D[7:5], D[2:0]};
				end
				else begin 
					reg_valid 		<= 1'b0;
					OUT[1:0]			<= D[4:3];
				end
			end
//			if(HREF) begin
//				count_bit 		= ~count_bit;
//				if(count_bit) begin
//					reg_valid		= 1'b1;
//					temp[7:0]		= D;
//					OUT			 	= {temp[15:13], temp[10:8],temp[4:3]};
//				end
//				else begin 
//					temp[15:8]		= D;
//					reg_valid 		= 1'b0;
//					OUT				= 8'd0;
//				end
//			end
			else begin
				count_bit			= 1'b0;
				reg_valid 			= 1'b0;
				OUT					= 8'd0;
			end
		end
	endcase

end*/
   
endmodule
