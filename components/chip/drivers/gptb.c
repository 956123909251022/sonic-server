/***********************************************************************//** 
 * \file  gptb.c
 * \brief  Enhanced purpose timer driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-1-17 <td>V0.0 <td>ljy     <td>initial
 * </table>
 * *********************************************************************
*/
#include "sys_clk.h"
#include "drv/common.h"
#include "drv/gptb.h"
#include "csp_gptb.h"
//#include "drv/pin.h"
#include "drv/irq.h"
//#include "drv/etb.h"

#include "drv/pin.h"

extern void load2(void);
uint32_t gTick2;
//to maximize the  speed
 uint32_t gGptb0Prd;
 uint32_t gGptb1Prd;
uint32_t wGptb_Cmp_Buff[4] = {0};
// uint32_t gGptb2Prd;
// uint32_t gGptb3Prd;
// uint32_t gGptb4Prd;
// uint32_t gGptb5Prd;


// /**
// \brief  Basic configuration
// \param  ptGptbBase    	pointer of gptb register structure
// \param  ptgptbPwmCfg   	refer to csi_gptb_config_t
// \return CSI_OK/CSI_ERROR
//*/
//csi_error_t csi_gptb_config_init(csp_gptb_t *ptGptbBase, csi_gptb_config_t *ptgptbPwmCfg)
//{
//	uint32_t wClkDiv;
//	uint32_t wCrVal;
//	uint32_t wCmpLoad; 
//	uint32_t wPrdrLoad; 
//	
//	if(ptgptbPwmCfg->wFreq == 0 ){ptgptbPwmCfg->wFreq =100;}
//	
//	
//	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable   ??????????????
//	
//	csp_gptb_clken(ptGptbBase);
//	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
//	csp_gptb_reset(ptGptbBase);											// reset 
//	
//	wClkDiv = (csi_get_pclk_freq() / ptgptbPwmCfg->wFreq / 30000);		// clk div value
//	if(wClkDiv == 0)wClkDiv = 1;
//	
//	wPrdrLoad  = (csi_get_pclk_freq()/ptgptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
//			
//	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
//	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
//    
//	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
//	
//	if(ptgptbPwmCfg->byWorkmod==GPTB_CAPTURE)
//	{
//	 	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptgptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
//		wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptgptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
//		
//		if(ptgptbPwmCfg->byCaptureCapLden)wCrVal|=GPTB_CAPLD_EN;
//		if(ptgptbPwmCfg->byCaptureRearm)  wCrVal|=GPTB_CAPREARM;
//		
//		wPrdrLoad=0xFFFF;
//	}
//	
//	if(ptgptbPwmCfg->byBurst)
//	{
//		wCrVal=(wCrVal & ~(GPTB_CGSRC_MSK))|((ptgptbPwmCfg->byCgsrc&0x03)<<GPTB_CGSRC_POS);
//		wCrVal=(wCrVal & ~(GPTB_CGFLT_MSK))|((ptgptbPwmCfg->byCgflt&0x07)<<GPTB_CGFLT_POS);
//	}
//	
//    csp_gptb_clken(ptGptbBase);                                             // clkEN
//	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
//	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv );					// clk div
//	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				        // prdr load value
//		
//	if(ptgptbPwmCfg->byDutyCycle){
//	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptgptbPwmCfg->byDutyCycle /100);	    // cmp load value
//	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					    // cmp load value
//	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
//	}
//	
//	if(ptgptbPwmCfg->wInt)
//	{
//		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->wInt, true);		//enable interrupt
//		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
//	}
//	
//	gGptb0Prd=wPrdrLoad;
//	
//	return CSI_OK;
//}

