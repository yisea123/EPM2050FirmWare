/*********************************************************************************************************
Copyright (C), 2011-8-3 ,  Skyray Instrment Co.,LTD.
File name:      	menu.h
Author:             方卫龙       Version:       V2.5 Date:      2011-8-3 11:37:43
Description:

Others:
Function List:
		extern void Menu0(void);	//记忆地址归为0x01000000
		extern void DisPleaseWait(void);	 //显示	   PleaseWait...
		extern fp32 NewLcdVol(uint32 menuid); //显示更新
		extern void GetMenuFormHoldreg(void);	//e2prom 与 hold寄存器 的读写操作
		extern void SetMenuList(void);
		extern void SetHoldregToE2rom(uint8 type);
		extern void GetHoldregFormE2rom(void);
History:
   1. Date:                2011-8-3 12:52:43
       Author:             何顺杰
	   Modification:    	规范了注释
************************************************************************************************************/
#ifndef __MENU_H
#define __MENU_H

/*********************************************************************************************************
**               头文件包含
*********************************************************************************************************/
#include "global.h"

/*********************************************************************************************************
**               C++代码兼容
*********************************************************************************************************/
#ifdef __cplusplus
extern "C" {
#endif                                                                  /*  __cplusplus                 */

//typedef struct
//{
//    uint32   KeyStateIndex;          //当前状态索引号//当前状态的id
//
//    uint32    KeyUpState;             //按下"向上"键时转向的状态索引号
//    void     (*UpFunc)();            //按下"向上"键时要执行的功能；
//    uint32    KeyDownState;           //按下"向下"键时转向的状态索引号
//    void     (*DownFunc)();
//    uint32    KeyLeftState;           //左
//    void     (*LeftFunc)();
//    uint32    KeyRightState;          //右
//    void     (*RightFunc)();
//    uint32    KeyMenuState;          //按下"menu"键
//    void     (*MenuFunc)();
//    uint32    KeyEnterState;          //按下"回车"键时转向的状态索引号
//    void     (*EnterFunc)();
//    uint32    KeyUnionState;
//    void     (*UnionFunc)();
//}KbdTabStruct;
    extern void ErrDatChange(void);
    typedef struct
    {
        uint8 Title;
        uint8 Tfont;
        uint8 TitleX1;
        uint8 TitleY1;
        uint8 TiArea1[4];

        uint8 Child;
        uint8 SetArea[8][4];

        uint8 ChildXY[8][2];     //8子菜单起始x,y坐标
        uint8 ChArea[8][4];      //8子菜单区域(for 反白)
        uint8 CenterX1;
        uint8 CenterY1;
        uint8 CeArea1[4];
        uint8 CenterX2;
        uint8 CenterY2;
        uint8 CeArea2[4];


    } MenuStruct;


    typedef struct
    {   uint32 Id;					//菜单ID
        uint8  Same;               //同级菜单个数
        uint8  Show;               //一屏可显示的菜单个数
        uint8  Lower;              //有没有子菜单
        uint8  Sel;                //菜单显示方式
        uint8	place[3];			//菜单前图标位置

        uint8  CCh[15];            //作为子菜单的内容（第二种语言或字体）
        uint8  TCh[15];            //标题内容

        uint8	CEn[25];
        uint8  TEn[25];


    } MenuTitel;


    extern void WriteCaliParTime(uint16 add,TimeDat *CaliParTime);

    extern uint8 const NextPrompt[][30];
    extern uint8 areaMiddleEE[4];
    extern uint8 const areaMenu[][4];
    extern uint8 areaMiddle[4];

    extern uint32 KeyFuncIndex;
    extern uint8  KeyUpRun,TimeOverFlg;
    extern uint8  KeyDownRun;
    extern uint8  KeyLeftRun;
    extern uint8  KeyRightRun;
    extern uint8  KeyMenuRun;
    extern uint8  KeyEnterRun;
    extern uint8  KeyDRRun,CountReadTime;
    extern uint8 area[4];
    extern int CountReaderr;
    extern uint8 FilterErrTimes;
    extern uint8  language;
    extern uint8  language1;
    extern uint8 showFlg;

    extern uint8  DisUnits;
    extern uint32 UserPassword;

    extern uint8  GetNumSign;
    extern uint32 GetInNum;
    extern uint32 GetNumMenu;               //光标左1 =1，左2 =2
    extern uint32 MenuId;
    extern uint8 ZhendianStartFlg ;
    extern uint32 MenuIdBak;
    extern uint32 MenuHistory;              //menu id
    extern MenuTitel  MenuTab[MENU_SIZE];
    extern MenuStruct const MenuFormat;
    extern int MainMeauFlg;

    extern void  (*KeyFuncPtr)();
    /*********************************************************************************************************
    * Function name: Menu0
    * Descriptions:  记忆地址归为0x01000000
    * input parameters:   无
    * output parameters:无
    * Returned value: 无
    *********************************************************************************************************/
    extern void Menu0(void);

    /*********************************************************************************************************
    * Function name:   Menm0Updata
    * Descriptions://Measure refurbish 主界面显示  时间 电量
    * input parameters:   无
    * output parameters:无
    * Returned value: 无
    *********************************************************************************************************/
    extern void Menm0Updata(void);
    extern void MainMenuSta(void);
    extern void MainMenuSen(void)	;
    extern void MainMenuSen2(void)	;
    extern void DisHexDat(uint8 *arr,uint8 dat);
    extern void OutputStation(void);
    extern void MainMenu(void)	 ;
    extern void SensorMenu(void);
    extern uint8 RunStuation, WarnStuation,
           EeorT,EeorH,EeorL,EeorP,EeorMear,EeorMenmory ;
    extern void MainMenuSta(void);
    extern uint16 EEORStuation1;

    /*********************************************************************************************************
    * Function name:DisPleaseWait
    * Descriptions: 显示please wait ***
    * input parameters:   无
    * output parameters:无
    * Returned value: 无
    *********************************************************************************************************/
    extern void DisPleaseWait(void);

    /*********************************************************************************************************
    * Function name:   NewLcdVol
    * Descriptions:      刷新显示
    * input parameters:   menuid
    * output parameters:无
    * Returned value: 0
    *********************************************************************************************************/
    extern void NewLcdVol(uint32 menuid);

    /*********************************************************************************************************
    * Function name:GetMenuFormHoldreg
    * Descriptions: 从寄存器中读值 并赋值给相应参数
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void GetMenuFormHoldreg(void);

    /*********************************************************************************************************
    * Function name:SetMenuList
    * Descriptions:  测量参数赋值
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void SetMenuList(void);

    /*********************************************************************************************************
    * Function name: SetHoldregToE2rom
    * Descriptions:         将hold寄存器的值写入 e2prom
    * input parameters: type 写入的字节数类型
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void SetHoldregToE2rom(uint8 type);

    /*********************************************************************************************************
    * Function name:   GetHoldregFormE2rom
    * Descriptions:   从e2prom中读数到 hold寄存器
    * input parameters:
    * output parameters:
    * Returned value:
    *********************************************************************************************************/
    extern void GetHoldregFormE2rom(void);
    /*********************************************************************************************************
    * Function name: prompt
    * Descriptions: 提示信息
    * input parameters:   type 提示信息种类 0-5
    						0 esc         enter
    						1 esc  updown enter
    						2			  enter
    						3 esc  updown enter
    						4 esc  enter
    						5 esc  updown enter
    * output parameters:无
    * Returned value: 无
    *********************************************************************************************************/

    extern void prompt(uint8 type);
    extern void ShowEeor(uint16 errdat1);

//extern uint16 stata_ydata_Buzzer,stata_ydata_Contrast,stata_ydata_Print,stata_ydata_Battry,stata_xdata;
//extern uint16 stata_ydata_PrintOnOff,stata_back_light;
//extern MainMenu(void) ;

//
//extern uint8 prompt0[];
//extern uint8 prompt1[];
//extern uint8 prompt2[];
//extern uint8 prompt3[];
//extern uint8 prompt4[];
//extern uint8 prompt5[];


#ifdef __cplusplus
}
#endif                                                                  /*  __cplusplus                 */

#endif

/********************************************************************************************************
  End Of File
********************************************************************************************************/

