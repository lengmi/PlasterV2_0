#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "driverlib/sysctl.h"
#include "RA8875.h"
#include "IODevice.h"
#include "Display.h"
#include "touch.h"
#include "ui.h"
#include "assistant.h"
#include "DataManage.h"

uint32_t OptionPara = 0;
textWidget *CurrentUI;

extern bool OpticGate;
extern bool Laser;
extern bool Fan;
extern bool Blow;
extern bool Scan;

extern double Diameter;
extern double Distance;
extern uint32_t PointAmount;
extern uint32_t HoleAmount;


extern textWidget MainSet,MainRun,MainTest,MainTitle,MainSubline;

__Canvas(MainSet,NULL,&MainRun,250,80,300,80,RA8875_GREEN,RA8875_RED,"设    置",8,0);
__Canvas(MainRun,&MainSet,&MainTest,250,200,300,80,RA8875_GREEN,RA8875_RED,"运    行",8,0);
__Canvas(MainTest,&MainRun,&MainTitle,250,320,300,80,RA8875_GREEN,RA8875_RED,"测    试",8,0);
__Canvas(MainTitle,&MainTest,&MainSubline,0,0,799,39,RA8875_WHITE,RA8875_RED,"欢迎使用武汉市凯瑞迪激光产品",28,0);
__Canvas(MainSubline,&MainTitle,NULL,0,440,799,39,RA8875_WHITE,RA8875_RED,"速度(m/s)：",17,0);

extern textWidget RunStop,RunState,RunTime,RunTitle,RunSubline;
__Canvas(RunStop,NULL,&RunState,250,80,300,80,RA8875_GREEN,RA8875_RED,"停    止",8,2);
__Canvas(RunState,&RunStop,&RunTime,150,200,500,60,RA8875_WHITE,RA8875_RED,"系统状态：",18,2);
__Canvas(RunTime,&RunState,&RunTitle,150,300,500,60,RA8875_WHITE,RA8875_RED,"运行时间",8,2);
__Canvas(RunTitle,&RunTime,&RunSubline,0,0,799,39,RA8875_WHITE,RA8875_RED,"运            行",16,2);
__Canvas(RunSubline,&RunTitle,NULL,0,440,799,39,RA8875_WHITE,RA8875_RED,"速度(m/s)：",17,2);


extern textWidget TestOptic,TestFan,TestLaser,TestBlow,TestReturn,TestScan,TestTitle,TestSubline;
__Canvas(TestOptic,NULL,&TestFan,100,80,200,80,RA8875_GREEN,RA8875_RED,"光闸",4,3);
__Canvas(TestFan,&TestOptic,&TestLaser,500,80,200,80,RA8875_GREEN,RA8875_RED,"抽风",4,3);
__Canvas(TestLaser,&TestFan,&TestBlow,100,200,200,80,RA8875_GREEN,RA8875_RED,"激光",4,3);
__Canvas(TestBlow,&TestLaser,&TestReturn,500,200,200,80,RA8875_GREEN,RA8875_RED,"吹风",4,3);
__Canvas(TestReturn,&TestBlow,&TestScan,100,320,200,80,RA8875_GREEN,RA8875_RED,"返回",4,3);
__Canvas(TestScan,&TestReturn,&TestTitle,500,320,200,80,RA8875_GREEN,RA8875_RED,"振镜",4,3);
__Canvas(TestTitle,&TestScan,&TestSubline,0,0,799,39,RA8875_WHITE,RA8875_RED,"测            试",16,3);
__Canvas(TestSubline,&TestTitle,NULL,0,440,799,39,RA8875_WHITE,RA8875_RED,"速度(m/s)：",17,3);

extern textWidget SetOption,SetSub,SetAdd,SetDiscard,SetSave,SetDiameter,SetPointAmount,\
									SetDistance,SetHoleAmount,SetTitle,SetSubline;
