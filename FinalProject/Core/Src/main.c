/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
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
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */
void WHO_AM_I(void);
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
//x
uint8_t byte1;
uint8_t byte2;
int16_t x;
int16_t x_dir = 0;

//y
uint8_t y_byte1;
uint8_t y_byte2;
int16_t y;
int16_t y_dir = 0;

/* USER CODE BEGIN PV */
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();
	__HAL_RCC_GPIOC_CLK_ENABLE(); // Enable the GPIOC clock in the RCC
  __HAL_RCC_GPIOB_CLK_ENABLE(); // Enable the GPIOB clock in the RCC
	__HAL_RCC_I2C2_CLK_ENABLE();
	
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();
	
	// Set LEDS
	GPIO_InitTypeDef initStr1 = {GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_6 | GPIO_PIN_7 | GPIO_PIN_0,
															 GPIO_MODE_OUTPUT_PP,
															 GPIO_SPEED_FREQ_LOW,
															 GPIO_NOPULL};
	
	HAL_GPIO_Init(GPIOC, &initStr1); // Initialize pins 
	
	// Setting the GPIO Modes:  
	GPIOB->OTYPER = (1 << 11) | (1 << 13);	// Open Drain on PB11 and PB13
	GPIOB->MODER = (1 << 23) | (1 << 27) | (1 << 28);	// Mode 
	GPIOB->AFR[1] = (1 << 12) | (1 << 22) | (1 << 20); 
	
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_14, GPIO_PIN_SET); // Start PB14 high
	HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, GPIO_PIN_SET); // Start PC0 high
	
	// Initializing the I2C Peripheral
	I2C2->TIMINGR = 0x10420F13;
	I2C2->CR1 = 1; // PE bit
	
	// Part1: WHO AM I Function
	//WHO_AM_I();
	//while (1){}
			
	// Set the STOP bit in the CR2 register to release the I2C bus
	//I2C2-> CR2 |= (1 << 14);
															 
	// Initializing Gryoscope: 
	
	// Enable the X and Y sensing axes in the CTRL_REG1 register
	I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
	I2C2->CR2 |= (2 << 16) | (0x69 << 1);		// Setting NBytes and Slave Address
	I2C2->CR2 &= ~(0x400);  // Clear RD_WRN to write transfer
	I2C2->CR2 |= (1 << 13);	// Start Bit 

	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//Write the address of the CN register into the I2C transmit register. (TXDR)
	I2C2->TXDR = 0x20; 
	
	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
				break; 
		}
	}
	
	I2C2->TXDR = 0x0B;
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
				break; 
		}
	}

		
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
	
	while (1){		
		
		
		///////////////// Starting X axis /////////////////
		
		// Enable the X and Y sensing axes in the CTRL_REG1 register
		I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
		I2C2->CR2 |= (1 << 16) | (0x69 << 1);		// Setting NBytes and Slave Address
		I2C2->CR2 &= ~(0x400);  // Clear RD_WRN to write transfer
		I2C2->CR2 |= (1 << 13);	// Start Bit 
		
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
		while(1){
			
			// TXIS
			if((I2C2->ISR & (1 << 1))){
				I2C2->TXDR = 0xA8;
				break;
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			}
			
		}
		
		//Wait until the TC (Transfer Complete) flag is set
		while(1){
			if((I2C2->ISR & (1 << 6))){
				break; 
			}
		}
		
		
		// Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to
		// indicate a read operation.	
	
		I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
		I2C2->CR2 |= (2 << 16) | (0x69 << 1) | (1 << 10);		
		I2C2->CR2 |= (1 << 13); // Start Bit
		
		// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
		while (1)
		{
			// RXNE 
			if((I2C2->ISR & (1 << 2))){
				byte1 = I2C2->RXDR; 
				break; 
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); 
			}
			
		}
		
		// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
		while (1)
		{
			
			// RXNE
			if((I2C2->ISR & (1 << 2))){
				byte2 = I2C2->RXDR; 
				break; 
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); 
			}
			
		}
	
		//Wait until the TC (Transfer Complete) flag is set
		while(1){
			if((I2C2->ISR & (1 << 6))){
				break; 
			}
		}
		
		// Set the STOP bit in the CR2 register to release the I2C bus
		I2C2-> CR2 |= (1 << 14);
		
		x = (byte2 << 8) | byte1;  
		
		x_dir += x;
		
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
		HAL_Delay(100);
		
		///////////////// Starting Y axis /////////////////
		
		// Enable the X and Y sensing axes in the CTRL_REG1 register
		I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
		I2C2->CR2 |= (1 << 16) | (0x69 << 1);		// Setting NBytes and Slave Address
		I2C2->CR2 &= ~(0x400);  // Clear RD_WRN to write transfer
		I2C2->CR2 |= (1 << 13);	// Start Bit 
		
		// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
		while(1){
			
			// TXIS
			if((I2C2->ISR & (1 << 1))){
				I2C2->TXDR = 0xAA;
				break;
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET);
			}
			
		}
		
		//Wait until the TC (Transfer Complete) flag is set
		while(1){
			if((I2C2->ISR & (1 << 6))){
				break; 
			}
		}
		
		
		// Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to
		// indicate a read operation.	
	
		I2C2->CR2 &= ~((0x7F << 16) | (0x3FF << 0));
		I2C2->CR2 |= (2 << 16) | (0x69 << 1) | (1 << 10);		
		I2C2->CR2 |= (1 << 13); // Start Bit
		
		// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
		while (1)
		{
			// RXNE 
			if((I2C2->ISR & (1 << 2))){
				y_byte1 = I2C2->RXDR; 
				break; 
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); 
			}
			
		}
		
		// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
		while (1)
		{
			
			// RXNE
			if((I2C2->ISR & (1 << 2))){
				y_byte2 = I2C2->RXDR; 
				break; 
			}
			
			// NACKF
			if((I2C2->ISR & (1 << 4))){
				HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); 
			}
			
		}
	
		//Wait until the TC (Transfer Complete) flag is set
		while(1){
			if((I2C2->ISR & (1 << 6))){
				break; 
			}
		}
		
		// Set the STOP bit in the CR2 register to release the I2C bus
		I2C2-> CR2 |= (1 << 14);
		
		y = (y_byte2 << 8) | y_byte1;  
		
		y_dir += y;
		
		HAL_Delay(100);
		
		// X axis 
		if(x_dir < 0){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_RESET);  
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_SET); 
		}
		else{ 
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_9, GPIO_PIN_RESET); 
		}
		
		// Y axis 
		if(y_dir < 0){
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); 
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_RESET); 
		}
		else{
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_RESET); 
			HAL_GPIO_WritePin(GPIOC, GPIO_PIN_7, GPIO_PIN_SET); 
		}
	
	}
	
