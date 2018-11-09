
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



reg [7:0]    OUT;
reg          count_bit;
reg 			 reg_valid;
reg 			 fsm;

assign PIXEL = OUT;
assign SAMP_RDY = reg_valid;

localparam IDLE = 0;
localparam READ = 1;

always @ (posedge CLK) begin
	case (fsm)
		IDLE: begin
			fsm <= !RES && HREF ? READ : IDLE;
			count_bit <= 1'b0;
			reg_valid <= 1'b0;
			OUT 		 <= 8'd0;
 		end
		READ: begin
			fsm 					= (VSYNC || RES) ? IDLE : READ;
			
			if(HREF) begin
				count_bit 		= ~count_bit;
				if(count_bit) begin
					reg_valid		<= 1'b0;
					OUT[7:2]		 	<= {D[7:5], D[2:0]};
				end
				else begin 
					reg_valid 		<= 1'b1;
					OUT[1:0]			<= D[4:3];
				end
			end
			else begin
				count_bit			<= 1'b0;
				reg_valid 			<= 1'b0;
				OUT					<= 8'd7;
			end
		end
	endcase

end
   
endmodule
