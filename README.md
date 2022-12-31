# Microcontroller controlled chemical reactions - Titration
## Proposal
We wish to use microcontrollers to control a chemical reaction. As an example we have chosen Titration. This is mainly because this is a slightly less complicated chemical reaction, with a stable endpoint following which no reaction will occur. The link to our complete proposal: [Team_Proposal.pdf](https://github.com/harishramesh98/Project_Team_Stonkiometry/blob/7acee29b5e772328fcbecf8f17a8a46d4d0e9c68/Proposal/ESE_5190_PROJECT_PROPOSAL_TEAM_STONKIOMETRY.pdf)

Our Result : [![Final Output](https://i.ytimg.com/vi/uNyA_uMM8rk/hqdefault.jpg)](https://youtu.be/uNyA_uMM8rk "Final Output")

## Functionality to be implemented as a part of the proposal
Primary Objectives:
- Firstly we want to be able to sense the color change.
- We want to be able to rotate open and close the burette's knob.
Secondary objectives:
- Automate reading the amount of liquid dispensed by the burette.

## Components used:
### 1. RP2040
![image](https://user-images.githubusercontent.com/38978733/205590895-fba17c42-1fa2-400a-8e27-4175002fcc75.jpg) ![image](https://user-images.githubusercontent.com/38978733/205590897-ba245065-50f3-4131-baf4-95f8df0171a4.jpg)
We have used both the RP2040 QtPy as well as the RP2040 Pico4ML in setting up our project.

### 2. Titration Kit
![image](https://user-images.githubusercontent.com/38978733/205590893-88327c66-e70a-482e-8dd3-428f3c677749.jpg)
For titration, our primary component from the kit is te burette, whose know allows control of the flow of reagents into the reaction. The beaker is where the reaction will take place and the endpoint will be sensed. The clamp stand will be used to prop the burette up. As far as the chemicals go, we have currently decided on using Dilute Hydrochloric Acid as the acid, dilute Potassium Hydroxide as the base and phenopthalein as the indicator. We plan to meet with a chemistry professor for some of the components, and so this is subject to change.

### 3. APDS9960
![image](https://user-images.githubusercontent.com/38978733/205590913-e94108d6-2fb1-43c2-b307-7172ca7375ba.jpg)
We have used the APDS9960 to sense the color change when the end point is reached.

### 4. Motor
![image](https://user-images.githubusercontent.com/38978733/205590891-829168b6-fec8-4414-88bb-cd8fe5db12ed.jpg)
We had ordered the NEMA-17 stepper motor and corresponding motor driver because of its high speed and torque. However, due to logistical delays, we have experimented with the parallax servo motor.  

## Progress:
- We are using the APDS9960 to sense the color change when the reaction has completed. To set this up we used the i2c_bus_scan code from the pico_examples as a reference. In order to compare current data and previous data, we have set up structures, variables and counters to emulate a ring buffer. This setup constantly maintains track of the color it senses. Program is exited when the endpoint is reached. This is setup with the logic that the color will persist when the end-point is reached. PIO has been used in the implementation. The APDS9960 interfaces with the RP2040 using I2C. Data is sampled once every second.
- The servo motor has been set up to rotate slightly and return to starting point immediately so as to allow a small amount of reagent in. This needed setting up PWM. So we referred to PWM in pico_examples as well as a few codes online (mainly https://github.com/metanav/pico_servo_pio). PIO has been used in this implementation. 

## Midpoint outputs:
- The APDS9960 is used to sense the color change. Since we mainly expect to use phenopthalein, the solution is expected to turn pink. So we pay extra attention to the rval and bval stored in the color data register. While the code has been written to account for the color vanishing when the end-point has not been reached, due to time constraints the demo only shows a situation with a persistent pink color.

[![JustAPDS](https://res.cloudinary.com/marcomontalbano/image/upload/v1672446811/video_to_markdown/images/youtube--ksuHuA3v_wM-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/ksuHuA3v_wM "JustAPDS")

- The motor has been designed to open only slightly before closing. This functionality can be seen in the video.

[![Just Servo](https://res.cloudinary.com/marcomontalbano/image/upload/v1672447051/video_to_markdown/images/youtube--b4WSYsbX98U-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/b4WSYsbX98U "Just Servo")

## Issues faced and potential fixes:
- Our initial proposal involved using the camera on the Pico4ML. This was complicated mainly because this would require implementing a base level of computer vision as well as it might have delays and overheads associated with processing. We decided to use the APDS9960 instead to sense the color change. As it uses I2C the programming is pretty lightweight and fast. 
- Since we are using the sensitive APDS9960 to sense color, we need to isolate it. We could facilitate this using a structure like a shoebox to isolate the beaker. However, this also requires to illuminate the beaker inside with some mild lighting. This could be done using a white LED and a translucent screen.
- Right now our code is built on two seperate microcontrollers. Daisy-chaining them is a possibility, however, for both speed and ease of programming, it is preferable if the same board handled both functions.
- The endpoint that stops the program execution is something that requires experimentation. In our case a lot of the arbitrary limits we have decided requires some experimentation to tune and fix. This in turn requires time.

## Progress as of 9-December-2022
- We have combined both functionalities to work on a single RP2040 QtPy board, using 2 PIOs and state machines. We have been able to trigger a lighter servo motor, however we need to be able to trigger a heavier servo motor, and the power source we used (a power bank) gave insufficient current to drive the heavier servo. However, as the PWM implemented is the same, once scaled, we can expect similar response. We have ideated different ideas to couple the motor to the burette. We also need to work on testing and tuning the parameters involved for color change.

[![Both working Together](https://res.cloudinary.com/marcomontalbano/image/upload/v1672447099/video_to_markdown/images/youtube--kvPNIWoTFEg-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/kvPNIWoTFEg "Both working Together")

## Progress as of 10-December-2022
- Coupling with Burette achieved. Able to modulate the pulse width, triggered using the PIO on the RP2040 QtPy, to precisely control the knob on the burette to allow measured amount of liquid through the burette.

![](https://github.com/harishramesh98/Project_Team_Stonkiometry/blob/a6f44f397fdeb8a0a89d155b8089a7bd66b511cb/Outputs/regulating_burette.gif)

- Minor setback with the APDS Sensor. With the actual reaction, that we set up manually to see the expected color values, we noticed because of the way the base example code was written, accessing the APDS registers made it so that the outputs were in multiples of 512. This made it hard to differentiate between the expected Red, Blue and Green values in the registers. While the APDS is very sensitive and has a wide range of detection, it does not show enogh variation for the pale color that the titration reaction produces. As a work around, we could rewrite the logic to work with smaller changes, however this could give non-deterministic performance. We could use a seperate board for sensing color and maybe run that board on python as it seems easier to read the APDS using circuit python. We could also look into other sensors.

## Progress as of 13-December-2022 - COMPLETED
- We incrementally tested and made changes to our program until we reached desired results. Our initial algorithm did not produce the results we needed. We went back to the drawing board and rewrote a new algorithm that we had to debug and make changes as we went. Without going too much into details, we emulated a ring buffer again, but this time we collected extensive data with how the APDS interacted with the colors we expected and strong light sources. After tabulating this in excel and generating graphs we began noticing patterns that we needed to account for and fine-tune our program.  

- Data Observed for red color without indicator

![image](https://user-images.githubusercontent.com/38978733/207529740-c30cad27-eb86-4527-aa76-047bdf6e0423.png)

- Data observed for green color without indicator

![image](https://user-images.githubusercontent.com/38978733/207529758-e9c1ee34-2722-4224-ac29-7e1a1c5e9989.png)

- Data observed for blue color without indicator

![image](https://user-images.githubusercontent.com/38978733/207529777-4ae29922-43a9-4a90-83c7-611474e8a3e5.png)

- Data Observed for red color with indicator

![image](https://user-images.githubusercontent.com/38978733/207528997-535744e1-7ca0-44c7-ad19-3a095f8f53e2.png)

- Data observed for green color with indicator

![image](https://user-images.githubusercontent.com/38978733/207529289-cfb5a315-372f-4f0a-ab65-10778c363aea.png)

- Data observed for blue color with indicator

![image](https://user-images.githubusercontent.com/38978733/207529589-c66b046a-30eb-4610-b4d3-2e9c79472024.png)

- With this data after rewriting our code and logic we realized we needed a much stronger power source and that we needed the APDS to settle for some time with the light source before it can detect some change in the color. For this purpose we simply added a 10 second long delay after the APDS is configured before the start of the reaction. After making relevant changes we tested it incrementally using basic colored paper and seeing how it interacted. Once we achieved some consistent outputs we tested with water and introduced color with a colored liquid. For our tests we used some leftover Mountain Dew.

[![Trial with mountain dew](https://res.cloudinary.com/marcomontalbano/image/upload/v1672447172/video_to_markdown/images/youtube--7M5td8tMtU4-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/7M5td8tMtU4 "Trial with mountain dew")

- After achieving consistency with the Mountain Dew setup we shifted to the actual reaction.
- The chemical reaction we selected was dilute Citric Acid (C₆H₈O₇) as the acid, dilute Sodium Hydroxide (NaOH) as the base and Phenopthalein as the indicator. This is a somewhat safer reaction to perform outside the safety of a proper laboratory. Their concentrations were 0.04M and 0.1 M respectively. After some fine-tuning and testing we achieved desired results. We had to again modify our code for this purpose, a significant change we made here is to stop the motor when it observes a minor color change and check if the color settles before triggerring the motor again. Eventually we got the RP2040 to perform as we needed it to.

[![Final Output](https://res.cloudinary.com/marcomontalbano/image/upload/v1672447203/video_to_markdown/images/youtube--uNyA_uMM8rk-c05b58ac6eb4c4700831b2b3070cd403.jpg)](https://youtu.be/uNyA_uMM8rk "Final Output")

# Conclusion
We have automated and controlled titration of Citric acid against Sodium Hydroxide. Considering our program, it can detect most color changes, however it does have it flaws. We used a magnetic stirrer so that the color settles or dissipates faster. It also needs an appreciable difference in the color that the solution turns to. For future scope we could add a stopwatch or timer to measure the duration of the reaction. Considering the large amount of distance the liquid drawn creates on the burette, we dropped our initial idea to photograph the start and end-points. We can implement temperature sensing and controlling more complex reactions. As it stands we have implemented controlling a burette and sensing color. Both these functionalities are implemented with PIO in the RP2040 QtPy.



