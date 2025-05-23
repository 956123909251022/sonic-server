/***********************************************************************//** 
 * \file  ept.c
 * \brief  Enhanced purpose timer driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-17 <td>V0.0  <td>ljy   <td>initial
 * <tr><td> 2023-5-10  <td>V0.1 <td>wch   <td>modify
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/common.h>
#include <drv/ept.h>
#include <drv/irq.h>
#include <sys_clk.h>
#include "csp_ept.h"

uint32_t g_wEptPrd;

/** \brief Basic configuration
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptEptCfg: refer to csi_ept_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_config_init(csp_ept_t *ptEptBase, csi_ept_config_t *ptEptCfg)
{
	uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptEptCfg->wFreq == 0 ){ptEptCfg->wFreq =100;}
		
	csi_clk_enable((uint32_t *)ptEptBase);								// clk enable
	csp_ept_clken(ptEptBase);
	csp_ept_wr_key(ptEptBase);                                           //Unlocking
	csp_ept_reset(ptEptBase);											// reset 
	
	if(ptEptCfg->byCountingMode==EPT_UPDNCNT){
		    wClkDiv = (csi_get_pclk_freq()  / ptEptCfg->wFreq /2 / 30000);		//clk div value
			if(wClkDiv == 0)wClkDiv = 1;	
			wPrdrLoad  = (csi_get_pclk_freq()/ptEptCfg->wFreq /2 /wClkDiv);	    //prdr load value
		
	}else{
			wClkDiv = (csi_get_pclk_freq() / ptEptCfg->wFreq / 30000);		// clk div value
			if(wClkDiv == 0)wClkDiv = 1;	
			wPrdrLoad  = (csi_get_pclk_freq()/ptEptCfg->wFreq/wClkDiv);	    //prdr load value
	}	
			
	wCrVal =ptEptCfg->byCountingMode | (ptEptCfg->byStartSrc<<EPT_STARTSRC_POS) |
	        ptEptCfg->byOneshotMode<<EPT_OPMD_POS | (ptEptCfg->byWorkmod<<EPT_MODE_POS);
    
	wCrVal=(wCrVal & ~(EPT_PSCLD_MSK))   |((ptEptCfg->byPscld&0x03)   <<EPT_PSCLD_POS);
	
	if(ptEptCfg->byWorkmod==EPT_CAPTURE)
	{
	 	wCrVal=(wCrVal & ~(EPT_CAPMD_MSK))   |((ptEptCfg->byCaptureCapmd&0x01)   <<EPT_CAPMD_POS);
		wCrVal=(wCrVal & ~(EPT_STOPWRAP_MSK))|((ptEptCfg->byCaptureStopWrap&0x03)<<EPT_STOPWRAP_POS);
		wCrVal=(wCrVal & ~(EPT_CMPA_RST_MSK))|((ptEptCfg->byCaptureLdaret&0x01)  <<EPT_CMPA_RST_POS);
		wCrVal=(wCrVal & ~(EPT_CMPB_RST_MSK))|((ptEptCfg->byCaptureLdbret&0x01)  <<EPT_CMPB_RST_POS);
		wCrVal=(wCrVal & ~(EPT_CMPC_RST_MSK))|((ptEptCfg->byCaptureLdcret&0x01)  <<EPT_CMPC_RST_POS);
		wCrVal=(wCrVal & ~(EPT_CMPD_RST_MSK))|((ptEptCfg->byCaptureLddret&0x01)  <<EPT_CMPD_RST_POS);
		
		if(ptEptCfg->byCaptureCapLden)wCrVal|=EPT_CAPLD_EN;
		if(ptEptCfg->byCaptureRearm)  wCrVal|=EPT_CAPREARM;
		
		wPrdrLoad=0xFFFF;
	}
	
	
    csp_ept_clken(ptEptBase);                                           // clkEN
	csp_ept_set_cr(ptEptBase, wCrVal);									// set bt work mode
	csp_ept_set_pscr(ptEptBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_ept_set_prdr(ptEptBase, (uint16_t)wPrdrLoad);				    // prdr load value
			
		
	if(ptEptCfg->byDutyCycle>=100){wCmpLoad=0;}
	else if(ptEptCfg->byDutyCycle==0){wCmpLoad=wPrdrLoad+1;}
	else{wCmpLoad =wPrdrLoad-(wPrdrLoad * ptEptCfg->byDutyCycle /100);}			
	csp_ept_set_cmpa(ptEptBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_ept_set_cmpb(ptEptBase, (uint16_t)wCmpLoad);
	csp_ept_set_cmpc(ptEptBase, (uint16_t)wCmpLoad);
	csp_ept_set_cmpd(ptEptBase, (uint16_t)wCmpLoad);
	
	
	if(ptEptCfg->wInt)
	{
		csp_ept_int_enable(ptEptBase, ptEptCfg->wInt, true);			//enable interrupt
		csi_irq_enable((uint32_t *)ptEptBase);							//enable  irq
	}
	
	g_wEptPrd=wPrdrLoad;
	
	return CSI_OK;
}

/** \brief capture configuration
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptEptCapCfg: refer to csi_ept_captureconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_capture_init(csp_ept_t *ptEptBase, csi_ept_captureconfig_t *ptEptCapCfg)
{
	uint32_t wClkDiv=0;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
			
	csi_clk_enable((uint32_t *)ptEptBase);								 // clk enable	
	csp_ept_clken(ptEptBase);
	csp_ept_wr_key(ptEptBase);                                           //Unlocking
	csp_ept_reset(ptEptBase);											 // reset 
	
			                                                             // clk div value
//	if(wClkDiv == 0){wClkDiv = 1;}
					
	wCrVal =ptEptCapCfg->byCountingMode | (ptEptCapCfg->byStartSrc<<EPT_STARTSRC_POS) |
	        ptEptCapCfg->byOneshotMode<<EPT_OPMD_POS | (ptEptCapCfg->byWorkmod<<EPT_MODE_POS);
    
	wCrVal=(wCrVal & ~(EPT_PSCLD_MSK))   |((ptEptCapCfg->byPscld&0x03)   <<EPT_PSCLD_POS);
	
	wCrVal=(wCrVal & ~(EPT_CAPMD_MSK))   |((ptEptCapCfg->byCaptureCapmd&0x03)   <<EPT_CAPMD_POS);
	wCrVal=(wCrVal & ~(EPT_STOPWRAP_MSK))|((ptEptCapCfg->byCaptureStopWrap&0x03)<<EPT_STOPWRAP_POS);
	wCrVal=(wCrVal & ~(EPT_CMPA_RST_MSK))|((ptEptCapCfg->byCaptureLdaret&0x01)  <<EPT_CMPA_RST_POS);
	wCrVal=(wCrVal & ~(EPT_CMPB_RST_MSK))|((ptEptCapCfg->byCaptureLdbret&0x01)  <<EPT_CMPB_RST_POS);
	wCrVal=(wCrVal & ~(EPT_CMPC_RST_MSK))|((ptEptCapCfg->byCaptureLdcret&0x01)  <<EPT_CMPC_RST_POS);
	wCrVal=(wCrVal & ~(EPT_CMPD_RST_MSK))|((ptEptCapCfg->byCaptureLddret&0x01)  <<EPT_CMPD_RST_POS);
	
	wCrVal|=EPT_CAPLD_EN;
	wCrVal|=EPT_CAPREARM;
	wPrdrLoad=0xFFFF;

    csp_ept_clken(ptEptBase);                                           // clkEN
	csp_ept_set_cr(ptEptBase, wCrVal);									// set bt work mode
	csp_ept_set_pscr(ptEptBase, (uint16_t)wClkDiv);					// clk div
	csp_ept_set_prdr(ptEptBase, (uint16_t)wPrdrLoad);				    // prdr load value
	
	if(ptEptCapCfg->wInt)
	{
		csp_ept_int_enable(ptEptBase, ptEptCapCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptEptBase);							//enable  irq
	}
	
	g_wEptPrd=wPrdrLoad;
	
	return CSI_OK;
}

/** \brief wave configuration
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptEptPwmCfg: refer to csi_ept_pwmconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_ept_wave_init(csp_ept_t *ptEptBase, csi_ept_pwmconfig_t *ptEptPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptEptPwmCfg->wFreq == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptEptBase);								// clk enable
	
	csp_ept_clken(ptEptBase);
	csp_ept_wr_key(ptEptBase);                                           //Unlocking
	csp_ept_reset(ptEptBase);											// reset 

	if(ptEptPwmCfg->byCountingMode==EPT_UPDNCNT){
		    wClkDiv = (csi_get_pclk_freq()  / ptEptPwmCfg->wFreq /2 / 30000);		// clk div value
			if(wClkDiv == 0)wClkDiv = 1;	
			wPrdrLoad  = (csi_get_pclk_freq()/ptEptPwmCfg->wFreq /2 /wClkDiv);	    //prdr load value
		
	}else{
			wClkDiv = (csi_get_pclk_freq() / ptEptPwmCfg->wFreq / 30000);		// clk div value
			if(wClkDiv == 0)wClkDiv = 1;	
			wPrdrLoad  = (csi_get_pclk_freq()/ptEptPwmCfg->wFreq/wClkDiv);	    //prdr load value
	}	
		
	wCrVal =ptEptPwmCfg->byCountingMode | (ptEptPwmCfg->byStartSrc<<EPT_STARTSRC_POS) |
	        ptEptPwmCfg->byOneshotMode<<EPT_OPMD_POS | (ptEptPwmCfg->byWorkmod<<EPT_MODE_POS);
    
	wCrVal=(wCrVal & ~(EPT_PSCLD_MSK))   |((ptEptPwmCfg->byPscld&0x03)   <<EPT_PSCLD_POS);
	

    csp_ept_clken(ptEptBase);                                           // clkEN
	csp_ept_set_cr(ptEptBase, wCrVal);									// set bt work mode
	csp_ept_set_pscr(ptEptBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_ept_set_prdr(ptEptBase, (uint16_t)wPrdrLoad);				    // prdr load value
		
	if(ptEptPwmCfg->byDutyCycle>=100){wCmpLoad=0;}
	else if(ptEptPwmCfg->byDutyCycle==0){wCmpLoad=wPrdrLoad+1;}
	else{wCmpLoad =wPrdrLoad-(wPrdrLoad * ptEptPwmCfg->byDutyCycle /100);}		
	csp_ept_set_cmpa(ptEptBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_ept_set_cmpb(ptEptBase, (uint16_t)wCmpLoad);
	csp_ept_set_cmpc(ptEptBase, (uint16_t)wCmpLoad);
	csp_ept_set_cmpd(ptEptBase, (uint16_t)wCmpLoad);
	
	
	if(ptEptPwmCfg->wInt)
	{
		csp_ept_int_enable(ptEptBase, ptEptPwmCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptEptBase);							//enable  irq
	}
	
	g_wEptPrd=wPrdrLoad;
	
	return CSI_OK;	
}

/** \brief enable/disable ept burst 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] byCgsrc:cgr src 
 *  \param[in] byCgflt:cfg flt
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_burst_enable(csp_ept_t *ptEptBase,uint8_t byCgsrc,uint8_t byCgflt, bool bEnable)
{
	csp_ept_set_burst(ptEptBase,byCgsrc,bEnable);	
	csp_ept_flt_init(ptEptBase,byCgflt,bEnable);
	return CSI_OK;
}
/** \brief Channel configuration
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptPwmCfg: refer to csi_ept_pwmchannel_config_t
 *  \param[in] eChannel: Channel label
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_channel_config(csp_ept_t *ptEptBase, csi_ept_pwmchannel_config_t *ptPwmCfg, csi_ept_channel_e eChannel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val=  ptPwmCfg -> byActionZro 
	              | ( ptPwmCfg -> byActionPrd  << EPT_ACT_PRD_POS  )
				  | ( ptPwmCfg -> byActionC1u  << EPT_ACT_C1U_POS  )
				  | ( ptPwmCfg -> byActionC1d  << EPT_ACT_C1D_POS  )
				  | ( ptPwmCfg -> byActionC2u  << EPT_ACT_C2U_POS  )
				  | ( ptPwmCfg -> byActionC2d  << EPT_ACT_C2D_POS  )
				  | ( ptPwmCfg -> byActionT1u  << EPT_ACT_T1U_POS  )
				  | ( ptPwmCfg -> byActionT1d  << EPT_ACT_T1D_POS  )
				  | ( ptPwmCfg -> byActionT2u  << EPT_ACT_T2U_POS  )
				  | ( ptPwmCfg -> byActionT2d  << EPT_ACT_T2D_POS  )
				  | ( ptPwmCfg -> byChoiceC1sel  << EPT_C1SEL_POS  )
				  | ( ptPwmCfg -> byChoiceC2sel  << EPT_C2SEL_POS  );
				  
	switch (eChannel)
	{	
		case (EPT_CHANNEL_1):csp_ept_set_aqcr1(ptEptBase,w_AQCRx_Val);
			break;
		case (EPT_CHANNEL_2):csp_ept_set_aqcr2(ptEptBase,w_AQCRx_Val);
			break;
		case (EPT_CHANNEL_3):csp_ept_set_aqcr3(ptEptBase,w_AQCRx_Val);
            break;
		case (EPT_CHANNEL_4):csp_ept_set_aqcr4(ptEptBase,w_AQCRx_Val);
		    break;
		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}
/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eLdmd: refer to csi_ept_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_ept_shdwldmd_e
 *  \param[in] eChannel: refer to csi_ept_comp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_channel_cmpload_config(csp_ept_t *ptEptBase, csi_ept_ldmd_e eLdmd, csi_ept_shdwldmd_e eShdwldmd ,csi_ept_comp_e eChannel)
{			  
	switch (eChannel)
	{	
		case (EPT_COMPA):ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPA_LD_MSK) )    |  (eLdmd    << EPT_CMPA_LD_POS);
		                     ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPA_LDTIME_MSK) )|  (eShdwldmd <<EPT_CMPA_LDTIME_POS);
			break;
		case (EPT_COMPB):ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPB_LD_MSK) )    |  (eLdmd    << EPT_CMPB_LD_POS);
		                     ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPB_LDTIME_MSK) )|  (eShdwldmd << EPT_CMPB_LDTIME_POS);
			break;
		case (EPT_COMPC):ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPC_LD_MSK) )    |  (eLdmd    << EPT_CMPC_LD_POS);
		                     ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPC_LDTIME_MSK) )|  (eShdwldmd << EPT_CMPC_LDTIME_POS);
            break;
		case (EPT_COMPD):ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPD_LD_MSK) )    |  (eLdmd    << EPT_CMPD_LD_POS);
		                     ptEptBase -> CMPLDR = (ptEptBase -> CMPLDR &~(EPT_CMPD_LDTIME_MSK) )|  (eShdwldmd << EPT_CMPD_LDTIME_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}
 /**
 \brief  Channel AQLDR configuration
 \param  ptEptBase    	pointer of ept register structure
 \param  eLdmd   	        refer to csi_ept_ldmd_e
 \param  eShdwldmd         refer to csi_ept_shdwldmd_e
 \param  channel        refer to csi_ept_channel_e
 \return CSI_OK /CSI_ERROR
*/
csi_error_t csi_ept_channel_aqload_config(csp_ept_t *ptEptBase, csi_ept_ldmd_e eLdmd, csi_ept_shdwldmd_e eShdwldmd ,csi_ept_channel_e eChannel)
{			  
	switch (eChannel)
	{	
		case (EPT_CHANNEL_1):ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_AQCR1_SHDWEN_MSK) )|  (eLdmd << EPT_AQCR1_SHDWEN_POS);
		                     ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_LDAMD_MSK) )|  (eShdwldmd << EPT_LDAMD_POS);
			break;
		case (EPT_CHANNEL_2):ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_AQCR2_SHDWEN_MSK) )|  (eLdmd << EPT_AQCR2_SHDWEN_POS);
		                     ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_LDBMD_MSK) )|  (eShdwldmd << EPT_LDBMD_POS);
			break;
		case (EPT_CHANNEL_3):ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_AQCR3_SHDWEN_MSK) )|  (eLdmd << EPT_AQCR3_SHDWEN_POS);
		                     ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_LDCMD_MSK) )|  (eShdwldmd << EPT_LDCMD_POS);
            break;
		case (EPT_CHANNEL_4):ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_AQCR4_SHDWEN_MSK) )|  (eLdmd << EPT_AQCR4_SHDWEN_POS);
		                     ptEptBase -> AQLDR = (ptEptBase -> AQLDR &~(EPT_LDDMD_MSK) )|  (eShdwldmd << EPT_LDDMD_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief DeadZoneTime configuration loading 
 *  
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eDbldr: refer to csi_ept_dbldr_e
 *  \param[in] eDbldmd: refer to csi_ept_dbldmd_e
 *  \param[in] eShdwdbldmd: refer to csi_ept_shdwdbldmd_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_dbload_config(csp_ept_t *ptEptBase, csi_ept_dbldr_e eDbldr,csi_ept_dbldmd_e eDbldmd,csi_ept_shdwdbldmd_e eShdwdbldmd)
{   uint32_t wVal;
	wVal=csp_ept_get_dbldr(ptEptBase);
	switch (eDbldr)
	{	
		case (EPT_DBCR) :wVal=( wVal &~(0x01<<EPT_DBCR_SHDWEN_POS) )|(eDbldmd << EPT_DBCR_SHDWEN_POS);
		             wVal=( wVal &~(0x03<<(EPT_DBCR_SHDWEN_POS+1)))|(eShdwdbldmd << (EPT_DBCR_SHDWEN_POS+1));
			break;
		case (EPT_DBDTR):wVal=( wVal &~(0x01<<EPT_DBDTR_SHDWEN_POS) )|(eDbldmd << EPT_DBDTR_SHDWEN_POS);
		             wVal=( wVal &~(0x03<<(EPT_DBDTR_SHDWEN_POS+1)))|(eShdwdbldmd << (EPT_DBDTR_SHDWEN_POS+1));
		    break;
		case (EPT_DBDTF):wVal=( wVal &~(0x01<<EPT_DBDTF_SHDWEN_POS) )|(eDbldmd << EPT_DBDTF_SHDWEN_POS);
		             wVal=( wVal &~(0x03<<(EPT_DBDTF_SHDWEN_POS+1)))|(eShdwdbldmd << (EPT_DBDTF_SHDWEN_POS+1));
            break;
		case (EPT_DCKPSC):wVal=( wVal &~(0x01<<EPT_DCKPSC_SHDWEN_POS))|(eDbldmd << EPT_DCKPSC_SHDWEN_POS);
		              wVal=( wVal &~(0x03<<(EPT_DCKPSC_SHDWEN_POS+1)))|(eShdwdbldmd << (EPT_DCKPSC_SHDWEN_POS+1));
		    break;
		default:return CSI_ERROR;
			break;
	}
	csp_ept_set_dbldr(ptEptBase,wVal);
			
	return CSI_OK;
}   
/** \brief DeadZoneTime configuration 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptDeadZoneCfg: refer to csi_ept_deadzone_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_dz_config(csp_ept_t *ptEptBase, csi_ept_deadzone_config_t *ptDeadZoneCfg)
{  uint32_t wVal;
   
	wVal=csp_ept_get_dbcr(ptEptBase);	
	wVal=(wVal&~(EPT_DCKSEL_MSK))|(ptDeadZoneCfg-> byDcksel <<EPT_DCKSEL_POS);
	wVal=(wVal&~(EPT_CHA_DEDB_MSK))|(ptDeadZoneCfg-> byChaDedb<<EPT_CHA_DEDB_POS);
	wVal=(wVal&~(EPT_CHB_DEDB_MSK))|(ptDeadZoneCfg-> byChbDedb<<EPT_CHB_DEDB_POS);
	wVal=(wVal&~(EPT_CHC_DEDB_MSK))|(ptDeadZoneCfg-> byChcDedb<<EPT_CHC_DEDB_POS);
	csp_ept_set_dbcr( ptEptBase, wVal);	 
	csp_ept_set_dpscr(ptEptBase	,ptDeadZoneCfg-> hwDpsc);
	
	wVal=csi_get_pclk_freq();
	wVal=(1000000000/(wVal/(ptDeadZoneCfg->hwDpsc+1)));    //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;	
	csp_ept_set_dbdtr(ptEptBase	,ptDeadZoneCfg-> hwRisingEdgeTime /wVal);
	csp_ept_set_dbdtf(ptEptBase	,ptDeadZoneCfg-> hwFallingEdgeTime/wVal);
	
	return CSI_OK;	
}
/** \brief channelmode configuration 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptDeadZoneCfg: refer to csi_ept_deadzone_config_t
 *  \param[in] eChannel: refer to csi_ept_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_channelmode_config(csp_ept_t *ptEptBase,csi_ept_deadzone_config_t *ptDeadZoneCfg,csi_ept_channel_e eChannel)
{    uint32_t wVal;
     wVal=csp_ept_get_dbcr(ptEptBase);	
	 switch (eChannel)
	{	
		case (EPT_CHANNEL_1): wVal=(wVal&~(DB_CHA_OUTSEL_MSK)) |(ptDeadZoneCfg-> byChxOuselS1S0   <<DB_CHA_OUTSEL_POS);
		                      wVal=(wVal&~(DB_CHA_POL_MSK))    |(ptDeadZoneCfg-> byChxPolarityS3S2<<DB_CHA_POL_POS);
							  wVal=(wVal&~(DB_CHA_INSEL_MSK))  |(ptDeadZoneCfg-> byChxInselS5S4   <<DB_CHA_INSEL_POS);
							  wVal=(wVal&~(DB_CHA_OUTSWAP_MSK))|(ptDeadZoneCfg-> byChxOutSwapS8S7 <<DB_CHA_OUTSWAP_POS);
			break;
		case (EPT_CHANNEL_2): wVal=(wVal&~(DB_CHB_OUTSEL_MSK)) |(ptDeadZoneCfg-> byChxOuselS1S0   <<DB_CHB_OUTSEL_POS);
		                      wVal=(wVal&~(DB_CHB_POL_MSK))    |(ptDeadZoneCfg-> byChxPolarityS3S2<<DB_CHB_POL_POS);
							  wVal=(wVal&~(DB_CHB_INSEL_MSK))  |(ptDeadZoneCfg-> byChxInselS5S4   <<DB_CHB_INSEL_POS);
							  wVal=(wVal&~(DB_CHB_OUTSWAP_MSK))|(ptDeadZoneCfg-> byChxOutSwapS8S7 <<DB_CHB_OUTSWAP_POS);		            
		    break;
		case (EPT_CHANNEL_3): wVal=(wVal&~(DB_CHC_OUTSEL_MSK)) |(ptDeadZoneCfg-> byChxOuselS1S0   <<DB_CHC_OUTSEL_POS);
		                      wVal=(wVal&~(DB_CHC_POL_MSK))    |(ptDeadZoneCfg-> byChxPolarityS3S2<<DB_CHC_POL_POS);
							  wVal=(wVal&~(DB_CHC_INSEL_MSK))  |(ptDeadZoneCfg-> byChxInselS5S4   <<DB_CHC_INSEL_POS);
							  wVal=(wVal&~(DB_CHC_OUTSWAP_MSK))|(ptDeadZoneCfg-> byChxOutSwapS8S7 <<DB_CHC_OUTSWAP_POS);
		             
            break;
		default:return CSI_ERROR;
			break;
	}
	csp_ept_set_dbcr( ptEptBase, wVal);
	return CSI_OK;
}
/** \brief Carrier output parameter configuration 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptChopperCfg: refer to csi_ept_Chopper_config_t
 *  \return none
 */
void csi_ept_chopper_config(csp_ept_t *ptEptBase, csi_ept_Chopper_config_t *ptChopperCfg)
{ 
	uint32_t wVal;
    wVal=csp_ept_get_cpcr(ptEptBase);
 	wVal= ( wVal &~ EPT_OSPWTH_MSK )     |( (ptChopperCfg -> byChopperOutOspwth&0x1f)  << EPT_OSPWTH_POS);	
	wVal= ( wVal &~ EPT_CDIV_MSK )       |( (ptChopperCfg -> byChopperOutCdiv  &0xf)   << EPT_CDIV_POS);
	wVal= ( wVal &~ EPT_CDUTY_MSK )      |( (ptChopperCfg -> byChopperOutCduty &0x7)   << EPT_CDUTY_POS);
	wVal= ( wVal &~ EPT_CP_SRCSEL_MSK )  |( (ptChopperCfg -> byChopperOutCasel &0x01)  << EPT_CP_SRCSEL_POS);
	csp_ept_set_cpcr(ptEptBase,wVal) ;  		   
}
/** \brief Carrier output
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] byCh: refer to to csi_ept_chx_e
 *  \param[in] bEn: ENABLE/DISABLE
 *  \return  CSI_OK;
 */
csi_error_t csi_ept_chopper_enable(csp_ept_t *ptEptBase, csi_ept_chx_e eCh, bool bEn)
{   
	uint32_t wVal;
	wVal=csp_ept_get_cpcr(ptEptBase);
	switch (eCh)
	{	
		case (EPTCHAX): wVal=(  wVal &~ EPT_CPEN_CHAX_MSK )|( bEn  << EPT_CPEN_CHAX_POS);
			break;
		case (EPTCHAY): wVal=(  wVal &~ EPT_CPEN_CHAY_MSK )|( bEn  << EPT_CPEN_CHAY_POS);
			break;
		case (EPTCHBX): wVal=(  wVal &~ EPT_CPEN_CHBX_MSK )|( bEn  << EPT_CPEN_CHBX_POS);
            break;
		case (EPTCHBY): wVal=(  wVal &~ EPT_CPEN_CHBY_MSK )|( bEn  << EPT_CPEN_CHBY_POS);
		    break;
		case (EPTCHCX): wVal=(  wVal &~ EPT_CPEN_CHCX_MSK )|( bEn  << EPT_CPEN_CHCX_POS);
            break;
		case (EPTCHCY): wVal=(  wVal &~ EPT_CPEN_CHCY_MSK )|( bEn  << EPT_CPEN_CHCY_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	csp_ept_set_cpcr(ptEptBase,wVal) ;  
	return CSI_OK;
}
/** \brief State of emergency configuration 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptEmCfg: refer to csi_ept_emergency_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_emergency_config(csp_ept_t *ptEptBase, csi_ept_emergency_config_t *ptEmCfg)
{ uint32_t wEmsrc;
  uint32_t wEmsrc2;
  uint32_t wEmpol;
  uint32_t wEmecr;

    wEmsrc2=csp_ept_get_src2(ptEptBase);
	wEmsrc2=(wEmsrc2 & (~EPT_EPPACE0_MSK) & (~EPT_EPPACE1_MSK) ) | (ptEmCfg -> byFltpace1 << EPT_EPPACE1_POS) | (ptEmCfg ->byFltpace0  << EPT_EPPACE0_POS);
	wEmsrc2=(wEmsrc2 &~0xff0000) |  ptEmCfg ->byOrl1 <<16;
	wEmsrc2=(wEmsrc2 &~0xff)     |  ptEmCfg ->byOrl0 ;
	csp_ept_set_src2(ptEptBase,wEmsrc2);
		
	wEmsrc = csp_ept_get_src(ptEptBase);
    wEmsrc=(  wEmsrc &~ EPT_SEL_MSK_EP(ptEmCfg -> byEpx) )|( ptEmCfg -> byEpxInt  << EPT_SEL_POS_EP(ptEmCfg -> byEpx));
    csp_ept_set_src(ptEptBase,wEmsrc);
	
    wEmpol=csp_ept_get_empol(ptEptBase);	
	 switch (ptEmCfg ->byEpxInt)
	 {    case (EBI0): wEmpol=( wEmpol  &~ POL_MSK_EBI(0)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(0) );break;
		  case (EBI1): wEmpol=( wEmpol  &~ POL_MSK_EBI(1)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(1) );break;
		  case (EBI2): wEmpol=( wEmpol  &~ POL_MSK_EBI(2)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(2) );break;
		  case (EBI3): wEmpol=( wEmpol  &~ POL_MSK_EBI(3)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(3) );break;
		  case (CMP_0): wEmpol=( wEmpol  &~ POL_MSK_EBI(4)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(4) );break;
		  case (CMP_1): wEmpol=( wEmpol  &~ POL_MSK_EBI(5)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(5) );break;
		  default:break;
	 }
	csp_ept_set_empol(ptEptBase,wEmpol);

    wEmecr =  csp_ept_get_emecr(ptEptBase);	
	wEmecr =(wEmecr & (~EPT_LCKMD_MSK_EP(ptEmCfg ->byEpx))) | (   ptEmCfg ->byEpxLckmd <<  EPT_LCKMD_POS_EP(ptEmCfg ->byEpx));
	wEmecr =(wEmecr & (~EPT_EMSOR_SHDWEN_MSK          )) | (  (ptEmCfg ->byOsrshdw&0x01) <<  EPT_EMOSR_SHDWEN_POS     );		
	csp_ept_set_emecr(ptEptBase,wEmecr);
			
	return CSI_OK;
}
/** \brief State of emergency configuration 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eOsrCh: refer to csi_ept_osrchx_e
 *  \param[in] eEmout: refer to csp_ept_emout_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_emergency_pinout(csp_ept_t *ptEptBase,csi_ept_osrchx_e  eOsrCh ,csi_ept_emout_e eEmout)
{ 
	uint32_t wEmosr;
    wEmosr=csp_ept_get_emosr(ptEptBase);	
	 switch (eOsrCh)
	 {    case (EMCOAX): wEmosr=( wEmosr &~(EPT_EMCHAX_O_MSK) )|( eEmout <<EPT_EMCHAX_O_POS);break;
		  case (EMCOBX): wEmosr=( wEmosr &~(EPT_EMCHBX_O_MSK) )|( eEmout <<EPT_EMCHBX_O_POS);break;
          case (EMCOCX): wEmosr=( wEmosr &~(EPT_EMCHCX_O_MSK) )|( eEmout <<EPT_EMCHCX_O_POS);break;
		  case (EMCOD):  wEmosr=( wEmosr &~(EPT_EMCHD_O_MSK) ) |( eEmout <<EPT_EMCHD_O_POS);break;
		  case (EMCOAY): wEmosr=( wEmosr &~(EPT_EMCHAY_O_MSK) )|( eEmout <<EPT_EMCHAY_O_POS);break;
		  case (EMCOBY): wEmosr=( wEmosr &~(EPT_EMCHBY_O_MSK) )|( eEmout <<EPT_EMCHBY_O_POS);break;
		  case (EMCOCY): wEmosr=( wEmosr &~(EPT_EMCHCY_O_MSK) )|( eEmout <<EPT_EMCHCY_O_POS);break;
		  default:return CSI_ERROR;break;
	 }
    csp_ept_set_emosr(ptEptBase,wEmosr);
	return CSI_OK;
}
/** \brief ept configuration Loading
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptGlobal: refer to csi_ept_Global_load_control_config_t
 *  \return none
 */
void csi_ept_gload_config(csp_ept_t *ptEptBase,csi_ept_Global_load_control_config_t *ptGlobal)
{   uint32_t w_GLDCR;	
	w_GLDCR = 0;
    w_GLDCR = (w_GLDCR &~EPT_GLDEN_MSK)       | ((ptGlobal->bGlden & 0x01)<<EPT_GLDEN_POS);
	w_GLDCR = (w_GLDCR &~EPT_GLDMD_MSK)       | ((ptGlobal->byGldmd & 0x0f)<<EPT_GLDMD_POS);
	w_GLDCR = (w_GLDCR &~EPT_GLDCR_OSTMD_MSK) | ((ptGlobal->bOstmd & 0x01)<<EPT_GLDCR_OSTMD_POS);
	w_GLDCR = (w_GLDCR &~EPT_GLDPRD_MSK)      | ((ptGlobal->bGldprd & 0x07)<<EPT_GLDPRD_POS);
	w_GLDCR = (w_GLDCR &~EPT_GLDCNT_MSK)      | ((ptGlobal->bGldprd & 0x07)<<EPT_GLDCNT_POS);
	csp_ept_set_gldcr(ptEptBase,w_GLDCR);	
}

/** \brief CLDCFG loading
 * 
 *  \param[in] ptEptBase：pointer of ept register structure
 *  \param[in] eGlo:  refer to csi_ept_Global_load_gldcfg_e  
 *  \param[in] bENABLE：ENABLE or DISABLE
 *  \return CSI_OK
 */
csi_error_t csi_ept_gldcfg(csp_ept_t *ptEptBase ,csi_ept_Global_load_gldcfg_e eGlo,bool bENABLE)
{
   	switch (eGlo)
	{	
		case (EPT_GLO_PRDR): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_PRDR_MSK)) |(bENABLE << EPT_LD_PRDR_POS) ;
			break;
		case (EPT_GLO_CMPA): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_CMPA_MSK)) |(bENABLE << EPT_LD_CMPA_POS) ;
			break;
		case (EPT_GLO_CMPB): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_CMPB_MSK)) |(bENABLE << EPT_LD_CMPB_POS) ;
		    break;
		case (EPT_GLO_CMPC): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_CMPC_MSK)) |(bENABLE << EPT_LD_CMPC_POS) ;
		    break;
		case (EPT_GLO_CMPD): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_CMPD_MSK)) |(bENABLE << EPT_LD_CMPD_POS) ;
		    break;	
		case (EPT_GLO_DBDTR):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_DBDTR_MSK))|(bENABLE << EPT_LD_DBDTR_POS);
		    break;
		case (EPT_GLO_DBDTF):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_DBDTF_MSK))|(bENABLE << EPT_LD_DBDTF_POS);
		    break;
		case (EPT_GLO_DBCR): ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_DBCR_MSK)) |(bENABLE << EPT_LD_DBCR_POS );
		    break;
		case (EPT_GLO_AQCR1):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_AQCRA_MSK)) |(bENABLE << EPT_LD_AQCRA_POS );
		    break;
		case (EPT_GLO_AQCR2):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_AQCRB_MSK)) |(bENABLE << EPT_LD_AQCRB_POS );
		    break;
		case (EPT_GLO_AQCR3):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_AQCRC_MSK)) |(bENABLE << EPT_LD_AQCRC_POS );
		    break;
		case (EPT_GLO_AQCR4):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_AQCRD_MSK)) |(bENABLE << EPT_LD_AQCRD_POS );
		    break;	
	    case (EPT_GLO_AQCSF):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_AQCSF_MSK)) |(bENABLE << EPT_LD_AQCSF_POS );
			 break;
		case (EPT_GLO_EMOSR):ptEptBase -> GLDCFG  = (ptEptBase -> GLDCFG & ~(EPT_LD_EMOSR_MSK)) |(bENABLE << EPT_LD_EMOSR_POS );
			 break;	 
		default: return CSI_ERROR;
			break;
	}   
	return CSI_OK;
}

