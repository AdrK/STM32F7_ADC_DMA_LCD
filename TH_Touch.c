#include "main.h"

/*----------------------------------------------------------------------------
 *      Thread 1 'Thread_Name': Sample thread
 *---------------------------------------------------------------------------*/
 
void TH_Touch (void const *argument);                             // thread function
osThreadId tid_Touch;                                          // thread id
osThreadDef (TH_Touch, osPriorityNormal, 1, 0);                   // thread object

int Init_TH_Touch (void) {

  tid_Touch = osThreadCreate (osThread(TH_Touch), NULL);
  if (!tid_Touch) return(-1);
  
  return(0);
}

void TH_Touch (void const *argument) {

  while (1) {
    ; // Insert thread code here...
    osThreadYield ();                                           // suspend thread
  }
}
/*
Touch_struct* Touch_Callback( Touch_struct* l_Touched)
{
	uint8_t PRESS_FLAG=0;
	TOUCH_STATE* l_pState;
	Touch_GetState( l_pState );
	
	l_Touched->pState->pressed = l_pState->pressed;
	l_Touched->pState->x = l_pState->x;
	l_Touched->pState->y = l_pState->y;
	
	if(	l_pState->pressed )
		PRESS_FLAG=1;
	else
		PRESS_FLAG=0;
	
	if(	((l_pState->x >=0)&&(l_pState->x <= 20) && (l_pState->y >=Trigger_Point-16) && (l_pState->y <=Trigger_Point+16)) || PRESS_FLAG )
					l_Touched->ID = ID_TRIGGER;
	
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
*/