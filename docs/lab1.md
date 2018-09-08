# Lab 1 Demonstration and Results
## Objectives
* Familiarize ourselves with the Arduino IDE and Arduino Uno
* Learn introductory Arduino programming and basic Arduino functionalities
* Create simple circuits and Arduino programs to explore digital and analog Arduino I/O ports, built-in Arduino functions, and serial communication
* Construct our robot and write a program to make it move in a square

## Introduction

In Lab 1, we began with using digital I/O pins to make internal and external LEDs blink. We then used a potentiometer to explore using analog circuits with the Arduino. We started by simply reading out analog values from the potentiometer on the serial port. Then we used these values to adjust the brightness of an external LED and the speed of a continuously rotating servo. The lab culminated in building a functional robot that could drive autonomously in a square.

## Blink

We made the Uno’s built-in LED blink by running an Arduino example code, found as follows: 

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/blinkAccessScreenshot.png" width="800"/>
    <font size="2">
    <figcaption> Blinking LED example code location
    </figcaption>
    </font>
</figure>

The code worked as expected, as can be seen by the following demonstration of the internal LED blinking:

<iframe width="600" height="350" src="https://www.youtube-nocookie.com/embed/Xe242vkh8Xo?rel=0&amp;controls=0&amp;showinfo=0?autoplay=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

Next, we modified the LED blink sketch for an external LED. We adjusted the example blink code to configure one of the digital I/O ports (pin 13 in the code shown below) for the external LED. In doing so we made the LED blink by toggling the output of the digital I/O pin connected to the external LED between HIGH and LOW. For digital I/O ports, the output signal can be set either at HIGH, at 3.3V, or LOW, at 0V. Thus, when we used the Arduino function digitalWrite() to output a HIGH to the digital pin, we output 3.3V to the circuit, which turned the LED on. Outputting a LOW, or 0V, turned the LED off. 

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Code-blink.PNG" width="600">
    <font size="2">
    <figcaption> Blinking External LED Code
    </figcaption>
    </font>
</figure>

We set up a circuit for the external LED using a solderless breadboard with wires to connect the LED in series with a 1k Ohm resistor to a digital I/O pin and the Arduino ground, as shown in the circuit schematic below. When we used larger resistors in the circuit, the LED was dimmer.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-Blinking.PNG" width="200">
    <font size="2">
    <figcaption> Schematic for blinking external LED 
    </figcaption>
    </font>
</figure>

Our setup worked as expected, as shown by the following demonstration of the external LED blinking:

<iframe width="560" height="315" src="https://www.youtube.com/embed/pbVh3dkpa-c?rel=0&amp;controls=0&amp;showinfo=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>



## Serial Monitor and Analog Pins

To experiment with using the serial monitor and analog pins, we tested reading values from a potentiometer. We connected the potentiometer to the Arduino by connecting its output to an analog input pin of the Arduino, and connecting the potentiometer to the Arduino's 5V power and ground. To avoid accidentally short-circuiting the LED, we also added a 300 Ohm resistance in series with the potentiometer. We used the analogRead() function to read the potentiometer values and used the Serial.println() function to print those values to the serial monitor. These functions are included in the code for the LED analog output.

## LED Analog Output

Next, we used the analog output from the potentiometer to control the brightness of the LED. After reading the potentiometer value, we adjusted that value and used the analogWrite() function to output it to an Arduino pin with PWM capability (pin 10 in the code below). We had to adjust the output value to be within the range accepted by the analogWrite() function; the potentiometer readings returned values between 0 and 1023, and the analogWrite() function accepts values between 0 and 255.


<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Code-analog.PNG" width="500">
    <font size="2">
    <figcaption> Blinking LED code location
    </figcaption>
    </font>
</figure>

We set up a circuit according to the following schematic, similar to what we had already set up to read the potentiometer values and adding the connection of the LED to a PWM pin:

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-PotentiometerBlink.PNG" width="500">
    <font size="2">
    <figcaption> Schematic for controlling LED brightness with potentiometer
    </figcaption>
    </font>
</figure>

In accordance with our earlier observation of larger resistances resulting in a dimmer LED, adjusting the potentiometer changed the brightness of the LED, as shown by the following demonstration of the LED controlled by the potentiometer:

<iframe width="560" height="315" src="https://www.youtube.com/embed/Dvo0qhI7H8Y?rel=0&amp;controls=0&amp;showinfo=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>



## Servos

The Arduino analog I/O pins can control the turn direction and speed of the servos. We relied primarily on the Arduino’s servo library to control the servo. On a high level, we dictated the analog I/O port that will be used to control the servos and then used the write function of the servo library to dictate the speed and direction of the servo. We combined the servo with the potentiometer, allowing the potentiometer to control the speed and direction of the servo.

### Demonstration of Servos Controlled via Pulse Width Modulation (PWM)


<iframe width="560" height="315" src="https://www.youtube.com/embed/rJJShOBNLfM?rel=0&amp;controls=0&amp;showinfo=0" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-ServoPotentiometer.PNG" width="500">
    <font size="2">
    <figcaption> Schematic for controlling servo with potentiometer
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Code-servo.PNG" width="300">
    <font size="2">
    <figcaption> Schematic for controlling blink with potentiometer
    </figcaption>
    </font>
</figure>

## Robot

We built the robot with a chassis and two servos with wheels attached, and placed the Arduino on top.  We also built in a 5v DC phone charging port to power the robot.  Finally, we wrote a simple Arduino program for the robot to drive in a square.

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Code-robotDrive.PNG" width="300">
    <font size="2">
    <figcaption> Schematic for controlling blink with potentiometer
    </figcaption>
    </font>
</figure>

<figure>
    <img src="https://raw.githubusercontent.com/PBC48/ECE-3400-Fall-2018/master/docs/images/Schematic-Robot.PNG" width="600">
    <font size="2">
    <figcaption> Schematic for current robot
    </figcaption>
    </font>
</figure>

### Demonstration of Robot Moving in a Square

<iframe width="560" height="315" src="https://www.youtube.com/embed/az1fnLFKQlA?rel=0&amp;controls=0&amp;showinfo=0&amp;start=1" frameborder="0" allow="autoplay; encrypted-media" allowfullscreen></iframe>

## Conclusion and Result

The robot moved as expected in the square and there were no problems with the speed of the servos. However, we did encounter a problem with the direction of one of the motors.  The orientation of the two motors was inconsistent so the right and left wheels spun in opposite directions.  This slightly complicated the code to direct the robot, requiring a roundabout solution even to go in a straight line.  In the next lab section, we will fix the installation of the wheels by reversing the orientation of one of the wheels. 