/** \brief Software trigger loading
 * 
 *  \param[in] ptEptBase： pointer of ept register structure
 *  \return none
 */
void csi_ept_gload_sw(csp_ept_t *ptEptBase)
{
	csp_ept_set_gldcr2(ptEptBase,0x02);
}
/** \brief rearm  loading
 * 
 *  \param[in] ptEptBase： pointer of ept register structure
 *  \return none
 */
void csi_ept_gload_rearm(csp_ept_t *ptEptBase)
{
	csp_ept_set_gldcr2(ptEptBase,0x01);
}
/** \brief start ept
 * 
 *  \param[in] ptEptBase： pointer of ept register structure
 *  \return none
 */ 
void csi_ept_start(csp_ept_t *ptEptBase)
{   csp_ept_wr_key(ptEptBase); 
	csp_ept_start(ptEptBase);
}
/** \brief SW stop EPT counter
 * 
 *  \param[in] ptEptBase： pointer of ept register structure
 *  \return none
 */
void csi_ept_stop(csp_ept_t *ptEptBase)
{
	csp_ept_wr_key(ptEptBase);
	csp_ept_stop(ptEptBase);
}
/** \brief set EPT start mode. 
 * 
 *  \param[in] ptEptBase：pointer of ept register structure
 *  \param[in] eMode：EPT_SW/EPT_SYNC
 *  \return none
 */
