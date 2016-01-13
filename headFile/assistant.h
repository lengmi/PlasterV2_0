#ifndef _ASSISTANT_H_
#define _ASSISTANT_H_

#include <stdint.h>
#include <stdbool.h>

#define STEP 500

#define LEDRED GPIO_PIN_1
#define LEDGREEN GPIO_PIN_2
#define LEDBLUE GPIO_PIN_3
#define LEDLIGHTOFF 0x00

#define __Delay5us() do\
									{ SysCtlDelay(10); }\
									while(0);

#define __Delay100us() do\
									{ SysCtlDelay(SysCtlClockGet()/10000); }\
									while(0);

#define __Delay1ms() do\
									{ SysCtlDelay(SysCtlClockGet()/1000); }\
									while(0);
#define __Delay10ms() do\
									{ SysCtlDelay(SysCtlClockGet()/100); }\
									while(0);
#define __Delay50ms() do\
									{ SysCtlDelay(SysCtlClockGet()/20); }\
									while(0);
#define __Delay100ms() do\
									{ SysCtlDelay(SysCtlClockGet()/10); }\
									while(0);
#define __Delay1s() do\
									{ SysCtlDelay(SysCtlClockGet()); }\
									while(0);

extern void LED_LightOn(uint8_t color, bool on);
extern void StrConv_Int2Str(char * str, uint8_t units, int32_t intVar);
extern void StrConv_Float2Str(char *strFloat, float floatData, int units);
extern void StrConv_AddPrefixAndSubfix(char *str, char *strPrefix, char *subFix);
#endif
