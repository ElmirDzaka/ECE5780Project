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

* How to run the program
* Step-by-step bullets
```
code blocks for commands
```

### MPU6050

Any advise for common problems or issues.
```
command to run if program contains helper info
```

### I2C Implementation

<img width="300" alt="initialize" src="https://user-images.githubusercontent.com/43626153/166521463-8f9dc8c4-467b-4fcd-9ee5-442a0a43e53b.png">
<img width="300" alt="read values" src="https://user-images.githubusercontent.com/43626153/166521635-eb9770c8-9775-45fb-9cb3-08797ee0931f.png">



### Motors

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