void csi_ept_set_start_mode(csp_ept_t *ptEptBase, csi_ept_stmd_e eMode)
{
	csp_ept_set_start_src(ptEptBase, (csp_ept_startsrc_e)eMode);
}
/** \brief set EPT operation mode
 * 
 *  \param[in] ptEptBase：pointer of ept register structure
 *  \param[in] eMode：EPT_OP_CONT/EPT_OP_OT
 *  \return none
 */
void csi_ept_set_os_mode(csp_ept_t *ptEptBase, csi_ept_opmd_e eMode)
{
	csp_ept_set_opmd(ptEptBase, (csp_ept_opmd_e)eMode);
}

/** \brief set EPT stop status
 * 
 *  \param[in] ptEptBase :   pointer of ept register structure
 *  \param[in] eSt :	 EPT_STP_HZ/EPT_STP_LOW
 *  \return none
 */
void csi_ept_set_stop_st(csp_ept_t *ptEptBase, csi_ept_stpst_e eSt)
{	
  csp_ept_set_stop_st(ptEptBase,(csp_ept_stpst_e)eSt);
}

/** \brief get counter period to calculate the duty cycle. 
 * 
 *  \param[in] ptEptBase :   pointer of ept register structure
 *  \return counter period (reg data)
 */
uint16_t csi_ept_get_prdr(csp_ept_t *ptEptBase)
{
	return csp_ept_get_prdr(ptEptBase);
}

