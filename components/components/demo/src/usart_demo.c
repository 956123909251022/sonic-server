/***********************************************************************//** 
 * \file  uart_test.c
 * \brief  UART_TEST description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-30 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
/* Includes ---------------------------------------------------------------*/
#include <string.h>
#include <drv/usart.h>
#include <drv/pin.h>


#include "demo.h"

/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private macro-----------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/

ringbuffer_t g_tRingbuf1;						//循环buffer
uint8_t 	 g_byRxBuf1[USART_RECV_MAX_LEN];	//接收缓存

static uint8_t s_byRecvBuf[64];					//接收缓存
static uint8_t s_byRecvLen = 0;	

/** \brief usart dma send data
 *  \brief USART通过DMA发送数据，确保ETCB模块初始化(使能)，etcb初始化函数：csi_etb_init()
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_send_dma_demo(void)
{
	int iRet = 0;
	uint8_t bySdData[36]={31,32,33,34,5,6,7,8,9,10,21,22,23,24,25,26,27,28,29,30,31,32,33,34,35,36,37,38,39,40,1,2,3};
	volatile uint8_t byRecv;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//CK,同步模式时使用
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置

//	csi_pin_set_mux(PA14, PA14_USART0_TX);				//TX	
//	csi_pin_set_mux(PA15, PA15_USART0_RX);				//RX
//	csi_pin_set_mux(PA13, PA13_USART0_CK);				//CK
//	csi_pin_pull_mode(PA15,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置	
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV8;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_NONE;		//使用TXFIFO中断（默认推荐）
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	csi_etb_init();										//使能ETB模块

	csi_usart_dma_tx_init(USART0, DMA_CH0, ETB_CH10);	//发送DMA初始化，选择DMA通道和ETB触发通道，DMA_CH: 0~3; ETB_CH: 8~11
	
	while(1)
	{
		byRecv = csi_usart_getc(USART0);
		if(byRecv == 0x06)
			csi_usart_send_dma(USART0,(void *)bySdData, DMA_CH0, 31);	//采用DMA方式发送
		mdelay(10);
		if(csi_dma_get_msg(DMA_CH0, ENABLE))			//获取发送完成消息，并清除消息
		{
			//添加用户代码
			nop;
		}
		nop;
		nop;
	}
	
	return iRet;
}

/** \brief usart dma receive data
 *  \brief USART通过DMA接收数据，确保ETCB模块初始化(使能)，etcb初始化函数：csi_etb_init()
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_dma_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//CK，同步模式时使用
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
//	csi_pin_set_mux(PA14, PA14_USART0_TX);				//TX	
//	csi_pin_set_mux(PA15, PA15_USART0_RX);				//RX
//	csi_pin_set_mux(PA13, PA13_USART0_CK);				//CK
	
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_NONE;		//使用TXFIFO中断（默认推荐）
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	csi_etb_init();										//使能ETB模块

	csi_usart_dma_rx_init(USART0, DMA_CH3, ETB_CH11);			//DMA接收初始化，选择DMA通道和ETB触发通道，DMA_CH: 0~3; ETB_CH: 8~11
	csi_usart_recv_dma(USART0,(void*)s_byRecvBuf, DMA_CH3, 25);	//DMA接收
	
	while(1)
	{
		if(csi_dma_get_msg(DMA_CH3, ENABLE))						//获取接收完成消息，并清除消息
		{
			//添加用户代码
			csi_usart_send(USART0, (void*)s_byRecvBuf, 25);
			nop;
			break;
		}
		mdelay(10);
		nop;
	}
	
	return iRet;
}

/** \brief usart char receive and send 
 *  \brief USART接收/发送一个字符，轮询方式
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_char_demo(void)
{
	int iRet = 0;
	volatile uint8_t byRecv;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//USART0 TX管脚配置	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//USART0 RX管脚配置
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//CK，同步模式时使用
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_NONE;		//使用TXFIFO中断（默认推荐）
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		byRecv = csi_usart_getc(USART0);
		csi_usart_putc(USART0, byRecv+1);
		
		mdelay(10);
	}
	
	return iRet;
}

/** \brief usart send a bunch of data; polling(sync,no interrupt)mode
 *  \brief USART发送一串数据，TX使用轮询
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_send_demo(void)
{
	int iRet = 0;
	uint8_t bySdData[30]={31,25,20,34,55,6,7,8,9,10,21,22,23,24,25,26,10,11,12,13,14,15,16,17,18,19,1,2,3};
	volatile uint8_t byRecv;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体

	csi_pin_set_mux(PB02, PB02_USART0_TX);				//USART0 TX管脚配置	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//USART0 RX管脚配置
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//CK，同步模式时使用
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_NONE;		//不使用中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		byRecv = csi_usart_getc(USART0);
		if(byRecv == 0x06)
			byRecv = csi_usart_send(USART0,(void *)bySdData,18);		//采用轮询方式,调用该函数时，UART发送中断关闭
		
		nop;
	}

	return iRet;
}

/** \brief usart send a bunch of data; interrupt(nsync) mode
 *  \brief USART发送一串数据，TX使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_send_int_demo(void)
{
	int iRet = 0;
	uint8_t bySdData[30]={31,25,20,34,55,6,7,8,9,10,21,22,23,24,25,26,10,11,12,13,14,15,16,17,18,19,1,2,3};
	volatile uint8_t byRecv;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体

	csi_pin_set_mux(PB02, PB02_USART0_TX);				//USART0 TX管脚配置	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//USART0 RX管脚配置
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//CK，同步模式时使用
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_TXRIS;		//使用TXFIFO中断（默认推荐）
	tUsartCfg.byTxMode		= USART_TX_MODE_INT;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口	
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能


	while(1)
	{
		byRecv = csi_usart_getc(USART0);
		if(byRecv == 0x06)
		{
			csi_usart_send(USART0,(void *)bySdData,18);		//采用中断方式。调用改函数时，UART发送中断使能
			while(1)			
			{
				//如果有需要，可用于判断发送是否完成；
				if(csi_usart_get_msg(USART0, USART_SEND, ENABLE))		//获取发送完成消息，并清除状态(设置为idle)，串口发送一串数据
				{
					//发送状态有三种，IDLE(空闲)/SEND(发送中)/DONE(发送完成)
					//具体定义参考：uart.h中csi_uart_state_e,
					nop;;
					break;
				}
			}
		}
		nop;
	}

	return iRet;	
}

/** \brief usart receive a bunch of data; polling(sync) mode
 *  \brief USART接收指定长度数据，RX使用轮询(不使用中断)，带超时处理(单位：ms)
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_demo(void)
{
	int iRet = 0;
	volatile uint8_t byRxBuf[32] = {0};
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	volatile uint8_t byRecv;
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_NONE; 		//不/使用中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询模式
	tUsartCfg.byRxMode		= USART_RX_MODE_POLL;		//接收模式：轮询模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		byRecv = csi_usart_receive(USART0, (void *)byRxBuf,16,1000);	//UART接收采用轮询方式(同步)	
		if(byRecv == 16)
			csi_usart_send(USART0,(void *)byRxBuf,byRecv);				//UART发送采用轮询方式(同步)	
	}

	return iRet;
}

/** \brief USART中断函数，接收数据使用中断方式(FIFO/RX两种中断)，在此中断函数中接收数据
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] byIdx: usart id number(0)
 */
