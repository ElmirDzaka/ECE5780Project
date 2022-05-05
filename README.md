# Two-Wheel Balancing Robot (Inverted Pendulum) 

## Overview
This project combines an accelerometer and gyroscope with an STM32 microcontroller to implement an inverted pendulum in the form of a balancing robot. The robot is implemented by taking in accelerometer and gyroscope values continuously based on a sampling rate. The sensor values are generated from an external MPU chip that contains a gyroscope and accelerometer. This chip interfaces with a STM32 microcontroller using I2C protocol. This protocal ensures that values are from the MPU are sent to the at the proper time that the STM32 asks for. 

Once the values are recieved, the STM32 sends the values to a PID class that rotates the motors (wheels) of the robot based on the values from the MPU. The values recieved from the gyroscope determine the angle of the robot while the values recieved from the accelerometer determine the rate of acceeleration of the robot. These values combined help the motors keep the robot stabalized. A diagram showing a brief overview of the project can be seen below. For a more detailed look through the project, check out the Instructions sections for further information.

//diagram here

## Purpose
The purpose of the project is to learn how to use I2C and PIDs by integrating an external MPU chip containing a gyroscope and accelerometer sensor. Learning I2C protocol is done by interfacing an external MPU containing two sensors with an STM32F0 microcontroller. Learning PIDs is done by learning PID and ADC logic so that it can be adapted for this project. combining these learning objectives together allows for the construction of a balancing robot if implemented correctly.

## Functionality

The functionality of the robot is a bit awkward with the materials used in this project. In it's current state, the robot takes in values from an MPU that sits on a balanced frame. Once moving, the robot tries to stay balanced as it is moving forward. With the given materials, there is a small limitation to the robot. This project uses two custom motor drivers that were designed for an encoded Pollolu Motor. The motor drivers take in a wired barrel connection from an AC adapter, limiting the robot to a wired connection. If you are interested in designing this robot with a wireless/barrel-less power connection (power from STM32F0, AD2, etc via breadboard), then consider using a standard L298N motor driver [such as this one](https://www.amazon.com/Qunqi-Controller-Module-Stepper-Arduino/dp/B014KMHSW6/ref=asc_df_B014KMHSW6/?tag=hyprod-20&linkCode=df0&hvadid=167139094796&hvpos=&hvnetw=g&hvrand=3649658821866149140&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9029767&hvtargid=pla-306436938191&psc=1). This motor driver can be wired via breadboard, minimizing wires and clutter, as well as having the capability of connecting with other motors. More ideas can be found in the Additional Ideas section.


## Base Requirements Implemented

