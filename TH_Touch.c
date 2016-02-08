#include "main.h"
// Exported variables
Touch_struct 	g_Touched_Cur;
osMutexDef		(Touch_Mutex);
osMutexId		IDTouch_Mutex;
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
extern uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq);
extern void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense);

// Local functions
void TH_Touch (void const *argument);                             // thread function
osThreadId tid_Touch;                                          		// thread id
osThreadDef (TH_Touch, osPriorityHigh, 1, 0);                   // thread object
Touch_struct Touch_Callback( Touch_struct l_Touched	);

int Init_TH_Touch (void) {
  tid_Touch = osThreadCreate (osThread(TH_Touch), NULL);
	IDTouch_Mutex = osMutexCreate(osMutex(Touch_Mutex));
	//osMutexRelease(IDTouch_Mutex);
	if(IDTouch_Mutex == NULL)
		Error_Handler();
  if (!tid_Touch) return(-1);
  
  return(0);
}

void TH_Touch (void const *argument) {
	static Touch_struct Touched_new;
	
  while (1) {
		osDelay(10);
		
		Touch_GetState( &Touched_new.pState );
		osMutexWait(IDTouch_Mutex, osWaitForever);
			{
			
			if( (g_Touched_Cur.pState.x != Touched_new.pState.x) ||
					(g_Touched_Cur.pState.y != Touched_new.pState.y) )
				{
					
					g_Touched_Cur = Touch_Callback(Touched_new);
				}
			}
		osMutexRelease(IDTouch_Mutex);
    osThreadYield ();                                           // suspend thread
  }
}

Touch_struct Touch_Callback( Touch_struct l_Touched	)
{
	static uint8_t PRESS_FLAG=0; // It must be static

	if(	l_Touched.pState.pressed )
	{
		if(	((l_Touched.pState.x >=0)&&(l_Touched.pState.x <= 20) && (l_Touched.pState.y >=Trigger_Point-16) && (l_Touched.pState.y <=Trigger_Point+16)) || PRESS_FLAG ) // If trigger tapped, pressed or held
		{
			PRESS_FLAG = 1;
			l_Touched.ID = ID_TRIGGER;
		}
		else
			l_Touched.ID = ID_NULL;
	}
	else
	{
		PRESS_FLAG = 0;
	}
	
	switch( l_Touched.ID )
	{
		case ID_TRIGGER: // in this case in MSG we have level of triggering
		{
			
			
		} break;
		
		default:{}		
	}
	return l_Touched;
}

