
 
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
#include <drv/irq.h>

/** \brief       initialize the system
 * 
 *  \param[in]   none
 *  \return      None
 */
__attribute__((weak)) void system_init(void)
{
	//config system clk, close interrupt
	CK_CPU_DISALLNORMALIRQ;					//disable total interrupt
	csi_iwdt_close();						//close iwdt
	csi_irq_set_all_prio(IRQ_PRIO_3);		//the all irq priority configure to the lowest
	csi_sysclk_config(tClkConfig);			//sysclk config
	csi_calc_clk_freq();					//calculate(updata) sclk and pclk
	csi_tick_init();						//init systick(coret)
	CK_CPU_ENALLNORMALIRQ;					//enable total interrupt
	
	//user add init
}
