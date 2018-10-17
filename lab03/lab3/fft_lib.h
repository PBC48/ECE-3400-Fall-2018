/**
 * Function for microphone and IR sensor. Both use FFT library
*/


#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft
#define IR_BIN 40
#define MIC_BIN 18

#include <FFT.h> // include the library

enum device {
    MIC,
    IR
};

// uint8_t AVERAGE;
// uint8_t FBIN;
// uint8_t ADC_RESTART;

//uint32_t IR_target_freq = 6080;
//uint32_t smp_freq = 19235*2;

uint32_t sum = 0;


void turn_led(int &i){
  int pin = 7;
  pinMode(pin,OUTPUT);
  digitalWrite(pin,i?HIGH:LOW);
  
}

void calculate_FFT(device d){

    byte TIMSK0_temp = TIMSK0;
    byte ADCSRA_temp = ADCSRA;
    byte ADMUX_temp = ADMUX;
    byte DIDR0_temp = DIDR0;

    if(d==IR){
        TIMSK0 = 0; // turn off timer0 for lower jitter
        ADCSRA = 0xe7; // set the adc to free running mode
        ADMUX = 0x41; // use adc1
        DIDR0 = 0x02; // turn off the digital input for adc1
    }else{
        TIMSK0 = 0; // turn off timer0 for lower jitter
        ADCSRA = 0xe7; // set the adc to free running mode
        ADMUX = 0x40; // use adc0
        DIDR0 = 0x01; // turn off the digital input for adc0
    }
    sum = 0;
  
    uint8_t index = d == MIC ? MIC_BIN : IR_BIN;
    uint8_t average = d == MIC ? 15 : 5;
    for(int j=0;j<average;j++) { // reduces jitter
        cli();  // UDRE interrupt slows this way down on arduino1.0
        for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
            while(!(ADCSRA & 0x10)); // wait for adc to be ready
            ADCSRA = d == MIC ? 0xf7 : 0xf5; // restart adc
            byte m = ADCL; // fetch adc data
            byte j = ADCH;
            int k = (j << 8) | m; // form into an int
            k -= 0x0200; // form into a signed int
            k <<= 6; // form into a 16b signed int
            fft_input[i] = k; // put real data into even bins
            fft_input[i+1] = 0; // set odd bins to 0
        }
        fft_window(); // window the data for better frequency response
        fft_reorder(); // reorder the data before doing the fft
        fft_run(); // process the data in the fft
        fft_mag_log(); // take the output of the fft
        sei(); 
        sum+=fft_log_out[index];  
    }
    sum /= average;

    TIMSK0 = TIMSK0_temp;
    ADCSRA = ADCSRA_temp;
    ADMUX = ADMUX_temp;
    DIDR0 = DIDR0_temp;
        
}

uint16_t OverAnalogRead(uint8_t pin){
  ADMUX = 0x40|pin; // use adc pin
  while(!(ADCSRA & 0x10)); // wait for adc to be ready
  ADCSRA = 0xf5; // restart adc
  byte m = ADCL; // fetch adc data
  byte j = ADCH;
  uint16_t k = (j << 8) | m; // form into an int
  k -= 0x0200; // form into a signed int
  k <<= 6; // form into a 16b signed int
  return k;
}

     
