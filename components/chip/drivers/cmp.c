/***********************************************************************//** 
 * \file  cmp.c
 * \brief  csi cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-11-22 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/cmp.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include "csp_syscon.h"
#include "csp_cmp.h"

/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

/** \brief Enable cmp power manage
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntSrc: cmp interrupt source
 *  \param[in] bEnable: cmp irq enable or disable
 *  \return none
 */
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc,bool bEnable)
{
	csp_cmp_int_enable(ptCmpBase, (cmp_int_e)eIntSrc,bEnable);
	if (bEnable) 
	{
		csi_irq_enable((uint32_t *)ptCmpBase);
	}
	else 
	{
		if (csp_cmp_get_imcr(ptCmpBase)) 
		{
			csi_irq_disable((uint32_t *)ptCmpBase);
		}
	}
}

/** \brief init cmp
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] ptCmpCfg: pointer of cmp parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_init(csp_cmp_t *ptCmpBase,csi_cmp_config_t *ptCmpCfg)
{
	csi_error_t tRet = CSI_OK;
	csi_clk_enable((uint32_t *)ptCmpBase);
	csp_cmp_clk_enable(ptCmpBase, ENABLE);
	csp_cmp_inpcr(ptCmpBase ,ptCmpCfg->byNsel,ptCmpCfg->byPsel);
	if(ptCmpCfg->byNsel == CMP_N_SEL_1VBUF)
	{
		csp_cmp_inpcr_1vbuf_enable(ptCmpBase,ENABLE);
	}
	csp_cmp_hystpol(ptCmpBase , ptCmpCfg->byPhystpol,ptCmpCfg->byPhystsel);
	csp_cmp_polarity(ptCmpBase , ptCmpCfg->byPolarity);
	csp_cmp_out(ptCmpBase , ptCmpCfg->byCpoSel);
	csi_cmp_int_enable(ptCmpBase, ptCmpCfg->wInt,ENABLE);
	return tRet;
}

/** \brief start cmp
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return none
 */
void csi_cmp_start(csp_cmp_t *ptCmpBase)
{
	csp_cmp_enable(ptCmpBase);
}

/** \brief stop cmp
 * 
 *  \param[in] ptBtBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_stop(csp_cmp_t *ptCmpBase)
{
    csp_cmp_disable(ptCmpBase);
}

/** \brief cmp dflt1 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt1 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt1_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt1_config_t *ptCmpDfltCfg)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{
		csp_cmp_dflt1_enable(ptCmpBase ,ENABLE);
		csp_cmp_dfcr1(ptCmpBase,ptCmpDfltCfg->byDepth1,ptCmpDfltCfg->byDivn1,ptCmpDfltCfg->byDivm1);
	}
	else
	{
		csp_cmp_dflt1_enable(ptCmpBase ,DISABLE);		
	}
	return tRet;
}

/** \brief cmp dflt2 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt2 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt2_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt2_config_t *ptCmpDfltCfg)
{
	csi_error_t tRet = CSI_OK;
	if(bEnable == ENABLE)
	{	
		csp_cmp_dflt2_enable(ptCmpBase ,ENABLE);
		csp_cmp_dfcr2(ptCmpBase,ptCmpDfltCfg->byDepth2,ptCmpDfltCfg->byDivn2,ptCmpDfltCfg->byDivm2);
	}
	else
	{
		csp_cmp_dflt2_enable(ptCmpBase ,DISABLE);
	}
	return tRet;
}

/** \brief cmp wflt config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] ptCmpWfcrCfg: pointer of cmp wflt config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_wfcr_config(csp_cmp_t *ptCmpBase,csi_cmp_wfcr_config_t *ptCmpWfcrCfg)
{
	csi_error_t tRet = CSI_OK;

	csp_cmp_wf_set(ptCmpBase,ENABLE,ptCmpWfcrCfg->byWfalign,ptCmpWfcrCfg->byWoset);
	csp_cmp_wfcr(ptCmpBase , ptCmpWfcrCfg->hwWcnt,ptCmpWfcrCfg->byClkDiv,ptCmpWfcrCfg->byDcnt);
	
	return tRet;
}

/** \brief cmp evtrg output config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eTrgOut: cmp evtrg out port (0)
 *  \param[in] eTrgSrc: cmp evtrg source(0~3) 
 *  \param[in] bEnable: cmp evtrg enable or disable
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase, csi_cmp_trgout_e eTrgOut, csi_cmp_trgsrc_e eTrgSrc)
{
	if(eTrgOut == CMP_TRGOUT)
		csp_cmp_evtrg(ptCmpBase, ENABLE ,(cmp_eve_sel_e)eTrgSrc);
	else 
		return CSI_ERROR;
		
	return CSI_OK;
}
/** \brief cmp evtrg output enable/disable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eTrgOut: cmp evtrg out port (0)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_evtrg_enable(csp_cmp_t *ptCmpBase, csi_cmp_trgout_e eTrgOut, bool bEnable)
{
	if(eTrgOut == CMP_TRGOUT)
		ptCmpBase->CR = (ptCmpBase->CR & ~CMP_SYNCOE_MSK) | (bEnable << CMP_SYNCOE_POS);
	else
		return CSI_ERROR;
		
	return CSI_OK;
}

/** \brief cmp out status
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] byOutCh: cmp out ch sel(cmp0~cmp1)
 *  \return out status(0~1)
 */
uint8_t csi_cmp_get_out(csp_cmp_t *ptCmpBase,uint8_t byOutCh)
{
	if(byOutCh == CMP0_OUT)
		return csp_cmp_get_out0(ptCmpBase);
	else
		return csp_cmp_get_out1(ptCmpBase);
}

/** \brief clear cmp interrupt status
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eIntMode: EDGEDET_MODE or RAWDET_MODE
 *  \return none
 */
void csi_cmp_clr_isr(csp_cmp_t *ptCmpBase,csi_cmp_intsrc_e eIntMode)
{
	csp_cmp_clr_isr(ptCmpBase,(cmp_int_e)eIntMode);
}

/** \brief get cmp interrupt status
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \return cmp int status
 */
uint32_t csi_cmp_get_isr(csp_cmp_t *ptCmpBase)
{
	return csp_cmp_get_isr(ptCmpBase);
}

/** \brief Enable cmp power manage
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: cmp lpwken enable or disable
 */
void csi_cmp_lpwken_enable(csp_cmp_t *ptCmpBase, bool bEnable)
{
	csp_cmp_lpwken_enable(ptCmpBase,bEnable);
}