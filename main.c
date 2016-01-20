/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "analyzer.h"

extern uint32_t os_time;
uint32_t HAL_GetTick(void) { 
  return os_time; 
}

/* Private typedef -----------------------------------------------------------*/

/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef  g_AdcHandle;
DMA_HandleTypeDef  g_DmaHandle;
osThreadId Main_thID;
static uint8_t Trigger_Point=255-50;
// Main aquisition memory block
uint32_t values[ADC_BUFFER_LENGTH] __attribute__((at(0xC0400000)));
// Sample buffer
volatile unsigned short values_BUF[ADC_BUFFER_LENGTH];
/* Private function prototypes -----------------------------------------------*/
static void SystemClock_Config(void);
static void Error_Handler(void);
static void MPU_Config(void);
static void CPU_CACHE_Enable(void);
static void Hello_MSG(void);
Touch_struct* Touch_Callback( Touch_struct* l_Touched);
extern int Init_TH_Touch (void);
extern void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense);
extern uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq);
extern HAL_StatusTypeDef ADC_INIT(ADC_HandleTypeDef* AdcHandle);
extern HAL_StatusTypeDef ConfigureDMA(DMA_HandleTypeDef* DmaHandle, ADC_HandleTypeDef* AdcHandle);

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
	static volatile uint16_t i=0;
	static uint16_t Triggered_Sample=0;	
	osEvent evt;
	GUI_MEMDEV_Handle hMem0;
	//TOUCH_STATE * 	pState;
	Touch_struct Touched;
  ///////////////////////////////////////////////////////////////////////////////////////////
  /* Configure the MPU attributes as Write Through */
  MPU_Config();
  /* Enable the CPU Cache */
  CPU_CACHE_Enable();
  osKernelInitialize();                 // initialize CMSIS-RTOS
	///////////////////////////////////////////////////////////////////////////////////////////
	// Hardware initialize
  if( HAL_Init() != HAL_OK)
		Error_Handler();
	BSP_SDRAM_Init();
	Touch_Initialize();
	if( ConfigureDMA(&g_DmaHandle, &g_AdcHandle) != HAL_OK)
		Error_Handler();
	if( ADC_INIT(&g_AdcHandle) != HAL_OK)
		Error_Handler();
	///////////////////////////////////////////////////////////////////////////////////////////
  /* Configure the System clock to have a frequency of 216 MHz */
  SystemClock_Config();
	// Thread initialization
	Init_TH_Touch ();
	// RTOS Start Kernel
  osKernelStart();                      // start thread execution 
	// Get Main Thread ID
	Main_thID = osThreadGetId();
	///////////////////////////////////////////////////////////////////////////////////////////
	//GUI Initialize
	GUI_Init();
	osDelay(100);
	GUI_Clear();
	// Welcome user
	Hello_MSG();
	
	GUI_SelectLayer(0);
	hMem0 = GUI_MEMDEV_Create(0,0,480,272);
	
	if(!GUI_CURSOR_GetState())
		GUI_CURSOR_Select(&GUI_CursorCrossM);
	
	GUI_CURSOR_Show();
	
	///////////////////////////////////////////////////////////////////////////////////////////
	// Start data acquire
	HAL_ADC_Start_DMA(&g_AdcHandle, values, ADC_BUFFER_LENGTH);
	///////////////////////////////////////////////////////////////////////////////////////////
	
	/* Infinite loop */
  while (1)
  {
Anal_CH1_Set(/*Main loop start*/);
Anal_CH2_Set(/*Wait start*/);
		evt = osSignalWait(DMA_ConvCpltSig,2);
		if( evt.status == osEventTimeout)
				Error_Handler();
Anal_CH2_Reset(/*Wait finish*/);
		
		
Anal_CH3_Set(/*Copy start*/);
		for(i=0;i<ADC_BUFFER_LENGTH;i++)	// <- Temporary. Take the full advantage of DMA !
		values_BUF[i]=255-values[i];
Anal_CH3_Reset(/*Copy finish*/);
		
		HAL_ADC_Start_DMA(&g_AdcHandle, values, ADC_BUFFER_LENGTH);
		osSignalClear(Main_thID, DMA_ConvCpltSig);
			
		GUI_CURSOR_SetPosition(Touched.pState->x,Touched.pState->y);
			
		Triggered_Sample = Trigger(Trigger_Point, values_BUF, ADC_BUFFER_LENGTH, 1348000UL);
		//if(Triggered_Sample >=20)Triggered_Sample -=20; // Offset to see the edge in the center <- bullshit ?
		
		GUI_MEMDEV_Select(hMem0);
		GUI_MEMDEV_Clear(hMem0);
		
		GUI_SetColor(GUI_ORANGE);
		GUI_FillRect(0,0,480,272);
		
		GUI_SetColor(GUI_BLACK);
		GUI_DrawRect(34,5,474,266);		
		
		GUI_SetColor(GUI_WHITE);
		GUI_FillRect(35,6,475,266);
		
		Draw_GraphGrid(440,260,40,40);
		
		GUI_SetColor(GUI_BLUE);
		/*Draw garph start*/HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_1);
		GUI_DrawGraph((short*)&values_BUF[Triggered_Sample],440,35,6); // Useful: GUI_COUNTOF(values)
		/*Draw garph finish*/HAL_GPIO_TogglePin(GPIOI,GPIO_PIN_1);
		
		GUI_SetColor(GUI_GREEN);
		GUI_DrawHLine(Trigger_Point,0,480);
		GUI_FillCircle(15,Trigger_Point,10);
		
		GUI_SetColor(GUI_YELLOW);
		GUI_DrawCircle(15,Trigger_Point,10);
		
		GUI_MEMDEV_CopyToLCD(hMem0);
