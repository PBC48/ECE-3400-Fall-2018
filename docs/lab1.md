# Lab 1 Demonstration and Results
## Introduction

The purpose of lab 1 is to learn introductory Arduino programming through the Arduino IDE and Arduino Uno as well as to gain exposure to prototyping a robot. Through the completion of the lab, we became familiar with basic Arduino functionalities. We worked with Arduino I/O ports as well as serial communication for LED, servo control, and communication using built-in Arduino functions. We used Arduino built-in functions to read analog inputs and set the I/O pins on the Arduino Uno. The lab culminated with the team building a functional robot that could drive in a square.

## Blink

We set up blinking on the Uno built-in IDE by using an Arduino example code. This was done by going to File -> Examples -> 1.Basics-> Blink. After opening the file, we compiled and uploaded the code onto the Uno using the right arrow button. 
  
### Demonstration of Internal LED Blinking
<a href="http://www.youtube.com/watch?feature=player_embedded&v=Xe242vkh8Xo" target="_blank"><img src="http://img.youtube.com/vi/Xe242vkh8Xo/0.jpg" 
alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

We then modified the LED blink sketch for an external LED. We used a solderless breadboard with wires connected to a digital I/O pin and the Arduino ground. We used a resistor greater than 300 Ohms in the circuit. As resistance increased, the LED dimmed. 


To toggle the LED light, we toggled the digital I/O port connected to the external LED. We made a few adjustments to the example blink code.

![BlinkCode](/images/blinkcode.png)

### Demonstration of External LED Blinking
<a href="http://www.youtube.com/watch?feature=player_embedded&v=pbVh3dkpa-c" target="_blank"><img src="http://img.youtube.com/vi/pbVh3dkpa-c/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>

### Demonstration of LED Controlled by Potentiometer

<a href="http://www.youtube.com/watch?feature=player_embedded&v=Dvo0qhI7H8Y" target="_blank"><img src="http://img.youtube.com/vi/Dvo0qhI7H8Y/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>
### Demonstration of Servos Controlled via Pulse Width Modulation (PWM)


<a href="http://www.youtube.com/watch?feature=player_embedded&v=rJJShOBNLfM" target="_blank"><img src="http://img.youtube.com/vi/rJJShOBNLfM/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>
### Demonstration of Robot Moving in a Square


<a href="http://www.youtube.com/watch?feature=player_embedded&v=az1fnLFKQlA" target="_blank"><img src="http://img.youtube.com/vi/az1fnLFKQlA/0.jpg" alt="Demonstration of Internal LED Blinking" width="240" height="180" border="10" /></a>
