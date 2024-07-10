/***********************************************************************//** 
 * \file  iwdt_demo.c
 * \brief  IWDT_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY    <td>initial
 * <tr><td> 2021-7-28 <td>V0.1 <td>YYM    <td>modify
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "drv/etb.h"
#include "drv/pin.h"
#include "drv/bt.h"
#include "drv/lpt.h"
#include "drv/ept.h"
#include "drv/adc.h"
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/*
 * 多个一对一触发模式
 *EXI1 		 --> BT_SYNC0 
 *BT0_TRGOUT --> LPT_SYNC0 
*/
int etcb_one_trg_one_demo0(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				                //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA01,PA01_INPUT);		
	csi_pin_pull_mode(PA01, GPIO_PULLUP);						//PA01 上拉
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断	
	csi_exi_set_evtrg(1, TRGSRC_EXI1, 1);
	
	csi_bt_start_sync(BT0, 10);
	csi_bt_set_sync(BT0,BT_TRG_SYNCIN0, BT_TRG_ONCE, ENABLE);  
	csi_bt_set_evtrg(BT0, 0, BT_TRGSRC_PEND);
	
	csi_lpt_start_sync(LPT,LPT_CLK_ISCLK,200);
	csi_lpt_set_sync(LPT, LPT_TRG_SYNCIN0, LPT_SYNC_ONCE, ENABLE);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ;  	    //BT0 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN0;   	    //LPT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
	return iRet;
}

/*
 * 一对一触发模式
 *EPT --> ADC_SYNC0 
*/
uint8_t  byAdcChnlNum;                                          //采样序列的通道数
const csi_adc_seq_t SeqCfg1[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN1,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN2,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN3,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN10,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN11,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
	{ADCIN12,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
}; 

//中断回调函数，使用ETCB模式，触发ADC采集的过程中，将中断处理函数apt_adc_irqhandler替换成apt_etcb_adc_irqhandler
int16_t hwAdc_Data[6] = {0};
void apt_etcb_adc_irqhandler(csp_adc_t *ptAdcBase)
{
	uint32_t wIntStat = csp_adc_get_sr(ptAdcBase) & csp_adc_get_isr(ptAdcBase);
	
	csi_pin_toggle(PA05);
	
	//ADC EOC interrupt
	if(wIntStat&ADC12_EOC)
	{
		for(uint8_t adi = 0;adi < byAdcChnlNum;adi ++)
		{
			hwAdc_Data[adi] = csi_adc_read_channel(ADC0, adi);
		}
		csp_adc_clr_sr(ptAdcBase, ADC12_EOC);	
	}
}

void etcb_adc_config(void)
{
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
	csi_pin_set_mux(PB00, PB00_ADC_AIN3);
	
	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
	csi_pin_set_mux(PA012, PA012_ADC_AIN12);

	byAdcChnlNum = 6;                                           //通道数配置为6路                               
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_EOC;                         //打开EOC中断

	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg1;		    //ADC 采样序列： 具体参考结构体变量 SeqCfg1
	
	csi_adc_init(ADC0, &tAdcConfig);							 //初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byAdcChnlNum);	 //配置ADC采样序列
	csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN0, ADC_TRG_CONTINU, 0);     //选择ADC_SYNCEN0同步事件
	csi_adc_start(ADC0);										 //启动ADC	
}