__attribute__((weak)) void gptb0_irqhandler(csp_gptb_t *ptGptbBase)
  {  	
	if(((csp_gptb_get_emisr(ptGptbBase) & B_EM_INT_CPUF))==B_EM_INT_CPUF)
	{ 
	  nop;//udelay(10);		  
	  ptGptbBase -> EMHLCLR |=B_EM_INT_CPUF;
	  csp_gptb_clr_emisr(ptGptbBase,B_EM_INT_CPUF);	
	 }
	  
	if(((csp_gptb_get_emisr(ptGptbBase) & B_EM_INT_EP3))==B_EM_INT_EP3)
	{ 
	  nop;udelay(10);
	  csp_gptb_clr_emHdlck(ptGptbBase, B_EP3);
	  csp_gptb_clr_emisr(ptGptbBase,B_EM_INT_EP3);	
	 }
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_TRGEV0))==GPTB_INT_TRGEV0)
	{
	   csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV0);
	}	
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_TRGEV1))==GPTB_INT_TRGEV1)
	{
	   csp_gptb_clr_isr(ptGptbBase, GPTB_INT_TRGEV1);
	}	
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD0))==GPTB_INT_CAPLD0)
	{
	  wGptb_Cmp_Buff[0]=csp_gptb_get_cmpa(ptGptbBase);
	  csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD0);	
	}
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD1))==GPTB_INT_CAPLD1)
	{
	  wGptb_Cmp_Buff[0]=csp_gptb_get_cmpa(ptGptbBase);
	  wGptb_Cmp_Buff[1]=csp_gptb_get_cmpb(ptGptbBase);
	  csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD1);	
	}
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD2))==GPTB_INT_CAPLD2)
	{
	  wGptb_Cmp_Buff[0]=csp_gptb_get_cmpa(ptGptbBase);
	  wGptb_Cmp_Buff[1]=csp_gptb_get_cmpb(ptGptbBase);	
	  wGptb_Cmp_Buff[2]=csp_gptb_get_cmpaa(ptGptbBase);
	  csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD2);	
	}
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD3))==GPTB_INT_CAPLD3)
	{
	  wGptb_Cmp_Buff[0]=csp_gptb_get_cmpa(ptGptbBase);
	  wGptb_Cmp_Buff[1]=csp_gptb_get_cmpb(ptGptbBase);	
	  wGptb_Cmp_Buff[2]=csp_gptb_get_cmpaa(ptGptbBase);
	  wGptb_Cmp_Buff[3]=csp_gptb_get_cmpba(ptGptbBase);	
	  csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD3);	
	}
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_PEND))==GPTB_INT_PEND)
	{
	   csi_gpio_port_set_high(GPIOA0, (0x01ul << 0));			
		nop;
	   csi_gpio_port_set_low (GPIOA0, (0x01ul << 0));
	   csp_gptb_clr_isr(ptGptbBase, GPTB_INT_PEND);
	}
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CBU))==GPTB_INT_CBU)
	{	
		gTick2++;if(gTick2>=5){	
								   //load2();	
	                               gTick2=0;
								   csi_gpio_port_set_high(GPIOA0, (0x01ul << 0));
								   csi_gptb_channel_cmpload_config(GPTB0, GPTB_CMPLD_IMM, GPTB_LDCMP_ZRO ,GPTB_CAMPA);
	                               csi_gptb_channel_cmpload_config(GPTB0, GPTB_CMPLD_IMM, GPTB_LDCMP_ZRO ,GPTB_CAMPB);
								   csi_gptb_change_ch_duty(GPTB0,GPTB_CAMPA, 25);
								   csi_gptb_change_ch_duty(GPTB0,GPTB_CAMPB, 25);
								   csi_gpio_port_set_low (GPIOA0, (0x01ul << 0));
								   
								   
		                         }
	    csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBU);
	   	
	}	

		
  }

__attribute__((weak)) void gptb1_irqhandler(csp_gptb_t *ptGptbBase)
  {  	
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CAPLD1))==GPTB_INT_CAPLD1)
	{		
//		  csi_pin_set_high(PA0);
	  csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CAPLD1);
//		  csi_pin_set_low(PA0); 		
	} 
  }
/** \brief capture configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptgptbPwmCfg: refer to csi_ept_captureconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_capture_init(csp_gptb_t *ptGptbBase, csi_gptb_captureconfig_t *ptgptbPwmCfg)
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
					
	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CAPMD_MSK))   |((ptgptbPwmCfg->byCaptureCapmd&0x01)   <<GPTB_CAPMD_POS);
	wCrVal=(wCrVal & ~(GPTB_STOPWRAP_MSK))|((ptgptbPwmCfg->byCaptureStopWrap&0x03)<<GPTB_STOPWRAP_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPA_RST_MSK))|((ptgptbPwmCfg->byCaptureLdaret&0x01)  <<GPTB_CMPA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPB_RST_MSK))|((ptgptbPwmCfg->byCaptureLdbret&0x01)  <<GPTB_CMPB_RST_POS);
	
	wCrVal=(wCrVal & ~(GPTB_CMPAA_RST_MSK))|((ptgptbPwmCfg->byCaptureLdaaret&0x01)  <<GPTB_CMPAA_RST_POS);
	wCrVal=(wCrVal & ~(GPTB_CMPBA_RST_MSK))|((ptgptbPwmCfg->byCaptureLdbaret&0x01)  <<GPTB_CMPBA_RST_POS);

	wCrVal|=GPTB_CAPLD_EN;//CAPMODE_SEL
	wCrVal|=GPTB_CAPREARM;
	wPrdrLoad=0xFFFF;

    csp_gptb_clken(ptGptbBase);                                         // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);								// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);				// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
	
	if(ptgptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->wInt, true);   //enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	gGptb0Prd=wPrdrLoad;
	
	return CSI_OK;
}

/** \brief wave configuration
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] ptgptbPwmCfg: refer to csi_ept_pwmconfig_t
 *  \return error code \ref csi_error_t
 */
