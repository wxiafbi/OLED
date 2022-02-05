/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    usart.h
  * @brief   This file contains all the function prototypes for
  *          the usart.c file
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ATH10_H__
#define __ATH10_H__

#ifdef __cplusplus
extern "C"
{
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

    /* USER CODE BEGIN Includes */

    /* USER CODE END Includes */

    extern UART_HandleTypeDef huart1;

/* USER CODE BEGIN Private defines */
#define AHT10AddWr 0x70 //AHT10写数据地址
#define AHT10AddRd 0x71 //AHT10读数据地址
//extern uint8_t Init_cmd1[4] = {0x70, 0xe1, 0x08, 0x00};
/* USER CODE END Private defines */



/* USER CODE BEGIN Prototypes */
void AHT10_Reset(void);
void AHT10_ReadData(void);
char AHT10_ReadStatus(void);
char AHT10_Read_CalEnable(void); 
char AHT10_Init(void);
void AHT10_Data(double *temp, double *humi);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __USART_H__ */
