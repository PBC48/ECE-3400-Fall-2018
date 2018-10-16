#include <SPI.h>
#include "Mirf.h"
#include <nRF24L01.h>
#include "MirfHardwareSpiDriver.h"

int value;

void setup()
{
  Serial.begin(9600);
  Mirf.spi = &MirfHardwareSpi;
  Mirf.init();
  // Set own address - unique 5 character string
  Mirf.setRADDR((byte *)"FGHIJ");
  Mirf.payload = sizeof(value);
  Mirf.channel = 90;
  Mirf.config(); 
  Serial.println("Listening...");
}

void loop()
{
  if(Mirf.dataReady()) {
    Mirf.getData((byte *) &value);
    Serial.print("Got data: ");
    Serial.println(value);
  }
}
