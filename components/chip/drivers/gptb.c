/***********************************************************************//** 
 * \file  gptb.c
 * \brief  Enhanced purpose timer driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-1-17  <td>V0.0  <td>ljy     <td>initial
 * <tr><td> 2023-5-10  <td>V0.1 <td>wch     <td>modify
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/gptb.h>
#include <csp_gptb.h>
#include <drv/irq.h>

uint32_t g_wGptb0Prd;

/** \brief capture configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptGptbCapCfg: refer to csi_ept_captureconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_captureconfig_t *ptGptbCapCfg)
{
	uint32_t wClkDiv=0;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
			
	csi_clk_enable((uint32_t *)ptGptbBase);								 // clk enable
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);	                                     // Unlocking	
	csp_gptb_reset(ptGptbBase);											 // reset 
			                                                             // clk div value
	if(wClkDiv == 0){wClkDiv = 1;}
					
	wCrVal =ptGptbCapCfg->byCountingMode | (ptGptbCapCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptGptbCapCfg->byOneshotMode<<GPTB_OPMD_POS | (ptGptbCapCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptGptbCapCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptGptbCapCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
	wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptGptbCapCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptGptbCapCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptGptbCapCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CMPAA_RST_MSK))|((ptGptbCapCfg->byCaptureLdaaret&0x01)  <<GPTB_CMPAA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPBA_RST_MSK))|((ptGptbCapCfg->byCaptureLdbaret&0x01)  <<GPTB_CMPBA_RST_POS);

	wCrVal|=GPTB_CAPLD_EN;//CAPMODE_SEL
	wCrVal|=GPTB_CAPREARM;
	wPrdrLoad=0xFFFF;

    csp_gptb_clken(ptGptbBase);                                         // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);								// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
	
	if(ptGptbCapCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptGptbCapCfg->wInt, true);   //enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	g_wGptb0Prd=wPrdrLoad;
	
	return CSI_OK;
}

/** \brief wave configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptGptbPwmCfg: refer to csi_ept_pwmconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_gptb_wave_init(csp_gptb_t *ptGptbBase, csi_gptb_pwmconfig_t *ptGptbPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptGptbPwmCfg->wFreq == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable   GPTB_UPDNCNT
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	if(ptGptbPwmCfg->byCountingMode==GPTB_UPDNCNT){
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq /2/ 30000);		// clk div value		
		if(wClkDiv == 0)wClkDiv = 1;	
		wPrdrLoad  = (csi_get_pclk_freq()/ptGptbPwmCfg->wFreq/2/wClkDiv);	    //prdr load value
	}else{
		wClkDiv = (csi_get_pclk_freq() / ptGptbPwmCfg->wFreq / 30000);		// clk div value		
		if(wClkDiv == 0)wClkDiv = 1;	
		wPrdrLoad  = (csi_get_pclk_freq()/ptGptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
	}
		
	wCrVal =ptGptbPwmCfg->byCountingMode | (ptGptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptGptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptGptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptGptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	
    csp_gptb_clken(ptGptbBase);                                           // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
		
	if(ptGptbPwmCfg->byDutyCycle){
	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptGptbPwmCfg->byDutyCycle /100);	// cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);

	}
	
	if(ptGptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptGptbPwmCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	g_wGptb0Prd=wPrdrLoad;
	
	return CSI_OK;	
}


/** \brief initialize gpta data structure
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] wTimeOut: the timeout for gpta, unit: us, 20us < wTimeOut < 3S
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_gptb_timer_init(csp_gptb_t *ptGptbBase, uint32_t wTimeOut)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wPrdrLoad; 
	
	if(wTimeOut == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	
	apt_timer_set_load_value(wTimeOut,TIMER_16BIT_MODE);
	wPrdrLoad = apt_timer_get_prdrload_value();
	wClkDiv = apt_timer_get_clkdiv_value();
	
	wCrVal =GPTB_UPCNT | (GPTB_SYNC_START<<GPTB_STARTSRC_POS) | (GPTB_WAVE<<GPTB_MODE_POS);
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((GPTB_LDPSCR_ZRO&0x03)   <<GPTB_PSCLD_POS);	

    csp_gptb_clken(ptGptbBase);                                         // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);								// set gpta work mode
	csi_gptb_count_mode(ptGptbBase, GPTB_OP_CONT);                      // gpta count mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value

	csp_gptb_int_enable(ptGptbBase, GPTB_INT_PEND, true);		        //enable interrupt
	csi_irq_enable((uint32_t *)ptGptbBase);							    //enable  irq
	
	return CSI_OK;					
}

/** \brief set gpta count mode
 * 
 *  \param[in] ptGptaBase: pointer of gpta register structure
 *  \param[in] eCntMode: gpta count mode, one pulse/continuous
 *  \return none
 */ 
