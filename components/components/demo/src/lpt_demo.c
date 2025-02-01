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
#include <drv/lpt.h>
#include <drv/pin.h>
#include <drv/etb.h>
#include <drv/bt.h>
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

//	csi_lpt_change_duty(LPT, 0);
//	csi_lpt_change_duty(LPT, 50);
//	csi_lpt_change_duty(LPT, 100);
//	mdelay(1000);
//	csi_lpt_change_duty(LPT, 50);
	
	return iRet;	
}

/** \brief lpt sync trg count
 *  
 *  \param[in] none
 *  \return error code
 */
int lbt_sync_trg_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体

	csi_pin_set_mux(PB00, PB00_INPUT);									//PB00 配置为输入
	csi_pin_pull_mode(PB00, GPIO_PULLUP);								//PB00 上拉
	csi_pin_irq_mode(PB00, EXI_GRP0, GPIO_IRQ_FALLING_EDGE);			//PB00 下降沿产生中断，选择中断组16
	csi_pin_irq_enable(PB00, ENABLE);									//PB00 中断使能

	csi_exi_set_evtrg(EXI_TRGOUT0, TRGSRC_EXI0, 0);						//EXI0(PB00) 触发EXI_TRGOUT0(PB00用EXI0触发输出)

	csi_lpt_timer_init(LPT,LPT_CLK_PCLK_DIV4,5);       					//初始化lpt
	csi_lpt_set_sync(LPT, LPT_TRG_SYNCIN0, LPT_SYNC_ONCE, ENABLE);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT0;  	    //EXI_TRGOUT5作为触发源
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN0;   	    //LPT同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	
	return iRet;
}

/** \brief lpt trg out
 *  
 *  \param[in] none
 *  \return error code
 */
int lbt_trg_out_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				    				//ETB 参数配置结构体

	csi_lpt_timer_init(LPT,LPT_CLK_PCLK_DIV4,50);   				//初始化lpt,选用内部超低功耗时钟,定时50ms,默认采用PEND中断
	csi_lpt_set_evtrg(LPT, LPT_TRGOUT0, LPT_TRGSRC_PRD, 1);
	
	csi_bt_timer_init(BT1,5000);									//BT定时1ms
	csi_bt_set_sync(BT1, BT_TRG_SYNCIN0, BT_TRG_ONCE, DISABLE);		//外部触发bt启动(SYNCIN0)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0;  	    				//LPT作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    				//BT1同步输入0 作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    				//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    				//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_lpt_start(LPT);	                            				//启动lpt,触发BT1启动
	
	return iRet;
}

/** \brief lpt software trg out
 *  
 *  \param[in] none
 *  \return error code
 */
int lbt_soft_trg_out_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				    				//ETB 参数配置结构体
	
	csi_bt_timer_init(BT1,5000);									//BT定时1ms
	csi_bt_set_sync(BT1, BT_TRG_SYNCIN0, BT_TRG_ONCE, DISABLE);		//外部触发bt启动(SYNCIN0)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  						//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0;  	    				//LPT作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    				//BT1同步输入0 作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    				//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    				//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_lpt_soft_evtrg(LPT);	                            		//lpt软件触发, 触发BT1启动
	
	return iRet;
}
