/***********************************************************************//** 
 * \file  lcd_demo.c
 * \brief  LCD示例代码

 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2021-12-03 <td>V0.0  <td>LQ     <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <drv/lcd.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/lcd.h>
#include <drv/irq.h>
#include <drv/rtc.h>
#include <drv/iwdt.h>
#include <drv/pm.h>
#include <drv/reliability.h>

#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/
/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
/* device instance ------------------------------------------------------*/

uint8_t bySendBata[32] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20,21,22,23,24,25,26,27,28,29,30,31,32};
static uint8_t s_RtcSec = 0;

static void lcd_rtc_data(uint8_t *bypBuf, uint8_t byPos, uint8_t byData)
{
	switch(byData)
	{
		case 0:
			bypBuf[byPos+1] &= 0x0d;
			break;
		case 1:
			bypBuf[byPos] &= 0x00;
			bypBuf[byPos+1] &= 0x0d;
			break;
		case 2:
			bypBuf[byPos] &= 0x0d;
			bypBuf[byPos+1] &= 0x0e;
			break;
		case 3:
			bypBuf[byPos] &= 0x0c;
			break;
		case 4:
			bypBuf[byPos] &= 0x02;
			break;
		case 5:
			bypBuf[byPos] &= 0x0e;
			bypBuf[byPos+1] &= 0x0b;
			break;
		case 6:
			bypBuf[byPos+1] &= 0x0b;
			break;
		case 7:
			bypBuf[byPos] &= 0x04;
			bypBuf[byPos+1] &= 0x0d;
			break;
		case 9:
			bypBuf[byPos] &= 0x0e;
			break;
	}
}


/** \brief lcd display
 * 
 *  \param[in] none
 *  \return error code
 */
int lcd_disp_demo(void)
{
	int iRet = 0;
	uint8_t i;
	uint32_t wSegMsk = 0x0Cfffffc;		//lcd seg2->26
	uint8_t  wComMsk = 0x0f;			//lcd com0->3
	csi_lcd_config_t tLcdCfg;
	
	
	csi_lcd_gpio_init(wSegMsk, wComMsk);		//LCD GPIO 配置：com0-com3   seg2-seg31   4*30
	
	tLcdCfg.byClkSrc 	= LCD_CLKSRC_ISOSC;		//LCD时钟源选择ISOSC
	tLcdCfg.byFreq 		= 80;					//LCD刷新频率 =40Hz，选择范围：30~100Hz
	tLcdCfg.byVlcd		= LCD_VLCD_IN_4V0;		//VLCD选择内部3.2V
	tLcdCfg.byDutyBias	= LCD_DUTY1_4_BIAS1_3;	//LCD Duty = 1/4, Bias = 1/3
	tLcdCfg.byDead		= LCD_DEAD_2PHASE;		//LCD帧死区控制，选择3个相位周期，死区可以控制对比度
	tLcdCfg.byDrvNet	= LCD_DRVNET_PWLEV1;	//LCD驱动网络(电阻网络), 共有4档，功耗PWLEV0 > PWLEV1 > PWLEV2 > PWLEV3
	tLcdCfg.byDpEn		= DISABLE;				//去耦电容使能控制
	tLcdCfg.byInt		= LCD_INTSRC_NONE;		//是否用中断，不用
	
	iRet = csi_lcd_init(LCD, &tLcdCfg);
	if(iRet == CSI_OK)
		csi_lcd_start(LCD);
			
	//csi_lcd_set_blink(LCD, LCD_BLINK_SEG8, LCD_BLINK_FRE_F2, 2);		//LCD 闪烁频率选择不等于LCD_BLINK_FRE_F2， 最后一个参数无意义
	csi_lcd_write_data(LCD, bySendBata, 2, 22);
	
	do
	{
		for(i = 2; i < 24; i++)
		{
			bySendBata[i] = 0x0f;
		}
		
		csi_lcd_write_data(LCD, bySendBata, 2, 22);
//		
		mdelay(1000);

		
		for(i = 2; i < 24; i++)
		{
			if(7 == i)
				bySendBata[i] = 0x07;
			else
				bySendBata[i] = 0x0f;
		}
		csi_lcd_write_data(LCD, bySendBata, 2, 22);
		
		mdelay(1000);
		nop;
		nop;
	}while(1);
	
	return iRet;
}

