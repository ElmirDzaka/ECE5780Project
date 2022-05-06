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

The MPU6050 is a 3-axis MPU containing both accelerometer and gyroscope sensors. The values from the sensors are passed to the STM32F0 microcontroller via I2C as described in the I2C section above. Before that can happen, initialization of the MPU6050 is required. To accomplish this, two datasheets need to be examined thoroughly to figure out everything that is needed. The [registers datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Register-Map1.pdf) and [device datasheet](https://invensense.tdk.com/wp-content/uploads/2015/02/MPU-6000-Datasheet1.pdf) can be found here. The MPU6050 is shown below for reference.

![Screenshot 2022-05-05 145914](https://user-images.githubusercontent.com/43626153/167024670-60cd0ab5-df69-4b45-ae00-3a4c6e5a40e9.jpg)

Once recieving the MPU6050, it is required to solder the pins to effectively use the device. The pins can be attatched to a breadboard for easier maniupulation. As seen in the datasheet and picuture, the MPU has a ground and VCC pin that can take in either 3.3V or 5V. Once powered, the MPU will light up a green LED showing that it is working properly. The MPU also has an SDA and SCL pin so that it can be connected to another device, such as the STM32F0. This device is much different to initialize compared to the gyroscope on the STM32F0 board, because the initialization process requires more steps than the gyroscope sensor built-in to the STM32F0. The detailed explanation with code is shown below:

### I2C Initialization
   We have to start by initializing the I2C protocol on the STM32F0. to do this, we just set the pins and bus timing according to the specfications of the datasheet. The code for setting the protocol with the STM32F0 board used can be seen below:

```
//initialize pins
GPIOB->MODER |= (1<<23) | (1 <<27) | (1<<28) ; //Pin PB11 and PB13 and PB14 (output mode)
GPIOC->MODER |= (1<<0); //PC0 (output mode)
GPIOB->OTYPER |= (1<<11) | (1<<13); //open drain PB11 and PB13
GPIOB->AFR[1] |=  (1<<12)|(1<<20) | (1<<22); //ISC2_SDA
I2C2 ->TIMINGR |= 0x10420F13; //bus timing 
I2C2 ->CR1 |= 1; //PE bit
	
//initialize PB14 and PC0 to high
HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, GPIO_PIN_SET); 
HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0, GPIO_PIN_SET);
	
```

In the I2C.c file, I also choose to initialize LEDs in this step, but that isn't necessary. LEDs help debug the code to ensure that the values have been sent and recieved properly. This function is critical since it ensures that the the two devices can communicate properly.

### Reading Who_am_I Address

Reading the who_am_I address from the MPU6050 is critical to knowing if the STM32F0 recognizes the device, and that I2C was set up properly. To do this, we just follow the steps that were outlined in the I2C method above. By looking at the I2C registers in the STM32F0 datasheet, we see that control register 2 (CR2) allows for setting the amount of bytes we want going through the data line, as well as setting the address of the "slave" (MPU6050). After that, we have to indicate whether we are reading or writing from the slave address. Lastly, we initiate the I2C transfer by setting the start bit. We need to axess a specifix address in the MPU6050 (who_am_I address), so we need to write to that address first before we check the contents of the address. This similar process is repeated in every function to initiate data transfer. The code for this is seen below: 
  
```
I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave Address of MPU6050
I2C2->CR2 &= ~(0x400); //READ_WRN to write
I2C2->CR2 |= (1 << 13);	// Start Bit 
```
   After the transaction starts, we need to check if either the Transmit Register or Recieve Register is empty depending on if we are reading or writing from the STM32F0. In this case, we need to write to the who_am_I address first, so we will check the transmit register. We also need to check the "slave" not acknowledged register (NACKF) to see if the MPU6050 is still hooked up properly, and to ensure that we are still following I2C protocol. The best way to check is to light up an LED based on which register is triggered at a conditional. The code for that is shown below:
  
  ```
 // Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); 
	}
 ```
  If the TXIS register is empty, that means it is ready to write to an address. Now, we look at the STM32F0 datasheet and see that we write to the TXDR register in the STM32F0. This is done with the code below: DISCLAIMER: I defined most registers from the MPU6050 as variables, the respective variables and their addresses can be seen in the I2C.c file.
 
  ```
//Write the address of the “WHO_AM_I” register into the I2C transmit register. (TXDR)
	I2C2->TXDR = WHO_AM_I_REG; 
 ```
  Now, the two devices have to wait until data transfer is complete, and once it is, we can check if the transfer complete (TC) register. This register will be set high once the transfer is complete. The code for that is seen below:
 
```
 //Wait until the TC (Transfer Complete) flag is set
 while(1){
	 if((I2C2->ISR & (1 << 6))){
			break; 
	 }
		
 }
 ```
  Now that we are at the address we want (who_am_I address), we can check the contents to see if they match the contents listed in the MPU6050 product datasheet. We can do this by initiating a read operation for the STM32F0. We initiate a transfer much like the write operation above, but we select a read operation instead. This is seen below:
 
```
// Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to
// indicate a read operation.
I2C2->CR2 |= (1 << 16) | (MPU6050_ADDR << 1) | (1 << 10);		
I2C2->CR2 |= (1<<10); //read	
I2C2->CR2 |= (1 << 13); // Start Bit
```
 Now, all we do is check the receive register and wait until data transfer (TC) is complete before reading the results. Unlike writing, we have to wait for data to be fully read before doing an action, while writing requires doing an action and then waiting for the TC register. After that, we check to see if the contents match the content value listed in the who_am_I register in the datasheet. After we are done with this function, we have to set the STOP bit to ensure the I2C bus is released for another operation. The code is shown below:
 
 ```
// Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to
  // indicate a read operation.
	I2C2->CR2 |= (1 << 16) | (MPU6050_ADDR << 1) | (1 << 10);		
	I2C2->CR2 |= (1<<10); //read	
	I2C2->CR2 |= (1 << 13); // Start Bit
	
	// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
	while (1)
  {
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
  }
	
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); //error
	}
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
			break; 
		}
	}
	
	// Check the contents of the RXDR register to see if it matches 0x68. (expected value of the “WHO_AM_I” register)
	if(I2C2->RXDR == 0x68){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); //works
	}
	
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
```
 
  For the rest of the instructions, the I2C protocol won't be gone in depth as it was in the who_am_I section since it all follows the same protocol. 
  
### Waking Up MPU Device
When looking at the MPU6050 product datasheet, it says that the MPU6050 requires setting some bits in the PWR_MGMT register to "wake up" the sensors. These bits will enable the internal clock of the device. Once we write to this register, we need to write some bits inside the register to set up the internal clock to 8 Mhz. The process is very similar to the code above, and the full code can be seen in the "wake_up_mpu" function in the I2C.c file. 

### Setting Sampling Rate of Device Sensors
Next, the sampling rate of the sensors needs to be set. The sampling rate determines how often the sensors from the MPU collect data. In this case, this involes the gyroscope and accelerometer sensors. Since I decided to use an 8Mhz clock, I want to sample at a rate of about 1kHz. This is because the math is pretty easy. According to the device datasheet, we write an 0x07 to divide by 8, giving us a 1kHZ sampling rate. A more detailed explanation can be found on the register datasheet for the MPU6050.

### Initializing Gyroscope and Accelerometer
Before data values can be read, the gyroscope and accelerometer sensors need to be initialized. To do this, we need to write to either the GYRO_CONFIG register or the ACCEL_CONFIG register in their respective functions. The datasheet for the MPU6050 registers says that writing to each of these registers enables the sensors. After that, we need to write a value into the sensors to configure the type of output the sensor outputs. The datasheet goes into detail as to what each mode means. For this project, I decided to get raw values of +-250 degrees/s for the gyroscope and +-2g for the accelerometer. This was accomplished by writing a 0 into both registers. These values are just the "raw" values that the sensors generate. To get readable values, we need to do some math to change the values. This is done when readingthe values.



### Reading and Sending Sensor Values
Now that the initialization of the MPU6050 is done, the values can now be read. To check if the initialization worked, we can use an Analog Discovery 2 kit to connect the SCL and SDA lines from the STM32F0 to the AD2 (Important: we still need the SCL and SDA lines connected to the MPU6050, luckily the STM32F0 can connect two wires to the same pin). In the AD2, we can provide the MPU6050 with power using the "Supplies" setting, and we can check the data being received and sent using the "Protocol" section. In the Supplies section, we just want a positive supply of 3.3 volts. In the Protocol section, we want to change the protocol to I2C and set the DIO pins accordingly. Once everything is set properly, the values should look something like the image below (click image to enlarge): 

<img width="300" alt="initialize" src="https://user-images.githubusercontent.com/43626153/166521463-8f9dc8c4-467b-4fcd-9ee5-442a0a43e53b.png">

From here, we can start looking at the "read_gyro" and "read_accel" functions in the I2C.c file. To store the values from this function, I decided to make a struct in the I2C.h header file that stores the values generated from the sensors. The struct allows the main.c file to access the struct once it's declared, and read the values accordingly from the struct. Before that can be done, the functions have to be implemented first. To read the sensor values, we need to go to either the ACCEL_YOUT_H_REG or the  GYRO_YOUT_H_REG depending on the function. The sensors are configered to generate a 16-bit value, but since a register can only store a byte, the MPU6050 stores two registers for sensor data; the high register and the low register. The high register stores the high bits and vice versa. We need to start at the high register since it comes before the low register. If we set the I2C protocol to read 2 bytes (which we do), then the next byte it reads is the low register. full register information is found in the respective registers in the MPU6050 register datasheet. From here, we need to store the values in the struct to pass along to the main.c function. before we do that, we need to generate the interpreted value from the raw value. For our values, the datasheet says we need to divide the raw value by 131 for gyro and 16384 for accel. The code for passing the values into the struct can be seen below.

```
//combine high bits and low bits of accel H/L registers to get full value
mpu_struct ->Accel_Y = (int16_t)(data[0] << 8 | data[1]);
	
//convert accel since sensitivity is 16384 LSB/g
mpu_struct->Ay = mpu_struct->Accel_Y / 16384.0;
```

To read the data, we need to call these functions in the infinite while loop in the main.c function so that we continuously keep generating data. Once we do that, we can see something like the blow image in our AD2 device.

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
