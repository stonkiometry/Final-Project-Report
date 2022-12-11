# Microcontroller controlled chemical reactions - Titration
## Proposal
We wish to use microcontrollers to control a chemical reaction. As an example we have chosen Titration. This is mainly because this is a slightly less complicated chemical reaction, with a stable endpoint following which no reaction will occur. The link to our complete proposal: https://github.com/harishramesh98/Project_Team_Stonkiometry/blob/7acee29b5e772328fcbecf8f17a8a46d4d0e9c68/Proposal/ESE_5190_PROJECT_PROPOSAL_TEAM_STONKIOMETRY.pdf

## Functionality to be implemented as a part of the proposal
Primary Objectives:
- Firstly we want to be able to sense the color change.
- We want to be able to rotate open and close the burette's knob.
Secondary objectives:
- Automate reading the amount of liquid dispensed by the burette.

## Components used:
### 1. RP2040
<img src=https://user-images.githubusercontent.com/38978733/205590895-fba17c42-1fa2-400a-8e27-4175002fcc75.jpg width="200" height="200"/> <img src=https://user-images.githubusercontent.com/38978733/205590897-ba245065-50f3-4131-baf4-95f8df0171a4.jpg width="200" height="200"/><br>
We have used both the RP2040 QtPy as well as the RP2040 Pico4ML in setting up our project.

### 2. Titration Kit
<img src=https://user-images.githubusercontent.com/38978733/205590893-88327c66-e70a-482e-8dd3-428f3c677749.jpg width="300" height="300"/>
For titration, our primary component from the kit is te burette, whose know allows control of the flow of reagents into the reaction. The beaker is where the reaction will take place and the endpoint will be sensed. The clamp stand will be used to prop the burette up. As far as the chemicals go, we have currently decided on using Dilute Hydrochloric Acid as the acid, dilute Potassium Hydroxide as the base and phenopthalein as the indicator. We plan to meet with a chemistry professor for some of the components, and so this is subject to change.

### 3. APDS9960
<img src=https://user-images.githubusercontent.com/38978733/205590913-e94108d6-2fb1-43c2-b307-7172ca7375ba.jpg width="200" height="200"/>
We have used the APDS9960 to sense the color change when the end point is reached.

### 4. Motor
<img src=https://user-images.githubusercontent.com/38978733/205590889-bc8f02c8-7277-463e-8a5d-1f99b7c407ce.jpg width="200" height="200"/><img src=https://user-images.githubusercontent.com/38978733/205590891-829168b6-fec8-4414-88bb-cd8fe5db12ed.jpg width="200" height="200"/> <br>
We have ordered the NEMA-17 stepper motor and corresponding motor driver because of its high speed and torque. However, due to logistical delays, we have experimented with the parallax servo motor.  

## Progress:
- We are using the APDS9960 to sense the color change when the reaction has completed. To set this up we used the i2c_bus_scan code from the pico_examples as a reference. In order to compare current data and previous data, we have set up structures, variables and counters to emulate a ring buffer. This setup constantly maintains track of the color it senses. Program is exited when the endpoint is reached. This is setup with the logic that the color will persist when the end-point is reached. PIO has been used in the implementation. The APDS9960 interfaces with the RP2040 using I2C. Data is sampled once every second.
- The servo motor has been set up to rotate slightly and return to starting point immediately so as to allow a small amount of reagent in. This needed setting up PWM. So we referred to PWM in pico_examples as well as a few codes online (mainly https://github.com/metanav/pico_servo_pio). PIO has been used in this implementation. 

## Midpoint outputs:
- The APDS9960 is used to sense the color change. Since we mainly expect to use phenopthalein, the solution is expected to turn pink. So we pay extra attention to the rval and bval stored in the color data register. While the code has been written to account for the color vanishing when the end-point has not been reached, due to time constraints the demo only shows a situation with a persistent pink color.

https://user-images.githubusercontent.com/38978733/205597049-97f19b58-5c57-406a-bf03-e7bdcf53fbaa.mp4

- The motor has been designed to open only slightly before closing. This functionality can be seen in the video.

https://user-images.githubusercontent.com/38978733/205597223-51e67dcd-fb41-46d8-ad68-22e2b736a537.mp4

## Issues faced and potential fixes:
- Our initial proposal involved using the camera on the Pico4ML. This was complicated mainly because this would require implementing a base level of computer vision as well as it might have delays and overheads associated with processing. We decided to use the APDS9960 instead to sense the color change. As it uses I2C the programming is pretty lightweight and fast. 
- Since we are using the sensitive APDS9960 to sense color, we need to isolate it. We could facilitate this using a structure like a shoebox to isolate the beaker. However, this also requires to illuminate the beaker inside with some mild lighting. This could be done using a white LED and a translucent screen.
- Right now our code is built on two seperate microcontrollers. Daisy-chaining them is a possibility, however, for both speed and ease of programming, it is preferable if the same board handled both functions.
- The endpoint that stops the program execution is something that requires experimentation. In our case a lot of the arbitrary limits we have decided requires some experimentation to tune and fix. This in turn requires time.

## Progress as of 9-December-2022
- We have combined both functionalities to work on a single RP2040 QtPy board, using 2 PIOs and state machines. We have been able to trigger a lighter servo motor, however we need to be able to trigger a heavier servo motor, and the power source we used (a power bank) gave insufficient current to drive the heavier servo. However, as the PWM implemented is the same, once scaled, we can expect similar response. We have ideated different ideas to couple the motor to the burette. We also need to work on testing and tuning the parameters involved for color change.

https://user-images.githubusercontent.com/38978733/206837550-ae40caa8-ad9f-41df-afe8-85a7afe7959d.mp4

## Progress as of 10-December-2022
- Coupling with Burette achieved. Able to modulate the pulse width, triggered using the PIO on the RP2040 QtPy, to precisely control the knob on the burette to allow measured amount of liquid through the burette.

![](https://github.com/harishramesh98/Project_Team_Stonkiometry/blob/a6f44f397fdeb8a0a89d155b8089a7bd66b511cb/Outputs/regulating_burette.gif)

- Minor setback with the APDS Sensor. With the actual reaction, that we set up manually to see the expected color values, we noticed because of the way the base example code was written, accessing the APDS registers made it so that the outputs were in multiples of 512. This made it hard to differentiate between the expected Red, Blue and Green values in the registers. While the APDS is very sensitive and has a wide range of detection, it does not show enogh variation for the pale color that the titration reaction produces. As a work around, we could rewrite the logic to work with smaller changes, however this could give non-deterministic performance. We could use a seperate board for sensing color and maybe run that board on python as it seems easier to read the APDS using circuit python. We could also look into other sensors.
