
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
#include "tkey.h"
#include <string.h>
#include <csi_drv.h>
#include <iostring.h>

/* defines------------------------------------------------------*/
uint32_t dwKey_Map_bk;

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

void usart_init(void)
{
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PA08, PA08_USART0_TX);				//USART0 TX管脚配置	

	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_NONE;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.wInt			= USART_INTSRC_NONE;		//使用TXFIFO中断（默认推荐）
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
}


void bt_init(void)
{
	csi_bt_pwm_config_t tPwmCfg;				//BT PWM输出参数初始化配置结构体
	
	csi_pin_set_mux(PA09, PA09_BT1_OUT);		//P09 作为BT0 PWM输出引脚
	
	//init timer pwm para config
	tPwmCfg.byIdleLevel = BT_PWM_IDLE_LOW;		//PWM 输出空闲电平
	tPwmCfg.byStartLevel= BT_PWM_START_LOW;		//PWM 输出起始电平
	tPwmCfg.byDutyCycle = 50;								//PWM 输出占空比(0 < DutyCycle < 100)		
	tPwmCfg.wFreq 		= 4000;								//PWM 输出频率
	//tPwmCfg.byInt 	= BT_INTSRC_PEND | BT_INTSRC_CMP;	//PWM 中断配置(PEND and CMP)
	tPwmCfg.byInt		= BT_INTSRC_NONE;
	
	csi_bt_pwm_init(BT1, &tPwmCfg);							//初始化BT1 PWM输出
	BT1->RSSR |=0X01;
}

/*
 * 操作步骤
 * ① 右键chip组件，选择ptions for "XXX"选项
 * ② linker选项卡界面，Library Name输入对应的库名称，如:_110Tkey_F_1_00.a
 * ③ Library Path后面输入路径：drivers
 * 然后在tkey_parameter.c里配置相关的参数
 */

void tkey_demo(void)
{
	SYSCON->CLCR = 0xa5a09500; //B4
	csi_pin_set_mux(PA011,PA011_OUTPUT);//LED
	//bt_init(); //用于蜂鸣器驱动
	usart_init();			
	csi_tkey_init();
	delay_ums(1000);
	while(1)
	{
		
		if(dwKey_Map!=0)
		{
			if(dwKey_Map!=dwKey_Map_bk)
			{
				dwKey_Map_bk=dwKey_Map;
				csi_pin_set_low(PA011);
				//BT1->RSSR |=0X01;//用于蜂鸣器驱动
			}
			
		}else
		{
			dwKey_Map_bk=0;
			csi_pin_set_high(PA011);
			//BT1->RSSR &=0X0;//用于蜂鸣器驱动
		}
		//tkey 采样值打印，用于配合上位机观察实时波形
		csi_usart_putc(USART0, 0X0D);
		csi_usart_putc(USART0, 0X0A);
		for(int i =15;i<32;i++)
		{

			csi_usart_putc(USART0, (hwSampling_data0[i]>>8));
			csi_usart_putc(USART0, (hwSampling_data0[i]&0xff));
			
			csi_usart_putc(USART0, (hwBaseline_data0[i]>>8));
			csi_usart_putc(USART0, (hwBaseline_data0[i]&0xff));
			
		}
	
	}
}