void csi_gptb_count_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eCntMode)
{
	csp_gptb_set_opmd(ptGptbBase, (csp_gptb_opmd_e)eCntMode);
}

/** \brief enable/disable gptb burst 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCgsrc:cgr src 
 *  \param[in] byCgflt:cfg flt
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_burst_enable(csp_gptb_t *ptGptbBase,uint8_t byCgsrc,uint8_t byCgflt, bool bEnable)
{
	csp_gptb_set_burst(ptGptbBase,byCgsrc,bEnable);	
	csp_gptb_flt_init(ptGptbBase,byCgflt,bEnable);
	return CSI_OK;
}

/** \brief Channel CMPLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of ept register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_camp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_cmpload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_comp_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_COMPA):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LD_MSK) )    |  (eLdmd    << GPTB_CMPA_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LDTIME_MSK) )|  (eShdwldmd <<GPTB_CMPA_LDTIME_POS);
			break;
		case (GPTB_COMPB):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LD_MSK) )    |  (eLdmd    << GPTB_CMPB_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LDTIME_MSK) )|  (eShdwldmd << GPTB_CMPB_LDTIME_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief Channel configuration
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptPwmCfg: refer to csi_gptb_pwmchannel_config_t
 *  \param[in] eChannel:  refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_config(csp_gptb_t *ptGptbBase, csi_gptb_pwmchannel_config_t *ptPwmCfg, csi_gptb_channel_e eChannel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val=  ptPwmCfg -> byActionZro 
	              | ( ptPwmCfg -> byActionPrd  << GPTB_ACT_PRD_POS  )
				  | ( ptPwmCfg -> byActionC1u  << GPTB_ACT_C1U_POS  )
				  | ( ptPwmCfg -> byActionC1d  << GPTB_ACT_C1D_POS  )
				  | ( ptPwmCfg -> byActionC2u  << GPTB_ACT_C2U_POS  )
				  | ( ptPwmCfg -> byActionC2d  << GPTB_ACT_C2D_POS  )
				  | ( ptPwmCfg -> byActionT1u  << GPTB_ACT_T1U_POS  )
				  | ( ptPwmCfg -> byActionT1d  << GPTB_ACT_T1D_POS  )
				  | ( ptPwmCfg -> byActionT2u  << GPTB_ACT_T2U_POS  )
				  | ( ptPwmCfg -> byActionT2d  << GPTB_ACT_T2D_POS  )
				  | ( ptPwmCfg -> byChoiceC1sel  << GPTB_C1SEL_POS  )
				  | ( ptPwmCfg -> byChoiceC2sel  << GPTB_C2SEL_POS  );
				  
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):csp_gptb_set_aqcra(ptGptbBase,w_AQCRx_Val);
			break;
		case (GPTB_CHANNEL_2):csp_gptb_set_aqcrb(ptGptbBase,w_AQCRx_Val);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief Channel AQLDR configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLdmd: refer to csi_gptb_ldmd_e
 *  \param[in] eShdwldmd: refer to csi_gptb_shdwldmd_e
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_aqload_config(csp_gptb_t *ptGptbBase, csi_gptb_ldmd_e eLdmd, csi_gptb_shdwldmd_e eShdwldmd ,csi_gptb_channel_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR1_SHDWEN_MSK) )|  (eLdmd << GPTB_AQCR1_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDAMD_MSK) )|  (eShdwldmd << GPTB_LDAMD_POS);
			break;
		case (GPTB_CHANNEL_2):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR2_SHDWEN_MSK) )|  (eLdmd << GPTB_AQCR2_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDBMD_MSK) )|  (eShdwldmd << GPTB_LDBMD_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief DeadZoneTime configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptDeadZoneCfg: refer to csi_gptb_deadzone_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_dz_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *ptDeadZoneCfg)
{  uint32_t wVal;
   
	wVal=csp_gptb_get_dbcr(ptGptbBase);	
	wVal=(wVal&~(GPTB_DCKSEL_MSK))  |(ptDeadZoneCfg-> byDcksel <<GPTB_DCKSEL_POS);
	wVal=(wVal&~(GPTB_CHA_DEDB_MSK))|(ptDeadZoneCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
//	wVal=(wVal&~(GPTB_CHB_DEDB_MSK))|(ptDeadZoneCfg-> byChbDedb<<GPTB_CHB_DEDB_POS);
//	wVal=(wVal&~(GPTB_CHC_DEDB_MSK))|(ptDeadZoneCfg-> byChcDedb<<GPTB_CHC_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, wVal);	 
	csp_gptb_set_dpscr(ptGptbBase, ptDeadZoneCfg-> hwDpsc);
	
	wVal=csi_get_pclk_freq();
	wVal=(1000000000/(wVal/(ptDeadZoneCfg->hwDpsc+1)));    //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;	
	csp_gptb_set_dbdtr(ptGptbBase	,ptDeadZoneCfg-> hwRisingEdgereGister /wVal);
	csp_gptb_set_dbdtf(ptGptbBase	,ptDeadZoneCfg-> hwFallingEdgereGister/wVal);
	
	return CSI_OK;	
}
/** \brief DeadZoneTime configuration loading 
 *  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eDbldr: refer to csi_gptb_dbldr_e
 *  \param[in] eDbldmd: refer to csi_gptb_dbldmd_e
 *  \param[in] eShdwdbldmd: refer to csi_gptb_shdwdbldmd_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_dbload_config(csp_gptb_t *ptGptbBase, csi_gptb_dbldr_e eDbldr,csi_gptb_dbldmd_e eDbldmd,csi_gptb_shdwdbldmd_e eShdwdbldmd)
{   uint32_t wVal;
	wVal=csp_gptb_get_dbldr(ptGptbBase);
	switch (eDbldr)
	{	
		case (GPTB_DBCR) :wVal=( wVal &~(GPTB_DBLDR_CRSHDWEN_MSK) )|(eDbldmd << GPTB_DBLDR_CRSHDWEN_POS);
		             wVal=( wVal &~(GPTB_DBLDR_LDCRMODE_MSK))|(eShdwdbldmd << GPTB_DBLDR_LDCRMODE_POS);
			break;
		case (GPTB_DBDTR):wVal=( wVal &~GPTB_DBLDR_SHDWDTR_MSK )|(eDbldmd << GPTB_DBLDR_SHDWDTR_POS );
		             wVal=( wVal &~GPTB_DBLDR_LDDTRMD_MSK )|(eShdwdbldmd << GPTB_DBLDR_LDDTRMD_POS);
		    break;
		case (GPTB_DBDTF):wVal=( wVal &~GPTB_DBLDR_SHDWDTF_MSK )|(eDbldmd << GPTB_DBLDR_SHDWDTF_POS);
		             wVal=( wVal &~GPTB_DBLDR_LDDTFMD_MSK )|(eShdwdbldmd <<GPTB_DBLDR_LDDTFMD_POS);
            break;
		case (GPTB_DCKPSC):wVal=( wVal &~GPTB_DBLDR_SHDWPSC_MSK)|(eDbldmd << GPTB_DBLDR_SHDWPSC_POS);
		              wVal=( wVal &~GPTB_DBLDR_LDPSCMD_MSK)|(eShdwdbldmd << GPTB_DBLDR_LDPSCMD_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbldr(ptGptbBase,wVal);
			
	return CSI_OK;
}   

/** \brief channelmode configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptDeadZoneCfg: refer to csi_gptb_deadzone_config_t
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channelmode_config(csp_gptb_t *ptGptbBase,csi_gptb_deadzone_config_t *ptDeadZoneCfg,csi_gptb_channel_e eChannel)
{    uint32_t wVal;
     wVal=csp_gptb_get_dbcr(ptGptbBase);	
	 switch (eChannel)
	{	
		case (GPTB_CHANNEL_1): wVal=(wVal&~(DB_CHA_OUTSEL_MSK)) |(ptDeadZoneCfg-> byChxOuselS1S0   <<DB_CHA_OUTSEL_POS);
		                      wVal=(wVal&~(DB_CHA_POL_MSK))    |(ptDeadZoneCfg-> byChxPolarityS3S2<<DB_CHA_POL_POS);
							  wVal=(wVal&~(DB_CHA_INSEL_MSK))  |(ptDeadZoneCfg-> byChxInselS5S4   <<DB_CHA_INSEL_POS);
							  wVal=(wVal&~(DB_CHA_OUTSWAP_MSK))|(ptDeadZoneCfg-> byChxOutSwapS8S7 <<DB_CHA_OUTSWAP_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbcr( ptGptbBase, wVal);
	return CSI_OK;
}

/** \brief State of emergency configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptEmCfg: refer to csi_gptb_emergency_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_emergency_cfg(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *ptEmCfg)
{ uint32_t wEmsrc;
  uint32_t wEmsrc2;
  uint32_t wEmpol;
  uint32_t wEmecr;

    wEmsrc2=csp_gptb_get_src2(ptGptbBase);
	wEmsrc2=(wEmsrc2 & (~GPTB_EMSRC2_FLT_PACE0_MSK)) | (ptEmCfg ->byFltpace0  << GPTB_EMSRC2_FLT_PACE0_POS);
	wEmsrc2=(wEmsrc2 &~0xff0000) |  ptEmCfg ->byOrl1 <<16;
	wEmsrc2=(wEmsrc2 &~0xff)     |  ptEmCfg ->byOrl0 ;
	csp_gptb_set_src2(ptGptbBase,wEmsrc2);
		
	wEmsrc = csp_gptb_get_src(ptGptbBase);
    wEmsrc=(  wEmsrc &~ GPTB_SEL_MSK_EP(ptEmCfg -> byEpx) )|( ptEmCfg -> byEpxInt  << GPTB_SEL_POS_EP(ptEmCfg -> byEpx));
    csp_gptb_set_src(ptGptbBase,wEmsrc);
	
    wEmpol=csp_gptb_get_empol(ptGptbBase);	
	 switch (ptEmCfg ->byEpxInt)
	 {    case (GPTB_EBI0): wEmpol=( wEmpol  &~ POL_MSK_EBI(0)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(0) );break;
		  case (GPTB_EBI1): wEmpol=( wEmpol  &~ POL_MSK_EBI(1)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(1) );break;
		  case (GPTB_EBI2): wEmpol=( wEmpol  &~ POL_MSK_EBI(2)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(2) );break;
		  case (GPTB_EBI3): wEmpol=( wEmpol  &~ POL_MSK_EBI(3)) | (ptEmCfg -> byPolEbix <<POL_POS_EBI(3) );break;
		  default:break;
	 }
	csp_gptb_set_empol(ptGptbBase,wEmpol);

    wEmecr =  csp_gptb_get_emecr(ptGptbBase);	
	wEmecr =(wEmecr & (~GPTB_LCKMD_MSK_EP(ptEmCfg ->byEpx))) | (   ptEmCfg ->byEpxLckmd <<  GPTB_LCKMD_POS_EP(ptEmCfg ->byEpx));
	csp_gptb_set_emecr(ptGptbBase,wEmecr);
			
	return CSI_OK;
}
/** \brief State of emergency configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eOsrch: refer to csi_gptb_osrchx_e
 *  \param[in] eEmout: refer to csi_gptb_emout_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_emergency_pinxout(csp_gptb_t *ptGptbBase,csi_gptb_osrchx_e  eOsrch ,csi_gptb_emout_e eEmout)
{ uint32_t wEmosr;
    wEmosr=csp_gptb_get_emosr(ptGptbBase);	
	 switch (eOsrch)
	 {    case (GPTB_EMCOAX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAX_MSK) )|( eEmout <<GPTB_EMOSR_EM_COAX_POS);break;
		  case (GPTB_EMCOBX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COBX_MSK) )|( eEmout <<GPTB_EMOSR_EM_COBX_POS);break;
		  case (GPTB_EMCOAY): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAY_MSK) )|( eEmout <<GPTB_EMOSR_EM_COAY_POS);break;		
		  default:return CSI_ERROR;break;
	 }
    csp_gptb_set_emosr(ptGptbBase,wEmosr);
	return CSI_OK;
}

/** \brief gptb configuration Loading
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptGloCfg: refer to csi_gptb_Global_load_control_config_t
 *  \return none
 */
