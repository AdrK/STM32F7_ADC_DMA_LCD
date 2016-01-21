#include "main.h"
// Exported variables
Touch_struct g_Touched;
// Static variables

// Extern variables
extern uint8_t Trigger_Point;
extern osThreadId tid_TH_GUI;
// Main aquisition memory block
extern uint32_t values[ADC_BUFFER_LENGTH] __attribute__((at(0xC0400000)));
// Sample buffer
extern volatile unsigned short values_BUF[ADC_BUFFER_LENGTH];

// Extern functions
extern void Error_Handler(void);
Touch_struct* Touch_Callback( Touch_struct* l_Touched);
extern uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq);
extern void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense);

// Local functions
void TH_Touch (void const *argument);                             // thread function
osThreadId tid_Touch;                                          		// thread id
osThreadDef (TH_Touch, osPriorityNormal, 1, 0);                   // thread object

int Init_TH_Touch (void) {
  tid_Touch = osThreadCreate (osThread(TH_Touch), NULL);
  if (!tid_Touch) return(-1);
  
  return(0);
}

void TH_Touch (void const *argument) {
	static Touch_struct* Pg_Touched = &g_Touched;
  while (1) {
    osSignalWait(GUI_TouchStateReqSig,(uint32_t)0);
		Touch_Callback(Pg_Touched);
		osSignalSet(tid_TH_GUI,GUI_TouchGetSig);
    osThreadYield ();                                           // suspend thread
  }
}

Touch_struct* Touch_Callback( Touch_struct* l_Touched)
{
	static uint8_t PRESS_FLAG=0; // This must be static
	TOUCH_STATE* l_pState;
	Touch_GetState( l_pState );
	
	l_Touched->pState->pressed = l_pState->pressed;
	l_Touched->pState->x = l_pState->x;
	l_Touched->pState->y = l_pState->y;

	if(	l_pState->pressed )
	{
		if(	((l_pState->x >=0)&&(l_pState->x <= 20) && (l_pState->y >=Trigger_Point-16) && (l_pState->y <=Trigger_Point+16)) || PRESS_FLAG ) // If trigger tapped pressed or hold
		{
			PRESS_FLAG = 1;
			l_Touched->ID = ID_TRIGGER;
		}
		else
			l_Touched->ID = ID_NULL;
	}
	else
	{
		PRESS_FLAG = 0;
	}
	
	switch( l_Touched->ID )
	{
		case ID_TRIGGER:
		{
			l_Touched->MSG = l_pState->y;
			
		} break;
		
		default:{}		
	}
	return l_Touched;
}
