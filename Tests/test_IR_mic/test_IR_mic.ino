#include "fft_lib.h"
#include "mux.h"

void setup(){
    Serial.begin(9600);
    mux_init();
}


void loop(){
  set_mux_select(0);
    calculate_FFT(MIC);
    Serial.print(F("AUDIO SUM: "));
    Serial.print(sum);
    set_mux_select(1);
    calculate_FFT(IR);
    Serial.print("--------");
    Serial.print(F("IR SUM: ")); Serial.println(sum);
}
