/***********************************************************************//** 
 * \file  lowPower_demo.c
 * \brief  low power demo code
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-2 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include <drv/pm.h>
#include <drv/pin.h>
#include <drv/rtc.h>
#include <drv/lpt.h>
#include <drv/irq.h>
#include <drv/iwdt.h>
#include <drv/tick.h>
#include <drv/common.h> 
#include <drv/reliability.h>

#include "demo.h"
#include "iostring.h"
#include "csp_syscon.h"
#include "board_config.h"

/* Private macro------------------------------------------------------*/
#define	SRAM1_LEN		256

/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
uint8_t *bypSramAddr = (uint8_t *)0x20002000;	


volatile uint8_t byExiFlg = 0x00;

/** \brief 进入低功耗模式前的准备动作
 *  \param[in] none
 *  \return error code
 */
static void prepare_lp(void)				  
{
	//USER CODE，如记忆管脚状态
}

/** \brief 退出低功耗模式后恢复动作
 *  \param[in] none
 *  \return error code
 */
static void wkup_lp(void)					   
{
	//USER CODE，如恢复管脚状态
	csi_pin_set_low(PB02);
}

/** \brief 通过外部PA00/PB00/PA12/PB011(ALV0~3)唤醒shutdown
 * 
 *  \param  none
 *  \return none
 */
void lp_exi_wakeup_shutdown_demo(void)
{
	uint16_t hwRstSrc; 
	uint8_t byWkIntSrc; 
	
	csi_pm_mode_e ePmMode = PM_MODE_SHUTDOWN;
	
	hwRstSrc = csi_get_rst_reason();							//获取复位信息，SHUTDOWN唤醒后，CHIP会复位
	if(hwRstSrc)
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);	//打印mcu复位信息
		byWkIntSrc = csi_pm_get_wkint();
		my_printf("WakeInt Source = 0x%x \n", byWkIntSrc);		//打印WkInt信息
		csi_pm_clr_wkint(byWkIntSrc);
		csi_clr_rst_reason(hwRstSrc);
	}
	
	csi_pin_set_mux(PA00,PA00_INPUT);				//PA00 INPUT as WAEKUP source
	csi_pin_pull_mode(PA00, GPIO_PULLDOWN);
	
	csi_pin_set_mux(PB02,PB02_OUTPUT);				//PB02 OUTPUT
	
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);


#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pm_config_wakeup_source(WKUP_ALV0, ENABLE);			//选择唤醒源WKUP_ALV0，即PA00唤醒，高电平唤醒
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	mdelay(10);
	
	switch(ePmMode)											//打印睡眠模式信息
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
	
	while(1) 
	{
		csi_pin_set_high(PB02);
		
		csi_pm_enter_sleep(ePmMode);						//进入睡眠模式
		mdelay(100);
		csi_pin_set_low(PB02);
		mdelay(100);
	}

}
/** \brief 通过RTC唤醒SNOOZE模式
 * 
 *  \param  none
 *  \return none
 */
void lp_rtc_wakeup_snooze_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SNOOZE;		
	uint16_t hwRstSrc = csi_get_rst_reason();
	
	if(hwRstSrc)									//获取并打印复位信息
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);
		csi_clr_rst_reason(hwRstSrc);				//清除复位信息
	}
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);				//PA05 OUTPUT
	
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_config_wakeup_source(WKUP_RTC, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	
	
	//rtc初始化配置
	{
		csi_rtc_config_t tRtcConfig;
		
		tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;  		//选择时钟源
		tRtcConfig.byFmt = RTC_24FMT;				  	//选择时间模式
		csi_rtc_init(RTC, &tRtcConfig);				  	//初始化RTC
		csi_rtc_start_as_timer(RTC, RTC_TIMER_0_5S);	//每0.5s进一次中断,即0.5s唤醒一次
		csi_rtc_start(RTC);	
	}
	mdelay(10);
	
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
	
	while(1) 
	{
		csi_pin_set_high(PA05);
		
		csi_pm_enter_sleep(ePmMode);
		csi_pin_set_low(PA05);
		mdelay(100);
	}
}

/** \brief 通过LPT唤醒DEEPSLEEP模式
 * 
 *  \param  none
 *  \return none
 */
void lp_lpt_wakeup_deepsleep_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_DEEPSLEEP;		
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);				//PA05 OUTPUT
	
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);

