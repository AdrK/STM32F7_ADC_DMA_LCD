#include "cmsis_os.h"                   // CMSIS RTOS header file
#include "GUI.h"
#include "main.h"

void GUIThread (void const *argument);              // thread function
uint16_t RisingEdge(uint16_t Treshold, uint16_t Jmp_Treshold, uint32_t* Signal, uint16_t Sig_Size);

osThreadId tid_GUIThread;                           // thread id
osThreadDef (GUIThread, osPriorityHigh, 1, 8192);   // thread object
extern uint32_t values[ADC_BUFFER_LENGTH];
extern ADC_HandleTypeDef g_AdcHandle;

int Init_GUIThread (void) {

  tid_GUIThread = osThreadCreate (osThread(GUIThread), NULL);
  if(!tid_GUIThread) return(-1);
  
  return(0);
}

void GUIThread (void const *argument) {
	short values_TEMP[480];	
	uint16_t IRQ_EdgeCTR=0;
	uint16_t i=0;
	
  GUI_Init();                     //Initialize the Graphics Component
	GUI_SetFont(&GUI_Font32_1);
	GUI_SetBkColor(GUI_BLACK);
	GUI_SetPenSize(20);
	GUI_SetColor(GUI_RED);
	GUI_SetTextMode(GUI_TM_NORMAL);
	
  while (1) {
		GUI_Delay(40);
		
		osSignalWait(0xF0F0, 5000);
		__HAL_ADC_DISABLE_IT(&g_AdcHandle,ADC_IT_EOC);
		//IRQ_EdgeCTR = RisingEdge(25, 50, values, ADC_BUFFER_LENGTH); // Remember about ">>" inside Rising_Edge	
		//if(IRQ_EdgeCTR > (ADC_BUFFER_LENGTH-480)) IRQ_EdgeCTR=1024;
		
		for(i=0; i < 480; i++)
		{
			values_TEMP[i] = (short)values[i];
		}
		
		GUI_Clear();
		//GUI_DispStringHCenterAt("Oscyloskop v1.0" , 250, 200);
		GUI_DrawGraph(values_TEMP,480,0,0); // Useful: GUI_COUNTOF(values)
		__HAL_ADC_ENABLE_IT(&g_AdcHandle,ADC_IT_EOC);
		osSignalClear(tid_GUIThread, 0xF0F0);
		
#ifdef RTE_Graphics_Touchscreen   /* Graphics Input Device Touchscreen enabled */
    GUI_TOUCH_Exec();             /* Execute Touchscreen support */
#endif
    GUI_Exec();                   /* Execute all GUI jobs ... Return 0 if nothing was done. */
    GUI_X_ExecIdle();             /* Nothing left to do for the moment ... Idle processing */
  }
}

uint16_t RisingEdge(uint16_t Treshold, uint16_t Jmp_Treshold, uint32_t* Signal, uint16_t Sig_Size){

	uint32_t	R_1=0;
	uint32_t	R_2=0;
	uint32_t	R_3=0;
	
	uint16_t	Count=0;
	uint16_t	i=0;
		
	for( i=3;	i < ( Sig_Size - 4 );	i++ )
		{
			R_1 = ( Signal[i+1]>>1 ) - ( Signal[i+0]>>1 );
			R_2 = ( Signal[i+3]>>1 ) - ( Signal[i+0]>>1 );
			R_3 = ( Signal[i+4]>>1 ) - ( Signal[i+0]>>1 );
			
			if( R_1 >= Jmp_Treshold ) return (i);
			if( R_2 >= Jmp_Treshold ) return (i-2);
			if( R_3 >= Jmp_Treshold ) return (i-3);
			
			if( R_1 >= 2 )
			{
				Count += 1;
				if( Count == Treshold ) return (i);
			}
			
			if( R_2 >= 5 )
			{
				Count += 2;
				if( Count == Treshold ) return (i-2);
			}
			
			if( R_3 >= 6 )
			{
				Count += 4;
				if( Count == Treshold ) return (i-3);
			}
		}
	return 0;
}
/****END OF FILE****/
