/***********************************************************************//** 
 * \file  pm.h
 * \brief  head file of power management
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial for csi
 * <tr><td> 2022-2-9  <td>V0.1  <td>ZJY   <td>Perfect handling
 * </table>
 * *********************************************************************
*/

#ifndef _PM_H_
#define _PM_H_

#include <sys_clk.h>
#include <stdint.h>
#include <drv/common.h>
#include <csi_core.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef CONFIG_USER_PM
typedef struct{
	void (*prepare_to_sleep)(void);
	void (*wkup_frm_sleep)(void);
	void (*prepare_to_deepsleep)(void);
	void (*wkup_frm_deepsleep)(void);
	void (*prepare_to_snooze)(void);
	void (*wkup_frm_snooze)(void);
	void (*prepare_to_shutdown)(void);
	void (*wkup_frm_shutdown)(void);
}pm_core_t;

extern pm_core_t g_tPmCore;

#endif

typedef enum { 
    WKUP_ALV0 	= 0,			//only shutdown, PA00
	WKUP_ALV1,					//only shutdown, PB00
	WKUP_ALV2,					//only shutdown, PA12
	WKUP_ALV3,					//only shutdown, PB011
	WKUP_IWDT 	= 8,			//deepsleep/snooze/shutdown
	WKUP_RTC,					//deepsleep/snooze/shutdown
	WKUP_LPT,
	WKUP_LVD,
	WKUP_TCH,
	WKUP_CMP	= 14
}csi_wakeup_src_e;

typedef enum {
	SP_IDLE_PCLK	= (0x01ul << 8),	///< IDLE_PCLK for sleep
	SP_IDLE_HCLK 	= (0x01ul << 9),	///< DLE_HCLK for sleep		
	DP_ISOSC 		= (0x01ul << 12),	///< ISOSC for deepsleep				
	DP_IMOSC	 	= (0x01ul << 13),	///< ISOSC for deepsleep	
	DP_ESOSC		= (0x01ul << 14),	///< ISOSC for deepsleep					
	DP_EMOSC		= (0x01ul << 15)	///< ISOSC for deepsleep
} csi_pm_clk_e;

typedef enum {
	SNOOZE_TOUCH_POWER	= 0,	///< TOUCH power control of snooze mode
	SNOOZE_LCD_POWER			///< LCD power control of snooze mode
} csi_snooze_power_e;

typedef enum {
	PM_MODE_LPRUN		= 0,	///< LowPower Running mode
	PM_MODE_SLEEP,				///< Sleep mode		
	PM_MODE_DEEPSLEEP,			///< Normal DeepSleep mode					
	PM_MODE_SNOOZE,				///< Snooze mode of DeepSleep	
	PM_MODE_SHUTDOWN,			///< ShutDown mode of DeepSleep	
} csi_pm_mode_e;

/**
  \brief       SoC enter low-power mode, each chip's implementation is different
               called by csi_pm_enter_sleep
  \param[in]   mode        low-power mode
  \return      error code
*/
csi_error_t csi_pm_enter_sleep(csi_pm_mode_e eMode);

/**
  \brief       SoC the wakeup source.
  \param[in]   wakeup_num  wakeup source num
  \param[in]   enable      flag control the wakeup source is enable or not
  \return      error code
*/
csi_error_t csi_pm_config_wakeup_source(csi_wakeup_src_e eWkupSrc, bool enable);

/**
  \brief       TOUCH/LCD power disable/enable 0f snooze mode
  \param[in]   ePower		\ref csi_snooze_power_e
  \param[in]   enable		enable/disable 
  \return      none
*/
csi_error_t csi_pm_snooze_power_enable(csi_snooze_power_e ePower, bool bEnable);

/**
  \brief       deep sleep mode, osc enable/disable.
  \param[in]   eSrc			\ref csi_pm_clk_e
  \param[in]   enable  		enable/disable deep sleep osc
  \return      error code
*/
void csi_pm_clk_enable(csi_pm_clk_e eOsc, bool bEnable);

/**
  \brief       clear wkalv int status
  \param[in]   eWkAlv		WKUP_ALV0 - WKUP_ALV_ALL
  \return      none
*/
void csi_pm_clr_wkint(uint8_t byWkInt);

/**
  \brief       get wkalv int status
  \param[in]   eWkAlv: WKUP_ALV0~WKUP_ALV_ALL
  \return      none
*/
uint8_t csi_pm_get_wkint(void);

/**
  \brief       to attach user defined functions, 
   * to use in case user wants to preserve the scene in lp mode 
  \param	eMd: low power mode
  \param   pBeforeSlp: funtion to be called before lp
  \param   pBeforeSlp: funtion to be called after wakeup 
  \return      None.
*/
void csi_pm_attach_callback(csi_pm_mode_e eMd, void *pBeforeSlp, void *pWkup);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_PM_H_ */
