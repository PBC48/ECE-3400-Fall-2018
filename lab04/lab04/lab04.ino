#include <Wire.h>

#define OV7670_I2C_ADDRESS 0x21 // For Write/*TODO: write this in hex (eg. 0xAB) */
#define T1 3  //GPIO 33 treasure[0]
#define T2 4  //GPIO 32 treasure[1]
#define C1  5  //GPIO 31 color ; blue or red
#define C2  6  //GPIO 30 color ; blue or red
#define RED 2
#define BLUE 1
#define NONE 0
#define SQUARE 2
#define TRIANGLE 1
#define DIAMOND 3
 



///////// Main Program //////////////
void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // TODO: READ KEY REGISTERS
  Serial.println("Before writing Key Registers");
  read_key_registers();
  delay(100);
  Serial.println("Writing Key Registers");
//  Init_OV7670();
  Serial.println(OV7670_write_register(0x12,0x80)); //COM7 reset all regs
//  Serial.println(OV7670_write_register(0x12,0x0E)); //COM7 enable color bar test and QCIF
//  Serial.println(OV7670_write_register(0x42,0x08)); //COM17 enables color bar DSP
  
  Serial.println(OV7670_write_register(0x12,0x0c)); //COM7 QCIF 176x144 resolution  
  Serial.println(OV7670_write_register(0x0c,0x08)); //COM3 Enable Scaling
  Serial.println(OV7670_write_register(0x14,0x61)); //COM9 reduce noise
  Serial.println(OV7670_write_register(0x40,0xD0)); //COM15 select output range and RGB565
  Serial.println(OV7670_write_register(0x11,0xC0)); //CLKRC two clk both same speed, use external clk
  Serial.println(OV7670_write_register(0x1E,0x30)); //MVFP flip/mirror | was at 0x30
//  Serial.println(OV7670_write_register(0x3B,0x80)); //night mode
//   Serial.println(OV7670_write_register(0x69,0xff)); //fixed gain control
  
  //Serial.println(OV7670_write_register(0x41,0x07)); //AWB Gain Enable
  
//  Serial.println(OV7670_write_register(0x02,0x00)); //Gain control red
//  Serial.println(OV7670_write_register(0x01,0x00)); //Gain control blue
  Serial.println("Wrote registers");
  set_color_matrix();
  read_key_registers();
  Serial.println("finished");
  init_com();
}



void loop(){
    decoder();
    delay(1000);
 }
///////// Function Definition //////////////

void init_com(){
  pinMode(T1,INPUT);
  pinMode(T2,INPUT);
  pinMode(C1,INPUT);
  pinMode(C2,INPUT);
}

uint8_t decoder(){
  uint8_t treasure1 = digitalRead(T1);
  uint8_t treasure2 = digitalRead(T2);
  uint16_t color1     = digitalRead(C1);
  uint16_t color2     = digitalRead(C2);
  Serial.print("T1: ");Serial.print(treasure1);
  Serial.print(" T2: ");Serial.print(treasure2);
  Serial.print(" C1: ");Serial.print(color1);
  Serial.print(" C2: ");Serial.print(color2);
  
  uint8_t treasure  = (treasure2 << 1)|treasure1;
  uint8_t color   =   (color2<<1)|(color1);
  Serial.println("");
  Serial.print("---COLOR: ");
  if(color==BLUE) Serial.print("BLUE");
  else if(color==RED)    Serial.print("RED");
  else             Serial.print("NONE");
  Serial.print( " Treasure: ");
  if( treasure == SQUARE)
      Serial.print("SQUARE");
  else if( treasure == TRIANGLE)
      Serial.print("TRIANGLE");
  else if( treasure == DIAMOND)
      Serial.print("DIAMOND");
  else
      Serial.print("NONE");
  Serial.println("");
}

void read_key_registers(){
  Serial.println("read key regs");

    ReadOV7670(0x12);
    ReadOV7670(0x0c);
    ReadOV7670(0x14);
    ReadOV7670(0x40);
    ReadOV7670(0x42);
    ReadOV7670(0x11);
    ReadOV7670(0x1e);
}

String OV7670_write_register(int reg_address, byte data){
  return OV7670_write(reg_address, &data, 1);
 }

void set_color_matrix(){
    OV7670_write_register(0x4f, 0x80);
    OV7670_write_register(0x50, 0x80);
    OV7670_write_register(0x51, 0x00);
    OV7670_write_register(0x52, 0x22);
    OV7670_write_register(0x53, 0x5e);
    OV7670_write_register(0x54, 0x80);
    OV7670_write_register(0x56, 0x40);
    OV7670_write_register(0x58, 0x9e);
    OV7670_write_register(0x59, 0x88);
    OV7670_write_register(0x5a, 0x88);
    OV7670_write_register(0x5b, 0x44);
    OV7670_write_register(0x5c, 0x67);
    OV7670_write_register(0x5d, 0x49);
    OV7670_write_register(0x5e, 0x0e);
    OV7670_write_register(0x69, 0x00);
    OV7670_write_register(0x6a, 0x40);
    OV7670_write_register(0x6b, 0x0a);
    OV7670_write_register(0x6c, 0x0a);
    OV7670_write_register(0x6d, 0x55);
    OV7670_write_register(0x6e, 0x11);
    OV7670_write_register(0x6f, 0x9f);
    OV7670_write_register(0xb0, 0x84);
}
  void ReadOV7670(byte regID){
  // Reading from a register is done in two steps
  // Step 1: Write register address to the slave
  // from which data is to be read.
  Wire.beginTransmission(0x21); // 7-bit Slave address
  Wire.write(regID);  // reading from register
  Wire.endTransmission();
 
  // Step 2: Read 1 byte from Slave
  Wire.requestFrom(0x21, 1);
  Serial.print("Read request Status:");
  Serial.println(Wire.available());
  Serial.print(regID,HEX);
  Serial.print(":");
  Serial.println(Wire.read(),HEX);
  }

  
String OV7670_write(int start, const byte *pData, int size){
    int n,error;
    Wire.beginTransmission(OV7670_I2C_ADDRESS);
    n = Wire.write(start);
    if(n != 1){
      return "I2C ERROR WRITING START ADDRESS";   
    }
    n = Wire.write(pData, size);
    if(n != size){
      return "I2C ERROR WRITING DATA";
    }
    error = Wire.endTransmission(true);
    if(error != 0){
      return String(error);
    }
    return "no errors :)";
 }

byte read_register_value(int register_address){
  byte data = 0;
  Wire.beginTransmission(OV7670_I2C_ADDRESS);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_ADDRESS,1);
  Serial.println("b4 while");
  while(Wire.available()<1);
  Serial.println("after while");
  data = Wire.read();
  return data;
}
