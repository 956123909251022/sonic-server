
/***********************************************************************//** 
 * \file  clk.c
 * \brief  clock management for all peripherals
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0 <td>WNN   <td>initial
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/
#include <drv/clk.h>
#include <drv/pin.h>

#include "csp.h"
#include "board_config.h"
#include "csp_syscon.h"
#include "lpt.h"
#include <drv/irq.h>

extern csi_clkmap_t clk_map[];

/** \brief clock enable
 * 
 *  Enable peripheral clock in SYSCON
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_enable(void *pIpBase)
{
    csi_clkmap_t *ptMap = clk_map;

    while(ptMap->wRegBase) 
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
			if((uint32_t)pIpBase != CORET_ADDR_BASE)
				soc_clk_enable((clk_module_e)ptMap->wModule);
			else
				SYSCON->GCER = (0x01ul << ptMap->wModule);
            break;
        }
        ptMap++;
    }
	
//	switch((uint32_t)pIpBase)
//	{
//		case APB_IFC_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)IFC_SYS_CLK);		//IFC clk enable
//			break;
//		case APB_ADC0_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)ADC_SYS_CLK);		//ADC clk enable
//			break;
//		case APB_TKEY_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)TOUCH_SYS_CLK);		//TKEY clk enable
//			break;
//		case APB_ETCB_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)ETCB_SYS_CLK);		//ETCB clk enable
//			break;
//		case APB_UART0_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)UART0_SYS_CLK);		//UART0 clk enable
//			break;
//		case APB_UART1_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)UART1_SYS_CLK);		//UART1 clk enable
//			break;
//		case APB_UART2_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)UART2_SYS_CLK);		//UART2 clk enable
//			break;
//		case APB_SPI0_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)SPI_SYS_CLK);		//SPI clk enable
//			break;
//		case APB_SIO0_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)SIO_SYS_CLK);		//SIO clk enable
//			break;
//		case APB_I2C0_BASE:
//			csp_pcer0_clk_en(SYSCON, (uint32_t)I2C_SYS_CLK);		//I2C clk enable
//			break;
//		case APB_WWDT_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)WWDT_SYS_CLK - 32);	//WWDT clk enable
//			break;
//		case APB_RTC_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)RTC_SYS_CLK - 32);	//RTC clk enable
//			break;
//		case APB_LPT_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)LPT_SYS_CLK - 32);	//LPT clk enable
//			break;
//		case APB_CNTA_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)CNTA_SYS_CLK - 32);	//CNTA clk enable
//			break;
//		case APB_BT0_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)BT0_SYS_CLK - 32);	//BT0 clk enable
//			break;
//		case APB_BT1_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)BT1_SYS_CLK - 32);	//BT1 clk enable
//			break;
//		case APB_GPTA0_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)GPT_SYS_CLK - 32);	//GPT clk enable
//			break;
//		case APB_EPT0_BASE:
//			csp_pder1_clk_dis(SYSCON, (uint32_t)EPT_SYS_CLK - 32);	//EPT clk enable
//			break;
//		default:
//			break;
//	}
}

/** \brief clock disable
 * 
 *  Disable peripheral clock in SYSCON
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_disable(void *pIpBase)
{
    csi_clkmap_t *ptMap = clk_map;

	while(ptMap->wRegBase)
	{
        if((uint32_t)pIpBase == ptMap->wRegBase) 
		{
			if((uint32_t)pIpBase != CORET_ADDR_BASE)
				soc_clk_disable((clk_module_e)ptMap->wModule);
			else
				SYSCON->GCDR = (0x01ul << ptMap->wModule);
            break;
        }
        ptMap++;
    }
}
/** \brief emosc enable
 * 
 *  enable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_emosc_enable(uint32_t wFreq)
{
	
	if ((csi_pin_get_mux(XIN_PIN) != XIN_PIN_FUNC) || (csi_pin_get_mux(XOUT_PIN) != XOUT_PIN_FUNC))
		return CSI_ERROR;
	
	if (wFreq > 20000000)
		csp_set_em_gain(SYSCON, 0x1f);
	else
		csp_set_em_gain(SYSCON, 0x7);
		
	//csp_set_em_gain(SYSCON, 0x1f);				
	
	if (wFreq == EMOSC_32K_VALUE) {
		csp_set_em_lfmd(SYSCON, ENABLE);
		SYSCON->OSTR = (SYSCON->OSTR) & ((~0x3ff)|(0xff));
	}
	

	SYSCON->GCER = EMOSC;
	//wait for EMOSC to stable
	while(!(csp_get_ckst(SYSCON)& EMOSC));
	return CSI_OK;
}

/** \brief emosc disable
 * 
 *  disable external main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_emosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_EMOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = EMOSC;
		return CSI_OK;
	}
	
//	if (csp_emosc_disable(SYSCON_REG_BASE) == CSP_FAIL)
//		return CSI_ERROR;
//	else
//		return CSI_OK;
}
/** \brief esosc enable
 * 
 *  enable external sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_esosc_enable(uint32_t wFreq)
{
	
	if ((csi_pin_get_mux(SXIN_PIN) != SXIN_PIN_FUNC) || (csi_pin_get_mux(SXOUT_PIN) != SXOUT_PIN_FUNC))
		return CSI_ERROR;
	
	csp_set_es_gain(SYSCON, 0x7);
	

	SYSCON->GCER = ESOSC;
	//wait for EMOSC to stable
	while(!(csp_get_ckst(SYSCON)& ESOSC));
	return CSI_OK;
}

/** \brief esosc disable
 * 
 *  disable external sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_esosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_ESOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = ESOSC;
		return CSI_OK;
	}
}
	

/** \brief imosc enable
 * 
 *  enable internal main oscillator in SYSCON
 * 
 *  \param byFre. 	0 - 5MHz
 * 					1 - 4MHz
 * 					2 - 2MHz
					3 - 131KHz
 *  \return csi_error_t
 */
