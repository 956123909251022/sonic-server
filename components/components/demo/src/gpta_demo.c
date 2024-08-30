/***********************************************************************//** 
 * \file  gpta_demo.c
 * \brief  TIMER_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-11 <td>V0.0 <td>ljy     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <gpta.h>
#include <drv/pin.h>
#include "drv/etb.h"

#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/


/** \brief GPTA捕获示例代码
 *   		- 捕获2次产生一次捕获中断
 *     		- 由PA01触发外部事件1，经过ETCB  触发sync2 捕获
 * 			- 信号由PA01的高低电平切换产生（一直高电平意味着没有触发）
 *  \param[in] none
 *  \return error code
 */

int gpta_capture_demo(void)
{
	int iRet = 0;	
    volatile uint8_t ch;

	csi_pin_set_mux(PA01,PA01_INPUT);		
	csi_pin_pull_mode(PA01, GPIO_PULLDOWN);						//PA01 上拉
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断	
	csi_exi_set_evtrg(EXI_TRGOUT1, TRGSRC_EXI1, 1);
//------------------------------------------------------------------------------------------------------------------------		
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体	
	tEtbConfig.byChType  = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp   = ETB_EXI_TRGOUT1 ;  	//...作为触发源
	tEtbConfig.byDstIp   =  ETB_GPTA0_SYNCIN2;  //GPTB0 同步输入2作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功						//ch < 0,则获取通道号失败		
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
//------------------------------------------------------------------------------------------------------------------------	
//------------------------------------------------------------------------------------------------------------------------	
//	csi_pin_set_mux(PB010,   PB010_GPTA0_CHA);//28
//	csi_pin_set_mux(PB011,   PB011_GPTA0_CHB);//29
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_captureconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = GPTA_CAPTURE;                     //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = GPTA_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT; 
	tPwmCfg.byStartSrc      = GPTA_SYNC_START;				    //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byCaptureCapmd   = 0;                               //0:连续捕捉模式    1h：一次性捕捉模式
	tPwmCfg.byCaptureStopWrap=4-1;                              //Capture模式下，捕获事件计数器周期设置值
	tPwmCfg.byCaptureLdaret   =0;                                //CMPA捕捉载入后，计数器值计数状态控制位(1h：CMPA触发后，计数器值进行重置;0h：CMPA触发后，计数器值不进行重置)
	tPwmCfg.byCaptureLdbret   =1; 
	tPwmCfg.byCaptureLdaaret  =0;  
	tPwmCfg.byCaptureLdbaret  =0;                                                          	
	tPwmCfg.wInt 		 =GPTA_INTSRC_CAPLD3;                   //interrupt//
		
	csi_gpta_capture_init(GPTA0, &tPwmCfg);

//------------------------------------------------------------------------------------------------------------------------
    csi_gpta_set_sync(GPTA0, GPTA_TRG_SYNCEN2, GPTA_TRG_CONTINU, GPTA_AUTO_REARM_ZRO);//使能SYNCIN2外部触发
//	csi_gpta_set_extsync_chnl(GPTA0, GPTA_TRG_SYNCEN2,0);                             //SYNCIN2--TRGSEL0
//	csi_gpta_set_evtrg(GPTA0, GPTA_TRG_OUT0, GPTA_TRG01_SYNC);                          //TRGSEL0	
//	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_TRGEV0,true);
//------------------------------------------------------------------------------------------------------------------------	
//	csi_gpta_filter_config_t  tpFiltercfg;
//	tpFiltercfg.byFiltSrc        =GPTA_FILT_SYNCIN2;
//	tpFiltercfg.byWinInv         =1;                              //0h：窗口不反转，窗口有效区间禁止滤波输入;  1h：窗口反转，  窗口有效区间使能滤波输入
//	tpFiltercfg.byWinAlign       =GPTA_ALIGN_ZRO;                  //窗口对齐模式选择															  
//	tpFiltercfg.byWinCross       =1;                              //滤波窗跨越窗口对齐点:  0h：禁止跨窗口对齐点;  1h：允许跨窗口对齐点
//	tpFiltercfg.byWinOffset      =0xffff/2;
//	tpFiltercfg.byWinWidth       =0xffff/2;
//    csi_gpta_set_sync_filter(GPTA0, &tpFiltercfg);
//------------------------------------------------------------------------------------------------------------------------	
	csi_gpta_start(GPTA0);//start  timer
    while(1){		
		  		      
		    mdelay(200);                        
		    	
		    mdelay(200);
	}			
	return iRet;
};

