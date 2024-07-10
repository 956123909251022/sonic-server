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

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/



/* Private variablesr-----------------------------------------------------*/
uint8_t g_byLedData[11] = {0x3f, 0x6, 0x5b, 0x4f, 0x66, 0x6d, 0x7d, 0x07,0x7f, 0x6f, 0x80} ; 
uint8_t g_byLedData8[10] = {0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,0x7f, 0x7f} ; 



void io_config(uint8_t byGrp)
{
	switch (byGrp) {
		case 1:
		csi_pin_set_mux(PA013, PA013_LED_S7);
		csi_pin_set_mux(PA012, PA012_LED_S6);
		csi_pin_set_mux(PA011, PA011_LED_S5);
		csi_pin_set_mux(PA010, PA010_LED_S4);
		csi_pin_set_mux(PA09, PA09_LED_S3);
		csi_pin_set_mux(PA08, PA08_LED_S2);
		csi_pin_set_mux(PA07, PA07_LED_S1);
		csi_pin_set_mux(PA06, PA06_LED_S0);
		break;
	
		case 0: 
		csi_pin_set_mux(PA12, PA12_LED_S7);
		csi_pin_set_mux(PA11, PA11_LED_S6);
		csi_pin_set_mux(PA10, PA10_LED_S5);
		csi_pin_set_mux(PB04, PB04_LED_S4);
		csi_pin_set_mux(PC01, PC01_LED_S3);
		csi_pin_set_mux(PB03, PB03_LED_S2);
		csi_pin_set_mux(PA015, PA015_LED_S1);
		csi_pin_set_mux(PA014, PA014_LED_S0);
		break;
		
		case 2:
		csi_pin_set_mux(PA12, PA12_LED_S7);
		csi_pin_set_mux(PA11, PA11_LED_S6);
		csi_pin_set_mux(PA10, PA10_LED_S5);
		csi_pin_set_mux(PB04, PB04_LED_S4);
		csi_pin_set_mux(PB03, PB03_LED_S2);
		csi_pin_set_mux(PA011, PA011_LED_S0);
		csi_pin_set_mux(PA012, PA012_LED_S1);
		csi_pin_set_mux(PA013, PA013_LED_S3);
		
		csi_pin_set_mux(PA16, PA16_LED_COM0);
		csi_pin_set_mux(PB05, PB05_LED_COM1);
		csi_pin_set_mux(PA17, PA17_LED_COM2);
		csi_pin_set_mux(PB06, PB06_LED_COM3);
		csi_pin_set_mux(PB07, PB07_LED_COM4);
		csi_pin_set_mux(PA18, PA18_LED_COM5);
		csi_pin_set_mux(PB010, PB010_LED_COM6);
		csi_pin_set_mux(PB011, PB011_LED_COM7);
		csi_pin_set_mux(PA19, PA19_LED_COM8);
		csi_pin_set_mux(PA113, PA113_LED_C0M9);
		return;
		break;
		
		
		
		default:
			break;
		
	}
	
	//mdelay(3000);
	//csi_swd_unlock(); //如果要使用SWD口，前面一定要加delay函数，否则复位后将很难连上芯片。
	csi_pin_set_mux(PA16, PA16_LED_COM0);
	csi_pin_set_mux(PB05, PB05_LED_COM1);
	csi_pin_set_mux(PA17, PA17_LED_COM2);
	csi_pin_set_mux(PB06, PB06_LED_COM3);
	csi_pin_set_mux(PB07, PB07_LED_COM4);
	csi_pin_set_mux(PA18, PA18_LED_COM5);
	csi_pin_set_mux(PB08, PB08_LED_COM6);
	csi_pin_set_mux(PA111, PA111_LED_C0M7);
	csi_pin_set_mux(PA112, PA112_LED_C0M8);
	csi_pin_set_mux(PA113, PA113_LED_C0M9);
}

/**
  \brief  LED示例代码:依次点亮8个数码管
   *  
  \return      csi_error_t
*/


csi_error_t led_demo(void)
{	
	csi_led_config_t ptLedCfg;
	io_config(2);
	
	ptLedCfg.byClk = LED_PCLK_DIV8;
	ptLedCfg.hwComMask = 0x3ff;
	ptLedCfg.byBrt = LED_100;
	ptLedCfg.byOnTime = 0x7f;
	ptLedCfg.byBreakTime = 50;
	ptLedCfg.byInt = LED_INTSRC_NONE;
	
	csi_led_init(LED, &ptLedCfg);		
	csi_led_lighton(LED);	
	
		
	csi_led_write_data(LED, 0, g_byLedData[0]);
	mdelay(1000);
	csi_led_write_data(LED, 1, g_byLedData[1]);
	mdelay(1000);
	csi_led_write_data(LED, 2, g_byLedData[2]);
	mdelay(1000);
	csi_led_write_data(LED, 3, g_byLedData[3]);
	mdelay(1000);
	csi_led_write_data(LED, 4, g_byLedData[4]);
	mdelay(1000);
	csi_led_write_data(LED, 5, g_byLedData[5]);
	mdelay(1000);
	csi_led_write_data(LED, 6, g_byLedData[6]);
	mdelay(1000);
	csi_led_write_data(LED, 7, g_byLedData[7]);
	mdelay(1000);
	csi_led_write_data(LED, 8, g_byLedData[8]);
	mdelay(1000);
	csi_led_write_data(LED, 9, g_byLedData[9]);
	
	while(1) {
	}
	
	
}
