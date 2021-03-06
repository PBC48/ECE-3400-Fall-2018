`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144

///////* DON'T CHANGE THIS PART *///////
module DE0_NANO(
	CLOCK_50,
	GPIO_0_D,
	GPIO_1_D,
	KEY
);

//=======================================================
//  PARAMETER declarations
//=======================================================
localparam RED = 8'b111_00_00;
localparam GREEN = 8'b000_111_00;
localparam BLUE = 8'b000_000_11;

//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK - DON'T NEED TO CHANGE THIS //////////
input 		          		CLOCK_50;

//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
output 		    [33:0]		GPIO_0_D;
//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
input 		    [33:20]		GPIO_1_D;
input 		     [1:0]		KEY;

///// PIXEL DATA /////
reg [7:0]	pixel_data_RGB332 = RED | GREEN | BLUE;
///// READ/WRITE ADDRESS /////
reg [14:0] X_ADDR;
reg [14:0] Y_ADDR;
wire [14:0] WRITE_ADDRESS;
reg [14:0] READ_ADDRESS;

//assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);
assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);
///// VGA INPUTS/OUTPUTS /////
wire 			VGA_RESET;
wire [7:0]	VGA_COLOR_IN;
wire [9:0]	VGA_PIXEL_X;
wire [9:0]	VGA_PIXEL_Y;
wire [7:0]	MEM_OUTPUT;
wire			VGA_VSYNC_NEG;
wire			VGA_HSYNC_NEG;
reg			VGA_READ_MEM_EN;

/// CAMERA INPUTS/OUTPUTS //
wire [7:0] D;
wire       CAM_VSYNC;
wire       CAM_HREF;

assign Data = GPIO_1_D[33:26];
assign CAM_VSYNC = GPIO_1_D[25];
assign CAM_HREF = GPIO_1_D[24];

assign GPIO_0_D[5] = VGA_VSYNC_NEG;
assign VGA_RESET = ~KEY[0];
///// I/O for Img Proc /////
wire [8:0] RESULT;

/* WRITE ENABLE */
reg W_EN;

///////* CREATE ANY LOCAL WIRES YOU NEED FOR YOUR PLL *///////
wire clk24_PLL;
wire clk25_PLL;
wire clk50_PLL;
wire pclk;
wire pixel_data_val;
wire [7:0] down_sample_output;

assign pclk = GPIO_0_D[1];
///////* INSTANTIATE YOUR PLL HERE *///////
ahhhPLL	ahhhPLL_inst (
	.inclk0 ( CLOCK_50 ),
	.c0 ( clk24_PLL ),
	.c1 ( clk25_PLL ),
	.c2 ( clk50_PLL )
	);
//assign clock to GPIO port
assign GPIO_0_D[0] = clk24_PLL;

/////// DOWNSAMPLER  ///////
DOWNSAMPLER down(
	.RES(VGA_RESET),
	.CLK(pclk),
	.D(Data),
	.HREF(CAM_HREF),
//	.VSYNC(CAM_VSYNC),
	.PIXEL(down_sample_output),
    .valid(pixel_data_val)
//	.XADDR(X_ADDR),
//	.YADDR(Y_ADDR)
);
///////* M9K Module *///////
Dual_Port_RAM_M9K mem(
   .input_data(down_sample_output),
	.w_addr(WRITE_ADDRESS),
	.r_addr(READ_ADDRESS),
	.w_en(W_EN),
	.clk_W(CLOCK_50),
	.clk_R(clk25_PLL), // DO WE NEED TO READ SLOWER THAN WRITE??
	.output_data(MEM_OUTPUT)
);

///////* VGA Module *///////
VGA_DRIVER driver (
	.RESET(VGA_RESET),
	.CLOCK(clk25_PLL),
	.PIXEL_COLOR_IN(VGA_READ_MEM_EN ? MEM_OUTPUT : BLUE),
	.PIXEL_X(VGA_PIXEL_X),
	.PIXEL_Y(VGA_PIXEL_Y),
	.PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
   .H_SYNC_NEG(GPIO_0_D[7]),
   .V_SYNC_NEG(VGA_VSYNC_NEG)
);

///////* Image Processor *///////
IMAGE_PROCESSOR proc(
	.PIXEL_IN(MEM_OUTPUT),
	.CLK(clk25_PLL),
	.VGA_PIXEL_X(VGA_PIXEL_X),
	.VGA_PIXEL_Y(VGA_PIXEL_Y),
	.VGA_VSYNC_NEG(VGA_VSYNC_NEG),
	.RESULT(RESULT)
);
reg   new_row;
reg 	new_image;

always @ (negedge pclk) begin
	new_image <= CAM_VSYNC==1 ? 1 : 0;
	new_row   <= CAM_HREF ==1 ? 1 : 0;
end

always @ (posedge pclk)begin//(posedge clk24_PLL) begin
	if (VGA_RESET || new_image) begin
		Y_ADDR <= 10'b0;
		X_ADDR <= 10'b0;
	end	
	else if (Y_ADDR == `SCREEN_HEIGHT) begin
		Y_ADDR <= 10'b0;
		X_ADDR <= 10'b0;
   end 
	else if(X_ADDR < `SCREEN_WIDTH && pixel_data_val) begin
		X_ADDR <= X_ADDR + 1;
   end
	else if (Y_ADDR < `SCREEN_HEIGHT && new_row) begin
	   X_ADDR <= 10'b0;
		Y_ADDR <= Y_ADDR + 1;
	end
	
	if (!VGA_RESET && pixel_data_val) begin
		W_EN <= 1'b1;
	end
	else begin
		W_EN <= 1'b0;
	end
