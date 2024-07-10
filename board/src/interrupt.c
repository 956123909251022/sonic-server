/***********************************************************************//** 
 * \file  interrupt.c
 * \brief  source file for interrypt handlers
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

/* Includes ------------------------------------------------------------------*/

#include <soc.h>
#include <drv/gpio.h>
#include <drv/uart.h>
#include <drv/adc.h>
#include <drv/pin.h>
#include <drv/gpio.h>
#include <drv/iwdt.h>
#include <drv/cmp.h>
#include "board_config.h"
#include "csp.h"
#include "ifc.h"
#include "iic.h"

/* externs function--------------------------------------------------------*/
extern void bt_irqhandler(csp_bt_t *ptBtBase);
extern void tick_irqhandler(void);					
extern void dma_irqhandler(csp_dma_t *ptDmaBase);
extern void uart_irqhandler(csp_uart_t *ptUartBase,uint8_t byIdx);
extern void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx);
extern void lin_irqhandler(csp_lin_t *ptLinBase, uint8_t byIdx);
extern void cnta_irqhandler(csp_cnta_t *ptCntaBase);
extern void spi_irqhandler(csp_spi_t *ptSpiBase);
extern void sio_irqhandler(csp_sio_t *ptSioBase);
extern void adc_irqhandler(csp_adc_t *ptAdcBase);
extern void ept_initen_irqhandler(csp_ept_t *ptEptBase);
extern void gpta0_initen_irqhandler(csp_gpta_t *ptGptaBase);
extern void gpta1_initen_irqhandler(csp_gpta_t *ptGptaBase);
extern void gptb0_irqhandler_pro(csp_gptb_t *ptGptbBase);
extern void gptb1_irqhandler_pro(csp_gptb_t *ptGptbBase);
extern void gpio_irqhandler(uint8_t byExiNum);
extern void ifc_irqhandler(void);
extern void lcd_irqhandler(csp_lcd_t *ptLcdBase);
extern void lpt_irqhandler(csp_lpt_t *ptLptBase);
extern void cmp_irqhandler(csp_cmp_t *ptCmpBase);
extern void led_irqhandler(csp_led_t *ptLedBase);
extern void wwdt_irqhandler(void);
extern void rtc_irqhandler(csp_rtc_t *ptRtcBase);


/* private function--------------------------------------------------------*/

/* extern variablesr------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/

uint32_t g_wAdcData[16];
uint8_t g_byAdcDone = 0;
/*************************************************************/
//CORET Interrupt
//EntryParameter:NONE
//ReturnValue:NONE
/*************************************************************/

void coret_int_handler(void) 
{
    // ISR content ...
	tick_irqhandler();		//system coret 
}

void syscon_int_handler(void) 
{
    // ISR content ...

	if(csp_syscon_get_int_st(SYSCON) & LVD_INT)
	{
		nop;
		csp_syscon_int_clr(SYSCON, LVD_INT);
	}
	
	if(csp_syscon_get_int_st(SYSCON) & IWDT_INT)
	{
		nop;
		//csi_pin_toggle(PA05);
		csp_syscon_int_clr(SYSCON, IWDT_INT);
		//csi_iwdt_feed();
	}
}

/** \brief only used when DFLASH parallel mode PGM is enabled
 *  \param none
 *  \return none
 */

void ifc_int_handler(void) 
{	
	// ISR content ...
	ifc_irqhandler();
}

void adc_int_handler(void) 
{
	// ISR content ...
	adc_irqhandler(ADC0);
	
}

void ept0_int_handler(void) 
{	
	// ISR content ...
	ept_initen_irqhandler(EPT0);
	
}
void dma_int_handler(void)
{
	// ISR content ...	 
	dma_irqhandler(DMA);
}

void wwdt_int_handler(void)
{
	 // ISR content ...
	 wwdt_irqhandler();
}

void gpta0_int_handler(void) 
{
    gpta0_initen_irqhandler(GPTA0);
}

void gpta1_int_handler(void) 
{
    gpta1_initen_irqhandler(GPTA1);
}

void gptb0_int_handler(void) 
{
	gptb0_irqhandler_pro(GPTB0);
}

void gptb1_int_handler(void) 
{
    gptb1_irqhandler_pro(GPTB1);
}

void rtc_int_handler(void) 
{
    //ISR content ...
	rtc_irqhandler(RTC);
}

void uart0_int_handler(void) 
{
    // ISR content ...
	uart_irqhandler(UART0, 0);
}
void uart1_int_handler(void) 
{
    // ISR content ...
	uart_irqhandler(UART1, 1);
}
void uart2_int_handler(void) 
{
    // ISR content ...
	uart_irqhandler(UART2, 2);
}

void usart0_int_handler(void) 
{// when use lin demo,please uncomment lin_irqhandler function,and comment USART0_irqhandler function.

	// ISR content ...
	usart_irqhandler(USART0, 0);
	//lin_irqhandler(LIN0, 0);
}

void sio_int_handler(void) 
{
   // ISR content ...
   sio_irqhandler(SIO0);
}

void i2c_int_handler(void) 
{
    // ISR content ...
	i2c_irqhandler(I2C0);
}
void spi0_int_handler(void) 
{
    // ISR content ...
	spi_irqhandler(SPI0);
}

void exi0_int_handler(void) 			
{
    // ISR content ...
	gpio_irqhandler(0);
}
void exi1_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(1);
}
void exi2_3_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(2);
}
void exi4_9_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(3);
}
void exi10_15_int_handler(void) 
{
    // ISR content ...
	gpio_irqhandler(4);
}

void cnta_int_handler(void) 
{
    // ISR content ...
	cnta_irqhandler(CNTA);
}
void tkey_int_handler(void) 
{
    // ISR content ...
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
	
#endif

}
void lpt_int_handler(void) 
{
    // ISR content ...
	lpt_irqhandler(LPT);
}
void led_int_handler(void) 
{
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
	led_irqhandler(LED);
#endif

}
void cmp_int_handler(void) 
{
    // ISR content ...
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
	cmp_irqhandler(CMP0);
#endif

}
void bt0_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT0);
}

void bt1_int_handler(void) 
{
    // ISR content ...
	bt_irqhandler(BT1);
}

void lcd_int_handler(void) 
{
    // ISR content ...
#if	defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
	lcd_irqhandler(LCD);
#endif

}

/*************************************************************/
/*************************************************************/
/*************************************************************/
void PriviledgeVioHandler(void) 
{
    // ISR content ...
	while(1){};
}

void SystemDesPtr(void) 
{
    // ISR content ...
	while(1){};
}

void MisalignedHandler(void) 
{
    // ISR content ...
	while(1){};
}

void IllegalInstrHandler(void) 
{
    // ISR content ...
	while(1){};
}

void AccessErrHandler(void) 
{
    // ISR content ...
	while(1){};
}

void BreakPointHandler(void) 
{
    // ISR content ...
	while(1){};
}

void UnrecExecpHandler(void) 
{
    // ISR content ...
	while(1){};
}

void Trap0Handler(void) 
{
    // ISR content ...
	while(1){};
}

void Trap1Handler(void) 
{
    // ISR content ...
	while(1){};
}

void Trap2Handler(void) 
{
    // ISR content ...
	while(1){};
}

void Trap3Handler(void) 
{
    // ISR content ...
	while(1){};

}

void PendTrapHandler(void) 
{
    // ISR content ...
	while(1){};
}




/******************* (C) COPYRIGHT 2020 APT Chip *****END OF FILE****/