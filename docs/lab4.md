# Lab 4: FPGA and Color Detection

## Objectives


## Introduction

## FPGA

### Setup

### Camera FPGA Communication

#### Polling from camera

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab04/vgatiming.PNG" width="400"/>
    <font size="2">
    <figcaption> <b> Timing Diagram for the OV7670 camera </b>
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab04/cam_fsm.PNG" width="400"/>
    <font size="2">
    <figcaption> <b> FSM for getting data from camera </b>
    </figcaption>
    </font>
</figure>

|states|Function|
|---------|----------|
|IDLE|Start of device. We wait here for camera to start|
|NEW FRAME|Start of new frame where we reset the X and Y address|
|POLL|Enable to downsampler and poll from camera|
|UPDATE ROW|New row of pixels. Update Y address and reset X address|
|WAIT|Wait for start of next frame or row|

```vhdl
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
```

#### Downsampler

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab04/timingdiagram565.PNG" width="400"/>
    <font size="2">
    <figcaption> <b> Timing Diagram for Outputing RGB565 </b>
    </figcaption>
    </font>
</figure> 

|states|Function|
|---------|----------|
|IDLE|wait for start of reading|
|READ|start reading from camera|

``` vhdl
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
```
 
### Color Detection

#### Color Bar First attempt

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab04/IMG_1161.jpg" width="400"/>
    <font size="2">
    <figcaption> <b> First attempt at the color bar test </b>
    </figcaption>
    </font>
</figure>

<iframe width="560" height="315" src="https://www.youtube.com/embed/0lKN7Tkrx0Q" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

### Integrating


## Arduino


### Disabling Arduino's Internal Pullup resistor

### Writing Registers

```cpp
void setup() {
    Wire.begin();
    Serial.begin(9600);
    delay(100);
    Serial.println(OV7670_write_register(0x12,0x80)); //COM7 reset all regs
    //  Serial.println(OV7670_write_register(0x12,0x06)); //color bar test

    //  Serial.println(OV7670_write_register(0x12,0x0E)); //COM7 enable color bar test and QCIF
    Serial.println(OV7670_write_register(0x12,0x0c)); //COM7 QCIF 176x144 resolution

    
    Serial.println(OV7670_write_register(0x0c,0x08)); //COM3 Enable Scaling
    Serial.println(OV7670_write_register(0x14,0x11)); //COM9 reduce noise
    Serial.println(OV7670_write_register(0x40,0xD0)); //COM15 select output range and RGB565
    //  Serial.println(OV7670_write_register(0x42,0x08)); //COM17 enables color bar DSP
    Serial.println(OV7670_write_register(0x11,0xC0)); //CLKRC two clk both same speed, use external clk
    Serial.println(OV7670_write_register(0x1E,0x30)); //MVFP flip/mirror

    set_color_matrix();
    
}
```

|Registers|Name|Function|
|---------|----------|---|
|0x12|COM7|Reset regs; enable/disable color bar test; sets QCIF 176x144 resolution|
|0x0c|COM3|Enable scaling|
|0x14|COM9|Auto gain ceiling 2x; Freeze AGC/AEC|
|0x40|COM15|Max output range [00] to [FF]; Use RGB 565|
|0x42|COM17|DSP color bar enable|
|0x11|CLKRC|Use 24 MHz clk from FPGA; pclk can be as high as xclk|
|0x1E|MVFP|flip and mirror image|


### Communicating with FPGA

## Camera System Integration

## Conclusion
