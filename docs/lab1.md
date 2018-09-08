# Lab 1 Demonstration and Results
## Objectives
* Familiarize ourselves with the Arduino IDE and Arduino Uno
* Learn introductory Arduino programming and basic Arduino functionalities
* Create simple circuits and Arduino programs to explore digital and analog Arduino I/O ports, built-in Arduino functions, and serial communication
* Construct our robot and write a program to make it move in a square

## Introduction

In Lab 1, we began with using digital I/O pins to make internal and external LEDs blink. We then used a potentiometer to explore using analog circuits with the Arduino. We started by simply reading out analog values from the potentiometer on the serial port. Then we used these values to adjust the brightness of an external LED and the speed of a continuously rotating servo. The lab culminated in building a functional robot that could drive autonomously in a square.

## Blink

We set up blinking on the Uno’s built-in LED by using an Arduino example code found as follows: 

![alt text](https://github.com/PBC48/ECE-3400-Fall-2018/tree/master/docs/images/blinkAccessScreenshot.png "Logo Title Text 1")


<figure>
    <img src="https://github.com/PBC48/ECE-3400-Fall-2018/tree/master/docs/images/Code-analog.PNG" width="400">
    <font size="2">
    <figcaption> Blinking LED code location<br> </a> 
    </figcaption>
    </font>
</figure>

### Demonstration of Internal LED Blinking
<a href="http://www.youtube.com/watch?feature=player_embedded&v=Xe242vkh8Xo" target="_blank"><img src="http://img.youtube.com/vi/Xe242vkh8Xo/0.jpg" 
alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

Next, we modified the LED blink sketch for an external LED. We set up a circuit for the external LED using a solderless breadboard with wires to connect the LED in series with a 1kOhm resistor to a digital I/O pin and the Arduino ground. When we used larger resistors in the circuit, the LED dimmed. 

<figure>
    <img src="https://github.com/PBC48/ECE-3400-Fall-2018/tree/master/docs/images/Schematic-Blinking.PNG" width="400">
    <font size="2">
    <figcaption> Schematic for blinking external LED<br> </a> 
    </figcaption>
    </font>
</figure>


To make the LED blink, we toggled the digital I/O port connected to the external LED between HIGH and LOW. We made a few adjustments to the example blink code to configure one of the I/O ports (pin 13 in the code shown below) for the external LED.

<figure>
    <img src="https://github.com/PBC48/ECE-3400-Fall-2018/tree/master/docs/images/Code-blink.PNG" width="400">
    <font size="2">
    <figcaption> Blinking LED code location<br> </a> 
    </figcaption>
    </font>
</figure>

### Demonstration of External LED Blinking
<a href="http://www.youtube.com/watch?feature=player_embedded&v=pbVh3dkpa-c" target="_blank"><img src="http://img.youtube.com/vi/pbVh3dkpa-c/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

For digital I/O ports, the output signal can be set either at HIGH, at 3.3v, or LOW, at 0v. Thus, when we used digitalwrite to output a high to the digital pin, we output 3.3v to the circuit, which turned the LED on. Outputting the 0v low turned the LED off.

## Serial Monitor and Analog Pins

We experimented with using potentiometers to control LED brightness. First, we tested the potentiometer readings using analogRead() and the serial monitor. We connected one wire to one of the side pins which goes to the 5v Arduino output pin, and the other side pin to a wire connecting to ground. We used the built in funcion Serial.println() to output analog values from the potentiometer’s middle pin. To avoid accidentally short circuiting the LED, we also added a 300 Ohm resistance in series with the potentiometer. 

## LED Analog Output

We used analogRead() to read the analog output from the potentiometer and to control the brightness of the LED in software. 

![LED Analog Code]("/docs/lab01 media/LEDAnalog.PNG")

### Demonstration of LED Controlled by Potentiometer

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Dvo0qhI7H8Y" target="_blank"><img src="http://img.youtube.com/vi/Dvo0qhI7H8Y/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

## Servos

The Arduino analog I/O pins can control the turn direction and speed of the servos. We relied primarily on the Arduino’s servo library to control the servo. On a high level, we dictated the analog I/O port that will be used to control the servos and then used the write function of the servo library to dictate the speed and direction of the servo. We combined the servo with the potentiometer, allowing the potentiometer to control the speed and direction of the servo.

### Demonstration of Servos Controlled via Pulse Width Modulation (PWM)


<a href="http://www.youtube.com/watch?feature=player_embedded&v=rJJShOBNLfM" target="_blank"><img src="http://img.youtube.com/vi/rJJShOBNLfM/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

## Robot

We built the robot with a chassis and two servos with wheels attached, and placed the Arduino on top.  We also built in a 5v DC phone charging port to power the robot.  Finally, we wrote a simple Arduino program for the robot to drive in a square.

### Demonstration of Robot Moving in a Square

<a href="http://www.youtube.com/watch?feature=player_embedded&v=az1fnLFKQlA" target="_blank"><img src="http://img.youtube.com/vi/az1fnLFKQlA/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

## Conclusion and Result

The robot moved as expected in the square and there were no problems with the speed of the servos. However, we did encounter a problem with the direction of one of the motors.  The orientation of the two motors was inconsistent so the right and left wheels spun in opposite directions.  This slightly complicated the code to direct the robot, requiring a roundabout solution even to go in a straight line.  In the next lab section, we will fix the installation of the wheels by reversing the orientation of one of the wheels. 
