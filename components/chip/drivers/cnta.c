/***********************************************************************//** 
 * \file  cnta.c
 * \brief  csi cnta driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-9-09 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-1-09 <td>V0.1  <td>ZJY     <td>modify
 * <tr><td> 2021-11-23 <td>V0.2  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

#include <sys_clk.h>
#include <drv/cnta.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/porting.h>
#include <drv/tick.h>

#include "csp_cnta.h"
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/


/** \brief initialize cnta data structure
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] wTimeOut:the timeout for cnta; pclk = 48000000hz，1-21845 us
 *  \return error code \ref csi_error_t
 */ 
static uint32_t apt_cnta_get_load_value(uint32_t wTimesOut,uint8_t byClkDivn)
{
	uint32_t wTempLoad = 1;
	
	if((csi_get_pclk_freq() % 1000000) == 0)
	{
		wTempLoad  =  csi_get_pclk_freq() / 1000000  * wTimesOut / 2 / (0x01 << byClkDivn); 		
	}
	else if((csi_get_pclk_freq() % 4000) <= 2000)                          //最大5556000 
	{
		wTempLoad  =  csi_get_pclk_freq() / 4000  * wTimesOut / 500 / (0x01 << byClkDivn); 
	}
	else
	{
		wTempLoad  =  csi_get_pclk_freq() / 1000  * wTimesOut / 2000 / (0x01 << byClkDivn); 
	}
	return wTempLoad;
} 
 
csi_error_t csi_cnta_timer_init(csp_cnta_t *ptCntaBase,uint32_t wTimeOut)
{
	uint8_t byClkDiv,byIndex;
	uint32_t wTempLoad = 1;
	
	for(byIndex=0;byIndex<4;byIndex++)
	{
		byClkDiv = byIndex;
		wTempLoad  =  apt_cnta_get_load_value(wTimeOut,byClkDiv);
		if(wTempLoad <= 0xffff)
		{
			if( (wTempLoad==0xfffe) || (wTempLoad==0xffff))
			{
				wTempLoad = 0xfffd;
				break;
			}
			if(wTempLoad<3)
			{
				wTempLoad = 3;
			}
			else
			{
				wTempLoad = wTempLoad-3;	
			}
			break;
		}	
	}
	
	if( (4 == byIndex) && (wTempLoad>0xffff) )//limit data
	{
		wTempLoad = 0xfffd;
		byClkDiv = 3;
	}
	
	
	csi_clk_enable((uint32_t *)ptCntaBase);
    csp_cnta_soft_rst(ptCntaBase);
	csp_cnta_set_ckdiv(ptCntaBase, byClkDiv);
	csp_cnta_count_mode(ptCntaBase, CNTA_REPEAT_MODE);
	
	csp_cnta_set_datal(ptCntaBase, wTempLoad);
	csp_cnta_set_datah(ptCntaBase, wTempLoad);
	csp_cnta_soft_updata(ptCntaBase);	
	csp_cnta_int_enable(ptCntaBase, CNTA_PENDL_INT, true);
	csi_irq_enable((uint32_t *)ptCntaBase);

	csp_cnta_set_carrier(ptCntaBase, CNTA_CARRIER_EN);
	csp_cnta_set_envelope(ptCntaBase,CNTA_CARRIER_OUT);
	csp_cnta_set_carrier_start_level(ptCntaBase,CNTA_OSP_LOW);
	csp_cnta_set_carrier_stop_level(ptCntaBase,CNTA_REMSTAT_LOW);
	
	
	return CSI_OK;
}

/** \brief start cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_cnta_start(csp_cnta_t *ptCntaBase)
{		
	csp_cnta_start(ptCntaBase);	
}

/** \brief stop cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return None
 */ 
void csi_cnta_stop(csp_cnta_t *ptCntaBase)
{	
    csp_cnta_stop(ptCntaBase);
}

/** \brief get cnta datah load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datah load value
 */ 
uint32_t csi_cnta_get_datah_value(csp_cnta_t *ptCntaBase)
{	
	return csp_cnta_get_datah(ptCntaBase);
}

/** \brief get cnta datal load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datal load value
 */ 
uint32_t csi_cnta_get_datal_value(csp_cnta_t *ptCntaBase)
{	
	return csp_cnta_get_datal(ptCntaBase);
}

/** \brief set cnta count mode
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] eCntMode: cnta count mode, one pulse/continuous
 *  \return none
 */ 
void csi_cnta_count_mode(csp_cnta_t *ptCntaBase, csi_cnta_cntmode_e eCntMode)
{
	csp_cnta_count_mode(ptCntaBase, eCntMode);
}

