/***********************************************************************//** 
 * \file  bt.c
 * \brief  csi bt driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/

#include <sys_clk.h>
#include <drv/bt.h>
#include <drv/irq.h>
#include <drv/tick.h>
#include <drv/pin.h>

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
static uint8_t	s_byBtCtrl[2]	= {0, 0};
static uint8_t	s_byIdleBk[2]	= {0, 0};		//idle level backup
static uint8_t	s_byStaBk[2]	= {0, 0};		//start level backup
static uint16_t	s_hwPscrVal[2]	= {0, 0};		//pscr value backup
static uint32_t	s_wPrdrVal[2]	= {0, 0};		//prdr value backup
static uint32_t	s_wCmpVal[2]	= {0, 0};		//cmp value backup


/** \brief bt interrupt handle function
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
__attribute__((weak)) void bt_irqhandler(csp_bt_t *ptBtBase)
{
    // ISR content ...
	volatile uint32_t wMisr = csp_bt_get_isr(ptBtBase);
	
	if(wMisr & BT_PEND_INT)					//PEND interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_PEND_INT);
	}
	
	if(wMisr & BT_CMP_INT)					//CMP interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_CMP_INT);
	}
	
	if(wMisr & BT_OVF_INT)					//OVF interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_OVF_INT);
	}
	
	if(wMisr & BT_EVTRG_INT)				//EVTRG interrupt
	{
		csp_bt_clr_isr(ptBtBase, BT_EVTRG_INT);
	}
}
/** \brief get bt number 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return bt number 0/1
 */ 
static uint8_t apt_get_bt_num(csp_bt_t *ptBtBase)
{
	switch((uint32_t)ptBtBase)
	{
		case APB_BT0_BASE:		//bt0
			return 0;		
		case APB_BT1_BASE:		//bt1
			return 1;
		default:
			return 0xff;		//error
	}
}
/** \brief initialize bt data structure
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] wTimeOut: the timeout for bt, unit: us, 20us < wTimeOut < 3S
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_timer_init(csp_bt_t *ptBtBase, uint32_t wTimeOut)
{
	uint32_t wTmLoad;
	uint32_t wClkDiv;
	uint8_t byIdx = apt_get_bt_num(ptBtBase);
	
	if((0 == wTimeOut) || (byIdx > 1))
		return CSI_ERROR;
	
	s_byBtCtrl[byIdx] =0;
	csi_clk_enable(ptBtBase);													//bt clk enable
	csp_bt_soft_rst(ptBtBase);													//reset bt
	
	apt_timer_set_load_value(wTimeOut,TIMER_16BIT_MODE);
	wTmLoad = apt_timer_get_prdrload_value();
	wClkDiv = apt_timer_get_clkdiv_value();
		
	csp_bt_set_cr(ptBtBase, (BT_IMMEDIATE << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) |		//bt work mode
			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN);
	csp_bt_set_pscr(ptBtBase, (uint16_t)wClkDiv - 1);						//bt clk div	
	csp_bt_set_prdr(ptBtBase, (uint16_t)wTmLoad);							//bt prdr load value
	csp_bt_set_cmp(ptBtBase, (uint16_t)(wTmLoad >> 1));						//bt prdr load value
	csp_bt_int_enable(ptBtBase, BT_PEND_INT, true);							//enable PEND interrupt
	csi_irq_enable((uint32_t *)ptBtBase);									//enable bt irq
	
    return CSI_OK;
}
/** \brief set bt count mode
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eCntMode: bt count mode, one pulse/continuous
 *  \return none
 */ 
void csi_bt_count_mode(csp_bt_t *ptBtBase, csi_bt_cntmode_e eCntMode)
{
	csp_bt_count_mode(ptBtBase, eCntMode);
}

