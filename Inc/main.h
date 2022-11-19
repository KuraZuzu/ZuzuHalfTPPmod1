/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
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
#include "stdio.h"
/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */
#include "arm_math.h"
/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */
extern void machineMeasureSpeed();
extern void machineRun(float32_t speed);
extern void testMeasureSpeed();
extern void testEncoder();
extern void testBatteryConsole();
extern void testConsoleDistSensor();
extern void machineBuzzer();
extern void testTimer();
/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define LS_ADC_Pin GPIO_PIN_0
#define LS_ADC_GPIO_Port GPIOC
#define RS_ADC_Pin GPIO_PIN_1
#define RS_ADC_GPIO_Port GPIOC
#define RF_ADC_Pin GPIO_PIN_2
#define RF_ADC_GPIO_Port GPIOC
#define UI_LED1_Pin GPIO_PIN_3
#define UI_LED1_GPIO_Port GPIOC
#define Gyro_SPI_cs_Pin GPIO_PIN_4
#define Gyro_SPI_cs_GPIO_Port GPIOA
#define Motor1_Phase_Pin GPIO_PIN_6
#define Motor1_Phase_GPIO_Port GPIOA
#define Motor2_Phase_Pin GPIO_PIN_7
#define Motor2_Phase_GPIO_Port GPIOA
#define UI_LED2_Pin GPIO_PIN_4
#define UI_LED2_GPIO_Port GPIOC
#define UI_LED3_Pin GPIO_PIN_5
#define UI_LED3_GPIO_Port GPIOC
#define LF_ADC_Pin GPIO_PIN_0
#define LF_ADC_GPIO_Port GPIOB
#define Battery_ADC_Pin GPIO_PIN_1
#define Battery_ADC_GPIO_Port GPIOB
#define DIST_LED4_Pin GPIO_PIN_2
#define DIST_LED4_GPIO_Port GPIOB
#define DIST_LED3_Pin GPIO_PIN_10
#define DIST_LED3_GPIO_Port GPIOB
#define Buzzer_PWM_Pin GPIO_PIN_6
#define Buzzer_PWM_GPIO_Port GPIOC
#define Motor2_PWM_Pin GPIO_PIN_8
#define Motor2_PWM_GPIO_Port GPIOA
#define Motor1_PWM_Pin GPIO_PIN_9
#define Motor1_PWM_GPIO_Port GPIOA
#define DIST_LED1_Pin GPIO_PIN_15
#define DIST_LED1_GPIO_Port GPIOA
#define DIST_LED2_Pin GPIO_PIN_3
#define DIST_LED2_GPIO_Port GPIOB
#define ENC1_A_Pin GPIO_PIN_4
#define ENC1_A_GPIO_Port GPIOB
#define ENC1_B_Pin GPIO_PIN_5
#define ENC1_B_GPIO_Port GPIOB
#define ENC2_A_Pin GPIO_PIN_6
#define ENC2_A_GPIO_Port GPIOB
#define ENC2_B_Pin GPIO_PIN_7
#define ENC2_B_GPIO_Port GPIOB
/* USER CODE BEGIN Private defines */
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