/** \brief  update ept PRDR and CMPx reg value
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eComp: select which COMP to set(COMPA or COMPB or COMPC or COMPD)
 *  \param[in] hwPrdr: ept PRDR reg  value
 *  \param[in] hwCmp: ept COMP reg value
 *  \return none
 */
csi_error_t csi_ept_prdr_cmp_update(csp_ept_t *ptEptBase,csi_ept_comp_e eComp, uint16_t hwPrdr, uint16_t hwCmp) 
{
	csp_ept_set_prdr(ptEptBase, (uint16_t)hwPrdr);			//set EPT PRDR Value
	switch (eComp)
	{	
		case (EPT_COMPA):
			csp_ept_set_cmpa(ptEptBase, (uint16_t)hwCmp);	//set EPT COMPA Value
			break;
			
		case (EPT_COMPB):
			csp_ept_set_cmpb(ptEptBase, (uint16_t)hwCmp);	//set EPT COMPB Value
			break;
			
		case (EPT_COMPC):
			csp_ept_set_cmpc(ptEptBase, (uint16_t)hwCmp);	//set EPT COMPC Value
		    break;
			
		case (EPT_COMPD):
			csp_ept_set_cmpd(ptEptBase, (uint16_t)hwCmp);	//set EPT COMPD Value
		    break;
			
		default: 
			return CSI_ERROR;
			break;
	}
    return (CSI_OK);
}