void csi_gptb_global_config(csp_gptb_t *ptGptbBase,csi_gptb_Global_load_control_config_t *ptGloCfg)
{   uint32_t wGldcr;	
	wGldcr =0;
    wGldcr = (wGldcr &~GPTB_GLDEN_MSK)       | ((ptGloCfg->bGlden & 0x01)<<GPTB_GLDEN_POS);
	wGldcr = (wGldcr &~GPTB_GLDMD_MSK)       | ((ptGloCfg->byGldmd & 0x0f)<<GPTB_GLDMD_POS);
	wGldcr = (wGldcr &~GPTB_GLDCR_OSTMD_MSK) | ((ptGloCfg->bOstmd & 0x01)<<GPTB_GLDCR_OSTMD_POS);
	wGldcr = (wGldcr &~GPTB_GLDPRD_MSK)      | ((ptGloCfg->bGldprd & 0x07)<<GPTB_GLDPRD_POS);
	csp_gptb_set_gldcr(ptGptbBase,wGldcr);	

}

/** \brief CLDCFG loading
 * 
 *  \param[in] ptGptbBase of gptb register structure
 *  \param[in] eGlo:  csi_gptb_Global_load_gldcfg  
 *  \param[in] bEnable or DISABLE
 *  \return CSI_OK
 */
