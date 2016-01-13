#include "spi.h"
#include "RA8875.h"
#include "assistant.h"

//–¥ºƒ¥Ê∆˜µÿ÷∑
void LCD_CmdWrite(uint8_t addr)
{
		uint16_t tmp = PREFIXWRITECMD | addr;
		SSI0_Write(tmp);
		//SSI0_ThrowDummyData();
}

//–¥ ˝æ›
void LCD_DataWrite(uint8_t data)
{
		uint16_t tmp = PREFIXWRITEDATA | data;
		SSI0_Write(tmp);
		//SSI0_ThrowDummyData();
}

//∂¡ ˝æ›
uint8_t LCD_DataRead(void)
{
		SSI0_FlushFIFO();
		SSI0_Write(PREFIXREADDATA);
		uint16_t tmp = SSI0_Read();
	  return (uint8_t)(tmp%0x100);
}

///////////////check busy
void LCD_ChkBusy(void)
{
	uint8_t temp; 	
	do
	{
		temp=LCD_StatusRead();
		__Delay1ms();
	}while((temp&0x80)==0x80);		   
}
///////////////check bte busy
void Chk_BTE_Busy(void)
{
	uint8_t temp; 	
	do
	{
	temp=LCD_StatusRead();
	}while((temp&0x40)==0x40);		   
}
///////////////check dma busy
void Chk_DMA_Busy(void)
{
	uint8_t temp; 	
	do
	{
	LCD_CmdWrite(0xBF);
	temp =LCD_DataRead();
	}while((temp&0x01)==0x01);   
} 

//”≤º˛∏¥Œª
void LCD_HardReset(void)
{
		SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);
		GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_4);
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0x00);
		__Delay100ms();
		GPIOPinWrite(GPIO_PORTF_BASE,GPIO_PIN_4,0xff);
}


//register: Status Register, RO
uint8_t LCD_StatusRead(void)
{
		SSI0_FlushFIFO();
		SSI0_Write(PREFIXREADSTATUS);
		uint16_t tmp = SSI0_Read();
	  return (uint8_t)(tmp%0x100);
}

//-------------------------------------System & Configuration Register-------------------------------------

//0x01: Power and Display Control register (PWRR)

void LCD_DisplayOn(bool on)
{
	LCD_CmdWrite(0x01);
	if(on)
	{
		LCD_DataWrite(0x80);
	}
	else
	{
		LCD_DataWrite(0x00);
	}
}

//»Ìº˛∏¥Œª£¨ø…ƒ‹‘Ï≥…œ‘ æ∆¡“Ï≥££¨≤ªΩ®“È π”√
void LCD_SoftReset(void)
{
		LCD_CmdWrite(0x01);
		LCD_DataWrite(0x01);
		LCD_DataWrite(0x00);
		SysCtlDelay(SysCtlClockGet()/10);
}

//0x02: Memory Read/Write Command (MRWC)
//////////////writing text
void LCD_StringDisplay(char *str)
{   
		LCD_TextModeSet();
		LCD_CmdWrite(0x02);
		while(*str != '\0')
		{
			LCD_DataWrite(*str);
			++str;	 	
			LCD_ChkBusy();		
		} 
}

//0x04: Pixel Clock Setting Register (PCSR)
void PCLK_Set(void)
{
		LCD_CmdWrite(0x04);    //PCLK
		LCD_DataWrite(0x81);   //Pixel Data fetch @ falling edge; PCLK=2*Sys Clock
}
//0x05: Serial Flash/Rom Configure Register (SROC)
void SerialFlash_Reset(void)
{
		LCD_CmdWrite(0x05);
		LCD_DataWrite(0x00);
}

void SerialFlash_Select(uint8_t option)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x05);
		temp = LCD_DataRead()|(option & cSetD7);
		LCD_DataWrite(temp);
}

void SerialFlash_Mode3Set(void)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x05);
		temp = LCD_DataRead()|(cSetD5);
		LCD_DataWrite(temp);
}

void SerialFlash_Read5Bus(void)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x05);
		temp = (LCD_DataRead()|cSetD3)& cClrD4;
		LCD_DataWrite(temp);
}	

void SerialFlash_DMAModeSet(void)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x05);
		temp = LCD_DataRead()|(cSetD2);
		LCD_DataWrite(temp);
}

void SerialFlash_DataLatchModeSet(uint8_t mode)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x05);
		temp = LCD_DataRead()| (mode & 0x03);
		LCD_DataWrite(temp);
}

void SerialFlash_PicDispModeSet(void)
{
		LCD_CmdWrite(0x05);
		LCD_DataWrite(cSetD7|cSetD5|cSetD3|cSetD2);
}
//0x06: Serial Flash/Rom Clock Setting Register (SFCLR)
void SerialFlash_ClockDivSet(uint8_t clkDiv)
{
		LCD_CmdWrite(0x06);
		LCD_DataWrite(clkDiv);
}
//0x10: System Configuration Register (SYSR)
void SYSR_ColorDepthSet(void)
{
		LCD_CmdWrite(0x10);
		LCD_DataWrite(0x0c);  //16bpp, 5:R,6:G,5:B
}	
//0x12: General Purpose Input (GPI)

//0x13: General Purpose Output (PGO)

