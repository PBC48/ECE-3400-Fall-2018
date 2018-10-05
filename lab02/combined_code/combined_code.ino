#define LOG_OUT 1 // use the log output function
#define FFT_N 256 // set to 256 point fft


#include <FFT.h> // include the library
#include "IR_init.h"

enum states{
    START,
    AUDIO_FFT,
    AUDIO_PROC,
    IR_FFT,
    IR_PROC
};

uint8_t state;

void turn_led(int i){
  int pin = LED_BUILTIN;
  pinMode(pin,OUTPUT);
  digitalWrite(pin,i?HIGH:LOW);
}


void setup() {
  Serial.begin(115200); // use the serial port
  ir_init();
}

void loop() {
  switch (state){   
        case START:
            Serial.println("starting");
            state = AUDIO_FFT;
            break;

        case AUDIO_FFT:
            Serial.println("AUDIO_FFT");
            ADMUX = 0x40; // use adc0
            for(int j=0;j<N;j++) { // reduces jitter
                cli();  // UDRE interrupt slows this way down on arduino1.0
                for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
                    while(!(ADCSRA & 0x10)); // wait for adc to be ready
                    ADCSRA = 0xf7; // restart adc
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
                
                sum += fft_log_out[18];
            }
            sum/=N;
            state = AUDIO_PROC;
            break;
        
        case AUDIO_PROC:
            Serial.println("AUDIO_PROC");
            Serial.println(sum);
            if(sum > 90){
                turn_led(1);
                state = IR_FFT;
            }else{
                turn_led(0);
            }
            sum = 0;
            break;

        case IR_FFT:
            Serial.println("IR_FFT");
            ADMUX = 0x41; // use adc1
            for(int j=0;j<N;j++) { // reduces jitter
                cli();  // UDRE interrupt slows this way down on arduino1.0
                for (int i = 0 ; i < 512 ; i += 2) { // save 256 samples
                    while(!(ADCSRA & 0x10)); // wait for adc to be ready
                    ADCSRA = 0xf5; // restart adc
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
                
                sum += fft_log_out[40];
            }
            sum/=N;
            state = IR_PROC;
            break;

        case IR_PROC:
            Serial.println("IR_PROC");
            Serial.println(sum);
            if(sum>55){
                turn_led(1);
                state = START;
            }else{
                turn_led(0);
            }
            sum = 0;
            
            break;
  }  
 
}
