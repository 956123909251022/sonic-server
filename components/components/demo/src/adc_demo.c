/***********************************************************************//** 
 * \file  adc_demo.c
 * \brief ADC_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-7 <td>V0.0  <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/adc.h>
#include <drv/pin.h>
#include <drv/bt.h>
#include <drv/dma.h>
#include <iostring.h>
#include "demo.h"

/* externs function--------------------------------------------------------*/

/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//ADC采样序列通道参数配置，默认情况，重复采样和平均系数为1(ADC采样值不平均)
//ADC触发根据实际应用进行配置
const csi_adc_seq_t tSeqCfg[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN1,		ADC_CV_COUNT_16,			ADC_AVG_COF_16,		ADCSYNC_NONE},
	{ADCIN2,		ADC_CV_COUNT_16,			ADC_AVG_COF_16,		ADCSYNC_NONE},
	{ADCIN4,		ADC_CV_COUNT_16,			ADC_AVG_COF_16,		ADCSYNC_NONE},
}; 

const csi_adc_seq_t tSeqTsCfg[] =
{
	//输入通道		//连续重复采样次数		//平均系数			//触发源选择
	{ADCIN_INTVREF,	ADC_CV_COUNT_128,		ADC_AVG_COF_128,	ADCSYNC_NONE},
};

//采样序列的通道数
static uint8_t 	s_byChnlNum = sizeof(tSeqCfg)/sizeof(tSeqCfg[0]);

static uint16_t hwDmaTranCnt = 0;					//DMA 传输次数
static uint32_t wDaBuf[3] = {0,0,0};				//ADC DMA 每次传输3通道数据缓存


/** \brief 单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动ADC
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_demo(void)
{
	int iRet = 0;
	uint8_t i;
	volatile int16_t nDataBuf[3] = {0,0,0};				//存放三通道采样值
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	//csi_pin_set_mux(PA00, PA00_ADC_AIN0);				//ADC GPIO作为输入通道
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
//	csi_pin_set_mux(PB00, PB00_ADC_AIN3);
	csi_pin_set_mux(PB01, PB01_ADC_AIN4);
//	csi_pin_set_mux(PB02, PB02_ADC_AIN5);
//	csi_pin_set_mux(PA06, PA06_ADC_AIN6);
//	csi_pin_set_mux(PA07, PA07_ADC_AIN7);
//	csi_pin_set_mux(PA08, PA08_ADC_AIN8);
//	csi_pin_set_mux(PA09, PA09_ADC_AIN9);
//	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
//	csi_pin_set_mux(PA012, PA012_ADC_AIN12);
//	csi_pin_set_mux(PA013, PA013_ADC_AIN13);
//	csi_pin_set_mux(PB03, PB03_ADC_AIN14);
//	csi_pin_set_mux(PC01, PC01_ADC_AIN15);
//	csi_pin_set_mux(PB04, PB04_ADC_AIN16);
//	csi_pin_set_mux(PC00, PC00_ADC_AIN17);
//	csi_pin_set_mux(PA02, PA02_ADC_AIN18);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;			//ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;					//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, s_byChnlNum);	//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		for(i = 0; i < s_byChnlNum; i++)
		{
			nDataBuf[i] = csi_adc_read_channel(ADC0, i);					//分别读ADC采样序列通道：0~s_byChnlNum
			
			if(nDataBuf[i]  < 0)
				my_printf("ADC sample channel timeout: d%\n",i);			//采样超时
			else
				my_printf("ADC channel value of seq: %d \n", nDataBuf[i]);	//串口打印采样值
		}
		
	}while(0);
	
	return iRet;
}

/** \brief 连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_demo(void)
{
	int iRet = 0;
	uint8_t i;
	volatile int16_t nDataBuf[3] = {0,0,0};				//存放三通道采样值
	csi_adc_config_t tAdcConfig;
	
		//adc 输入管脚配置
	//csi_pin_set_mux(PA00, PA00_ADC_AIN0);				//ADC GPIO作为输入通道
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
//	csi_pin_set_mux(PB00, PB00_ADC_AIN3);
	csi_pin_set_mux(PB01, PB01_ADC_AIN4);
//	csi_pin_set_mux(PB02, PB02_ADC_AIN5);
//	csi_pin_set_mux(PA06, PA06_ADC_AIN6);
//	csi_pin_set_mux(PA07, PA07_ADC_AIN7);
//	csi_pin_set_mux(PA08, PA08_ADC_AIN8);
//	csi_pin_set_mux(PA09, PA09_ADC_AIN9);
//	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
//	csi_pin_set_mux(PA012, PA012_ADC_AIN12);
//	csi_pin_set_mux(PA013, PA013_ADC_AIN13);
//	csi_pin_set_mux(PB03, PB03_ADC_AIN14);
//	csi_pin_set_mux(PC01, PC01_ADC_AIN15);
//	csi_pin_set_mux(PB04, PB04_ADC_AIN16);
//	csi_pin_set_mux(PC00, PC00_ADC_AIN17);
//	csi_pin_set_mux(PA02, PA02_ADC_AIN18);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;							//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;						//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;			//ADC 转换模式： 连续转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;				//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;					//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;		//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, s_byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC
	
	do
	{
		for(i = 0; i < s_byChnlNum; i++)
		{
			nDataBuf[i] = csi_adc_read_channel(ADC0, i);						//分别读ADC采样序列通道：0~s_byChnlNum
			
			if(nDataBuf[i]  < 0)
				my_printf("ADC sample channel timeout: d%\n",i);				//采样超时
			else
				my_printf("ADC channel value of seq: %d \n", nDataBuf[i]);		//串口打印采样值
		}
	
		csi_adc_stop(ADC0);														//停止转换
		mdelay(10);
		
		csi_adc_start(ADC0);													//启动ADC
		
		do
		{
			for(i = 0; i < s_byChnlNum; i++)
			{
				nDataBuf[i] = csi_adc_read_channel(ADC0, i);					//分别读ADC采样序列通道：0~s_byChnlNum
				
				if(nDataBuf[i]  < 0)
					my_printf("ADC sample channel timeout: d%\n",i);			//采样超时
				else
					my_printf("ADC channel value of seq: %d \n", nDataBuf[i]);	//串口打印采样值
			}
			
		}while(1);
	}while(0);
	
	return iRet;
}

/** \brief adc中断处理函数,用户使能ADC中断时，可在中断函数中读取采样序列中通道值
 * 
 *  \param[in] none
 *  \return error code
 */
