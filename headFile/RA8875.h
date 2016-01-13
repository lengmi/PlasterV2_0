#ifndef _RA8875_H_
#define _RA8875_H_

#include <stdint.h>
#include <stdbool.h>
	
	
extern void LCD_CmdWrite(uint8_t addr);
extern void LCD_DataWrite(uint8_t data);
extern uint8_t LCD_DataRead(void);
extern uint8_t LCD_StatusRead(void);
	
extern void LCD_Initial(void);
extern void LCD_TextModeSet(void);
extern void LCD_GraphicModeSet(void);
extern void LCD_DisplayOn(bool on);
extern void LCD_SoftReset(void);
extern void LCD_HardReset(void);
extern void LCD_ClearWindow(void);
extern void LCD_StringDisplay(char *str);
extern void LCD_ChkBusy(void);

extern void Graphic_CursorPositionSet(uint16_t XPosition, uint16_t YPosition);
extern void Memory_CursorPositionSet(uint16_t XPosition, uint16_t YPosition);
extern void FontCtl_Enlarge(uint8_t xdir, uint8_t ydir);
extern void FontCtl_LineDistanceSet(uint8_t lineDistance);
extern void FontCtl_Rotate(bool rotate);
extern void FontCtl_CursorPositionSet(uint16_t XPosition, uint16_t YPosition);
extern void FontCtl_FontSizeWidthSet(uint8_t size, uint8_t width);
extern void FontCtl_ExternelCGROMEnable(bool en);
extern void FontCtl_ExternelROMFontSet(uint8_t fontRom, uint8_t fontCoding, uint8_t font);
extern void FontCtl_ExternelROMFontSelect(void);
extern void FontCtl_CNZHDisplaySet(void);

	
extern void Window_ActivePositionSet(uint16_t XStart, uint16_t XEnd, uint16_t YStart, uint16_t YEnd);
extern void Window_ScollPositionSet(uint16_t XStart, uint16_t XEnd, uint16_t YStart, uint16_t YEnd);

extern void BTE_FunctionCtlSet(bool en, bool sourceDataMode, bool destDataMode);
extern void BTE_CodeSet(uint8_t ROPCode, uint8_t operationCode);
extern void BTE_LayerCtlSet(uint8_t scrollMode, bool FLWTranEn, uint8_t layerMode);
extern void BTE_LayerCtlTansSet(uint8_t layer1, uint8_t layer2);
extern void BTE_XSourcePointSet(uint16_t XSource);
extern void BTE_YSourcePointSet(uint16_t YSource);
extern void BTE_XDstPointSet(uint16_t XDst);
extern void BTE_YDstPointSet(uint16_t YDst);
extern void BTE_WidthSet(uint16_t width);
extern void BTE_HeightSet(uint16_t height);
extern void BTE_ForegroundColorSet(uint8_t setR, uint8_t setG, uint8_t setB);
extern void BTE_ForegroundColorSetMix(uint32_t foreColor);
extern void BTE_BackgroundColorSet(uint8_t setR, uint8_t setG, uint8_t setB);
extern void BTE_BackgroundColorSetMix(uint32_t backColor);
extern void BTE_BackgroundTranColorSet(uint8_t setR, uint8_t setG, uint8_t setB);
extern void BTE_BackgroundTranColorSetMix(uint32_t backColor);
extern void BTE_PatternNOSet(uint8_t pattern);

extern uint16_t TouchPanel_GetX(void);
extern uint16_t TouchPanel_GetY(void);
extern bool TouchPanel_CheckTouch(void);
extern void TouchPanel_Enable(bool en);
extern void TouchPanel_AutoModeSet(void);
extern void TouchPanel_MannualModeSet(void);
extern void TouchPanel_MunualLatchX(void);
extern void TouchPanel_MunualLatchY(void);
extern void TouchPanel_MunualWaitTP(void);
extern void TouchPanel_MunualIdle(void);
extern void TouchPanel_IntClean(void);


extern void Backlight_PWM1Initial(void);
extern void Backlight_PWM1DutySet(uint8_t duty);

//画圆线矩形
extern void Draw_LineSet(void);
extern void Draw_SquareSet(void);
extern void Draw_TriangleSet(void);
extern void Draw_CircleSet(void);
extern void Draw_LSTStart(bool start);
extern void Draw_CircleStart(bool start);
extern void Draw_CSTFill(bool fill);
extern bool Draw_LSTComplete(void);
extern bool Draw_CircleComplete(void);
extern void Draw_LSXStartSet(uint16_t XStart);
extern void Draw_LSYStartSet(uint16_t YStart);
extern void Draw_LSXEndSet(uint16_t XEnd);
extern void Draw_LSYEndSet(uint16_t YEnd);
extern void Draw_LSPositionSet(uint16_t XStart, uint16_t YStart, uint16_t XEnd, uint16_t YEnd);
extern void Draw_CircleXCenterSet(uint16_t XCenter);
extern void Draw_CircleYCenterSet(uint16_t YCenter);
extern void Draw_CircleRadiusSet(uint8_t radius);
//画弧线、椭圆、带倒角的方形
extern void Draw_ECSStart(bool start);
extern void Draw_ECSFill(bool fill);
extern void Draw_ECurveSet(void);
extern void Draw_CSquareSet(void);
extern void Draw_EllipseSet(void);
extern void Draw_ECurvePartSelect(uint8_t part);
extern bool Draw_ECSComplete(void);
extern void Draw_EESLongAxisSet(uint16_t longAxis);
extern void Draw_EESShortAxisSet(uint16_t shortAxis);
extern void Draw_EESXCenterSet(uint16_t XCenter);
extern void Draw_EESYCenterSet(uint16_t YCenter);
extern void Draw_TriangleXPoint2Set(uint16_t XPoint2);
extern void Draw_TriangleYPoint2Set(uint16_t YPoint2);
extern bool Draw_LSTComplete(void);
	
//数据接口定义
#define PREFIXWRITEDATA 0x0000
#define PREFIXWRITECMD  0x8000
#define PREFIXREADSTATUS 0xc000
#define PREFIXREADDATA	0x4000

//颜色定义
#define	RA8875_BLACK            0x0000
#define	RA8875_BLUE             0x001F
#define	RA8875_RED              0xF800
#define	RA8875_GREEN            0x07E0
#define RA8875_CYAN             0x07FF
#define RA8875_MAGENTA          0xF81F
#define RA8875_YELLOW           0xFFE0  
#define RA8875_WHITE            0xFFFF

#define cClrD0	0xfe
#define cClrD1  0xfd 
#define cClrD2  0xfb 
#define cClrD3  0xf7
#define cClrD4  0xef 
#define cClrD5  0xdf
#define cClrD6  0xbf
#define cClrD7	0x7f

#define cSetD0  0x01
#define cSetD1  0x02
#define cSetD2  0x04
#define cSetD3  0x08
#define cSetD4  0x10
#define cSetD5  0x20
#define cSetD6  0x40
#define cSetD7  0x80
#endif
