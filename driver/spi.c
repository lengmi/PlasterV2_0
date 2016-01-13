#include "spi.h"
#include "assistant.h"

//------------------------------------LCD-------------------------------------------
//初始化SPI端口
void SSI0_InitialSPI(void)     
{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);

		GPIOPinConfigure(GPIO_PA2_SSI0CLK);
		GPIOPinConfigure(GPIO_PA3_SSI0FSS);
		GPIOPinConfigure(GPIO_PA5_SSI0TX);
		GPIOPinConfigure(GPIO_PA4_SSI0RX);
		GPIOPinTypeSSI(GPIO_PORTA_BASE,GPIO_PIN_5|GPIO_PIN_4|GPIO_PIN_3|GPIO_PIN_2);

		SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER,50000, 16);
		SSIEnable(SSI0_BASE);
		SSI0_FlushFIFO();
}

//SSI0传输数据
void SSI0_Write(uint16_t data)
{
		while(SSIBusy(SSI0_BASE));
		SSIDataPut(SSI0_BASE, data);
}

//SSI0接收数据
uint16_t SSI0_Read(void)
{
		uint32_t dataTmp=0;
		while(SSIBusy(SSI0_BASE));
		SSIDataGet(SSI0_BASE, &dataTmp);
		//SSIDataPut(SSI0_BASE,0x00);
		SSIDataGet(SSI0_BASE, &dataTmp);
		return (uint16_t)dataTmp;
}

//SSI0清空FIFO缓存
void SSI0_FlushFIFO(void)
{
		uint32_t dataTmp=0;
		while(SSIDataGetNonBlocking(SSI0_BASE, &dataTmp));
}

////丢弃多余的字节
//void SSI0_ThrowDummyData(void)
//{
//		uint32_t dataTmp=0;
//		SSIDataGet(SSI0_BASE, &dataTmp);
//}

//-----------------------------------振镜--------------------------------------------
void SPI_Open(void)
{
		__PutClkLow();
		__PutSyncLow();
		__PutXHigh();
		__PutYHigh();
		__PutSyncHigh();
}

void SPI_Close(void)
{
		__PutClkHigh();
		__PutSyncLow();
		__PutXHigh();
		__PutYHigh();
		__PutClkLow();
		__Delay5us();
}

void SPI_Send(uint8_t valueX, uint8_t valueY)
{
		uint8_t i,temp;
		for(i=0;i<8;i++)
		{
				__PutClkHigh();
				temp = valueX << i;
				if(temp>=0x80)
				{
						__PutXHigh();
				}
				else
				{
						__PutXLow();
				}
				temp = valueY << i;
				if(temp >= 0x80)
				{
						__PutYHigh();
				}
				else
				{
						__PutYLow();
				}
				__Delay5us();
				__PutClkLow();
				__Delay5us();
		}
}

void Scan_WriteData(uint16_t valueX, uint16_t valueY)
{
		uint8_t valXL,valYL,valXH,valYH;
		valYH = (uint8_t)(valueY>>8);
		valXH = (uint8_t)(valueX>>8);
		valYL = (uint8_t)valueY;
		valXL = (uint8_t)valueX;
		SPI_Open();
		SPI_Send(valXH,valYH);
		SPI_Send(valXL,valYL);
		SPI_Close();
}