/** \brief GPTA波形输出示例代码
 *   		-10kHZ   输出波形
 *     		-PWMA在50%和20%之间切换
 * 			-
 *  \param[in] none
 *  \return error code
 */
int gpta_pwm_demo(void)
{
	int iRet = 0;	
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PB010,   PB010_GPTA0_CHA);//28
	csi_pin_set_mux(PB011,   PB011_GPTA0_CHB);//29
//	csi_pin_set_mux(PB04,   PB04_GPTA1_CHA);
//	csi_pin_set_mux(PA10,   PA10_GPTA1_CHB);
//------------------------------------------------------------------------------------------------------------------------	
//    csi_gpta_channel_cmpload_config(GPTA0, GPTA_CMPLD_SHDW, GPTA_LDCMP_ZRO ,GPTA_CAMPA);
//	csi_gpta_channel_cmpload_config(GPTA0, GPTA_CMPLD_SHDW, GPTA_LDCMP_ZRO ,GPTA_CAMPB);
	csi_gpta_pwmconfig_t tPwmCfg;								  
	tPwmCfg.byWorkmod        = GPTA_WAVE;                        //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTA_UPDNCNT;                     //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTA_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTA_SYNC_START;					 //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTA_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								 //pwm ouput duty cycle//PWM初始值(X%)			
	tPwmCfg.wFreq 			 = 10000;							 //pwm ouput frequency	
	tPwmCfg.wInt 		 	 = GPTA_INTSRC_CBU;                     //interrupt
	csi_gpta_wave_init(GPTA0, &tPwmCfg);
//------------------------------------------------------------------------------------------------------------------------
//    csi_gpta_channel_aqload_config(GPTA0, GPTA_LD_SHDW, GPTA_LDCMP_PRD ,GPTA_CHANNEL_1);//配置波形控制寄存器的载入模式：Immediate/Shadow  注意：在改变AQLDR寄存器时 会清除相应的AQCRx
//	csi_gpta_channel_aqload_config(GPTA0, GPTA_LD_SHDW, GPTA_LDCMP_PRD ,GPTA_CHANNEL_2);
	
	csi_gpta_pwmchannel_config_t  tEptchannelCfg;
	tEptchannelCfg.byActionZro    =   GPTA_LO;
	tEptchannelCfg.byActionPrd    =   GPTA_NA;
	tEptchannelCfg.byActionC1u    =   GPTA_HI;
	tEptchannelCfg.byActionC1d    =   GPTA_LO;
	tEptchannelCfg.byActionC2u    =   GPTA_HI;
	tEptchannelCfg.byActionC2d    =   GPTA_LO;
	tEptchannelCfg.byActionT1u    =   GPTA_LO;
	tEptchannelCfg.byActionT1d    =   GPTA_LO;
	tEptchannelCfg.byActionT2u    =   GPTA_NA;
	tEptchannelCfg.byActionT2d    =   GPTA_NA;
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPA;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_1);//channel
	tEptchannelCfg.byChoiceC1sel  =   GPTA_CMPB;
	tEptchannelCfg.byChoiceC2sel  =   GPTA_CMPB;
	csi_gpta_channel_config(GPTA0, &tEptchannelCfg,  GPTA_CHANNEL_2);
//------------------------------------------------------------------------------------------------------------------------

//    csi_gpta_Global_load_control_config_t  Globaldemo;
//	Globaldemo.bGlden    =   DISABLE;                   //全局的Shadow到Active寄存器载入控制使能
//	Globaldemo.byGldmd   =   GPTA_LDGLD_SW;//GPTA_LDGLD_PRD;//GPTA_LDGLD_ZRO;    //全局载入触发事件选择
//	Globaldemo.bOstmd    =   GPTA_LDMD_ANYTIME;                  //One Shot 载入模式使能控制位
//	Globaldemo.bGldprd   =   0 ;                                 //全局载入触发周期选择。可以选择N次触发条件满足后，才进行一次全局载入。   0;(立即触发)
//	Globaldemo.byGldcnt  =   0 ;                                 //事件计数器
//	csi_gpta_global_config(GPTA0,&Globaldemo);
//	
//	csi_gpta_gldcfg(GPTA0 ,bycmpa_A ,ENABLE);
//    csi_gpta_gldcfg(GPTA0 ,bycmpb_A ,ENABLE);
////	csi_gpta_global_rearm(GPTA0) ;                                //重置ONE SHOT模式。ONE SHOT模式下，一次触发后，需要重置模式才允许再次触发
////	csi_gpta_global_sw(GPTA0) ;                                   //软件产生一次GLD触发
//------------------------------------------------------------------------------------------------------------------------		