/** \brief start bt
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_start(csp_bt_t *ptBtBase)
{
	uint8_t byIdx = apt_get_bt_num(ptBtBase);
	
	if(byIdx > 1)
		return CSI_ERROR;
	
	if(0 == s_byBtCtrl[byIdx])
		csp_bt_start(ptBtBase);
	else
		csp_bt_stop(ptBtBase);
		
	return CSI_OK;
}
/** \brief stop bt
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
void csi_bt_stop(csp_bt_t *ptBtBase)
{
    csp_bt_stop(ptBtBase);
}

/** \brief stop bt
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eIntSrc: bt interrupt source
 *  \param[in] bEnable: enable/disable interrupt
 *  \return none
 */ 
void csi_bt_int_enable(csp_bt_t *ptBtBase, csi_bt_intsrc_e eIntSrc, bool bEnable)
{
	csp_bt_int_enable(ptBtBase, eIntSrc, bEnable);	
	
	if (bEnable) {
		csi_irq_enable((uint32_t *)ptBtBase);
	}
	else {
		if (eIntSrc == csp_bt_get_isr(ptBtBase)) {
			csi_irq_disable((uint32_t *)ptBtBase);
		}
	}
}
/** \brief get bt remaining value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
uint32_t csi_bt_get_remaining_value(csp_bt_t *ptBtBase)
{
    uint32_t wTimeVal = csp_bt_get_prdr(ptBtBase) - csp_bt_get_cnt(ptBtBase);

	return wTimeVal;
}
/** \brief get bt load value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return none
 */ 
uint32_t csi_bt_get_load_value(csp_bt_t *ptBtBase)
{
	return csp_bt_get_prdr(ptBtBase);
}
/** \brief check bt is running
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \return rue: running; false: stopped
 */ 
bool csi_bt_is_running(csp_bt_t *ptBtBase)
{
	CSI_PARAM_CHK(ptBtBase, false);
	 
	if(ptBtBase->RSSR & BT_START)
		return true;
	else 
		return false;
}

/** \brief bt pwm init 
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] ptBtPwmCfg: pointer of bt pwm parameter config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_bt_pwm_init(csp_bt_t *ptBtBase, csi_bt_pwm_config_t *ptBtPwmCfg)
{
	//uint32_t wClkDiv;
	//uint32_t wCmpLoad; 
	//uint32_t wPrdrLoad; 
	uint32_t wCrVal;
	uint8_t byIdx = apt_get_bt_num(ptBtBase);
	
	if(byIdx > 1)
		return CSI_ERROR;
	
	s_byBtCtrl[byIdx] = 1;
	s_byIdleBk[byIdx] = ptBtPwmCfg->byIdleLevel;
	s_byStaBk[byIdx]  = ptBtPwmCfg->byStartLevel;
	
	if(ptBtPwmCfg->byDutyCycle == 0)
	{
		if(ptBtPwmCfg->byStartLevel)
			ptBtPwmCfg->byIdleLevel = BT_PWM_START_LOW;
		else
			ptBtPwmCfg->byIdleLevel = BT_PWM_START_HIGH;
	}
	else if(ptBtPwmCfg->byDutyCycle >= 100)
	{
		ptBtPwmCfg->byDutyCycle = 100;
		
		if(ptBtPwmCfg->byStartLevel)
			ptBtPwmCfg->byIdleLevel = BT_PWM_START_HIGH;
		else
			ptBtPwmCfg->byIdleLevel = BT_PWM_START_LOW;
	}
	else
	{
		s_byBtCtrl[byIdx] = 0;
		if(ptBtPwmCfg->wFreq == 0)
			return CSI_ERROR;
	}
	
	csi_clk_enable((uint32_t *)ptBtBase);									//bt clk enable
	csp_bt_soft_rst(ptBtBase);												//reset bt
		
	s_hwPscrVal[byIdx] = (csi_get_pclk_freq() / ptBtPwmCfg->wFreq / 30000);	//bt clk div value
	if(s_hwPscrVal[byIdx] == 0)
		s_hwPscrVal[byIdx] = 1;
	
	s_wPrdrVal[byIdx]  = csi_get_pclk_freq() / (s_hwPscrVal[byIdx] * ptBtPwmCfg->wFreq);	//prdr load value
	s_wCmpVal[byIdx] = s_wPrdrVal[byIdx] * ptBtPwmCfg->byDutyCycle / 100;					//cmp load value	
	wCrVal = BT_CLK_EN | (BT_SHDOW << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) | (BT_PCLKDIV << BT_EXTCKM_POS) |
				(BT_CNTRLD_EN << BT_CNTRLD_POS) | (ptBtPwmCfg->byIdleLevel << BT_IDLEST_POS) | (ptBtPwmCfg->byStartLevel << BT_STARTST_POS);
	
	csp_bt_set_cr(ptBtBase, wCrVal);										//set bt work mode
	csp_bt_set_pscr(ptBtBase, (uint16_t)s_hwPscrVal[byIdx] - 1);			//bt clk div
	csp_bt_set_prdr(ptBtBase, (uint16_t)s_wPrdrVal[byIdx]);					//bt prdr load value
	csp_bt_set_cmp(ptBtBase, (uint16_t)s_wCmpVal[byIdx]);					//bt cmp load value
	
	if(ptBtPwmCfg->byInt)
	{
		csp_bt_int_enable(ptBtBase, ptBtPwmCfg->byInt, true);				//enable interrupt
		csi_irq_enable((uint32_t *)ptBtBase);								//enable bt irq
	}
	
	return CSI_OK;
}
/** \brief  updata bt para and cmp reg value
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] hwPrdr: bt padr reg  value
 *  \param[in] hwCmp: bt cmp reg value
 *  \return none
 */