csi_error_t csi_gptb_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_Global_load_gldcfg eGlo,bool bEnable)
{
   	switch (eGlo)
	{	
		case (GPTB_PRDR): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_PRDR_MSK)) |(bEnable << GPTB_LD_PRDR_POS) ;
			break;
		case (GPTB_CMPA): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_CMPA_MSK)) |(bEnable << GPTB_LD_CMPA_POS) ;
			break;
		case (GPTB_CMPB): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_CMPB_MSK)) |(bEnable << GPTB_LD_CMPB_POS) ;
		    break;
		case (GPTB_AQCR1):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCRA_MSK)) |(bEnable << GPTB_LD_AQCRA_POS );
		    break;
		case (GPTB_AQCR2):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCRB_MSK)) |(bEnable << GPTB_LD_AQCRB_POS );
		    break;
	    case (GPTB_AQCSF):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCSF_MSK)) |(bEnable << GPTB_LD_AQCSF_POS );
			 break;
		default: return CSI_ERROR;
			break;
	}   
	return CSI_OK;
}



/** \brief Software trigger loading
 * 
 *  \param[in] ptGptbBase pointer of gptb register structure
 *  \return none
*/
void csi_gptb_global_sw(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_SW_GLD);
}

/** \brief rearm  loading
 * 
 *  \param[in] ptGptbBase pointer of gptb register structure
 *  \return none
*/
void csi_gptb_global_rearm(csp_gptb_t *ptGptbBase)
{
	csp_gptb_set_gldcr2(ptGptbBase,GPTB_OSREARM_EN);
}
/** \brief start gptb
 * 
 *  \param[in] ptGptbBase pointer of gptb register structure
 *  \return none
*/
void csi_gptb_start(csp_gptb_t *ptGptbBase)
{   
	csp_gptb_wr_key(ptGptbBase); 
	csp_gptb_start(ptGptbBase);
}
/** \brief SW stop GPTB counter
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \return none
*/
void csi_gptb_stop(csp_gptb_t *ptGptbBase)
{
	csp_gptb_wr_key(ptGptbBase);
	csp_gptb_stop(ptGptbBase);
}
/** \brief set gptb start mode. 
 * 
 *  \param[in] ptGptbBase ：pointer of gptb register structure
 *  \param[in] eMode GPTB_SW/GPTB_SYNC
 *  \return none
 */
