#include "main.h"

extern ADC_HandleTypeDef g_AdcHandle;
extern signed short values[ADC_BUFFER_LENGTH];
extern uint16_t i;
extern uint8_t IRQ_FLAG;

HAL_StatusTypeDef ADC_INIT(ADC_HandleTypeDef* AdcHandle)
{
	GPIO_InitTypeDef gpioInit;
	ADC_ChannelConfTypeDef adcChannel;
	
	__GPIOC_CLK_ENABLE();
	__ADC3_CLK_ENABLE();

	gpioInit.Pin = GPIO_PIN_10;
	gpioInit.Mode = GPIO_MODE_ANALOG;
	gpioInit.Pull = GPIO_NOPULL;
	HAL_GPIO_Init(GPIOF, &gpioInit);

	HAL_NVIC_SetPriority(ADC_IRQn, 0, 0);
	HAL_NVIC_EnableIRQ(ADC_IRQn);

	AdcHandle->Instance = ADC3;

	AdcHandle->Init.ClockPrescaler = ADC_CLOCKPRESCALER_PCLK_DIV8;
	AdcHandle->Init.Resolution = ADC_RESOLUTION_8B;
	AdcHandle->Init.ScanConvMode = DISABLE;
	AdcHandle->Init.ContinuousConvMode = ENABLE;
	AdcHandle->Init.DiscontinuousConvMode = DISABLE;
	AdcHandle->Init.NbrOfDiscConversion = 0;
	AdcHandle->Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
	AdcHandle->Init.ExternalTrigConv = ADC_EXTERNALTRIGCONV_T1_CC1;
	AdcHandle->Init.DataAlign = ADC_DATAALIGN_RIGHT;
	AdcHandle->Init.NbrOfConversion = 1;
	AdcHandle->Init.DMAContinuousRequests = ENABLE;
	AdcHandle->Init.EOCSelection = DISABLE;

	if (HAL_ADC_Init(AdcHandle) != HAL_OK)
	{
		/* ADC initialization Error */
		return HAL_ERROR;
	}

	adcChannel.Channel = ADC_CHANNEL_8;
	adcChannel.Rank = 1;
	/*
		ADC_SAMPLETIME_3CYCLES
		ADC_SAMPLETIME_15CYCLES
		ADC_SAMPLETIME_28CYCLES
		ADC_SAMPLETIME_56CYCLES
		ADC_SAMPLETIME_84CYCLES
		ADC_SAMPLETIME_112CYCLES
		ADC_SAMPLETIME_144CYCLES
		ADC_SAMPLETIME_480CYCLES
	*/
	adcChannel.SamplingTime = ADC_SAMPLETIME_28CYCLES;
	adcChannel.Offset = 0;

  if (HAL_ADC_ConfigChannel(AdcHandle, &adcChannel) != HAL_OK)
  {
    /* Channel Configuration Error */
    return HAL_ERROR;
  }
	return HAL_OK;
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{
	values[i++] = HAL_ADC_GetValue(&g_AdcHandle);
	if(i>=2047)
	{
		HAL_ADC_Stop_IT(&g_AdcHandle);
		i=0;
		IRQ_FLAG=1;
	}
}

void ADC_IRQHandler()
{
		HAL_ADC_IRQHandler(&g_AdcHandle);
}