__attribute__((weak))void adc_irqhandler(csp_adc_t *ptAdcBase)
{
	uint8_t i;
 	volatile uint16_t hwDataBuf[3];
	
	uint32_t wIntStat = csp_adc_get_sr(ptAdcBase) & csp_adc_get_imr(ptAdcBase);
	
	for(i = 0; i < s_byChnlNum; i++)						
	{
		if(wIntStat & ADC12_SEQ(i))								//ADC采样序列状态
		{
			hwDataBuf[i] = csp_adc_get_data(ptAdcBase, i);		//读取采样序列中ADC通道值
			csp_adc_clr_sr(ptAdcBase, ADC12_SEQ(i));			//清除状态
			my_printf("ADC channel value of seq: %d \n", hwDataBuf[i]);
		}
	}
}
/** \brief  中断单次转换模式：ADC启动后进行整个序列采样直到结束，中间不能停止；进行下一次序列转换需重新启动ADC
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_oneshot_int_demo(void)
{
	int iRet = 0;
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	//csi_pin_set_mux(PA00, PA00_ADC_AIN0);				//ADC GPIO作为输入通道
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
//	csi_pin_set_mux(PB00, PB00_ADC_AIN3);
	csi_pin_set_mux(PB01, PB01_ADC_AIN4);
//	csi_pin_set_mux(PB02, PB02_ADC_AIN5);
//	csi_pin_set_mux(PA06, PA06_ADC_AIN6);
//	csi_pin_set_mux(PA07, PA07_ADC_AIN7);
//	csi_pin_set_mux(PA08, PA08_ADC_AIN8);
//	csi_pin_set_mux(PA09, PA09_ADC_AIN9);
//	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
//	csi_pin_set_mux(PA012, PA012_ADC_AIN12);
//	csi_pin_set_mux(PA013, PA013_ADC_AIN13);
//	csi_pin_set_mux(PB03, PB03_ADC_AIN14);
//	csi_pin_set_mux(PC01, PC01_ADC_AIN15);
//	csi_pin_set_mux(PB04, PB04_ADC_AIN16);
//	csi_pin_set_mux(PC00, PC00_ADC_AIN17);
//	csi_pin_set_mux(PA02, PA02_ADC_AIN18);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 8;									//ADC clk两分频：clk = pclk/8
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;					//ADC 转换模式： 单次转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_SEQ0 | ADC_INTSRC_SEQ1 |
				ADC_INTSRC_SEQ2;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, s_byChnlNum);		//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC

	return iRet;
}
/** \brief 中断连续转换模式：ADC启动后进行整个序列采样，序列采样完成后，继续从序列第一个通道开始，如此循环。采样期间可以停止
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_int_demo(void)
{
	int iRet = 0;
	csi_adc_config_t tAdcConfig;
	
	//adc 输入管脚配置
	//csi_pin_set_mux(PA00, PA00_ADC_AIN0);				//ADC GPIO作为输入通道
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
//	csi_pin_set_mux(PB00, PB00_ADC_AIN3);
	csi_pin_set_mux(PB01, PB01_ADC_AIN4);
//	csi_pin_set_mux(PB02, PB02_ADC_AIN5);
//	csi_pin_set_mux(PA06, PA06_ADC_AIN6);
//	csi_pin_set_mux(PA07, PA07_ADC_AIN7);
//	csi_pin_set_mux(PA08, PA08_ADC_AIN8);
//	csi_pin_set_mux(PA09, PA09_ADC_AIN9);
//	csi_pin_set_mux(PA010, PA010_ADC_AIN10);
//	csi_pin_set_mux(PA011, PA011_ADC_AIN11);
//	csi_pin_set_mux(PA012, PA012_ADC_AIN12);
//	csi_pin_set_mux(PA013, PA013_ADC_AIN13);
//	csi_pin_set_mux(PB03, PB03_ADC_AIN14);
//	csi_pin_set_mux(PC01, PC01_ADC_AIN15);
//	csi_pin_set_mux(PB04, PB04_ADC_AIN16);
//	csi_pin_set_mux(PC00, PC00_ADC_AIN17);
//	csi_pin_set_mux(PA02, PA02_ADC_AIN18);
	
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 8;									//ADC clk两分频：clk = pclk/8
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_SEQ0 | ADC_INTSRC_SEQ1 |
				ADC_INTSRC_SEQ2;								//ADC 中断配置：SEQ_END(0~2)中断使能，默认(推荐使用)SEQ_END中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, s_byChnlNum);	//配置ADC采样序列
	csi_adc_start(ADC0);										//启动ADC

	return iRet;
}
/** \brief ADC 连续采样模式，采样3通道数据，通过DMA传输已采集的3通道数据
 *  \brief BT定时，每20ms，ADC传输一次采样数据
 * 
 *  \param[in] none
 *  \return error code
 */