//0x14: LCD Horizontal Display Width Register (HDWR)
void HDWR_Set(uint16_t horizonPixels)
{
		uint8_t tmp = (uint8_t)(horizonPixels/8-1);
		LCD_CmdWrite(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                   
		LCD_DataWrite(tmp);//Horizontal display width(pixels) = (HDWR + 1)*8                      
}
//0x15: Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
void HNDFTR_Set(void)
{	
		LCD_CmdWrite(0x15); //Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
		LCD_DataWrite(0x03);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0] 
}
//0x16: LCD Horizontal Non-Display Period Register (HNDR)
void HNDR_Set(void)
{
		LCD_CmdWrite(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                      
		LCD_DataWrite(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8 
}
//0x17: HSYNC Start Position Register (HSTR)
void HSTR_Set(void)
{
		LCD_CmdWrite(0x17); //HSTR//HSYNC Start Position[4:0]                                   
		LCD_DataWrite(0x02);//HSYNC Start Position(PCLK) = (HSTR + 1)*8
}	
//0x18: HSYNC Pulse Width Register (HPWR)
void HPWR_Set(void)
{
		LCD_CmdWrite(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                  
		LCD_DataWrite(0x00);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8  
}

void LCD_HorizontalParaSet(uint16_t horizonPixels)
{
		HDWR_Set(800);
		HNDFTR_Set();
		HNDR_Set();
		HSTR_Set();
		HPWR_Set();
}
//0x19: LCD Vertical Display Height Register (VDHR0)
//0x1a: LCD Vertical Display Height Register0 (VDHR1)
void LCD_VerticalPixelSet(uint16_t verticalPixels)
{
		uint8_t tmp = (uint8_t)(verticalPixels%0x100-1);
		LCD_CmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                         
		LCD_DataWrite(tmp);//Vertical pixels = VDHR + 1   
		tmp = (uint8_t)verticalPixels/0x100;
		LCD_CmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]                           
		LCD_DataWrite(tmp);//Vertical pixels = VDHR + 1 
}
//0x1b: LCD Vertical Non-Display Period Register (VNDR0)
void VNDR0_Set(void)
{
		LCD_CmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                     
		LCD_DataWrite(0x14);//Vertical Non-Display area = (VNDR + 1)
}
//0x1c: LCDVertical Non-Display Period Register (VNDR1)
void VNDR1_Set(void)
{	
		LCD_CmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                       
		LCD_DataWrite(0x00);//Vertical Non-Display area = (VNDR + 1) 
}
//0x1d: VSYNC Start Position Register (VSTR0)
void VSTR0_Set()
{
		LCD_CmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                 
		LCD_DataWrite(0x06);//VSYNC Start Position(PCLK) = (VSTR + 1) 
}
//0x1e: VSYNC Start Position Register (VSTR1)
void VSTR1_Set(void)
{
		LCD_CmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]                                   
		LCD_DataWrite(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)  
}
//0x1f: VSYNC Pulse Width Register (VPWR)
void VPWR_Set(void)
{	
		LCD_CmdWrite(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                     
		LCD_DataWrite(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)
}

void LCD_VerticalParaSet(uint16_t verticalPixels)
{
		LCD_VerticalPixelSet(480);
		VNDR0_Set();
		VNDR1_Set();
		VSTR0_Set();
		VSTR1_Set();
		VPWR_Set();
}
//--------------------------------------LCD Display Control Register------------------------------------

//0x20: Display Configuration Register (DPCR)
void LCD_LayerMode(bool oneLayer)
{
		uint8_t temp = 0;
		LCD_CmdWrite(0x20);//DPCR
		temp = LCD_DataRead();
		if(oneLayer)
		{
			temp &= cClrD7 ;
		}
		else
		{
			temp |= cSetD7;
		}
		LCD_DataWrite(temp);
}
//0x21: Font Control Register0 (FNCR0)
void FontCtl_ExternelCGROMEnable(bool en)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x21);
		temp = LCD_DataRead();
		if(en)
		{
				LCD_DataWrite(0x20|temp);
		}
		else
		{
				LCD_DataWrite(cClrD4&temp);
		}

}

//0x22: Font Control Register1 (FNCR1)
void FontCtl_FullAlignment(bool full)
{
		uint8_t temp;
		LCD_CmdWrite(0x22);//FNCR0
		temp = LCD_DataRead();
		if(full)
		{
			temp |= cSetD7;
		}
		else
		{
			temp &= cClrD7;
		}
		LCD_CmdWrite(0x22);
		LCD_DataWrite(temp);
}

void FontCtl_ShowBackgroundColor(bool show)
{
		uint8_t temp;
		LCD_CmdWrite(0x22);//FNCR0
		temp = LCD_DataRead();
		if(show)
		{
			temp |= cSetD6;
		}
		else
		{
			temp &= cClrD6;
		}
		LCD_CmdWrite(0x22);
		LCD_DataWrite(temp);
}

void FontCtl_Rotate(bool rotate)
{
		uint8_t temp;
		LCD_CmdWrite(0x22);//FNCR0
		temp = LCD_DataRead();
		if(rotate)
		{
			temp |= cSetD4;
		}
		else
		{
			temp &= cClrD4;
		}
		LCD_CmdWrite(0x22);
		LCD_DataWrite(temp);
}

void FontCtl_Enlarge(uint8_t xdir, uint8_t ydir)
{
		uint8_t temp = 0;
		if(xdir<=3&&ydir<=3)
		{
			LCD_CmdWrite(0x22);
			temp = LCD_DataRead()|((xdir << 2)|ydir);
			LCD_CmdWrite(0x22);
			LCD_DataWrite(temp);
		}
}
//0x23: CGRAM Select Register (CGSR)

//0x24: Horizontal Scroll Offset Register0 (HOFS0)

//0x25: Horizontal Scroll Offset Register1 (HOFS1)
void Scroll_HorizontalOffset(uint16_t offset)
{
		uint8_t temp = (uint8_t)(offset%0x100);
		LCD_CmdWrite(0x24);
		LCD_DataWrite(temp);
		temp = (uint8_t)(offset/0x100);
		temp &= 0x07;
		LCD_CmdWrite(0x25);
		LCD_DataWrite(temp);
}	

//0x26: Vertical Scroll Offset Register 0 (VOFS0)

//0x27: Vertical Scroll Offset Register 1 (VOFS1)
void Scroll_VerticalOffset(uint16_t offset)
{
		uint8_t temp = (uint8_t)(offset%0x100);
		LCD_CmdWrite(0x26);
		LCD_DataWrite(temp);
		temp = (uint8_t)(offset/0x100);
		temp &= 0x03;
		LCD_CmdWrite(0x27);
		LCD_DataWrite(temp);
}

//0x29: Font Line Distance Setting Register (FLDR)––º‰æ‡
void FontCtl_LineDistanceSet(uint8_t lineDistance)
{
		lineDistance &= 0x0f;
		LCD_CmdWrite(0x29);
		LCD_DataWrite(lineDistance);
}
//0x2a: Font Write Cursor Horizontal Position Register 0 (F_CURXL)

//0x2b: Font Write Cursor Horizontal Position Register 1 (F_CURXH)
void FontCtl_CursorXPositionSet(uint16_t XPosition)
{
		uint8_t temp = (uint8_t)(XPosition%0x100);
		LCD_CmdWrite(0x2a);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XPosition/0x100);
		temp &= 0x03;
		LCD_CmdWrite(0x2b);
		LCD_DataWrite(temp);
}
//0x2c: Font Write Cursor Vertical Position Register 0 (F_CURYL)

//0x2d: Font Write Cursor Vertical Position Register 1 (F_CURYH)
void FontCtl_CursorYPositionSet(uint16_t YPosition)
{
		uint8_t temp = (uint8_t)(YPosition%0x100);
		LCD_CmdWrite(0x2c);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YPosition/0x100);
		temp &= 0x01;
		LCD_CmdWrite(0x2d);
		LCD_DataWrite(temp);
}

void FontCtl_CursorPositionSet(uint16_t XPosition, uint16_t YPosition)
{
		FontCtl_CursorXPositionSet(XPosition);
		FontCtl_CursorYPositionSet(YPosition);
}
//0x2e: Font Write Type Setting Register (◊÷º‰æ‡”Î◊÷¥Û–°) 
//size: 0-16*16;1-24*24;2-32*32;
//width: ◊÷∑˚º‰æ‡÷µ
void FontCtl_FontSizeWidthSet(uint8_t size, uint8_t width)
{
		uint8_t temp = (size<<6&0xc0)|(width&0x3f);
		LCD_CmdWrite(0x2e);
		LCD_DataWrite(temp);
}