void csi_gptb_set_start_mode(csp_gptb_t *ptGptbBase, csi_gptb_stmd_e eMode)
{
	csp_gptb_set_start_src(ptGptbBase, (csp_gptb_startsrc_e)eMode);
}
/** \brief set gptb operation mode
 * 
 *  \param[in] ptGptbBase ：pointer of gptb register structure
 *  \param[in] eMode ： GPTB_OP_CONT/GPTB_OP_OT
 *  \return none
 */
void csi_gptb_set_os_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eMode)
{
	csp_gptb_set_opmd(ptGptbBase, (csp_gptb_opmd_e)eMode);
}
/** \brief set gptb stop status
 * 
 *  \param[in] ptGptbBase :   pointer of gptb register structure
 *  \param[in] eSt 	 GPTB_STPST_HZ/GPTB_STPST_LOW
 *  \return none
 */
void csi_gptb_set_stop_st(csp_gptb_t *ptGptbBase, csi_gptb_stpst_e eSt)
{	
  csp_gptb_set_stop_st(ptGptbBase,(csp_gptb_stpst_e)eSt);
}

/** \brief get counter period to calculate the duty cycle. 
 * 
 *  \param[in] ptGptbBase  :  pointer of gptb register structure
 *  \return counter period (reg data)
 */
