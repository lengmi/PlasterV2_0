#ifndef _IODEVICE_H_
#define _IODEVICE_H_
#include <stdint.h>
#include <stdbool.h>
#define __LaserOn() do{ \
											GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0xff); \
										} while(0);

#define __LaserOff() do{ \
											GPIOPinWrite(GPIO_PORTA_BASE, GPIO_PIN_6, 0x00); \
										} while(0);

#define __BlowOn() do{ \
											GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0xff); \
										} while(0);

#define __BlowOff() do{ \
											GPIOPinWrite(GPIO_PORTB_BASE, GPIO_PIN_4, 0x00); \
										} while(0);

#define __FanOn() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0xff); \
										} while(0);

#define __FanOff() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_5, 0x00); \
										} while(0);

#define __TowerLightRedOn() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0xff); \
										} while(0);

#define __TowerLightRedOff() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_1, 0x00); \
										} while(0);

#define __TowerLightGreenOn() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0xff); \
										} while(0);

#define __TowerLightGreenOff() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_2, 0x00); \
										} while(0);

#define __TowerLightYellowOn() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0xff); \
										} while(0);

#define __TowerLightYellowOff() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_3, 0x00); \
										} while(0);

#define __OpticGateOn() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0xff); \
										} while(0);

#define __OpticGateOff() do{ \
											GPIOPinWrite(GPIO_PORTE_BASE, GPIO_PIN_4, 0x00); \
										} while(0);

#define __ScanOn() do{ \
											 \
										} while(0);		

#define __ScanOff() do{ \
											 \
										} while(0);	

#define __MonitorWaterEnable() do{ \
																	GPIOIntEnable(GPIO_PORTB_BASE,GPIO_INT_PIN_3);\
															 }while(0)

#define __MonitorWaterDisable() do{ \
																	GPIOIntDisable(GPIO_PORTB_BASE,GPIO_INT_PIN_3);\
															}while(0)

extern void GPIO_Initial(void);
extern void TowerLight_IdleMode(void);
extern void TowerLight_RunMode(void);
extern void TowerLight_ErrorMode(void);
extern void IODevice_AutoRun(void);
extern void IODevice_AutoStop(void);
extern void IODevice_Initial(void);
extern void IODevice_OpticGateOn(bool on);
extern void IODevice_FanOn(bool on);
extern void IODevice_LaserOn(bool on);
extern void IODevice_BlowOn(bool on);
extern void IODevice_ScanOn(bool on);
										
extern uint32_t IODevice_GetHandModeOperation(void);
										
#endif