csi_error_t  csi_gptb_wave_init(csp_gptb_t *ptGptbBase, csi_gptb_pwmconfig_t *ptgptbPwmCfg)
{
    uint32_t wClkDiv;
	uint32_t wCrVal;
	uint32_t wCmpLoad; 
	uint32_t wPrdrLoad; 
	
	if(ptgptbPwmCfg->wFreq == 0 ){return CSI_ERROR;}
		
	csi_clk_enable((uint32_t *)ptGptbBase);								// clk enable   GPTB_UPDNCNT
	
	csp_gptb_clken(ptGptbBase);
	csp_gptb_wr_key(ptGptbBase);                                           //Unlocking
	csp_gptb_reset(ptGptbBase);											// reset 
	
	if(ptgptbPwmCfg->byCountingMode==GPTB_UPDNCNT){
		wClkDiv = (csi_get_pclk_freq() / ptgptbPwmCfg->wFreq /2/ 30000);		// clk div value		
		if(wClkDiv == 0)wClkDiv = 1;	
		wPrdrLoad  = (csi_get_pclk_freq()/ptgptbPwmCfg->wFreq/2/wClkDiv);	    //prdr load value
	}else{
		wClkDiv = (csi_get_pclk_freq() / ptgptbPwmCfg->wFreq / 30000);		// clk div value		
		if(wClkDiv == 0)wClkDiv = 1;	
		wPrdrLoad  = (csi_get_pclk_freq()/ptgptbPwmCfg->wFreq/wClkDiv);	    //prdr load value
	}
		
	wCrVal =ptgptbPwmCfg->byCountingMode | (ptgptbPwmCfg->byStartSrc<<GPTB_STARTSRC_POS) |
	        ptgptbPwmCfg->byOneshotMode<<GPTB_OPMD_POS | (ptgptbPwmCfg->byWorkmod<<GPTB_MODE_POS);
    
	wCrVal=(wCrVal & ~(GPTB_PSCLD_MSK))   |((ptgptbPwmCfg->byPscld&0x03)   <<GPTB_PSCLD_POS);
	
	
    csp_gptb_clken(ptGptbBase);                                           // clkEN
	csp_gptb_set_cr(ptGptbBase, wCrVal);									// set bt work mode
	csp_gptb_set_pscr(ptGptbBase, (uint16_t)wClkDiv - 1);					// clk div
	csp_gptb_set_prdr(ptGptbBase, (uint16_t)wPrdrLoad);				    // prdr load value
		
	if(ptgptbPwmCfg->byDutyCycle){
	wCmpLoad =wPrdrLoad-(wPrdrLoad * ptgptbPwmCfg->byDutyCycle /100);	// cmp load value
	csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);					// cmp load value
	csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);

	}
	
	if(ptgptbPwmCfg->wInt)
	{
		csp_gptb_int_enable(ptGptbBase, ptgptbPwmCfg->wInt, true);		//enable interrupt
		csi_irq_enable((uint32_t *)ptGptbBase);							//enable  irq
	}
	
	gGptb0Prd=wPrdrLoad;
	
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
	csp_gptb_set_opmd(ptGptbBase, eCntMode);
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
 *  \param[in] tld: refer to csp_gptb_cmpdata_ldmd_e
 *  \param[in] tldamd: refer to csp_gptb_ldamd_e
 *  \param[in] eChannel: refer to csi_gptb_camp_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_cmpload_config(csp_gptb_t *ptGptbBase, csp_gptb_cmpdata_ldmd_e tld, csp_gptb_ldamd_e tldamd ,csi_gptb_camp_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_CAMPA):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LD_MSK) )    |  (tld    << GPTB_CMPA_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPA_LDTIME_MSK) )|  (tldamd <<GPTB_CMPA_LDTIME_POS);
			break;
		case (GPTB_CAMPB):   ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LD_MSK) )    |  (tld    << GPTB_CMPB_LD_POS);
		                     ptGptbBase -> CMPLDR = (ptGptbBase -> CMPLDR &~(GPTB_CMPB_LDTIME_MSK) )|  (tldamd << GPTB_CMPB_LDTIME_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief Channel configuration
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] tPwmCfg: refer to csi_gptb_pwmchannel_config_t
 *  \param[in] eChannel:  refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_config(csp_gptb_t *ptGptbBase, csi_gptb_pwmchannel_config_t *tPwmCfg, csi_gptb_channel_e eChannel)
{
    uint32_t w_AQCRx_Val;
	
	w_AQCRx_Val=  tPwmCfg -> byActionZro 
	              | ( tPwmCfg -> byActionPrd  << GPTB_ACT_PRD_POS  )
				  | ( tPwmCfg -> byActionC1u  << GPTB_ACT_C1U_POS  )
				  | ( tPwmCfg -> byActionC1d  << GPTB_ACT_C1D_POS  )
				  | ( tPwmCfg -> byActionC2u  << GPTB_ACT_C2U_POS  )
				  | ( tPwmCfg -> byActionC2d  << GPTB_ACT_C2D_POS  )
				  | ( tPwmCfg -> byActionT1u  << GPTB_ACT_T1U_POS  )
				  | ( tPwmCfg -> byActionT1d  << GPTB_ACT_T1D_POS  )
				  | ( tPwmCfg -> byActionT2u  << GPTB_ACT_T2U_POS  )
				  | ( tPwmCfg -> byActionT2d  << GPTB_ACT_T2D_POS  )
				  | ( tPwmCfg -> byChoiceC1sel  << GPTB_C1SEL_POS  )
				  | ( tPwmCfg -> byChoiceC2sel  << GPTB_C2SEL_POS  );
				  
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
 *  \param[in] tld: refer to csp_gptb_ld_e
 *  \param[in] tldamd: refer to csp_gptb_ldamd_e
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channel_aqload_config(csp_gptb_t *ptGptbBase, csp_gptb_ld_e tld, csp_gptb_ldamd_e tldamd ,csi_gptb_channel_e eChannel)
{			  
	switch (eChannel)
	{	
		case (GPTB_CHANNEL_1):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR1_SHDWEN_MSK) )|  (tld << GPTB_AQCR1_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDAMD_MSK) )|  (tldamd << GPTB_LDAMD_POS);
			break;
		case (GPTB_CHANNEL_2):ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_AQCR2_SHDWEN_MSK) )|  (tld << GPTB_AQCR2_SHDWEN_POS);
		                      ptGptbBase -> AQLDR = (ptGptbBase -> AQLDR &~(GPTB_LDBMD_MSK) )|  (tldamd << GPTB_LDBMD_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	return CSI_OK;
}