//	while(1){}
		
}


void WHO_AM_I(void){
	
	// Reading the Register 
	// 1. Set the L3GD20 slave address = 0x69
	I2C2->CR2 |= (1 << 16) | (0x69 << 1);		
	I2C2->CR2 &= ~(0x400);
	I2C2->CR2 |= (1 << 13);	// Start Bit 														 
	

	// Wait until either of the TXIS (Transmit Register Empty/Ready) or NACKF (Slave NotAcknowledge) flags are set.
	while(1){
		if((I2C2->ISR & (1 << 1)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
	}
	
	//Txis flag
	if(I2C2->ISR & (1 << 1)){
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); 
	}
	
	//Write the address of the ?WHO_AM_I? register into the I2C transmit register. (TXDR)
	I2C2->TXDR = 0x0F; 
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
			break; 
		}
	}
	
	//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_6, GPIO_PIN_SET); // Start PB14 high
	
	// Reload the CR2 register with the same parameters as before, but set the RD_WRN bit to
  // indicate a read operation.
	//I2C2->CR2 |= 0;		
	I2C2->CR2 |= (1 << 16) | (0x69 << 1) | (1 << 10);		
	I2C2->CR2 |= (1 << 13); // Start Bit
	
	// Wait until either of the RXNE (Receive Register Not Empty) or NACKF (Slave NotAcknowledge) flags are set.
	while (1)
  {
		if((I2C2->ISR & (1 << 2)) | (I2C2->ISR & (1 << 4))){
			break; 
		}
  }
	
	if(I2C2->ISR & (1 << 2)){
		//HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET); 
	}
	
	//Wait until the TC (Transfer Complete) flag is set
	while(1){
		if((I2C2->ISR & (1 << 6))){
			break; 
		}
	}
	
	// Check the contents of the RXDR register to see if it matches 0xD4. (expected value of the ?WHO_AM_I? register)
	if(I2C2->RXDR == 0xD3){
		HAL_GPIO_WritePin(GPIOC, GPIO_PIN_8, GPIO_PIN_SET);
	}
	
	// Set the STOP bit in the CR2 register to release the I2C bus
	I2C2-> CR2 |= (1 << 14);
	
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
