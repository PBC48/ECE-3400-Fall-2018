
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
    ADCSRA = 0xe5; // adc prescaler
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

## Acoustic Team

From our analysis of the FFT and our determination of the frequency bin width, we determined that our 660Hz audio signal should fall in the fifth bin. We confirmed this by inputting a 660Hz sine wave from a function generator through a 330-ohm resistor into an analog Arduino pin and running the example fft_adc_serial code. We graphed the FFT output, as shown below. From this graph we saw our expected peak in the fifth bin.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/fft_660.PNG" width="500"/>
    <font size="2">
    <figcaption> FFT output from fft_adc_serial example code with 660Hz Signal from Function Generator
    </figcaption>
    </font>
</figure>

Next we created the simple microphone circuit from the lab document:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/simple_microphone.PNG" width="300"/>
    <font size="2">
    <figcaption> Basic Microphone Circuit from Lab Document
    </figcaption>
    </font>
</figure>

Although we had trouble getting any signal out of the microphone at first, we eventually were able to see a response on the oscilloscope when we played a 660Hz tone near the microphone from a tone generator on our phones. We observed that the amplitude of the signal was around 40 mV, and the output was unsteady and weak. 

To amplify this signal to a more detectable and readable value, we created a simple non-inverting amplifier, starting with a modest gain of around 5 to be safe. We first tested the amplifier with an input signal from the function generator, reading the output on the oscilloscope. For a while we were not able to obtain any output signal at all. After switching our op amp from an LF353 to LM358AN, the amplifier worked as expected; we saw the desired output and amplification.

We then put the microphone output through the amplifier. We again were not able to obtain any output signal, so we re-examined our connection between the microphone and amplifier and added a DC bias. Rather than send the simple microphone circuit output straight into the amplifier, we decided to keep the capacitor from the example circuit to remove its DC offset, and use a voltage divider to create a small, 50mV bias at the input to the amplifier. This allowed us to control our DC offset and left room for a large amplification. After solving this problem, we successfully increased our amplification to around 52. We played the 660Hz tone near the microphone and read the output to the scope to confirm that the signal was what we expected it to be. Our final design was the following amplifier circuit:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/Acoustic.png" width="500"/>
    <font size="2">
    <figcaption> Acoustic Amplifier Circuit Design
    </figcaption>
    </font>
</figure>

Finally, we connected the output from the amplified microphone signal to an analog Arduino pin and ran the FFT code. For clarity, we plotted only the bin 5 output on the serial plotter, and watched it spike when we brought the 660Hz tone near the microphone. In an effort to filter out more ambient noise, we narrowed the bin frequency width by changing the prescalar for the ADC clock input to 128. This was done by changing the ADC Control and Status Register A:

```cpp
    ADCSRA = 0xe5;
```

Re-calculating the frequency bin width we determined we should find our 660Hz frequency in bin 19, and we confirmed this by running the FFT and plotting the results:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/fft_660_128.PNG" width="500"/>
    <font size="2">
    <figcaption> Acoustic Amplifier Circuit Design
    </figcaption>
    </font>
</figure>

As a finishing touch, we created a placeholder “robot start” function by lighting an LED whenever the 660Hz tone was detected, which we did by setting a threshold intensity value for bin 19 as shown in the following code:

```cpp
  if(fft_log_out[18] > 90){
      digitalWrite(LED,HIGH);
    } else {
      digitalWrite(LED,LOW);
    }
```

A demonstration of this detection is shown in the following video:

<iframe width="560" height="315" src="https://www.youtube.com/embed/JvM9OUa2xY0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>


## Optical Team

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

### Upgraded Design
The signal strength of the FFT at our desired bin was already strong but we wanted to implement noise filtering which means we need a filter. We also want to amplify the values in order to utilize all 10 bits of the ADC for a higher resolution reading. The Arduino Analog input can only take in voltage values from 0 to 5 volts which means that any the input voltage can’t be negative or higher than that which will result in cut off and possibly damaging the circuit. Thus, this is the resulting schematic:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/schematic-Phototransistor%20with%20OpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> Schematic of IR with OpAmp and bandpass filter
    </figcaption>
    </font>
</figure>

We opted with using a high pass filter to remove any DC bias inherent in the output of the sensor because the DC bias is already high at around 4v. We then amplified the filtered signal by a factor of 20 which can be adjusted as needed. The amplification seems like a good amount based on the detection strength of the IR sensor. After we amplified the signal, we ran the output through a low pass filter that removes any high frequency noise and harmonics. This completes our bandpass filter.


### Testing
For testing we started with unit tests by turning on the hat and holding it a certain distance from the phototransistor and check the output of the FFT printing to serial. We also implemented a blinking LED that would increase blinking rate as the IR gets closer to the phototransistor. The frequency of the blink rates tell us how close the hat is to the IR sensor. This tells us that the sensor is working as intended. 

We also reedited the FFT library's codes to record FFT values in a single FFT cycle for better side by side comparison. Here are the results:

We divided the tests as such:
-off: IR hat turned off
-far: IR hat 1.5 intersections away from sensor
-mid: IR hat 0.5 intersections away from sensor
-close: IR hat right next to sensor

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

From this two comparisons, we can see that the op amp increases mid range performance of the IR sensor by detecting more of the IR hat's correct frequency signal whereas the harmonics appears to be more filtered out as a result of the installed bandpass filters. The long range performance appears to be unaffected by the augmentation and the close range performance clearly increased slightly.


After testing that the sensor could detect the desired signal, we then tested the robustness of our filtering software and hardware by giving it decoy signals. We used a decoy IR signal at around 12kHz placed next to the sensor and read its FFT's. We also used the decoy to test our sensor for detecting different frequencies. 
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
We reached two conclusions with this test. The augmented sensor worked with different frequencies because the signal strengths were clearly amplified. Although for this test, it appears some of the noise were amplified as well but it was not significant. We also see that the 40th bin of the FFT is contains only background signals. This means that the signal strength will not be enough for the threshhold to detect a false positive. With these two tests, we proved that the IR sensor's hardware and software can detect the desired frequency amount and augmented it so that it detect farther away signals.  


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