//void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx)
//{
//	//此中断例程支持RXFIFO/RX/TXDONE/RXTO四种中断
//	switch(csp_usart_get_isr(ptUsartBase) & 0x5101)								//获取rxfifo/tx/txfifo/rxtimeout中断状态
//	{
//		case US_RXRIS_INT:					
//			//使用接收FIFO中断接收数据
//			while(csp_usart_get_sr(ptUsartBase) & US_RNE)						//接收FIFO非空
//			{
//				s_byRecvBuf[s_byRecvLen] = csp_usart_get_data(ptUsartBase);
//				s_byRecvLen ++;
//				if(s_byRecvLen > 31)											//接收完32个bytes，接收buf从头开始接收										
//				{
//					csi_usart_send(ptUsartBase,(void *)s_byRecvBuf, s_byRecvLen);//UART发送采用轮询方式，发送接收到的32bytes
//					s_byRecvLen = 0;
//				}
//			}
//			break;
//		case US_RXRDY_INT:	
//			//使用RX接收中断接收数据
//			s_byRecvBuf[s_byRecvLen] = csp_usart_get_data(ptUsartBase);
//			s_byRecvLen ++;
//			if(s_byRecvLen > 31)												//接收完32个bytes，接收buf从头开始接收										
//			{
//				csi_usart_send(ptUsartBase,(void *)s_byRecvBuf, s_byRecvLen);	//UART发送采用轮询方式，发送接收到的32bytes
//				s_byRecvLen = 0;
//			}
//			break;
//		case US_TXRIS_INT:					
//			//使用发送FIFO中断发送数据，下面处理支持csi_usart_send接口
//			//用户可按自己习惯方式处理中断发送（此时不支持csi_usart_send接口）
//			csp_usart_set_data(ptUsartBase, *g_tUsartTran[byIdx].pbyTxData);	//发送数据
//			g_tUsartTran[byIdx].hwTxSize --;
//			g_tUsartTran[byIdx].pbyTxData ++;
//			
//			if(g_tUsartTran[byIdx].hwTxSize == 0)
//			{	
//				csp_usart_int_enable(ptUsartBase, US_TXRIS_INT, DISABLE);		//关闭发送FIFO中断		
//				g_tUsartTran[byIdx].bySendStat = USART_STATE_DONE;				//发送完成
//			}
//			break;
//		case US_TIMEOUT_INT:				
//			//字节接收超时中断，可以作为一串字符是否结束的依据
//			//使用接收FIFO中断时，为了效率FIFO接收中断触发点初始化默认配置为FIFO的1/2(4个字节)，达不到4个字节时，不能触发FIFO接收中断
//			//使能接收超时中断可以接收到达不到触发FIFO中断的数据
//			while(csp_usart_get_sr(ptUsartBase) & US_RNE)						//接收FIFO非空
//			{
//				s_byRecvBuf[s_byRecvLen] = csp_usart_get_data(ptUsartBase);
//				s_byRecvLen ++;
//			}
//			csp_usart_clr_isr(ptUsartBase,US_TIMEOUT_INT);						//清除中断标志(状态)
//			csp_usart_cr_cmd(USART0, US_STTTO | US_FIFO_EN | US_RXFIFO_1_2);	//使能字节接收超时和FIFO配置
//			break;
//		default:
//			break;
//	}
//} 
/** \brief USART接收中断，RX使用接收中断，TX不使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_rx_int_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= DISABLE;					//禁止接收超时
	tUsartCfg.wInt			= USART_INTSRC_RXRDY; 		//使用RX接收中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_INT;		//接收模式：中断接收模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能

	return iRet;
}

/** \brief USART接收FIFO中断，RX使用FIFO中断，TX不使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_rxfifo_int_demo(void)
{
	int iRet = 0;
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= ENABLE;					//使能接收超时
	tUsartCfg.wInt			= USART_INTSRC_RXRIS 
					| USART_INTSRC_TIMEOUT;				//使用RXFIFO中断（默认推荐）和接收超时中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_INT;		//接收模式：中断接收模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能

	return iRet;
}

/** \brief usart receive assign(fixed) length data; interrupt(async) mode
 *  \brief USART接收指定长度数据，RX使用中断，TX不使用中断
 * 
 *  \param[in] none
 *  \return error code
 */
