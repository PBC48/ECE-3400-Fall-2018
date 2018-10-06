
# Lab 2: Analog Circuitry and FFTs

## Objectives

## Introduction
In lab 2, we added hardware sensors and signal processing capabilities to the robot.  We split into two subteams, with Tara and Chrissy working on acoustics and Xiaoyu and Patrick on optical sensing. The start of our final maze will commence with a 660 Hz whistle blow, so the acoustic team used an Electret microphone and an amplifying circuit to detect the tone and distinguish it from background noise.  The optical group used an IR transistor to detect other robots emitting IR at 6.08kHz, and ignore decoys (18kHz).

## FFT/ADC
The “Fast Fourier Transform” is an operation that uses the Discrete Time Fourier Transform to sample a signal over time and return its frequency components.  Because both sub teams must implement algorithms that detect only specific frequencies in a noisy environment, we each installed the Open Music Labs FFT library in our Arduino IDE and studied how the script distinguished frequencies, with the goal of identifying the frequency bin where we should look for our signal.  First, we looked at the sampling frequency of the ADC. There are two ways to identify this rate -- the first is to use the oscilloscope and the digitalWrite() function to change the state of the digital pin whenever the ADC finishes one conversion. The scope will then measure the frequency of the output wave. Based on our implementation, we know that the ADC converts two values per period, so the frequency is about 38 kHz.  
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/frequency%20of%20ADC.PNG" width="800"/>
    <font size="2">
    <figcaption> Frequency of the ADC
    </figcaption>
    </font>
</figure>
To confirm this result, we referenced the manual, which indicates that the prescaler division factor of the ADC is set to 32.  Given the 16MHz Arduino and the 13 clock cycles to convert, we used the formula Arduino clock cycle / total conversion clock cycles / prescaler to find the 38 kHz sampling frequency. 

Calling the FFT function is simple using the Music Labs' library where we first must setup the ADC settings:
```cpp
    ADMUX = 0x40; // use adc0
    ADCSRA = 0xe7; // adc prescaler
```
The FFT libraries takes care of the actual calculations of frequencies from the analog input to FFT outputs. We had to take the necessary samples for the library to calculate:
```cpp
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
```
Once this is down we can grab our data using 
```cpp
    fft_log_out
```
which hosts the stored data in bins with a specific frequency range per bin.

## Acoustics

<iframe width="560" height="315" src="https://www.youtube.com/embed/JvM9OUa2xY0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Optical

<iframe width="560" height="315" src="https://www.youtube.com/embed/_hD_c_GUQas" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

### Initial design
We used the OP598A phototransistor to detect IR signals. The phototransistor was built exactly like the schemamtic from the lab, with a 1.8k reistor connected to 5V power supply and the photoresistor connected to ground. We first put that output into the oscilioscope and got the following reading for FFT: 

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IR_FFT_without%20opamp.PNG" width="800"/>
    <font size="2">
    <figcaption> Oscilloscope FFT of IR Sensor without augmentations
    </figcaption>
    </font>
</figure>

The result is from turning the IR hat near the sensor. The signal strength appears to be pretty strong as there are clearly readings at the 6kHz mark and all its harmonics. For the Arduino, we used FFT library from Music Labs' FFT. With the library, we can check the intensity of a desired frequency range to see if the it recieved any signals. We chose a 256 point FFT with a known ADC sampling frequency of 38.5kHz. Therefore, each bin of the FFT has a frequency range of Fs/N or around 150Hz per bin. Thus the 6.08kHz desired frequency is located at bin 40. Thus, all we have to call is 
```cpp
    fft_log_out[40]; //fft_log_out is the strength of the signal converted to dB.
```
to get our readings.

### Upgraded design
The signal strength of the FFT at our desired bin was already strong but we wanted to implement noise filtering which means we need a filter. We also want to amplify the values in order to utilize all 10 bits of the ADC for a higher resolution reading. The Arduino Analog input can only take in voltage values from 0 to 5 volts which means that any the input voltage can’t be negative or higher than that which will result in cut off and possibly damaging the circuit. Thus, this is the resulting schematic:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/schematic-Phototransistor%20with%20OpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> Schematic of IR with OpAmp and bandpass filter
    </figcaption>
    </font>
</figure>

We opted with using a high pass filter to remove any DC bias inherent in the output of the sensor. Then we added our own DC bias of 2.5V to put the steady state signal in the middle of the possible voltage range of the pins. This allows us to take in negative voltage since the negative voltage will be offset by the DC to above zero. 



### Testing
For testing we started with unit tests by turning on the hat and holding it a certain distance from the phototransistor and check the output of the FFT printing to serial. We also implemented a blinking LED that would increase blinking rate as the IR gets closer to the phototransistor. The frequency of the blink rates tell us how close the hat is to the IR sensor. This tells us that the sensor is working as intended. 




<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IRnoOpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of IR without Op Amp
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IROpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of IR with Op Amp
    </figcaption>
    </font>
</figure>


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/DecoyOpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of Decoy 
    </figcaption>
    </font>
</figure>


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/Decoy.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of Decoy with Op Amp
    </figcaption>
    </font>
</figure>



## Integration

<iframe width="560" height="315" src="https://www.youtube.com/embed/v4Z3QcfFZ4k" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/Lab2integrated_1.PNG" width="800"/>
    <font size="2">
    <figcaption> First Part of State Machine
    </figcaption>
    </font>
</figure>
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/Lab2integratedcode_2.PNG" width="800"/>
    <font size="2">
    <figcaption> Second Part of State Machine
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/20181003_164149.jpg"/>
    <font size="2">
    <figcaption> The IR sensor and microphone together!
    </figcaption>
    </font>
</figure>
