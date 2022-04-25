/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : I2C.c
  * @brief          : File Intializes and configures I2C protocol
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */

/* Includes --------------------------------------*/
#include <math.h>
#include "I2C.h"
#include "main.h"






#define RAD_TO_DEG 57.295779513082320876798154814105

#define WHO_AM_I_REG 0x75
#define PWR_MGMT_1_REG 0x6B
#define SMPLRT_DIV_REG 0x19
#define ACCEL_CONFIG_REG 0x1C
#define ACCEL_XOUT_H_REG 0x3B
#define TEMP_OUT_H_REG 0x41
#define GYRO_CONFIG_REG 0x1B
#define GYRO_XOUT_H_REG 0x43
#define GYRO_XOUT_L_REG 0x44

// Setup MPU6050
#define MPU6050_ADDR 0x68
const uint16_t i2c_timeout = 100;
const double Accel_Z_corrector = 14418.0;

uint32_t timer;





//I2C_HandleTypeDef hi2c2;

void I2C2_init(void){
	
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  /* USER CODE BEGIN 2 */
	GPIOB->MODER |= (1<<23) | (1 <<27) | (1<<28) ; //Pin PB11 and PB13 and PB14 (output mode)
	GPIOC->MODER |= (1<<0); //PC0 (output mode)
	GPIOB->OTYPER |= (1<<11) | (1<<13); //open drain PB11 and PB13
	GPIOB->AFR[1] |=  (1<<12)|(1<<20) | (1<<22); //ISC2_SDA
	I2C2 ->TIMINGR |= 0x10420F13; //bus timing 
	I2C2 ->CR1 |= 1; //PE bit
	
	//initialize PB14 and PC0 to high
	HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14, GPIO_PIN_SET); 
	HAL_GPIO_WritePin(GPIOC,GPIO_PIN_0, GPIO_PIN_SET);
	
	// setting up leds
  GPIO_InitTypeDef init = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7, 
                                                             GPIO_MODE_OUTPUT_PP,
                                                             GPIO_SPEED_FREQ_LOW,
                                                             GPIO_NOPULL};
		
	HAL_GPIO_Init(GPIOC, &init); // Initialize leds
																														 
	GPIO_InitTypeDef init2 = {GPIO_PIN_4, GPIO_MODE_INPUT, GPIO_MODE_ANALOG,GPIO_SPEED_FREQ_LOW,GPIO_NOPULL};
	
	HAL_GPIO_Init(GPIOA, &init2);
	
}

void who_am_i(void){
	
	//I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
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
	
	//Write the address of the “WHO_AM_I” register into the I2C transmit register. (TXDR)
	I2C2->TXDR = WHO_AM_I_REG; 
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
			break; 
		}
		
	}
	
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PB14 high
	
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
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error
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
	
	
};


//wake up sensors and set clock to 
void wake_up_mpu (void){
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}
	
	//write to power reg to enable clock and wake up
	I2C2->TXDR =  PWR_MGMT_1_REG;//control reg from MPU
	
	
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
				break; 
		}
	}
	
	
	//set value to to wake up, clock=8MHZ
	I2C2->TXDR =  0x00; 
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}
		
	
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
	
	
};

//sets sampling rate of MPU6050
void sample_rate(void){
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}
	
	//write to sampling reg to divide 8Mhz clock by 8 to get 1kHZ
	I2C2->TXDR =  SMPLRT_DIV_REG;//control reg from MPU
	
	
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
				break; 
		}
	}
	
	
	//set sampling rate to 1kHZ
	I2C2->TXDR =  0x07; 
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}
		
	
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
	
	
	
}



void gyro_Init(void){
	
	
//	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}
	
	
	
	//enable gyro config register  from MPU
	I2C2->TXDR =  GYRO_CONFIG_REG;//control reg from MPU
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
				break; 
		}
	}
	
	
	//set range to 
	I2C2->TXDR =  0x00; // sets +- 250 degrees/s for gyro
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}

		
	
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
	
	

};

void read_gyro(MPU6050_t *Datastruct){
	uint8_t Rec_Data[2]; //read 2 bytes of data from Gyro_Xout_H register (only need x axis)
	
	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}
	
	//go to gyro read reg
	I2C2->TXDR =  GYRO_XOUT_H_REG;//control reg from MPU
	
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}

	
	//read high gyro reg
	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (2<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 |= (1<<10); //READ_WRN to READ
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	
	
	// Wait until either of the RXNE (Recieve Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}

	
	//get higher 8 bits
	Rec_Data[0] = I2C2->RXDR;
	
	
	// Wait until either of the RXNE (Recieve Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}

	
	
	
	//get lower 8 bits
	Rec_Data[1] = I2C2->RXDR;


	
	//combine high bits and low bits of gyroscope H/L registers to get full value
	Datastruct ->Gyro_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
	
	//convert gyroscope degrees  to (250 degrees/s) since sensitivity is 131 LSB/degree/s
	Datastruct->Gx = Datastruct->Gyro_X_RAW / 131.0;
	
	//delay just in case
	HAL_Delay(100);
	
}


void accel_Init(void){
	
		I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
		I2C2->CR2 &= ~(0x400); //READ_WRN to write
		I2C2->CR2 |= (1 << 13);	// Start Bit 
	
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
		while(1){
			if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
				break; 
			}
		}
		
	
		//NACKF flag
		if(I2C2->ISR & (1 << 4)){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
		}
	
		//enable gyro config register  from MPU
		I2C2->TXDR = ACCEL_CONFIG_REG;//control reg from MPU
		
		
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
		while(1){
			if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
					break; 
			}
		}
		
		//set range to 
		I2C2->TXDR =  0x00; // sets +- 2g degrees/s for gyro
	
	
	
};

void read_accel(MPU6050_t *Datastruct){
	uint8_t Rec_Data[2]; //read 2 bytes of data from accel_Xout_H register (only need x axis)
	
	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (1<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 &= ~(0x400); //READ_WRN to write
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	//combine high bits and low bits of acclerometer H/L registers to get full value
		
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}
	
	//go to gyro read reg
	I2C2->TXDR =  ACCEL_XOUT_H_REG;//control reg from MPU
	
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}

	
	//read high gyro reg
	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (2<<16) | (MPU6050_ADDR << 1); //set NBYTES and Slave ADDress of MPU6050
	I2C2->CR2 |= (1<<10); //READ_WRN to READ
	I2C2->CR2 |= (1 << 13);	// Start Bit 
	
	
	
	// Wait until either of the RXNE (Recieve Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}

	
	//get higher 8 bits
	Rec_Data[0] = I2C2->RXDR;
	
	
	// Wait until either of the RXNE (Recieve Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//NACKF flag
	if(I2C2->ISR & (1 << 4)){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); //error red
	}

	
	
	
	//get lower 8 bits
	Rec_Data[1] = I2C2->RXDR;


	
	//combine high bits and low bits of gyroscope H/L registers to get full value
	Datastruct ->Accel_X_RAW = (int16_t)(Rec_Data[0] << 8 | Rec_Data[1]);
	
	//convert accel since sensitivity is 16384 LSB/g
	Datastruct->Ax = Datastruct->Accel_X_RAW / 16384.0;
	
	//delay just in case
	HAL_Delay(100);
}
