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
///////////////////////////PROVIDED CODE////////////////////////////////
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
wire [7:0]	    VGA_COLOR_IN;
wire [9:0]	    VGA_PIXEL_X;
wire [9:0]	    VGA_PIXEL_Y;
wire [7:0]	    MEM_OUTPUT;
wire			VGA_VSYNC_NEG;
wire			VGA_HSYNC_NEG;
reg			    VGA_READ_MEM_EN;
assign GPIO_0_D[5] = VGA_VSYNC_NEG;
assign VGA_RESET = ~KEY[0];
///// I/O for Img Proc /////
wire [8:0] RESULT;

/* WRITE ENABLE */
reg W_EN;
///////////////////////////END PROVIDED CODE///////////////////////


///////* INSTANTIATE YOUR PLL HERE *///////
ahhhPLL	ahhhPLL_inst (
	.inclk0 ( CLOCK_50 ),
	.c0 ( clk24_PLL ),
	.c1 ( clk25_PLL ),
	.c2 ( clk50_PLL )
	);


/////// DOWNSAMPLER  ///////
DOWNSAMPLER down(
	.RES(down_sample_reset),
	.CLK(pclk),
	.D(Data),
	.HREF(CAM_HREF),
 	.VSYNC(CAM_VSYNC),
	.PIXEL(down_sample_output),
    .SAMP_RDY(downsampler_rdy)
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
	.RESULT(RESULT),
	.RDY(image_proc_rdy)
);

/// CAMERA INPUTS/OUTPUTS //
wire       CAM_VSYNC;
wire       CAM_HREF;
assign Data = GPIO_1_D[33:26];
assign CAM_VSYNC = GPIO_1_D[25];
assign CAM_HREF = GPIO_1_D[24];

//assign clock to GPIO port
assign GPIO_0_D[0] = clk24_PLL; //Output clock for camera
assign pclk = GPIO_1_D[23];      //clk from camera

///////* CREATE ANY LOCAL WIRES YOU NEED FOR YOUR PLL *///////
wire        clk24_PLL;
wire        clk25_PLL;
wire        clk50_PLL;
wire        pclk;
wire        downsampler_rdy;
wire			image_proc_rdy;
wire [7:0]  down_sample_output;
reg         down_sample_reset;
reg  [2:0]  control_state;
reg  [2:0]  next_state;
wire [7:0]	Data;

//ALways start in IDLE in case camera begins to transmit when FPGA not ready
//Then we wait for next frame
localparam  STATE_IDLE          = 3'd0; //wait for camera to send start of frame
localparam  STATE_NEW_FRAME     = 3'd1; //new frame incoming
localparam  STATE_POLL	        = 3'd2; //get downsample data from camera, update x-addr
localparam  STATE_UPDATE_ROW    = 3'd3; //increase y-addr and reset x-addr for next row
localparam  STATE_WAIT          = 3'd4; //wait for next row

//state transition
always @(posedge pclk) begin
	 if (VGA_RESET)
        control_state = STATE_IDLE;
    else
        control_state = next_state;
end

//next state logic
always @(*) begin
	case (control_state)
        STATE_IDLE: begin
            next_state                  = CAM_VSYNC ? STATE_NEW_FRAME : STATE_IDLE;
        end
        STATE_NEW_FRAME: begin
            next_state                  = CAM_HREF ? STATE_POLL : STATE_NEW_FRAME;
        end
        STATE_POLL: begin
            next_state                  = CAM_HREF ? STATE_POLL : STATE_UPDATE_ROW;
        end
        
        STATE_UPDATE_ROW: begin
            next_state                  = STATE_WAIT;
        end
        STATE_WAIT: begin
            if (CAM_VSYNC) next_state       	= STATE_NEW_FRAME;
            else if (CAM_HREF) next_state   	= STATE_POLL;
            else next_state             		= STATE_WAIT;
        end
        default: begin
            next_state = STATE_IDLE;
        end
    endcase
end

// output logic
always @(*) begin
    case (control_state)
        STATE_IDLE: begin
            W_EN                <= 1'b0;
            down_sample_reset   <= 1'b1;
        end
        STATE_NEW_FRAME: begin
            down_sample_reset   <= 1'b0; //we need to poll as soon as HREF hits high
            W_EN                <= downsampler_rdy ? 1'b1 : 1'b0;
        end
        STATE_POLL: begin
            down_sample_reset   <= 1'b0;
				W_EN					  <= downsampler_rdy ? 1'b1 : 1'b0;
				
        end
        STATE_UPDATE_ROW: begin
            down_sample_reset   <= 1'b1;
            W_EN                <= 1'b0;
        end
        STATE_WAIT: begin
            down_sample_reset   <= 1'b1;
            W_EN                <= 1'b0;
        end
        default: begin
            W_EN                <= 1'b0;
            down_sample_reset   <= 1'b1;
        end
    endcase
end

// clked output logic
always @(posedge pclk) begin
	case (control_state)
        STATE_IDLE: begin
        end
        STATE_NEW_FRAME: begin
            X_ADDR              <= 15'b0;
            Y_ADDR              <= 15'b0;
         
        end
        STATE_POLL: begin
				
				//increments xaddr after downsampler finishes and writes to mem
				X_ADDR			 	<= downsampler_rdy ? X_ADDR + 1 : X_ADDR; 
				
        end

        STATE_UPDATE_ROW: begin
				Y_ADDR				 <= Y_ADDR + 1;
            X_ADDR             <= 15'b0;
        end
        STATE_WAIT: begin
            
        end
        default: begin
            
        end
    endcase
end
/*
always @ (posedge clk25_PLL) begin
	if (X_ADDR < `SCREEN_WIDTH/2 && Y_ADDR < `SCREEN_HEIGHT/2) begin
		pixel_data_RGB332 = 8'b000_111_11;
	end
	else begin
		pixel_data_RGB332 = RED | BLUE;
	end
end */
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