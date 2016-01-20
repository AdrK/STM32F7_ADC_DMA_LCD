#include "main.h"

uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq);

uint16_t Trigger(uint8_t Trig_SP, volatile unsigned short* Signal, uint16_t Sig_Size, uint32_t Sample_Freq) {

	uint16_t i=0;
	uint16_t ctr=0;
	//uint8_t Treshold = 2;
	Sig_Size -=500;
	
	for(i=0;i<=Sig_Size;i++)
	{
		if(Signal[i] < Trig_SP)
		{
			ctr=i;
			break;
		}
	}
	
	for(i=ctr;i<=Sig_Size;i++)
	{
		if(Signal[i] >= Trig_SP)
		{
			return i;
		}
	}
	
	return 0;
}
