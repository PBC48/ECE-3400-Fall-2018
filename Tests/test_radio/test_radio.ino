#include "radio.h"





void setup(){
    Serial.begin(9600);
    radio_init(role_ping_out);
}


void loop(){
    while (!radio_transmit);
    delay(1000);
}