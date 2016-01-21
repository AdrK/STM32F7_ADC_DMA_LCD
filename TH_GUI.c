#include "main.h"
#include "analyzer.h"

// Extern variables
extern Touch_struct g_Touched;
extern uint8_t Trigger_Point;
extern ADC_HandleTypeDef  g_AdcHandle;
extern DMA_HandleTypeDef  g_DmaHandle;
extern osThreadId tid_Touch;
// Private functions
void Hello_MSG(void);
void TH_GUI (void const *argument);                             // thread function
osThreadId tid_TH_GUI;                                          // thread id
osThreadDef (TH_GUI, osPriorityNormal, 1, 0);                   // thread object

// Extern functions
extern void Error_Handler(void);
// Main aquisition memory block
extern uint32_t values[ADC_BUFFER_LENGTH] __attribute__((at(0xC0400000)));
// Sample buffer
extern volatile unsigned short values_BUF[ADC_BUFFER_LENGTH];
extern uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq);
extern void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense);

int Init_TH_GUI (void) {
	
	//GUI Initialize
	GUI_Init();
	GUI_Clear();
	///////////////////////////////////////////////////////////////////////////////////////////
	// Welcome user
	Hello_MSG();
  tid_TH_GUI = osThreadCreate (osThread(TH_GUI), NULL);
  if (!tid_TH_GUI) return(-1);
  
  return(0);
}

void TH_GUI (void const *argument) {
	static volatile uint16_t i=0;
	static uint16_t Triggered_Sample=0;	
	osEvent evt;
	GUI_MEMDEV_Handle hMem0;
	GUI_SelectLayer(0);
	hMem0 = GUI_MEMDEV_Create(0,0,480,272);
	
	if(!GUI_CURSOR_GetState())
		GUI_CURSOR_Select(&GUI_CursorCrossM);
	
	GUI_CURSOR_Show();
  while (1) {
    Anal_CH1_Set(/*Main loop start*/);
																															Anal_CH2_Set(/*Wait start*/);
		evt = osSignalWait(DMA_ConvCpltSig,(uint32_t)2);
		if( evt.status == osEventTimeout)
				Error_Handler();
																															Anal_CH2_Reset(/*Wait finish*/);
		osSignalSet(tid_Touch,GUI_TouchStateReqSig);
		
																															Anal_CH3_Set(/*Copy start*/);
		for(i=0;i<ADC_BUFFER_LENGTH;i++)	// <- Temporary. Take the full advantage of DMA !
		values_BUF[i]=255-values[i];
																															Anal_CH3_Reset(/*Copy finish*/);
		
		HAL_ADC_Start_DMA(&g_AdcHandle, values, ADC_BUFFER_LENGTH);
		osSignalClear(tid_TH_GUI, DMA_ConvCpltSig);
		
		osSignalWait(GUI_TouchGetSig,(uint32_t)0);
		GUI_CURSOR_SetPosition(g_Touched.pState->x,g_Touched.pState->y);
		
		Trigger_Point = g_Touched.MSG;
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
    osThreadYield ();                                           // suspend thread
  }
}

void Hello_MSG(void) {

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