/** \brief lcd display snooze 
 * 
 *  \param[in] none
 *  \return error code
 */
int lcd_disp_sleep_demo(void)
{
	
	int iRet = 0;
	uint8_t i;
	uint32_t wSegMsk = 0x03fffffc;		//lcd seg2->26
	uint8_t  wComMsk = 0x0f;			//lcd com0->3
	
	uint16_t hwRstSrc;
	
	csi_pm_mode_e ePmMode = PM_MODE_SNOOZE;
	csi_lcd_config_t tLcdCfg;
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	
	hwRstSrc = csi_get_rst_reason();							//获取系统复位信息		
	if(hwRstSrc)
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);	//打印系统复位信息
		csi_clr_rst_reason(hwRstSrc);
	}
	
	csi_pin_set_mux(PA00, PA00_OUTPUT);				//PA00 OUTPUT
	csi_pin_set_mux(PA05, PA05_OUTPUT);				//PA01 OUTPUT
	
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	
	
//	csi_pin_set_mux(SXIN_PIN, SXIN_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(SXOUT_PIN, SXOUT_PIN_FUNC);		//ESOSC管脚使能	
	
	//if(csp_get_ureg(SYSCON, 1) != 0x55)
	{
		tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;		//外部副晶振作为RTC时钟源
		tRtcConfig.byFmt = RTC_24FMT;				//选择时间模式
		csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
		
		tRtcTime.iYear	= 21;
		tRtcTime.iMon 	= 12;
		tRtcTime.iMday 	= 9;
		tRtcTime.iHour	= 15;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
		tRtcTime.iMin 	= 50;
		tRtcTime.iSec 	= 59;	
		csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
		csi_rtc_start_as_timer(RTC, RTC_TIMER_1S);	//每1s进一次中断

		csi_rtc_start(RTC);							//RTC开始计时
	}
	
//	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;			//选择时钟源
//	tRtcConfig.byFmt = RTC_12FMT;					//选择时间模式
//	csi_rtc_init(RTC, &tRtcConfig);					//初始化设置
//	csi_rtc_start_as_timer(RTC, RTC_TIMER_1S);		//每1s进一次中断
//	csi_rtc_start(RTC);	
	
	csi_pm_config_wakeup_source(WKUP_RTC, ENABLE);				//RTC唤醒
	csi_pm_snooze_power_enable(SNOOZE_LCD_POWER, ENABLE);		//LCD电源使能在snooze模式下
	
	
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);		//回读当前时间
	s_RtcSec = tRtcTimeRdbk.iSec;
	
	csi_lcd_gpio_init(wSegMsk, wComMsk);		//LCD GPIO 配置：com0-com3   seg2-seg31   4*30
	
	tLcdCfg.byClkSrc 	= LCD_CLKSRC_ISOSC;		//LCD时钟源选择ISOSC
	tLcdCfg.byFreq 		= 80;					//LCD刷新频率 =40Hz，选择范围：30~100Hz
	tLcdCfg.byVlcd		= LCD_VLCD_IN_3V4;		//VLCD选择内部3.2V
	tLcdCfg.byDutyBias	= LCD_DUTY1_4_BIAS1_3;	//LCD Duty = 1/4, Bias = 1/3
	tLcdCfg.byDead		= LCD_DEAD_4PHASE;		//LCD帧死区控制，选择3个相位周期，死区可以控制对比度
	tLcdCfg.byDrvNet	= LCD_DRVNET_PWLEV1;	//LCD驱动网络(电阻网络), 共有4档，功耗PWLEV0 > PWLEV1 > PWLEV2 > PWLEV3
	tLcdCfg.byDpEn		= DISABLE;				//去耦电容使能控制
	tLcdCfg.byInt		= LCD_INTSRC_NONE;		//是否用中断，不用
	
	iRet = csi_lcd_init(LCD, &tLcdCfg);
	if(iRet == CSI_OK)
	{
		csi_lcd_start(LCD);
		my_printf("LCD Init Succeed!\n");
	}

	mdelay(500);
	for(i = 2; i < 26; i++)						//全部点亮
	{
		bySendBata[i] = 0x0f;
	}
	csi_lcd_write_data(LCD, bySendBata, 2, 22);
	my_printf("Light up all!\n");
	
	mdelay(500);
	
	my_printf("Display Rtc Min and Sec!\n");
	
	do
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);	//回读当前时间
		my_printf("Rtc Min = %d, Sec = %d \n", tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
		
		for(i = 2; i < 26; i++)
		{
			if(7 == i)
			{
				bySendBata[i] |= 0x07;
				
				if(bySendBata[7] & 0x08)
					bySendBata[7] &= 0x07;		//seg7/com3
				else
					bySendBata[7] |= 0x08;		//seg7/com3
			}
			else
				bySendBata[i] = 0x0f;
		}
		
		lcd_rtc_data(bySendBata, 2, tRtcTimeRdbk.iMin/10);			//hour
		lcd_rtc_data(bySendBata, 4, tRtcTimeRdbk.iMin%10);			//hour
		lcd_rtc_data(bySendBata, 6, tRtcTimeRdbk.iSec/10);			//min
		lcd_rtc_data(bySendBata, 8, tRtcTimeRdbk.iSec%10);			//min
		
		
		csi_lcd_write_data(LCD, bySendBata,2,22);
		
		
		if(!((tRtcTimeRdbk.iSec+1)%3))
		{
			switch(ePmMode)
			{
				case PM_MODE_SLEEP:
					my_printf("Enter Sleep Mode\n");
					break;
				case PM_MODE_DEEPSLEEP:
					my_printf("Enter Deep-Sleep mode\n");
					break;
				case PM_MODE_SNOOZE:
					my_printf("Enter Snooze Mode\n");
					break;
				case PM_MODE_SHUTDOWN:
					my_printf("Enter ShutDown Mode\n");
					break;
				default:
					break;
			}
			
			csp_set_ureg(SYSCON, 1, 0x55);				
			csi_pin_set_high(PA05);						//PA01 OUTPUT high
			csi_pm_enter_sleep(ePmMode);				//进入snooze模式
			//mdelay(100);
			csi_pin_set_low(PA05);
			mdelay(100);	
		}
		
		mdelay(500);

	}while(1);
	
	
	return iRet;
}

