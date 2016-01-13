#include "spi.h"
#include "RA8875.h"
#include "assistant.h"
#include "driverlib/fpu.h"
#include "driverlib/rom.h"
#include <stdlib.h>
#include <math.h>
#include "IODevice.h"
#include "Display.h"
#include "touch.h"
#include "ui.h"
#include "DataManage.h"
#include "Interrupt.h"

uint32_t Operation;
uint64_t SpeedPulse;

extern bool SystemState;
extern bool ErrorState;

extern textWidget MainSet,MainRun,MainTest,MainTitle,MainSubline;
extern textWidget RunStop,RunState,RunTime,RunTitle,RunSubline;
extern textWidget TestOptic,TestFan,TestLaser,TestBlow,TestReturn,TestScan,TestTitle,TestSubline;
extern textWidget SetOption,SetSub,SetAdd,SetDiscard,SetSave,SetDiameter,SetPointAmount,SetDistance,\
									SetHoleAmount,SetTitle,SetSubline;
extern textWidget ErrorWarning;

int main(void)
{
		ROM_FPUEnable();
		ROM_FPULazyStackingEnable();
		ROM_SysCtlClockSet(SYSCTL_SYSDIV_4 | SYSCTL_USE_PLL | SYSCTL_OSC_MAIN | SYSCTL_XTAL_16MHZ);
		__Delay100ms();
		GPIO_Initial();
	
		Interrupt_PB3Init();
		Interrupt_Timer2Init();
		Interrupt_WDTimer0Init();
	
		SSI0_InitialSPI();
		LCD_HardReset();
		LCD_Initial();
		BTE_BackgroundColorSetMix(RA8875_BLACK);
		LCD_ClearWindow();
		FontCtl_CNZHDisplaySet();
		TouchPanel_TouchEnable();
		__Delay100ms();
		LCD_ClearWindow();
	
		Operation = GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6);
	
		DataManage_ReadData(0);
		LCD_DisplayInterface(&MainSet);
	
		while(1)
		{
				Operation = IODevice_GetHandModeOperation();
				switch(Operation)
				{
						case 0x00: if(!SystemState)
											 {
												 IODevice_AutoRun();
												 LCD_DisplayInterface(&RunStop);
												 LCD_DisplayNumber("手动模式",RA8875_RED,400,214);
											 }
											 break;
					
						case 0x40: if(SystemState)
											 {
													IODevice_AutoStop();
													IODevice_Initial();
													LCD_DisplayInterface(&MainSet);
											 }
											 break;
					
						default:  IODevice_AutoStop();
											UI_Desktop();	
				}
		}
}
