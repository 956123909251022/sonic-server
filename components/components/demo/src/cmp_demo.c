/***********************************************************************//** 
 * \file  cmp_demo.c
 * \brief  CMP_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-11-25 <td>V0.0 <td>YYM     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <cmp.h>
#include <pin.h>
#include "drv/etb.h"
#include <drv/lpt.h>
#include <drv/bt.h>
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief 比较器基本功能测试demo
 *  \brief N-和P+输入不同的电平值，如果P+大于N-，将输出高电平，如果P+小于N-，将输出低电平
 * 
 *  \param[in] none
 *  \return error code
 */
int cmp_base_demo(void)
{
	int iRet = 0;
	csi_cmp_config_t tCmpCfg;
	
	csi_pin_set_mux(PA02, PA02_OUTPUT);	  

	csi_pin_set_mux(PA00,PA00_CMP_IN0);	
	csi_pin_set_mux(PA110,PA110_CMP_IN1);		
	csi_pin_set_mux(PA111,PA111_CMP0_OUT);	
	
	tCmpCfg.byNsel = CMP_N_SEL_CP0;                   //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_0mv;           //比较器输入迟滞
	tCmpCfg.byPhystsel = CMP_PHYST_POL_DIS;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;          //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel  = CMP_CPOS_OUT_IN;	          //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_EDGEDET;	      	  //中断模式
	csi_cmp_init(CMP0,&tCmpCfg);
	csi_cmp_start(CMP0);
	return iRet;	
}


/** \brief 比较器滤波功能测试demo
 *  \brief 滤波器1是一个积分滤波器，当滤波器采样到‘1’时，积分计数器加1，反之，采样到‘0’时，积分计数器减1，
 *  \brief 当积分计数器值≥CMP_DFCR1[DEPTH](滤波时间周期)时，输出高；当积分计数器值= 0时，输出低。
 *  \brief 如果PCLK时钟配置为24M，滤波时钟周期为： 24M/(119+1)/2^2 => 20us
 *  \brief 滤波积分的时间周期为20*CMP_DFCR_DEPTH1_16 = 320us。
 *  
 *  \brief 滤波器2是一个去抖滤波器，只有当采样到的电平连续CMP_DFCR2[DEPTH](去抖时间周期)次保持一致，滤波器才会改变输出。
 *  \brief 如果PCLK时钟配置为24M，滤波时钟周期为： 24M/(119+1)/2^2 => 20us
 *  \brief 滤波去抖的时间周期为20*CMP_DFCR_DEPTH2_16 = 320us。
 *   
 *  \param[in] none
 *  \return error code
 */
int cmp_dfcr_demo(void)
{
	int iRet = 0;
	
	csi_pin_set_mux(PA00,PA00_CMP_IN0);	
	csi_pin_set_mux(PA110,PA110_CMP_IN1);		
	csi_pin_set_mux(PA111,PA111_CMP0_OUT);	
	
	csi_cmp_config_t tCmpCfg;
	tCmpCfg.byNsel = CMP_N_SEL_CP0;                    //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_10mv;               //比较器输入迟滞 10mv
	tCmpCfg.byPhystsel = CMP_PHYST_POL_DIS;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;           //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel = CMP_CPOS_OUT_IN;	              //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_NONE;	      		      //中断模式
	csi_cmp_init(CMP0,&tCmpCfg);
	
	csi_cmp_dflt1_config_t tCmpDflt1Cfg;
	tCmpDflt1Cfg.byDepth1 = CMP_DFCR_DEPTH1_16;         //数字滤波1深度
	tCmpDflt1Cfg.byDivn1  = 2;                          //分频系数N
	tCmpDflt1Cfg.byDivm1  = 119;	                    //分频系数M
	csi_cmp_dflt1_config(CMP0,ENABLE,&tCmpDflt1Cfg);
	
	csi_cmp_dflt2_config_t tCmpDflt2Cfg;
	tCmpDflt2Cfg.byDepth2 = CMP_DFCR_DEPTH2_16;        //数字滤波2深度
	tCmpDflt2Cfg.byDivn2  = 2;                        //分频系数N
	tCmpDflt2Cfg.byDivm2  = 119;	                  //分频系数M
	csi_cmp_dflt2_config(CMP0,DISABLE,&tCmpDflt2Cfg);
	
	csi_cmp_start(CMP0);
	return iRet;	
}

/** \brief 比较器窗口功能测试demo
 *  \brief 通过BT0触发比较器窗口滤波功能
 *  \brief 窗口的宽度由时钟分频和窗口计数决定，同时可配置相应的窗口延迟
 *  \brief 如果PCLK时钟配置为24M，窗口滤波时钟周期为： 24M/（4+1） => 5/24(us)
 *  \brief 窗口滤波的时间宽度为5/24 * 200 = 17us，电平翻转将在窗口内生效，窗口外不进行电平翻转
 *   
 *  \param[in] none
 *  \return error code
 */