/** \brief DeadZoneTime configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] tCfg: refer to csi_gptb_deadzone_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_dz_config(csp_gptb_t *ptGptbBase, csi_gptb_deadzone_config_t *tCfg)
{  uint32_t w_Val;
   
	w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	w_Val=(w_Val&~(GPTB_DCKSEL_MSK))  |(tCfg-> byDcksel <<GPTB_DCKSEL_POS);
	w_Val=(w_Val&~(GPTB_CHA_DEDB_MSK))|(tCfg-> byChaDedb<<GPTB_CHA_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHB_DEDB_MSK))|(tCfg-> byChbDedb<<GPTB_CHB_DEDB_POS);
//	w_Val=(w_Val&~(GPTB_CHC_DEDB_MSK))|(tCfg-> byChcDedb<<GPTB_CHC_DEDB_POS);
	csp_gptb_set_dbcr( ptGptbBase, w_Val);	 
	csp_gptb_set_dpscr(ptGptbBase, tCfg-> hwDpsc);
	
	w_Val=csi_get_pclk_freq();
	w_Val=(1000000000/(w_Val/(tCfg->hwDpsc+1)));    //NS/(1/(48000000/(DPSC+1))*10^9) // 500NS/(1000/48) = 24;	
	csp_gptb_set_dbdtr(ptGptbBase	,tCfg-> hwRisingEdgereGister /w_Val);
	csp_gptb_set_dbdtf(ptGptbBase	,tCfg-> hwFallingEdgereGister/w_Val);
	
	return CSI_OK;	
}
/** \brief DeadZoneTime configuration loading 
 *  
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byVal: refer to csi_gptb_dbdldr_e
 *  \param[in] byWod: refer to csp_gptb_shdw_e
 *  \param[in] byWod2: refer to csp_gptb_lddb_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_dbload_config(csp_gptb_t *ptGptbBase, csi_gptb_dbdldr_e byVal,csp_gptb_shdw_e byWod,csp_gptb_lddb_e byWod2)
{   uint32_t w_Val;
	w_Val=csp_gptb_get_dbldr(ptGptbBase);
	switch (byVal)
	{	
		case (GPTB_DBCR) :w_Val=( w_Val &~(GPTB_DBLDR_CRSHDWEN_MSK) )|(byWod << GPTB_DBLDR_CRSHDWEN_POS);
		             w_Val=( w_Val &~(GPTB_DBLDR_LDCRMODE_MSK))|(byWod2 << GPTB_DBLDR_LDCRMODE_POS);
			break;
		case (GPTB_DBDTR):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTR_MSK )|(byWod << GPTB_DBLDR_SHDWDTR_POS );
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTRMD_MSK )|(byWod2 << GPTB_DBLDR_LDDTRMD_POS);
		    break;
		case (GPTB_DBDTF):w_Val=( w_Val &~GPTB_DBLDR_SHDWDTF_MSK )|(byWod << GPTB_DBLDR_SHDWDTF_POS);
		             w_Val=( w_Val &~GPTB_DBLDR_LDDTFMD_MSK )|(byWod2 <<GPTB_DBLDR_LDDTFMD_POS);
            break;
		case (GPTB_DCKPSC):w_Val=( w_Val &~GPTB_DBLDR_SHDWPSC_MSK)|(byWod << GPTB_DBLDR_SHDWPSC_POS);
		              w_Val=( w_Val &~GPTB_DBLDR_LDPSCMD_MSK)|(byWod2 << GPTB_DBLDR_LDPSCMD_POS);
		    break;
		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbldr(ptGptbBase,w_Val);
			
	return CSI_OK;
}   

/** \brief channelmode configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] tCfg: refer to csi_gptb_deadzone_config_t
 *  \param[in] eChannel: refer to csi_gptb_channel_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_channelmode_config(csp_gptb_t *ptGptbBase,csi_gptb_deadzone_config_t *tCfg,csi_gptb_channel_e eChannel)
{    uint32_t w_Val;
     w_Val=csp_gptb_get_dbcr(ptGptbBase);	
	 switch (eChannel)
	{	
		case (GPTB_CHANNEL_1): w_Val=(w_Val&~(DB_CHA_OUTSEL_MSK)) |(tCfg-> byChxOuselS1S0   <<DB_CHA_OUTSEL_POS);
		                      w_Val=(w_Val&~(DB_CHA_POL_MSK))    |(tCfg-> byChxPolarityS3S2<<DB_CHA_POL_POS);
							  w_Val=(w_Val&~(DB_CHA_INSEL_MSK))  |(tCfg-> byChxInselS5S4   <<DB_CHA_INSEL_POS);
							  w_Val=(w_Val&~(DB_CHA_OUTSWAP_MSK))|(tCfg-> byChxOutSwapS8S7 <<DB_CHA_OUTSWAP_POS);
			break;

		default:return CSI_ERROR;
			break;
	}
	csp_gptb_set_dbcr( ptGptbBase, w_Val);
	return CSI_OK;
}

/** \brief State of emergency configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] tCfg: refer to csi_gptb_emergency_config_t
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_emergency_cfg(csp_gptb_t *ptGptbBase, csi_gptb_emergency_config_t *tCfg)
{ uint32_t wEmsrc;
  uint32_t wEmsrc2;
  uint32_t wEmpol;
  uint32_t wEmecr;

    wEmsrc2=csp_gptb_get_src2(ptGptbBase);
	wEmsrc2=(wEmsrc2 & (~GPTB_EMSRC2_FLT_PACE0_MSK)) | (tCfg ->byFltpace0  << GPTB_EMSRC2_FLT_PACE0_POS);
	wEmsrc2=(wEmsrc2 &~0xff0000) |  tCfg ->byOrl1 <<16;
	wEmsrc2=(wEmsrc2 &~0xff)     |  tCfg ->byOrl0 ;
	csp_gptb_set_src2(ptGptbBase,wEmsrc2);
		
	wEmsrc = csp_gptb_get_src(ptGptbBase);
    wEmsrc=(  wEmsrc &~ GPTB_SEL_MSK_EP(tCfg -> byEpx) )|( tCfg -> byEpxInt  << GPTB_SEL_POS_EP(tCfg -> byEpx));
    csp_gptb_set_src(ptGptbBase,wEmsrc);
	
    wEmpol=csp_gptb_get_empol(ptGptbBase);	
	 switch (tCfg ->byEpxInt)
	 {    case (B_EBI0): wEmpol=( wEmpol  &~ POL_MSK_EBI(0)) | (tCfg -> byPolEbix <<POL_POS_EBI(0) );break;
		  case (B_EBI1): wEmpol=( wEmpol  &~ POL_MSK_EBI(1)) | (tCfg -> byPolEbix <<POL_POS_EBI(1) );break;
		  case (B_EBI2): wEmpol=( wEmpol  &~ POL_MSK_EBI(2)) | (tCfg -> byPolEbix <<POL_POS_EBI(2) );break;
		  case (B_EBI3): wEmpol=( wEmpol  &~ POL_MSK_EBI(3)) | (tCfg -> byPolEbix <<POL_POS_EBI(3) );break;
		  default:break;
	 }
	csp_gptb_set_empol(ptGptbBase,wEmpol);

    wEmecr =  csp_gptb_get_emecr(ptGptbBase);	
	wEmecr =(wEmecr & (~GPTB_LCKMD_MSK_EP(tCfg ->byEpx))) | (   tCfg ->byEpxLckmd <<  GPTB_LCKMD_POS_EP(tCfg ->byEpx));
	csp_gptb_set_emecr(ptGptbBase,wEmecr);
			
	return CSI_OK;
}
/** \brief State of emergency configuration 
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh: refer to csi_gptb_osrchx_e
 *  \param[in] byCh2: refer to csp_gptb_emout_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_emergency_pinxout(csp_gptb_t *ptGptbBase,csi_gptb_osrchx_e  byCh ,csp_gptb_emout_e byCh2)
{ uint32_t wEmosr;
    wEmosr=csp_gptb_get_emosr(ptGptbBase);	
	 switch (byCh)
	 {    case (GPTB_EMCOAX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAX_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COAX_POS);break;
		  case (GPTB_EMCOBX): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COBX_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COBX_POS);break;
		  case (GPTB_EMCOAY): wEmosr=( wEmosr &~(GPTB_EMOSR_EM_COAY_MSK) )|( byCh2 <<GPTB_EMOSR_EM_COAY_POS);break;		
		  default:return CSI_ERROR;break;
	 }
    csp_gptb_set_emosr(ptGptbBase,wEmosr);
	return CSI_OK;
}

/** \brief gptb configuration Loading
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] tCfg: refer to csi_gptb_Global_load_control_config_t
 *  \return none
 */
