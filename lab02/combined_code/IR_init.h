

int N = 10;
uint32_t target_freq = 6080;
uint32_t smp_freq = 19235*2;

uint32_t sum = 0;


void ir_init(){
    TIMSK0 = 0; // turn off timer0 for lower jitter
    ADCSRA = 0xe7; // set the adc to free running mode
    ADMUX = 0x40; // use adc0
    DIDR0 = 0x01; // turn off the digital input for adc0
}