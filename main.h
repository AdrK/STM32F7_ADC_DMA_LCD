/**
  ******************************************************************************
  * @file    main.h 
  * @author  Adrian Kurylak
  * @date    25-June-2015
  * @brief   Header for main.c module
  ******************************************************************************
  */
  
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

/* Includes ------------------------------------------------------------------*/
#include "stm32f7xx.h"                  // Device header
#include "stm32f7xx_hal.h"
#include "stm32f7xx_hal_conf.h"         // Keil::Device:STM32Cube Framework:Classic
#include "stm32746g_discovery_sdram.h"  // Keil.STM32F746G-Discovery::Board Support:Drivers:SDRAM
#include "Board_Touch.h"                // ::Board Support:Touchscreen
#include "Board_LED.h"                  // ::Board Support:LED
#include "GUI.h"
#ifdef _RTE_
#include "RTE_Components.h"             // Component selection
#endif
#ifdef RTE_CMSIS_RTOS                   // when RTE component CMSIS RTOS is used
#include "cmsis_os.h"                   // CMSIS RTOS header file
#endif


/* Exported types ------------------------------------------------------------*/
#pragma anon_unions
typedef struct {
	uint8_t ID;
	TOUCH_STATE pState;
	union {
		uint32_t MSG_32;
		uint16_t MSG_16;
		uint8_t MSG_8;
		uint8_t MSG;
	};
} Touch_struct;
/* Exported controls IDs -----------------------------------------------------*/
#define ID_NULL			0x00
#define ID_TRIGGER	0x01
/* Exported macros -----------------------------------------------------------*/
#define ADC_BUFFER_LENGTH 		1024
#define DEFAULT_TRIGGER_POINT	255-50
#define DMA_ConvCpltSig 			0xF0F0
#define DMA_ConvHalfCpltSig 	0xF0F1
#define GUI_TouchStateReqSig 	0xA0A0
#define GUI_TouchGetSig				0xA0A1
#define debug_mode
/* Exported functions ------------------------------------------------------- */

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
