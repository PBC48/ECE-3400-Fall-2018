#include "fft_lib.h"
#include "mux.h"

void setup(){
    Serial.begin(9600);
    mux_init();
}


void loop(){
    calculate_FFT(MIC);
    Serial.print(F("AUDIO SUM: "));
    Serial.print(sum);
    set_mux_select(1);
    calculate_FFT(IR);
    Serial.println("--------IN IR_DECT");
    Serial.print(F("IR SUM: ")); Serial.println(sum);
}