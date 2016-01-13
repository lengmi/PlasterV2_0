#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_types.h"
#include "inc/hw_memmap.h"
#include "driverlib/sysctl.h"
#include "driverlib/eeprom.h"
#include "driverlib/rom.h"
#include "DataManage.h"

double Diameter = 0.0;
double Distance = 0.0;
uint32_t PointAmount = 0;
uint32_t HoleAmount = 0;

void DataManage_ReadData(uint32_t addr)
{
		uint32_t para[4] ={0};
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
		while(ROM_EEPROMInit()==EEPROM_INIT_ERROR);
		ROM_EEPROMRead(para,addr*ADDR_UNIT,sizeof(para));
		Diameter = ((double)para[0])/10.0;
		PointAmount = para[1];
		Distance = (double)para[2]/10.0;
		HoleAmount = para[3];
		SysCtlDelay(1000);
		ROM_SysCtlPeripheralDisable(SYSCTL_PERIPH_EEPROM0);
}

void DataManage_StoreData(uint32_t addr)
{
		uint32_t para[4] ={0};
		para[0] = (uint32_t)(Diameter *10);
		para[1] = PointAmount;
		para[2] = (uint32_t) Distance*10;
		para[3] = HoleAmount;
		ROM_SysCtlPeripheralEnable(SYSCTL_PERIPH_EEPROM0);
		while(ROM_EEPROMInit()==EEPROM_INIT_ERROR);
		ROM_EEPROMProgram(para,addr*ADDR_UNIT,sizeof(para));
		ROM_SysCtlDelay(1000);
		ROM_SysCtlPeripheralDisable(SYSCTL_PERIPH_EEPROM0);
}
