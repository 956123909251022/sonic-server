/***********************************************************************//** 
 * \file  tkey_parameter.c
 * \brief  csi tkey parameter
 * \copyright Copyright (C) 2015-2022 @ ASMCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>ASM AE Team   <td>initial
 * </table>
 * *********************************************************************
*/ 

#include <drv/tkey.h>

void csi_tkey_parameter_init(void)
{

/****************************************************
//TK basic parameters
*****************************************************/
	dwTkeyIoEnable=TCH_EN(0)|TCH_EN(1)|TCH_EN(2)|TCH_EN(3)|TCH_EN(4)|TCH_EN(5)|TCH_EN(6)|TCH_EN(7)|             //TCH_EN(10)swdio TCH_EN(13)swclk  
				   TCH_EN(11)|TCH_EN(12)|TCH_EN(14)|TCH_EN(15)|TCH_EN(16)|TCH_EN(17)|TCH_EN(18)|TCH_EN(19)|
				   TCH_EN(20)|TCH_EN(21)|TCH_EN(22)|TCH_EN(23)|TCH_EN(25)|TCH_EN(26)|TCH_EN(27)|TCH_EN(28);     //TCH_EN(24)fvr  
				
	
	byTkeyGlobalSens=1;								//TK Global Tkey Sensitivity,0=invalid;
	hwTkeyGlobalTrigger=50;							//TK Global Tkey Trigger,0=invalid;
	byTkeyGlobalIcon=3;								//TK Global Tkey Icon,0=invalid;
	byPressDebounce=3;								//Press debounce 1~10
	byReleaseDebounce=3;							//Release debounce 1~10
	byKeyMode=1;									//Key mode 0=first singlekey,1=multi key,2=strongest single-key
	byMultiTimesFilter=0;							//MultiTimes Filter,>4 ENABLE <4 DISABLE
	byValidKeyNum=4;								//Valid Key number when touched
	byBaseUpdateSpeed=10;							//baseline update speed
	byTkeyRebuildTime=16;							//longpress rebuild time = byTkeyRebuildTime*1s  0=disable
/****************************************************
//TK parameter fine-tuning
*****************************************************/
	byTkeyFineTurn=DISABLE;							//Tkey sensitivity fine tuning ENABLE/DISABLE
	byTkeyChxSens[0]=2;								//TCHx manual Sensitivity
	byTkeyChxSens[1]=2;								//TCHx manual Sensitivity
	byTkeyChxIcon[0]=5;								//TCHx manual ICON
	byTkeyChxIcon[1]=5;								//TCHx manual ICON
/****************************************************
//TK special parameter define
*****************************************************/
	hwTkeyPselMode=TK_PWRSRC_AVDD;					//tk power sel:TK_PWRSRC_FVR/TK_PWRSRC_AVDD   when select TK_PSEL_FVR PA0.0(TCH24) need a 104 cap
	hwTkeyEcLevel=TK_ECLVL_1V;						//C0 voltage sel:TK_ECLVL_1V/TK_ECLVL_2V/TK_ECLVL_3V/TK_ECLVL_3V6
	hwTkeyFvrLevel=TK_FVR_NONE;					//FVR level:TK_FVR_2048V/TK_FVR_4096V/TK_FVR_NONE
/****************************************************
//TK low power function define
*****************************************************/
	byTkeyLowPowerMode=ENABLE;						//touch key can goto sleep when TK lowpower mode enable
	byTkeyLowPowerLevel=5;							//0=none 1=16ms 2=32ms 3=64ms 4=128ms,>4=186ms Scan interval when sleep
	byTkeyWakeupLevel=2;							//0~7;The larger the value the higher the wakeup threshold
	dwTkeyWakeupIoEnable=TCH_EN(1);					//Sleep wakeup TCHx configuration, Currently only one channel is supported
/****************************************************
//TK Hidden function define
*****************************************************/
	//Used to modify the default state when TCHx is not enabled;0/3=Z,1=high,2=low(Default)
	//byTkeyDstStatus=1;

	//Used to enable the default internal resistor of TCHx;0=disable(Default),1=enable
	//byTkeyIntrStatus=1;	

	//Used to modify the scan interval;0=none(Default),1=16ms,2=32ms,3=64ms,4=128ms,>4=186ms
	//byTkeyImtvtimTime=1;
	
	//Used to modify the Bounce of the forced negative Rebuild;>=1 (Default=3)
	//The higher the number, the longer it takes to trigger the update mechanism
	//byTkeyNegBuildBounce=10;

	//Used to modify the Bounce of the forced postive Rebuild;>=1 (Default=3)
	//The higher the number, the longer it takes to trigger the update mechanism
	//byTkeyPosBuildBounce=10;		
}