csi_error_t csi_imosc_enable(uint8_t byFre)
{
	csp_set_imosc_fre(SYSCON, byFre);
	SYSCON->GCER = IMOSC;
	//wait for IMOSC to stable
	while(!(csp_get_ckst(SYSCON)& IMOSC));
	return CSI_OK;
	
//	csp_imosc_enable(SYSCON_REG_BASE, byFre);
//	return CSI_OK;
}

/** \brief imosc disable
 * 
 *  disable internal main oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_imosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_IMOSC)
		return CSI_ERROR;
	else{
		SYSCON->GCDR = IMOSC;
		return CSI_OK;
	}
	
//	if (csp_imosc_disable(SYSCON_REG_BASE) == CSP_FAIL)
//		return CSI_ERROR;
//	else
//		return CSI_OK;
}

 /** \brief hfosc enable
 * 
 *  enable high frequency oscillator in SYSCON
 * 
 *  \param byFre. 	0 - 48MHz
 * 					1 - 24MHz
 * 					2 - 12MHz
					3 - 6KHz
 *  \return csi_error_t.
 */
csi_error_t csi_hfosc_enable(uint8_t byFre)
{
	csp_set_hfosc_fre(SYSCON, byFre);	
	SYSCON->GCER = HFOSC;
	//wait for HFOSC to stable
	while(!(csp_get_ckst(SYSCON)& HFOSC));
	return CSI_OK;
	
//	csp_hfosc_enable(SYSCON_REG_BASE, byFre);
//	return CSI_OK;
}

/** \brief hfosc disable
 * 
 *  disable high frequency oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_hfosc_disable(void)
{
	if ((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_HFOSC)
		return CSI_ERROR;
	else
	{
		SYSCON->GCDR = HFOSC;
		return CSI_OK;
	}
	
//	if (csp_hfosc_disable(SYSCON_REG_BASE) == CSP_FAIL)
//		return CSI_ERROR;
//	else
//		return CSI_OK;
}

/** \brief isosc enable
 * 
 *  enable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t
 */