__Canvas(SetOption,NULL,&SetSub,500,70,200,60,RA8875_GREEN,RA8875_RED,"选  项",6,1);
__Canvas(SetSub,&SetOption,&SetAdd,460,190,80,80,RA8875_GREEN,RA8875_RED,"减",2,1);
__Canvas(SetAdd,&SetSub,&SetDiscard,660,190,80,80,RA8875_GREEN,RA8875_RED,"加",2,1);
__Canvas(SetDiscard,&SetAdd,&SetSave,420,320,160,70,RA8875_GREEN,RA8875_RED,"放弃",4,1);
__Canvas(SetSave,&SetDiscard,&SetDiameter,620,320,160,70,RA8875_GREEN,RA8875_RED,"保存",4,1);
__Canvas(SetDiameter,&SetSave,&SetPointAmount,100,80,250,60,RA8875_BLUE,RA8875_RED,"孔径(mm):",14,1);
__Canvas(SetPointAmount,&SetDiameter,&SetDistance,100,165,250,60,RA8875_WHITE,RA8875_RED,"点数:",6,1);
__Canvas(SetDistance,&SetPointAmount,&SetHoleAmount,100,255,250,60,RA8875_WHITE,RA8875_RED,"间距(mm):",14,1);
__Canvas(SetHoleAmount,&TestReturn,&SetTitle,100,340,250,60,RA8875_WHITE,RA8875_RED,"孔数:",6,1);
__Canvas(SetTitle,&SetHoleAmount,&SetSubline,0,0,799,39,RA8875_WHITE,RA8875_RED,"设            置",16,1);
__Canvas(SetSubline,&TestTitle,NULL,0,440,799,39,RA8875_WHITE,RA8875_RED,"速度(m/s)：",17,1);

__Canvas(ErrorWarning,NULL,NULL,0,200,799,80,RA8875_GREEN,RA8875_RED,"冷水机异常，请检查。",20,4);

extern void MainSetSet(void);
extern void MainRunSet(void);
extern void MainTestSet(void);
extern void SetOptionSet(void);
extern void SetSubSet(void);
extern void SetAddSet(void);
extern void SetDiscardSet(void);
extern void SetSaveSet(void);
extern void RunStopSet(void);
extern void TestOpticSet(void);
extern void TestFanSet(void);
extern void TestLaserSet(void);
extern void TestBlowSet(void);
extern void TestReturnSet(void);
extern void TestScanSet(void);

void UI_Reset(void)
{
		TestOptic.foreColor = RA8875_RED;
		TestFan.foreColor = RA8875_RED;
		TestLaser.foreColor = RA8875_RED;
		TestBlow.foreColor = RA8875_RED;
		TestScan.foreColor = RA8875_RED;
}

uint32_t UI_ButtonColor(bool option)
{
		if(!option)
		{
			return RA8875_RED;
		}
		else
		{
			return RA8875_BLUE;
		}
}

void UI_Desktop(void)
{
		uint16_t xPixel,yPixel;
		UI_Reset();
		IODevice_Initial();
		LCD_DisplayInterface(&MainSet);
		while(IODevice_GetHandModeOperation()&0x80)
		{
				TouchPanel_GetPixel(&xPixel,&yPixel);
				switch(TouchPanel_GetButtonIndex(CurrentUI,xPixel,yPixel))
				{
								//设置按钮
								case 0: LCD_DisplayInterface(&SetOption);
												UI_DisplayPara();
												break;
								//运行按钮
								case 1: IODevice_AutoRun();
												LCD_DisplayInterface(&RunStop);
												LCD_DisplayNumber("自动模式",RA8875_RED,400,214);
												break;
								//测试按钮
							  case 2: LCD_DisplayInterface(&TestOptic);
												break;
								//设置——>选项按钮
								case 100:	SetOptionSet();
													break;
								//设置——>减
								case 101:	SetSubSet();
													break;
								//设置——>加
								case 102: SetAddSet();
													break;
								//设置——>放弃
								case 103: SetDiscardSet();
													LCD_DisplayInterface(&MainSet);
													break;
								//设置——>保存
								case 104: SetSaveSet();
													LCD_DisplayInterface(&MainSet);
													break;
								//运行——>停止
								case 200: IODevice_AutoStop();
													LCD_DisplayInterface(&MainSet);
													break;
								//测试——>光闸
								case 300: TestOpticSet(); 
													break;
								//测试——>抽风
								case 301: TestFanSet();
													break;
								//测试——>激光
								case 302:	TestLaserSet();
													break;
								//测试——>吹风
								case 303: TestBlowSet();
													break;
								//测试——>返回
								case 304: TestReturnSet();
													break;
								//测试——>振镜
								case 305: TestScanSet();
													break;
				}
		}
		IODevice_AutoStop();
		IODevice_Initial();
		LCD_DisplayInterface(&MainSet);
}
//--------------------------------------------设置---------------------------------------
void SetOptionSet(void)
{
		OptionPara ++;
		if(4==OptionPara) OptionPara=0;
		switch(OptionPara)
		{
				case 0: SetHoleAmount.backColor = RA8875_WHITE;
								SetDiameter.backColor = RA8875_BLUE;
								LCD_DisplayWidget(SetHoleAmount);
								LCD_DisplayWidget(SetDiameter);
								break;
				case 1: SetDiameter.backColor = RA8875_WHITE;
								SetPointAmount.backColor = RA8875_BLUE;
								LCD_DisplayWidget(SetPointAmount);
								LCD_DisplayWidget(SetDiameter);
								break;
				case 2: SetPointAmount.backColor = RA8875_WHITE;
								SetDistance.backColor = RA8875_BLUE;
								LCD_DisplayWidget(SetPointAmount);
								LCD_DisplayWidget(SetDistance);
								break;
				case 3: SetHoleAmount.backColor = RA8875_BLUE;
								SetDistance.backColor = RA8875_WHITE;
								LCD_DisplayWidget(SetHoleAmount);
								LCD_DisplayWidget(SetDistance);
								break;
		}	
}
void SetAddSet(void)
{
		switch(OptionPara)
		{
				case 0: Diameter += 0.1;
								if(Diameter > 1.2) Diameter =0.2;
								break;
				case 1:	PointAmount += 1;
								if(PointAmount > 36) PointAmount = 1;
								break;
				case 2: Distance += 1;
								if(Distance > 20) Distance = 2;
								break;
				case 3:	HoleAmount += 1;
								if(HoleAmount > 50) HoleAmount=1; 
								break;
		}
		UI_DisplayPara();
}
void SetSubSet(void)
{
		switch(OptionPara)
		{
				case 0: if(Diameter < 0.3) Diameter = 1.2;
								else Diameter -= 0.1;
								break;
				case 1:	if(PointAmount < 2) PointAmount = 35;
								else PointAmount -= 1;
								break;
				case 2: if(Distance < 3) Distance = 19;
								else Distance -= 1;
								break;
				case 3:	if(HoleAmount < 2) HoleAmount=49; 
								else HoleAmount -= 1;
								break;
		}
		UI_DisplayPara();
}

