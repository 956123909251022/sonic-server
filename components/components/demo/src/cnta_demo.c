/***********************************************************************//** 
 * \file  cnta_demo.c
 * \brief  counterA 示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-09-21 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-11-23 <td>V0.1  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "cnta.h"
#include "gpio.h"
#include "pin.h"
#include "board_config.h"
/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/


/**
  \brief       counterA 定时中断示例
  \return      int
*/
int cnta_timer_demo(void)
{	
	int iRet = 0;
	
	csi_pin_set_mux(PA110,PA110_CNTA_REM);//set counter output pin	该管脚的周期即为设置的时间
	//csi_pin_set_mux(PA113,PA113_CNTA_REM);//set counter output pin
	//csi_pin_set_mux(PA17,PA17_CNTA_REM);//set counter output pin,这个脚是烧录管脚的数据线，调试PA17_CNTA_REM功能，记得在测之前加个2-3秒的延时，除非是最终版本
	
	// (byDivTemp / pclk) * (tmp_load) *1000000*2  单位us  byDivTemp只能为1，2，4，8
	//极限时间最小:byDivTemp=1 tmp_load=1     代入得到 (1 / pclk) * (1+3) *1000000*2       
	//极限时间最大:byDivTemp=8 tmp_load=65533 代入得到 (8 / pclk) * (65533+3) *1000000*2 
	//当pclk确定后，如果设置的时间超出了范围，将会被设置成默认最大值
	//当pclk为48000000hz时，极限范围为0.167-21.845us  实际我们用到的是1-21845 us
	csi_cnta_timer_init(CNTA,1000);//1000us 进入一次中断
	csi_cnta_start(CNTA);
	while(1)
	{
		nop;
	}
	return iRet;
}

/**
  \brief       counterA pwm示例
  \return      int
*/
int cnta_pwm_demo(void)
{		
	int iRet = 0;
	csi_cnta_pwm_config_t tPwmCfg;
	
	tPwmCfg.byStartLevel = CNTA_POLAR_LOW;		//开始极性(0:DATAL控制计数器A输出的低电平宽度 1:DATAL控制计数器A输出的高电平宽度)
	tPwmCfg.byStopLevel = CNTA_STOP_LOW;    	 //结束极性低:用在CARRIERON为1，ENVELOPE为0时的条件下
	tPwmCfg.byDutyCycle = 33;           		//占空比
	tPwmCfg.byInt = CNTA_INTSRC_NONE;   		//无中断源
	tPwmCfg.wFreq = 38000;              		//频率 hz
	
	csi_pin_set_mux(PA110,PA110_CNTA_REM);//set counter output pin	
	//csi_pin_set_mux(PA113,PA113_CNTA_REM);//set counter output pin
	//csi_pin_set_mux(PA17,PA17_CNTA_REM);//set counter output pin,这个脚是烧录管脚的数据线，调试PA17_CNTA_REM功能，记得在测之前加个2-3秒的延时，除非是最终版本
	
	csi_cnta_pwm_init(CNTA,&tPwmCfg);
	csi_cnta_bt0_sync(CNTA, CNTA_PEND_CARR_SET, CNTA_MATCH_CARR_CLR,CNTA_HW_DIS);//结合bt0的定时器和pwm示例，设置硬件自动打开或者关闭载波
	csi_cnta_start(CNTA);

	while(1)
	{
		nop;
	}
	return iRet;
}

