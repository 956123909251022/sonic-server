
/*============================================================================
 * Name        : main.c
 * Author      : $(username)
 * Version     : 0.0.0
 * Copyright   : Your copyright notice
 * Description : Simple function in C, Ansi-style
 *============================================================================
 */
/* include ----------------------------------------------------------------*/
#include "demo.h"
#include "pin.h"
#include "led.h"
#include "tkey.h"
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>

/* defines------------------------------------------------------*/
//#define _serialplot_debug_en
uint32_t dwKey_Map_bk;
unsigned char LED_CHAR_[11] = {
0x3F,//0
0x06,//1
0x5B,//2
0x4F,//3
0x66,//4
0x6D,//5
0x7D,//6
0x07,//7
0x7F,//8
0x6F,//9
0XFF
};
/* externs variablesr------------------------------------------------------*/
extern volatile uint16_t hwSampling_data0[32];
extern volatile uint16_t hwBaseline_data0[32];
extern volatile uint16_t hwSampling_data1[32];
extern volatile uint16_t hwBaseline_data1[32];
extern volatile uint16_t hwSampling_data2[32];
extern volatile uint16_t hwBaseline_data2[32];
extern volatile uint16_t hwOffset_data0_abs[32];
extern volatile uint16_t hwOffset_data1_abs[32];
extern volatile uint16_t hwOffset_data2_abs[32];

void uart_init(void)
{
	csi_uart_config_t tUartConfig;				//UART1 参数配置结构体
	
	csi_pin_set_mux(PB00, PB00_UART2_TX);		//TX	
	
	tUartConfig.byParity = UART_PARITY_NONE;		//校验位，奇校验
	tUartConfig.wBaudRate = 115200;				//波特率，115200
	tUartConfig.wInt = UART_INTSRC_NONE;		//UART中断关闭，轮询(同步)方式
	tUartConfig.byTxMode = UART_TX_MODE_POLL;	//发送 轮询模式
	tUartConfig.byRxMode = UART_RX_MODE_POLL;	//接收 轮询模式
	
	csi_uart_init(UART2, &tUartConfig);			//初始化串口
	csi_uart_start(UART2, UART_FUNC_TX);		//开启UART的RX和TX功能，也可单独开启RX或者TX功能
}

void led_init(void)
{
	csi_led_config_t ptLedCfg;
	csi_pin_set_mux(PA013, PA013_LED_S7);
	csi_pin_set_mux(PA012, PA012_LED_S6);
	csi_pin_set_mux(PA011, PA011_LED_S5);
	csi_pin_set_mux(PA010, PA010_LED_S4);
	csi_pin_set_mux(PA09, PA09_LED_S3);
	csi_pin_set_mux(PA08, PA08_LED_S2);
	csi_pin_set_mux(PA07, PA07_LED_S1);
	csi_pin_set_mux(PA06, PA06_LED_S0);
	csi_pin_set_mux(PA16, PA16_LED_COM0);
	csi_pin_set_mux(PB05, PB05_LED_COM1);
	
	ptLedCfg.byClk = LED_PCLK_DIV8;
	ptLedCfg.hwComMask = 0x3;
	ptLedCfg.byBrt = LED_100;
	ptLedCfg.byOnTime = 0x7f;
	ptLedCfg.byBreakTime = 50;
	ptLedCfg.byInt = LED_INTSRC_NONE;
	
	csi_led_init(LED, &ptLedCfg);		
	csi_led_lighton(LED);	

	csi_led_write_data(LED, 0, 0x40);
	csi_led_write_data(LED, 1, 0x40);

}