end

always @ (posedge clk25_PLL) begin
	if (X_ADDR < `SCREEN_WIDTH/2 && Y_ADDR < `SCREEN_HEIGHT/2) begin
		pixel_data_RGB332 = 8'b000_111_11;
	end
	else begin
		pixel_data_RGB332 = RED | BLUE;
	end
end
///////* Update Read Address *///////
always @ (VGA_PIXEL_X, VGA_PIXEL_Y) begin
		READ_ADDRESS = (VGA_PIXEL_X + VGA_PIXEL_Y*`SCREEN_WIDTH);
		if(VGA_PIXEL_X>(`SCREEN_WIDTH-1) || VGA_PIXEL_Y>(`SCREEN_HEIGHT-1))begin
				VGA_READ_MEM_EN = 1'b0;
		end
		else begin
				VGA_READ_MEM_EN = 1'b1;
		end
end

	
endmodule 

`define SCREEN_WIDTH 176
`define SCREEN_HEIGHT 144

///////* DON'T CHANGE THIS PART *///////
module DE0_NANO(
	CLOCK_50,
	GPIO_0_D,
	GPIO_1_D,
	KEY
);

//=======================================================
//  PARAMETER declarations
//=======================================================
localparam RED = 8'b111_00_00;
localparam GREEN = 8'b000_111_00;
localparam BLUE = 8'b000_000_11;

//=======================================================
//  PORT declarations
//=======================================================

//////////// CLOCK - DON'T NEED TO CHANGE THIS //////////
input 		          		CLOCK_50;

//////////// GPIO_0, GPIO_0 connect to GPIO Default //////////
output 		    [33:0]		GPIO_0_D;
//////////// GPIO_0, GPIO_1 connect to GPIO Default //////////
input 		    [33:20]		GPIO_1_D;
input 		     [1:0]		KEY;

///// PIXEL DATA /////
reg [7:0]	pixel_data_RGB332 = RED | GREEN | BLUE;
///// READ/WRITE ADDRESS /////
reg [14:0] X_ADDR;
reg [14:0] Y_ADDR;
wire [14:0] WRITE_ADDRESS;
reg [14:0] READ_ADDRESS;

//assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);
assign WRITE_ADDRESS = X_ADDR + Y_ADDR*(`SCREEN_WIDTH);
///// VGA INPUTS/OUTPUTS /////
wire 			VGA_RESET;
wire [7:0]	VGA_COLOR_IN;
wire [9:0]	VGA_PIXEL_X;
wire [9:0]	VGA_PIXEL_Y;
wire [7:0]	MEM_OUTPUT;
wire			VGA_VSYNC_NEG;
wire			VGA_HSYNC_NEG;
reg			VGA_READ_MEM_EN;

/// CAMERA INPUTS/OUTPUTS //
wire [7:0] D;
wire       CAM_VSYNC;
wire       CAM_HREF;

assign Data = GPIO_1_D[33:26];
assign CAM_VSYNC = GPIO_1_D[25];
assign CAM_HREF = GPIO_1_D[24];

assign GPIO_0_D[5] = VGA_VSYNC_NEG;
assign VGA_RESET = ~KEY[0];
///// I/O for Img Proc /////
wire [8:0] RESULT;

/* WRITE ENABLE */
reg W_EN;

///////* CREATE ANY LOCAL WIRES YOU NEED FOR YOUR PLL *///////
wire clk24_PLL;
wire clk25_PLL;
wire clk50_PLL;
wire pclk;
wire pixel_data_val;
wire [7:0] down_sample_output;


///////* INSTANTIATE YOUR PLL HERE *///////
ahhhPLL	ahhhPLL_inst (
	.inclk0 ( CLOCK_50 ),
	.c0 ( clk24_PLL ),
	.c1 ( clk25_PLL ),
	.c2 ( clk50_PLL )
	);
