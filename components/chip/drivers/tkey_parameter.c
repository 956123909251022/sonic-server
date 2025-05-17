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

#include "csp_tkey.h"
#include <drv/tkey.h>

void csi_tkey_parameter_init(void)
{

/****************************************************
//TK basic parameters
*****************************************************/
	dwTkeyIoEnable=TCH_EN(0)|TCH_EN(1)|TCH_EN(2)|TCH_EN(3)|TCH_EN(4)|TCH_EN(5)|TCH_EN(6)|TCH_EN(7)|TCH_EN(9)|                 //TCH_EN(10)swdio  tch8 tx
                                 TCH_EN(11)|TCH_EN(12)|TCH_EN(14)|TCH_EN(15)|TCH_EN(16)|TCH_EN(17)|TCH_EN(18)|TCH_EN(19)|TCH_EN(20)|                 //TCH_EN(13)swclk                
                                 TCH_EN(21)|TCH_EN(22)|TCH_EN(23)|TCH_EN(25)|TCH_EN(26)|TCH_EN(27)|TCH_EN(28)|TCH_EN(29)|TCH_EN(30)|                //TCH_EN(24)fvr        
                                 TCH_EN(31);

	byTkeyGlobalSens=3;								//TK Global Tkey Sensitivity,0=invalid;
	byTkeyGlobalTrigger=50;							//TK Global Tkey Trigger,0=invalid;
	byTkeyGlobalIcon=4;								//TK Global Tkey Icon,0=invalid;
	byPressDebounce=3;								//Press debounce 1~10
	byReleaseDebounce=3;							//Release debounce 1~10
	byKeyMode=2;									//Key mode 0=first singlekey,1=multi key,2=strongest single-key
	byMultiTimesFilter=0;							//MultiTimes Filter,>4 ENABLE <4 DISABLE
	byValidKeyNum=2;								//Valid Key number when touched
	byBaseUpdateSpeed=5;									//baseline update speed
	byTkeyRebuildTime=10;							//longpress rebuild time = byTkeyRebuildTime*1s  0=disable
/****************************************************
//TK parameter fine-tuning
*****************************************************/
	byTkeyFineTurn=DISABLE;							//Tkey sensitivity fine tuning ENABLE/DISABLE
	byTkeyChxSens[0]=4;								//TCHx manual Sensitivity
	byTkeyChxSens[1]=4;								//TCHx manual Sensitivity
/****************************************************
//TK special parameter define
*****************************************************/
	hwTkeyPselMode=TK_PWRSRC_AVDD;					//tk power sel:TK_PWRSRC_FVR/TK_PWRSRC_AVDD   when select TK_PSEL_FVR PA0.0(TCH24) need a 104 cap
	hwTkeyEcLevel=TK_ECLVL_1V;						//C0 voltage sel:TK_ECLVL_1V/TK_ECLVL_2V/TK_ECLVL_3V/TK_ECLVL_3V6
	hwTkeyFvrLevel=TK_FVR_NONE;						//FVR level:TK_FVR_2048V/TK_FVR_4096V/TK_FVR_NONE
}