
/***********************************************************************//** 
 * \file  board_config.h
 * \brief  system clock management for cpu clock(HCLK) and peri clock(PCLK)
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-10 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _BOARD_CONFIG_H_
#define _BOARD_CONFIG_H_

#ifdef __cplusplus
extern "C" {
#endif

/* emosc value */
//外部主振荡器频率值
#ifndef EMOSC_VALUE
#define EMOSC_VALUE             		16000000U
#endif

/* user heap size */
//定义用户堆的大小，用于动态分配内存，可根据具体需求调整大小
#define USER_HEAP_SIZE					0x40

/* example pin manager */
//如果定义了DBG_PRINT2PC，串口对应脚和PC连上后，可以通过serial pane里的UART #x 看到串口数据
#define CONSOLE_IDX                     2
#define CONSOLE_TXD                     PA111
#define CONSOLE_RXD                     PA112
#define CONSOLE_TXD_FUNC                PA111_UART2_TX
#define CONSOLE_RXD_FUNC                PA112_UART2_RX

/* emosc pin */
//外部主振荡器管脚
#define XIN_PIN							PA03
#define XOUT_PIN						PA04
#define XIN_PIN_FUNC					PA03_OSC_XI
#define XOUT_PIN_FUNC					PA04_OSC_XO

/* esosc pin */
//外部副振荡器管脚
#define SXIN_PIN						PA01
#define SXOUT_PIN						PA02
#define SXIN_PIN_FUNC					PA01_OSC_SXI
#define SXOUT_PIN_FUNC					PA02_OSC_SXO

///* spi pin */
//#define SPI_MOSI_PIN            		PB011            
//#define SPI_MISO_PIN            		PB010            
//#define SPI_SCK_PIN             		PB09  
//#define SPI_NSS_PIN             		PB08            
//#define SPI_MOSI_PIN_FUNC       		PB011_SPI0_MOSI
//#define SPI_MISO_PIN_FUNC       		PB010_SPI0_MISO
//#define SPI_SCK_PIN_FUNC				PB09_SPI0_SCK
//#define SPI_NSS_PIN_FUNC        		PB08_SPI0_NSS
//
///* iic pin */
//#define IIC_SCL_PIN             		PA19   
//#define IIC_SDA_PIN             		PA110    
//#define IIC_SCL_PIN_FUNC        		PA19_I2C0_SCL
//#define IIC_SDA_PIN_FUNC        		PA110_I2C0_SDA
//
///* sio pin */
//#define SIO_PIN					   	PA013
//#define SIO_PIN_FUNC					PA013_SIO


//csi Interrupt Handling Enable, 1/0(OPEN/CLOSE)
//系统中断处理配置默认是全部打开，对应的中断处理会调用处理函数，会占用的代码空间
//用户不需要用到某个中断时，可以将对应中断宏定义值设置为0可节约代码空间
#define	CORET_INT_HANDLE_EN				1		//CORET 
#define	IFC_INT_HANDLE_EN				1		//IFC 
#define	ADC_INT_HANDLE_EN				1		//ADC
#define	EPT_INT_HANDLE_EN				1		//EPT 
#define	DMA_INT_HANDLE_EN				1		//DMA
#define	WWDT_INT_HANDLE_EN				1		//WWDT
#define	GPTA0_INT_HANDLE_EN				1		//GPTA0
#define	GPTA1_INT_HANDLE_EN				1		//GPTA1
#define	GPTB0_INT_HANDLE_EN				1		//GPTB0
#define	GPTB1_INT_HANDLE_EN				1		//GPTB1
#define	RTC_INT_HANDLE_EN				1		//RTC
#define	UART0_INT_HANDLE_EN				1		//UART0
#define	UART1_INT_HANDLE_EN				1		//UART1
#define	UART2_INT_HANDLE_EN				1		//UART2
#define	USART0_INT_HANDLE_EN			1		//USART0
#define	SIO_INT_HANDLE_EN				1		//SIO
#define	I2C_INT_HANDLE_EN				1		//I2C
#define	SPI_INT_HANDLE_EN				1		//SPI
#define	EXI0_INT_HANDLE_EN				1		//EXI0(GPIO external interrupt), EXI GROUP0 or GROUP16
#define	EXI1_INT_HANDLE_EN				1		//EXI1(GPIO external interrupt), EXI GROUP1 or GROUP17
#define	EXI2_3_INT_HANDLE_EN			1		//EXI2~3(GPIO external interrupt), EXI GROUP2~3 or GROUP18~19
#define	EXI4_9_INT_HANDLE_EN			1		//EXI4~9(GPIO external interrupt), EXI GROUP4~9 
#define	EXI10_15_INT_HANDLE_EN			1		//EXI10~15(GPIO external interrupt), EXI GROUP10~15 
#define	CNTA_INT_HANDLE_EN				1		//CNTA
#define	LPT_INT_HANDLE_EN				1		//LPT
#define	BT0_INT_HANDLE_EN				1		//BT0
#define	BT1_INT_HANDLE_EN				1		//BT1
#define	CMP_INT_HANDLE_EN				1		//CMP
#define	LCD_INT_HANDLE_EN				1		//LCD
#define	LED_INT_HANDLE_EN				1		//LED
#define	TKEY_INT_HANDLE_EN				1		//TOUCH

#ifdef __cplusplus
}
#endif

#endif /* _BOARD_CONFIG_H_ */

