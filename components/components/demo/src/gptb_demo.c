/***********************************************************************//** 
 * \file  gptb_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11  <td>V0.0 <td>ljy     <td>initial
 * <tr><td> 2023-5-10  <td>V0.1 <td>wch     <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/gptb.h>
#include <drv/pin.h>
#include "drv/etb.h"
#include "demo.h"
#include <irq.h>

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
static uint32_t s_wGptbCapBuff[4] = {0};

/** \brief GPTB基本定时功能
 * 
 *  \param[in] none
 *  \return error code
 */
int gptb_timer_demo(void)
{
	int iRet = 0;
	
	csi_gptb_timer_init(GPTB0, 10000);		//初始化GPTB0, 定时10000us； GPTB定时，默认采用向上计数，PEND中断
	
	csi_gptb_start(GPTB0);                  //启动定时器

	return iRet;	
}

/** \brief GPTB捕获示例代码， 测量周期时间
 *   		- 捕获4次产生一次捕获中断，CMPBA捕获后，计数器进行重置
 *     		- 由PA01触发外部事件1，经过ETCB  触发sync2 捕获
 * 			- 信号由PA01的高低电平切换产生（一直高电平意味着没有触发）
 *          - CMPA捕获的是第一次周期值，CMPB捕获的是第二次周期值，CMPAA捕获的是第三次周期值,CMPBA捕获的是第四次周期值
 *  \param[in] none
 *  \return error code
 * 
 * 
 PA01输入波形——          —————          —————           —————          —————
				|          |        |          |        |           |         |         |        |
				|          |        |          |        |           |         |         |        |
				——————        ——————         ——————          —————        ————
			   CMPA                CMPB                CMPAA                CMPBA               CMPA   
*/
int gptb_capture_sync_demo0(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
    volatile uint8_t ch;

	csi_pin_set_mux(PA01,PA01_INPUT);		
	csi_pin_pull_mode(PA01, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断
	csi_pin_irq_enable(PA01, ENABLE);							//使能GPIO中断	
	csi_exi_set_evtrg(EXI_TRGOUT1, TRGSRC_EXI1, 1);	
//------------------------------------------------------------------------------------------------------------------------		
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT1 ;  	//...作为触发源
	tEtbConfig.byDstIp   =  ETB_GPTB0_SYNCIN2;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_captureconfig_t tCapCfg;								  
	tCapCfg.byWorkmod        =  GPTB_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tCapCfg.byCountingMode   =  GPTB_UPCNT;                       //CNYMD  //计数方向
	tCapCfg.byStartSrc       =  GPTB_SYNC;				    	//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tCapCfg.byPscld          =  GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tCapCfg.byCaptureCapmd    = GPTB_CAPMD_CONT;                  //0:连续捕捉模式    1h：一次性捕捉模式
	tCapCfg.byCaptureStopWrap = 4-1;                              //Capture模式下，捕获事件计数器周期设置值
	tCapCfg.byCaptureLdaret   = 0;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tCapCfg.byCaptureLdbret   = 0;    
	tCapCfg.byCaptureLdaaret  = 0;    
	tCapCfg.byCaptureLdbaret  = 1;    	
	tCapCfg.wInt 		      = GPTB_INTSRC_CAPLD3;                   //interrupt
	csi_gptb_capture_init(GPTB0, &tCapCfg);	
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_set_sync (GPTB0, GPTB_TRG_SYNCEN2, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);
//------------------------------------------------------------------------------------------------------------------------
//	csi_gptb_filter_config_t  tFiltercfg;
//	tFiltercfg.byFiltSrc        =GPTB_FILT_SYNCIN2;
//	tFiltercfg.byWinInv         =1;                              //0h：窗口不反转，窗口有效区间禁止滤波输入;  1h：窗口反转，  窗口有效区间使能滤波输入
//	tFiltercfg.byWinAlign       =GPTB_ALIGN_ZRO;                  //窗口对齐模式选择															  
//	tFiltercfg.byWinCross       =1;                              //滤波窗跨越窗口对齐点:  0h：禁止跨窗口对齐点;  1h：允许跨窗口对齐点
//	tFiltercfg.byWinOffset      =g_wGptb0Prd/2;
//	tFiltercfg.byWinWidth       =g_wGptb0Prd/2;
//    csi_gptb_set_sync_filter(GPTB0, &tFiltercfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_start(GPTB0);//start  timer
	
	while(1){		
		  		      
		    mdelay(200);                        
		    
		    mdelay(200);
	}	
	
	return iRet;
}

/** \brief GPTB捕获示例代码，测试低电平时间
 *   		- 捕获1次产生一次捕获中断，CMPA捕获后，计数器进行重置
 *     		- 由PA01下降沿产生外部事件0，经过ETCB  触发sync0，重置和启动计数器
 *          - 由PA01外部扩展口，上升沿产生外部事件5，经过ETCB  触发sync2 捕获，上升沿捕获值存放在CMPA中
 * 			- 信号由PA01的高低电平切换产生（一直高或低电平意味着没有触发）
 *          - CMPA捕获的是下降沿时间
 *  \param[in] none
 *  \return error code
 * 
 * 
 PA01输入波形——          —————          —————           ———
                |          |        |          |        |           |        
	            |          |        |          |        |           |        
                ——————        ——————         ——————          
                RESET      CMPA     RESET     CMPA      RESET       CMPA               

*/
int gptb_capture_sync_demo1(void)
{
	int iRet = 0;	
    volatile uint8_t ch;

	csi_pin_set_mux(PA01,PA01_INPUT);		
	csi_pin_pull_mode(PA01, GPIO_PULLUP);						//PA01 上拉
	
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断，选择中断组1
	csi_exi_set_evtrg(EXI_TRGOUT0, TRGSRC_EXI1, 1);				//PA01 下降沿产生事件0
	csi_pin_irq_mode(PA01,EXI_GRP16,GPIO_IRQ_RISING_EDGE);    	//PA01 上升沿产生中断，选择中断组16                                   
	csi_exi_set_evtrg(EXI_TRGOUT5, TRGSRC_EXI16,1);	 			//PA01 上升沿产生事件5
	
	csi_pin_irq_enable(PA01, ENABLE);                           //PA01 中断使能 
//------------------------------------------------------------------------------------------------------------------------	
	csi_etb_config_t tEtbConfig;	
	//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT0 ;  	//...作为触发源
	tEtbConfig.byDstIp   = ETB_GPTB0_SYNCIN0;  //GPTB0 同步输入0作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);			
//------------------------------------------------------------------------------------------------------------------------		
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT5 ;  	//...作为触发源
	tEtbConfig.byDstIp   = ETB_GPTB0_SYNCIN2;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod         = GPTB_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode    = GPTB_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode     = GPTB_OP_CONT; 
	tPwmCfg.byStartSrc        = GPTB_SYNC;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld           = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd    = 0;                               //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap = 1-1;                              //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   = 1;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   = 0; 
	tPwmCfg.byCaptureLdaaret  = 0;  
	tPwmCfg.byCaptureLdbaret  = 0;  
	tPwmCfg.wInt 		      = GPTB_INTSRC_CAPLD0;                   //interrupt//

	csi_gptb_capture_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_set_sync(GPTB0, GPTB_TRG_SYNCEN0, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN0外部触发
    csi_gptb_set_sync(GPTB0, GPTB_TRG_SYNCEN2, GPTB_TRG_CONTINU, GPTB_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
	csi_gptb_start(GPTB0);//start  timer
    while(1){
		mdelay(200);                        
		mdelay(200);
	}			
	return iRet;
};


/** \brief GPTB波形输出示例代码
 *   		-10kHZ，占空比50%   输出波形
 *     		-可通过以下两种方式灵活调整PWM参数
 * 			--csi_gptb_change_ch_duty：修改PWM占空比
 *			--csi_gptb_prdr_cmp_update：修改PWM周期寄存器和比较寄存器的值			-
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_demo(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA013, PA013_GPTB0_CHAX);						//PIN:8
    csi_pin_set_mux(PA014, PA014_GPTB0_CHAY);                       //PIN:9 
    csi_pin_set_mux(PB04,  PB04_GPTB0_CHB  );                       //PIN:13 
//------------------------------------------------------------------------------------------------------------------------	
    csi_gptb_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_OP_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SYNC_START;					//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 10000;							//pwm ouput frequency	
	tPwmCfg.wInt 			 = GPTB_INTSRC_NONE;                //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------		
	csi_gptb_pwmchannel_config_t  channel;
	channel.byActionZro    =   GPTB_LO;
	channel.byActionPrd    =   GPTB_NA;
	channel.byActionC1u    =   GPTB_HI;
	channel.byActionC1d    =   GPTB_LO;
	channel.byActionC2u    =   GPTB_HI;
	channel.byActionC2d    =   GPTB_LO;
	channel.byActionT1u    =   GPTB_LO;
	channel.byActionT1d    =   GPTB_LO;
	channel.byActionT2u    =   GPTB_NA;
	channel.byActionT2d    =   GPTB_NA;
	channel.byChoiceC1sel  =   GPTB_CMPA;
	channel.byChoiceC2sel  =   GPTB_CMPA;
	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_1);//channel
	channel.byChoiceC1sel  =   GPTB_CMPB;
	channel.byChoiceC2sel  =   GPTB_CMPB;
	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------
//	csi_gptb_Global_load_control_config_t  Gldcfg;
//	Gldcfg.bGlden        =  ENABLE;//DISABLE               //全局载入使能
//    Gldcfg.bOstmd        =  DISABLE;                       //One Shot 载入模式使能控制位
//	Gldcfg.bGldprd       =  0;                             //周期值
//	Gldcfg.byGldcnt      =  0;							   //计数值
//	Gldcfg.byGldmd       =  GPTB_LDGLD_SW;//GPTB_LDGLD_PRD;                //选择触发载入条件
//	csi_gptb_global_config(GPTB0,&Gldcfg);
//	
//	csi_gptb_gldcfg(GPTB0 ,bycmpa_B ,ENABLE);
//	csi_gptb_gldcfg(GPTB0 ,bycmpb_B ,ENABLE);
////	csi_gptb_global_rearm(GPTB0) ;                         //重置ONE SHOT模式。ONE SHOT模式下，一次触发后，需要重置模式才允许再次触发
////	csi_gptb_global_sw(GPTB0) ;                            //软件产生一次GLD触发
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_set_evtrg(GPTB0, GPTB_TRGOUT0, GPTB_TRGSRC_ZRO);      //TRGSEL0
	csi_gptb_set_evtrg(GPTB0, GPTB_TRGOUT1, GPTB_TRGSRC_PRD);      //TRGSEL1
	csi_gptb_int_enable(GPTB0, GPTB_INTSRC_TRGEV0 , ENABLE);
	csi_gptb_int_enable(GPTB0, GPTB_INTSRC_TRGEV1 , ENABLE);
	
	csi_gptb_start(GPTB0);//start  timer
//------------------------------------------------------------------------------------------------------------------------	
	while(1)
	{	
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA,50);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB,50);
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPA,2400,600); 	//修改PWM1周期为2400，比较值为600
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPB,2400,600); 	//修改PWM2周期为2400，比较值为600 
		mdelay(100); 

		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 20);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 20);
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPA,2400,1800); 	//修改PWM1周期为2400，比较值为1800
		csi_gptb_prdr_cmp_update(GPTB0,GPTB_COMPB,2400,1800); 	//修改PWM2周期为2400，比较值为1800 
		mdelay(100);	
	}
	return iRet;
}

/** \brief GPTB波形输出+互补对称波形+死区时间（上升沿500ns+下降沿200ns）
 *   		-10kHZ   PA015、PA014、PB05输出波形；PA012、PA08、PA04输出反向波形
 *     		-PWM在80%和40%之间切换
 * 			-此为电机FOC基本波形设置
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_dz_demo(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA013, PA013_GPTB0_CHAX);						//PIN:8
    csi_pin_set_mux(PA014, PA014_GPTB0_CHAY);                       //PIN:9 
    csi_pin_set_mux(PB04,  PB04_GPTB0_CHB  );                       //PIN:13 
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = GPTB_WAVE;                        //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = GPTB_UPDNCNT;                     //CNYMD  //计数方向
	tPwmCfg.byOneshotMode   = GPTB_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc      = GPTB_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byDutyCycle 	= 50;							    //pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			= 10000;						    //pwm ouput frequency			
	tPwmCfg.wInt 		    = GPTB_INTSRC_NONE;                  //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//	csi_gptb_set_sync (GPTB0, GPTB_TRG_SYNCEN2, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);	
//	csi_gptb_set_evtrg(GPTB0, GPTB_TRGOUT0, GPTB_TRGSRC_PE1);    //EP1用trg0输出，
//	csi_gptb_int_enable(GPTB0, GPTB_INT_TRGEV0,true);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_pwmchannel_config_t  tGptbchannelCfg;
	tGptbchannelCfg.byActionZro    =   GPTB_LO;
	tGptbchannelCfg.byActionPrd    =   GPTB_NA;
	tGptbchannelCfg.byActionC1u    =   GPTB_HI;
	tGptbchannelCfg.byActionC1d    =   GPTB_LO;
	tGptbchannelCfg.byActionC2u    =   GPTB_NA;
	tGptbchannelCfg.byActionC2d    =   GPTB_NA;
	tGptbchannelCfg.byActionT1u    =   GPTB_LO;
	tGptbchannelCfg.byActionT1d    =   GPTB_LO;
	tGptbchannelCfg.byActionT2u    =   GPTB_NA;
	tGptbchannelCfg.byActionT2d    =   GPTB_NA;
	tGptbchannelCfg.byChoiceC1sel  =   GPTB_CMPA;
	tGptbchannelCfg.byChoiceC2sel  =   GPTB_CMPA;	
	csi_gptb_channel_config(GPTB0, &tGptbchannelCfg,  GPTB_CHANNEL_1);//channel
	tGptbchannelCfg.byChoiceC1sel  =   GPTB_CMPB;
	tGptbchannelCfg.byChoiceC2sel  =   GPTB_CMPB;	
	csi_gptb_channel_config(GPTB0, &tGptbchannelCfg,  GPTB_CHANNEL_2);
		
//csp_gptb_set_aqtscr(GPTB0,GPTB_T1,EP1);//波形输出T事件选择
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_deadzone_config_t  tGptbDeadZoneCfg;
	tGptbDeadZoneCfg.byDcksel               = GPTB_DB_DPSC;    //
	tGptbDeadZoneCfg.hwDpsc                 = 0;              //FDBCLK = FHCLK / (DPSC+1)
	tGptbDeadZoneCfg.hwRisingEdgeTime       = 500;             //上升沿-ns
	tGptbDeadZoneCfg.hwFallingEdgeTime      = 200;             //下降沿-ns
	tGptbDeadZoneCfg.byChaDedb              = GPTB_DB_AR_BF;      //不使用死区双沿
	csi_gptb_dz_config(GPTB0,&tGptbDeadZoneCfg);
	
	tGptbDeadZoneCfg.byChxOuselS1S0      = GPTB_DBOUT_AR_BF;      //使能通道A的上升沿延时，使能通道B的下降沿延时
	tGptbDeadZoneCfg.byChxPolarityS3S2   = GPTB_DB_POL_B;         //通道A和通道B延时输出电平是否反向
	tGptbDeadZoneCfg.byChxInselS5S4      = GPTB_DBCHAIN_AR_AF;    //PWMA作为上升沿和下降沿延时处理的输入信号
	tGptbDeadZoneCfg.byChxOutSwapS8S7    = GPTB_DBCHAOUT_OUTA_A_OUTB_B;   //OUTA=通道A输出，OUTB=通道B输出
    csi_gptb_channelmode_config(GPTB0,&tGptbDeadZoneCfg,GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_start(GPTB0);//start  timer
    while(1)
	{			
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 80);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 80);		  
		mdelay(200);                        
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 40);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 40);
		mdelay(200);
	}			
	return iRet;
};

/** \brief GPTB波形输出+互补对称波形+死区时间（上升沿500ns+下降沿200ns）+软锁止/硬锁止
 *   		-10kHZ   
 *     		-PWM在80%和40%之间切换
 * 			-此为电机FOC基本波形设置
 *          -EBIx的输入需上拉或下拉，也可开启内部的上下拉
 *  \param[in] none
 *  \return error code
 */
int gptb_pwm_dz_em_demo(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
    csi_pin_set_mux(PA013, PA013_GPTB0_CHAX);						//PIN:8
    csi_pin_set_mux(PA014, PA014_GPTB0_CHAY);                       //PIN:9 

	csi_pin_set_mux(PA09,PA09_EBI0);csi_pin_pull_mode(PA09,GPIO_PULLUP);
	csi_pin_set_mux(PB04,PB04_EBI1);csi_pin_pull_mode(PB04,GPIO_PULLUP);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = GPTB_WAVE;                        //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = GPTB_UPDNCNT;                     //CNYMD  //计数方向
	tPwmCfg.byOneshotMode   = GPTB_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc      = GPTB_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = GPTB_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byDutyCycle 	= 50;							    //pwm ouput duty cycle//PWM初始值(0~100)		
	tPwmCfg.wFreq 			= 10000;						    //pwm ouput frequency			
	tPwmCfg.wInt 			= GPTB_INTSRC_PEND;                                //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gptb_pwmchannel_config_t  tGptbchannelCfg;
	tGptbchannelCfg.byActionZro    =   GPTB_LO;
	tGptbchannelCfg.byActionPrd    =   GPTB_NA;
	tGptbchannelCfg.byActionC1u    =   GPTB_HI;
	tGptbchannelCfg.byActionC1d    =   GPTB_LO;
	tGptbchannelCfg.byActionC2u    =   GPTB_NA;
	tGptbchannelCfg.byActionC2d    =   GPTB_NA;
	tGptbchannelCfg.byActionT1u    =   GPTB_LO;
	tGptbchannelCfg.byActionT1d    =   GPTB_LO;
	tGptbchannelCfg.byActionT2u    =   GPTB_NA;
	tGptbchannelCfg.byActionT2d    =   GPTB_NA;
	tGptbchannelCfg.byChoiceC1sel  =   GPTB_CMPA;
	tGptbchannelCfg.byChoiceC2sel  =   GPTB_CMPA;	
	csi_gptb_channel_config(GPTB0, &tGptbchannelCfg,  GPTB_CHANNEL_1);//channel	
	tGptbchannelCfg.byChoiceC1sel  =   GPTB_CMPB;
	tGptbchannelCfg.byChoiceC2sel  =   GPTB_CMPB;	
	csi_gptb_channel_config(GPTB0, &tGptbchannelCfg,  GPTB_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gptb_deadzone_config_t  tGptbDeadZoneCfg;
	tGptbDeadZoneCfg.byDcksel               = GPTB_DB_DPSC;     //
	tGptbDeadZoneCfg.hwDpsc                 = 0;              //FDBCLK = FHCLK / (DPSC+1)
	tGptbDeadZoneCfg.hwRisingEdgeTime   = 500;             //上升沿-ns
	tGptbDeadZoneCfg.hwFallingEdgeTime  = 200;             //下降沿-ns
	tGptbDeadZoneCfg.byChaDedb              = GPTB_DB_AR_BF;        //不使用死区双沿
	csi_gptb_dz_config(GPTB0,&tGptbDeadZoneCfg);
	
	tGptbDeadZoneCfg.byChxOuselS1S0      = GPTB_DBOUT_AR_BF;      //使能通道A的上升沿延时，使能通道B的下降沿延时
	tGptbDeadZoneCfg.byChxPolarityS3S2   = GPTB_DB_POL_B;         //通道A和通道B延时输出电平是否反向
	tGptbDeadZoneCfg.byChxInselS5S4      = GPTB_DBCHAIN_AR_AF;    //PWMA作为上升沿和下降沿延时处理的输入信号
	tGptbDeadZoneCfg.byChxOutSwapS8S7    = GPTB_DBCHAOUT_OUTA_A_OUTB_B;   //OUTA=通道A输出，OUTB=通道B输出  CHOUTX_OUA_OUB
    csi_gptb_channelmode_config(GPTB0,&tGptbDeadZoneCfg,GPTB_CHANNEL_1);
//------------------------------------------------------------------------------------------------------------------------	
    csi_gptb_emergency_config_t   tGptbEmCfg;             //紧急状态输入
    tGptbEmCfg.byEpxInt    = GPTB_EBI0 ;                     //EPx选择外部IO端口（EBI0~EBI4）
    tGptbEmCfg.byPolEbix   = GPTB_EBI_POL_L;                 //EBIx的输入有效极性选择控制
	tGptbEmCfg.byEpx       = GPTB_EP3;                       //使能EPx
	tGptbEmCfg.byEpxLckmd  = GPTB_EP_HLCK;                   //使能 软/硬 锁
	tGptbEmCfg.byOsrshdw   = GPTB_IMMEDIATE;                 //锁止端口状态载入方式
    tGptbEmCfg.byFltpace0  = GPTB_EPFLT0_2P;                 //EP0、EP1、EP2和EP3的数字去抖滤波检查周期数
	if(tGptbEmCfg.byEpxInt == GPTB_ORL0){tGptbEmCfg.byOrl0 = GPTB_ORLx_EBI0 |GPTB_ORLx_EBI1|GPTB_ORLx_EBI2;}
	if(tGptbEmCfg.byEpxInt == GPTB_ORL1){tGptbEmCfg.byOrl1 = GPTB_ORLx_EBI0 |GPTB_ORLx_EBI1|GPTB_ORLx_EBI2;}
	csi_gptb_emergency_cfg(GPTB0,&tGptbEmCfg);
	
    csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAX,GPTB_EMOUT_L);    //紧急状态下输出状态设置（注意mos/igbt的驱动电平）
	csi_gptb_emergency_pinxout(GPTB0,GPTB_EMCOAY,GPTB_EMOUT_L);
	
    csi_gptb_emint_en(GPTB0,GPTB_INTSRC_EP3);             //紧急状态输入中断使能
//------------------------------------------------------------------------------------------------------------------------	
//	csi_gptb_set_sync (GPTB0, GPTB_TRG_SYNCEN3, GPTB_TRG_CONTINU,GPTB_AUTO_REARM_ZRO);	
//	csi_gptb_set_evtrg (GPTB0, GPTB_TRGOUT0, GPTB_TRGSRC_EP0);    //EPx用trg0输出，GPTB_TRGSRC_EP0
//	csi_gptb_int_enable(GPTB0, GPTB_INT_TRGEV0,true);
//------------------------------------------------------------------------------------------------------------------------
//	csi_gptb_reglk_config_t  tReglk;                                                                    //
//	tReglk.byPrdr	= 0;                                                                                 //0x0  不链接
//	tReglk.byRssr    = 1;																                    //0x1  GPTB0
//	tReglk.byCmpa    = 1;																					//0x2  GPTA0
//	tReglk.byCmpb    = 1;																					//0x3  GPTA1
//	tReglk.byGld2    = 0;																					//0x4  GPTB0
//	tReglk.byEmslclr = 1;																					//0x5  GPTB1
//	tReglk.byEmhlclr = 1;																					//0x6  
//	tReglk.byEmicr   = 1;																					//0x7  
//	tReglk.byEmfrcr  = 1;																					//0x8  
//	tReglk.byAqosf   = 1;																					//0x9  
//	tReglk.byAqcsf   = 1;  																				//0xa  
//    csi_gptb_reglk_config(GPTB0,&tReglk);
//------------------------------------------------------------------------------------------------------------------------
    csi_gptb_start(GPTB0);//start  timer	
//------------------------------------------------------------------------------------------------------------------------	
    while(1)
	{			
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 80);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 80);
		mdelay(100);		
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPA, 40);
		csi_gptb_change_ch_duty(GPTB0,GPTB_COMPB, 40);
		mdelay(100);
	}			
	return iRet;
}