/** \brief cnta pwm init 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptCntaPwmCfg:point of pwm parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_pwm_init(csp_cnta_t *ptCntaBase,csi_cnta_pwm_config_t *ptCntaPwmCfg)
{	
	csi_error_t ret = CSI_OK;
	volatile uint32_t wDatahLoad; 
	volatile uint32_t wDatalLoad;
	uint32_t wPeriod;
	cnta_osp_e eOsp = 0;
	cnta_remstat_e eRemStat = 0;	
	
	if(ptCntaPwmCfg->wFreq == 0)//wFreq need > 0
		return CSI_ERROR;
		
	csi_clk_enable((uint32_t *)ptCntaBase);
    csp_cnta_soft_rst(ptCntaBase);
	
	wPeriod = (csi_get_pclk_freq() / ptCntaPwmCfg->wFreq) >> CNTA_CK_DIV1;
	
	if(wPeriod * ptCntaPwmCfg->byDutyCycle < 300)//When the high duty cycle is less than 3 cycles,he high level counter will be set to 0
		wDatahLoad = 0x00;
	else
		wDatahLoad = wPeriod * ptCntaPwmCfg->byDutyCycle / 100 - 3 ;
		
	if(wPeriod * (100 - ptCntaPwmCfg->byDutyCycle) < 300)//When the low duty cycle is less than 3 cycles,he low level counter will be set to 0	
		wDatalLoad = 0x00;
	else
		wDatalLoad = wPeriod * (100 - ptCntaPwmCfg->byDutyCycle) / 100 - 3 ;
	
	
	if(ptCntaPwmCfg->byStartLevel == CNTA_POLAR_LOW)			//initial polarity
		eOsp = CNTA_OSP_LOW;
	else if(ptCntaPwmCfg->byStartLevel == CNTA_POLAR_HIGH)
		eOsp = CNTA_OSP_HIGH;
		
	if(ptCntaPwmCfg->byStopLevel == CNTA_STOP_LOW)				//stop output level
		eRemStat = CNTA_REMSTAT_LOW;
	else if(ptCntaPwmCfg->byStopLevel == CNTA_STOP_HIGH)
		eRemStat = CNTA_REMSTAT_HIGH;
	
	if( (wDatahLoad==0xfffe) || (wDatahLoad==0xffff))
	{
		wDatahLoad = 0xfffd;
	}
	
	if( (wDatalLoad==0xfffe) || (wDatalLoad==0xffff))
	{
		wDatalLoad = 0xfffd;
	}
		
	csp_cnta_set_ckdiv(ptCntaBase, CNTA_CK_DIV1);		//cnta clk = pclk/eClkDiv
	csp_cnta_count_mode(ptCntaBase, CNTA_REPEAT_MODE);
	
	csp_cnta_set_carrier(ptCntaBase, CNTA_CARRIER_EN);
	csp_cnta_set_envelope(ptCntaBase,CNTA_CARRIER_OUT);
	csp_cnta_set_carrier_start_level(ptCntaBase,eOsp);
	csp_cnta_set_carrier_stop_level(ptCntaBase,eRemStat);

	csp_cnta_set_datah(ptCntaBase, wDatahLoad);
	csp_cnta_set_datal(ptCntaBase, wDatalLoad);
	csp_cnta_soft_updata(ptCntaBase);
	csp_cnta_int_enable(ptCntaBase, ptCntaPwmCfg->byInt, true);

	return ret;
}

/** \brief updata cnta pwm freq para: (datah and datal value)
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] hwDatah: rem ouput high time long 
 *  \param[in] hwDatal: rem ouput low time long  
 *  \param[in] eUpdata: data reg soft updata control
 *  \return none
 */
void csi_cnta_pwm_para_updata(csp_cnta_t *ptCntaBase, uint16_t hwDatah, uint16_t hwDatal, csi_cnta_sw_updata_e eUpdata)
{	
	
	csp_cnta_set_datah(ptCntaBase,hwDatah);	
	csp_cnta_set_datal(ptCntaBase,hwDatal);
	if(eUpdata == CNTA_SW_EN)
	{
		csp_cnta_soft_updata(ptCntaBase);
	}
}

/** \brief set cnta tc sync 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] eTcPendRem: carrieron control of tc pend end 
 *  \param[in] eTcMatchRem: carrieron control of tc pulse match 
 *  \param[in] eHwUpdata: cadata reg auto updata control
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cnta_bt0_sync(csp_cnta_t *ptCntaBase, csi_cnta_tcpend_e eTcPendRem, csi_cnta_tcmatch_e eTcMatchRem,csi_cnta_hw_updata_e eHwUpdata)
{
	csp_cnta_set_btpend_rem_con(ptCntaBase, eTcPendRem);
	csp_cnta_set_btmatch_rem_con(ptCntaBase,eTcMatchRem);
	csp_cnta_set_hw_strobe_data(ptCntaBase,eHwUpdata);
	
	return CSI_OK;
}

/** \brief cnta interrupt handle weak function
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
__attribute__((weak)) void cnta_irqhandler(csp_cnta_t *ptCntaBase)
{
	
	//The time to enter the interrupt is: set in csi_cnta_timer_init
	//add you code         
}
