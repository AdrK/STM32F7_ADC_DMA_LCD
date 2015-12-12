#include "main.h"

extern ADC_HandleTypeDef g_AdcHandle;
extern DMA_HandleTypeDef  g_DmaHandle;
extern uint32_t values[ADC_BUFFER_LENGTH];
extern volatile unsigned short values_BUF[ADC_BUFFER_LENGTH];
uint16_t ctr;
extern uint8_t IRQ_FLAG;
extern osThreadId Main_thID;

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
	adcChannel.SamplingTime = ADC_SAMPLETIME_3CYCLES;
	adcChannel.Offset = 0;

  if (HAL_ADC_ConfigChannel(AdcHandle, &adcChannel) != HAL_OK)
  {
    /* Channel Configuration Error */
    return HAL_ERROR;
  }
	return HAL_OK;
}

HAL_StatusTypeDef ConfigureDMA(DMA_HandleTypeDef* DmaHandle, ADC_HandleTypeDef* AdcHandle)
{
    __DMA2_CLK_ENABLE(); 
    DmaHandle->Instance = DMA2_Stream0;
  
    DmaHandle->Init.Channel  = DMA_CHANNEL_2;
    DmaHandle->Init.Direction = DMA_PERIPH_TO_MEMORY;
    DmaHandle->Init.PeriphInc = DMA_PINC_DISABLE;
    DmaHandle->Init.MemInc = DMA_MINC_ENABLE;
    DmaHandle->Init.PeriphDataAlignment = DMA_PDATAALIGN_WORD;
    DmaHandle->Init.MemDataAlignment = DMA_MDATAALIGN_WORD;
    DmaHandle->Init.Mode = DMA_CIRCULAR;
    DmaHandle->Init.Priority = DMA_PRIORITY_HIGH;
    DmaHandle->Init.FIFOMode = DMA_FIFOMODE_DISABLE;         
    DmaHandle->Init.FIFOThreshold = DMA_FIFO_THRESHOLD_HALFFULL;
    DmaHandle->Init.MemBurst = DMA_MBURST_SINGLE;
    DmaHandle->Init.PeriphBurst = DMA_PBURST_SINGLE; 
    
	
	if (HAL_DMA_Init(DmaHandle) != HAL_OK)
  {
    /* Configuration Error */
    return HAL_ERROR;
  }    
    __HAL_LINKDMA(AdcHandle, DMA_Handle, *DmaHandle);
 
    HAL_NVIC_SetPriority(DMA2_Stream0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(DMA2_Stream0_IRQn);
	
		return HAL_OK;
}

void HAL_ADC_ConvHalfCpltCallback(ADC_HandleTypeDef* AdcHandle)
{	
	//HAL_ADC_Stop_DMA(&g_AdcHandle);
	
	//osSignalSet(Main_thID,DMA_ConvHalfCpltSig);
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* AdcHandle)
{	
	HAL_ADC_Stop_DMA(&g_AdcHandle);
	osSignalSet(Main_thID,DMA_ConvCpltSig);
	//HAL_GPIO_TogglePin (GPIOB, GPIO_PIN_14);
}

 void DMA2_Stream0_IRQHandler()
{
		HAL_DMA_IRQHandler(&g_DmaHandle);
}
		
void ADC_IRQHandler()
{
		HAL_ADC_IRQHandler(&g_AdcHandle);
}
