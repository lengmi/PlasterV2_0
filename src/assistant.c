#include <string.h>
#include <stdio.h>
#include <math.h>
#include "assistant.h"
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/gpio.h"



void LED_LightOn(uint8_t color, bool on)
{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, color);
		if(on)
		{
			GPIOPinWrite(GPIO_PORTF_BASE,color,0xff);
		}
		else
		{
			GPIOPinWrite(GPIO_PORTF_BASE,color,0x00);
		}		
}

void StrConv_Int2Str(char * str, uint8_t units, int32_t intVar)
{
		unsigned int i = 0;
		if(units <20)
		{
				for(i=0;i<units;i++)
				{
						*(str+units-1-i)='0'+intVar%10;
						intVar/=10;
				}
				*(str+units)='\0';
		}
}

void StrConv_Float2Str(char *strFloat, float floatData, int units)
{
  char tmp[20];
	sprintf(strFloat, "%f", floatData);
	while(strlen(strFloat)<units+1)
	{
		strcat(strFloat,"0");
	}

	if(strlen(strFloat)>units+1)
	{
		strncpy(tmp,strFloat,units+1);
		strcpy(strFloat,tmp);
		strFloat[units+1] = '\0';
	}
}

void StrConv_AddPrefixAndSubfix(char *str, char *strPrefix, char *subFix)
{
    char tmp[50];
    strcpy(tmp,strPrefix);
    strcat(tmp,str);
    strcat(tmp,subFix);
    strcpy(str,tmp);
}

void CountPointArray(int32_t *x, int32_t *y, double D, uint32_t amount)
{
		double thata=2*3.141593/amount;
		uint32_t i=0;
		double temp = 0.0;
		for(i=0;i<amount;i++)
		{
				*(x+i)= (int32_t)(D/2)*cos(temp)*500;
				*(y+i)= (int32_t)(D/2)*sin(temp)*500;
				temp += thata;
		}
}
