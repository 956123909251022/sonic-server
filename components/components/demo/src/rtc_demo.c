/***********************************************************************//** 
 * \file  rtc_demo.c
 * \brief  rtc demo code
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-5-27 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "rtc.h"
#include "bt.h"
#include "etb.h"
#include "pin.h"
#include "iostring.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/

/* private function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

//extern uint8_t AlarmA_Flag;
//extern uint8_t AlarmB_Flag;

/** \brief 设置rtc时间的示例代码：包括时钟源、时间模式、时间设置，当前时间回读函数
 * 
 *  \param[in] none
 *  \return    none
 */
void rtc_set_time_demo(void)
{	
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	uint32_t wSec = 0;
	
	csi_pin_set_mux(PA03, PA03_OSC_XI);			//设置PA0.3为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	csi_pin_set_mux(PA04, PA04_OSC_XO);         //设置PA0.4为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	
	csi_pin_set_mux(PA01,PA01_OSC_SXI);         //设置PA0.1为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	csi_pin_set_mux(PA02,PA02_OSC_SXO);         //设置PA0.2为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	
	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;		//外部副晶振作为RTC时钟源
	tRtcConfig.byFmt = RTC_24FMT;				//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
	
	
	tRtcTime.iYear = 22;
	tRtcTime.iMon  = 2;
	tRtcTime.iMday = 14;
//	tRtcTime.iWday = 1;                       //星期1-7
	tRtcTime.iPm   = RTC_AM;					// AM or PM
	tRtcTime.iHour = 9;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iHour = 9;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iMin  = 50;
	tRtcTime.iSec  = 59;	
	csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
	csi_rtc_start(RTC);							//RTC开始计时
	
	csi_rtc_change_fmt(RTC, RTC_12FMT);			//修改时间模式为12小时制
	tRtcTime.iYear = 22;
	tRtcTime.iMon  = 2;
	tRtcTime.iMday = 14;
//	tRtcTime.iWday = 1;                       //星期1-7
	tRtcTime.iPm   = RTC_AM;					// AM or PM
	tRtcTime.iHour = 11;
	tRtcTime.iHour = 9;
	tRtcTime.iMin  = 59;
	tRtcTime.iSec  = 56;	
	csi_rtc_set_time(RTC, &tRtcTime);			//修改时间以后需要重新start rtc
	csi_rtc_start(RTC);
	
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);		//回读当前时间
	my_printf("12FMT: %d:%d:%d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
	
	csi_rtc_change_fmt(RTC, RTC_24FMT);			//修改时间模式为24小时制
	csi_rtc_get_time(RTC,  &tRtcTimeRdbk);		//回读当前时间
	my_printf("24FMT: %d:%d:%d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec);
	
	csi_rtc_change_fmt(RTC, RTC_12FMT);			//修改时间模式为12小时制
	
	while(1)
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		if(wSec != tRtcTimeRdbk.iSec ) 
		{
			wSec = tRtcTimeRdbk.iSec;
			my_printf("%d:%d:%d pm= %d\n", tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
			
		}
	}
}

/** \brief 设置rtc 闹钟的示例代码：包括闹表模式、闹钟时间设置，当前时间回读和获取距离闹表时间到还有多久
 * 
 *  \param[in] none
 *  \return    none
 */

void rtc_alarm_demo(void)	
{	
	uint32_t wSec = 0;
	
	csi_rtc_time_t tRtcTime, tRtcTimeRdbk;
	csi_rtc_time_t tAlmTime;
	csi_rtc_time_t tBlmTime;
	csi_rtc_config_t tRtcConfig;
	
//	csi_pin_set_mux(PA03, PA03_OSC_XI);			//设置PA0.3为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PA04, PA04_OSC_XO);         //设置PA0.4为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	
	csi_pin_set_mux(PA01,PA01_OSC_SXI);         //设置PA0.1为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	csi_pin_set_mux(PA02,PA02_OSC_SXO);         //设置PA0.2为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	
	tRtcConfig.byClkSrc = RTC_CLKSRC_ESOSC;		//外部副晶振作为RTC时钟源
	tRtcConfig.byFmt    = RTC_24FMT;			//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				//初始化RTC
	
	tRtcTime.iYear = 22;                      //年
	tRtcTime.iMon  = 2;                       //月
	tRtcTime.iMday = 14;                      //日
//	tRtcTime.iWday = 1;                       //星期1-7
	tRtcTime.iHour = 9;                       //时
	tRtcTime.iMin  = 59;                      //分
	tRtcTime.iSec  = 55;                      //秒
	tRtcTime.iPm   = RTC_AM;	                //AM、PM
	csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
	csi_rtc_start(RTC);							//RTC开始工作
	
//	/*** 闹铃A设置 ***/
//	tAlmTime.iWday = THURSDAYS | TUESDAYS;        //星期二、四闹铃
//	tAlmTime.iHour = 5;                           //闹铃A时设置，如果不考虑小时则填0xFF
//	tAlmTime.iMin  = 0;                           //闹铃A分设置，如果不考虑分时则填0xFF
//	tAlmTime.iSec  = 0;							//闹铃A秒设置，如果不考虑秒则填0xFF
//	tAlmTime.iPm   = RTC_AM;                      //闹铃A上午、下午位设置，12H制有效，24H该位无效
//	csi_rtc_set_alarm(RTC, RTC_ALMA,&tAlmTime);	    //设置闹钟A	
	
	/*** 闹铃A设置 ***/
	tAlmTime.iWday = EVERYDAY;                  //每天闹铃
	tAlmTime.iHour = 10;			             //闹铃A时设置，如果不考虑小时则填0xFF
	tAlmTime.iMin  = 0;                        //闹铃A分设置，如果不考虑分时则填0xFF
	tAlmTime.iSec  = 3;			             //闹铃A秒设置，如果不考虑秒则填0xFF			
	tAlmTime.iPm   = RTC_AM;                   //闹铃A上午、下午位设置，12H制有效，24H该位无效	
	csi_rtc_set_alarm(RTC, RTC_ALMA,&tAlmTime);	 //设置闹钟A	
	
	
	/*** 闹铃B设置 ***/
//	tBlmTime.iWday =  SATURDAYS | SUNDAYS;     //星期六、日闹铃
	tBlmTime.iWday =  EVERYDAY;                //每天闹铃
	tBlmTime.iHour = 10;                       //闹铃B时设置，如果不考虑小时则填0xFF
	tBlmTime.iMin  = 0;                        //闹铃B分设置，如果不考虑分则填0xFF
	tBlmTime.iSec  = 5;			             //闹铃B秒设置，如果不考虑秒则填0xFF			    
	tBlmTime.iPm   = RTC_AM;                   //闹铃B上午、下午位设置，12H制有效，24H该位无效	
	csi_rtc_set_alarm(RTC,RTC_ALMB,&tBlmTime);	 //设置闹钟B	
	
	while(1)
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		if(wSec != tRtcTimeRdbk.iSec )
		{
			wSec = tRtcTimeRdbk.iSec;
			my_printf("20%d.%d.%d  %d  %d:%d:%d pm= %d\n",tRtcTimeRdbk.iYear,tRtcTimeRdbk.iMon,tRtcTimeRdbk.iMday, tRtcTimeRdbk.iWday,tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
		}
		if(csi_rtc_get_int_msg(RTC_INTSRC_ALMA,1)) //判断闹铃B中断信息是否产生，并且清除中断信息标志位
		{
			my_printf("RTC AlarmA has triggered!!\n");
		}
		if(csi_rtc_get_int_msg(RTC_INTSRC_ALMB,1)) //判断闹铃B中断信息是否产生，并且清除中断信息标志位
		{
			my_printf("RTC AlarmB has triggered!!\n");
		}
	}
}

/** \brief 示例如何将RTC当做一个简单timer来使用
 * 
 *  \param[in] none
 *  \return    none
 */
void rtc_timer_demo(void)
{
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime;
	
//	csi_pin_set_mux(PA03, PA03_OSC_XI);			//设置PA0.3为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PA04, PA04_OSC_XO);         //设置PA0.4为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	
//	csi_pin_set_mux(PA01,PA01_OSC_SXI);         //设置PA0.1为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PA02,PA02_OSC_SXO);         //设置PA0.2为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	
	tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;  	//选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				//初始化RTC
	
	tRtcTime.iYear = 22;
	tRtcTime.iMon  = 2;
	tRtcTime.iMday = 14;
//	tRtcTime.iWday = 1;                       //星期1-7
	tRtcTime.iHour = 1;
	tRtcTime.iMin  = 0;
	tRtcTime.iSec  = 0;
	tRtcTime.iPm   = RTC_AM;	
	csi_rtc_set_time(RTC, &tRtcTime);				//设置时间
	
	
	csi_rtc_start_as_timer(RTC, RTC_TIMER_1MIN);	  	//每1分钟进一次中断
	csi_rtc_start(RTC);									//RTC开始工作
	
	while(1);
}


/** \brief 示例RTC通过ETCB触发BT工作，AlarmA触发BT0 PWM输出、AlarmB触发BT1 PWM输出
 * 
 * 
 *  \param[in] none
 *  \return    none
 */
 void rtc_trgev_demo(void)	
 {
	csi_bt_pwm_config_t tPwmCfg;
	
	csi_etb_config_t tEtbConfig1,tEtbConfig2;
	csi_rtc_config_t tRtcConfig;
	csi_rtc_time_t tRtcTime,tRtcTimeRdbk;
	csi_rtc_time_t tAlmTime;
	csi_rtc_time_t tBlmTime;
	volatile uint8_t ch;
	uint32_t wSec = 0;
	
	
//	csi_pin_set_mux(PA03, PA03_OSC_XI);			//设置PA0.3为SXIN，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PA04, PA04_OSC_XO);         //设置PA0.4为SXOUT，如果使用外部主晶振作为RTC时钟源，必须先对管脚进行配置
	
//	csi_pin_set_mux(PA01,PA01_OSC_SXI);         //设置PA0.1为XIN，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
//	csi_pin_set_mux(PA02,PA02_OSC_SXO);         //设置PA0.2为XOUT，如果使用外部副晶振作为RTC时钟源，必须先对管脚进行配置
	
	tRtcConfig.byClkSrc = RTC_CLKSRC_ISOSC;		//选择时钟源
	tRtcConfig.byFmt = RTC_24FMT;				//选择时间模式
	csi_rtc_init(RTC, &tRtcConfig);				//初始化设置
	
	tRtcTime.iYear = 22;
	tRtcTime.iMon  = 2;
	tRtcTime.iMday = 14;
//	tRtcTime.iWday = 1;                       //星期1-7
	tRtcTime.iHour = 9;						//如果是12小时制，回读到的hour范围为1->12；如果是24小时制，回读到的hour范围是0->23
	tRtcTime.iMin = 59;
	tRtcTime.iSec = 55;
	tRtcTime.iPm  = RTC_AM;	
	csi_rtc_set_time(RTC, &tRtcTime);			//设置时间
	csi_rtc_start(RTC);							//RTC开始计时
	
	
	/*** 闹铃A设置 ***/
	tAlmTime.iWday = EVERYDAY;                 //每天准时闹铃
	tAlmTime.iHour = 10;			             //闹铃A时设置，如果不考虑小时则填0xFF
	tAlmTime.iMin  = 0;                        //闹铃A分设置，如果不考虑分时则填0xFF
	tAlmTime.iSec  = 5;			             //闹铃A秒设置，如果不考虑秒则填0xFF			
	tAlmTime.iPm   = RTC_AM;                   //闹铃A上午、下午位设置，12H制有效，24H该位无效	
	csi_rtc_set_alarm(RTC, RTC_ALMA,&tAlmTime);	 //设置闹钟A	
	
	
	/*** 闹铃B设置 ***/
	tBlmTime.iWday =  EVERYDAY;                //每天准时闹铃
	tBlmTime.iHour = 10;                       //闹铃B时设置，如果不考虑小时则填0xFF
	tBlmTime.iMin  = 0;                        //闹铃B分设置，如果不考虑分则填0xFF
	tBlmTime.iSec  = 6;			             //闹铃B秒设置，如果不考虑秒则填0xFF			    
	tBlmTime.iPm   = RTC_AM;                   //闹铃B上午、下午位设置，12H制有效，24H该位无效	
	csi_rtc_set_alarm(RTC,RTC_ALMB,&tBlmTime);	 //设置闹钟B	
	
	/*** RTC触发输出配置 ***/	
	csi_rtc_set_evtrg(RTC, RTC_TRGOUT0, RTC_TRGOUT_ALRMA, 0);  //RTC TRGEV0 闹铃A到时产生trigger event
	csi_rtc_set_evtrg(RTC, RTC_TRGOUT1, RTC_TRGOUT_ALRMB, 0);  //RTC TRGEV1 闹铃B到时产生trigger event
	
	/*** 触发目标事件BT0配置 ***/
	csi_pin_set_mux(PA19, PA19_BT0_OUT);					//PB02 作为BT0 PWM输出引脚
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;								//PWM 输出频率
	tPwmCfg.byInt		= BT_INTSRC_NONE;
	csi_bt_pwm_init(BT0, &tPwmCfg);							//初始化BT0 PWM输出
	csi_bt_set_sync(BT0, BT_TRG_SYNCIN0, BT_TRG_CONTINU, ENABLE);	//外部触发bt启动(SYNCIN0)
	
	/*** 触发目标事件BT1配置 ***/ 	
	csi_pin_set_mux(PA110, PA110_BT1_OUT);					//PB02 作为BT0 PWM输出引脚
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_HIGH;					//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_HIGH;				//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 1000;								//PWM 输出频率
	tPwmCfg.byInt		= BT_INTSRC_NONE;
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT0 PWM输出
	csi_bt_set_sync(BT1, BT_TRG_SYNCIN0, BT_TRG_CONTINU, ENABLE);	//外部触发bt启动(SYNCIN0)
	
	/*** ETCB0配置 ***/
	tEtbConfig1.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig1.bySrcIp  = ETB_RTC_TRGOUT0 ;  	    //RTC_SYNC0 触发输出0作为触发源
	tEtbConfig1.bySrcIp1 = 0xff;      
	tEtbConfig1.bySrcIp2 = 0xff;
	tEtbConfig1.byDstIp  = ETB_BT0_SYNCIN0;   	    //BT0 同步输入作为目标事件
	tEtbConfig1.byDstIp1 = 0xff;
	tEtbConfig1.byDstIp2 = 0xff;
	tEtbConfig1.byTrgMode = ETB_HARDWARE_TRG;       //硬件触发
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig1.byChType);	//自动获取空闲通道号,ch >= 0 获取成功
	csi_etb_ch_config(ch,&tEtbConfig1);
	
	/*** ETCB1配置 ***/
	tEtbConfig2.byChType = ETB_ONE_TRG_ONE;  		//单个源触发单个目标
	tEtbConfig2.bySrcIp  = ETB_RTC_TRGOUT1 ;  	    //RTC_SYNC0 触发输出0作为触发源
	tEtbConfig2.bySrcIp1 = 0xff;      
	tEtbConfig2.bySrcIp2 = 0xff;
	tEtbConfig2.byDstIp  =  5;   	                //BT1 同步输入作为目标事件
	tEtbConfig2.byDstIp1 = 0xff;
	tEtbConfig2.byDstIp2 = 0xff;
	tEtbConfig2.byTrgMode = ETB_HARDWARE_TRG;       //硬件触发
	csi_etb_init();
	ch = csi_etb_ch_alloc(tEtbConfig2.byChType);	//自动获取空闲通道号,ch >= 0 获取成功
	csi_etb_ch_config(ch,&tEtbConfig2);
	
	
	while(1)
	{
		csi_rtc_get_time(RTC,  &tRtcTimeRdbk);
		
		
		if(wSec != tRtcTimeRdbk.iSec ) 
		{
			wSec = tRtcTimeRdbk.iSec;
			my_printf("20%d.%d.%d  %d:%d:%d pm= %d\n",tRtcTimeRdbk.iYear,tRtcTimeRdbk.iMon,tRtcTimeRdbk.iMday, tRtcTimeRdbk.iHour, tRtcTimeRdbk.iMin, tRtcTimeRdbk.iSec, tRtcTimeRdbk.iPm);
		}

	}
 }