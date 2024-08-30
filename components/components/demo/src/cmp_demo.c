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

/** \brief cmp base demo
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


/** \brief cmp dfcr demo
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

/** \brief cmp wfcr demo
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
	
	csi_cmp_set_evtrg(CMP0, CMP_EVE_DOWN_UP, ENABLE);
	
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