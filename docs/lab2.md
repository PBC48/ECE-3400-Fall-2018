
# Lab 2: Analog Circuitry and FFTs

## Objectives

## Introduction
In lab 2, we added hardware sensors and signal processing capabilities to the robot.  We split into two subteams, with Tara and Chrissy working on acoustics and Xiaoyu and Patrick on optical sensing. The start of our final maze will commence with a 660 Hz whistle blow, so the acoustic team used an Electret microphone and an amplifying circuit to detect the tone and distinguish it from background noise.  The optical group used an IR transistor to detect other robots emitting IR at 6.08kHz, and ignore decoys (18kHz).

## FFT/ADC
The “Fast Fourier Transform” is an operation that uses the Discrete Time Fourier Transform to sample a signal over time and return its frequency components.  Because both sub teams must implement algorithms that detect only specific frequencies in a noisy environment, we each installed the Open Music Labs FFT library in our Arduino IDE and studied how the script distinguished frequencies, with the goal of identifying the frequency bin where we should look for our signal.  First, we looked at the sampling frequency of the ADC.  There are two ways to identify this rate -- the first is to use the oscilloscope and the digitalWrite() function to change the state of the digital pin whenever the ADC finishes one conversion. The scope will then measure the frequency of the output wave. Based on our implementation, we know that the ADC converts two values per period, so the frequency is about 38 kHz.  To confirm this result, we referenced the manual, which indicates that the prescaler division factor of the ADC is set to 32.  Given the 16MHz Arduino and the 13 clock cycles to convert, we used the formula Arduino clock cycle / total conversion clock cycles / prescaler to find the 38 kHz sampling frequency. The resulting frequency range of each bin is around 148Hz.
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/frequency%20of%20ADC.PNG" width="800"/>
    <font size="2">
    <figcaption> Frequency of the ADC
    </figcaption>
    </font>
</figure>

## Acoustics

<iframe width="560" height="315" src="https://www.youtube.com/embed/JvM9OUa2xY0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Optical
<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IR_NO_OPAMP.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of IR without Op Amp
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IR_WITH_OPAMP.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of IR with Op Amp
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/IR_FFT_without%20opamp.PNG" width="800"/>
    <font size="2">
    <figcaption> FFT of IR without augmentations
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/lab02/schematic-Phototransistor%20with%20OpAmp.PNG" width="800"/>
    <font size="2">
    <figcaption> Schematic of IR with OpAmp
    </figcaption>
    </font>
</figure>

## Integration
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