int usart_recv_int_demo(void)
{
	int iRet = 0;
	uint8_t byRxBuf[32];
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	uint16_t hwRecvNum = 1;
	volatile uint16_t hwRecvLen;
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	csi_pin_pull_mode(PA06,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	
	//接收缓存配置，实例化接收ringbuf，将ringbuf接收数据缓存指向用户定义的的接收buffer(g_byRxBuf)
	//需要传入参数：串口设备/ringbuf结构体指针/接收buffer/接收buffer长度
	csi_usart_set_buffer(USART0, &g_tRingbuf1, g_byRxBuf1, sizeof(g_byRxBuf1));
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= ENABLE;					//使能接收超时
	tUsartCfg.wInt			= USART_INTSRC_RXRIS 
						| USART_INTSRC_TIMEOUT;			//使用中断,使用FXFIFO中断（默认推荐）和接收超时中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询/中断模式
	tUsartCfg.byRxMode		= USART_RX_MODE_INT_FIX;	//接收模式：中断定长接收模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		//从串口缓存（UART接收循环buffer）里面读取数据，返回读取数据个数
		//用户应用根据实际不同协议来处理数据
		
		if(hwRecvNum == 1)		//单个字节收数据(读接收ringbuf)
		{
			hwRecvLen = csi_usart_receive(USART0,(void *)byRxBuf, hwRecvNum, 0);	//读取接收循环buffer数据, 有数据返回数据
			if(hwRecvLen == hwRecvNum)
				csi_usart_putc(USART0,*byRxBuf);
		}
		else if(hwRecvNum > 1)	//多个字节收数据(读接收ringbuf)
		{
			hwRecvLen = csi_usart_receive(USART0,(void *)byRxBuf, hwRecvNum, 0);	//读取接收循环buffer数据
			if(hwRecvLen == hwRecvNum)
				csi_usart_send(USART0,(void *)byRxBuf, hwRecvNum);				//UART发送采用轮询方式(同步)
		}
	}

	return iRet;
}
/** \brief usart receive a bunch of data; interrupt(async) mode
 *  \brief USART接收到一串字符串，RX使用中断模式，TX不使用中断
 *
 *  \param[in] none
 *  \return error code
 */