int cmp_wfcr_demo(void)
{
	uint8_t ch;
	int iRet = 0;
	
	csi_pin_set_mux(PA02, PA02_OUTPUT);	  
	csi_pin_set_mux(PA03, PA03_OUTPUT);	 
	csi_pin_set_low(PA02);	
	
	csi_pin_set_mux(PA00,PA00_CMP_IN0);	
	csi_pin_set_mux(PA110,PA110_CMP_IN1);		
	csi_pin_set_mux(PA111,PA111_CMP0_OUT);	
	
	csi_cmp_config_t tCmpCfg;
	tCmpCfg.byNsel = CMP_N_SEL_CP0;                    //N- 端口选择
	tCmpCfg.byPsel = CMP_P_SEL_CP1;	                  //P+ 端口选择
	tCmpCfg.byPhystpol = CMP_PHYST_POL_10mv;               //比较器输入迟滞 10mv
	tCmpCfg.byPhystsel = CMP_PHYST_POL_DIS;	          //比较器输入迟滞特性极性选择
	tCmpCfg.byPolarity = CMP_POL_OUT_DIRECT;           //比较器输出极性选择 0:不反向
	tCmpCfg.byCpoSel = CMP_CPOS_OUT_IN;	              //CMP_OUT管脚上输出信号选择 0h：滤波前信号直接输出 	1h：滤波后信号输出 
	tCmpCfg.wInt = CMP_INTSRC_NONE;	      //中断模式
	csi_cmp_init(CMP0,&tCmpCfg);	
	
	csi_cmp_wfcr_config_t tCmpWfcrCfg;
	tCmpWfcrCfg.byWfalign = CMP_WFCR_ALIGN_ALLOW;    //窗口滤波触发事件对齐设置
	tCmpWfcrCfg.byWoset   = CMP_WFCR_OSET_HIGH;      //窗口滤波设置输出初始化
	tCmpWfcrCfg.byClkDiv  = 4;                   //时钟分频
	tCmpWfcrCfg.byDcnt    = 0;                   //窗口延迟
	tCmpWfcrCfg.hwWcnt    = 200;                 //窗口计数
	csi_cmp_wfcr_config(CMP0,&tCmpWfcrCfg);
	
	csi_cmp_set_evtrg(CMP0, CMP_TRGOUT,CMP_EVE_DOWN_UP);
	
	csi_cmp_start(CMP0);	

	csi_bt_timer_init(BT0, 2000);		//初始化BT0, 定时2000us； BT定时，默认采用PEND中断
	csi_bt_start(BT0);					//启动定时器  
	csi_bt_set_evtrg(BT0, BT_TRGOUT, BT_TRGSRC_PEND);   
  
	csi_etb_config_t tEtbConfig;	                 //ETB 参数配置结构体	  	
	tEtbConfig.byChType = ETB_ONE_TRG_ONE;  		 //单个源触发单个目标
	tEtbConfig.bySrcIp  = ETB_BT0_TRGOUT ; 
	tEtbConfig.byDstIp =  ETB_CMP0_SYNCIN;   	     //CMP0 同步输入作为目标事件
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;
   
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig.byChType);	    //自动获取空闲通道号,ch >= 0 获取成功
	if(ch < 0)
		return -1;								    //ch < 0,则获取通道号失败
	iRet = csi_etb_ch_config(ch, &tEtbConfig);	
	
	while(1)
	{
		csi_pin_toggle(PA02);
		udelay(1);
	}	

	return iRet;	
}

/** \brief CMP interrupt handle function
 * 
 *  \param[in] none
 *  \return none
 */ 
__attribute__((weak)) void cmp_irqhandler(csp_cmp_t *ptCmpBase)
{
    // ISR content ...
	csp_cmp_t *ptCmp0Base, *ptCmp1Base;
	ptCmp0Base = (csp_cmp_t *)(APB_CMP0_BASE);
	ptCmp1Base = (csp_cmp_t *)(APB_CMP1_BASE);
	
	if(csp_cmp_get_isr(ptCmpBase) & CMP0_EDGEDET0_INT)
	{
		csp_cmp_clr_isr(ptCmp0Base,CMP_EDGEDET_INT);
	}
	else if(csp_cmp_get_isr(ptCmpBase) & CMP1_EDGEDET1_INT)
	{
		csp_cmp_clr_isr(ptCmp1Base,CMP_EDGEDET_INT);	
	}
	else if(csp_cmp_get_isr(ptCmpBase) & CMP0_RAWDET0_INT)
	{
		csp_cmp_clr_isr(ptCmp0Base,CMP_RAWDET_INT);	
	}
	else if(csp_cmp_get_isr(ptCmpBase) & CMP1_RAWDET1_INT)
	{
		csp_cmp_clr_isr(ptCmp1Base,CMP_RAWDET_INT);	
	}
}