### GPIO
* GPIO pins are a vital part of integrating the STM32 with all the other hardware components in the project. The GPIO hardware abstraction library (HAL) allows for the interface of device peripherals from the STM32F0. The peripherals allow for the manipulation of hardware. In this project, we require the GPIO HAL libraries to set up the clock and pins of the STM32, as well as configuring the I2C protocol for the STM32F0. The manipulation of the GPIO ports allows the STM32F0 to know what it needs to do. For a more detailed look through what GPIO options the STM32F0 has to offer, a detailed datasheet can be found  [here](https://www.st.com/content/ccc/resource/technical/document/reference_manual/c2/f8/8a/f2/18/e6/43/96/DM00031936.pdf/files/DM00031936.pdf/jcr:content/translations/en.DM00031936.pdf).

### I2C 
* I2C protocol is critical to having the robot work. I2C allows the STM32F0 to act as a "master" device that takes in values from a "slave" device, which in this case is the MPU. I2C is configurable using the high-level interface provided by the I2C peripheral in the STM32F0 device peripherals. The STM32F0 device peripheral mitigates the bus complexity of the I2C protocol by accessing and configuirng registers to do what you need. The I2C protocol uses a series of *frames* and conditions to define data transfer. The two types of framess are address frames and data frames. How they are used in this project is gone more in-depth in their respective sub-sections. To do the data transfer, the protocol requres two lines; an SCL line and SDA line. The SDA line transfers the sensor data from the MPU to the STM32F0, while the SCL line line ensures that the data is sent while the master's clock is low. The MPU collects or *samples* the data during the rising edge of the master's clock. 
  * Address Frame: The address frame is initiated with a start condition which pulls the SDA line low, while keeping the SCL line high. This notifies the MPU (slave) that a transmission is about to begin. After this, the addressing bits of the I2C addresses (7 bits usually) and read/write bit are transmitted. This indicated the addresses that are being accessed, along with a bit that says whether or not the STM32F0 is reading or writing from the MPU. The final bit is the *acknowledgement* (ACK) bit, which allows the MPU to turn the SDA line hihg and wait for more instructions from the STM32F0.
  * Data Frame: The data frame contains the data based on the number of bytes needed from the MPU. the MPU follows the SCL clock and read/write bit to send data bytes based on the STM32F0's request. After data transfer is complete, the STM32F0 generates a *stop condition* and signals the release of the bus by transitioning the SDA line. 

### Motor and Analog
* The motor lab is...
* The motor lab includes the analog lab since it uses an ADC (Analog to Digital Converter)...  




## Instructions

An in-depth instruction guide that follows the construction of the project can be seen in the sub-headings below.

### Materials

* [Wheels](https://www.amazon.com/dp/B087Q9WM66?psc=1&ref=ppx_yo2ov_dt_b_product_details)
* [MPU6050 containing gyroscope and accelerometer sensors](https://www.amazon.com/dp/B00LP25V1A?psc=1&ref=ppx_yo2ov_dt_b_product_details)
* 2x [AC Barrel Jack Adaptor 6V 2A](https://www.amazon.com/dp/B00LU8Z9JI?psc=1&ref=ppx_yo2ov_dt_b_product_details)
* [Plexiglass Sheets](https://www.amazon.com/dp/B08NPMVJGQ?psc=1&ref=ppx_yo2ov_dt_b_product_details)
* [Plexiglass Pipes](https://www.amazon.com/dp/B09D7YFSV3?psc=1&ref=ppx_yo2ov_dt_b_product_details)
* [Saw For Cutting Pipes](https://www.amazon.com/WORX-WORXSAW-4-1-Compact-Circular/dp/B00ZFR4GJE/ref=asc_df_B00ZFR4GJE/?tag=hyprod-20&linkCode=df0&hvadid=309807921328&hvpos=&hvnetw=g&hvrand=12350142610661096784&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9029767&hvtargid=pla-405661640088&th=1)
* [Superglue For Plexiglass](https://www.amazon.com/Gorilla-105801-Super-1-Pack-Clear/dp/B08QR2XRKD/ref=sr_1_4?crid=D4VF64VD4OOD&keywords=super+glue&qid=1651690474&s=hi&sprefix=super%2Ctools%2C143&sr=1-4)
* 2x [Pollolu 47:1 Gear Ratio Motor](https://www.pololu.com/product/4885)
* [Adaptors To Fit Motors Wth 12mm Wheels](https://www.pololu.com/product/2684)
* [Solder Kit For MPU6050](https://www.amazon.com/Soldering-Iron-Kit-Temperature-Desoldering/dp/B07S61WT16/ref=sr_1_5?crid=VAXD6HT0JNPP&keywords=soldering+iron&qid=1651690658&sprefix=solder%2Caps%2C104&sr=8-5)
* [Analog Discovery 2](https://www.amazon.com/Digilent-Discovery-Oscilloscope-Analyzer-Variable/dp/B018OPOQOS/ref=asc_df_B018OPOQOS/?tag=hyprod-20&linkCode=df0&hvadid=344057338224&hvpos=&hvnetw=g&hvrand=486485434278545132&hvpone=&hvptwo=&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9029767&hvtargid=pla-522977565083&psc=1&tag=&ref=&adgrpid=70729707553&hvpone=&hvptwo=&hvadid=344057338224&hvpos=&hvnetw=g&hvrand=486485434278545132&hvqmt=&hvdev=c&hvdvcmdl=&hvlocint=&hvlocphy=9029767&hvtargid=pla-522977565083)
* [Breadboard(s)](https://www.amazon.com/Breadboards-Solderless-Breadboard-Distribution-Connecting/dp/B07DL13RZH/ref=sr_1_2_sspa?keywords=breadboard&qid=1651690756&s=industrial&sprefix=bread%2Cindustrial%2C102&sr=1-2-spons&psc=1&smid=AX8SR0V05IQ2E&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExTU43QldUR1c2Skk4JmVuY3J5cHRlZElkPUEwOTY0Mjc3M0I0RU8yNlFIS1E1SiZlbmNyeXB0ZWRBZElkPUEwNzgyNTA2Mzk3RkFTTTg1Qk9INSZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU=)
* [Jumper Wires For Breadboard](https://www.amazon.com/EDGELEC-Breadboard-Optional-Assorted-Multicolored/dp/B07GD2BWPY/ref=sr_1_1_sspa?crid=JHEAJD2SKMHB&keywords=jumper%2Bwires&qid=1651690789&s=industrial&sprefix=jump%2Cindustrial%2C107&sr=1-1-spons&spLa=ZW5jcnlwdGVkUXVhbGlmaWVyPUExVk9MNjBTVEJQSUhCJmVuY3J5cHRlZElkPUEwMzk2MTU0MzA1MkpaVUlPSEhDUSZlbmNyeXB0ZWRBZElkPUEwOTQ1NDM2MUpBN1RMSkJGREFMWiZ3aWRnZXROYW1lPXNwX2F0ZiZhY3Rpb249Y2xpY2tSZWRpcmVjdCZkb05vdExvZ0NsaWNrPXRydWU&th=1)


### MPU6050 Initialization

The MPU6050 is a 3-axis MPU containing both accelerometer and gyroscope sensors. The values from the sensors are passed to the STM32F0 microcontroller via I2C as described in the I2C section above. Before that can happen, initialization of the MPU6050 is required. To accomplish this, two datasheets need to be examined thoroughly to figure out everything that is needed. The [registers datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf) and [device datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) can be found here. This device is much different to initialize compared to the gyroscope on the STM32F0 board, because the initialization process requires more steps than the gyroscope sensor built-in to the STM32F0. The detailed explanation with code is shown below:

* I2C Initialization
  * ddd 
* Reading Who_am_I Address 
* Waking Up MPU Device
* Setting Sampling Rate of Device Sensors
* Initializing Gyroscope and Accelerometer

```
command to run if program contains helper info
```

```
code blocks for commands
```

### Reading and Sending Sensor Values


<img width="300" alt="initialize" src="https://user-images.githubusercontent.com/43626153/166521463-8f9dc8c4-467b-4fcd-9ee5-442a0a43e53b.png">
<img width="300" alt="read values" src="https://user-images.githubusercontent.com/43626153/166521635-eb9770c8-9775-45fb-9cb3-08797ee0931f.png">



### Configuring PID For Motors

### Making Robot Frame

### Combining Components Together

## Milestones

Explain how it was changed during the modeling process of the project

* Milestone 1: Getting Who_am_I from MPU6050
* Milestone 2: Read Values from MPU6050
* Milestone 3: Get Motors Working Using Correct PID
* Milestone 4: Assemble with Robot Frame

## Author

Elmir Dzaka: (Linkedin?)

## Additional Ideas 



## Acknowledgments