#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif

	csi_pm_config_wakeup_source(WKUP_LPT, ENABLE);			//配置唤醒源
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);					//SNOOZE模式下时钟开启/关闭
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	//lpt初始化配置
	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时500ms,默认采用PEND中断
	csi_lpt_start(LPT);	  
	mdelay(10);
	
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
	
	while(1) 
	{
		csi_pin_set_high(PA05);
		csi_pm_enter_sleep(ePmMode);
		my_printf("Wakeup From Deep-Sleep Mode...\n");
		csi_pin_set_low(PA05);
		mdelay(200);
	}
}

/** \brief 各种源唤醒低功耗的示例代码，低功耗的模式=sleep/deepsleep/snooze/shutdown
 * 		   sleep模式时,tick中断会唤醒cpu，测试时需要注意(可在system_init中注释掉tick初始化)
 * 
 *  \param  none
 *  \return none
 */
void lp_wakeup_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SHUTDOWN;			//PM_MODE_SLEEP/PM_MODE_DEEPSLEEP/PM_MODE_SNOOZE/PM_MODE_SHUTDOWN
	uint16_t hwRstSrc = csi_get_rst_reason();
	uint8_t byWkIntSrc; 
	
	if(hwRstSrc)												//获取并打印复位信息
	{
		my_printf("System Reset Source = 0x%x \n", hwRstSrc);
		csi_clr_rst_reason(hwRstSrc);							//清除复位信息
		
		//shutdown mode
		if(ePmMode == PM_MODE_SHUTDOWN)
		{
			byWkIntSrc = csi_pm_get_wkint();
			my_printf("WakeInt Source = 0x%x \n", byWkIntSrc);		//打印WkInt信息
			csi_pm_clr_wkint(byWkIntSrc);							//清除WkInt状态
		}
	}
	
	csi_pin_set_mux(PB02,PB02_OUTPUT);				//PB02 OUTPUT
	
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	csi_pin_toggle(PB02);
	mdelay(200);
	
#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pin_set_mux(PB01,PB01_INPUT);							//PB01 输入							
	csi_pin_pull_mode(PB01, GPIO_PULLUP);						//PB01 上拉
	csi_pin_irq_mode(PB01,EXI_GRP1, GPIO_IRQ_FALLING_EDGE);		//PB01 下降沿产生中断
	csi_pin_irq_enable(PB01, EXI_GRP1, ENABLE);					//PB01 中断使能，选择中断组0	
	csi_vic_set_wakeup_irq(EXI1_IRQ_NUM);
	
	csi_pm_clk_enable(SP_IDLE_PCLK, DISABLE);					//sleep模式下关闭PCLK
	csi_pm_clk_enable(SP_IDLE_HCLK, DISABLE);					//sleep模式下关闭HCLK
//	csi_pm_clk_enable(DP_ISOSC, ENABLE);
//	csi_pm_clk_enable(DP_IMOSC, ENABLE);
//	csi_pm_clk_enable(DP_ESOSC, ENABLE);
//	csi_pm_clk_enable(DP_EMOSC, ENABLE);
	
	//shutdown wkup source
	if(ePmMode == PM_MODE_SHUTDOWN)
	{
		csi_pin_set_mux(PA00,PA00_INPUT);						//PA00 INPUT as WAEKUP source(shutdown)
		csi_pin_pull_mode(PA00, GPIO_PULLDOWN);
		csi_pm_config_wakeup_source(WKUP_ALV0, ENABLE);			//选择唤醒源WKUP_ALV0，即PA00唤醒，高电平唤醒
	}

	
	//LPT WAKEUP DeepSleep/snooze
//	csi_lpt_timer_init(LPT,LPT_CLK_ISCLK, 500);       		//初始化lpt,选用内部超低功耗时钟,定时200ms,默认采用PEND中断
//	csi_lpt_start(LPT);	  
	
	//LVD WAKEUP	DeepSleep/snooze/shutdown
	//csi_lvd_int_enable(LVD_INTF,LVD_30);  						//VDD掉电到3.9V即触发LVD中断
	
	
	//CMP WAKUP DeepSleep
//	cmp_base_demo();
	
//	csi_pin_set_mux(SXIN_PIN, SXIN_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(SXOUT_PIN, SXOUT_PIN_FUNC);		//ESOSC管脚使能	
//	csi_pin_set_mux(PA03, PA03_OSC_XI);
//	csi_pin_set_mux(PA04, PA04_OSC_XO);
	//RTC WAKEUP DeepSleep/snooze/shutdown
//	{
//		csi_rtc_config_t tRtcConfig;
//		
//		tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;  		//选择时钟源
//		tRtcConfig.byFmt = RTC_24FMT;				  	//选择时间模式
//		csi_rtc_init(RTC, &tRtcConfig);				  	//初始化RTC
//		csi_rtc_start_as_timer(RTC, RTC_TIMER_0_5S);	//每1s进一次中断
//		csi_rtc_start(RTC);	
//	}

	//IWDT WAKEUP DeepSleep/snooze/shutdown
