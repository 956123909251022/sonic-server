/***********************************************************************//** 
 * \file  board_config.c
 * \brief  board configuration 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-11-23 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#include <sys_clk.h>
#include <drv/pm.h> 
#include <drv/pin.h> 
#include <drv/reliability.h> 
#include "board_config.h"
#include "sys_console.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
extern int32_t console_init(sys_console_t *handle);

/* externs variablesr-------------------------------------------------*/
/* variablesr---------------------------------------------------------*/
sys_console_t g_tConsole;

/// system clock configuration parameters to define source, source freq(if selectable), sdiv and pdiv
csi_clk_config_t g_tClkConfig = 
	{SRC_HFOSC, HFOSC_48M_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
	//{SRC_EMOSC, EMOSC_VALUE, SCLK_DIV1, PCLK_DIV1, 5556000, 5556000};
	//{SRC_IMOSC, IMOSC_5M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_HFOSC, HFOSC_48M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_IMOSC, IMOSC_4M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_IMOSC, IMOSC_2M_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_IMOSC, IMOSC_131K_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};
	//{SRC_ESOSC, ESOSC_VALUE, SCLK_DIV1, PCLK_DIV1,5556000, 5556000};


/** \brief board initialize config; 
 * 
 *  \param[in] none
 *  \return none
 */ 
__attribute__((weak)) void board_init(void)
{
    //g_tConsole config for print
	g_tConsole.uart_id = (uint32_t)CONSOLE_IDX;
    g_tConsole.baudrate = CONSOLE_BAUDRATE;
    g_tConsole.tx.pin = CONSOLE_TXD;   
    g_tConsole.tx.func = CONSOLE_TXD_FUNC;
 
    g_tConsole.rx.pin = CONSOLE_RXD;
    g_tConsole.rx.func = CONSOLE_RXD_FUNC;
	g_tConsole.uart = (csp_uart_t *)(APB_UART0_BASE + CONSOLE_IDX * 0x1000);
    console_init(&g_tConsole);
	
#ifdef CONFIG_USER_PM									//low power manage switch
	if(csi_get_rst_reason() & RST_SRC_SNOOZE_WKUP)		//wake up from snooze mode
	{
		g_tPmCore.wkup_frm_snooze = (void *)csi_ureg_read(USER_REG0);
		if(g_tPmCore.wkup_frm_snooze)
			g_tPmCore.wkup_frm_snooze();
	}
	else if(csi_get_rst_reason() & RST_SRC_SHD_WKUP)	//wake up from shutdown mode
	{
		g_tPmCore.wkup_frm_shutdown = (void *)csi_ureg_read(USER_REG0);
		if(g_tPmCore.wkup_frm_shutdown)
			g_tPmCore.wkup_frm_shutdown();
	}
#endif

}

