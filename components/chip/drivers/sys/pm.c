/***********************************************************************//** 
 * \file  pm.c
 * \brief  power management
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial for csi
 * <tr><td> 2022-2-9  <td>V0.1  <td>ZJY   <td>Perfect handling
 * </table>
 * *********************************************************************
*/

#include <drv/pm.h>
#include <drv/reliability.h>

#ifdef CONFIG_USER_PM
/// to make user defined prepaare_to_stop() and wkup_frm_stop() possible
pm_core_t g_tPmCore;

/** \brief to attach user defined functions, 
 * 
 *  to use in case user wants to preserve the scene in lp mode 
 *  \param[in] eMd: low power mode
 *  \param[in] pfBeforeSlp: funtion to be called before lp
 *  \param[in] pfBeforeSlp: funtion to be called after wakeup 
 *  \return None.
 */
void csi_pm_attach_callback(csi_pm_mode_e eMd, void *pfBeforeSlp, void *pfWkup)
{
	switch(eMd)
	{
		case (PM_MODE_SLEEP):
				g_tPmCore.prepare_to_sleep = pfBeforeSlp;
				g_tPmCore.wkup_frm_sleep = pfWkup;
			break;
		case (PM_MODE_DEEPSLEEP):
				g_tPmCore.prepare_to_deepsleep = pfBeforeSlp;
				g_tPmCore.wkup_frm_deepsleep = pfWkup;
			break;
		case (PM_MODE_SNOOZE):
				g_tPmCore.prepare_to_snooze = pfBeforeSlp;
				g_tPmCore.wkup_frm_snooze  = pfWkup;
				csi_ureg_write(USER_REG0, (uint32_t)pfWkup);			//save pfWkup addr to USER_REG0
			break;
		case (PM_MODE_SHUTDOWN):
				g_tPmCore.prepare_to_shutdown = pfBeforeSlp;
				g_tPmCore.wkup_frm_shutdown  = pfWkup;
				csi_ureg_write(USER_REG0, (uint32_t)pfWkup);			//save pfWkup addr to USER_REG0
			break;
		default:
			break;
	}
}

#endif

/** \brief enter sleep for eMode
 * 
 *  \param[in] eMode: \ref csi_pm_mode_e
 *  \return error code
 */
static csi_error_t apt_sleep(csi_pm_mode_e eMode)
{
	csi_error_t ret = CSI_OK;
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	switch (eMode)
	{
		case PM_MODE_SLEEP:
			//__DOZE();
			asm("doze");
			break;
		case PM_MODE_DEEPSLEEP:
			csp_set_deepsleep_mode(ptSysconBase, DPSLEEP_MODE);	
			//__STOP();
			asm("stop");
			break;
		case PM_MODE_SNOOZE:
			csp_set_deepsleep_mode(ptSysconBase, SNOOZE_MODE);	
			//__STOP();
			asm("stop");
			break;
		case PM_MODE_SHUTDOWN:
			csp_set_deepsleep_mode(ptSysconBase, SHUTDOWN_MODE);	
			//__STOP();
			asm("stop");
			break;
		default:
			ret = CSI_ERROR;
			break;
	}
	return ret;
}
/** \brief choose the pm mode to enter
 * 
 *  \param[in] mode: \ref csi_pm_mode_e
 *  \return error code
 */
csi_error_t csi_pm_enter_sleep(csi_pm_mode_e eMode)
{
	switch (eMode)
	{
		case PM_MODE_SLEEP:		
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.prepare_to_sleep)
					g_tPmCore.prepare_to_sleep();			//enter sleep,User handler 
			#endif
			apt_sleep(PM_MODE_SLEEP);
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.wkup_frm_sleep)
					g_tPmCore.wkup_frm_sleep();				//wakeup from sleep,User handler 	
			#endif
			break;
		case PM_MODE_DEEPSLEEP:
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.prepare_to_deepsleep)
					g_tPmCore.prepare_to_deepsleep();		//enter deep-sleep,User handler 
			#endif
			apt_sleep(PM_MODE_DEEPSLEEP);	
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.wkup_frm_deepsleep)
					g_tPmCore.wkup_frm_deepsleep();			//wakeup from deep-sleep,User handler 	
			#endif
			break;
		case PM_MODE_SNOOZE:
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.prepare_to_snooze)				//enter snooze,User handler 
					g_tPmCore.prepare_to_snooze();
			#endif
			apt_sleep(PM_MODE_SNOOZE);	
			break;
		case PM_MODE_SHUTDOWN:
			#ifdef CONFIG_USER_PM
				if(g_tPmCore.prepare_to_shutdown)			//enter shutdown,User handler 
					g_tPmCore.prepare_to_shutdown();
			#endif
			apt_sleep(PM_MODE_SHUTDOWN);	
			break;
		default:
            return CSI_ERROR;
	}
	
	return CSI_OK;
}

