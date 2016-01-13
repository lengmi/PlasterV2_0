#include <stdlib.h>
#include "RA8875.h"
#include "driverlib/sysctl.h"
#include "Display.h"
#include "assistant.h"

extern textWidget* CurrentUI;

void LCD_DisplayWidget(textWidget text)
{
		//LCD_DisplayOn(0);
		uint16_t xMiddle = (text.tRect.xStart + text.tRect.xEnd)/2;
		uint16_t yMiddle = (text.tRect.yStart + text.tRect.yEnd)/2;
		uint16_t strLength = text.strNumber * 16;
		BTE_ForegroundColorSetMix(text.backColor);
		Draw_SquareSet();
		Draw_LSPositionSet(text.tRect.xStart,text.tRect.yStart,text.tRect.xEnd,text.tRect.yEnd);
		Draw_CSTFill(false);
		Draw_LSTStart(true);
		BTE_ForegroundColorSetMix(text.foreColor);
		LCD_TextModeSet();
		FontCtl_CursorPositionSet(xMiddle - strLength/2,yMiddle -16);
		LCD_StringDisplay(text.str);
		//LCD_DisplayOn(1);
}

void LCD_DisplayInterface(textWidget* textUI)
{
		textWidget* tTextWidget = textUI;
		LCD_ClearWindow();
		LCD_DisplayOn(0);
		do
		{
			LCD_DisplayWidget(*tTextWidget);
			tTextWidget = tTextWidget->next;
		}while(tTextWidget != NULL);
		LCD_DisplayOn(1);
		CurrentUI = textUI;
}

void LCD_DisplayNumber(char *numStr,uint32_t color, uint16_t x, uint16_t y)
{
		LCD_TextModeSet();
		BTE_ForegroundColorSetMix(color);
		FontCtl_CursorPositionSet(x,y);
		LCD_StringDisplay(numStr);
}

void LCD_CountDown(uint32_t number)
{
		char strTemp[20];
		while(number-->0)
		{
				__Delay100ms();
				StrConv_Int2Str(strTemp,3,number);
				LCD_DisplayNumber(strTemp,RA8875_WHITE,720,400);
		}
}