void gptb0_init(void)
{
	csi_pin_set_mux(PB02, PB02_GPTB0_CHAX);
	csi_gptb_pwmconfig_t tPwmCfg;
	tPwmCfg.byWorkmod        = GPTB_WAVE;                       //WAVE  波形模式
	tPwmCfg.byCountingMode   = GPTB_UPDNCNT;                    //CNYMD  //计数方向
	tPwmCfg.byOneshotMode    = GPTB_OP_CONT;                    //OPM    //单次或连续(工作方式)
	tPwmCfg.byStartSrc       = GPTB_SYNC_START;					//软件使能同步触发使能控制（RSSR中START控制位）//启动方式
	tPwmCfg.byPscld          = GPTB_LDPSCR_ZRO;                 //PSCR(分频)活动寄存器载入控制。活动寄存器在配置条件满足时，从影子寄存器载入更新值		
	tPwmCfg.byDutyCycle 	 = 50;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 3000;							//pwm ouput frequency	
	tPwmCfg.wInt 		 = GPTB_INTSRC_CBU;                    //interrupt
	csi_gptb_wave_init(GPTB0, &tPwmCfg);
	csi_gptb_pwmchannel_config_t  channel;
	channel.byActionZro    =   B_LO;
	channel.byActionPrd    =   B_NA;
	channel.byActionC1u    =   B_HI;
	channel.byActionC1d    =   B_LO;
	channel.byActionC2u    =   B_HI;
	channel.byActionC2d    =   B_LO;
	channel.byActionT1u    =   B_LO;
	channel.byActionT1d    =   B_LO;
	channel.byActionT2u    =   B_NA;
	channel.byActionT2d    =   B_NA;
	channel.byChoiceC1sel  =   GPTB_CMPA;
	channel.byChoiceC2sel  =   GPTB_CMPA;
	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_1);//channel
	channel.byChoiceC1sel  =   GPTB_CMPB;
	channel.byChoiceC2sel  =   GPTB_CMPB;
	csi_gptb_channel_config(GPTB0, &channel,  GPTB_CHANNEL_2);
	csi_gptb_start(GPTB0);//start  timer
}
void tk_led(void)
{
	uint8_t i,byTk_data;
	uint32_t wTemp = dwKey_Map;

	if(wTemp == 0) {
		byTk_data=0xff;
	}
	else {
		for(i = 0; i < 32; i++) {
			if(wTemp & 0x01)
			{
				byTk_data = i;        
				break;
			}
			wTemp = (wTemp >> 1);
		}
	}

	if(byTk_data==0xff) {
		csi_led_write_data(LED, 1, 0x40);   
		csi_led_write_data(LED, 0, 0x40);   
	}
	else {//显示对应的TKEY通道号
		csi_led_write_data(LED, 1,  LED_CHAR_[byTk_data%10]);
		csi_led_write_data(LED, 0,LED_CHAR_[byTk_data/10]);
	}
}

/*
 * 操作步骤
 * ① 右键chip组件，选择ptions for "XXX"选项
 * ② linker选项卡界面，Library Name输入对应的库名称，如:_110Tkey_F_1_00.a，版本说明请参考csi使用文档的相关章节
 * ③ Library Path后面输入路径：drivers
 * 然后在sdk_110x->apt32f110x_chip->drivers->tkey_parameter.c里配置相关的参数，配置说明请参考csi使用文档的相关章节
 */

void tkey_demo(void)
{
	csi_pin_set_mux(PB01,PB01_OUTPUT);//LED
	gptb0_init();				//蜂鸣器
#ifdef	_serialplot_debug_en
	uart_init();
#endif
	led_init();					//LED数码管
	csi_tkey_init();
	delay_ums(3000);
	while(1)
	{
		tk_led();
		if(dwKey_Map!=0)
		{
			if(dwKey_Map!=dwKey_Map_bk)
			{
				dwKey_Map_bk=dwKey_Map;
				csi_pin_set_low(PB01);
				csi_gptb_start(GPTB0);//start  timer
				delay_ums(300);
			}
			
		}else
		{
			dwKey_Map_bk=0;
			csi_pin_set_high(PB01);
			csi_gptb_stop(GPTB0);//stop  timer
		}
		//tkey 采样值打印，用于配合上位机观察实时波形
#ifdef	_serialplot_debug_en
		csi_uart_putc(UART2, 0X0D);
		csi_uart_putc(UART2, 0X0A);
		for(int i =0;i<16;i++)
		{

			csi_uart_putc(UART2, (hwSampling_data0[i]>>8));
			csi_uart_putc(UART2, (hwSampling_data0[i]&0xff));
			
			csi_uart_putc(UART2, (hwBaseline_data0[i]>>8));
			csi_uart_putc(UART2, (hwBaseline_data0[i]&0xff));
			
		}
#endif
	}
}




