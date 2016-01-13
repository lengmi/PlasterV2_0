#include "IODevice.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "inc/hw_gpio.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"
#include "driverlib/rom.h"
#include "Display.h"

bool OpticGate = false;
bool Laser = false;
bool Fan = false;
bool Blow = false;
bool Scan = false;
bool SystemState = false;
bool ErrorState = false;


void GPIO_Initial(void)
{
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOB);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOC);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOD);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
	
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_6);             //Laser
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTB_BASE, GPIO_PIN_2|GPIO_PIN_4);  //预留、吹风机
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTD_BASE, GPIO_PIN_0| \
	                        GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);				//振镜Y、X、CLK、SYN
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTE_BASE, GPIO_PIN_0|GPIO_PIN_1|\
		                      GPIO_PIN_2|GPIO_PIN_3 | GPIO_PIN_5);      //预留、塔灯红、绿、黄、光闸、抽风机
		ROM_GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|\
                 	                         GPIO_PIN_3|GPIO_PIN_4);  //板上显示灯：红、蓝、绿；LCD复位
	
		ROM_GPIOPadConfigSet(GPIO_PORTB_BASE, GPIO_PIN_3, \
											GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);		//冷水机开关信号
		
		ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_6);							//测速
	
		ROM_GPIOPinTypeGPIOInput(GPIO_PORTD_BASE, GPIO_PIN_6);   //手动开关、运行模式
		HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) =  GPIO_LOCK_KEY;
		HWREG(GPIO_PORTD_BASE+GPIO_O_CR) |= 0x80;
		HWREG(GPIO_PORTD_BASE+GPIO_O_AFSEL) &= ~(0x80);
		HWREG(GPIO_PORTD_BASE+GPIO_O_DEN) |= 0x80;
		ROM_GPIOPinTypeGPIOInput(GPIO_PORTC_BASE, GPIO_PIN_7);
		HWREG(GPIO_PORTD_BASE+GPIO_O_LOCK) = 0;
}


//塔灯的三种工作情况
void TowerLight_IdleMode(void)
{
		__TowerLightRedOff();
		__TowerLightGreenOff();
		__TowerLightYellowOn();
}

void TowerLight_RunMode(void)
{
		__TowerLightRedOff();
		__TowerLightYellowOff();
		__TowerLightGreenOn();
}

void TowerLight_ErrorMode(void)
{
		__TowerLightGreenOff();
		__TowerLightYellowOff();
		__TowerLightRedOn();
}

//标记位初始化
void IODevice_StateInitial(void)
{
		OpticGate = false;
		Laser = false;
		Fan = false;
		Blow = false;
		Scan = false;
		SystemState = false;;
		ErrorState = false;
}

//------------------------------------光闸
void IODevice_OpticGateOn(bool on)
{
		if(on)
		{
				if(!OpticGate)
				{
						OpticGate = true;
						__OpticGateOn();
				}
		}
		else
		{
				if(OpticGate)
				{
						OpticGate = false;
						__OpticGateOff();
				}
		}
}

//-------------------------------------激光
void IODevice_LaserOn(bool on)
{
		if(on)
		{
				Laser = true;
				__LaserOn();
				__MonitorWaterEnable();
		}
		else
		{
				Laser = false;
				__LaserOff();
				__MonitorWaterDisable();
		}
}

//--------------------------------------风机
void IODevice_FanOn(bool on)
{
		if(on)
		{
				Fan = true;
				__FanOn();
		}
		else
		{
				Fan = false;
				__FanOff();
		}
}

 //-----------------------------吹风
void IODevice_BlowOn(bool on)
{
		if(on)
		{
				if(!Blow)
				{
						Blow = true;
						__BlowOn();
				}
		}
		else
		{
				if(Blow)
				{
						Blow = false;
						__BlowOff();
				}
		}
}

//-----------------------------------振镜
void IODevice_ScanOn(bool on)
{
		if(on)
		{
				if(!Scan)
				{
						Scan = true;
						__ScanOn();
				}
		}
		else
		{
				if(Scan)
				{
						Scan = false;
						__ScanOff();
				}
		}
}

void IODevice_AutoRun(void)
{
	if(!SystemState)
	{
			SystemState = true;
			IODevice_LaserOn(true);
			IODevice_ScanOn(true);
			IODevice_FanOn(true);
			IODevice_BlowOn(true);
			LCD_CountDown(50);
			IODevice_OpticGateOn(true);
			TowerLight_RunMode();
	}
}

void IODevice_AutoStop(void)
{
		if(SystemState)
		{
			SystemState = false;
			IODevice_LaserOn(false);
			IODevice_OpticGateOn(false);
			IODevice_ScanOn(false);
			LCD_CountDown(20);
			IODevice_FanOn(false);
			IODevice_BlowOn(false);
			TowerLight_IdleMode();
		}
}

void IODevice_ErrorHandle(void)
{
		ErrorState = true;
		SystemState = false;
		TowerLight_ErrorMode();
		IODevice_LaserOn(false);
		IODevice_OpticGateOn(false);
		IODevice_ScanOn(false);
		SysCtlDelay(SysCtlClockGet()*0.5);
		IODevice_FanOn(false);
		IODevice_BlowOn(false);
}

void IODevice_Initial(void)
{
		IODevice_StateInitial();
		TowerLight_IdleMode();
		IODevice_LaserOn(false);
		IODevice_OpticGateOn(false);
		IODevice_ScanOn(false);
		//SysCtlDelay(SysCtlClockGet()*0.5);
		IODevice_FanOn(false);
		IODevice_BlowOn(false);
}

//-------------------------------------------------------两个手动按键

uint32_t IODevice_GetHandModeOperation(void)
{
		uint32_t temp = ROM_GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_7) & GPIO_PIN_7;
		return (temp | (ROM_GPIOPinRead(GPIO_PORTD_BASE,GPIO_PIN_6)&GPIO_PIN_6));
}
