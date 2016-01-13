#ifndef _SPI_H_
#define _SPI_H_

#include <stdint.h>
#include <stdbool.h>
#include "inc/hw_memmap.h"
#include "inc/hw_ssi.h"
#include "inc/hw_types.h"
#include "driverlib/ssi.h"
#include "driverlib/gpio.h"
#include "driverlib/pin_map.h"
#include "driverlib/sysctl.h"

#define __PutSyncHigh() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3, 0xff);}\
												while(0);
#define __PutSyncLow() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_3, 0x00);}\
												while(0);
												
#define __PutClkHigh() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2, 0xff);}\
												while(0);
#define __PutClkLow() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_2, 0x00);}\
												while(0);
												
#define __PutXHigh() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1, 0xff);}\
												while(0);
#define __PutXLow() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_1, 0x00);}\
												while(0);
												
#define __PutYHigh() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0, 0xff);}\
												while(0);
#define __PutYLow() do \
												{GPIOPinWrite(GPIO_PORTD_BASE,GPIO_PIN_0, 0x00);}\
												while(0);

												

extern void SSI0_InitialSPI(void);
extern void SSI0_Write(uint16_t data);
extern uint16_t SSI0_Read(void);
extern void SSI0_FlushFIFO(void);
//extern void SSI0_ThrowDummyData(void);

extern void Scan_WriteData(uint16_t valueX, uint16_t valueY);
#endif