void csi_bt_prdr_cmp_updata(csp_bt_t *ptBtBase, uint16_t hwPrdr, uint16_t hwCmp) 
{
	csp_bt_set_prdr(ptBtBase, hwPrdr);						//bt prdr load value
	csp_bt_set_cmp(ptBtBase, hwCmp);						//bt cmp load value
}
/** \brief  updata bt pwm duty cycle
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] byDutyCycle: duty cycle(0 -> 100)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_pwm_duty_cycle_updata(csp_bt_t *ptBtBase, uint8_t byDutyCycle) 
{
	volatile uint32_t wCmpVal;		// = s_wPrdrVal * byDutyCycle /100;
	uint8_t  byIdleLev = 0;
	uint8_t byIdx = apt_get_bt_num(ptBtBase);
	
	if(byIdx > 1)
		return CSI_ERROR;
	
	if(byDutyCycle >= 100)
		byDutyCycle = 100;
	
	switch(byDutyCycle)
	{
		case 0:
			s_byBtCtrl[byIdx] = 1;
			if(s_byStaBk[byIdx])
				byIdleLev = BT_PWM_START_LOW;
			else
				byIdleLev = BT_PWM_START_HIGH;
				
			csp_bt_set_idlelev(ptBtBase, byIdleLev);
			csp_bt_stop(ptBtBase);
			csp_bt_set_cmp(ptBtBase, 0);
			break;
		case 100:
			s_byBtCtrl[byIdx] =1;
			csp_bt_set_idlelev(ptBtBase, s_byStaBk[byIdx]);
			csp_bt_stop(ptBtBase);
			csp_bt_set_cmp(ptBtBase, s_wPrdrVal[byIdx]);
			break;
		default:
			wCmpVal = s_wPrdrVal[byIdx] * byDutyCycle /100;
			csp_bt_set_cmp(ptBtBase, wCmpVal);
			if(s_byBtCtrl[byIdx])
			{
				csp_bt_set_idlelev(ptBtBase, s_byIdleBk[byIdx]);
				csp_bt_start(ptBtBase);
				s_byBtCtrl[byIdx] = 0;
			}
			break;
	}
	
	return CSI_OK;
}

/** \brief  updata bt pwm freq and duty cycle
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] wFreq: pwm frequency, wFreq > 0 
 *  \param[in] byDutyCycle: pwm duty cycle(0 ~ 100)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_pwm_updata(csp_bt_t *ptBtBase, uint32_t wFreq, uint8_t byDutyCycle) 
{
	uint8_t  byIdleLev = 0;
	uint8_t byIdx = apt_get_bt_num(ptBtBase);
	
	if((wFreq == 0) || (byIdx > 1))
		return CSI_ERROR;
		
	if(byDutyCycle >= 100)
		byDutyCycle = 100;
	
	s_hwPscrVal[byIdx] = (csi_get_pclk_freq() / wFreq / 30000);		//bt clk div value
	if(s_hwPscrVal[byIdx] == 0)
		s_hwPscrVal[byIdx] = 1;
	
	s_wPrdrVal[byIdx] = (csi_get_pclk_freq() / (s_hwPscrVal[byIdx] * wFreq));
	
	switch(byDutyCycle)
	{
		case 0:
			s_byBtCtrl[byIdx] =1;
			if(s_byStaBk[byIdx])
				byIdleLev = BT_PWM_START_LOW;
			else
				byIdleLev = BT_PWM_START_HIGH;
				
			csp_bt_set_idlelev(ptBtBase, byIdleLev);
			csp_bt_stop(ptBtBase);
			
			csp_bt_set_pscr(ptBtBase, s_hwPscrVal[byIdx] - 1);			//bt clk div
			csp_bt_set_prdr(ptBtBase, s_wPrdrVal[byIdx]);				//bt prdr load value
			csp_bt_set_cmp(ptBtBase, 0);								//bt cmp load value
			break;
		case 100:
			s_byBtCtrl[byIdx] =1;
			csp_bt_set_idlelev(ptBtBase, s_byStaBk[byIdx]);
			csp_bt_stop(ptBtBase);
			
			csp_bt_set_pscr(ptBtBase, s_hwPscrVal[byIdx] - 1);			//bt clk div
			csp_bt_set_prdr(ptBtBase, s_wPrdrVal[byIdx]);				//bt prdr load value
			csp_bt_set_cmp(ptBtBase, 100);		
			break;
		default:
			s_wCmpVal[byIdx] = s_wPrdrVal[byIdx] * byDutyCycle /100;
			csp_bt_set_pscr(ptBtBase, s_hwPscrVal[byIdx] - 1);			//bt clk div
			csp_bt_set_prdr(ptBtBase, s_wPrdrVal[byIdx]);				//bt prdr load value
			csp_bt_set_cmp(ptBtBase, s_wCmpVal[byIdx]);	
			if(s_byBtCtrl[byIdx])
			{
				csp_bt_set_idlelev(ptBtBase, s_byIdleBk[byIdx]);
				csp_bt_start(ptBtBase);
				s_byBtCtrl[byIdx] = 0;
			}
			break;
	}

	return CSI_OK;
}

/** \brief bt sync input evtrg config  
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eTrgin: bt sync evtrg input channel(0~1)
 *  \param[in] eTrgMode: bt evtrg mode, continuous/once
 *  \param[in] bAutoRearm: auto rearm, ENABLE/DISABLE(true/false)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_set_sync(csp_bt_t *ptBtBase,csi_bt_trgin_e eTrgin, csi_bt_trgmode_e eTrgMode, bool bAutoRearm)
{
	if(eTrgin > BT_TRG_SYNCIN2)
		return CSI_ERROR;
		
	ptBtBase->CR = ptBtBase->CR & ~(BT_SYNCCMD_MSK | BT_OSTMD_MSK(eTrgin) | BT_AREARM_MSK);
	
	ptBtBase->CR |=  (BT_SYNC_EN << BT_SYNC_POS(eTrgin)) | (BT_SYNCMD_DIS << BT_SYNCCMD_POS) | 
				(eTrgMode << BT_OSTMD_POS(eTrgin)) | (bAutoRearm << BT_AREARM_POS);
	
	if(eTrgin == BT_TRG_SYNCIN1)
	{
		ptBtBase->CR |= BT_EXTCKM_MSK;		//selecet count clk source
		//ptBtBase->RSSR |= BT_START;			//start bt				
	}
	else
		ptBtBase->CR &= ~BT_EXTCKM_MSK;		//selecet count clk source
		
	return CSI_OK; 
}
/** \brief rearm bt sync evtrg  
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eTrgin: bt sync evtrg input channel(0~1)
 *  \return none
 */
