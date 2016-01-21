#include "main.h"

void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense);

void Draw_GraphGrid(uint16_t XSize, uint16_t YSize, uint8_t XDense, uint8_t YDense){
	//
	// THIS CAN BE DONE FASTER.
	//
	uint16_t XStart,YStart;	
	uint8_t Xstep = XSize/XDense;
	uint8_t Ystep = YSize/YDense;
	uint8_t i=0;
	//unsigned char Cur_Ls;
	//unsigned char Cur_Color;
	
	XStart = 34;
	YStart = 5;
	
	GUI_SetLineStyle(GUI_LS_DOT);
	GUI_SetColor(GUI_DARKGRAY);
	
	for(i=1;i<=Xstep;i++)
		GUI_DrawLine(XStart+i*XDense,YStart,XStart + i*XDense,YStart+YSize);
	
	for(i=1;i<=Ystep;i++)
		GUI_DrawLine(XStart,YStart+i*YDense,XStart+XSize,YStart+i*YDense);

}
