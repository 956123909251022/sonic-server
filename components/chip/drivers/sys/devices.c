/***********************************************************************//** 
 * \file  devices.c
 * \brief  source file for the devices
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-5-13 <td>V0.0  <td>ZJY    <td>initial
 * </table>
 * *********************************************************************
*/
//#include <csi_config.h>
#include <stdio.h>
#include <soc.h>
#include <sys_clk.h>
#include <drv/irq.h>

#include "csp.h"


///!!! DO NOT DEFINE HERE, DEFINE in "Define" in Compiler tab to make it globally available !!! 
//#define IS_CHIP_1101
//#define IS_CHIP_1102
//#define IS_CHIP_1103
//#define IS_CHIP_1104

VIC_Type  		*VIC0		= (VIC_Type*)(VIC_ADDR_BASE);
csp_coret_t	 	*CORET0 	= (csp_coret_t *)(CORET_ADDR_BASE);
csp_ifc_t 		*IFC   		= (csp_ifc_t *)(APB_IFC_BASE);
csp_syscon_t 	*SYSCON  	= (csp_syscon_t *)(APB_SYS_BASE);
csp_etb_t   	*ETCB 		= (csp_etb_t *)(APB_ETCB_BASE);
csp_adc_t 		*ADC0    	= (csp_adc_t *)(APB_ADC0_BASE);

#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
csp_cmp_t 		*CMP0    	= (csp_cmp_t *)(APB_CMP0_BASE);		
csp_cmp_t 		*CMP1    	= (csp_cmp_t *)(APB_CMP1_BASE);	
#endif
	
csp_gpio_t 		*GPIOA0		= (csp_gpio_t *)(APB_GPIOA0_BASE);		
csp_gpio_t 		*GPIOA1		= (csp_gpio_t *)(APB_GPIOA1_BASE);
csp_gpio_t 		*GPIOB0		= (csp_gpio_t *)(APB_GPIOB0_BASE);
csp_gpio_t 		*GPIOC0		= (csp_gpio_t *)(APB_GPIOC0_BASE);
csp_igrp_t		*GPIOGRP  	= (csp_igrp_t *)(APB_IGRP_BASE);
csp_uart_t 		*UART0   	= (csp_uart_t *)(APB_UART0_BASE);
csp_uart_t 		*UART1   	= (csp_uart_t *)(APB_UART1_BASE);
csp_uart_t 		*UART2   	= (csp_uart_t *)(APB_UART2_BASE);
csp_usart_t 	*USART0   	= (csp_usart_t *)(APB_USART0_BASE);
csp_spi_t  		*SPI0     	= (csp_spi_t *)(APB_SPI0_BASE);
csp_sio_t  		*SIO0     	= (csp_sio_t *)(APB_SIO0_BASE);
csp_i2c_t  		*I2C0     	= (csp_i2c_t *)(APB_I2C0_BASE);
csp_cnta_t  	*CNTA       = (csp_cnta_t *)(APB_CNTA_BASE);
csp_gpta_t 		*GPTA0     	= (csp_gpta_t *)(APB_GPTA0_BASE);
csp_gpta_t 		*GPTA1     	= (csp_gpta_t *)(APB_GPTA1_BASE);
csp_gptb_t 		*GPTB0     	= (csp_gptb_t *)(APB_GPTB0_BASE);
csp_gptb_t 		*GPTB1     	= (csp_gptb_t *)(APB_GPTB1_BASE);
csp_ept_t 		*EPT0     	= (csp_ept_t *)(APB_EPT0_BASE);
csp_lpt_t 		*LPT     	= (csp_lpt_t *)(APB_LPT_BASE) ;
csp_wwdt_t 		*WWDT      	= (csp_wwdt_t *)(APB_WWDT_BASE);
csp_bt_t  		*BT0 	   	= (csp_bt_t *)(APB_BT0_BASE) ;
csp_bt_t  		*BT1 	   	= (csp_bt_t *)(APB_BT1_BASE) ;
csp_crc_t 		*CRC      	= (csp_crc_t *)(AHB_CRC_BASE);
csp_rtc_t 		*RTC      	= (csp_rtc_t *)(APB_RTC_BASE);
csp_dma_t 		*DMA     	= (csp_dma_t *)(AHB_DMA_BASE);

#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
csp_led_t 		*LED      	= (csp_led_t *)(APB_LED_BASE);
#endif

#if	defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
csp_lcd_t 		*LCD      	= (csp_lcd_t *)(APB_LCD_BASE);
#endif

#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
csp_tkey_t 		*TKEY      	= (csp_tkey_t *)(APB_TKEY_BASE);
#endif