void csi_bt_rearm_sync(csp_bt_t *ptBtBase,csi_bt_trgin_e eTrgin)
{
	csp_bt_rearm_sync(ptBtBase, eTrgin);
}
/** \brief bt evtrg output config
 * 
 *  \param[in] ptBtBase:pointer of bt register structure
 *  \param[in] eTrgOut: evtrg out port (only BT_TRGOUT)
 *  \param[in] eTrgSrc: evtrg source(1~3) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_set_evtrg(csp_bt_t *ptBtBase, csi_bt_trgout_e eTrgOut, csi_bt_trgsrc_e eTrgSrc)
{
	if (eTrgOut > 0)
		return CSI_ERROR;
	
	if(eTrgSrc == BT_TRGSRC_DIS)
		csp_bt_set_evtrg(ptBtBase,0x00);
	else
		csp_bt_set_evtrg(ptBtBase,(eTrgSrc | (BT_TRGOE_EN << BT_TRGOE_POS)));
		
	return CSI_OK;
}
/** \brief bt evtrg output enable/disable
 * 
 *  \param[in] ptBtBase: pointer of bt register structure
 *  \param[in] eTrgOut: bt evtrg out port (0)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_bt_evtrg_enable(csp_bt_t *ptBtBase, csi_bt_trgout_e eTrgOut, bool bEnable)
{
	if(eTrgOut == BT_TRGOUT)
		ptBtBase->EVTRG = (ptBtBase->EVTRG & ~BT_TRGOE_MSK) | (bEnable << BT_TRGOE_POS);
	else
		return CSI_ERROR;
		
	return CSI_OK;
}
/** \brief bt software generates a trigger event
 * 
 *  \param[in] ptBtBase:pointer of bt register structure
 *  \return error code \ref csi_error_t
 */