/** \brief gptb interrupt handle weak function
 *  \param[in] none
 *  \return    none
 */
__attribute__((weak)) void gptb_irqhandler(csp_gptb_t *ptGptbBase)
{  	
	volatile uint32_t wEMMisr = csp_gptb_get_emisr(ptGptbBase);
	volatile uint32_t wMisr   = csp_gptb_get_isr(ptGptbBase);	
	
	//GPTB emergency interrupt
	if(wEMMisr > 0)
	{
		if((wEMMisr & GPTB_EM_INT_EP0) == GPTB_EM_INT_EP0)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP0);
		}
		if((wEMMisr & GPTB_EM_INT_EP1) == GPTB_EM_INT_EP1)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP1);
		}
		if((wEMMisr & GPTB_EM_INT_EP2) == GPTB_EM_INT_EP2)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP2);
		}
		if((wEMMisr & GPTB_EM_INT_EP3) == GPTB_EM_INT_EP3)
		{
			csp_gptb_clr_emisr(ptGptbBase, GPTB_EM_INT_EP3);
		}	
		if(((csp_gptb_get_emisr(ptGptbBase) & GPTB_EM_INT_CPUF)) == GPTB_EM_INT_CPUF)
		{ 
			nop;		  
			ptGptbBase -> EMHLCLR |= GPTB_EM_INT_CPUF;
			csp_gptb_clr_emisr(ptGptbBase,GPTB_EM_INT_CPUF);	
		}
	}
	
	//GPTB interrupt
	if(wMisr > 0)
	{
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_TRGEV0))==GPTB_INT_TRGEV0)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV0);
		}	
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_TRGEV1))==GPTB_INT_TRGEV1)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV1);
		}	
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD0))==GPTB_INT_CAPLD0)
		{
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD0);	
		}
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD1))==GPTB_INT_CAPLD1)
		{
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD1);	
		}
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD2))==GPTB_INT_CAPLD2)
		{
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);	
			s_wGptbCapBuff[2]=csp_gptb_get_cmpaa(ptGptbBase);
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD2);	
		}
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD3))==GPTB_INT_CAPLD3)
		{
			s_wGptbCapBuff[0]=csp_gptb_get_cmpa(ptGptbBase);
			s_wGptbCapBuff[1]=csp_gptb_get_cmpb(ptGptbBase);	
			s_wGptbCapBuff[2]=csp_gptb_get_cmpaa(ptGptbBase);
			s_wGptbCapBuff[3]=csp_gptb_get_cmpba(ptGptbBase);	
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD3);	
		}
		if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_PEND))==GPTB_INT_PEND)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_PEND);
		}
		if((wMisr & GPTB_INT_CAU) == GPTB_INT_CAU)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAU);
		}
		if((wMisr & GPTB_INT_CAD) == GPTB_INT_CAD)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAD);
		}
		if((wMisr & GPTB_INT_CBU) == GPTB_INT_CBU)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBU);
		}
		if((wMisr & GPTB_INT_CBD) == GPTB_INT_CBD)
		{
			csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBD);
		}	
	}
}