uint16_t csi_gptb_get_prdr(csp_gptb_t *ptGptbBase)
{
	return csp_gptb_get_prdr(ptGptbBase);
}

/** \brief set gptb phsr  
 * 
 *  \param[in] ptGptbBase ：pointer of gptb register structure
 *  \param[in] hwPhsr  :    phsr value
*   \param[in] bEnable :    ENABLE/DISABLE
 *  \return none
 */
void csi_gptb_set_phsr(csp_gptb_t *ptGptbBase, uint16_t hwPhsr,bool bEnable)
{
	csp_gptb_set_phsr(ptGptbBase, hwPhsr);
	csp_gptb_phsen_enable(ptGptbBase, bEnable);
}

/** \brief change gptb output dutycycle. 
 * 
 *  \param[in] ptGptbBase :    pointer of gptb register structure
 *  \param[in] eCh  :          refer to csi_gptb_comp_e
 *	\param[in] wDuty :   duty of PWM:0%-100%
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_change_ch_duty(csp_gptb_t *ptGptbBase, csi_gptb_comp_e eCh, uint32_t wDuty)
{ uint16_t  hwCmpLoad;
  uint16_t  hwPrd;
    hwPrd=csp_gptb_get_prd(ptGptbBase);
	if(wDuty>=100){hwCmpLoad=0;}
	else if(wDuty==0){hwCmpLoad=hwPrd+1;}
	else{hwCmpLoad =hwPrd-(hwPrd * wDuty /100);}
    
	switch (eCh)
	{	
		case (GPTB_COMPA):csp_gptb_set_cmpa(ptGptbBase, (uint16_t)hwCmpLoad);
			break;
		case (GPTB_COMPB):csp_gptb_set_cmpb(ptGptbBase, (uint16_t)hwCmpLoad);
			break;

		default: return CSI_ERROR;
			break;
	}
    return CSI_OK;
}

/** \brief software force lock
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: refer to csi_gptb_ep_e 
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_force_em(ptGptbBase,(csp_gptb_ep_e)eEp);
}

/** \brief get harklock status
 * 
 *  \param[in] ptGptbBase : pointer of gptb register structure
 *  \return uint8_t 0 or 1
 */
uint8_t csi_gptb_get_hdlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emHdlck(ptGptbBase));
}
/** \brief clear harklock status
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: external emergency input: csi_gptb_ep_e  
 *  \return none               
 */
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{
	csp_gptb_clr_emHdlck(ptGptbBase, (csp_gptb_ep_e)eEp);
}

/**
 \brief get 	 softlock status
 \param ptGptb    pointer of gptb register structure
 \return uint8_t  0 or 1
*/
uint8_t csi_gptb_get_sftlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emSdlck(ptGptbBase));
}

/** \brief clear softlock status
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEp: external emergency input: csi_gptb_ep_e
 *  \return none
 */
void csp_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csi_gptb_ep_e eEp)
{	
	csp_gptb_clr_emSdlck(ptGptbBase,  (csp_gptb_ep_e)eEp);
}

/** \brief enable/disable gptb in debug mode
 * 
 *  \param[in]  ptGptbBase      pointer of gptb register structure
 *  \param[in]   bEnable		ENABLE/DISABLE
 *  \return none
 */
void csi_gptb_debug_enable(csp_gptb_t *ptGptbBase, bool bEnable)
{
	csp_gptb_dbg_enable(ptGptbBase, bEnable);
}

/** \brief enable/disable gptb emergencyinterruption
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEmint: refer to csi_gptb_emint_e
 *  \return none
 */
void csi_gptb_emint_en(csp_gptb_t *ptGptbBase, csi_gptb_emint_e eEmint)
{   csi_irq_enable((uint32_t *)ptGptbBase);	
    csp_gptb_Emergency_emimcr(ptGptbBase,(csp_gptb_emint_e)eEmint);
}

