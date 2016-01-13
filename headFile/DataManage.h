#ifndef _DATAMANAGE_H_
#define _DATAMANAGE_H_

#include <stdint.h>
//#include <stdbool.h>

#define ADDR_UNIT 16

extern void DataManage_ReadData(uint32_t addr);
extern void DataManage_StoreData(uint32_t addr);
#endif
