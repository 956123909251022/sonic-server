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
#include "tkey.h"

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

/*************************************************************/
//CORET Interrupt
//EntryParameter:NONE
//ReturnValue:NONE
/*************************************************************/

void coret_int_handler(void) 
{
#if	CORET_INT_HANDLE_EN
    // ISR content ...
	tick_irqhandler();		//system coret 
	#if	TKEY_INT_HANDLE_EN
		#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
			csi_tkey_basecnt_process();
		#endif
	#endif
#endif
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
#if	IFC_INT_HANDLE_EN	
	// ISR content ...
	ifc_irqhandler();
	
#endif
}

void adc_int_handler(void) 
{
#if	ADC_INT_HANDLE_EN
	// ISR content ...
	adc_irqhandler(ADC0);
	
#endif
}

void ept0_int_handler(void) 
{	
#if	EPT_INT_HANDLE_EN	
	// ISR content ...
	ept_initen_irqhandler(EPT0);
	
#endif
}
void dma_int_handler(void)
{
#if DMA_INT_HANDLE_EN
	// ISR content ...	 
	dma_irqhandler(DMA);
	
#endif
}

void wwdt_int_handler(void)
{
#if WWDT_INT_HANDLE_EN
	 // ISR content ...
	 wwdt_irqhandler();
	 
#endif
}

void gpta0_int_handler(void) 
{
#if GPTA0_INT_HANDLE_EN
    gpta0_initen_irqhandler(GPTA0);
	
#endif
}

void gpta1_int_handler(void) 
{
#if GPTA1_INT_HANDLE_EN
    gpta1_initen_irqhandler(GPTA1);
	
#endif
}

void gptb0_int_handler(void) 
{
#if GPTB0_INT_HANDLE_EN
	gptb0_irqhandler_pro(GPTB0);
	
#endif
}

void gptb1_int_handler(void) 
{
#if GPTB1_INT_HANDLE_EN
    gptb1_irqhandler_pro(GPTB1);
	
#endif
}

void rtc_int_handler(void) 
{
#if	RTC_INT_HANDLE_EN
    //ISR content ...
	rtc_irqhandler(RTC);
	
#endif
}

void uart0_int_handler(void) 
{
#if	UART0_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART0, 0);
	
#endif
}
void uart1_int_handler(void) 
{
#if	UART1_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART1, 1);
	
#endif	
}
void uart2_int_handler(void) 
{
#if	UART2_INT_HANDLE_EN
    // ISR content ...
	uart_irqhandler(UART2, 2);
#endif
}

void usart0_int_handler(void) 
{
// when use lin demo,please uncomment lin_irqhandler function,and comment USART0_irqhandler function.
#if	USART0_INT_HANDLE_EN
	// ISR content ...
	usart_irqhandler(USART0, 0);
	//lin_irqhandler(LIN0, 0);
#endif
}

void sio_int_handler(void) 
{
#if	SIO_INT_HANDLE_EN
   // ISR content ...
   sio_irqhandler(SIO0);
   
#endif
}

void i2c_int_handler(void) 
{
#if	I2C_INT_HANDLE_EN
    // ISR content ...
	i2c_irqhandler(I2C0);
	
#endif
}
void spi0_int_handler(void) 
{
#if	SPI_INT_HANDLE_EN
    // ISR content ...
	spi_irqhandler(SPI0);
#endif
}

void exi0_int_handler(void) 			
{
#if	EXI0_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(0);
	
#endif
}
void exi1_int_handler(void) 
{
#if	EXI1_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(1);

#endif
}
void exi2_3_int_handler(void) 
{
#if	EXI2_3_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(2);
	
#endif
}
void exi4_9_int_handler(void) 
{
#if	EXI4_9_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(3);
	
#endif
}
void exi10_15_int_handler(void) 
{
#if	EXI10_15_INT_HANDLE_EN
    // ISR content ...
	gpio_irqhandler(4);
#endif
}

void cnta_int_handler(void) 
{
#if	CNTA_INT_HANDLE_EN
    // ISR content ...
	cnta_irqhandler(CNTA);
	
#endif
}
void tkey_int_handler(void) 
{
#if	TKEY_INT_HANDLE_EN
	#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
	// ISR content ...
	csi_tkey_int_process();
	#endif
#endif
}
void lpt_int_handler(void) 
{
#if	LPT_INT_HANDLE_EN
    // ISR content ...
	lpt_irqhandler(LPT);
#endif
}
void led_int_handler(void) 
{
#if	LED_INT_HANDLE_EN
	#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
		led_irqhandler(LED);
	#endif
#endif
}
void cmp_int_handler(void) 
{
#if	CMP_INT_HANDLE_EN
	#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
		// ISR content ...
		cmp_irqhandler(CMP0);
	#endif
#endif
}
void bt0_int_handler(void) 
{
#if	BT0_INT_HANDLE_EN
    // ISR content ...
	bt_irqhandler(BT0);
	
#endif
}

void bt1_int_handler(void) 
{
#if	BT1_INT_HANDLE_EN
    // ISR content ...
	bt_irqhandler(BT1);
	
#endif
}

void lcd_int_handler(void) 
{
#if	LCD_INT_HANDLE_EN
	#if	defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
		// ISR content ...
		lcd_irqhandler(LCD);
	#endif
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