/** \brief change ept output dutycycle. 
 * 
 *  \param[in] ptEptBase :    pointer of ept register structure
 *  \param[in] eCh   :        refer to csi_ept_chtype_e
 *	\param[in] wDuty :  duty of PWM:0%-100%
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_change_ch_duty(csp_ept_t *ptEptBase, csi_ept_comp_e eComp, uint32_t wDuty)
{ 
	uint16_t  wCmpLoad;

	if(wDuty>=100){wCmpLoad=0;}
	else if(wDuty==0){wCmpLoad=g_wEptPrd+1;}
	else{wCmpLoad =g_wEptPrd-(g_wEptPrd * wDuty /100);}

	switch (eComp)
	{	
		case (EPT_COMPA):csp_ept_set_cmpa(ptEptBase, (uint16_t)wCmpLoad);
			break;
		case (EPT_COMPB):csp_ept_set_cmpb(ptEptBase, (uint16_t)wCmpLoad);
			break;
		case (EPT_COMPC):csp_ept_set_cmpc(ptEptBase, (uint16_t)wCmpLoad);
		    break;
		case (EPT_COMPD):csp_ept_set_cmpd(ptEptBase, (uint16_t)wCmpLoad);
		    break;
		default: return CSI_ERROR;
			break;
	}
    return CSI_OK;
}

/** \brief software force lock
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eEp: external emergency input: EPT_EPI0~7 （EBI4 = LVD）
 *  \return none
 */