//0x2f: Serial Font ROM Setting Register (∏Ωº”◊÷ø‚—°‘Ò)
void FontCtl_ExternelROMFontSet(uint8_t fontRom, uint8_t fontCoding, uint8_t font)
{
		uint8_t temp = 0;
		temp = (fontRom << 5) & 0xe0;
		temp |= (fontCoding << 2) & 0xfc;
		temp |= font & 0x3;
		LCD_CmdWrite(0x2f);
		LCD_DataWrite(temp);
}

void FontCtl_ExternelROMFontSelect(void)
{
		FontCtl_ExternelCGROMEnable(1);
		FontCtl_ExternelROMFontSet(0x4,0x00,0x00);
}

void FontCtl_CNZHDisplaySet(void)
{
		FontCtl_ExternelROMFontSelect();
		FontCtl_Rotate(0);
		FontCtl_Enlarge(0,0);
		FontCtl_FontSizeWidthSet(0x02,0x00);
		FontCtl_LineDistanceSet(0x00);
}

//----------------------------Active Window and Scroll Window Setting Register-------------------

//0x30: Horizontal Start Point 0 of Active Window (HSAW0)

//0x31: Horizontal Start Point 1 of Active Window (HSAW1)
void Window_ActiveXStartPointSet(uint16_t XStart)
{
		uint8_t temp = XStart%0x100;
		LCD_CmdWrite(0x30);
		LCD_DataWrite(temp);
		temp = XStart/0x100;
		LCD_CmdWrite(0x31);
		LCD_DataWrite(temp);
}
//0x32: Vertical Start Point 0 of Active Window (VSAW0)

//0x33: Vertical Start Point 1 of Active Window (VSAW0)
void Window_ActiveYStartPointSet(uint16_t YStart)
{
		uint8_t temp = YStart%0x100;
		LCD_CmdWrite(0x32);
		LCD_DataWrite(temp);
		temp = YStart/0x100;
		LCD_CmdWrite(0x33);
		LCD_DataWrite(temp);
}
//0x34: Horizontal End Point 0 of Active Window (HEAW0)
//0x35: Horizontal End Point 1 of Active Window (HEAW1)
void Window_ActiveXEndPointSet(uint16_t XEnd)
{
		uint8_t temp = XEnd%0x100;
		LCD_CmdWrite(0x34);
		LCD_DataWrite(temp);
		temp = XEnd/0x100;
		LCD_CmdWrite(0x35);
		LCD_DataWrite(temp);
}


//0x36: Vertical End Point 0 of Active Window (VEAW0)
//0x37: Vertical End Point 1 of Active Window (VEAW0)
void Window_ActiveYEndPointSet(uint16_t YEnd)
{
		uint8_t temp = YEnd%0x100;
		LCD_CmdWrite(0x36);
		LCD_DataWrite(temp);
		temp = YEnd/0x100;
		LCD_CmdWrite(0x37);
		LCD_DataWrite(temp);
}

void Window_ActivePositionSet(uint16_t XStart, uint16_t XEnd, uint16_t YStart, uint16_t YEnd)
{
		Window_ActiveXStartPointSet(XStart);
		Window_ActiveXEndPointSet(XEnd);
		Window_ActiveYStartPointSet(YStart);
		Window_ActiveYEndPointSet(YEnd);
}
//0x38: Horizontal Start Point 0 of Scoll Window (HSSW0)
//0x39: Horizontal Start Point 1 of Scoll Window (HSSW1)
void Window_ScrollXStartPointSet(uint16_t XStart)
{
		uint8_t temp = XStart%0x100;
		LCD_CmdWrite(0x38);
		LCD_DataWrite(temp);
		temp = XStart/0x100;
		LCD_CmdWrite(0x39);
		LCD_DataWrite(temp);
}
//0x3a: Vertical Start Point 0 of Scoll Window (VSSW0)
//0x3b: Vertical Start Point 1 of Scoll Window (VSSW1)
void Window_ScrollYStartPointSet(uint16_t YStart)
{
		uint8_t temp = YStart%0x100;
		LCD_CmdWrite(0x3a);
		LCD_DataWrite(temp);
		temp = YStart/0x100;
		LCD_CmdWrite(0x3b);
		LCD_DataWrite(temp);
}
//0x3c: Horizontal End Point 0 of Scoll Window (HESW0)
//0x3d: Horizontal End Point 1 of Scoll Window (HESW1)
void Window_ScrollXEndPointSet(uint16_t XEnd)
{
		uint8_t temp = XEnd%0x100;
		LCD_CmdWrite(0x3c);
		LCD_DataWrite(temp);
		temp = XEnd/0x100;
		LCD_CmdWrite(0x3d);
		LCD_DataWrite(temp);
}

//0x3e: Vertical End Point 0 of Scoll Window (VESW0)
//0x3f: Vertical End Point 1 of Scoll Window (VESW0)
void Window_ScrollYEndPointSet(uint16_t YEnd)
{
		uint8_t temp = YEnd%0x100;
		LCD_CmdWrite(0x3e);
		LCD_DataWrite(temp);
		temp = YEnd/0x100;
		LCD_CmdWrite(0x3f);
		LCD_DataWrite(temp);
}

void Window_ScrollPositionSet(uint16_t XStart, uint16_t XEnd, uint16_t YStart, uint16_t YEnd)
{
		Window_ScrollXStartPointSet(XStart);
		Window_ScrollXEndPointSet(XEnd);
		Window_ScrollYStartPointSet(YStart);
		Window_ScrollYEndPointSet(YEnd);
}

//--------------------------------------------Cursor Setting Registers------------------------------------

//0x40: Memory Write Control Register 0 (MWCR0) 
void Cursor_ModeCtl(bool textMode, bool cursorEn, bool blinkEn, bool autoIncEn)
{
		uint8_t temp = 0;
		temp |= (textMode << 7);
		temp |= (cursorEn << 6);
		temp |= (blinkEn << 5);
		temp |= (autoIncEn << 1);
		LCD_CmdWrite(0x40);
		LCD_DataWrite(temp);
}

void LCD_TextModeSet(void)
{
		Cursor_ModeCtl(1,0,0,0);
}

void LCD_GraphicModeSet(void)
{
		Cursor_ModeCtl(0,0,0,0);
}

//0x41: Memory Write Control Register 1 (MWCR1) 

//0x44: Blink Time Control Register (BTCR)

//0x45: Memory Read Cursor Direction (MRCD)

//0x46: Memory Write Cursor Horizontal Position Register 0 (CURH0)
//0x47: Memory Write Cursor Horizontal Position Register 1 (CURH1)
void Memory_CursorXPositiomSet(uint16_t XPosition)
{
		LCD_CmdWrite(0x46);
		LCD_DataWrite((uint8_t)XPosition);
		LCD_CmdWrite(0x47);
		LCD_DataWrite((uint8_t)(XPosition >> 8));
}
//0x48: Memory Write Cursor Vertical Position Register 0 (CURV0)
//0x49: Memory Write Cursor Vertical Position Register 1 (CURV1)
void Memory_CursorYPositiomSet(uint16_t YPosition)
{
		LCD_CmdWrite(0x48);
		LCD_DataWrite((uint8_t)YPosition);
		LCD_CmdWrite(0x49);
		LCD_DataWrite((uint8_t)(YPosition >> 8));
}