int adc_samp_continuous_dma_transfer_demo(void)
{
	int ret = 0;
	uint8_t i;
	uint16_t hwTemp[3];
 	volatile uint16_t hwValBuf[1200];
	
	csi_dma_ch_config_t tDmaConfig;				
	csi_etb_config_t 	tEtbConfig;	
	csi_adc_config_t 	tAdcConfig;
	
	memset((uint8_t *)hwValBuf, 0, 2400);						//清除数据缓存					
	
	//adc 输入管脚配置
	//csi_pin_set_mux(PA00, PA00_ADC_AIN0);						//ADC GPIO作为输入通道
	csi_pin_set_mux(PA01, PA01_ADC_AIN1);
	csi_pin_set_mux(PA03, PA03_ADC_AIN2);
	csi_pin_set_mux(PB01, PB01_ADC_AIN4);

	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 0x02;									//ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x06;								//ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_CONTINU;					//ADC 转换模式： 连续转换
	tAdcConfig.byVrefSrc = ADCVERF_VDD_VSS;						//ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;							//ADC 中断配置： 无中断
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqCfg;				//ADC 采样序列： 具体参考结构体变量 tSeqCfg
	
	csi_adc_init(ADC0, &tAdcConfig);							//初始化ADC参数配置	
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, s_byChnlNum);	//配置ADC采样序列
	
	csi_adc_set_evtrg(ADC0, ADC_TRGOUT0,ADC_TRGSRC_EOC);		//配置ADC EOC作为触发源在触发端口0输出
	csi_adc_evtrg_enable(ADC0,ADC_TRGOUT0,DISABLE);				//禁止ADC触发通道0
	
	//dma 配置
	tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_32_BITS;		//传输数据宽度32bit，ADC_DRx[]为32位，地址递增1次为一个word
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，软件请求（软件触发）
	tDmaConfig.wInt			= DMA_INTSRC_TCIT;			//使用TCIT中断
	
	//etb 配置
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	= ETB_ADC_TRGOUT0;				//ADC触发端口0作为输出触发源
	tEtbConfig.byDstIp 	= ETB_DMA_CH0;					//ETB DMA通道0 作为触发目标
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	
	csi_etb_init();										//使能ETB模块
	ret = csi_etb_ch_config(ETB_CH8, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL = ETB_CH8~ETB_CH11
	if(ret < CSI_OK)
		return CSI_ERROR;
	
	ret = csi_dma_ch_init(DMA, DMA_CH0, &tDmaConfig);	//初始化DMA通道0
	csi_dma_ch_start(DMA, DMA_CH0, (void *)&(ADC0->DR[0]), (void *)wDaBuf, 1, 3);	//配置DMA传输源、目标地址和传输数量，并使能DMA通道0
	
	//BT定时
	csi_bt_timer_init(BT0, 20000);						//初始化BT0, 定时20000us(20ms)
	
	csi_adc_start(ADC0);								//启动ADC
	csi_bt_start(BT0);									//启动定时器
	
	while(1)
	{
		if(csi_dma_get_msg(DMA_CH0, ENABLE))			//获取发送完成消息，并清除消息
		{
			hwDmaTranCnt ++;
			
			if(hwDmaTranCnt > 400)						//传输400次到
			{
				csi_bt_stop(BT0);						//停止BT0
				csi_adc_stop(ADC0);						//停止ADC
			}
			else
			{
				for(i = 0; i < 3 ;i ++)
				{
					hwTemp[i] = wDaBuf[i];				//取16位的ADC采样数据		
				}
				//将400次(1200 halfword)传输数据拷贝到缓存hwValBuf中(用户可自行增加次数，考虑此处处理数据buf大小问题)
				memcpy(((uint8_t *)hwValBuf+6*(hwDmaTranCnt-1)), (uint8_t*)hwTemp, 6);
				
				csi_adc_evtrg_enable(ADC0,ADC_TRGOUT0,DISABLE);				//禁止ADC触发通道0
				//配置DMA传输源、目标地址和传输数量，并使能DMA通道0
				csi_dma_ch_start(DMA, DMA_CH0, (void *)&(ADC0->DR[0]), (void *)wDaBuf, 1, 3);
			}
		}
	}
	
	return 0;
}
/** \brief bt interrupt handle function
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
void bt_irqhandler(csp_bt_t *ptBtBase)
{
	// ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
		csi_adc_evtrg_enable(ADC0,ADC_TRGOUT0,ENABLE);					//使能ADC触发通道0,触发DMA传输，20ms传输一次
		csi_pin_toggle(PA06);
	}
}


/** \brief 使用内部温度传感器的ADC配置
 *  \brief 需要把任意一个通道配置成ADCIN_INTVREF
 * 
 *  \param[in] none
 *  \return none
 */
