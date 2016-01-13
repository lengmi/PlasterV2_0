#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/tm4c123gh6pm.h"
#include "driverlib/gpio.h"
#include "driverlib/sysctl.h"
#include "driverlib/timer.h"
#include "driverlib/interrupt.h"
#include "driverlib/rom.h"
#include "IODevice.h"
#include "Display.h"
#include "touch.h"
#include "ui.h"
#include "Interrupt.h"
#include "assistant.h"
#include "RA8875.h"

extern uint64_t SpeedPulse;

extern bool SystemState;
extern bool Laser;
extern textWidget ErrorWarning;
extern textWidget* CurrentUI;

extern void Interrupt_PB3Handler(void);
extern void Interrupt_Timer2AHandler(void);

bool LedRed = false;

void Interrupt_Timer2Init(void)
{
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER2);
		TimerConfigure(TIMER2_BASE,TIMER_CFG_PERIODIC);
		TimerLoadSet(TIMER2_BASE,TIMER_A,SysCtlClockGet()-1);
		IntEnable(INT_TIMER2A);
		TimerIntEnable(TIMER2_BASE,TIMER_TIMA_TIMEOUT);
		IntMasterEnable();
		TimerIntRegister(TIMER2_BASE,TIMER_A,Interrupt_Timer2AHandler);
		TimerEnable(TIMER2_BASE,TIMER_A);
}

void Interrupt_Timer2AHandler(void)
{
		char strTemp[20];
		TimerIntClear(TIMER2_BASE,INT_TIMER2A);
		LedRed = !LedRed;
		LED_LightOn(GPIO_PIN_1,LedRed);
		SpeedPulse = TimerValueGet64(WTIMER0_BASE);
		TimerLoadSet(WTIMER0_BASE,TIMER_A,0x00);
		StrConv_Int2Str(strTemp,8,(int32_t)SpeedPulse);
		//LCD_DisplayNumber(strTemp,RA8875_RED,400,8);
}

void Interrupt_WDTimer0Init(void)
{
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_WTIMER0);
		TimerConfigure(WTIMER0_BASE,TIMER_CFG_A_CAP_COUNT_UP);
		TimerControlEvent(WTIMER0_BASE,TIMER_A,TIMER_EVENT_POS_EDGE);
		TimerLoadSet(WTIMER0_BASE,TIMER_A,0x00);
		//TimerEnable(WTIMER0_BASE, TIMER_A);
}

void Interrupt_PB3Init(void)
{
		GPIOIntRegister(GPIO_PORTB_BASE,Interrupt_PB3Handler);
		ROM_GPIOPinTypeGPIOInput(GPIO_PORTB_BASE, GPIO_PIN_3);
		ROM_GPIOPadConfigSet(GPIO_PORTB_BASE,GPIO_PIN_3,GPIO_STRENGTH_2MA,GPIO_PIN_TYPE_STD_WPU);
		GPIOIntDisable(GPIO_PORTB_BASE,GPIO_INT_PIN_3);
	  GPIOIntClear(GPIO_PORTB_BASE,GPIO_INT_PIN_3);
		GPIOIntTypeSet(GPIO_PORTB_BASE,GPIO_INT_PIN_3,GPIO_LOW_LEVEL);
}

void Interrupt_PB3Handler(void)
{
		uint16_t x,y;
		GPIOIntDisable(GPIO_PORTB_BASE,GPIO_INT_PIN_3);
		IODevice_LaserOn(false);
		TowerLight_ErrorMode();
		LCD_ClearWindow();
		LCD_DisplayWidget(ErrorWarning);
		TouchPanel_GetPixel(&x,&y);
		do
		{
				TouchPanel_GetPixel(&x,&y);
				__Delay50ms();
		}
		while(y<200|y>280);	
		LCD_DisplayInterface(CurrentUI);
}
