
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
	csi_pin_drive(PA16, GPIO_DRIVE_LV1);
	csi_pin_set_mux(PB05, PB05_LED_COM1);
	csi_pin_drive(PB05, GPIO_DRIVE_LV1);
	
	ptLedCfg.byClk = LED_PCLK_DIV8;
	ptLedCfg.hwComMask = 0x3;
	ptLedCfg.byBrt = LED_100;
	ptLedCfg.byOnTime = 120;
	ptLedCfg.byBreakTime = 100;
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
	tPwmCfg.byDutyCycle 	 = 10;								//pwm ouput duty cycle//PWM初始值			
	tPwmCfg.wFreq 			 = 3000;							//pwm ouput frequency	
	tPwmCfg.wInt 		 = GPTB_INTSRC_NONE;                    //interrupt
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
	uint8_t i,byTk_data = 0;
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
/*
 * 库版本说明
 * lib_110tkey_F_X_XX：跳频版本，Touch使用中断处理，占用更多的SRAM及程序空间，抗干扰能力最强
 * lib_110tkey_C_X_XX：默认版本，Touch使用中断处理，SRAM及程序占用空间更小，速度更快，抗干扰性能稍低
 * lib_110tkey_MC_X_XX：主循环版本，使用Touch需要调用tkey_main_prog();函数，扫描时间更长，完全不占用中断，按键越多扫描时间越长，按键速度也会越慢
 * lib_110tkey_DMA_X_XX：TOUCH->DMA版本，使用DMA模块传递touch采样数据，不使用touch中断，速度最快，占用最多的SRAM，使用此版本后不再允许配置DMA3
*/


/* *******************    库的使用说明      ****************************
①F_X_XX 和 C_X_XX版本：

	需要在coret_int_handler中断里调用csi_tkey_basecnt_process()函数，在tkey_int_handler中断里调用csi_tkey_int_process()函数
	（调用csi_tkey_int_process()函数，占用时间时间如果影响到coret_int_handler的运行的话，也可以配置别的定时器中断去调用该函数，
	建议进中断的时间控制在10ms左右，调用见隔越小，TKEY反应越灵敏），在coret_int_handler的示例如下：
	void coret_int_handler(void) 
	{
	#if	CORET_INT_HANDLE_EN
		// ISR content ...
		tick_irqhandler();		//system coret 
		#if	TKEY_INT_HANDLE_EN
			#if	defined(IS_CHIP_1103)
				csi_tkey_basecnt_process();
			#endif
		#endif
	#endif	
	}
	
	void tkey_int_handler(void) 
	{
	#if	TKEY_INT_HANDLE_EN
		#if	defined(IS_CHIP_1103)
		// ISR content ...
		csi_tkey_int_process();
		#endif
	#endif
	}
②DMA_X_XX版本：
    只需要在coret_int_handler中断里调用csi_tkey_basecnt_process()函数，tkey_int_handler中断里不需要调用相关函数。
 
③MC_X_XX版本：

   只需要在主循环调用csi_tkey_main_prog();定时器中断和TKEY中断函数里不需要调用任何函数

 
所有版本的使用除了调用函数的地方不一样和需要调用的函数不一样，其他处理机制都是一样的。


*/
void tkey_demo(void)
{
	csi_pin_set_mux(PB01,PB01_OUTPUT);//LED
	gptb0_init();				//蜂鸣器
	csi_gptb_stop(GPTB0);//stop  timer
#ifdef	_serialplot_debug_en
	uart_init();
#endif
	led_init();					//LED数码管
	csi_tkey_init();
	delay_ums(3000);
	csi_gptb_stop(GPTB0);
	csi_gptb_start(GPTB0);
	while(1)
	{
		//csi_tkey_main_prog();//只有MC_X_XX版本才需要该函数。
		tk_led();
		if(dwKey_Map!=0)
		{
			if(dwKey_Map!=dwKey_Map_bk)
			{
				dwKey_Map_bk=dwKey_Map;
				csi_pin_set_low(PB01);
				///csi_gptb_start(GPTB0);//start  timer
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

	
void gptb0_irqhandler(csp_gptb_t *ptGptbBase)
{
	if(((csp_gptb_get_isr(ptGptbBase) & GPTB_INT_CBU))==GPTB_INT_CBU) {
	    csp_gptb_clr_isr(ptGptbBase, GPTB_INT_CBU);	   	
	}	
}	