/** \brief enable/disable gptb out trigger 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgOut: evtrg out port(0~1)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_evtrg_enable(csp_gptb_t *ptGptbBase, csi_gptb_trgout_e eTrgOut, bool bEnable)
{	
	if (eTrgOut > GPTB_TRGOUT1)
		return CSI_ERROR;
		
    csp_gptb_trg_xoe_enable(ptGptbBase, eTrgOut, bEnable);
	return CSI_OK;
}


/** \brief   One time software output 
 * 
 *  \param[in]   ptGptbBase: pointer of gptb register structure 
 *  \param[in]   hwCh:  GPTB_OSTSFA/GPTB_OSTSFB
 *  \param[in]   eAction: refer to csi_gptb_action_e
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_onetimesoftware_output(csp_gptb_t *ptGptbBase, uint16_t hwCh, csi_gptb_action_e eAction)
{	
	switch (hwCh){
	case GPTB_OSTSFA: ptGptbBase ->AQOSF |= GPTB_OSTSFA;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTA_MSK))|((eAction&0x03)<<GPTB_ACTA_POS);
	     break;
	case GPTB_OSTSFB: ptGptbBase ->AQOSF |= GPTB_OSTSFB;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTB_MSK))|((eAction&0x03)<<GPTB_ACTB_POS);
	     break;	

	default: return CSI_ERROR;
	     break;
    }
	return CSI_OK;
}
/** \brief  Continuous software waveform loading control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eLoadtime:    refer to csi_gptb_aqosf_e
 *  \return  none
 */
void csi_gptb_aqcsfload_config(csp_gptb_t *ptGptbBase, csi_gptb_aqosf_e eLoadtime)
{
	ptGptbBase ->AQOSF  = (ptGptbBase ->AQOSF &~(GPTB_AQCSF_LDTIME_MSK))|((eLoadtime&0x03)<<GPTB_AQCSF_LDTIME_POS);
}

 /** \brief Continuous software waveform control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \param[in] eAction: refer to  csi_gptb_aqcsf_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_continuous_software_waveform(csp_gptb_t *ptGptbBase, csi_gptb_channel_e eChannel, csi_gptb_aqcsf_e eAction)
{
	
	switch (eChannel){
	case GPTB_CHANNEL_1:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x03))|(eAction&0x03);            
	     break;
	case GPTB_CHANNEL_2:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x0c))|(eAction&0x03)<<2;
	     break;	

	default: return CSI_ERROR;
	     break;
    }		
	return CSI_OK;
}

/** \brief gptb  int  config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:     refer to to csi_gptb_int_e
 *  \param[in] bEnable:  ENABLE/DISABLE
 *  \return CSI_OK;
 */
void csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csi_gptb_int_e eInt, bool bEnable)
{  
	csp_gptb_int_enable(ptGptbBase,(csp_gptb_int_e)eInt,bEnable);
	csi_irq_enable((uint32_t *)ptGptbBase);	
	
}

/** \brief gptb sync input evtrg config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] eTrgMode: gptb sync evtrg mode, continuous/once
 *  \param[in] eAutoRearm: refer to csi_gptb_arearm_e 
 *  \return none
 */
void csi_gptb_set_sync(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_trgmode_e eTrgMode, csi_gptb_arearm_e eAutoRearm)
{
	csp_gptb_set_sync_mode(ptGptbBase, eTrgIn, (csp_gptb_syncmd_e)eTrgMode);
	csp_gptb_set_auto_rearm(ptGptbBase, (csp_gptb_arearm_e)eAutoRearm);
	csp_gptb_sync_enable(ptGptbBase, eTrgIn, ENABLE);
}

/** \brief gptb extsync input select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_extsync_chnl(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_syncrout_e eTrgChx)
{
	switch(eTrgChx)
	{
		case 0:
			csp_gptb_sync_trg0sel(ptGptbBase, eTrgIn);
			break;
		case 1:
			csp_gptb_sync_trg1sel(ptGptbBase, eTrgIn);
			break;
		default:
			return CSI_ERROR;
		
	}
	return CSI_OK;
}

/** \brief gptb sync input filter config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptFilter: pointer of csi_gptb_filter_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_sync_filter(csp_gptb_t *ptGptbBase, csi_gptb_filter_config_t *ptFilter)
{
	uint32_t wFiltCr;
	uint32_t wWindow;
	
	if(ptFilter->byFiltSrc > GPTB_TRG_SYNCEN5)
		return CSI_ERROR;
	wFiltCr = ptFilter->byFiltSrc | (ptFilter->byWinInv << GPTB_FLTBLKINV_POS) | 
			 (ptFilter->byWinAlign << GPTB_ALIGNMD_POS) | (ptFilter->byWinCross << GPTB_CROSSMD_POS);
	wWindow = ptFilter->byWinOffset | (ptFilter->byWinWidth << GPTB_FLT_WDW_POS);
	
	csp_gptb_set_trgftcr(ptGptbBase, wFiltCr);
	csp_gptb_set_trgftwr(ptGptbBase, wWindow);
	
	return CSI_OK;
}
/** \brief rearm gptb sync evtrg  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \return none
 */
