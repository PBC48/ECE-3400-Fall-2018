#include <Wire.h>

#define OV7670_I2C_ADDRESS 0x21 // For Write/*TODO: write this in hex (eg. 0xAB) */
#define T1 4  //GPIO 33 treasure[0]
//#define T2 4  //GPIO 32 treasure[1] //mux A5
//#define C1  5  //GPIO 31 color ; blue or red //mux A6
//#define C2  6  //GPIO 30 color ; blue or red //mux A7
#define RED 2
#define BLUE 1
#define NONE 0
#define SQUARE 2
#define TRIANGLE 1
#define DIAMOND 3
 


void init_com(){
  pinMode(T1,INPUT);
  //pinMode(T2,INPUT);
  //pinMode(C1,INPUT);
  //pinMode(C2,INPUT);
}


///////// Function Definition //////////////


uint8_t treasure_decode(int t1, int t2, int c1, int c2){
  uint8_t treasure1,treasure2,color1,color2;
  uint8_t treasure;
  uint8_t color  ;
  int redCount = 0, blueCount = 0,
  diaCount = 0, triCount = 0, sqCount = 0, nonCount = 0, nonTreasure = 0;
  for (int i = 0; i<100; i++){
    treasure1 = digitalRead(T1);
    treasure2 = digitalRead(T2);
    color1     = digitalRead(C1);
    color2     = digitalRead(C2);
    treasure  = (treasure2 << 1)|treasure1;
    color   =   (color2<<1)|(color1);
    if(color==BLUE) blueCount += 1;
    else if(color==RED)   redCount += 1;
    else nonCount += 1;
    if (treasure == SQUARE) sqCount +=1;
    else if( treasure == TRIANGLE) triCount += 1;
    else if( treasure == DIAMOND)  diaCount += 1;
    else nonTreasure += 1;
    delay(10);
  }
//   Serial.print("T1: "); Serial.print(treasure1);
//   Serial.print(" T2: ");Serial.print(treasure2);
//   Serial.print(" C1: ");Serial.print(color1);
//   Serial.print(" C2: ");Serial.print(color2);
  
  Serial.println(F(""));
  Serial.print(F("---COLOR: "));
  if( (blueCount>nonCount || redCount > nonCount) && blueCount>redCount) {
    Serial.print(F("BLUE"));
    color = BLUE;
  }
  else if((blueCount>nonCount || redCount > nonCount) && redCount>blueCount)  {  
    Serial.print(F("RED"));
    color = RED;
  }
  else{
       Serial.print(F("NONE"));
       color = NONE;
  }             
 
  Serial.print( F(" Treasure: "));
  if( ((sqCount>nonTreasure)||(triCount>nonTreasure)||(diaCount>nonTreasure)) 
  && (sqCount > triCount) && (sqCount > diaCount)){
       Serial.print(F("SQUARE"));
       treasure = SQUARE;
  }
     
  else if( ((sqCount>nonTreasure)||(triCount>nonTreasure)||(diaCount>nonTreasure)) 
  && (sqCount < triCount) && (triCount > diaCount)){
      Serial.print(F("TRIANGLE"));
      treasure = TRIANGLE;
  }   
  else if( ((sqCount>nonTreasure)||(triCount>nonTreasure)||(diaCount>nonTreasure)) 
  && (diaCount > triCount) && (triCount < diaCount)){
      Serial.print(F("DIAMOND"));
      treasure = DIAMOND;
  }
  else{
      Serial.print(F("NONE"));
      treasure= NONE;
  }

  // 0: NONE, 
  // 1: tr, 2: sr, 3: dr
  // 4: tb, 5: sb, 6: db 
  if (treasure == NONE && color == NONE){
      return 0;
  }else if(color==RED){
      if(treasure==TRIANGLE){
          return 1;
      }
      else if(treasure==SQUARE){
          return 2;
      }
      else {
          return 3;
      }
  }else if(color == BLUE){
      if(treasure==TRIANGLE){
          return 4;
      }
      else if(treasure==SQUARE){
          return 5;
      }
      else {
          return 6;
      }
  }else{
      return 0;
  }

  Serial.println("");

  Serial.print(F("red="));Serial.print(redCount);Serial.print(F(" blue="));Serial.print(blueCount);
  Serial.print(F(" nonCount="));Serial.print(nonCount);
  Serial.print(F(" sq="));Serial.print(sqCount);Serial.print(F(" tri="));Serial.print(triCount);
  Serial.print(F(" dia="));Serial.print(diaCount);Serial.print(F(" nonTreasure="));Serial.print(nonTreasure);
  Serial.println("");
    
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

String OV7670_write_register(int reg_address, byte data){
  return OV7670_write(reg_address, &data, 1);
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

///////// Main Program //////////////
void treasure_setup() {
  Wire.begin();
  Serial.println(F("treasure_setup"));
  Serial.println(OV7670_write_register(0x12,0x80)); //COM7 reset all regs
//  Serial.println(OV7670_write_register(0x12,0x0E)); //COM7 enable color bar test and QCIF
//  Serial.println(OV7670_write_register(0x42,0x08)); //COM17 enables color bar DSP
  
  Serial.println(OV7670_write_register(0x12,0x0c)); //COM7 QCIF 176x144 resolution  
  Serial.println(OV7670_write_register(0x0c,0x08)); //COM3 Enable Scaling
  Serial.println(OV7670_write_register(0x14,0x61)); //COM9 reduce noise
  Serial.println(OV7670_write_register(0x40,0xD0)); //COM15 select output range and RGB565
  Serial.println(OV7670_write_register(0x11,0xC0)); //CLKRC two clk both same speed, use external clk
  Serial.println(OV7670_write_register(0x1E,0x30)); //MVFP flip/mirror | was at 0x30
  Serial.println("Wrote registers");
  //set_color_matrix();
  //read_key_registers();
  init_com();
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