int usart_recv_dynamic_demo(void)
{
	int iRet = 0;
	uint8_t byRxBuf[128];
	csi_usart_config_t tUsartCfg;						//USART0 参数配置结构体
	volatile uint16_t hwRecvLen;
	
	csi_pin_set_mux(PB02, PB02_USART0_TX);				//TX	
	csi_pin_set_mux(PA06, PA06_USART0_RX);				//RX
	//csi_pin_set_mux(PA07, PA07_USART0_CK);				//RX	
	csi_pin_pull_mode(PA06,GPIO_PULLUP);

//	csi_pin_set_mux(PA14, PA14_USART0_TX);				//TX	
//	csi_pin_set_mux(PA15, PA15_USART0_RX);				//RX
//	csi_pin_set_mux(PA13, PA13_USART0_CK);				//CK
//	csi_pin_pull_mode(PA15,GPIO_PULLUP);				//RX管脚上拉使能, 建议配置
	
	//接收缓存配置，实例化接收ringbuf，将ringbuf接收数据缓存指向用户定义的的接收buffer(g_byRxBuf)
	//需要传入参数：串口设备/ringbuf结构体指针/接收buffer/接收buffer长度
	csi_usart_set_buffer(USART0, &g_tRingbuf1, g_byRxBuf1, sizeof(g_byRxBuf1));
	
	tUsartCfg.byClkSrc 		= USART_CLKSRC_DIV1;		//clk = PCLK
	tUsartCfg.byMode		= USART_MODE_ASYNC;			//异步模式
	tUsartCfg.byDatabit 	= USART_DATA_BITS_8;		//字节长度，8bit
	tUsartCfg.byStopbit 	= USART_STOP_BITS_1;		//停止位，1个
	tUsartCfg.byParity		= USART_PARITY_EVEN;		//偶校验
	tUsartCfg.bClkOutEn		= DISABLE;					//禁止USARTCLK输出；同步模式时，USARTCLK可以给另外设备上的USART提供clk，作为同步输入时钟使用
	tUsartCfg.wBaudRate 	= 115200;					//波特率：115200
	tUsartCfg.hwRecvTo		= 88;						//USART接收超时时间，单位：bit位周期，8个bytes(11bit*8=88, 115200波特率时=764us)
	tUsartCfg.bRecvToEn		= ENABLE;					//使能接收超时
	tUsartCfg.wInt			= USART_INTSRC_RXRIS 
					| USART_INTSRC_TIMEOUT;				//使用中断,使用FXFIFO中断（默认推荐）和接收超时中断
	tUsartCfg.byTxMode		= USART_TX_MODE_POLL;		//发送模式：轮询模式
	tUsartCfg.byRxMode		= USART_RX_MODE_INT_DYN;	//接收模式：中断动态接收模式
	
	csi_usart_init(USART0, &tUsartCfg);					//初始化串口
	csi_usart_start(USART0, USART_FUNC_RX_TX);			//开启USART的RX和TX功能，也可单独开启RX或者TX功能
	
	while(1)
	{
		
		if(csi_usart_get_msg(USART0,USART_RECV, ENABLE))				//获取判接收完成消息，并清除状态(设置为idle)，串口接收到一串字符
		{
			hwRecvLen = csi_usart_receive(USART0,(void*)byRxBuf,0,0);	//获取接收到的一串数据，返回数据长度, 后面两个参数无意义
			
			csi_usart_send(USART0,(void *)byRxBuf, hwRecvLen);			//UART发送采用轮询方式(同步)
			nop;
		}
		nop;
	}

	return iRet;
}