void Memory_CursorPositionSet(uint16_t XPosition, uint16_t YPosition)
{
		Memory_CursorXPositiomSet(XPosition);
		Memory_CursorYPositiomSet(YPosition);
}
//0x4a: Memory Read Cursor Horizontal Position Register 0 (RCURH0)

//0x4b: Memory Read Cursor Horizontal Position Register 1 (RCURH1)

//0x4c: Memory Read Cursor Vertical Position Register 0 (RCURV0)

//0x4d: Memory Read Cursor Vertical Position Register 1 (RCURV1)

//0x4e: Font Write Cursor and Memory Write Cursor Horizontal Size Register (CURHS)

//0x4f: Font Write Cursor and Memory Write Cursor Vertical Size Register (CURVS)

//---------------------------------------------Block Transfer Engine (BTE) Control Register-----------------------

//0x50: BTE Function Control Register 0 (BTECR0)
void BTE_FunctionCtlSet(bool en, bool sourceDataMode, bool destDataMode)
{
		uint8_t temp = ((en << 7) & cSetD7) | ((sourceDataMode << 6) & cSetD6) \
										| ((destDataMode << 5) & cSetD5);
		LCD_CmdWrite(0x50);
		LCD_DataWrite(temp);
}	
//0x51: BTE Function Control Register 1 (BTECR1)
void BTE_CodeSet(uint8_t ROPCode, uint8_t operationCode)
{
		uint8_t temp = ((ROPCode << 4) & 0x0f) | (operationCode & 0x0f);
		LCD_CmdWrite(0x51);
		LCD_DataWrite(temp);
}
//0x52: Layer Transparency Register 0 (LTPR0)
void BTE_LayerCtlSet(uint8_t scrollMode, bool FLWTranEn, uint8_t layerMode)
{
		uint8_t temp = ((scrollMode << 6) & (cSetD7 | cSetD6))\
										| ((FLWTranEn << 5) & cSetD5) | (layerMode & 0x07);
		LCD_CmdWrite(0x52);
		LCD_DataWrite(temp);
}
//0x53: Layer Transparency Register 1 (LTPR1)  Õ∏√˜∂»
void BTE_LayerCtlTansSet(uint8_t layer1, uint8_t layer2)
{
		uint8_t temp = ((layer1 << 4) & 0xf0) | (layer2 & 0x0f);
		LCD_CmdWrite(0x53);
		LCD_DataWrite(temp);
}
//0x54: Horizontal Source Point 0 of BTE (HSBE0)
//0x55: Horizontal Source Point 1 of BTE (HSBE1)
void BTE_XSourcePointSet(uint16_t XSource)
{
		uint8_t temp = (uint8_t)(XSource % 0x100);
		LCD_CmdWrite(0x54);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XSource / 0x100);
		LCD_CmdWrite(0x55);
		LCD_DataWrite(temp);
}
//0x56: Vertical Source Point 0 of BTE (VSBE0)
//0x57: Vertical Source Point 1 of BTE (VSBE1)
void BTE_YSourcePointSet(uint16_t YSource)
{
		uint8_t temp = (uint8_t)(YSource % 0x100);
		LCD_CmdWrite(0x56);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YSource / 0x100);
		LCD_CmdWrite(0x57);
		LCD_DataWrite(temp);
}
//0x58: Horizontal Destination Point 0 of BTE (HDBE0)
//0x59: Horizontal Destination Point 1 of BTE (HDBE1)
void BTE_XDstPointSet(uint16_t XDst)
{
		uint8_t temp = (uint8_t)(XDst % 0x100);
		LCD_CmdWrite(0x58);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XDst / 0x100);
		LCD_CmdWrite(0x59);
		LCD_DataWrite(temp);
}
//0x5a: Vertical Destination Point 0 of BTE (VDBE0) 
//0x5b: Vertical Destination Point 1 of BTE (VDBE1)
void BTE_YDstPointSet(uint16_t YDst)
{
		uint8_t temp = (uint8_t)(YDst % 0x100);
		LCD_CmdWrite(0x5a);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YDst / 0x100);
		LCD_CmdWrite(0x5b);
		LCD_DataWrite(temp);
}
//0x5c: BTE Width Register 0 (BEWR0)
//0x5d: BTE Width Register 1 (BEWR1)
void BTE_WidthSet(uint16_t width)
{
		uint8_t temp = (uint8_t)(width % 0x100);
		LCD_CmdWrite(0x5c);
		LCD_DataWrite(temp);
		temp = (uint8_t)(width / 0x100);
		LCD_CmdWrite(0x5d);
		LCD_DataWrite(temp);
}
//0x5e: BTE Height Register 0 (BEHR0)
//0x5f: BTE Height Register 1 (BEHR0)
void BTE_HeightSet(uint16_t height)
{
		uint8_t temp = (uint8_t)(height % 0x100);
		LCD_CmdWrite(0x5e);
		LCD_DataWrite(temp);
		temp = (uint8_t)(height / 0x100);
		LCD_CmdWrite(0x5f);
		LCD_DataWrite(temp);
}
//0x60: Background Color Register 0 (BGCR0) 
///////////////Background color settings
//0x61: Background Color Register 1 (BGCR1)
//0x62: Background Color Register 2 (BGCR2)
void BTE_BackgroundColorSet(uint8_t setR, uint8_t setG, uint8_t setB)
{
    LCD_CmdWrite(0x60);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x61);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x62);//BGCR0
	LCD_DataWrite(setB);
} 

void BTE_BackgroundColorSetMix(uint32_t backColor)
{
	
		LCD_CmdWrite(0x60);//BGCR0
		LCD_DataWrite((uint8_t)(backColor>>11));
		
		LCD_CmdWrite(0x61);//BGCR0
		LCD_DataWrite((uint8_t)(backColor>>5));
		
		LCD_CmdWrite(0x62);//BGCR0
		LCD_DataWrite((uint8_t)(backColor));
} 
//0x63: Foreground Color Register 0 (FGCR0)
//0x64: Foreground Color Register 1 (FGCR1)
//0x65: Foreground Color Register 2 (FGCR2)
void BTE_ForegroundColorSetMix(uint32_t foreColor)
{
	
		LCD_CmdWrite(0x63);//BGCR0
		LCD_DataWrite((uint8_t)(foreColor>>11));
		
		LCD_CmdWrite(0x64);//BGCR0
		LCD_DataWrite((uint8_t)(foreColor>>5));
		
		LCD_CmdWrite(0x65);//BGCR0
		LCD_DataWrite((uint8_t)(foreColor));
} 

