/***********************************************************************//** 
 * \file  led.c
 * \brief  LED driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author	<th>Description
 * <tr><td> 2020-12-8 <td>V0.0 <td>WNN   	<td>initial
 * </table>
 * *********************************************************************
*/
#include "led.h"
#include "irq.h"
#include "csp_common.h"
#include "sys_clk.h"


/** \brief   led init & configuration: clk, brightness, COM mask
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \param[in] tLedCfg:	pointer of led parameter config structure
 *  \return  csi_error_t  
 */
csi_error_t csi_led_init(csp_led_t *ptLedBase, csi_led_config_t *tLedCfg) 
{
	csi_clk_enable((uint32_t *)ptLedBase);	
	csp_led_set_brt(ptLedBase, tLedCfg->byBrt);
	csp_led_set_clk(ptLedBase, tLedCfg->byClk);
	csp_led_set_commask(ptLedBase, (tLedCfg->hwComMask) & 0x3ff);
	if(tLedCfg->byOnTime > 2096 || tLedCfg->byBreakTime > 524)
		return CSI_ERROR;
	csp_led_set_dcomcnt(ptLedBase, (tLedCfg->byOnTime/8 - 7));
	csp_led_set_novcnt(ptLedBase, (tLedCfg->byBreakTime/2 -7));
	
	if(tLedCfg->byInt) {
		csp_led_int_enable(ptLedBase, tLedCfg->byInt, ENABLE);
		csi_irq_enable((uint32_t *)ptLedBase);
	}
	return CSI_OK;
	
}
/** \brief LED interrupt enable/disable control
 * 
 *  \param[in] ptLedBase: pointer of bt register structure
 *  \param[in] eIntSrc: led interrupt source
 *  \param[in] bEnable: enable/disable interrupt
 *  \return none
 */ 
void csi_led_int_enable(csp_led_t *ptLedBase, csi_led_intsrc_e eIntSrc, bool bEnable)
{
	csp_led_int_enable(ptLedBase, eIntSrc, bEnable);
	
	if (bEnable) {
		csi_irq_enable((uint32_t *)ptLedBase);
	}
	else {
		if (eIntSrc == csp_led_get_isr(ptLedBase)) {
			csi_irq_disable((uint32_t *)ptLedBase);
		}
	}
}
/** \brief   write led data
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \param[in] byCom: to select which com
 *  \param[in] byData: SEG data for the specific com
 *  \return None
 */
void csi_led_write_data(csp_led_t *ptLedBase, uint8_t byCom, uint8_t byData)
{
	csp_led_set_setdata(ptLedBase, byCom, byData);
	
}
/** \brief  led blink control
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \param[in] eLedBlk: blink on or blink off
 * \param[in] hwOnMsk: on pattern
 * \return  None
 */
void csi_led_blink_control(csp_led_t *ptLedBase, csp_led_blk_e eLedBlk, uint16_t hwOnMsk)
{
	if(eLedBlk == LED_BLK_ON)
		csp_led_com_on(ptLedBase, (hwOnMsk & LED_BLK_MSK));
	else if(eLedBlk == LED_BLK_OFF)
		csp_led_com_off(ptLedBase, (hwOnMsk & LED_BLK_MSK));
}
/** \brief   led start
 * 
 * \param[in] ptLedBase: pointer of LED register structure
 * \return None
 */
void csi_led_lighton(csp_led_t *ptLedBase)
{
	csp_led_light_on(ptLedBase);
}
/** \brief led stop
 * 
 *  \param[in] ptLedBase: pointer of LED register structure
 *  \return None
 */
void csi_led_lightoff(csp_led_t *ptLedBase)
{	
	csp_led_light_off(ptLedBase);
}