/** \brief Config the wakeup source.
 * 
 *  \param[in] wakeup_num wakeup source num
 *  \param[in] enable  flag control the wakeup source is enable or not
 *  \return error code
 */
csi_error_t csi_pm_config_wakeup_source(csi_wakeup_src_e eWkupSrc, bool bEnable)
{
    uint32_t wIrqNum = 32;
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	switch (eWkupSrc)
	{
		case (WKUP_IWDT):
			wIrqNum = SYSCON_IRQ_NUM;
			break;
		case (WKUP_RTC):
			wIrqNum = RTC_IRQ_NUM;
			break;
		case (WKUP_LPT):
			wIrqNum = LPT_IRQ_NUM;
			break;
		case (WKUP_LVD):
			wIrqNum = SYSCON_IRQ_NUM;
			break;
		case (WKUP_TCH):
			wIrqNum = TKEY_IRQ_NUM;
			break;
		case (WKUP_CMP):
			wIrqNum = CMP_IRQ_NUM;
			break;
		case (WKUP_ALV0):					//PA00
		case (WKUP_ALV1):					//PB00
		case (WKUP_ALV2):					//PA12
		case (WKUP_ALV3):					//PB011
			break;
		default: 
			return CSI_ERROR;
	}
	
	if(eWkupSrc <= WKUP_CMP)
	{
		if(bEnable)
			ptSysconBase->WKCR |= (0x1 << eWkupSrc);
		else
			ptSysconBase->WKCR &= ~(0x1 << eWkupSrc);
			
		if(eWkupSrc >=  WKUP_IWDT)
		{
			if (bEnable)
				csi_vic_set_wakeup_irq(wIrqNum);
			else
				csi_vic_clear_wakeup_irq(wIrqNum);
		}
	}
	else 
		 return CSI_ERROR;
	
	return CSI_OK;
}
/** \brief TOUCH/LCD power disable/enable 0f snooze mode
 * 
 * \param[in] ePower: \ref csi_snooze_power_e
 * \param[in] enable: enable/disable sleep/deepsleep osc
 * \return none
 */
csi_error_t csi_pm_snooze_power_enable(csi_snooze_power_e ePower, bool bEnable)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	if(ePower > SNOOZE_LCD_POWER)
		return CSI_ERROR;
	else
	{
		if(bEnable)
			ptSysconBase->WKCR |= (0x01ul << (ePower + 24));
		else
			ptSysconBase->WKCR &= ~(0x01ul << (ePower + 24));
	}
	
	return CSI_OK;
}
/** \brief deep sleep mode, osc enable/disable.
 * 
 * \param[in] eSleepOsc: \ref csi_sleep_osc_e
 * \param[in] enable: enable/disable sleep/deepsleep osc
 * \return error code
 */
void csi_pm_clk_enable(csi_pm_clk_e eOsc, bool bEnable)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	
	if(bEnable)
	{
		ptSysconBase->GCER |= eOsc;
		while(!(csp_get_gcsr(ptSysconBase) & eOsc));
	}
	else
	{
		ptSysconBase->GCDR |= eOsc;
		while(csp_get_gcsr(ptSysconBase) & eOsc);
	}
}
/** \brief clear wkalv int status
 * 
 *  \param[in] byWkInt: WkInt Mask WKINT0~3 
 *  \return none
 */
void csi_pm_clr_wkint(uint8_t byWkInt)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	ptSysconBase->ICR = ((byWkInt & 0x0f) << 24);				//PA00/PB011/PA12/PB011
}
/** \brief clear wkalv int status
 * 
 *  \param[in] none
 *  \return WkInt Mask
 */
uint8_t csi_pm_get_wkint(void)
{
	csp_syscon_t *ptSysconBase  = (csp_syscon_t*)APB_SYS_BASE;
	return (uint8_t)((ptSysconBase->RISR >> 24) & 0x0f);
}