void adc_ts_init_demo(void)
{
	csi_adc_config_t tAdcConfig;
	uint8_t byNum = 1;
	//STEP 1: ADC初始化
	//adc 输入管脚配置
	csi_pin_set_mux(PA00, PA00_AVREF);                                      //GPIO的AF功能设置为VREF+                             
	//adc 参数配置初始化
	tAdcConfig.byClkDiv = 48;									                  //ADC clk两分频：clk = pclk/2
	tAdcConfig.bySampHold = 0x10;								              //ADC 采样时间： time = 16 + 6 = 22(ADC clk周期)
	tAdcConfig.byConvMode = ADC_CONV_ONESHOT;				  //ADC 转换模式： 单次转换；
	tAdcConfig.byVrefSrc = ADCVREF_FVR4096_VSS;					  //ADC 参考电压： 系统VDD
	tAdcConfig.wInt = ADC_INTSRC_NONE;                                 //中断设置
	tAdcConfig.ptSeqCfg = (csi_adc_seq_t *)tSeqTsCfg;	             //ADC 采样序列： 具体参考结构体变量 SeqCfg3
	csi_adc_init(ADC0, &tAdcConfig);							                  //初始化ADC参数配置
	csi_adc_set_seqx(ADC0, tAdcConfig.ptSeqCfg, byNum);	 	  //配置ADC采样序列	
}