Anal_CH1_Reset(/*Main loop finish*/);
  }
}

void Hello_MSG() {
	GUI_SetColor(GUI_BLUE);
	GUI_SetFont(&GUI_Font16_1);
	GUI_DispStringHCenterAt("Oscyloskop cyfrowy v1.1" , 240, 150);
	GUI_DispStringHCenterAt("Adrian Kurylak" , 240, 175);
	GUI_DispStringHCenterAt("Politechnika Wroclawska" , 240, 200);
	GUI_SetFont(&GUI_Font32_1);
	GUI_DispStringHCenterAt("Inicjalizacja" , 240, 120);
	osDelay(250);
	GUI_DispStringHCenterAt("Inicjalizacja." , 240, 120);
	osDelay(500);
	GUI_DispStringHCenterAt("Inicjalizacja.." , 240, 120);
	osDelay(500);
	GUI_DispStringHCenterAt("Inicjalizacja..." , 240, 120);
	osDelay(750);
}
/**
  * @brief  System Clock Configuration
  *         The system Clock is configured as follow : 
  *            System Clock source            = PLL (HSE)
  *            SYSCLK(Hz)                     = 216000000
  *            HCLK(Hz)                       = 216000000
  *            AHB Prescaler                  = 1
  *            APB1 Prescaler                 = 4
  *            APB2 Prescaler                 = 2
  *            HSE Frequency(Hz)              = 25000000
  *            PLL_M                          = 25
  *            PLL_N                          = 432
  *            PLL_P                          = 2
  *            PLL_Q                          = 9
  *            VDD(V)                         = 3.3
  *            Main regulator output voltage  = Scale1 mode
  *            Flash Latency(WS)              = 7
  * @param  None
  * @retval None
  */
static void SystemClock_Config(void){
  RCC_ClkInitTypeDef RCC_ClkInitStruct;
  RCC_OscInitTypeDef RCC_OscInitStruct;

  /* Enable HSE Oscillator and activate PLL with HSE as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSIState = RCC_HSI_OFF;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 25;
  RCC_OscInitStruct.PLL.PLLN = 432;  
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 9;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* activate the OverDrive to reach the 216 Mhz Frequency */
  if(HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 
     clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_7) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief  This function is executed in case of error occurrence.
  * @param  None
  * @retval None
  */
static void Error_Handler(void){
  /* User may add here some code to deal with this error */
  while(1)
  {
		LED_On(0);
		osDelay(100);
		LED_Off(0);
		osDelay(100);
		LED_On(0);
		osDelay(100);
		LED_Off(0);
		osDelay(100);
		LED_On(0);
		osDelay(100);
		LED_Off(0);
		osDelay(500);
  }
}

/**
  * @brief  Configure the MPU attributes as Write Through for SRAM1/2.
  * @note   The Base Address is 0x20010000 since this memory interface is the AXI.
  *         The Region Size is 256KB, it is related to SRAM1 and SRAM2  memory size.
  * @param  None
  * @retval None
  */
static void MPU_Config(void){
  MPU_Region_InitTypeDef MPU_InitStruct;
  
  /* Disable the MPU */
  HAL_MPU_Disable();

  /* Configure the MPU attributes as WT for SRAM */
  MPU_InitStruct.Enable = MPU_REGION_ENABLE;
  MPU_InitStruct.BaseAddress = 0x20010000;
  MPU_InitStruct.Size = MPU_REGION_SIZE_256KB;
  MPU_InitStruct.AccessPermission = MPU_REGION_FULL_ACCESS;
  MPU_InitStruct.IsBufferable = MPU_ACCESS_NOT_BUFFERABLE;
  MPU_InitStruct.IsCacheable = MPU_ACCESS_CACHEABLE;
  MPU_InitStruct.IsShareable = MPU_ACCESS_NOT_SHAREABLE;
  MPU_InitStruct.Number = MPU_REGION_NUMBER0;
  MPU_InitStruct.TypeExtField = MPU_TEX_LEVEL0;
  MPU_InitStruct.SubRegionDisable = 0x00;
  MPU_InitStruct.DisableExec = MPU_INSTRUCTION_ACCESS_ENABLE;

  HAL_MPU_ConfigRegion(&MPU_InitStruct);

  /* Enable the MPU */
  HAL_MPU_Enable(MPU_PRIVILEGED_DEFAULT);
}

/**
  * @brief  CPU L1-Cache enable.
  * @param  None
  * @retval None
  */
static void CPU_CACHE_Enable(void){
  /* Enable I-Cache */
  SCB_EnableICache();

  /* Enable D-Cache */
  SCB_EnableDCache();
}

#ifdef  USE_FULL_ASSERT

/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
