#ifndef _DEISPLAY_H_
#define _DEISPLAY_H_
#include <stdint.h>
#include <stdbool.h>

typedef struct __Widget textWidget;

typedef struct {
	uint16_t xStart;
	uint16_t yStart;
	uint16_t xEnd;
	uint16_t yEnd;
} rect;

struct __Widget{
	textWidget * parent;
	textWidget * next;
	rect tRect; 
	uint16_t backColor;
	uint16_t foreColor;
	char * str;
	uint8_t strNumber;
	uint8_t uiID;
};

#define CanvasStruct(psParent, psNext, ui16X, ui16Y,\
									width, height, backColor, foreColor, str,strNumber,uiID) \
									{														        \
						          (textWidget*) psParent,      \
											(textWidget*) psNext,				\
												{														\
													ui16X,											\
													ui16Y,											\
													ui16X+width, 								\
													ui16Y+height                \
												},							              \
												backColor,									\
												foreColor,									\
												str,												\
												strNumber,                  \
												uiID												\
									}	
#define __Canvas(sName,psParent, psNext, ui16X, ui16Y,\
									width, height, backColor, foreColor, str,strNumber,uiID) \
				textWidget sName = CanvasStruct(psParent, psNext, ui16X, ui16Y,\
									width, height, backColor, foreColor, str,strNumber,uiID) 

extern void LCD_DisplayWidget(textWidget text);
extern void LCD_DisplayInterface(textWidget* textUI);		
extern void LCD_DisplayNumber(char *numStr,uint32_t color, uint16_t x, uint16_t y);			
extern void LCD_CountDown(uint32_t number);									
#endif
