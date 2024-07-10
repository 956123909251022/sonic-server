
 
/***********************************************************************//** 
 * \file  system.c
 * \brief  CSI Device System Source File
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/  

#include <soc.h>
#include <sys_clk.h>
#include <drv/tick.h>
#include <drv/iwdt.h>

/** \brief       initialize the system
 * 
 *  \param[in]   none
 *  \return      None
 */
__attribute__((weak)) void system_init(void)
{
	CK_CPU_DISALLNORMALIRQ;
	
	csi_iwdt_close();						//close iwdt
	csi_sysclk_config();					//sysclk config
	csi_get_sclk_freq();
	csi_get_pclk_freq();
	csi_tick_init();
	csi_clk_calib();
	
	CK_CPU_ENALLNORMALIRQ;
}
