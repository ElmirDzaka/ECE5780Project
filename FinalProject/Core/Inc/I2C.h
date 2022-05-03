/**
  ******************************************************************************
  * File Name          : I2C.h
  * Description        : This file provides code for the configuration
  *                      of the I2C instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
  /* Define to prevent recursive inclusion -------------------------------------*/
#ifndef I2C_H
#define I2C_H

//#ifdef __cplusplus
//extern "C" {
//#endif


	/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

// MPU6050 structure
typedef struct
{
	//ACCL values
    int16_t Accel_Y;
    double Ay;
    
	//Gyro values
    int16_t Gyro_Y;
    double Gy;
  

} MPU6050_t;


/* USER CODE END Includes */


	/* USER CODE BEGIN Private defines */

	/* USER CODE END Private defines */
	void who_am_i(void);
	void I2C2_init(void);
	void gyro_Init(void);
	void read_gyro(MPU6050_t *Datastruct);
	void accel_Init(void);
	void read_accel(MPU6050_t *Datastruct);
    void wake_up_mpu(void);
    void sample_rate(void);

	/* USER CODE BEGIN Prototypes */

	/* USER CODE END Prototypes */

#endif /*I2C_H */

//#ifdef __cplusplus
//}
//#endif