csp_lin_t 		*LIN0   	= (csp_lin_t *)(APB_USART0_BASE);


const csi_irqmap_t irq_map[] = {
	
	{CORET_ADDR_BASE,			CORET_IRQ_NUM	},
    {APB_SYS_BASE,				SYSCON_IRQ_NUM	},
    {APB_IFC_BASE,              IFC_IRQ_NUM		},
    {APB_ADC0_BASE,             ADC_IRQ_NUM		},
    {APB_EPT0_BASE,           	EPT0_IRQ_NUM	},
	{AHB_DMA_BASE,           	DMA_IRQ_NUM		},
    {APB_WWDT_BASE,           	WWDT_IRQ_NUM	},
    {APB_GPTA0_BASE,           	GPTA0_IRQ_NUM 	},
	{APB_GPTA1_BASE,           	GPTA1_IRQ_NUM 	},
	{APB_GPTB0_BASE,           	GPTB0_IRQ_NUM 	},
	{APB_GPTB1_BASE,           	GPTB1_IRQ_NUM 	},
    {APB_RTC_BASE,           	RTC_IRQ_NUM		},
    {APB_UART0_BASE,           	UART0_IRQ_NUM	},
    {APB_UART1_BASE,            UART1_IRQ_NUM	},
	{APB_UART2_BASE,           	UART2_IRQ_NUM	},
	{APB_USART0_BASE,           USART_IRQ_NUM	},
	{APB_SIO0_BASE,             SIO_IRQ_NUM		},
    {APB_I2C0_BASE,             I2C_IRQ_NUM		},
    {APB_SPI0_BASE,             SPI_IRQ_NUM		},
    {APB_CNTA_BASE,            	CNTA_IRQ_NUM	},
    {APB_TKEY_BASE,             TKEY_IRQ_NUM	},
    {APB_LPT_BASE,            	LPT_IRQ_NUM		},
	{APB_LED_BASE,              LED_IRQ_NUM		},
	{APB_CMP0_BASE,             CMP_IRQ_NUM		},
	{APB_CMP1_BASE,             CMP_IRQ_NUM		},
    {APB_BT0_BASE,              BT0_IRQ_NUM		},
	{APB_BT1_BASE,              BT1_IRQ_NUM		},
	{APB_LCD_BASE,              LCD_IRQ_NUM     },
    {0,  						0				}
};

const csi_clkmap_t clk_map[] = {
	
	{CORET_ADDR_BASE,			CORET_SYS_CLK	},
	{APB_IFC_BASE	,			IFC_SYS_CLK		},
    {APB_ADC0_BASE	,    		ADC_SYS_CLK		},
    {APB_TKEY_BASE	,  			TOUCH_SYS_CLK	},
    {ETCB_SYS_CLK	,   		ETCB_SYS_CLK	},
    {APB_UART0_BASE	,  			UART0_SYS_CLK	},
    {APB_UART1_BASE	,    		UART1_SYS_CLK	},
    {APB_UART2_BASE	,    		UART2_SYS_CLK	},
	{APB_USART0_BASE,    		USART0_SYS_CLK	},
    {APB_SPI0_BASE	,  			SPI_SYS_CLK		},
    {APB_SIO0_BASE	,   		SIO_SYS_CLK		},
    {APB_I2C0_BASE	, 			I2C_SYS_CLK		},
	{APB_EPT0_BASE	, 			EPT0_SYS_CLK	},
	{APB_GPTA0_BASE	, 			GPTA0_SYS_CLK	},
	{APB_GPTA1_BASE	, 			GPTA1_SYS_CLK	},
	{APB_GPTB0_BASE	, 			GPTB0_SYS_CLK	},
	{APB_GPTB1_BASE	, 			GPTB1_SYS_CLK	},
    {APB_WWDT_BASE	,   		WWDT_SYS_CLK	},
    {APB_LPT_BASE	, 			LPT_SYS_CLK		},
	{APB_CNTA_BASE	,   		CNTA_SYS_CLK	},
	{APB_RTC_BASE	,   		RTC_SYS_CLK		},
    {APB_BT0_BASE	,   		BT0_SYS_CLK		},
    {APB_BT1_BASE	,   		BT1_SYS_CLK		},
    {APB_LED_BASE	,  			LED_SYS_CLK		},
    {APB_CMP0_BASE	, 			CMP0_SYS_CLK	},
	{APB_CMP1_BASE	, 			CMP1_SYS_CLK	},
	{APB_LCD_BASE	, 			LCD_SYS_CLK		},
	{0x00U, 					0xFFFFFFFFU		}
};