/** \brief  获取当前温度传感器温度值
 *  \brief   计算步骤： 
 *                    STEP1: ADC输入源选择INTVREF，且ADCC_CR[INTVREF_SEL] = 0x3时，采样到的值为ADC_TS
 *                    STEP2: ADC输入源选择INTVREF，且ADCC_CR[INTVREF_SEL] = 0x2时，采样到的值为ADC_1V    
 *			           STEP3: 读取芯片内部标定值
 *                    STEP4: 公式计算
 * 
 *  \brief   标定值说明：
 *                     标定值(4Bytes)已经保存在对应的地址中，在adc.h文件中已用宏定义表示
 *                            名称        地址                说明                                                     adc.h中的名字
 *                            TS1         0x80140         常温下温度传感器信息                      CALIBRATION_TS1
 *                            IR1          0x80144         常温下内部1V电压参考信息             CALIBRATION_IR1
 *                            TS2         0x80180         高温下温度传感器信息                      CALIBRATION_TS2
 *                            IR2          0x80184         高温下内部1V电压参考信息              CALIBRATION_IR2
 *                    每个标定值的对应字段说明如下
 *                            [31:28]FLAG + [27:16]TEMP + [15:12]rsvd + [11:0]DATA
 *                                                       |
 *                                            [27]为TEMP的符号位，0表示正温度，1表示负温度  
 *                    举例:  
 *                          若  TS1=  0x5d800673 
 *                          则  TS1_FLAG   = 0x5        有效数据
 *                                TS1_TEMP  = 0xd80   此温度值为负温度，此值为补码。计算时需要将其还原成原码
 *                                TS1_DATA  = 0x673   温度传感器的电压采样值
 * 
 *  \brief   公式如下：
 *              
 *                                                                  (TS2_TEMP - TS1_TEMP) * 0.0625           TS2_DATA            ADC_TS
 *              Tx = TS2_TEMP * 0.0625 - ————————————————— * (—————— - ——————)
 *                                                                      TS2_DATA             TS1_DATA                IR2_DATA            ADC_1V
 *                                                                   ——————  -  ———————
 *                                                                       IR2_DATA              IR1_DATA
 *              其中：xxx_TEMP的获取
 *                                如果是正温度：
 *                                                          TS1_TEMP  =   (float) (TS1 & 0x0fff0000) >> 16;
 *                                                          TS2_TEMP  =   (float) (TS2 & 0x0fff0000) >> 16;
 *                                如果是负温度：
 *                                                          TS1_TEMP  =   (float) (0 - ((((uint16_t)((TS1 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
 *                                                          TS2_TEMP  =   (float) (0 - ((((uint16_t)((TS2 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
 *                         xxx_DATA的获取
 *                                                          TS1_DATA = (float) (TS1 & 0x00000fff);
 *                                                          TS2_DATA = (float) (TS2 & 0x00000fff);
 *                                                          IR1_DATA  = (float) (IR1 & 0x00000fff);
 *                                                          IR2_DATA  = (float) (IR2 & 0x00000fff);
 * 
 * 
 *  \param[in] none
 *  \return temperature value
 */