void BTE_ForegroundColorSet(uint8_t setR,uint8_t setG,uint8_t setB)
{	    
    LCD_CmdWrite(0x63);//BGCR0
		LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x64);//BGCR0
		LCD_DataWrite(setG);

    LCD_CmdWrite(0x65);//BGCR0∑
		LCD_DataWrite(setB);
}
//0x66: Pattern Set No for BTE (PTNO)
void BTE_PatternNOSet(uint8_t pattern)
{
		LCD_CmdWrite(0x66);//BGCR0∑
		LCD_DataWrite(pattern);
}

//0x67: BackGround Color Register for Transparent 0 (BGTR0)
//0x68: BackGround Color Register for Transparent 1 (BGTR1)
//0x69: Background Color Register for Transparent 2 (BGTR2)
void BTE_BackgroundTranColorSet(uint8_t setR, uint8_t setG, uint8_t setB)
{
    LCD_CmdWrite(0x67);//BGCR0
	LCD_DataWrite(setR);
   
    LCD_CmdWrite(0x68);//BGCR0
	LCD_DataWrite(setG);

    LCD_CmdWrite(0x69);//BGCR0
	LCD_DataWrite(setB);
} 

void BTE_BackgroundTranColorSetMix(uint32_t backColor)
{
	
		LCD_CmdWrite(0x67);//BGCR0
		LCD_DataWrite((uint8_t)(backColor>>11));
		
		LCD_CmdWrite(0x68);//BGCR0
		LCD_DataWrite((uint8_t)(backColor>>5));
		
		LCD_CmdWrite(0x69);//BGCR0
		LCD_DataWrite((uint8_t)(backColor));
} 
//--------------------------------------Touch Panel Control Registers--------------------------------------

//0x70: Touch Panel Control Register 0 (TPCR0)
void TouchPanel_BasicSet(bool en, uint8_t sampTime, bool wakeupEn, bool ADCClock)
{
		uint8_t temp = 0x32;
		if(en)
		{
				temp |= cSetD7;
		}
		if(wakeupEn)
		{
				temp |= cSetD3;
		}
		temp |= (sampTime & 0x07) << 4;
		temp |= ADCClock & 0x07;
		LCD_CmdWrite(0x70);
		LCD_DataWrite(temp);
}
//0x71: Touch Panel Control Register 1 (TPCR1)
void TouchPanel_CtlSet(bool manualMode, bool deBounceEn, uint8_t TPMode)
{
		uint8_t temp = 0x00;
		if(manualMode)
		{
				temp |= cSetD6;
		}
		if(deBounceEn)
		{
				temp |= cSetD2;
		}
		temp |= (TPMode & 0x3);
		LCD_CmdWrite(0x71);
		LCD_DataWrite(temp);
}
//0x72: Touch Panel X High Byte Data Register (TPXH)
//0x73: Touch Panel Y High Byte Data Register (TPYH)
//0x74: Touch Panel X/Y Low Byte Data Register (TPXYL)
uint16_t TouchPanel_GetX(void)
{
		uint16_t temp = 0x00;
		LCD_CmdWrite(0x72);
		temp = LCD_DataRead();
		temp <<= 2;
		LCD_CmdWrite(0x74);
		temp |= (LCD_DataRead() & 0x03);		
		return temp;
}

uint16_t TouchPanel_GetY(void)
{
		uint16_t temp = 0x00;
		LCD_CmdWrite(0x73);
		temp = LCD_DataRead();
		temp <<= 2;
		LCD_CmdWrite(0x74);
		temp |= (LCD_DataRead() & 0x0c) >> 2;		
		return temp;
}

bool TouchPanel_CheckTouch(void)
{
		LCD_CmdWrite(0xf1);
		return ((LCD_DataRead() & cSetD2)==cSetD2);
}

void TouchPanel_Enable(bool en)
{
		TouchPanel_BasicSet(en, 0x00, false, 0x20);
		LCD_CmdWrite(0xf0);
		LCD_DataWrite(cSetD2);
}

void TouchPanel_AutoModeSet(void)
{
		TouchPanel_Enable(true);
		TouchPanel_CtlSet(false,true,0x01);
}
void TouchPanel_MannualModeSet(void)
{
		TouchPanel_Enable(true);
		TouchPanel_CtlSet(true,false,0x01);
}

void TouchPanel_MunualLatchX(void)
{
		TouchPanel_CtlSet(true,false,0x02);
}

void TouchPanel_MunualLatchY(void)
{
		TouchPanel_CtlSet(true,false,0x03);
}
void TouchPanel_MunualIdle(void)
{
		TouchPanel_CtlSet(true,false,0x00);
}

void TouchPanel_MunualWaitTP(void)
{
		TouchPanel_CtlSet(true,false,0x01);
}

void TouchPanel_IntClean(void)
{
		LCD_CmdWrite(0xf1);
		LCD_DataWrite(cSetD2);
}	

//--------------------------------------Graphic Cursor Setting Registers------------------------------------

//0x80: Graphic Cursor Horizontal Position Register 0 (GCHP0)
//0x81: Graphic Cursor Horizontal Position Register 1 (GCHP1)
void Graphic_CursorXPostionSet(uint16_t XPosition)
{
		LCD_CmdWrite(0x80);
		LCD_DataWrite((uint8_t)XPosition);
		LCD_CmdWrite(0x81);
		LCD_DataWrite((uint8_t)(XPosition>>8));
}
//0x82: Graphic Cursor Vertical Position Register 0 (GCVP0)
//0x83: Graphic Cursor Vertical Position Register 1 (GCVP1)
void Graphic_CursorYPositionSet(uint16_t YPosition)
{
		LCD_CmdWrite(0x82);
		LCD_DataWrite((uint8_t)YPosition);
		LCD_CmdWrite(0x83);
		LCD_DataWrite((uint8_t)(YPosition>>8));
}

void Graphic_CursorPositionSet(uint16_t XPosition, uint16_t YPosition)
{
		Graphic_CursorXPostionSet(XPosition);
		Graphic_CursorXPostionSet(YPosition);
}	
//0x84: Graphic Cursor Color 0 (GCC0)
//0x85: Graphic Cursor Color 1 (GCC1)
void Graphic_CursorColorSetMix(uint16_t color)
{
		
}
//--------------------------------------PLL Setting Registers-------------------------------------------

//0x88: PLL Control Register 1 (PLLC1)

//0x89: PLL Control Register 2 (PLLC2)

void PLL_Initial(void)
{
		LCD_CmdWrite(0x88);
		LCD_DataWrite(0x0a);
		__Delay1ms();
		LCD_CmdWrite(0x89);
		LCD_DataWrite(0x02);
		__Delay1ms();
}

//--------------------------------------PWM Control Register--------------------------------------------