void csi_ept_force_em(csp_ept_t *ptEptBase, csi_ept_ep_e eEp)
{
	csp_ept_force_em(ptEptBase,(csp_ept_ep_e)eEp);
}

/** \brief get harklock status
 * 
 *  \param[in] ptEptBase    pointer of ept register structure
 *  \return uint8_t 0 or 1
 */
uint8_t csi_ept_get_hdlck_st(csp_ept_t *ptEptBase)
{	
	return (csp_ept_get_emHdlck(ptEptBase));
}

/** \brief clear harklock status
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eEp: external emergency input: csi_ept_ep_e  
 *  \return none               
 */
void csi_ept_clr_hdlck(csp_ept_t *ptEptBase, csi_ept_ep_e eEp)
{
	csp_ept_clr_emHdlck(ptEptBase, (csp_ept_ep_e)eEp);
}

/** \brief get softlock status
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \return uint8_t 0 or 1
 */
uint8_t csi_ept_get_sftlck_st(csp_ept_t *ptEptBase)
{	
	return (csp_ept_get_emSdlck(ptEptBase));
}

/** \brief clear softlock status
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eEp: external emergency input: csi_ept_ep_e
 *  \return none
 */
void csi_ept_clr_sftlck(csp_ept_t *ptEptBase, csi_ept_ep_e eEp)
{	
	csp_ept_clr_emSdlck(ptEptBase,  (csp_ept_ep_e)eEp);
}