void UI_SetReset(void)
{
		OptionPara = 0;
		SetDiameter.backColor = RA8875_BLUE;
		SetPointAmount.backColor = RA8875_WHITE;
		SetDistance.backColor = RA8875_WHITE;
		SetHoleAmount.backColor = RA8875_WHITE;
}

void SetDiscardSet(void)
{
		DataManage_ReadData(0);
		UI_SetReset();
}
void SetSaveSet(void)
{
		DataManage_StoreData(0);
		UI_SetReset();
}

void UI_DisplayPara(void)
{
		char strTemp[20];
		StrConv_Float2Str(strTemp,Diameter,2);
		LCD_DisplayNumber(strTemp,RA8875_RED,275,94);
		StrConv_Int2Str(strTemp,2,PointAmount);
		LCD_DisplayNumber(strTemp,RA8875_RED,275,179);
		StrConv_Float2Str(strTemp,Distance,2);
		LCD_DisplayNumber(strTemp,RA8875_RED,275,269);
		StrConv_Int2Str(strTemp,2,HoleAmount);
		LCD_DisplayNumber(strTemp,RA8875_RED,275,354);
}
//-----------------------------------------------运行------------------------------------------
void RunStopSet(void)
{
		
}

//-----------------------------------------------测试-----------------------------------------
void TestOpticSet(void)
{
		OpticGate = !OpticGate;
		TestOptic.foreColor = UI_ButtonColor(OpticGate);
		LCD_DisplayWidget(TestOptic);
		IODevice_OpticGateOn(OpticGate);
		__Delay100ms();
}

void TestFanSet(void)
{
		Fan=!Fan;
		TestFan.foreColor = UI_ButtonColor(Fan);
		LCD_DisplayWidget(TestFan);
		IODevice_FanOn(Fan);
		__Delay100ms();
}

void TestLaserSet(void)
{
		Laser=!Laser;
		TestLaser.foreColor = UI_ButtonColor(Laser);
		LCD_DisplayWidget(TestLaser);
		IODevice_LaserOn(Laser);
		__Delay100ms();
}
void TestBlowSet(void)
{
		Blow=!Blow;
		TestBlow.foreColor = UI_ButtonColor(Blow);
		LCD_DisplayWidget(TestBlow);
		IODevice_BlowOn(Blow);
		__Delay100ms();
}
void TestReturnSet(void)
{		
		
		LCD_DisplayInterface(&MainSet);
}
void TestScanSet(void)
{
		Scan=!Scan;
		TestScan.foreColor = UI_ButtonColor(Scan);
		LCD_DisplayWidget(TestScan);
		IODevice_ScanOn(Scan);
		__Delay100ms();
}