//assign clock to GPIO port
assign GPIO_0_D[0] = clk24_PLL;
assign pclk = GPIO_0_D[1]; 


/////// DOWNSAMPLER  ///////
DOWNSAMPLER down(
	.RES(VGA_RESET),
	.CLK(pclk),
	.D(Data),
	.HREF(CAM_HREF),
 	.vsync(CAM_VSYNC),
	.PIXEL(down_sample_output),
    .valid(pixel_data_val)
//	.XADDR(X_ADDR),
//	.YADDR(Y_ADDR)
);
///////* M9K Module *///////
Dual_Port_RAM_M9K mem(
   .input_data(down_sample_output),
	.w_addr(WRITE_ADDRESS),
	.r_addr(READ_ADDRESS),
	.w_en(W_EN),
	.clk_W(CLOCK_50),
	.clk_R(clk25_PLL), // DO WE NEED TO READ SLOWER THAN WRITE??
	.output_data(MEM_OUTPUT)
);

///////* VGA Module *///////
VGA_DRIVER driver (
	.RESET(VGA_RESET),
	.CLOCK(clk25_PLL),
	.PIXEL_COLOR_IN(VGA_READ_MEM_EN ? MEM_OUTPUT : BLUE),
	.PIXEL_X(VGA_PIXEL_X),
	.PIXEL_Y(VGA_PIXEL_Y),
	.PIXEL_COLOR_OUT({GPIO_0_D[9],GPIO_0_D[11],GPIO_0_D[13],GPIO_0_D[15],GPIO_0_D[17],GPIO_0_D[19],GPIO_0_D[21],GPIO_0_D[23]}),
   .H_SYNC_NEG(GPIO_0_D[7]),
   .V_SYNC_NEG(VGA_VSYNC_NEG)
);

///////* Image Processor *///////
IMAGE_PROCESSOR proc(
	.PIXEL_IN(MEM_OUTPUT),
	.CLK(clk25_PLL),
	.VGA_PIXEL_X(VGA_PIXEL_X),
	.VGA_PIXEL_Y(VGA_PIXEL_Y),
	.VGA_VSYNC_NEG(VGA_VSYNC_NEG),
	.RESULT(RESULT)
);
reg   new_row;
reg   new_image;

reg prev_vsync;
reg curr_vsync;
reg prev_href;
reg curr_href;

reg [1:0] 

always @(posedge pclk) begin
	if(VGA_RESET) begin
		prev_vsync = 0;
		curr_vsync = 0;
		prev_href  = 0;
		curr_href  = 0;
	end
	else begin
		prev_vsync = curr_vsync;
		curr_vsync = CAM_VSYNC;
		prev_href  = curr_href;
		curr_href  = CAM_HREF;
	end
	
	if (prev_vsync == 0 && curr_vsync==1) begin 
		new_image = 1;
	end
	else begin
		new_image = 0;
	end
	
	if (prev_href==0 && curr_href==1) begin
		new_row = 1;
	end
	else begin
		new_row = 0;
	end
end



always @ (posedge pclk)begin//(posedge clk24_PLL) begin
	if (VGA_RESET || new_image) begin
		Y_ADDR <= 10'b0;
		X_ADDR <= 10'b0;
	end	
	else if (Y_ADDR == `SCREEN_HEIGHT) begin
		Y_ADDR <= 10'b0;
		X_ADDR <= 10'b0;
   end 
	else if(X_ADDR < `SCREEN_WIDTH && pixel_data_val) begin
		X_ADDR <= X_ADDR + 1;
   end
	else if (Y_ADDR < `SCREEN_HEIGHT && new_row) begin
	   X_ADDR <= 10'b0;
		Y_ADDR <= Y_ADDR + 1;
	end
	
	if (!VGA_RESET && pixel_data_val) begin
		W_EN <= 1'b1;
	end
	else begin
		W_EN <= 1'b0;
	end
end

/*always @ (posedge clk25_PLL) begin
	if (X_ADDR < `SCREEN_WIDTH/2 && Y_ADDR < `SCREEN_HEIGHT/2) begin
		pixel_data_RGB332 = 8'b000_111_11;
	end
	else begin
		pixel_data_RGB332 = RED | BLUE;
	end
end*/
///////* Update Read Address *///////
always @ (VGA_PIXEL_X, VGA_PIXEL_Y) begin
		READ_ADDRESS = (VGA_PIXEL_X + VGA_PIXEL_Y*`SCREEN_WIDTH);
		if(VGA_PIXEL_X>(`SCREEN_WIDTH-1) || VGA_PIXEL_Y>(`SCREEN_HEIGHT-1))begin
				VGA_READ_MEM_EN = 1'b0;
		end
		else begin
				VGA_READ_MEM_EN = 1'b1;
		end
end

	
endmodule 