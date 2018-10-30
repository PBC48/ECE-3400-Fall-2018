#include <Wire.h>

#define OV7670_I2C_WRITE_ADDRESS 0x21 // For Write/*TODO: write this in hex (eg. 0xAB) */
#define OV7670_I2C_READ_ADDRESS 0x21 // For Read/*TODO: write this in hex (eg. 0xAB) */


String OV7670_write(int start, const byte *pData, int size){
    int n,error;
    Wire.beginTransmission(OV7670_I2C_WRITE_ADDRESS);
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
  Wire.beginTransmission(OV7670_I2C_READ_ADDRESS);
  Wire.write(register_address);
  Wire.endTransmission();
  Wire.requestFrom(OV7670_I2C_READ_ADDRESS,1);
  Serial.println("b4 while");
  while(Wire.available()<1);
  Serial.println("after while");
  data = Wire.read();
  return data;
}


///////// Main Program //////////////
void setup() {
  Wire.begin();
  Serial.begin(9600);
  
  // TODO: READ KEY REGISTERS
  
  delay(100);
  Serial.println("Starting");
  Init_OV7670();
  // TODO: WRITE KEY REGISTERS
//   OV7670_write_register(0xF2,0x01); //RESET
//   OV7670_write_register(0x3E,0x08); //COM14
//   OV7670_write_register(0x11,0x40); //CLKRC
//   OV7670_write_register(0x12,(0x0c | 0x02)); //COM7
//   OV7670_write_register(0x40,0xf0); //COM15
//   OV7670_write_register(0x42,0x08); //COM17
//   OV7670_write_register(0x1E,0x30); //MVFP
  Serial.println("Wrote registers");
  set_color_matrix();
  read_key_registers();
  Serial.println("finished");
}

void loop(){
 }


///////// Function Definition //////////////
void read_key_registers(){
  Serial.println("read key regs");

    //byte out1 = 
    ReadOV7670(0x4f);
    
    //Serial.println(out1);
    // byte out2 = read_register_value(0x3e);
    // Serial.println(out2);
    // byte out3 = read_register_value(0x11);
    //  Serial.println(out3);
    // byte out4 = read_register_value(0x12);
    // Serial.println(out4);

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

void Init_OV7670(){
  //Reset All Register Values
  WriteOV7670(0x12,0x80);
  delay(100);
  WriteOV7670(0x3A, 0x04); //TSLB
 
  WriteOV7670(0x13, 0xC0); //COM8
  WriteOV7670(0x00, 0x00); //GAIN
  WriteOV7670(0x10, 0x00); //AECH
  WriteOV7670(0x0D, 0x40); //COM4
  WriteOV7670(0x14, 0x18); //COM9
  WriteOV7670(0x24, 0x95); //AEW
  WriteOV7670(0x25, 0x33); //AEB
  WriteOV7670(0x13, 0xC5); //COM8
  WriteOV7670(0x6A, 0x40); //GGAIN
  WriteOV7670(0x01, 0x40); //BLUE
  WriteOV7670(0x02, 0x60); //RED
  WriteOV7670(0x13, 0xC7); //COM8
  WriteOV7670(0x41, 0x08); //COM16
  WriteOV7670(0x15, 0x20); //COM10 - PCLK does not toggle on HBLANK
  }
void WriteOV7670(byte regID, byte regVal){
  // Slave 7-bit address is 0x21.
  // R/W bit set automatically by Wire functions
  // dont write 0x42 or 0x43 for slave address
  Wire.beginTransmission(0x21);
  // Reset all register values
  Wire.write(regID);
  Wire.write(regVal);
  Wire.endTransmission();
  delay(1);
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