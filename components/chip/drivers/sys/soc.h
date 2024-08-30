
 
/***********************************************************************//** 
 * \file  soc.h
 * \brief  CSI Core Peripheral Access Layer Header File for
 *           CSKYSOC Device Series
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-13 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/  

#ifndef _SOC_H_
#define _SOC_H_

#include <stdint.h>
#include <csi_core.h>
#include <drv/clk.h>
#include <drv/porting.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifndef ISOSC_VALU
#define ISOSC_VALUE				27000U
#endif

#ifndef ESOSC_VALUE
#define ESOSC_VALUE             32768U
#endif

#ifndef EMOSC_32K_VALUE
#define EMOSC_32K_VALUE			32768U
#endif

#ifndef IMOSC_5M_VALUE
#define IMOSC_5M_VALUE			5556000U
#endif
#ifndef IMOSC_4M_VALUE
#define IMOSC_4M_VALUE          4194000U
#endif
#ifndef IMOSC_2M_VALUE
#define IMOSC_2M_VALUE          2097000U
#endif
#ifndef IMOSC_131K_VALUE
#define IMOSC_131K_VALUE        131072U
#endif

#ifndef HFOSC_48M_VALUE
#define HFOSC_48M_VALUE     	48000000U
#endif
#ifndef HFOSC_24M_VALUE
#define HFOSC_24M_VALUE         24000000U
#endif
#ifndef HFOSC_12M_VALUE
#define HFOSC_12M_VALUE         12000000U
#endif
#ifndef HFOSC_6M_VALUE
#define HFOSC_6M_VALUE          6000000U
#endif


//UARTx id number
#define UART_IDX_NUM   		3
#define UART_RECV_MAX_LEN	128

//USARTx id number
#define USART_IDX_NUM   	1
#define USART_RECV_MAX_LEN	128

//DMA  id number
//max channel  number
#define DMA_IDX_NUM			1
#define DMA_CH_MAX_NUM		4

//ETB dma start channel  number
//ETB max channel  number
#define ETB_CH_DMA_STAR		ETB_CH8
#define ETB_CH_MAX_NUM		12

//FLASH
#define PFLASH_PAGES		256
#define DFLASH_PAGES		32

//INTERRUPT Number
typedef enum {
	CORET_IRQ_NUM			= 0U,						 					
	SYSCON_IRQ_NUM			= 1U,
	IFC_IRQ_NUM				= 2U,						 					
	ADC_IRQ_NUM				= 3U,
	EPT0_IRQ_NUM			= 4U,
	DMA_IRQ_NUM      		= 5U,
	WWDT_IRQ_NUM			= 6U,	
	EXI0_IRQ_NUM			= 7U,			//EXI GROUP0/GROUP16
	EXI1_IRQ_NUM			= 8U,			//EXI GROUP1/GROUP17
	GPTA0_IRQ_NUM			= 9U,
	GPTA1_IRQ_NUM      		= 10U,
	GPTB0_IRQ_NUM      		= 11U,
	RTC_IRQ_NUM     		= 12U,
	UART0_IRQ_NUM    		= 13U,
	UART1_IRQ_NUM    		= 14U,
	UART2_IRQ_NUM      		= 15U,
	USART_IRQ_NUM      		= 16U,
	GPTB1_IRQ_NUM     		= 17U,
	SIO_IRQ_NUM				= 18U,
	I2C_IRQ_NUM    			= 19U,
	SPI_IRQ_NUM				= 20U,
	EXI2_IRQ_NUM    		= 21U,			//EXI GROUP2~3/GROUP18~19
	EXI3_IRQ_NUM    		= 22U,			//EXI GROUP4~9
	EXI4_IRQ_NUM    		= 23U,			//EXI GROUP10~15
	CNTA_IRQ_NUM			= 24U,
	TKEY_IRQ_NUM    		= 25U,
	LPT_IRQ_NUM      		= 26U,
	LED_IRQ_NUM      		= 27U,
	CMP_IRQ_NUM     		= 28U,
	BT0_IRQ_NUM     		= 29U,
	BT1_IRQ_NUM      		= 30U,
	LCD_IRQ_NUM      		= 31U,
} irqn_type_e;


#define	GPIO_NUM		4
typedef enum {
	GPIOA0_PIN_NUM	= 16,
	GPIOA1_PIN_NUM	= 14,
	GPIOB0_PIN_NUM	= 12,
	GPIOC0_PIN_NUM	= 2
}pin_num_t;

typedef enum {
	PA00			= 0U,		//PA0.0->PA0.15; num = 16
	PA01			= 1U,		
	PA02			= 2U,
    PA03			= 3U,
    PA04			= 4U,
    PA05			= 5U,
    PA06			= 6U,
    PA07			= 7U,
    PA08			= 8U,
    PA09			= 9U,
    PA010			= 10U,
    PA011			= 11U,
    PA012			= 12U,
    PA013			= 13U,
    PA014			= 14U,
    PA015			= 15U,
	
	PA10			= 16U,		//PA1.0->PA1.13; num = 14
	PA11			= 17U,		
	PA12			= 18U,		
	PA13			= 19U,		
	PA14			= 20U,		
	PA15			= 21U,		
	PA16			= 22U,		
	PA17			= 23U,		
	PA18			= 24U,		
	PA19			= 25U,		
	PA110			= 26U,		
	PA111			= 27U,		
	PA112			= 28U,		
	PA113			= 29U,
	
    PB00			= 30U,		//PB0.0->PB0.11; num = 12
    PB01			= 31U,
    PB02			= 32U,
    PB03			= 33U,
    PB04			= 34U,
    PB05			= 35U,
	PB06			= 36U,
    PB07			= 37U,
    PB08			= 38U,
    PB09			= 39U,
    PB010			= 40U,
    PB011			= 41U,
	
	PC00			= 42U,		//PC0.0->PC0.1; num = 2
    PC01			= 43U,
} pin_name_e;


typedef enum {
	
	PC00_GPD				=  0U,
	PC00_INPUT				=  1U,		//input
	PC00_OUTPUT				=  2U,		//output
	PC00_OUTPUT_MONI		=  3U,		//output with monitor
    PC00_SIO				=  4U,			//PC0.0
    PC00_BT1_OUT          	=  5U,
	PC00_GPTB0_CHAX			=  6U,
	PC00_CLO				=  7U,
	PC00_AVGND				=  9U,
	PC00_ADC_AIN17          =  10U,
	PC00_CMP_IN7          	=  10U,
	PC00_TKEY_CH23        	=  10U,
	
	PA00_GPD				=  0U,
	PA00_INPUT				=  1U,		//input
	PA00_OUTPUT				=  2U,		//output
	PA00_OUTPUT_MONI		=  3U,		//output with monitor
    PA00_RTC_ALM			=  4U,		//PA0.0
    PA00_CMP1_OUT           =  5U,
    PA00_GPTB0_CHAY         =  6U,
    PA00_GPTB0_CHB          =  7U,
    PA00_AVREF           	=  10U,
	PA00_FVR        		=  10U,
    PA00_ADC_AIN0         	=  10U,
	PA00_CMP_IN0         	=  10U,
	PA00_TKEY_CH24        	=  10U,
	
	PA01_GPD				=  0U,
	PA01_INPUT				=  1U,		//input
	PA01_OUTPUT				=  2U,		//output
	PA01_OUTPUT_MONI		=  3U,		//output with monitor
	PA01_OSC_SXI			=  4U,		//PA0.1
    PA01_LPT_INOUT          =  5U,
    PA01_GPTB1_CHAX         =  6U,
    PA01_SIO           		=  7U,
	PA01_ADC_AIN1         	=  10U,
	PA01_TKEY_CH25        	=  10U,
	
	PA02_GPD				=  0U,
	PA02_INPUT				=  1U,		//input
	PA02_OUTPUT				=  2U,		//output
	PA02_OUTPUT_MONI		=  3U,		//output with monitor
    PA02_OSC_SXO			=  4U,		//PA0.2
    PA02_BT1_OUT            =  5U,
    PA02_GPTB1_CHAY         =  6U,
    PA02_GPTB1_CHB          =  7U,
	PA02_ADC_AIN18         	=  10U,
	PA02_TKEY_CH26        	=  10U,
	
	PA03_GPD				=  0U,
	PA03_INPUT				=  1U,		//input
	PA03_OUTPUT				=  2U,		//output
	PA03_OUTPUT_MONI		=  3U,		//output with monitor
	PA03_OSC_XI				=  4U,		//PA0.3
    PA03_I2C0_SDA           =  5U,
    PA03_SIO            	=  6U,
    PA03_I2C0_SCL          	=  7U,
	PA03_ADC_AIN2         	=  10U,
	PA03_CMP_IN2         	=  10U,
	PA03_TKEY_CH27        	=  10U,
	
	PA04_GPD				=  0U,
	PA04_INPUT				=  1U,		//input
	PA04_OUTPUT				=  2U,		//output
	PA04_OUTPUT_MONI		=  3U,		//output with monitor
	PA04_OSC_XO				=  4U,		//PA0.4
    PA04_I2C0_SCL           =  5U,
    PA04_LPT_INOUT          =  6U,
    PA04_I2C0_SDA           =  7U,
	PA04_INTVREF          	=  9U,
	PA04_CMP_IN3         	=  10U,
	PA04_TKEY_CH28        	=  10U,
	
	PA05_GPD				=  0U,
	PA05_INPUT				=  1U,		//input
	PA05_OUTPUT				=  2U,		//output
	PA05_OUTPUT_MONI		=  3U,		//output with monitor
	PA05_RESETB				=  4U,		//PA0.5
    PA05_BT1_OUT            =  5U,
    PA05_SIO          		=  6U,
    PA05_CLO           		=  7U,
    PA05_TKEY_C0            =  10U,
	
	PB00_GPD				=  0U,
	PB00_INPUT				=  1U,		//input
	PB00_OUTPUT				=  2U,		//output
	PB00_OUTPUT_MONI		=  3U,		//output with monitor
	PB00_UART2_TX			=  4U,		//PB0.0
    PB00_GPTB0_CHAX         =  5U,
    PB00_I2C0_SCL           =  6U,
    PB00_CLO	  	    	=  7U,
	PB00_ADC_AIN3         	=  10U,
	PB00_CMP_IN4         	=  10U,
	PB00_TKEY_CH29        	=  10U,
	PB00_ELVI       		=  10U,
	
	PB01_GPD				=  0U,
	PB01_INPUT				=  1U,		//input
	PB01_OUTPUT				=  2U,		//output
	PB01_OUTPUT_MONI		=  3U,		//output with monitor
	PB01_UART2_RX			=  4U,		//PB0.1
    PB01_GPTB0_CHAY         =  5U,
    PB01_I2C0_SDA           =  6U,
    PB01_SIO		  	    =  7U,
	PB01_ADC_AIN4         	=  10U,
	PB01_CMP_IN5         	=  10U,
	PB01_TKEY_CH30        	=  10U,
	
	PB02_GPD				=  0U,
	PB02_INPUT				=  1U,		//input
	PB02_OUTPUT				=  2U,		//output
	PB02_OUTPUT_MONI		=  3U,		//output with monitor
	PB02_GPTA0_CHA			=  4U,		//PB0.2
    PB02_GPTB0_CHAX         =  5U,
    PB02_USART0_TX          =  6U,
    PB02_UART1_TX  	        =  7U,
	PB02_ADC_AIN5         	=  10U,
	PB02_CMP_IN6         	=  10U,
	PB02_TKEY_CH31        	=  10U,

	PA06_GPD				=  0U,
	PA06_INPUT				=  1U,		//input
	PA06_OUTPUT				=  2U,		//output
	PA06_OUTPUT_MONI		=  3U,		//output with monitor
	PA06_LCD_COM0			=  4U,		//PA0.6
    PA06_GPTB0_CHB          =  5U,
    PA06_USART0_RX          =  6U,
    PA06_UART1_RX          	=  7U,
	PA06_SPI0_MOSI			=  8U,
	PA06_LED_S0     		=  9U,
	PA06_ADC_AIN6           =  10U,
	
	PA07_GPD				=  0U,
	PA07_INPUT				=  1U,		//input
	PA07_OUTPUT				=  2U,		//output
	PA07_OUTPUT_MONI		=  3U,		//output with monitor
	PA07_LCD_COM1			=  4U,		//PA0.7
    PA07_LPT_INOUT          =  5U,
    PA07_I2C0_SCL           =  6U,
    PA07_USART0_CK  	    =  7U,
	PA07_SPI0_NSS			=  8U,
	PA07_LED_S1    			=  9U,
	PA07_ADC_AIN7           =  10U,
	
	PA08_GPD				=  0U,
	PA08_INPUT				=  1U,		//input
	PA08_OUTPUT				=  2U,		//output
	PA08_OUTPUT_MONI		=  3U,		//output with monitor
	PA08_LCD_COM2			=  4U,		//PA0.8
	PA08_LCD_SEG0			=  4U,			
    PA08_CMP1_OUT           =  5U,
    PA08_I2C0_SDA           =  6U,
    PA08_USART0_TX	  	    =  7U,
	PA08_SPI0_SCK			=  8U,
	PA08_LED_S2          	=  9U,
	PA08_ADC_AIN8           =  10U,
	
	PA09_GPD				=  0U,
	PA09_INPUT				=  1U,		//input
	PA09_OUTPUT				=  2U,		//output
	PA09_OUTPUT_MONI		=  3U,		//output with monitor
	PA09_LCD_COM3			=  4U,		//PA0.9		
	PA09_LCD_SEG1			=  4U,		
    PA09_EBI0           	=  5U,
    PA09_GPTA0_CHA          =  6U,
    PA09_BT1_OUT	  	    =  7U,
	PA09_SPI0_MISO			=  8U,
	PA09_LED_S3           	=  9U,
	PA09_ADC_AIN9          	=  10U,
	
	PA010_GPD				=  0U,
	PA010_INPUT				=  1U,		//input
	PA010_OUTPUT			=  2U,		//output
	PA010_OUTPUT_MONI		=  3U,		//output with monitor
	PA010_LCD_COM4			=  4U,		//PA0.10
	PA010_LCD_SEG2			=  4U,			
    PA010_EPT_CHAY          =  5U,
    PA010_GPTA0_CHB         =  6U,
    PA010_GPTB1_CHAX	  	=  7U,
	PA010_SPI0_MOSI			=  8U,
	PA010_LED_S4         	=  9U,
	PA010_ADC_AIN10         =  10U,

	PA011_GPD				=  0U,
	PA011_INPUT				=  1U,		//input
	PA011_OUTPUT			=  2U,		//output
	PA011_OUTPUT_MONI		=  3U,		//output with monitor
	PA011_LCD_COM5			=  4U,		//PA0.11
	PA011_LCD_SEG3			=  4U,		
    PA011_EPT_CHBY          =  5U,
    PA011_GPTA1_CHA         =  6U,
	PA011_LED_S0			=  8U,
	PA011_LED_S5         	=  9U,
	PA011_ADC_AIN11         =  10U,

	PA012_GPD				=  0U,
	PA012_INPUT				=  1U,		//input
	PA012_OUTPUT			=  2U,		//output
	PA012_OUTPUT_MONI		=  3U,		//output with monitor
	PA012_LCD_COM6			=  4U,		//PA0.12
	PA012_LCD_SEG4			=  4U,	
    PA012_EPT_CHCY          =  5U,
    PA012_GPTA1_CHB         =  6U,
    PA012_LPT_INOUT	  	    =  7U,
	PA012_LED_S1			=  8U,
	PA012_LED_S6        	=  9U,
	PA012_ADC_AIN12        	=  10U,
	
	PA013_GPD				=  0U,
	PA013_INPUT				=  1U,		//input
	PA013_OUTPUT			=  2U,		//output
	PA013_OUTPUT_MONI		=  3U,		//output with monitor
	PA013_LCD_COM7			=  4U,		//PA0.13
	PA013_LCD_SEG5			=  4U,
    PA013_GPTB0_CHAX        =  5U,
    PA013_SIO         		=  6U,
	PA013_LED_S3			=  8U,			//?
	PA013_LED_S7        	=  9U,
	PA013_ADC_AIN13         =  10U,
	
	PA014_GPD				=  0U,
	PA014_INPUT				=  1U,		//input
	PA014_OUTPUT			=  2U,		//output
	PA014_OUTPUT_MONI		=  3U,		//output with monitor
	PA014_LCD_SEG6			=  4U,		//PA0.14
    PA014_GPTB0_CHAY        =  5U,
    PA014_GPTB0_CHB         =  6U,
    PA014_LED_S0		  	=  8U,
	PA014_TKEY_CH0			=  10U,
	
	PA015_GPD				=  0U,
	PA015_INPUT				=  1U,		//input
	PA015_OUTPUT			=  2U,		//output
	PA015_OUTPUT_MONI		=  3U,		//output with monitor
	PA015_LCD_SEG7			=  4U,			//PA0.15
    PA015_GPTB1_CHAX        =  5U,
    PA015_LED_S1            =  8U,
    PA015_TKEY_CH1		  	=  10U,
	
	PB03_GPD				=  0U,
	PB03_INPUT				=  1U,		//input
	PB03_OUTPUT				=  2U,		//output
	PB03_OUTPUT_MONI		=  3U,		//output with monitor
	PB03_LCD_SEG8			=  4U,		//PB0.3
    PB03_GPTB1_CHAY         =  5U,
    PB03_GPTB1_CHB          =  6U,
	PB03_LED_S2				=  8U,
	PB03_ADC_AIN14          =  10U,
	
	PC01_GPD				=  0U,
	PC01_INPUT				=  1U,		//input
	PC01_OUTPUT				=  2U,		//output
	PC01_OUTPUT_MONI		=  3U,		//output with monitor
	PC01_LCD_SEG9			=  4U,		//PC0.1
    PC01_GPTA0_CHA          =  5U,
	PC01_LED_S3				=  8U,
	PC01_ADC_AIN15          =  10U,
	
	PB04_GPD				=  0U,
	PB04_INPUT				=  1U,		//input
	PB04_OUTPUT				=  2U,		//output
	PB04_OUTPUT_MONI		=  3U,		//output with monitor
	PB04_LCD_SEG10			=  4U,		//PB0.4
    PB04_GPTB0_CHB         	=  5U,
    PB04_GPTA1_CHA          =  6U,
    PB04_EBI1		  		=  7U,
	PB04_LED_S4				=  8U,
	PB04_ADC_AIN16          =  10U,
	
	PA10_GPD				=  0U,
	PA10_INPUT				=  1U,		//input
	PA10_OUTPUT				=  2U,		//output
	PA10_OUTPUT_MONI		=  3U,		//output with monitor
	PA10_LCD_SEG11			=  4U,		//PA1.0
    PA10_EPT_CHAY      		=  5U,
    PA10_GPTA1_CHB          =  6U,
	PA10_LED_S5             =  8U,
    PA10_TKEY_CH2		  	=  10U,
	
	PA11_GPD				=  0U,
	PA11_INPUT				=  1U,		//input
	PA11_OUTPUT				=  2U,		//output
	PA11_OUTPUT_MONI		=  3U,		//output with monitor
	PA11_LCD_SEG12			=  4U,		//PA1.1
    PA11_EPT_CHBY      		=  5U,
    PA11_GPTB0_CHAX         =  6U,
	PA11_LED_S6             =  8U,
    PA11_TKEY_CH3		  	=  10U,
	
	PA12_GPD				=  0U,
	PA12_INPUT				=  1U,		//input
	PA12_OUTPUT				=  2U,		//output
	PA12_OUTPUT_MONI		=  3U,		//output with monitor
	PA12_LCD_SEG13			=  4U,		//PA1.2
    PA12_EPT_CHCY      		=  5U,
    PA12_GPTB0_CHAY         =  6U,
	PA12_GPTB0_CHB          =  7U,
	PA12_LED_S7             =  8U,
    PA12_TKEY_CH4		  	=  10U,
	
	PA13_GPD				=  0U,
	PA13_INPUT				=  1U,		//input
	PA13_OUTPUT				=  2U,		//output
	PA13_OUTPUT_MONI		=  3U,		//output with monitor
	PA13_LCD_SEG14			=  4U,		//PA1.3
    PA13_EPT_CHAX      		=  5U,
    PA13_CLO        		=  6U,
	PA13_USART0_CK         	=  7U,
    PA13_TKEY_CH5		  	=  10U,
	
	PA14_GPD				=  0U,
	PA14_INPUT				=  1U,		//input
	PA14_OUTPUT				=  2U,		//output
	PA14_OUTPUT_MONI		=  3U,		//output with monitor
	PA14_LCD_SEG15			=  4U,		//PA1.4
    PA14_EPT_CHBX      		=  5U,
    PA14_EBI2        		=  6U,
	PA14_USART0_TX          =  7U,
	PA14_UART1_TX           =  8U,
    PA14_TKEY_CH6		  	=  10U,
	
	PA15_GPD				=  0U,
	PA15_INPUT				=  1U,		//input
	PA15_OUTPUT				=  2U,		//output
	PA15_OUTPUT_MONI		=  3U,		//output with monitor
	PA15_LCD_SEG16			=  4U,		//PA1.5
    PA15_EPT_CHCX      		=  5U,
    PA15_EBI3        		=  6U,
	PA15_USART0_RX          =  7U,
	PA15_UART1_RX           =  8U,
    PA15_TKEY_CH7		  	=  10U,
	
	PA16_GPD				=  0U,
	PA16_INPUT				=  1U,		//input
	PA16_OUTPUT				=  2U,		//output
	PA16_OUTPUT_MONI		=  3U,		//output with monitor
	PA16_LCD_SEG17			=  4U,		//PA1.6
    PA16_EPT_CHD     		=  5U,
    PA16_CMP0_OUT        	=  6U,
	PA16_UART0_TX         	=  7U,
	PA16_LED_COM0           =  8U,
    PA16_TKEY_CH8		  	=  10U,
	
	PB05_GPD				=  0U,
	PB05_INPUT				=  1U,		//input
	PB05_OUTPUT				=  2U,		//output
	PB05_OUTPUT_MONI		=  3U,		//output with monitor
	PB05_LCD_SEG18			=  4U,		//PB0.5
    PB05_SIO          		=  5U,
    PB05_CMP1_OUT           =  6U,
    PB05_UART0_RX		  	=  7U,
	PB05_LED_COM1 			=  8U,
	PB05_TKEY_CH9          	=  10U,
	
	PA17_GPD				=  0U,
	PA17_INPUT				=  1U,		//input
	PA17_OUTPUT				=  2U,		//output
	PA17_OUTPUT_MONI		=  3U,		//output with monitor
	PA17_LCD_SEG19			=  4U,		//PA1.7
    PA17_DBUG_SWDIO     	=  5U,
    PA17_CNTA_REM       	=  6U,
	PA17_GPTA1_CHA         	=  7U,
	PA17_LED_COM2           =  8U,
    PA17_TKEY_CH10		  	=  10U,
	
	PB06_GPD				=  0U,
	PB06_INPUT				=  1U,		//input
	PB06_OUTPUT				=  2U,		//output
	PB06_OUTPUT_MONI		=  3U,		//output with monitor
	PB06_LCD_SEG20			=  4U,		//PB0.6
    PB06_I2C0_SCL          	=  5U,
    PB06_GPTA0_CHA          =  6U,
    PB06_UART2_TX		  	=  7U,
	PB06_LED_COM3 			=  8U,
	PB06_TKEY_CH11          =  10U,
	
	PB07_GPD				=  0U,
	PB07_INPUT				=  1U,		//input
	PB07_OUTPUT				=  2U,		//output
	PB07_OUTPUT_MONI		=  3U,		//output with monitor
	PB07_LCD_SEG21			=  4U,		//PB0.7
    PB07_I2C0_SDA          	=  5U,
    PB07_GPTA1_CHA          =  6U,
    PB07_UART2_RX		  	=  7U,
	PB07_LED_COM4			=  8U,
	PB07_TKEY_CH12          =  10U,
	
	PA18_GPD				=  0U,
	PA18_INPUT				=  1U,		//input
	PA18_OUTPUT				=  2U,		//output
	PA18_OUTPUT_MONI		=  3U,		//output with monitor
	PA18_LCD_SEG22			=  4U,		//PA1.8
    PA18_DBUG_SWCLK     	=  5U,
    PA18_USART0_TX       	=  6U,
	PA18_LPT_INOUT        	=  7U,
	PA18_LED_COM5           =  8U,
    PA18_TKEY_CH13		  	=  10U,
	
	PB08_GPD				=  0U,
	PB08_INPUT				=  1U,		//input
	PB08_OUTPUT				=  2U,		//output
	PB08_OUTPUT_MONI		=  3U,		//output with monitor
	PB08_LCD_SEG23			=  4U,		//PB0.8
    PB08_UART0_TX          	=  5U,
    PB08_USART0_RX          =  6U,
    PB08_SPI0_NSS		  	=  7U,
	PB08_LED_COM6			=  8U,
	PB08_TKEY_CH14          =  10U,
	
	PB09_GPD				=  0U,
	PB09_INPUT				=  1U,		//input
	PB09_OUTPUT				=  2U,		//output
	PB09_OUTPUT_MONI		=  3U,		//output with monitor
	PB09_LCD_SEG24			=  4U,		//PB0.9
    PB09_UART0_RX          	=  5U,
    PB09_GPTB1_CHB          =  6U,
    PB09_SPI0_SCK		  	=  7U,
	PB09_USART0_CK			=  8U,
	PB09_TKEY_CH15          =  10U,
	
	PB010_GPD				=  0U,
	PB010_INPUT				=  1U,		//input
	PB010_OUTPUT			=  2U,		//output
	PB010_OUTPUT_MONI		=  3U,		//output with monitor
	PB010_LCD_SEG25			=  4U,		//PB0.10
    PB010_GPTA0_CHA         =  5U,
    PB010_GPTA1_CHA         =  6U,
    PB010_SPI0_MISO		  	=  7U,
	PB010_LED_COM6			=  8U,
	PB010_TKEY_CH16         =  10U,
	
	PB011_GPD				=  0U,
	PB011_INPUT				=  1U,		//input
	PB011_OUTPUT			=  2U,		//output
	PB011_OUTPUT_MONI		=  3U,		//output with monitor
	PB011_LCD_SEG26			=  4U,		//PB0.11
    PB011_GPTA0_CHB         =  5U,
    PB011_GPTA1_CHB         =  6U,
    PB011_SPI0_MOSI		  	=  7U,
	PB011_LED_COM7			=  8U,
	PB011_USART0_CK			=  9U,
	PB011_TKEY_CH17         =  10U,
	
	PA19_GPD				=  0U,
	PA19_INPUT				=  1U,		//input
	PA19_OUTPUT				=  2U,		//output
	PA19_OUTPUT_MONI		=  3U,		//output with monitor
	PA19_LCD_SEG27			=  4U,		//PA1.9
    PA19_GPTB0_CHAY        	=  5U,
    PA19_BT0_OUT       		=  6U,
    PA19_USART0_TX		  	=  7U,
	PA19_LED_COM8			=  8U,
	PA19_I2C0_SCL			=  9U,
	PA19_TKEY_CH18        	=  10U,
	
	PA110_GPD				=  0U,
	PA110_INPUT				=  1U,		//input
	PA110_OUTPUT			=  2U,		//output
	PA110_OUTPUT_MONI		=  3U,		//output with monitor
	PA110_LCD_SEG28			=  4U,		//PA1.10
    PA110_GPTB1_CHAY        =  5U,
    PA110_CNTA_REM       	=  6U,
    PA110_USART0_RX		  	=  7U,
	PA110_BT1_OUT			=  8U,
	PA110_I2C0_SDA			=  9U,
	PA110_CMP_IN1        	=  10U,
	PA110_TKEY_CH19        	=  10U,
	
	PA111_GPD				=  0U,
	PA111_INPUT				=  1U,		//input
	PA111_OUTPUT			=  2U,		//output
	PA111_OUTPUT_MONI		=  3U,		//output with monitor
	PA111_LCD_SEG29			=  4U,		//PA1.11
	PA111_VLC3				=  4U,			
    PA111_GPTB1_CHAX        =  5U,
    PA111_UART2_TX      	=  6U,
    PA111_CMP0_OUT		  	=  7U,
	PA111_LED_C0M7			=  8U,
	PA111_USART0_CK			=  9U,
	PA111_TKEY_CH20       	=  10U,
	
	PA112_GPD				=  0U,
	PA112_INPUT				=  1U,		//input
	PA112_OUTPUT			=  2U,		//output
	PA112_OUTPUT_MONI		=  3U,		//output with monitor
	PA112_LCD_SEG30			=  4U,		//PA112
	PA112_VLC2				=  4U,			
    PA112_GPTB0_CHAX        =  5U,
    PA112_UART2_RX      	=  6U,
    PA112_BT1_OUT		  	=  7U,
	PA112_LED_C0M8			=  8U,
	PA112_I2C0_SCL			=  9U,
	PA112_TKEY_CH21       	=  10U,
	
	PA113_GPD				=  0U,
	PA113_INPUT				=  1U,		//input
	PA113_OUTPUT			=  2U,		//output
	PA113_OUTPUT_MONI		=  3U,		//output with monitor
	PA113_LCD_SEG31			=  4U,		//PA1.13
	PA113_VLC1				=  4U,			
    PA113_GPTB1_CHAX        =  5U,
    PA113_CNTA_REM      	=  6U,
    PA113_SPI0_NSS		  	=  7U,
	PA113_LED_C0M9			=  8U,
	PA113_I2C0_SDA			=  9U,
	PA113_TKEY_CH22       	=  10U,
	
	PIN_GPD					=  0U,
	PIN_INPUT				=  1U,		//input
	PIN_OUTPUT				=  2U,		//output
	PIN_OUTPUT_MONI			=  3U,		//output with monitor
	
	IOMAP    			    =  11U
} pin_func_e;

typedef enum{
	SWD_GRP0 = 0,
	SWD_GRP1,
	SWD_GRP2
}swd_grp_e;

//devices information, Bass Addr/Irq Num/Pclk Num
typedef struct {
	uint16_t hwRegBase;					//base addr		
    uint8_t byIrqNum;					//irq num
	uint8_t byModule;					//pclk in pcer bit num
} csi_devmap_t;

#define PFLASHBASE			0x00000000
#define PFLASHSIZE			0x00010000
#define PFLASHPAGE			256
#define PFLASHLIMIT			(PFLASHBASE + PFLASHSIZE) 
#define USEROPTION_ADDR		0x000800C0
#define DFLASHBASE			0x10000000
#define DFLASHSIZE			0x00000800
#define DFLASHPAGE			64
#define DFLASHLIMIT 		(DFLASHBASE + DFLASHSIZE) 

#ifdef REMAP
	#define SRAMBASE		0x00000000
	#define SRAMSIZE 		0x00002000
	#define SRAMLIMIT		(SRAMBASE + SRAMSIZE) 
	#define MEMVECTORBASE	0x00000700
	#define MEMVECTORSIZE	(0x50 << 2)
#else
	#define SRAMBASE 		0x20000000
	#define SRAMSIZE 		0x00002000
	#define SRAMLIMIT 		(SRAMBASE + SRAMSIZE) 
	#define MEMVECTORBASE 	0x20000700
	#define MEMVECTORSIZE 	(0x50 << 2)
#endif

//APB for APT32F110
//--Peripheral Address Setting
#define APB_PERI_BASE		0x40000000UL 
#define APB_SFM0_BASE   	(APB_PERI_BASE + 0x00000)
#define APB_IFC_BASE    	(APB_PERI_BASE + 0x10000)
#define APB_SYS_BASE  	    (APB_PERI_BASE + 0x11000)
#define APB_ETCB_BASE    	(APB_PERI_BASE + 0x12000)
#define APB_TKEY_BASE    	(APB_PERI_BASE + 0x20000)
#define APB_TKRFL_BASE    	(APB_PERI_BASE + 0x21000)
#define APB_ADC0_BASE    	(APB_PERI_BASE + 0x30000)
#define APB_BT0_BASE       	(APB_PERI_BASE + 0x50000)
#define APB_BT1_BASE      	(APB_PERI_BASE + 0x51000)
#define APB_CNTA_BASE     	(APB_PERI_BASE + 0x52000)
#define APB_GPTA0_BASE    	(APB_PERI_BASE + 0x53000)
#define APB_GPTA1_BASE    	(APB_PERI_BASE + 0x54000)
#define APB_GPTB0_BASE    	(APB_PERI_BASE + 0x55000)
#define APB_GPTB1_BASE    	(APB_PERI_BASE + 0x56000)
#define APB_EPT0_BASE   	(APB_PERI_BASE + 0x57000)
#define APB_EPT1_BASE   	(APB_PERI_BASE + 0x58000)
#define APB_EPT2_BASE   	(APB_PERI_BASE + 0x59000)
#define APB_RTC_BASE		(APB_PERI_BASE + 0x60000)
#define APB_LPT_BASE     	(APB_PERI_BASE + 0x61000)
#define APB_WWDT_BASE		(APB_PERI_BASE + 0x62000)
#define APB_LED_BASE    	(APB_PERI_BASE + 0x70000)

#define APB_UART0_BASE   	(APB_PERI_BASE + 0x80000)
#define APB_UART1_BASE   	(APB_PERI_BASE + 0x81000)
#define APB_UART2_BASE  	(APB_PERI_BASE + 0x82000)
#define APB_USART0_BASE   	(APB_PERI_BASE + 0x83000)
#define APB_SPI0_BASE  		(APB_PERI_BASE + 0x90000)	//SPI
#define APB_SPI1_BASE  		(APB_PERI_BASE + 0x91000)
#define APB_SPI2_BASE  		(APB_PERI_BASE + 0x92000)
#define APB_I2C0_BASE   	(APB_PERI_BASE + 0xA0000)	//IIC
#define APB_I2C1_BASE   	(APB_PERI_BASE + 0xA1000)
#define APB_SIO0_BASE  		(APB_PERI_BASE + 0xB0000)	
//
#define AHB_GPIO_BASE 		0x60000000
#define APB_GPIOA0_BASE  	(AHB_GPIO_BASE + 0x0000) 	//A0  
#define APB_GPIOA1_BASE  	(AHB_GPIO_BASE + 0x1000) 	//A1  
#define APB_GPIOB0_BASE  	(AHB_GPIO_BASE + 0x2000) 	//B0 
#define APB_GPIOC0_BASE  	(AHB_GPIO_BASE + 0x4000) 	//C0 
#define APB_GPIOD0_BASE  	(AHB_GPIO_BASE + 0x6000) 	//D0 
#define APB_GPIOE0_BASE  	(AHB_GPIO_BASE + 0x7000) 	//E0 
#define APB_IGRP_BASE		(AHB_GPIO_BASE + 0xF000)	//IGRP

#define APB_LCD_BASE    	(APB_PERI_BASE + 0xC0000)
#define APB_CMP0_BASE    	(APB_PERI_BASE + 0xD0000)
#define APB_CMP1_BASE    	(APB_PERI_BASE + 0xD1000)

#define AHB_RFL_BASE     	(SRAMBASE + 0x02000)
//
#define AHB_CRC_BASE     	0x50000000
#define AHB_HWD_BASE		0x70000000	
#define AHB_DMA_BASE    	0x80000000
#define CORE_ADDR_BASE  	0xE000E000					//CK802 CORE ADDR	
#define CORET_ADDR_BASE  	(CORE_ADDR_BASE + 0x10)		//CK802 CORET ADDR 	
#define VIC_ADDR_BASE  		(CORE_ADDR_BASE + 0x100)	//CK802 VIC ADDR	

#define C0_REGBase     0xE0000000
#define C0_SYSTICBase  (C0_REGBase + 0x1000)

//Error Address Definition
#ifdef REMAP
  #define SEND_ADDR 0x00003000
  #define UNUSED ((U8_T) 0xFF)
  #define UNUSED_ADDRESS 0xDEADDEADul
#else
  #define SEND_ADDR 0x20003000
  #define UNUSED ((U8_T) 0xFF)
  #define UNUSED_ADDRESS 0xDEADDEADul
#endif


//--Interrupt Bit Position
#define CORET_INT   		(0x01ul<<0)       //IRQ0
#define SYSCON_INT  		(0x01ul<<1)       //IRQ1
#define IFC_INT     		(0x01ul<<2)       //IRQ2
#define ADC_INT     		(0x01ul<<3)       //IRQ3
#define EPT0_INT    		(0x01ul<<4)       //IRQ4
#define DMA_INT    			(0x01ul<<5)       //IRQ5
#define WWDT_INT    		(0x01ul<<6)       //IRQ6
#define EXI0_INT    		(0x01ul<<7)       //IRQ7
#define EXI1_INT    		(0x01ul<<8)       //IRQ8
#define GPTA0_INT    		(0x01ul<<9)       //IRQ9 
#define GPTA1_INT    		(0x01ul<<10)      //IRQ10 
#define GPTB0_INT    		(0x01ul<<11)      //IRQ11 
#define RTC_INT     		(0x01ul<<12)      //IRQ12
#define UART0_INT   		(0x01ul<<13)      //IRQ13
#define UART1_INT   		(0x01ul<<14)      //IRQ14
#define UART2_INT   		(0x01ul<<15)      //IRQ15
#define USART_INT   		(0x01ul<<16)      //IRQ16
#define GPTB1_INT   		(0x01ul<<17)      //IRQ17
#define SIO_INT     		(0x01ul<<18)      //IRQ18
#define I2C_INT     		(0x01ul<<19)      //IRQ19
#define SPI_INT     		(0x01ul<<20)      //IRQ20
#define EXI2_INT    		(0x01ul<<21)      //IRQ21
#define EXI3_INT    		(0x01ul<<22)      //IRQ22
#define EXI4_INT    		(0x01ul<<23)      //IRQ23
#define CA_INT      		(0x01ul<<24)      //IRQ24
#define TKEY_INT    		(0x01ul<<25)      //IRQ25
#define LPT_INT     		(0x01ul<<26)      //IRQ26
#define LED_INT     		(0x01ul<<27)      //IRQ27
#define CMP_INT     		(0x01ul<<28)      //IRQ28
#define BT0_INT     		(0x01ul<<29)      //IRQ29
#define BT1_INT     		(0x01ul<<30)      //IRQ30
#define LCD_INT     		(0x01ul<<31)      //IRQ31

#define CONFIG_IRQ_NUM				32U
#define CK_CPU_ENALLNORMALIRQ		asm ("psrset ee,ie")
#define CK_CPU_DISALLNORMALIRQ		asm ("psrclr ie")



//ISR Define for generating special interrupt related ASM (CK802), with compile option -mistack
void MisalignedHandler(void) 		__attribute__((isr));
void AccessErrHandler(void) 		__attribute__((isr));
void IllegalInstrHandler(void) 		__attribute__((isr));
void PriviledgeVioHandler(void) 	__attribute__((isr));
void BreakPointHandler(void) 		__attribute__((isr));
void UnrecExecpHandler(void) 		__attribute__((isr));
void Trap0Handler(void) 			__attribute__((isr));
void Trap1Handler(void) 			__attribute__((isr));
void Trap2Handler(void) 			__attribute__((isr));
void Trap3Handler(void) 			__attribute__((isr));
void PendTrapHandler(void) 			__attribute__((isr));


void coret_int_handler(void) 		__attribute__((isr));
void syscon_int_handler(void) 		__attribute__((isr));
void ifc_int_handler(void) 			__attribute__((isr));
void adc_int_handler(void) 			__attribute__((isr));
void ept0_int_handler(void)	 		__attribute__((isr));
void dma_int_handler(void) 			__attribute__((isr));
void wwdt_int_handler(void) 		__attribute__((isr));
void exi0_int_handler(void) 		__attribute__((isr));
void exi1_int_handler(void) 		__attribute__((isr));
void gpta0_int_handler(void) 		__attribute__((isr));
void gpta1_int_handler(void) 		__attribute__((isr));
void gptb0_int_handler(void) 		__attribute__((isr));
void rtc_int_handler(void) 			__attribute__((isr));
void uart0_int_handler(void) 		__attribute__((isr));
void uart1_int_handler(void) 		__attribute__((isr));
void uart2_int_handler(void) 		__attribute__((isr));
void usart0_int_handler(void) 		__attribute__((isr));
void gptb1_int_handler(void) 		__attribute__((isr));
void sio_int_handler(void) 			__attribute__((isr));
void i2c_int_handler(void) 			__attribute__((isr));
void spi0_int_handler(void) 		__attribute__((isr));
void exi2_3_int_handler(void) 		__attribute__((isr));
void exi4_9_int_handler(void) 		__attribute__((isr));
void exi10_15_int_handler(void) 	__attribute__((isr));
void cnta_int_handler(void) 		__attribute__((isr));
void tkey_int_handler(void) 		__attribute__((isr));
void lpt_int_handler(void) 			__attribute__((isr));
void led_int_handler(void) 			__attribute__((isr));
void cmp_int_handler(void) 			__attribute__((isr));
void bt0_int_handler(void) 			__attribute__((isr));
void bt1_int_handler(void) 			__attribute__((isr));
void lcd_int_handler(void) 			__attribute__((isr));


#ifdef __cplusplus
}
#endif

#endif  /* _SOC_H_ */