void csi_bt_soft_evtrg(csp_bt_t *ptBtBase)
{
	csi_clk_enable((uint32_t *)ptBtBase);										
	csp_bt_set_evtrg(ptBtBase,(0x00 | (BT_TRGOE_EN << BT_TRGOE_POS)));
	csp_bt_evswf_en(ptBtBase);
}
//+++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++
/** \brief start bt by sync event
 * 
 *  \param[in] ptBtBase: handle bt handle to operate
 *  \param[in] wTimeOut: tthe timeout for bt, unit: us
 *  \return none
 */ 
void csi_bt_start_sync(csp_bt_t *ptBtBase, uint32_t wTimeOut)
{
	uint32_t wTmLoad;
	uint32_t wClkDiv;
	
	csi_clk_enable((uint32_t *)ptBtBase);									//bt clk enable
	csp_bt_soft_rst(ptBtBase);                                              //reset bt

	apt_timer_set_load_value(wTimeOut,TIMER_16BIT_MODE);
	wTmLoad = apt_timer_get_prdrload_value();
	wClkDiv = apt_timer_get_clkdiv_value();
	
	csp_bt_set_cr(ptBtBase, (BT_IMMEDIATE << BT_SHDW_POS) | (BT_CONTINUOUS << BT_OPM_POS) |		//bt work mode
			(BT_PCLKDIV << BT_EXTCKM_POS) | (BT_CNTRLD_EN << BT_CNTRLD_POS) | BT_CLK_EN );
	csp_bt_set_pscr(ptBtBase, (uint16_t)wClkDiv - 1);						//bt clk div	
	csp_bt_set_prdr(ptBtBase, (uint16_t)wTmLoad);							//bt prdr load value
	csp_bt_int_enable(ptBtBase, BT_PEND_INT, true);							//enable PEND interrupt
	csi_irq_enable((uint32_t *)ptBtBase);									//enable bt irq
}