//	csi_iwdt_init(IWDT_TO_1024);						//初始化看门狗，溢出时间为1000ms(系统复位时间)
//	csi_iwdt_irq_enable(IWDT_ALARMTO_4_8, ENABLE);		//使能看门狗报警中断，报警时间为4/8溢出时间(500ms)
//	csi_iwdt_open();	
//	csi_iwdt_feed();
	
	mdelay(10);
	
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
	
	while(1) 
	{
		csi_pin_set_high(PB02);
		
		csi_pm_enter_sleep(ePmMode);
		//csi_iwdt_feed();
		//mdelay(100);
		csi_pin_set_low(PB02);
		delay_ums(100);
		//my_printf("Wakeup From Sleep Mode...\n");
	}
}

/** \brief write sram1
 *  \param[in] none
 *  \return error code
 */
static void write_sram1(uint8_t byWrData)
{
	uint16_t i;
	uint8_t byVal;
	
	for(i = 0; i < SRAM1_LEN; i++)		
	{
		*(bypSramAddr+i) = byWrData; 
	}
	
	for(i = 0; i < SRAM1_LEN; i++)			
	{
		byVal = *(bypSramAddr+i);
		if(byVal != byWrData)
			break;
	}
	
	if(i < SRAM1_LEN)
		my_printf("Write Sram1 Data = 0x%x Fail!\n", byWrData);
	else
		my_printf("Write Sram1 Data = 0x%x Success!\n", byWrData);
}

static void read_sram1(uint8_t byData)
{
	uint16_t i;
	uint8_t byVal;
	for(i = 0; i < SRAM1_LEN; i++)
	{
		byVal = *(bypSramAddr+i);
		if(byVal != byData)
			break;
	}
	
	if(i < SRAM1_LEN)
		my_printf("Read Sram1 Data = 0x%x Fail!\n", byData);
	else
		my_printf("Read Sram1 Data = 0x%x Success!\n", byData);
}

/** \brief test ram1 of shutdown/snooze mode 
 * 
 *  \param  none
 *  \return none
 */
void lp_shutdown_sram1_demo(void)
{
	csi_pm_mode_e ePmMode = PM_MODE_SNOOZE;		//PM_MODE_SNOOZE/PM_MODE_SHUTDOWN
	uint16_t hwRstSrc; 
	
	hwRstSrc = csi_get_rst_reason(); 				//获取复位信息并打印
	my_printf("System Reset Source = 0x%x \n", hwRstSrc);
	if((hwRstSrc & RST_SRC_SHD_WKUP) || (hwRstSrc &RST_SRC_SNOOZE_WKUP))	//snooze或者shutdown模式						
	{
		read_sram1(0xaa);							//shutdown复位，读SRAM1数据，根据写入的数据（0x55/0xaa）
	}
	else
	{
		write_sram1(0xaa);							//非shutdown复位写入数据
	}
	csi_clr_rst_reason(hwRstSrc);					//清除复位信息
	
	csi_pin_set_mux(PA19,PA19_OUTPUT);				//PA01 OUTPUT
	csi_pin_set_low(PA19);	
	
	csi_pin_set_mux(PA05,PA05_OUTPUT);				//PA05 OUTPUT
	
	csi_pin_toggle(PA05);							//延时处理4ms
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);							
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);
	csi_pin_toggle(PA05);
	mdelay(200);


#ifdef CONFIG_USER_PM	
	csi_pm_attach_callback(ePmMode, prepare_lp, wkup_lp);	//需要在工程设置compiler tab下加入define CONFIG_USER_PM=1;
#endif
	
	csi_pm_clk_enable(DP_ISOSC, DISABLE);					//关闭deepsleep 模式下OSC
	csi_pm_clk_enable(DP_IMOSC, DISABLE);
	csi_pm_clk_enable(DP_ESOSC, DISABLE);
	csi_pm_clk_enable(DP_EMOSC, DISABLE);
	
	csi_pm_config_wakeup_source(WKUP_LVD, ENABLE);			//lvd 唤醒
	
	//LVD WAKEUP	DeepSleep/snooze/shutdown
	csi_lvd_int_enable(LVD_INTF,LVD_30);  					//VDD掉电到3.9V即触发LVD中断
	
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
	
	while(1) 
	{
		csi_pin_set_high(PA19);
		
		csi_pm_enter_sleep(ePmMode);
		//mdelay(100);
		csi_pin_set_low(PA19);
		mdelay(100);
	}
}
