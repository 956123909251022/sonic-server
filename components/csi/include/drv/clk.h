
/***********************************************************************//** 
 * \file  clk.h
 * \brief  head  file of clk.c
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-1-8 <td>V0.1  <td>WNN   <td> add declaration of csi_xxosc_enable()
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CLK_H_
#define _DRV_CLK_H_

#include <stdint.h>
#include <drv/common.h>
#include "csp_syscon.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CHECK_NUM                         3
#define CLCE_STEP_IM                      0x10000
#define CLCE_STEP_HF                      0x200

typedef enum{
	CLK_CALIB_IMCLK = 0,
	CLK_CALIB_HFCLK,
}clk_calib_e;

//typedef struct {
//	uint32_t wRegBase;
//    uint32_t wModule;
//} csi_clkmap_t;

typedef struct {
	uint32_t wClcrValue;
    uint32_t wClcrStep;
	uint32_t wThresholdMin;
    uint32_t wThresholdMax;	
	
	uint32_t wBtSumValue;
	uint16_t hwBtCntValue[CHECK_NUM];
} csi_clk_calib_t;


/**
 * \enum     csi_emfilt_e
 * \brief    emosc filtering select
 */
typedef enum {
	EM_FILT_5NS		= 0,	//Pulse filtering < 5ns  interval
    EM_FILT_10NS,			//Pulse filtering < 10ns interval
	EM_FILT_15NS,			//Pulse filtering < 15ns interval
	EM_FILT_20NS			//Pulse filtering < 20ns interval
} csi_emfilt_e;

/** \brief clock enable
 * 
 *  Enable peripheral clock in SYSCON
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_enable(void *pIpBase);

/** \brief clock disable
 * 
 *  Disable peripheral clock in SYSCON
 * 
 *  \param[in] pIpBase: pointer of peripheral Base address
 *  \return none.
 */
void csi_clk_disable(void *pIpBase);

/** \brief emosc enable
 * 
 *  enable external main oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_emosc_enable(uint32_t wFreq);

/** \brief emosc disable
 * 
 *  disable external main oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t.
 */
csi_error_t csi_emosc_disable(void);

/** \brief esosc enable
 * 
 *  enable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_esosc_enable(uint32_t wFreq);

/** \brief emosc disable
 * 
 *  disable external sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t.
 */
csi_error_t csi_esosc_disable(void);

/** \brief imosc enable
 * 
 *  enable internal main oscillator in SYSCON
 * 
 *  \param[in] byFre. 	0 - 5MHz
 * 						1 - 4MHz
 * 						2 - 2MHz
						3 - 131KHz
 *  \return csi_error_t
 */
csi_error_t csi_imosc_enable(uint8_t byFre);

/** \brief imosc disable
 * 
 *  disable internal main oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t.
 */
csi_error_t csi_imosc_disable(void );

/** \brief hfosc enable
 * 
 *  enable high frequency oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_hfosc_enable(uint8_t byFre);

/** \brief hfosc disable
 * 
 *  disable high frequency oscillator in SYSCON
 * 
 *  \param[in] byFre. 	0 - 48MHz
 * 						1 - 24MHz
 * 						2 - 12MHz
						3 - 6MHz
 *  \return csi_error_t.
 */
csi_error_t csi_hfosc_disable(void);

/** \brief isosc enable
 * 
 *  enable internal sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t
 */
csi_error_t csi_isosc_enable(void);

/** \brief isosc disable
 * 
 *  disable internal sub oscillator in SYSCON
 * 
 *  \param[in] none
 *  \return csi_error_t.
 */
csi_error_t csi_isosc_disable(void);


/** \brief emosc filtering enable
 * 
 *  \param[in] eFiltSel: \ref csi_emfilt_e
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none
 */
void csi_emosc_filter_enable(csi_emfilt_e eFiltSel, bool bEnable);

/** \brief esosc filtering enable
 * 
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return none
 */
void csi_esosc_filter_enable(bool bEnable);

/** \brief set emosc gain 
 * 
 *  \param[in] byEmGain: gain value, value < 0x20
 *  \return none
 */
void csi_emosc_set_gain(uint8_t byEmGain);

/** \brief set esosc gain 
 * 
 *  \param[in] byEsGain: gain value, value < 0x10
 *  \return none
 */
void csi_esosc_set_gain(uint8_t byEsGain);

/** \brief csi_clk_calib
 * 
 *  calibrate IMCLK or HFCLK 
 * 
 *  \return csi_error_t.
 */
csi_error_t csi_clk_calib(void);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_CLK_H_ */