/** \brief lcd display rtc of snooze mode 
 * 
 *  \param[in] none
 *  \return error code
 */

int lcd_disp_rtc_snooze_demo(void)
{
	
	int iRet = 0;
	uint8_t i;
	uint32_t wSegMsk = 0x03fffffc;		//lcd seg2->26
	uint8_t  wComMsk = 0x0f;			//lcd com0->3
	
	uint16_t hwRstSrc;
	
	csi_pm_mode_e ePmMode = PM_MODE_SNOOZE;			//PM_MODE_SNOOZE/PM_MODE_DEEPSLEEP
	csi_lcd_config_t tLcdCfg;
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	
	csi_pin_set_mux(PB02, PB02_OUTPUT);				//PB02 OUTPUT
	csi_pin_set_high(PB02);	
	csi_pin_set_mux(PA05, PA05_OUTPUT);				//PA01 OUTPUT
	
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	csi_pin_toggle(PA05);
	mdelay(250);
	
//	csi_pin_set_mux(SXIN_PIN, SXIN_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(SXOUT_PIN, SXOUT_PIN_FUNC);		//ESOSC管脚使能	
	
	hwRstSrc = csi_get_rst_reason();				//获取系统复位信息	
	my_printf("System Reset Source = 0x%x \n", hwRstSrc);		//打印系统复位信息	
	if(!(hwRstSrc & RST_SRC_SNOOZE_WKUP))
	{
		tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;		//外部副晶振作为RTC时钟源
		tRtcConfig.byFmt = RTC_24FMT;				//选择时间模式
		csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
		
		tRtcTime.iYear = 21;
		tRtcTime.iMon  = 12;
		tRtcTime.iMday = 9;
		tRtcTime.iHour = 15;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
		tRtcTime.iMin  = 50;
		tRtcTime.iSec  = 59;	
		csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
		csi_rtc_start_as_timer(RTC, RTC_TIMER_1S);	//每1s进一次中断

		csi_rtc_start(RTC);							//RTC开始计时
		my_printf("Init Rtc Complete!\n");			//打印初始化RTC信息
	}
	else
	{
		csi_rtc_start_as_timer(RTC, RTC_TIMER_1MIN);	//每1s进一次中断
	}
	
	csi_pin_set_low(PB02);							//指示用						
	csi_clr_rst_reason(hwRstSrc);					//清除复位信息
	
	csi_pm_config_wakeup_source(WKUP_RTC, ENABLE);				//RTC唤醒
	csi_pm_snooze_power_enable(SNOOZE_LCD_POWER, ENABLE);		//LCD电源使能在snooze模式下
	
	csi_lcd_gpio_init(wSegMsk, wComMsk);		//LCD GPIO 配置：com0-com3   seg2-seg31   4*30
	
	tLcdCfg.byClkSrc 	= LCD_CLKSRC_ISOSC;		//LCD时钟源选择ISOSC
	tLcdCfg.byFreq 		= 80;					//LCD刷新频率 =40Hz，选择范围：30~100Hz
	tLcdCfg.byVlcd		= LCD_VLCD_IN_3V2;		//VLCD选择内部3.2V
	tLcdCfg.byDutyBias	= LCD_DUTY1_4_BIAS1_3;	//LCD Duty = 1/4, Bias = 1/3
	tLcdCfg.byDead		= LCD_DEAD_NONE;		//LCD帧死区控制，选择3个相位周期，死区可以控制对比度
	tLcdCfg.byDrvNet	= LCD_DRVNET_PWLEV1;	//LCD驱动网络(电阻网络), 共有4档，功耗PWLEV0 > PWLEV1 > PWLEV2 > PWLEV3
	tLcdCfg.byDpEn		= DISABLE;				//去耦电容使能控制
	tLcdCfg.byInt		= LCD_INTSRC_NONE;		//是否用中断，不用
	
	iRet = csi_lcd_init(LCD, &tLcdCfg);
	if(iRet == CSI_OK)
	{
		csp_lcd_en(LCD);
		my_printf("LCD Init Succeed!\n");
	}
	
	for(i = 2; i < 26; i++)						//全部点亮
	{
		bySendBata[i] = 0x0f;
	}
	csi_lcd_write_data(LCD, bySendBata, 2, 22);
	
	my_printf("Light up all!\n");
	
	mdelay(500);
	
	
	do
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);	//回读当前时间
		for(i = 2; i < 26; i++)
		{
			if(7 == i)
			{
				bySendBata[i] |= 0x07;
				
				if(bySendBata[7] & 0x08)
					bySendBata[7] &= 0x07;		//seg7/com3
				else
					bySendBata[7] |= 0x08;		//seg7/com3
			}
			else
				bySendBata[i] = 0x0f;
		}
		
		lcd_rtc_data(bySendBata, 2, tRtcTimeRdbk.iMin/10);			//hour
		lcd_rtc_data(bySendBata, 4, tRtcTimeRdbk.iMin%10);			//hour
		lcd_rtc_data(bySendBata, 6, tRtcTimeRdbk.iSec/10);			//min
		lcd_rtc_data(bySendBata, 8, tRtcTimeRdbk.iSec%10);			//min
		
		csi_lcd_write_data(LCD, bySendBata,2,22);
		
		if(!((tRtcTimeRdbk.iSec+1)%2))
		{
			my_printf("Rtc Min = %d, Sec = %d \n", tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
			switch(ePmMode)
			{
				case PM_MODE_SLEEP:
					my_printf("Enter Sleep Mode\n");
					break;
				case PM_MODE_DEEPSLEEP:
					my_printf("Enter Deep-Sleep mode\n");
					break;
				case PM_MODE_SNOOZE:
					my_printf("Enter Snooze Mode\n");
					break;
				case PM_MODE_SHUTDOWN:
					my_printf("Enter ShutDown Mode\n");
					break;
				default:
					break;
			}
			
			//csp_set_ureg(SYSCON, 1, 0x55);				
			csi_pin_set_high(PA05);						//PA01 OUTPUT high,指示用
			csi_pin_set_high(PB02);						//PB02 OUTPUT high,指示用
			csi_pm_enter_sleep(ePmMode);				//进入snooze模式
			mdelay(100);
			csi_pin_set_low(PA05);
			mdelay(100);	
		}
		
		mdelay(500);

	}while(1);
	
	
	return iRet;
}