void etcb_ept_config(void)
{
//------------------------------------------------------------------------------------------------------------------------	
	csi_pin_set_mux(PA13, PA13_EPT_CHAX);						//PIN17
	csi_pin_set_mux(PA14, PA14_EPT_CHBX);						//PIN18
	csi_pin_set_mux(PA15, PA15_EPT_CHCX);						//PIN19
	
	csi_pin_set_mux(PA10, PA10_EPT_CHAY);						//PIN14	
	csi_pin_set_mux(PA11, PA11_EPT_CHBY);						//PIN15
	csi_pin_set_mux(PA12, PA12_EPT_CHCY);						//PIN16
//------------------------------------------------------------------------------------------------------------------------	
	csi_ept_config_t tPwmCfg;								  
	tPwmCfg.byWorkmod       = EPT_WAVE;                        //WAVE or CAPTURE    //计数或捕获	
	tPwmCfg.byCountingMode  = EPT_UPCNT;                       //CNYMD  //计数方向
	tPwmCfg.byOneshotMode   = EPT_OP_CONT;                     //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc      = EPT_SW_START;				       //软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld         = EPT_LDPSCR_ZRO;                  //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值	
	tPwmCfg.byDutyCycle 	= 50;							   //pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			= 1000;						       //pwm ouput frequency			
	tPwmCfg.wInt 			= EPT_INTSRC_TRGEV0;                  //interrupt
	csi_ept_config_init(EPT0, &tPwmCfg);
	
	csi_ept_pwmchannel_config_t  channel;
	channel.byActionZro    =   LO;
	channel.byActionPrd    =   LO;
	channel.byActionC1u    =   HI;
	channel.byActionC1d    =   HI;
	channel.byActionC2u    =   NA;
	channel.byActionC2d    =   NA;
	channel.byActionT1u    =   LO;
	channel.byActionT1d    =   LO;
	channel.byActionT2u    =   NA;
	channel.byActionT2d    =   NA;
	channel.byChoiceC1sel  =   EPT_CMPA;
	channel.byChoiceC2sel  =   EPT_CMPA;	
	csi_ept_channel_config(EPT0, &channel,  EPT_CHANNEL_1);
	csi_ept_channel_config(EPT0, &channel,  EPT_CHANNEL_2);
	csi_ept_channel_config(EPT0, &channel,  EPT_CHANNEL_3);	
	
	csi_ept_set_evtrg(EPT0, EPT_TRGOUT0, EPT_TRGSRC_PRD);         //选择EPT_TRG_OUT0触发事件
	csi_ept_evtrg_enable(EPT0, 0, ENABLE);
	csi_ept_start(EPT0);	
}

int etcb_one_trg_one_demo1(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体		
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);	
	etcb_ept_config();
	etcb_adc_config();

	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  	//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_ETP0_TRGOUT0 ;  	//EPT0 触发输出作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_ADC_SYNCIN0;   	//ADC_SYNCIN0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/*
 * 一对多触发模式
 *EPT -->ADC_SYNC0 ADC_SYNC1
*/
const csi_adc_seq_t SeqCfg2[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN1,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN2,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN1},
}; 

void etcb_adc_config12(void)
{
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);

	byAdcChnlNum = 2;                                
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_EOC;                         //打开EOC中断

	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg2;		    //ADC 采样序列： 具体参考结构体变量 SeqCfg2
	
	csi_adc_init(ADC0, &tAdcConfig);							 //初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byAdcChnlNum);	 //配置ADC采样序列
	csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN0, ADC_TRG_CONTINU, 0);     //选择ADC_SYNCEN0同步事件
	csi_adc_start(ADC0);										 //启动ADC	
}

int etcb_one_trg_more_demo(void)
{
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				//ETB 参数配置结构体		
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);	
	
	etcb_ept_config();
	etcb_adc_config12();

	tEtbConfig.byChType = ETB_ONE_TRG_MORE;  	//单个源触发多个目标
	tEtbConfig.bySrcIp  = ETB_ETP0_TRGOUT0 ;  	//EPT0 触发输出作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_ADC_SYNCIN0;   	//ADC_SYNCIN0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = ETB_ADC_SYNCIN1;      //ADC_SYNCIN1 同步输入作为目标事件
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	//自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								//ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/*
 * 多对一触发模式
 *EXI0 EXI1 EXI3--> ADC_SYNC0 
*/
const csi_adc_seq_t SeqCfg3[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN10,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},
	{ADCIN11,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
	{ADCIN12,		ADC_CV_COUNT_1,			ADC_AVG_COF_1,		ADCSYNC_IN0},	
};

void etcb_adc_config13(void)
{
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
	csi_pin_set_mux(PA012, PA012_ADC_AIN12);

	byAdcChnlNum = 3;                                           //通道数配置为3路                               
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 2;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_EOC;                         //打开EOC中断

	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)SeqCfg3;		        //ADC 采样序列： 具体参考结构体变量 SeqCfg3
	
	csi_adc_init(ADC0, &tAdcConfig);							 //初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byAdcChnlNum);	 //配置ADC采样序列
	csi_adc_set_sync(ADC0, ADC_TRG_SYNCEN0, ADC_TRG_CONTINU, 50);     //选择ADC_SYNCEN0同步事件
	csi_adc_start(ADC0);										 //启动ADC	
}

