/***********************************************************************//** 
 * \file  gpio_demo.c
 * \brief  GPIO_DEMO description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-4-25 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include "sys_clk.h"
#include <drv/gpio.h>
#include <drv/pin.h>
#include <iostring.h>
#include "demo.h"
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
#define		PINMASK_PA00		(0x01ul << 0)	
#define		PINMASK_PA01		(0x01ul << 1)
#define		PINMASK_PA02		(0x01ul << 2)	
#define		PINMASK_PA05		(0x01ul << 5)
#define		PINMASK_PA09		(0x01ul << 9)	
#define		PINMASK_PA010		(0x01ul << 10)
#define		PINMASK_PA013		(0x01ul << 13)

#define		PINMASK_PB01		(0x01ul << 1)
#define		PINMASK_PB02		(0x01ul << 2)
#define		PINMASK_PB05		(0x01ul << 5)
/* Private variablesr------------------------------------------------------*/

/** \brief gpio port output demo 
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_port_ouput_demo(void)
{
	int iRet = 0;
	uint32_t wPinMask = PINMASK_PA00 | PINMASK_PA02;				//GPIOA0端口，PA00/PA02
	
	csi_gpio_port_dir(GPIOA0, wPinMask, GPIO_DIR_OUTPUT);			//GPIOA0 端口配置为输出
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	csi_gpio_port_set_low(GPIOA0, wPinMask);						//输出低
	mdelay(100);
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	
	csi_gpio_port_output_mode(GPIOA0, wPinMask, GPIO_OPEN_DRAIN);	//GPIOA0 端口配置为开漏输出
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	csi_gpio_port_set_low(GPIOA0, wPinMask);						//输出低
	mdelay(100);
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	
	csi_gpio_port_output_mode(GPIOA0, wPinMask, GPIO_PUSH_PULL);	//GPIOA0 端口配置为推挽输出
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	csi_gpio_port_set_low(GPIOA0, wPinMask);						//输出低
	mdelay(100);
	csi_gpio_port_set_high(GPIOA0, wPinMask);						//输出高
	mdelay(100);
	
	my_printf("GPIO PINMASK: %d \n", wPinMask);
	
	return iRet;
}

/** \brief gpio port output demo 
 * 
 *  \param[in] none
 *  \return error code
 */
int gpio_port_input_demo(void)
{
	int iRet = 0;
	uint32_t wStatus;
	uint32_t wPinMask = PINMASK_PA00 | PINMASK_PA02;				//GPIOA0端口，PA00/PA02
	
	csi_gpio_port_dir(GPIOA0, wPinMask, GPIO_DIR_INPUT);			//GPIOA0 端口配置为输入
	csi_gpio_port_pull_mode(GPIOA0, wPinMask, GPIO_PULLNONE);		//无上下拉
	mdelay(100);
	wStatus = csi_gpio_port_read(GPIOA0,wPinMask);
	while(wStatus != 0);
	
	csi_gpio_port_pull_mode(GPIOA0, wPinMask, GPIO_PULLUP);			//上拉
	mdelay(100);
	wStatus = csi_gpio_port_read(GPIOA0,wPinMask);
	while(wStatus != wPinMask);
	
	csi_gpio_port_pull_mode(GPIOA0, wPinMask, GPIO_PULLDOWN);		//下拉
	mdelay(100);
	wStatus = csi_gpio_port_read(GPIOA0,wPinMask);
	while(wStatus != 0);
	
	return iRet;
}

 /** \brief gpio port interrupt
 * 
 *  \param[in] none
 *  \return error code
 */
 
int gpio_port_irq_demo(void)
{
	int iRet = 0;
	uint32_t wPinMask = PINMASK_PA00 | PINMASK_PA02 | PINMASK_PA013; //GPIOA0端口，PA00/PA02/PA013

	csi_gpio_port_dir(GPIOA0, wPinMask, GPIO_DIR_INPUT);			//端口配置为输入
	csi_gpio_port_pull_mode(GPIOA0, wPinMask, GPIO_PULLUP);			//上拉
	csi_gpio_port_irq_mode(GPIOA0,wPinMask,GPIO_IRQ_FALLING_EDGE);	//下降沿
	csi_gpio_port_irq_enable(GPIOA0,wPinMask,ENABLE);				//使能GPIOA0端口对应外部中断
	csi_gpio_port_vic_irq_enable(wPinMask, ENABLE);					//GPIOA0端口对应VIC中断使能
	
	return iRet;
}

/** \brief gpio interrupt handle function
 * 
 *  \param[in] byExiNum: exi interrupt vector num, 0~4
 *  \return none
 */ 
__attribute__((weak)) void gpio_irqhandler(uint8_t byExiNum)
{
	volatile uint32_t wExiSta; 
	wExiSta = csp_exi_port_get_isr(SYSCON);
	
    switch(byExiNum)
	{
		case 0:
			switch(wExiSta)								//EXI0
			{
				case STATUS_EXI0:						//int group0
					break;
				case STATUS_EXI16:						//int group16
					break;
			}
			break;
		case 1:
			switch(wExiSta)								//EXI1							
			{
				case STATUS_EXI1:						//int group1
					break;
				case STATUS_EXI17:						//int group17
					break;
			}
			break;
		case 2:
			switch(wExiSta)								//EXI2-3
			{
				case STATUS_EXI2:						//int group2
					break;
				case STATUS_EXI3:						//int group3
					break;
				case STATUS_EXI18:						//int group18
					break;
				case STATUS_EXI19:						//int group19
					break;
			}
			break;
		case 3:
			switch(wExiSta)								//EXI4-9
			{
				case STATUS_EXI4:						//int group4
					break;
				case STATUS_EXI5:						//int group5
					break;
				case STATUS_EXI6:						//int group6
					break;
				case STATUS_EXI7:						//int group7
					break;
				case STATUS_EXI8:						//int group8
					break;
				case STATUS_EXI9:						//int group9
					break;
			}
			break;
		case 4:
			switch(wExiSta)								//EXI10-15
			{
				case STATUS_EXI10:						//int grou10
					break;
				case STATUS_EXI11:						//int group11
					break;
				case STATUS_EXI12:						//int group12
					break;
				case STATUS_EXI13:						//int group13
					break;
				case STATUS_EXI14:						//int group14
					break;
				case STATUS_EXI15:						//int group15
					break;
			}
			break;
		default:
			break;
		
	}
	csp_exi_port_clr_isr(SYSCON,wExiSta);		//clear interrput 
}