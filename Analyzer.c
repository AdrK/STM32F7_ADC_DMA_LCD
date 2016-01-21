#include "analyzer.h"
#include "main.h"

// All needed macros are defined in header

#ifdef debug_mode
	void Analyzer_Init()
	{
		GPIO_InitTypeDef GPIO_InitStruct;
		__HAL_RCC_GPIOB_CLK_ENABLE();
		__HAL_RCC_GPIOI_CLK_ENABLE();
		GPIO_InitStruct.Pin = GPIO_PIN_14 | GPIO_PIN_15 | GPIO_PIN_8 | GPIO_PIN_9;
		GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
		GPIO_InitStruct.Pull = GPIO_PULLDOWN;
		GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
		HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		GPIO_InitStruct.Pin = GPIO_PIN_1;
		HAL_GPIO_Init(GPIOI, &GPIO_InitStruct);
		
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOI,GPIO_PIN_1,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_9,GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB,GPIO_PIN_8,GPIO_PIN_RESET);
				
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
		HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_1);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_8);
		
		
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_15);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_14);
		HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_1);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_9);
		HAL_GPIO_TogglePin(GPIOB,GPIO_PIN_8);
		
	}
#else
	void Analyzer_Init() {}
#endif