/** \brief enable/disable ept in debug mode
 * 
 *  \param[in]  ptEptBase      pointer of ept register structure
 *  \param[in]   bEnable		ENABLE/DISABLE
 *  \return none
 */
void csi_ept_debug_enable(csp_ept_t *ptEptBase, bool bEnable)
{
	csp_ept_dbg_enable(ptEptBase, bEnable);
}

/** \brief enable/disable ept emergencyinterruption
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eEbi: refer to csi_ept_emint_e
 *  \return none
 */
void csi_ept_emint_en(csp_ept_t *ptEptBase, csi_ept_emint_e eEm)
{   csi_irq_enable((uint32_t *)ptEptBase);		//enable  irq
    csp_ept_Emergency_emimcr(ptEptBase,(csp_ept_emint_e)eEm);
}

/** \brief enable/disable ept out trigger 
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eTrgOut: ept evtrg out port (0~3)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_evtrg_enable(csp_ept_t *ptEptBase, csi_ept_trgout_e eTrgOut, bool bEnable)
{	
	if (eTrgOut > EPT_TRGOUT3)
		return CSI_ERROR;
		
    csp_ept_trg_xoe_enable(ptEptBase, eTrgOut, bEnable);
	return CSI_OK;
}

/** \brief   One time software output 
 * 
 *  \param[in]   ptEptBase: pointer of ept register structure 
 *  \param[in]   hwCh: EPT_OSTSFA/EPT_OSTSFB/EPT_OSTSFC/EPT_OSTSFD		
 *  \param[in]   eAction: refer to csi_ept_action_e	
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_onetime_software_output(csp_ept_t *ptEptBase, uint16_t hwCh, csi_ept_action_e eAction)
{	
	switch (hwCh){
	case EPT_OSTSF1: ptEptBase ->AQOSF |= EPT_OSTSF1;
	                 ptEptBase ->AQOSF = (ptEptBase ->AQOSF &~(EPT_ACT1_MSK))|((eAction&0x03)<<EPT_ACT1_POS);
	     break;
	case EPT_OSTSF2: ptEptBase ->AQOSF |= EPT_OSTSF2;
	                 ptEptBase ->AQOSF = (ptEptBase ->AQOSF &~(EPT_ACT2_MSK))|((eAction&0x03)<<EPT_ACT2_POS);
	     break;	
    case EPT_OSTSF3: ptEptBase ->AQOSF |= EPT_OSTSF3;
	                 ptEptBase ->AQOSF = (ptEptBase ->AQOSF &~(EPT_ACT3_MSK))|((eAction&0x03)<<EPT_ACT3_POS);
	     break;
	case EPT_OSTSF4: ptEptBase ->AQOSF |= EPT_OSTSF4;
	                 ptEptBase ->AQOSF = (ptEptBase ->AQOSF &~(EPT_ACT4_MSK))|((eAction&0x03)<<EPT_ACT4_POS);
	     break;
	default: return CSI_ERROR;
	     break;
    }
	return CSI_OK;
}
/** \brief  Continuous software waveform loading control
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eLoadtime:    refer to csi_ept_aqosf_e
 *  \return  none
 */
void csi_ept_aqcsfload_config(csp_ept_t *ptEptBase, csi_ept_aqosf_e eLoadtime)
{
	ptEptBase ->AQOSF  = (ptEptBase ->AQOSF &~(EPT_AQCSF_LDTIME_MSK))|((eLoadtime&0x03)<<EPT_AQCSF_LDTIME_POS);
}
/** \brief Continuous software waveform control
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] byCh: refer to csi_ept_channel_e
 *  \param[in] eAction: refer to  csi_ept_aqosf_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_continuous_software_output(csp_ept_t *ptEptBase, csi_ept_channel_e eChannel, csi_ept_aqcsf_e eAction)
{
	
	switch (eChannel){
	case EPT_CHANNEL_1:  ptEptBase ->AQCSF = (ptEptBase ->AQCSF &~(0x03))|(eAction&0x03);            
	     break;
	case EPT_CHANNEL_2:  ptEptBase ->AQCSF = (ptEptBase ->AQCSF &~(0x0c))|(eAction&0x03)<<2;
	     break;	
    case EPT_CHANNEL_3:  ptEptBase ->AQCSF = (ptEptBase ->AQCSF &~(0x30))|(eAction&0x03)<<4;
	     break;
	case EPT_CHANNEL_4:  ptEptBase ->AQCSF = (ptEptBase ->AQCSF &~(0xc0))|(eAction&0x03)<<6;
	     break;
	default: return CSI_ERROR;
	     break;
    }		
	return CSI_OK;
}

/** \brief ept  int  config 
 *
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eInt: refer to to csi_ept_intsrc_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none;
 */
void csi_ept_int_enable(csp_ept_t *ptEptBase, csi_ept_intsrc_e eInt, bool bEnable)
{  
	csp_ept_int_enable(ptEptBase,(csp_ept_int_e)eInt,bEnable);
	csi_irq_enable((uint32_t *)ptEptBase);							//enable  irq
}

/** \brief ept sync input evtrg config  
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eTrgin: ept sync evtrg input channel(0~5)
 *  \param[in] eTrgMode: ept sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: refer to csi_ept_arearm_e 
 *  \return none
 */
void csi_ept_set_sync(csp_ept_t *ptEptBase, csi_ept_trgin_e eTrgIn, csi_ept_trgmode_e eTrgMode, csi_ept_arearm_e eAutoRearm)
{
	csp_ept_set_sync_mode(ptEptBase, eTrgIn, (csp_ept_syncmd_e)eTrgMode);
	csp_ept_set_auto_rearm(ptEptBase, (csp_ept_arearm_e)eAutoRearm);
	csp_ept_sync_enable(ptEptBase, eTrgIn, ENABLE);
}