void csi_gptb_global_config(csp_gptb_t *ptGptbBase,csi_gptb_Global_load_control_config_t *Global)
{   uint32_t w_GLDCR;	
	w_GLDCR =0;
    w_GLDCR = (w_GLDCR &~GPTB_GLDEN_MSK)       | ((Global->bGlden & 0x01)<<GPTB_GLDEN_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDMD_MSK)       | ((Global->byGldmd & 0x0f)<<GPTB_GLDMD_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDCR_OSTMD_MSK) | ((Global->bOstmd & 0x01)<<GPTB_GLDCR_OSTMD_POS);
	w_GLDCR = (w_GLDCR &~GPTB_GLDPRD_MSK)      | ((Global->bGldprd & 0x07)<<GPTB_GLDPRD_POS);
	csp_gptb_set_gldcr(ptGptbBase,w_GLDCR);	

}

/** \brief CLDCFG loading
 * 
 *  \param[in] ptGptbBase of gptb register structure
 *  \param[in] Glo:  csi_gptb_Global_load_gldcfg  
 *  \param[in] bENABLE：ENABLE or DISABLE
 *  \return CSI_OK
 */
csi_error_t csi_gptb_gldcfg(csp_gptb_t *ptGptbBase ,csi_gptb_Global_load_gldcfg Glo,bool bENABLE)
{
   	switch (Glo)
	{	
		case (byprdr_B): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_PRDR_MSK)) |(bENABLE << GPTB_LD_PRDR_POS) ;
			break;
		case (bycmpa_B): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_CMPA_MSK)) |(bENABLE << GPTB_LD_CMPA_POS) ;
			break;
		case (bycmpb_B): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_CMPB_MSK)) |(bENABLE << GPTB_LD_CMPB_POS) ;
		    break;

