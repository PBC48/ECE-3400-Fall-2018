#include "mux.h"


void setup(){
    mux_init();
    Serial.begin(9600);
}

void loop(){
    delay(500);
    set_mux_select(3);
    LEFTWALL = analogRead(MUX_OUT);
    set_mux_select(2);
    FRONTWALL = analogRead(MUX_OUT);
    Serial.print(F("FRONTWALL: "));
    Serial.print(FRONTWALL);

    Serial.print(F("-----LEFTWALL: "));
    Serial.print(LEFTWALL);
    set_mux_select(4);
    RIGHTWALL = analogRead(MUX_OUT);
    Serial.print(F("-----RIGHTWALL: "));
    Serial.println(RIGHTWALL);
}