csi_error_t csi_isosc_enable(void)
{
	SYSCON->GCER = ISOSC;
	//wait for ISOSC to stable
	while(!(csp_get_ckst(SYSCON)& ISOSC));
	return CSI_OK;
	
//	csp_isosc_enable(SYSCON_REG_BASE);
//	return CSI_OK;
}

/** \brief isosc disable
 * 
 *  disable internal sub oscillator in SYSCON
 * 
 *  \param none
 *  \return csi_error_t.
 */
csi_error_t csi_isosc_disable(void)
{
	if((SYSCON->SCLKCR & SYSCLK_SRC_MSK) == SC_ISOSC || csp_iwdt_rd_st(SYSCON))
		return CSI_ERROR;
	else 
	{
		SYSCON->GCDR = ISOSC;
		return CSI_OK;
	}	
//	if (csp_isosc_disable(SYSCON_REG_BASE) == CSP_FAIL)
//		return CSI_ERROR;
//	else
//		return CSI_OK;
}

/** \brief csi_clk_calib
 * 
 *  calibrate IMCLK or HFCLK 
 * 
 *  \return csi_error_t.
 */
csi_error_t csi_clk_calib(void)
{
	cclk_src_e    eClkSrc;
	volatile csi_clk_calib_t tClkCalib;
	volatile unsigned int  wPclkValue,wCheckData;
	volatile unsigned char byFlag = 1,byDiv; 
	
	eClkSrc = ((cclk_src_e) csp_get_clksrc(SYSCON));	
	if((eClkSrc != SRC_IMOSC)&&(eClkSrc != SRC_HFOSC))
		return CSI_ERROR;
	
	csi_clk_enable((uint32_t *)BT0);									//bt clk enable
	csp_bt_soft_rst(BT0);												//reset bt	
	wPclkValue = csi_get_pclk_freq();
	csp_bt_set_cr(BT0, (BT_IMMEDIATE << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) |		//bt work mode
			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN );
	if(wPclkValue > 24000000)  
	{
		byDiv = 7;
	}
	else if((wPclkValue > 12000000)&&(wPclkValue <= 24000000)) 
	{
		byDiv = 3;
	}
	else if((wPclkValue > 6000000)&&(wPclkValue <= 12000000))  
	{
		byDiv = 1;	
	}
	else
	{
		byDiv = 0;
	}
	csp_bt_set_pscr(BT0, byDiv);						                    //bt clk div
	csp_bt_set_prdr(BT0, 65535);
	csp_bt_int_enable(BT0, BT_PEND_INT, true);					        //enable PEND interrupt
	csi_irq_enable((uint32_t *)BT0);									//enable bt irq	
	
	csi_clk_enable((uint32_t *)LPT);
	csp_lpt_soft_rst(LPT);
	csp_lpt_clk_enable(LPT, ENABLE);	
	csp_lpt_set_clk(LPT, LPT_ISCLK, LPT_PSC_DIV1);
	csp_lpt_set_prdr(LPT, 216);                                         // 27*8  
	csi_lpt_int_enable(LPT,LPT_PEND_INT,ENABLE);	
		
	wCheckData = wPclkValue/(125*(byDiv + 1));                         //(wPclkValue*8)/(1000*(byDiv + 1))
	if(wCheckData <= 1200)
		return CSI_UNSUPPORTED;	

	tClkCalib.wThresholdMin= wCheckData - (wCheckData/120);
	tClkCalib.wThresholdMax = wCheckData + (wCheckData/120);	


	if(eClkSrc == SRC_IMOSC)
	{
		tClkCalib.wClcrStep = CLCE_STEP_IM;
	}
	else
	{
		tClkCalib.wClcrStep  = CLCE_STEP_HF;
	}
	
	asm  ("psrclr ie");                                                 //disable interrupt
	tClkCalib.wClcrValue = SYSCON->CLCR;
	
	while(byFlag)
	{
		csp_lpt_sw_start(LPT);
		csp_bt_start(BT0);
		csp_lpt_set_cnt(LPT, 0);
		csp_bt_set_cnt(BT0, 0);	
		
		while(!(csp_lpt_get_misr(LPT)&0x04));
		tClkCalib.hwBtCntValue[0] = csp_bt_get_cnt(BT0);
		csp_lpt_clr_misr(LPT, LPT_PEND_INT);
		
		csp_lpt_set_cnt(LPT, 0);
		csp_bt_set_cnt(BT0, 0);	
		while(!(csp_lpt_get_misr(LPT)&0x04));
		tClkCalib.hwBtCntValue[1] = csp_bt_get_cnt(BT0);
		csp_lpt_clr_misr(LPT, LPT_PEND_INT);
			
		csp_lpt_set_cnt(LPT, 0);
		csp_bt_set_cnt(BT0, 0);	
		while(!(csp_lpt_get_misr(LPT)&0x04));
		tClkCalib.hwBtCntValue[2] = csp_bt_get_cnt(BT0);
		csp_lpt_clr_misr(LPT, LPT_PEND_INT);		
		
		tClkCalib.wBtSumValue = tClkCalib.hwBtCntValue[1]+tClkCalib.hwBtCntValue[2];
		tClkCalib.wBtSumValue = tClkCalib.wBtSumValue/2;
		
		csp_bt_stop(BT0);
		csp_lpt_stop(LPT);
		csp_lpt_set_cnt(LPT, 0);
		csp_bt_set_cnt(BT0, 0);
		
		if(eClkSrc == SRC_HFOSC)
		{
			if(tClkCalib.wBtSumValue<0x800)
			{
				byFlag =0;
				return CSI_ERROR;	
			}
			else if(tClkCalib.wBtSumValue>tClkCalib.wThresholdMax)  
			{
				tClkCalib.wClcrValue = tClkCalib.wClcrValue - tClkCalib.wClcrStep ;
				if((tClkCalib.wClcrValue&0xfe00)==0)
				{
					byFlag =0;
					return CSI_ERROR;	
				}
				SYSCON->CLCR = tClkCalib.wClcrValue;
			}
			else if(tClkCalib.wBtSumValue<tClkCalib.wThresholdMin)  
			{
				tClkCalib.wClcrValue = tClkCalib.wClcrValue + tClkCalib.wClcrStep ;
				if((tClkCalib.wClcrValue&0xfe00)==0xfe00)
				{
					byFlag =0;
					return CSI_ERROR;	
				}
				SYSCON->CLCR = tClkCalib.wClcrValue;
			}
			else
			{
				byFlag =0;
			}
		}
		else
		{
			if(tClkCalib.wBtSumValue>tClkCalib.wThresholdMax)                    
			{
				tClkCalib.wClcrValue = tClkCalib.wClcrValue - tClkCalib.wClcrStep ;
				if((tClkCalib.wClcrValue&0xff0000)==0)
				{
					byFlag =0;
					return CSI_ERROR;	
				}
				SYSCON->CLCR = tClkCalib.wClcrValue;
			}
			else if(tClkCalib.wBtSumValue<tClkCalib.wThresholdMin)  
			{
				tClkCalib.wClcrValue = tClkCalib.wClcrValue + tClkCalib.wClcrStep ;
				if((tClkCalib.wClcrValue&0xff0000)==0xff0000)
				{
					byFlag =0;
					return CSI_ERROR;	
				}
				SYSCON->CLCR = tClkCalib.wClcrValue;
			}
			else
			{
				byFlag =0;
			}			
		}
		mdelay(1);
	}
	return CSI_OK;
	asm  ("psrset ee,ie");                                                //enable interrupt	
}