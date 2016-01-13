#include <stdlib.h>
#include "RA8875.h"
#include "driverlib/sysctl.h"
#include "assistant.h"
#include "touch.h"

void TouchPanel_CountPixel(uint16_t *x, uint16_t *y);

void TouchPanel_TouchEnable(void)
{
		TouchPanel_AutoModeSet();
		TouchPanel_IntClean();
}

void TouchPanel_GetPixel(uint16_t *X, uint16_t *Y)
{
		uint16_t x1,y1,x2,y2;
		if(TouchPanel_CheckTouch())
		{
				x1 = TouchPanel_GetX();
				y1 = TouchPanel_GetY();
				TouchPanel_CountPixel(&x1,&y1);
				TouchPanel_IntClean();
				SysCtlDelay(SysCtlClockGet()/30);
				if(TouchPanel_CheckTouch())
				{
						x2 = TouchPanel_GetX();
						y2 = TouchPanel_GetY();
						TouchPanel_CountPixel(&x2,&y2);
						if(x1-8<x2&&x2<x1+8&&y2>y1-8&&y2<y1+8)
						{
								*X = x2;
								*Y = y2;
						}
						else  
						{
							*X = 0;
							*Y = 0;
						}
				}
				else
				{
						*X = 0;
						*Y = 0;
				}
		}
		else
		{
				*X = 0;
			  *Y = 0;
		}
		TouchPanel_IntClean();
}

void TouchPanel_CountPixel(uint16_t *x, uint16_t *y)
{
		*x = (uint16_t)((*x)*0.849 - 40.684);
		*y = (uint16_t)((*y)*0.5510 - 46.779);
}

bool TouchPanel_ButtonCheck(textWidget rectButton, uint16_t x, uint16_t y)
{
		bool boolTmp = false;
		if(rectButton.tRect.xStart<x&&rectButton.tRect.yStart<y&&rectButton.tRect.xEnd>x&&rectButton.tRect.yEnd>y)
		{
				boolTmp = true;
		}
		return boolTmp;
}

uint32_t TouchPanel_GetButtonIndex(textWidget *uiWidget, uint16_t x, uint16_t y)
{
		uint32_t temp;
	char strTemp[20];
		textWidget* widgetTemp = uiWidget;
		temp = 0;
		do
		{
				if(TouchPanel_ButtonCheck(*widgetTemp,x,y))
						break;
				widgetTemp = widgetTemp->next;
				temp++;
		} while(widgetTemp != NULL);
					LCD_TextModeSet();
					StrConv_Int2Str(strTemp,4,temp + uiWidget->uiID*100);
					FontCtl_CursorPositionSet(0,400);
					LCD_StringDisplay(strTemp);

		return (temp + (uiWidget->uiID)*100);
}
