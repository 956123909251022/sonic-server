/***********************************************************************//** 
 * \file  led_demo.c
 * \brief  LED示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2021-12-03 <td>V0.0  <td>WNN     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "led.h"
#include "csp.h"
#include "pin.h"
#include "reliability.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/
/* Private variablesr-----------------------------------------------------*/
uint8_t g_byLedData[4] = {0x86,0xdb,0xcf,0xe6};//display code of 1,2,3,4 

void io_config(uint8_t byGrp)
{
	//SEG0~7
	switch (byGrp) 
	{
		case 0: 
			csi_pin_set_mux(PA014, PA014_LED_S0);
			csi_pin_set_mux(PA015, PA015_LED_S1);
			csi_pin_set_mux(PB03,  PB03_LED_S2);
			csi_pin_set_mux(PC01,  PC01_LED_S3);
			csi_pin_set_mux(PB04,  PB04_LED_S4);
			csi_pin_set_mux(PA10,  PA10_LED_S5);
			csi_pin_set_mux(PA11,  PA11_LED_S6);
			csi_pin_set_mux(PA12,  PA12_LED_S7);
		break;
		
		case 1:
			csi_pin_set_mux(PA06,  PA06_LED_S0);
			csi_pin_set_mux(PA07,  PA07_LED_S1);
			csi_pin_set_mux(PA08,  PA08_LED_S2);
			csi_pin_set_mux(PA09,  PA09_LED_S3);
			csi_pin_set_mux(PA010, PA010_LED_S4);
			csi_pin_set_mux(PA011, PA011_LED_S5);
			csi_pin_set_mux(PA012, PA012_LED_S6);
			csi_pin_set_mux(PA013, PA013_LED_S7);
		break;
	
		case 2:
			csi_pin_set_mux(PA011, PA011_LED_S0);
			csi_pin_set_mux(PA012, PA012_LED_S1);
			csi_pin_set_mux(PB03,  PB03_LED_S2);
			csi_pin_set_mux(PA013, PA013_LED_S3);
			csi_pin_set_mux(PB04,  PB04_LED_S4);
			csi_pin_set_mux(PA10,  PA10_LED_S5);
			csi_pin_set_mux(PA11,  PA11_LED_S6);
			csi_pin_set_mux(PA12,  PA12_LED_S7);
		break;
		
		default:
		break;
		
	}
			
	mdelay(3000);
	csi_swd_unlock();//如果要使用SWD口(PA17/PA18)，前面一定要加delay函数，否则复位后将很难连上芯片。
	
	//COM0~9
	csi_pin_set_mux(PA16,  PA16_LED_COM0);
	csi_pin_set_mux(PB05,  PB05_LED_COM1);
	csi_pin_set_mux(PA17,  PA17_LED_COM2);//SWDIO
	csi_pin_set_mux(PB06,  PB06_LED_COM3);
	csi_pin_set_mux(PB07,  PB07_LED_COM4);
	csi_pin_set_mux(PA18,  PA18_LED_COM5);//SWCLK
	csi_pin_set_mux(PB010, PB010_LED_COM6);
	csi_pin_set_mux(PB011, PB011_LED_COM7);
	csi_pin_set_mux(PA19,  PA19_LED_COM8);
	csi_pin_set_mux(PA113, PA113_LED_C0M9);
		
	//开启COM口强驱
	csi_pin_drive(PA16, GPIO_DRIVE_LV1);
	csi_pin_drive(PB05, GPIO_DRIVE_LV1);
	csi_pin_drive(PA17, GPIO_DRIVE_LV1);
	csi_pin_drive(PB06, GPIO_DRIVE_LV1);
	csi_pin_drive(PB07, GPIO_DRIVE_LV1);
	csi_pin_drive(PA18, GPIO_DRIVE_LV1);
	csi_pin_drive(PB08, GPIO_DRIVE_LV1);
	csi_pin_drive(PA111, GPIO_DRIVE_LV1);
	csi_pin_drive(PA112, GPIO_DRIVE_LV1);
	csi_pin_drive(PA113, GPIO_DRIVE_LV1);
}

/**
  \brief  LED示例代码
   * \brief  1、正常控制：依次点亮4个数码管，依次显示1，2，3，4
   * \brief  2、闪烁控制：关闭COM0，显示2，3，4
   * \brief  3、闪烁控制：关闭COM0/1，显示3，4
   * \brief  4、闪烁控制：关闭COM0/1/2，显示4
  \return      csi_error_t
 * 四位一体共阴数码管引脚图：
					  COM1  a   f COM2 COM3 b
						|	|	|	|	|	|
						|	|	|	|	|	|									
				|—————————————————|
				|  |——|  |——|  |——|  |——|  |
				|  |	|  |    |  |	|  |	|  |
				|  |——|  |——|  |——|  |——|  |
				|  |	|  |    |  |	|  |	|  |
				|  |——|. |——|. |——|. |——|. |		
				|—————————————————|
						|	|	|	|	|	|
						|	|	|	|	|	|
					    e   d   dp  c   g  COM4
*/
csi_error_t led_demo(void)
{	
	uint8_t byDisplayStatus=0;
	csi_led_config_t ptLedCfg;
	
	io_config(1);
	
	ptLedCfg.byClk = LED_PCLK_DIV8;
	ptLedCfg.hwComMask = 0x0f;			//COM0~3打开
	ptLedCfg.byBrt = LED_50;			//LED显示亮度50%
	ptLedCfg.byOnTime = 120;			//显示周期时间
	ptLedCfg.byBreakTime = 50;			//Non-Overlap时间
	ptLedCfg.byInt = LED_INTSRC_NONE;
	csi_led_init(LED, &ptLedCfg);			

	while(1)
	{
		switch(byDisplayStatus)
		{
			//display status 0:依次显示1，2，3，4
			case 0:
				csi_led_lighton(LED);
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
			
			//display status 1:关闭COM0，依次显示2，3，4
			case 1:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x01&LED_BLK_MSK));//disable COM0
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
				
			//display status 2:关闭COM0/1，依次显示3，4
			case 2:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x02&LED_BLK_MSK));//disable COM1
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus++;
				mdelay(1);
			break;
				
			//display status 3:关闭COM0/1/2，显示4
			case 3:
				csi_led_lighton(LED);
				csi_led_blink_control(LED, LED_BLK_OFF,(0x04&LED_BLK_MSK));//disable COM2
				for(uint8_t i = 0; i < 4; i++)
				{
					csi_led_write_data(LED, i, g_byLedData[i]);
					mdelay(50);
				}
				csi_led_lightoff(LED);
				byDisplayStatus = 0;
				mdelay(1);
				csi_led_blink_control(LED, LED_BLK_ON,(0x07&LED_BLK_MSK));//enable COM0/1/2
			break;
			
			default:
			break;
		}
		//clear display buffer
		for(uint8_t i = 0;i<4;i++)
			csi_led_write_data(LED, i, 0x00);
	}
}
/** \brief led interrupt handle function
 * 
 *  \param[in] args: dev of led
 *  \return none
 */ 
__attribute ((weak))void led_irqhandler(csp_led_t *ptLedBase)
{
	csp_led_clr_isr(ptLedBase, LED_INTSRC_ICEND|LED_INTSRC_IPEND);
}