void csi_gptb_rearm_sync(csp_gptb_t *ptGptbBase,csi_gptb_trgin_e eTrgin)
{
	csp_gptb_rearm_sync(ptGptbBase, eTrgin);
}
/** \brief gptb evtrg output config
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgOut: evtrg out port(0~1)
 *  \param[in] eTrgSrc: evtrg source(1~15) 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evtrg(csp_gptb_t *ptGptbBase, csi_gptb_trgout_e eTrgOut, csi_gptb_trgsrc_e eTrgSrc)
{
	csp_gptb_set_trgsel01(ptGptbBase, eTrgOut, eTrgSrc);			    
	csp_gptb_trg_xoe_enable(ptGptbBase, eTrgOut, ENABLE);				//evtrg out enable
	
	return CSI_OK;//
}


/** \brief gptb evtrg cntxinit control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCntChx: evtrg countinit channel(0~3)
 *  \param[in] byCntVal: evtrg cnt value
 *  \param[in] byCntInitVal: evtrg cntxinit value
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_evcntinit(csp_gptb_t *ptGptbBase, uint8_t byCntChx, uint8_t byCntVal, uint8_t byCntInitVal)
{

 if(byCntChx > 1)
  return CSI_ERROR;
 
 csp_gptb_set_trgprd(ptGptbBase, byCntChx, byCntVal - 1);    //evtrg count
 csp_gptb_trg_cntxinit(ptGptbBase, byCntChx, byCntInitVal);
 csp_gptb_trg_cntxiniten_enable(ptGptbBase, byCntChx, ENABLE);
 
 return CSI_OK;
}

/** \brief  gptb configuration Loading
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptReglkCfg: refer to csi_gptb_reglk_config_t
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_reglk_config(csp_gptb_t *ptGptbBase,csi_gptb_reglk_config_t *ptReglkCfg)
{   uint32_t w_GLK;	
	w_GLK =0;
    w_GLK = (w_GLK & ~GPTB_PRDR_MSK )| ((ptReglkCfg-> byPrdr & 0xF)<< GPTB_PRDR_POS);
	w_GLK = (w_GLK & ~GPTB_CMPA_MSK )| ((ptReglkCfg-> byCmpa & 0xF)<< GPTB_CMPA_POS);
	w_GLK = (w_GLK & ~GPTB_CMPB_MSK )| ((ptReglkCfg-> byCmpb & 0xF)<< GPTB_CMPB_POS);
	w_GLK = (w_GLK & ~GPTB_GLD2_MSK )| ((ptReglkCfg-> byGld2 & 0xF)<< GPTB_GLD2_POS);
	w_GLK = (w_GLK & ~GPTB_RSSR_MSK )| ((ptReglkCfg-> byRssr & 0xF)<< GPTB_RSSR_POS);
	csp_gptb_set_reglk(ptGptbBase,w_GLK);
	w_GLK =0;
	w_GLK = (w_GLK & ~GPTB_EMSLCLR_MSK )| ((ptReglkCfg-> byEmslclr & 0xF)<< GPTB_EMSLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMHLCLR_MSK )| ((ptReglkCfg-> byEmhlclr & 0xF)<< GPTB_EMHLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMICR_MSK )  | ((ptReglkCfg-> byEmicr   & 0xF)<< GPTB_EMICR_POS);
	w_GLK = (w_GLK & ~GPTB_EMFRCR_MSK ) | ((ptReglkCfg-> byEmfrcr  & 0xF)<< GPTB_EMFRCR_POS);
	w_GLK = (w_GLK & ~GPTB_AQOSF_MSK )  | ((ptReglkCfg-> byAqosf   & 0xF)<< GPTB_AQOSF_POS);
	w_GLK = (w_GLK & ~GPTB_AQCSF_MSK )  | ((ptReglkCfg-> byAqcsf   & 0xF)<< GPTB_AQCSF_POS);
	csp_gptb_set_reglk2(ptGptbBase,w_GLK);	
	return CSI_OK;
}

