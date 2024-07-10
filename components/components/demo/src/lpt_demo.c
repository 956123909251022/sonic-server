/***********************************************************************//** 
 * \file  lpt_demo.c
 * \brief  LPT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V0.0 <td>YYM     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <lpt.h>
#include <pin.h>
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief lpt timer
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_timer_demo(void)
{
	int iRet = 0;
	
	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK,500);       //初始化lpt,选用内部超低功耗时钟,定时200ms,默认采用PEND中断
	csi_lpt_start(LPT);	                             //启动lpt
	
	return iRet;	
}

/** \brief lpt pwm ouput
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_pwm_demo(void)
{
	int iRet = 0;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PA01, PA01_LPT_INOUT);						  //将PA01设为LPT_OUT
	
	tLptPwmCfg.byClksrc = LPT_CLK_PCLK_DIV4;                     //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_HIGH;                  //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                    //停止时highZ
	tLptPwmCfg.byCycle = 80;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInt 	= LPT_INTSRC_NONE;	                     //PWM 配置无中断
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
		csi_lpt_start(LPT);                                      //启动lpt
	}	
	
	return iRet;	
}

/** \brief lpt pwm duty ouput
 * 
 *  \param[in] none
 *  \return error code
 */
int lpt_pwm_duty_demo(void)
{
	int iRet = 0;
	uint32_t i;
	
	csi_lpt_pwm_config_t tLptPwmCfg;  	
	csi_pin_set_mux(PA01, PA01_LPT_INOUT);						  //将PA01设为LPT_OUT
	
	tLptPwmCfg.byClksrc = LPT_CLK_ISCLK;                         //PWM 时钟选择
	tLptPwmCfg.byStartpol = LPT_PWM_START_HIGH;                  //初始低电平
	tLptPwmCfg.byIdlepol  = LPT_PWM_IDLE_LOW;                    //停止时highZ
	tLptPwmCfg.byCycle = 80;                                     //PWM 输出占空比(0~100)	
	tLptPwmCfg.wFreq = 1000;                                     //PWM 输出频率
	tLptPwmCfg.byInt 	= LPT_INTSRC_NONE;	                     //PWM 配置无中断
	if(csi_lpt_pwm_init(LPT, &tLptPwmCfg) == CSI_OK){            //初始化lpt
		csi_lpt_start(LPT);                                      //启动lpt
	}
	for(i = 0;i < 101;i++)
	{
		mdelay(200);
		csi_lpt_change_duty(LPT, i);	
	}
	mdelay(1000);
	csi_lpt_change_duty(LPT, 50);
	
	return iRet;	
}