float adc_ts_gettemperature_demo(void)
{
	int16_t hwADC_TS;
	int16_t hwADC_1V;
	int16_t hw_chipTemp= 0;
	
	float fTS1_DATA, fTS1_TEMP, fTS2_DATA, fTS2_TEMP;
	float fIR1_DATA, fIR2_DATA;
	float fADC_TS, fADC_1V;
	float fTEMP_X = 0;
	
	uint8_t valid1,valid2,valid3,valid4;
	
	//STEP1: ADC输入源选择INTVREF，且ADCC_CR[INTVREF_SEL] = 0x3时，采样到的值为hwADC_TS
	ADC0->CR |= (0x03 << 17);                                                      //内部温度传感器采集
	csi_adc_start(ADC0);										                           //启动ADC	
	while(!(ADC0->SR & ADC12_SEQ(0)));
	hwADC_TS = csi_adc_read_channel(ADC0, 0);
		
	//STEP2: ADC输入源选择INTVREF，且ADCC_CR[INTVREF_SEL] = 0x2时，采样到的值为hwADC_1V
	ADC0->CR = (ADC0->CR & (~(0x03 << 17)) ) | (0x02 << 17);//切换到内部1.0v
	csi_adc_start(ADC0);										                            //启动ADC	
	while(!(ADC0->SR & ADC12_SEQ(0)));
	hwADC_1V = csi_adc_read_channel(ADC0, 0);
	
	//STEP3: 读取芯片内部标定值
	//已经用宏定义表示CALIBRATION_TS1 CALIBRATION_IR1 CALIBRATION_TS2 CALIBRATION_IR2
	valid1 = (CALIBRATION_TS1& 0xf0000000) >> 28;
	valid2 = (CALIBRATION_IR1& 0xf0000000) >> 28;
	valid3 = (CALIBRATION_TS2 & 0xf0000000) >> 28;
	valid4 = (CALIBRATION_IR2 & 0xf0000000) >> 28;
	
	//STEP4: 公式计算
	//当前温度(℃)= xxx_TEMP * 0.0625
	if( (0x5 == valid1) && (0x5 == valid2) && (0x5 == valid3) && (0x5 == valid4) )
	{
		//温度
		if((CALIBRATION_TS1 & 0x08000000) == 0x08000000) //负数
		{
			fTS1_TEMP = (float)(0 - ((((uint16_t)((CALIBRATION_TS1 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
		}
		else//正数
		{
			fTS1_TEMP = (float)((CALIBRATION_TS1 & 0x0fff0000) >> 16 );
		}
		
		if((CALIBRATION_TS2 & 0x08000000) == 0x08000000) //负数
		{
			fTS2_TEMP = (float)(0 - ((((uint16_t)((CALIBRATION_TS2 & 0x0fff0000) >> 16) - 1) ^ 0x7FF) & 0x7FF));
		}
		else//正数
		{
			fTS2_TEMP = (float)((CALIBRATION_TS2 & 0x0fff0000) >> 16 );
		}
		
		fTS1_TEMP = fTS1_TEMP * 0.0625;
		fTS2_TEMP = fTS2_TEMP * 0.0625;
		
		//ts
		fTS1_DATA = (float)(CALIBRATION_TS1 & 0x00000fff);
		fTS2_DATA = (float)(CALIBRATION_TS2 & 0x00000fff);
		fADC_TS = (float)(hwADC_TS & 0x0fff);
		
		//ir
		fIR1_DATA = (float)(CALIBRATION_IR1 & 0x00000fff);
		fIR2_DATA = (float)(CALIBRATION_IR2 & 0x00000fff);
		fADC_1V = (float)(hwADC_1V & 0x00000fff);
		
		fTEMP_X = fTS2_TEMP - (fTS2_TEMP - fTS1_TEMP) / (fTS2_DATA/fIR2_DATA - fTS1_DATA/fIR1_DATA ) * (fTS2_DATA/fIR2_DATA - fADC_TS/fADC_1V);
		hw_chipTemp = (int16_t)(fTEMP_X * 100);
		my_printf("110_temp = %d  |  hwADC_TS = %d  |  hwADC_1V = %d  \r\n", hw_chipTemp, hwADC_TS, hwADC_1V);
	}
	else 
	{
		my_printf("\r\nCalibration data is invalid !!!\r\n");
	}
	
	return fTEMP_X;
	
}