//0x8a: PWM1 Control Regiister (P1CR)
//0x8b: PWM1 Duty Cycle Register (P1DCR)
void Backlight_PWM1Enable(bool en)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8a);
		temp = LCD_DataRead();
		if(en)
		{
				LCD_DataWrite(cSetD7|temp);
		}
		else
		{
				LCD_DataWrite(cClrD7&temp);
		}
}
void Backlight_PWM1DasableLevelSelect(bool low)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8a);
		temp = LCD_DataRead();
		if(low)
		{
				LCD_DataWrite(cSetD6&temp);
		}
		else
		{
				LCD_DataWrite(cClrD6|temp);
		}
}
void Backlight_PWM1FunctionSelect(bool backlight)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8a);
		temp = LCD_DataRead();
		if(backlight)
		{
				LCD_DataWrite(cSetD6&temp);
		}
		else
		{
				LCD_DataWrite(cClrD6|temp);
		}
}

void Backlight_PWM1Initial(void)
{
		LCD_CmdWrite(0x8a);//PWM setting
		LCD_DataWrite(0x80);
		LCD_CmdWrite(0x8a);//PWM setting
		LCD_DataWrite(0x81);//open PWM
}

void Backlight_PWM1DutySet(uint8_t duty)
{
		LCD_CmdWrite(0x8b);//Backlight brightness setting
		LCD_DataWrite(duty);//Brightness parameter 0xff-0x00
}
		

//0x8c: PWM2 Control Regiister (P2CR)
//0x8d: PWM2 Duty Cycle Register (P2DCR)
void Backlight_PWM2Enable(bool en)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8c);
		temp = LCD_DataRead();
		if(en)
		{
				LCD_DataWrite(cSetD7|temp);
		}
		else
		{
				LCD_DataWrite(cClrD7&temp);
		}
}
void Backlight_PWM2DasableLevelSelect(bool low)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8c);
		temp = LCD_DataRead();
		if(low)
		{
				LCD_DataWrite(cSetD6&temp);
		}
		else
		{
				LCD_DataWrite(cClrD6|temp);
		}
}
void Backlight_PWM2FunctionSelect(bool backlight)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x8c);
		temp = LCD_DataRead();
		if(backlight)
		{
				LCD_DataWrite(cSetD6&temp);
		}
		else
		{
				LCD_DataWrite(cClrD6|temp);
		}
}

void Backlight_PWM2Initial(void)
{
		LCD_CmdWrite(0x8c);//PWM setting
		LCD_DataWrite(0x80);
		LCD_CmdWrite(0x8c);//PWM setting
		LCD_DataWrite(0x81);//open PWM
}

void Backlight_PWM2DutySet(uint8_t duty)
{
		LCD_CmdWrite(0x8d);//Backlight brightness setting
		LCD_DataWrite(duty);//Brightness parameter 0xff-0x00
}
//0x8e: Memory Clear Control Register (MCLR)
void LCD_ClearWindow(void)
{
		LCD_CmdWrite(0x8e);
		LCD_DataWrite(0x80);
}

//--------------------------------------Drawing Control Registers--------------------------------------

//0x90: Draw Line/Circle/Square Control Register (DCR)
void Draw_LineSet(void)
{
		LCD_CmdWrite(0x90);
		LCD_DataWrite(0x00);
}
void Draw_SquareSet(void)
{
		LCD_CmdWrite(0x90);
		LCD_DataWrite(cSetD4);
}

void Draw_TriangleSet(void)
{
		LCD_CmdWrite(0x90);
		LCD_DataWrite(cSetD0);
}
void Draw_CircleSet(void)
{
		LCD_CmdWrite(0x90);
		LCD_DataWrite(0x00);
}

void Draw_LSTStart(bool start)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x90);
		temp = LCD_DataRead();
		LCD_DataWrite(cSetD7 | (temp & cClrD6));
		while(!Draw_LSTComplete());
}

void Draw_CircleStart(bool start)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x90);
		temp = LCD_DataRead();
		LCD_DataWrite(cSetD6 | (temp & cClrD7));
}

void Draw_CSTFill(bool fill)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0x90);
		temp = LCD_DataRead();
		if(fill)
		{
				LCD_DataWrite(cSetD5 | temp);
		}
		else
		{
			LCD_DataWrite(cClrD5 & temp);
		}
}


bool Draw_LSTComplete(void)
{
		LCD_CmdWrite(0x90);
		return ((LCD_DataRead() & cSetD7) == 0x00);
}

bool Draw_CircleComplete(void)
{
		LCD_CmdWrite(0x90);
		return ((LCD_DataRead() & cSetD6) == 0x00);
}	

//0x91: Draw Line/Square Horizontal Start Address Register0 (DLHSR0)
//0x92: Draw Line/Square Horizontal Start Address Register1 (DLHSR1)
void Draw_LSXStartSet(uint16_t XStart)
{
		uint8_t temp = (uint8_t) XStart;
		LCD_CmdWrite(0x91);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XStart >> 8);
		LCD_CmdWrite(0x92);
		LCD_DataWrite(temp);
}
//0x93: Draw Line/Square Vertical Start Address Register0 (DLVSR0)
//0x94: Draw Line/Square Vertical Start Address Register1 (DLVSR1)
void Draw_LSYStartSet(uint16_t YStart)
{
		uint8_t temp = (uint8_t) YStart;
		LCD_CmdWrite(0x93);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YStart >> 8);
		LCD_CmdWrite(0x94);
		LCD_DataWrite(temp);
}
//0x95: Draw Line/Square Horizontal End Address Register0 (DLHER0)
//0x96: Draw Line/Square Horizontal End Address Register1 (DLHER1)
void Draw_LSXEndSet(uint16_t XEnd)
{
		uint8_t temp = (uint8_t) XEnd;
		LCD_CmdWrite(0x95);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XEnd >> 8);
		LCD_CmdWrite(0x96);
		LCD_DataWrite(temp);
}
//0x97: Draw Line/Square Vertical End Address Register0 (DLVER0)
//0x98: Draw Line/Square Vertical End Address Register1 (DLVER1)
void Draw_LSYEndSet(uint16_t YEnd)
{
		uint8_t temp = (uint8_t) YEnd;
		LCD_CmdWrite(0x97);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YEnd >> 8);
		LCD_CmdWrite(0x98);
		LCD_DataWrite(temp);
}

void Draw_LSPositionSet(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd)
{
		Draw_LSXStartSet(XStart);
		__Delay1ms();
		Draw_LSYStartSet(YStart);
		__Delay1ms();
		Draw_LSXEndSet(XEnd);
		__Delay1ms();
		Draw_LSYEndSet(YEnd);
		__Delay1ms();
}
//0x99: Draw Circle Center Horizontal Address Register0 (DCHR0)
//0x9a: Draw Circle Center Horizontal Address Register1 (DCHR1)
void Draw_CircleXCenterSet(uint16_t XCenter)
{
		uint8_t temp = (uint8_t) XCenter;
		LCD_CmdWrite(0x99);
		LCD_DataWrite(temp);
		temp = (uint8_t)(XCenter >> 8);
		LCD_CmdWrite(0x9a);
		LCD_DataWrite(temp);
}
//0x9b: Draw Circle Center Vertical Address Register0 (DCVR0)
//0x9c: Draw Circle Center Vertical Address Register1 (DCVR1)
void Draw_CircleYCenterSet(uint16_t YCenter)
{
		uint8_t temp = (uint8_t) YCenter;
		LCD_CmdWrite(0x9b);
		LCD_DataWrite(temp);
		temp = (uint8_t)(YCenter >> 8);
		LCD_CmdWrite(0x9c);
		LCD_DataWrite(temp);
}
//0x9d: Draw Circle Radius Register (DCRR)
void Draw_CircleRadiusSet(uint8_t radius)
{
		LCD_CmdWrite(0x9d);
		LCD_DataWrite(radius);
}
//0xa0: Draw Ellipse/Ellipse Curve/ Circle Squrare Contral Register
void Draw_ECSStart(bool start)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0xa0);
		temp = LCD_DataRead();
		if(start)
		{
				LCD_DataWrite(temp | cSetD7);
		}
		else
		{
				LCD_DataWrite(temp & cClrD7);
		}
}