//		case (bydbdtr_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_DBDTR_MSK))|(bENABLE << GPTB_LD_DBDTR_POS);
//		    break;
//		case (bydbdtf_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_DBDTF_MSK))|(bENABLE << GPTB_LD_DBDTF_POS);
//		    break;
//		case (bydbcr_B): ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_DBCR_MSK)) |(bENABLE << GPTB_LD_DBCR_POS );
//		    break;
		case (byaqcra_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCRA_MSK)) |(bENABLE << GPTB_LD_AQCRA_POS );
		    break;
		case (byaqcrb_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCRB_MSK)) |(bENABLE << GPTB_LD_AQCRB_POS );
		    break;
	
	    case (byaqcsf_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_AQCSF_MSK)) |(bENABLE << GPTB_LD_AQCSF_POS );
			 break;
//		case (byemosr_B):ptGptbBase -> GLDCFG  = (ptGptbBase -> GLDCFG & ~(GPTB_LD_EMOSR_MSK)) |(bENABLE << GPTB_LD_EMOSR_POS );
//			 break;	 
		default: return (1);
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
	csp_gptb_set_start_src(ptGptbBase, eMode);
}
/** \brief set gptb operation mode
 * 
 *  \param[in] ptGptbBase ：pointer of gptb register structure
 *  \param[in] eMode ： GPTB_OP_CONT/GPTB_OP_OT
 *  \return none
 */
