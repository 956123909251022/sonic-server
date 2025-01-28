/***********************************************************************//** 
 * \file  timer_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/bt.h>
#include <drv/pin.h>
#include <drv/etb.h>

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief bt timer
 * 
 *  \param[in] none
 *  \return error code
 */
int bt_timer_demo(void)
{
	int iRet = 0;
			
	csi_bt_timer_init(BT0, 10000);		//初始化BT0, 定时10000us； BT定时，默认采用PEND中断
	
	csi_bt_start(BT0);					//启动定时器

	return iRet;
}

/** \brief bt pwm ouput
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_pwm_demo(void)
{
	int iRet = 0;
	csi_bt_pwm_config_t tPwmCfg;							//BT PWM输出参数初始化配置结构体
	
	csi_pin_set_mux(PA19, PA19_BT0_OUT);					//PB02 作为BT0 PWM输出引脚
	
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1;								//PWM 输出频率
	//tPwmCfg.byInt 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInt		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT0, &tPwmCfg);							//初始化BT0 PWM输出
	csi_bt_start(BT0);										//启动BT0
		
	csi_bt_pwm_duty_cycle_updata(BT0, 10);
	csi_bt_pwm_duty_cycle_updata(BT0, 0);
	csi_bt_pwm_updata(BT0, 1000, 20);
	csi_bt_pwm_duty_cycle_updata(BT0, 50);
	csi_bt_pwm_duty_cycle_updata(BT0, 100);
	csi_bt_pwm_duty_cycle_updata(BT0, 70);
	csi_bt_pwm_updata(BT0, 10000, 50);
	csi_bt_pwm_duty_cycle_updata(BT0, 40);
	csi_bt_pwm_duty_cycle_updata(BT0, 0);
	
	return iRet;
}

/** \brief bt sync trg start 
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_start_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体		
	
	csi_pin_set_mux(PB01, PB01_INPUT);									//PB01 配置为输入
	csi_pin_pull_mode(PB01, GPIO_PULLUP);								//PB01 上拉
	csi_pin_irq_mode(PB01, EXI_GRP1, GPIO_IRQ_FALLING_EDGE);			//PB01 下降沿产生中断，选择中断组1
	csi_pin_irq_enable(PB01, ENABLE);									//PB01 中断使能	
	
	csi_exi_set_evtrg(EXI_TRGOUT0, TRGSRC_EXI1, 3);						//EXI1 触发EXI_TRGOUT0
	
	csi_bt_timer_init(BT0,20000);										//BT 20ms定时
	csi_bt_set_sync(BT0, BT_TRG_SYNCIN0, BT_TRG_ONCE, DISABLE);		//外部触发bt启动(SYNCIN0)
	
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT0;  	    //EXI_TRGOUT0作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	
	return iRet;
}

/** \brief bt sync trg count
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_count_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体

	csi_pin_set_mux(PB01, PB01_INPUT);									//PB01 配置为输入
	csi_pin_pull_mode(PB01, GPIO_PULLUP);								//PB01 上拉
	csi_pin_irq_mode(PB01, EXI_GRP18, GPIO_IRQ_FALLING_EDGE);			//PB01 下降沿产生中断，选择中断组18	
	csi_pin_irq_enable(PB01, ENABLE);									//PB01 中断使能
	
	csi_exi_set_evtrg(EXI_TRGOUT4, TRGSRC_EXI18, 0);					//EXI18(PB01) 触发EXI_TRGOUT4(PB01用EXI18触发输出)
	
	csi_bt_timer_init(BT0,20);											//BT定时
	csi_bt_set_sync(BT0, BT_TRG_SYNCIN1, BT_TRG_CONTINU, DISABLE);	//外部触发bt计数(SYNCIN1)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT4;  	    //EXI_TRGOUT4作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN1;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/** \brief bt sync trg count
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_sync_trg_stop_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体

	csi_pin_set_mux(PB01, PB01_INPUT);									//PB01 配置为输入
	csi_pin_pull_mode(PB01, GPIO_PULLUP);								//PB01 上拉
	csi_pin_irq_mode(PB01, EXI_GRP16, GPIO_IRQ_FALLING_EDGE);			//PB01 下降沿产生中断，选择中断组16	
	csi_pin_irq_enable(PB01, ENABLE);									//PB01 中断使能
	
	csi_exi_set_evtrg(EXI_TRGOUT5, TRGSRC_EXI16, 0);					//EXI16(PB01) 触发EXI_TRGOUT5(PB01用EXI16触发输出)
	
	csi_bt_timer_init(BT0,2000);										//BT定时
	csi_bt_set_sync(BT0, BT_TRG_SYNCIN2, BT_TRG_ONCE, DISABLE);		//外部触发停止BT(SYNCIN2)
	csi_bt_start(BT0);													//启动定时器
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT5;  	    //EXI_TRGOUT5作为触发源
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN2;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
	
}

/** \brief bt event trg count
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_trg_out_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体
	
	csi_bt_timer_init(BT0,10000);										//BT定时10ms
	csi_bt_set_evtrg(BT0, BT_TRGOUT, BT_TRGSRC_PEND);					//BT0 PEND事件触发输出
	csi_bt_start(BT0);													//启动定时器
	
	csi_bt_timer_init(BT1,1000);										//BT定时1ms
	csi_bt_count_mode(BT1, BT_CNT_ONCE);
	csi_bt_set_sync(BT1, BT_TRG_SYNCIN0, BT_TRG_CONTINU, DISABLE);		//外部触发bt启动(SYNCIN0)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  							//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT;  	    						//EXI_TRGOUT4作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    					//BT1 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/** \brief bt soft trg out
 *  
 *  \param[in] none
 *  \return error code
 */
int bt_soft_trg_out_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               			//ETB 参数配置结构体
	
	csi_bt_timer_init(BT1,1000);										//BT定时1ms
	csi_bt_set_sync(BT1, BT_TRG_SYNCIN0, BT_TRG_ONCE, DISABLE);			//外部触发bt启动(SYNCIN0)
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  							//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT;  	    						//EXI_TRGOUT4作为触发源
	tEtbConfig.byDstIp =  ETB_BT1_SYNCIN0;   	    					//BT1 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	
	
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	csi_bt_soft_evtrg(BT0);							//BT0软件触发输出
	
	return iRet;
}