/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "tim.h"
#include "../../lib/moto_control/moto_control.h"
#include "../../lib/key/key.h"
#include "../../lib/power/power.h"
#include "../../lib/oled/oled.h"
#include "../../lib/find_line/fine_line.h"
#include "../../lib/usart_openmv_stm32/usart_openmv_stm32.h"
#include "../../lib/led/led.h"
#include "usart.h"
#include "../../lib/encoder/encoder.h"

#include <string.h> 
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */


/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */
#define TWO_WHEELS		0
#define THREE_WHEELS	1
#define FOUR_WHEELS		2


#define WHEELS	THREE_WHEELS //�޸Ķ�Ӧ��ʵ�������л�

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SW1_Pin GPIO_PIN_0
#define SW1_GPIO_Port GPIOA
#define POWER_ADC_Pin GPIO_PIN_4
#define POWER_ADC_GPIO_Port GPIOA
#define AIN1_Pin GPIO_PIN_2
#define AIN1_GPIO_Port GPIOB
#define SPEED_IN1_Pin GPIO_PIN_12
#define SPEED_IN1_GPIO_Port GPIOB
#define SPEED_IN1_EXTI_IRQn EXTI15_10_IRQn
#define SPEED_IN2_Pin GPIO_PIN_13
#define SPEED_IN2_GPIO_Port GPIOB
#define SPEED_IN2_EXTI_IRQn EXTI15_10_IRQn
#define BIN1_Pin GPIO_PIN_8
#define BIN1_GPIO_Port GPIOC
#define BIN2_Pin GPIO_PIN_9
#define BIN2_GPIO_Port GPIOC
#define IIC2_SCL_Pin GPIO_PIN_11
#define IIC2_SCL_GPIO_Port GPIOA
#define IIC2_SDA_Pin GPIO_PIN_12
#define IIC2_SDA_GPIO_Port GPIOA
#define IIC1_SCL_Pin GPIO_PIN_11
#define IIC1_SCL_GPIO_Port GPIOC
#define IIC1_SDA_Pin GPIO_PIN_12
#define IIC1_SDA_GPIO_Port GPIOC
#define AIN2_Pin GPIO_PIN_3
#define AIN2_GPIO_Port GPIOB
#define PWMC_Pin GPIO_PIN_6
#define PWMC_GPIO_Port GPIOB
#define PWMD_Pin GPIO_PIN_7
#define PWMD_GPIO_Port GPIOB
#define PWMB_Pin GPIO_PIN_8
#define PWMB_GPIO_Port GPIOB
#define PWMA_Pin GPIO_PIN_9
#define PWMA_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
extern uint8_t car_speed_l,car_speed_r;
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