void csi_gptb_set_os_mode(csp_gptb_t *ptGptbBase, csi_gptb_opmd_e eMode)
{
	csp_gptb_set_opmd(ptGptbBase, eMode);
}
/** \brief set gptb stop status
 * 
 *  \param[in] ptGptbBase :   pointer of gptb register structure
 *  \param[in] eSt 	 GPTB_STPST_HZ/GPTB_STPST_LOW
 *  \return none
 */
void csi_gptb_set_stop_st(csp_gptb_t *ptGptbBase, csp_gptb_stpst_e eSt)
{	
  csp_gptb_set_stop_st(ptGptbBase,eSt);
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
 *  \param[in] eCh  :          refer to csi_gptb_camp_e
 *	\param[in] wActiveTime :   cmpx data to be set directly
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_change_ch_duty(csp_gptb_t *ptGptbBase, csi_gptb_camp_e eCh, uint32_t wActiveTime)
{ uint16_t  wCmpLoad;
  uint16_t  wPrd;
    wPrd=csp_gptb_get_prd(ptGptbBase);
	if(wActiveTime>=100){wCmpLoad=0;}
	else if(wActiveTime==0){wCmpLoad=wPrd+1;}
	else{wCmpLoad =wPrd-(wPrd * wActiveTime /100);}
    
	switch (eCh)
	{	
		case (GPTB_CAMPA):csp_gptb_set_cmpa(ptGptbBase, (uint16_t)wCmpLoad);
			break;
		case (GPTB_CAMPB):csp_gptb_set_cmpb(ptGptbBase, (uint16_t)wCmpLoad);
			break;

		default: return CSI_ERROR;
			break;
	}
    return CSI_OK;
}

/** \brief software force lock
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byEbi: external emergency input: EPT_EPI0~3 
 *  \return none
 */
void csi_gptb_force_em(csp_gptb_t *ptGptbBase, csp_gptb_ep_e byEbi)
{
	csp_gptb_force_em(ptGptbBase,byEbi);
}

/** \brief get harklock status
 * 
 *  \param[in] ptGptbBase : pointer of gptb register structure
 *  \return uint8_t 0b_ _ _ _ _ _ _ _
 */
uint8_t csi_gptb_get_hdlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emHdlck(ptGptbBase));
}
/** \brief clear harklock status
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEbi: external emergency input: csp_ept_ep_e  
 *  \return none               
 */
void csi_gptb_clr_hdlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEbi)
{
	csp_gptb_clr_emHdlck(ptGptbBase, eEbi);
}

/**
 \brief get 	 softlock status
 \param ptGptb    pointer of gptb register structure
 \return uint8_t 0b_ _ _ _ _ _ _ _
*/
uint8_t csi_gptb_get_sftlck_st(csp_gptb_t *ptGptbBase)
{	
	return (csp_gptb_get_emSdlck(ptGptbBase));
}

/** \brief clear softlock status
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eEpi: external emergency input: csp_gptb_ep_e
 *  \return none
 */
void csp_gptb_clr_sftlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEpi)
{	
	csp_gptb_clr_emSdlck(ptGptbBase, eEpi);
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
 *  \param[in] eEbi: refer to csp_gptb_emint_e
 *  \return none
 */
void csi_gptb_emergency_int_enable(csp_gptb_t *ptGptbBase, csp_gptb_emint_e eEm)
{   csi_irq_enable((uint32_t *)ptGptbBase);	
    csp_gptb_Emergency_emimcr(ptGptbBase,eEm);
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
 *  \param[in]   byCh:  GPTB_OSTSFA/GPTB_OSTSFB
 *  \param[in]   bEnable : GPTB_LDAQCR_ZRO/GPTB_LDAQCR_PRD/GPTB_LDAQCR_ZROPRD
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_onetimesoftware_output(csp_gptb_t *ptGptbBase, uint16_t byCh, csp_gptb_action_e bEnable)
{	
	switch (byCh){
	case GPTB_OSTSFA: ptGptbBase ->AQOSF |= GPTB_OSTSFA;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTA_MSK))|((bEnable&0x03)<<GPTB_ACTA_POS);
	     break;
	case GPTB_OSTSFB: ptGptbBase ->AQOSF |= GPTB_OSTSFB;
	                 ptGptbBase ->AQOSF = (ptGptbBase ->AQOSF &~(GPTB_ACTB_MSK))|((bEnable&0x03)<<GPTB_ACTB_POS);
	     break;	

	default: return CSI_ERROR;
	     break;
    }
	return CSI_OK;
}
/** \brief  Continuous software waveform loading control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] bEnable:    refer to csp_gptb_aqosf_e
 *  \return  none
 */