void Draw_ECSFill(bool fill)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0xa0);
		temp = LCD_DataRead();
		if(fill)
		{
				LCD_DataWrite(temp | cSetD6);
		}
		else
		{
				LCD_DataWrite(temp & cClrD6);
		}
}

void Draw_ECurveSet(void)
{
		LCD_CmdWrite(0xa0);
		LCD_DataWrite(cSetD4);
}

void Draw_CSquareSet(void)
{
		LCD_CmdWrite(0xa0);
		LCD_DataWrite(cSetD5);
}

void Draw_EllipseSet(void)
{
		LCD_CmdWrite(0xa0);
		LCD_DataWrite(0x00);
}

void Draw_ECurvePartSelect(uint8_t part)
{
		LCD_CmdWrite(0xa0);
		LCD_DataWrite((part & 0x03)|LCD_DataRead());
}

bool Draw_ECSComplete(void)
{
		LCD_CmdWrite(0xa0);
		return ((LCD_DataRead() & cSetD7)==0x00);
}

//0xa1: Draw Ellipse/Ellipse Square Long Axis Setting Register (ELL_A0)
//0xa2: Draw Ellipse/Ellipse Square Long Axis Setting Register (ELL_A1)
void Draw_EESLongAxisSet(uint16_t longAxis)
{
		LCD_CmdWrite(0xa1);
		LCD_DataWrite((uint8_t)longAxis);
		LCD_CmdWrite(0xa2);
		LCD_DataWrite((uint8_t)(longAxis>>8));
}
	
//0xa3: Draw Ellipse/Ellipse Square Short Axis Setting Register (ELL_B0)
//0xa4: Draw Ellipse/Ellipse Square Short Axis Setting Register (ELL_B1)
void Draw_EESShortAxisSet(uint16_t shortAxis)
{
		LCD_CmdWrite(0xa3);
		LCD_DataWrite((uint8_t)shortAxis);
		LCD_CmdWrite(0xa4);
		LCD_DataWrite((uint8_t)(shortAxis>>8));
}
//0xa5: Draw Ellipse/Ellipse Square Center Horizontal Address Register 0 (DEHR0)
//0xa6: Draw Ellipse/Ellipse Square Center Horizontal Address Register 1 (DEHR1)
void Draw_EESXCenterSet(uint16_t XCenter)
{
		LCD_CmdWrite(0xa5);
		LCD_DataWrite((uint8_t)XCenter);
		LCD_CmdWrite(0xa6);
		LCD_DataWrite((uint8_t)(XCenter>>8));
}
//0xa7: Draw Ellipse/Ellipse Square Center Vertical Address Register 0 (DEVR0)
//0xa8: Draw Ellipse/Ellipse Square Center Vertical Address Register 1 (DEHR1)
void Draw_EESYCenterSet(uint16_t YCenter)
{
		LCD_CmdWrite(0xa7);
		LCD_DataWrite((uint8_t)YCenter);
		LCD_CmdWrite(0xa8);
		LCD_DataWrite((uint8_t)(YCenter>>8));
}
//0xa9: Draw Triangle Point 2 Horizontal Address Register 0 (DTPH0)
//0xaa: Draw Triangle Point 2 Horizontal Address Register 1 (DTPH1)
void Draw_TriangleXPoint2Set(uint16_t XPoint2)
{
		LCD_CmdWrite(0xa9);
		LCD_DataWrite((uint8_t)XPoint2);
		LCD_CmdWrite(0xaa);
		LCD_DataWrite((uint8_t)(XPoint2>>8));
}
//0xab: Draw Triangle Point 2 Vertical Address Register 0 (DTPV0)
//0xac: Draw Triangle Point 2 Vertical Address Register 1 (DTPV1)
void Draw_TriangleYPoint2Set(uint16_t YPoint2)
{
		LCD_CmdWrite(0xab);
		LCD_DataWrite((uint8_t)YPoint2);
		LCD_CmdWrite(0xac);
		LCD_DataWrite((uint8_t)(YPoint2>>8));
}

//------------------------------------------------DMA Registers---------------------------------------
//0xb0: Source Starting Address REG0 (SSAR0)
//0xb1: Source Starting Address REG1 (SSAR1)
//0xb2: Source Starting Address REG2 (SSAR2)
void DMA_StartAddress(uint32_t addr)
{
		LCD_CmdWrite(0xb0);
		LCD_DataWrite(addr);
		LCD_CmdWrite(0xb1);
		LCD_DataWrite(addr >> 8);
		LCD_CmdWrite(0xb2);
		LCD_DataWrite(addr >> 16);
		LCD_CmdWrite(0xb3);
		LCD_DataWrite(addr >> 24);
}
//0xb4: Block Width REG 0(BWR0) / DMA Transfer Number REG 0 (DTNR0)
//0xb5: Block Width REG 1 (BWR1)
void DMA_BlockWidthSet(uint16_t width)
{
		LCD_CmdWrite(0xb4);
		LCD_DataWrite(width);
		LCD_CmdWrite(0xb5);
		LCD_DataWrite(width >> 8);
}
//0xb6: Block Height REG 0(BHR0) / DMA Transfer Number REG 1 (DTNR1)
//0xb7: Block Height REG 1(BHR1)
void DMA_BlockHeightSet(uint16_t height)
{
		LCD_CmdWrite(0xb6);
		LCD_DataWrite(height);
		LCD_CmdWrite(0xb7);
		LCD_DataWrite(height >> 8);
}
//0xb8: Source Picture Width REG0(SPWR0) /DMA Transfer Number REG 2 (DTNR2)
void DMA_TransferNumberSet(uint32_t num)
{
		LCD_CmdWrite(0xb4);
		LCD_DataWrite(num);
		LCD_CmdWrite(0xb6);
		LCD_DataWrite(num >> 8);
		LCD_CmdWrite(0xb8);
		LCD_DataWrite(num >> 16);
}
//0xb9: Source Picture Width REG1(SPWR1)
void DMA_SourcePicWidthSet(uint16_t width)
{
		LCD_CmdWrite(0xb8);
		LCD_DataWrite(width);
		LCD_CmdWrite(0xb9);
		LCD_DataWrite(width >> 8);
}
//0xbf: DMA Configuration REG (DMACR)

