#ifndef _TOUCH_H_
#define _TOUCH_H_
#include <stdint.h>
#include <stdbool.h>
#include "Display.h"

extern void TouchPanel_TouchEnable(void);
extern void TouchPanel_GetPixel(uint16_t *X, uint16_t *Y);
extern bool TouchPanel_ButtonCheck(textWidget rectButton, uint16_t x, uint16_t y);
extern uint32_t TouchPanel_GetButtonIndex(textWidget *uiWidget, uint16_t x, uint16_t y);
#endif