/** \brief ept sync -> evtrv config
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eTrgin: ept sync evtrg input channel(0~5)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_set_sync2evtrg(csp_ept_t *ptEptBase, csi_ept_trgin_e eTrgIn, uint8_t byTrgChx)
{
	switch(byTrgChx)
	{
		case 0:
			csp_ept_sync_trg0sel(ptEptBase, eTrgIn);
			break;
		case 1:
			csp_ept_sync_trg1sel(ptEptBase, eTrgIn);
			break;
		default:
			return CSI_ERROR;
		
	}
	return CSI_OK;
}

/** \brief ept sync input filter config  
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] ptFilter: pointer of sync input filter parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_set_sync_filter(csp_ept_t *ptEptBase, csi_ept_filter_config_t *ptFilter)
{
	uint32_t wFiltCr;
	uint32_t wWindow;
	
	if(ptFilter->byFiltSrc > EPT_FILT_SYNCIN5)
		return CSI_ERROR;
	wFiltCr = ptFilter->byFiltSrc | (ptFilter->byWinInv << EPT_FLTBLKINV_POS) | 
			(ptFilter->byWinAlign << EPT_ALIGNMD_POS) | (ptFilter->byWinCross << EPT_CROSSMD_POS);
	wWindow = ptFilter->hwWinOffset | (ptFilter->hwWinWidth << EPT_FLT_WDW_POS);
	
	csp_ept_set_trgftcr(ptEptBase, wFiltCr);
	csp_ept_set_trgftwr(ptEptBase, wWindow);
	
	return CSI_OK;
}
/** \brief rearm ept sync evtrg  
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eTrgin: ept sync evtrg input channel(0~5)
 *  \return none
 */
void csi_ept_rearm_sync(csp_ept_t *ptEptBase,csi_ept_trgin_e eTrgin)
{
	csp_ept_rearm_sync(ptEptBase, eTrgin);
}
/** \brief ept evtrg output config
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] eTrgOut: evtrg out port(0~3)
 *  \param[in] eTrgSrc: evtrg source(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_set_evtrg(csp_ept_t *ptEptBase, csi_ept_trgout_e eTrgOut, csi_ept_trgsrc_e eTrgSrc)
{
	switch (eTrgOut)
	{
		case EPT_TRGOUT0:
		case EPT_TRGOUT1: 
			if(eTrgSrc == EPT_TRGSRC_PEND)
				return CSI_ERROR;
			if(eTrgSrc == EPT_TRGSRC_DIS)								
			{
				csp_ept_trg_xoe_enable(ptEptBase, eTrgOut, DISABLE);	//disable evtrg source out
				return CSI_OK;
			}
			csp_ept_set_trgsrc01(ptEptBase, eTrgOut, (csp_ept_trgsrc01_e)eTrgSrc);
			break;
		case EPT_TRGOUT2:
		case EPT_TRGOUT3: 
			if(eTrgSrc == EPT_TRGSRC_EX)
				return CSI_ERROR;
			if(eTrgSrc == EPT_TRGSRC_DIS)								
			{
				csp_ept_trg_xoe_enable(ptEptBase, eTrgOut, DISABLE);	//disable evtrg source out
				return CSI_OK;
			}
			if (eTrgSrc == EPT_TRGSRC_PEND)
				eTrgSrc = 12;
			csp_ept_set_trgsrc23(ptEptBase, eTrgOut, (csp_ept_trgsrc23_e)eTrgSrc);
			break;
		default: 
			return CSI_ERROR;
	}
	
	csp_ept_trg_xoe_enable(ptEptBase, eTrgOut, ENABLE);					//evtrg out enable
	
	return CSI_OK;
}
/** \brief ept evtrg cntxinit control
 * 
 *  \param[in] ptEptBase: pointer of ept register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~3)
 *  \param[in] byCntVal: evtrg cnt value
 *  \param[in] byCntInitVal: evtrg cntxinit value
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_ept_set_evcntinit(csp_ept_t *ptEptBase, uint8_t byCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{

 if(byCntChx > EPT_CNT3INIT)
  return CSI_ERROR;
 
 csp_ept_set_trgprd(ptEptBase, byCntChx, byCntVal - 1);    //evtrg count
 csp_ept_trg_cntxinit(ptEptBase, byCntChx, byCntInitVal);
 csp_ept_trg_cntxiniten_enable(ptEptBase, byCntChx, ENABLE);
 
 return CSI_OK;
}

/** \brief  ept Link configuration
 * 
 *  \param[in] ptEptBase    	pointer of ept register structure
 *  \param[in] ptReglk           refer to csi_ept_reglk_config_t
 *  \return error code \ref csi_error_t
*/
void csi_ept_reglk_config(csp_ept_t *ptEptBase,csi_ept_reglk_config_t *ptReglk)
{   uint32_t w_GLK;	
	w_GLK =0;
    w_GLK = (w_GLK & ~EPT_PRDR_MSK )| ((ptReglk-> byPrdr & 0xF)<< EPT_PRDR_POS);
	w_GLK = (w_GLK & ~EPT_CMPA_MSK )| ((ptReglk-> byCmpa & 0xF)<< EPT_CMPA_POS);
	w_GLK = (w_GLK & ~EPT_CMPB_MSK )| ((ptReglk-> byCmpb & 0xF)<< EPT_CMPB_POS);
	w_GLK = (w_GLK & ~EPT_GLD2_MSK )| ((ptReglk-> byGld2 & 0xF)<< EPT_GLD2_POS);
	w_GLK = (w_GLK & ~EPT_RSSR_MSK )| ((ptReglk-> byRssr & 0xF)<< EPT_RSSR_POS);
	csp_ept_set_reglk(ptEptBase,w_GLK);
	w_GLK =0;
	w_GLK = (w_GLK & ~EPT_EMSLCLR_MSK )| ((ptReglk-> byEmslclr & 0xF)<< EPT_EMSLCLR_POS);
	w_GLK = (w_GLK & ~EPT_EMHLCLR_MSK )| ((ptReglk-> byEmhlclr & 0xF)<< EPT_EMHLCLR_POS);
	w_GLK = (w_GLK & ~EPT_EMICR_MSK )  | ((ptReglk-> byEmicr   & 0xF)<< EPT_EMICR_POS);
	w_GLK = (w_GLK & ~EPT_EMFRCR_MSK ) | ((ptReglk-> byEmfrcr  & 0xF)<< EPT_EMFRCR_POS);
	w_GLK = (w_GLK & ~EPT_AQOSF_MSK )  | ((ptReglk-> byAqosf   & 0xF)<< EPT_AQOSF_POS);
	w_GLK = (w_GLK & ~EPT_AQCSF_MSK )  | ((ptReglk-> byAqcsf   & 0xF)<< EPT_AQCSF_POS);
	csp_ept_set_reglk2(ptEptBase,w_GLK);	
}