void DMA_BlockModeSelect(bool block)
{
		LCD_CmdWrite(0xbf);
		if(block)
		{
				LCD_DataWrite(0x02);
		}
		else
		{
				LCD_DataWrite(0x00);
		}
}

void DMA_Start(bool start)
{
		uint8_t temp = 0x00;
		LCD_CmdWrite(0xbf);
		if(start)
			temp = LCD_DataRead() | cSetD0;
		else
			temp = LCD_DataRead();
		LCD_DataWrite(temp);
}

//----------------------------------------------Key & IO Control Register---------------------------------------

//0xc0: Key Scan Control Register 1 (KSCR1)

//0xc1: Key Scan Control Register 2 (KSCR2)

//0xc2: Key-Scan Data Register (KSDR0)

//0xc3: Key-Scan Data Register (KSDR1)

//0xc4: Key-Scan Data Register (KSDR2)

//0xc7: Extra General Purpose IO Register (GPIOX)

//--------------------------------------------Floating Window Control Register----------------------------------
//0xd0: Floating Windows Star Address XA 0 (FWSAXA0)

//0xd1: Floating Windows Star Address XA 1 (FWSAXA1)

//0xd2: Floating Windows Star Address YA 0 (FWSAYA0)

//0xd3: Floating Windows Star Address YA 1 (FWSAYA1)

//0xd4: Floating Windows Width 0 (FWW0)

//0xd5: Floating Windows Width 1 (FWW1)

//0xd6: Floating Windows Height 0 (FWH0)

//0xd7: Floating Windows Height 1 (FWW1)

//0xd8: Floating Windows Display X Address 0 (FWDXA0)

//0xd9: Floating Windows Display X Address 1 (FWDXA1)

//0xda: Floating Windows Display Y Address 0 (FWDYA0)

//0xdb: Floating Windows Display Y Address 1 (FWDYA1)

//--------------------------------------Serial Flash Control Register---------------------------

//0xe0: Serial Flash/ROM Direct Access Mode 

//0xe1: Serial Flash/ROM Direct Access Mode Address

//0xe2: Serial Flash/ROM Direct Access Data Read

//--------------------------------------Interrupt Control Registers--------------------------------

//0xf0: Interrupt Control Register 1 (INTC1)

//0xf1: Interrupt Control Register 2 (INTC2)

/////////////////Set the working window area
void Active_Window(uint32_t XL,uint32_t XR ,uint32_t YT ,uint32_t YB)
{
	uint8_t temp;
    //setting active window X
	temp=XL;   
    LCD_CmdWrite(0x30);//HSAW0
	LCD_DataWrite(temp);
	temp=XL>>8;   
    LCD_CmdWrite(0x31);//HSAW1	   
	LCD_DataWrite(temp);

	temp=XR;   
    LCD_CmdWrite(0x34);//HEAW0
	LCD_DataWrite(temp);
	temp=XR>>8;   
    LCD_CmdWrite(0x35);//HEAW1	   
	LCD_DataWrite(temp);

    //setting active window Y
	temp=YT;   
    LCD_CmdWrite(0x32);//VSAW0
	LCD_DataWrite(temp);
	temp=YT>>8;   
    LCD_CmdWrite(0x33);//VSAW1	   
	LCD_DataWrite(temp);

	temp=YB;   
    LCD_CmdWrite(0x36);//VEAW0
	LCD_DataWrite(temp);
	temp=YB>>8;   
    LCD_CmdWrite(0x37);//VEAW1	   
	LCD_DataWrite(temp);
}

void LCD_Initial(void)
{
		PLL_Initial();
		LCD_CmdWrite(0x10);	 //SYSR   bit[4:3] color  bit[2:1]=  MPU interface
		LCD_DataWrite(0x0c);   //          65K						 
	
		LCD_CmdWrite(0x04);    //PCLK
		LCD_DataWrite(0x81);   //
		SysCtlDelay(SysCtlClockGet()/1000);

		 //Horizontal set
		 LCD_CmdWrite(0x14); //HDWR//Horizontal Display Width Setting Bit[6:0]                   
		 LCD_DataWrite(0x63);//Horizontal display width(pixels) = (HDWR + 1)*8                   
		 LCD_CmdWrite(0x15); //Horizontal Non-Display Period Fine Tuning Option Register (HNDFTR)
		 LCD_DataWrite(0x03);//Horizontal Non-Display Period Fine Tuning(HNDFT) [3:0]            
		 LCD_CmdWrite(0x16); //HNDR//Horizontal Non-Display Period Bit[4:0]                      
		 LCD_DataWrite(0x03);//Horizontal Non-Display Period (pixels) = (HNDR + 1)*8             
		 LCD_CmdWrite(0x17); //HSTR//HSYNC Start Position[4:0]                                   
		 LCD_DataWrite(0x02);//HSYNC Start Position(PCLK) = (HSTR + 1)*8                         
		 LCD_CmdWrite(0x18); //HPWR//HSYNC Polarity ,The period width of HSYNC.                  
		 LCD_DataWrite(0x00);//HSYNC Width [4:0]   HSYNC Pulse width(PCLK) = (HPWR + 1)*8     
		 //Vertical set    
		 LCD_CmdWrite(0x19); //VDHR0 //Vertical Display Height Bit [7:0]                         
		 LCD_DataWrite(0xdf);//Vertical pixels = VDHR + 1                                        
		 LCD_CmdWrite(0x1a); //VDHR1 //Vertical Display Height Bit [8]                           
		 LCD_DataWrite(0x01);//Vertical pixels = VDHR + 1                                        
		 LCD_CmdWrite(0x1b); //VNDR0 //Vertical Non-Display Period Bit [7:0]                     
		 LCD_DataWrite(0x14);//Vertical Non-Display area = (VNDR + 1)                            
		 LCD_CmdWrite(0x1c); //VNDR1 //Vertical Non-Display Period Bit [8]                       
		 LCD_DataWrite(0x00);//Vertical Non-Display area = (VNDR + 1)                            
		 LCD_CmdWrite(0x1d); //VSTR0 //VSYNC Start Position[7:0]                                 
		 LCD_DataWrite(0x06);//VSYNC Start Position(PCLK) = (VSTR + 1)                           
		 LCD_CmdWrite(0x1e); //VSTR1 //VSYNC Start Position[8]                                   
		 LCD_DataWrite(0x00);//VSYNC Start Position(PCLK) = (VSTR + 1)                           
		 LCD_CmdWrite(0x1f); //VPWR //VSYNC Polarity ,VSYNC Pulse Width[6:0]                     
		 LCD_DataWrite(0x01);//VSYNC Pulse Width(PCLK) = (VPWR + 1)  
	 
		//Active_Window(0,799,0,479);

		Window_ActivePositionSet(0, 799, 0, 479);
		Backlight_PWM1Initial();
		Backlight_PWM1DutySet(0xff);
}