int etcb_more_trg_one_demo(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);	
	
	csi_pin_set_mux(PA00,PA00_INPUT);	
	csi_pin_set_mux(PA01,PA01_INPUT);	
	csi_pin_set_mux(PA03,PA03_INPUT);	
	
	csi_pin_pull_mode(PA00, GPIO_PULLNONE);						//PA00 无上下拉
	csi_pin_pull_mode(PA01, GPIO_PULLUP);						//PA01 上拉
	csi_pin_pull_mode(PA03, GPIO_PULLUP);						//PA03 上拉
	
	csi_pin_irq_mode(PA00,EXI_GRP0, GPIO_IRQ_RISING_EDGE);		//PA00 上升沿产生中断	
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断		
	csi_pin_irq_mode(PA03,EXI_GRP3, GPIO_IRQ_FALLING_EDGE);		//PA03 下降沿产生中断	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI0, 4);                       //PA00 4次上升沿触发目标事件     
	csi_exi_set_evtrg(2, TRGSRC_EXI1, 1);
	csi_exi_set_evtrg(3, TRGSRC_EXI3, 1);

	etcb_adc_config13();
	
	tEtbConfig.byChType = ETB_MORE_TRG_ONE;  		//多个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_EXI_TRGOUT2;      
	tEtbConfig.bySrcIp2 = ETB_EXI_TRGOUT3;
	tEtbConfig.byDstIp =  ETB_ADC_SYNCIN0;   	    //ADC0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	return iRet;
}

/*
 * 多对一/一对一混合触发模式
 *EXI0 EXI1 EXI3--> BT_SYNC0 
 *BT0_TRGOUT    --> LPT_SYNC0 
 *LPT_TRGOUT    --> ADC_SYNC0 
*/
int etcb_mix_demo(void)
{	
	int iRet = 0;
	volatile uint8_t ch;
	csi_etb_config_t tEtbConfig;				               //ETB 参数配置结构体		
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);	
	
	csi_pin_set_mux(PA00,PA00_INPUT);	
	csi_pin_set_mux(PA01,PA01_INPUT);	
	csi_pin_set_mux(PA03,PA03_INPUT);	
	
	csi_pin_pull_mode(PA00, GPIO_PULLNONE);						//PA00 无上下拉
	csi_pin_pull_mode(PA01, GPIO_PULLUP);						//PA01 上拉
	csi_pin_pull_mode(PA03, GPIO_PULLUP);						//PA03 上拉
	
	csi_pin_irq_mode(PA00,EXI_GRP0, GPIO_IRQ_RISING_EDGE);		//PA00 上升沿产生中断	
	csi_pin_irq_mode(PA01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PA01 下降沿产生中断		
	csi_pin_irq_mode(PA03,EXI_GRP3, GPIO_IRQ_FALLING_EDGE);		//PA03 下降沿产生中断	
	
	csi_exi_set_evtrg(1, TRGSRC_EXI0, 4);                       //PA00 4次上升沿触发目标事件     
	csi_exi_set_evtrg(2, TRGSRC_EXI1, 1);
	csi_exi_set_evtrg(3, TRGSRC_EXI3, 1);

	csi_bt_start_sync(BT0, 200);
	csi_bt_set_sync(BT0,BT_TRG_SYNCIN0, BT_TRG_ONCE, ENABLE);  
	
	csi_bt_set_evtrg(BT0, 0, BT_TRGSRC_PEND);
		
	csi_lpt_start_sync(LPT,LPT_CLK_ISCLK,100);
	csi_lpt_set_sync(LPT, 0, LPT_SYNC_ONCE, ENABLE);
	csi_lpt_set_evtrg(LPT, 0, LPT_TRGSRC_PRD, 1);
	
	etcb_adc_config13();
	
	tEtbConfig.byChType = ETB_MORE_TRG_ONE;  		//多个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_EXI_TRGOUT1 ;  	    //EXI1 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = ETB_EXI_TRGOUT2;      
	tEtbConfig.bySrcIp2 = ETB_EXI_TRGOUT3;
	tEtbConfig.byDstIp =  ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ;  	    //BT0 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_LPT_SYNCIN0;   	    //LPT 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_LPT_TRGOUT0 ;  	    //LPT 触发输出0作为触发源
	tEtbConfig.bySrcIp1 = 0xff;      
	tEtbConfig.bySrcIp2 = 0xff;
	tEtbConfig.byDstIp =  ETB_ADC_SYNCIN0;   	    //ADC0 同步输入作为目标事件
	tEtbConfig.byDstIp1 = 0xff;
	tEtbConfig.byDstIp2 = 0xff;
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
		
	return iRet;
}