//	csi_gpta_set_evtrg(GPTA0, GPTA_TRG_OUT0, GPTA_TRG01_ZRO);      //TRGSEL0
//	csi_gpta_set_evtrg(GPTA0, GPTA_TRG_OUT1, GPTA_TRG01_PRD);      //TRGSEL1
//	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_TRGEV0 , ENABLE);
//	csi_gpta_int_enable(GPTA0, GPTA_INTSRC_TRGEV1 , ENABLE);	
//	csi_gpta_set_evcntinit(GPTA0, GPTA_TRG_OUT0, 5, 0);           //TRGSEL0每5次产生一次事件输出
	
//------------------------------------------------------------------------------------------------------------------------
//    csi_gpta_feglk_config_t  FEGLKcfg;                                                                    //
//	FEGLKcfg.byPrdr	   = 0;                                                                                 //0x0  不链接
//	FEGLKcfg.byRssr    = 1;																                    //0x1  EPT0
//	FEGLKcfg.byCmpa    = 1;																					//0x2  GPTA0
//	FEGLKcfg.byCmpb    = 1;																					//0x3  GPTA1
//	FEGLKcfg.byGld2    = 0;																					//0x4  GPTB0
//	FEGLKcfg.byEmslclr = 1;																					//0x5  GPTB1
//	FEGLKcfg.byEmhlclr = 1;																					//0x6 
//	FEGLKcfg.byEmicr   = 1;																					//0x7  
//	FEGLKcfg.byEmfrcr  = 1;																					//0x8  
//	FEGLKcfg.byAqosf   = 1;																					//0x9  
//	FEGLKcfg.byAqcsf   = 1;  																				//0xa  
//    csi_gpta_reglk_config(GPTA0,&FEGLKcfg);
//------------------------------------------------------------------------------------------------------------------------
	csi_gpta_start(GPTA0);
//------------------------------------------------------------------------------------------------------------------------
	 while(1){		
		  	csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 20);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 20);
//			csi_gpta_global_sw(GPTA0) ;                                   //软件产生一次GLD触发
//			csi_gpio_port_set_high(GPIOA0, (0x01ul << 0));
		    mdelay(100);

//          csi_gpta_onetimesoftware_output(GPTA0,GPTA_OSTSF1,GPTA_LO);

//			csi_gpio_port_set_low (GPIOA0, (0x01ul << 0));
//            mdelay(1);
//			
		    csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPA, 50);
	        csi_gpta_change_ch_duty(GPTA0,GPTA_CAMPB, 50);
//	        csi_gpta_global_sw(GPTA0) ;                                   //软件产生一次GLD触发
//			csi_gpio_port_set_low (GPIOA0, (0x01ul << 0));
		    mdelay(100);
//			csi_gpta_aqcsfload_config(GPTA0, GPTA_AQCSF_NOW);
//			csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_L);
//			mdelay(10);
//			csi_gpta_continuous_software_waveform(GPTA0, GPTA_CHANNEL_1, GPTA_AQCSF_NONE);
	}			
    return iRet;
}

void load1(void)
{   
	csi_gpta_channel_aqload_config(GPTA0, GPTA_LD_IMM, GPTA_LDCMP_PRD ,GPTA_CHANNEL_1);
	csi_gpta_channel_aqload_config(GPTA0, GPTA_LD_IMM, GPTA_LDCMP_PRD ,GPTA_CHANNEL_2);
	csi_gpio_port_set_high(GPIOA0, (0x01ul << 0));
	csi_gpta_pwmchannel_config_t  channel1;
	channel1.byActionZro    =   LO;
	channel1.byActionPrd    =   LO;
	channel1.byActionC1u    =   LO;
	channel1.byActionC1d    =   LO;
	channel1.byActionC2u    =   LO;
	channel1.byActionC2d    =   LO;
	channel1.byActionT1u    =   LO;
	channel1.byActionT1d    =   LO;
	channel1.byActionT2u    =   LO;
	channel1.byActionT2d    =   LO;
	channel1.byChoiceC1sel  =   GPTA_CMPA;
	channel1.byChoiceC2sel  =   GPTA_CMPA;	
	csi_gpta_channel_config(GPTA0, &channel1,  GPTA_CHANNEL_1);//channel
//	channel.byChoiceC1sel  =   EPT_CMPB;
//	channel.byChoiceC2sel  =   EPT_CMPB;
	csi_gpta_channel_config(GPTA0, &channel1,  GPTA_CHANNEL_2);

	csi_gpio_port_set_low (GPIOA0, (0x01ul << 0));
}