void csi_gptb_aqcsfload_config(csp_gptb_t *ptGptbBase, csp_gptb_aqosf_e bEnable)
{
	ptGptbBase ->AQOSF  = (ptGptbBase ->AQOSF &~(GPTB_AQCSF_LDTIME_MSK))|((bEnable&0x03)<<GPTB_AQCSF_LDTIME_POS);
}

 /** \brief Continuous software waveform control
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] byCh: refer to csi_gptb_channel_e
 *  \param[in] bEnable: refer to  csp_gptb_aqcsf_e
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_continuous_software_waveform(csp_gptb_t *ptGptbBase, csi_gptb_channel_e byCh, csp_gptb_aqcsf_e bEnable)
{
	
	switch (byCh){
	case GPTB_CHANNEL_1:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x03))|(bEnable&0x03);            
	     break;
	case GPTB_CHANNEL_2:  ptGptbBase ->AQCSF = (ptGptbBase ->AQCSF &~(0x0c))|(bEnable&0x03)<<2;
	     break;	

	default: return CSI_ERROR;
	     break;
    }		
	return CSI_OK;
}

/** \brief gptb  input  config  
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eInt:     refer to to csp_gptb_int_e
 *  \param[in] bEnable:  ENABLE/DISABLE
 *  \return CSI_OK;
 */
void csi_gptb_int_enable(csp_gptb_t *ptGptbBase, csp_gptb_int_e eInt, bool bEnable)
{  
	csp_gptb_int_enable(ptGptbBase,eInt,bEnable);
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
	csp_gptb_set_sync_mode(ptGptbBase, eTrgIn, eTrgMode);
	csp_gptb_set_auto_rearm(ptGptbBase, eAutoRearm);
	csp_gptb_sync_enable(ptGptbBase, eTrgIn, ENABLE);
}

/** \brief gptb extsync input select
 * 
 *  \param[in] ptGptbBase: pointer of gptb register structure
 *  \param[in] eTrgin: gptb sync evtrg input channel(0~6)
 *  \param[in] byTrgChx: trgxsel channel(0~1)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_gptb_set_extsync_chnl(csp_gptb_t *ptGptbBase, csi_gptb_trgin_e eTrgIn, csi_gptb_syncrout_e byTrgChx)
{
	switch(byTrgChx)
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
 *  \param[in] Global: refer to csi_gptb_feglk_config_t
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_gptb_reglk_config(csp_gptb_t *ptGptbBase,csi_gptb_feglk_config_t *Global)
{   uint32_t w_GLK;	
	w_GLK =0;
    w_GLK = (w_GLK & ~GPTB_PRDR_MSK )| ((Global-> byPrdr & 0xF)<< GPTB_PRDR_POS);
	w_GLK = (w_GLK & ~GPTB_CMPA_MSK )| ((Global-> byCmpa & 0xF)<< GPTB_CMPA_POS);
	w_GLK = (w_GLK & ~GPTB_CMPB_MSK )| ((Global-> byCmpb & 0xF)<< GPTB_CMPB_POS);
	w_GLK = (w_GLK & ~GPTB_GLD2_MSK )| ((Global-> byGld2 & 0xF)<< GPTB_GLD2_POS);
	w_GLK = (w_GLK & ~GPTB_RSSR_MSK )| ((Global-> byRssr & 0xF)<< GPTB_RSSR_POS);
	csp_gptb_set_feglk(ptGptbBase,w_GLK);
	w_GLK =0;
	w_GLK = (w_GLK & ~GPTB_EMSLCLR_MSK )| ((Global-> byEmslclr & 0xF)<< GPTB_EMSLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMHLCLR_MSK )| ((Global-> byEmhlclr & 0xF)<< GPTB_EMHLCLR_POS);
	w_GLK = (w_GLK & ~GPTB_EMICR_MSK )  | ((Global-> byEmicr   & 0xF)<< GPTB_EMICR_POS);
	w_GLK = (w_GLK & ~GPTB_EMFRCR_MSK ) | ((Global-> byEmfrcr  & 0xF)<< GPTB_EMFRCR_POS);
	w_GLK = (w_GLK & ~GPTB_AQOSF_MSK )  | ((Global-> byAqosf   & 0xF)<< GPTB_AQOSF_POS);
	w_GLK = (w_GLK & ~GPTB_AQCSF_MSK )  | ((Global-> byAqcsf   & 0xF)<< GPTB_AQCSF_POS);
	csp_gptb_set_feglk2(ptGptbBase,w_GLK);	
	return CSI_OK;
}

