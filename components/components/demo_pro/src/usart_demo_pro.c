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

ringbuffer_t g_tRingbuf1;							//循环buffer
uint8_t 	 g_byRxBuf1[USART_RECV_MAX_LEN];		//接收缓存

//volatile uint8_t byRvUsart[30]={0};

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
						| USART_INTSRC_TIMEOUT;			//使用中断,使用FXFIFO中断（默认推荐）和接收超时中断
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

/** \brief usart interrupt handle function
 * 
 *  \param[in] ptUsartBase: pointer of usart register structure
 *  \param[in] byIdx: usart id number(0)
 *  \return none
 */ 
__attribute__((weak)) void usart_irqhandler(csp_usart_t *ptUsartBase,uint8_t byIdx)
{
	switch(csp_usart_get_isr(ptUsartBase) & 0x5100)			//rxfifo/txfifo/rxtimeout interrupt	
	{
		case US_RXRIS_INT:					//rx fifo interrupt
			if(g_tUsartTran[byIdx].ptRingBuf->hwDataLen < g_tUsartTran[byIdx].ptRingBuf->hwSize)	
			{
				while(csp_usart_get_sr(ptUsartBase) & US_RNE)		//Rxfifo non empty 
				{
					g_tUsartTran[byIdx].ptRingBuf->pbyBuf[g_tUsartTran[byIdx].ptRingBuf->hwWrite] = csp_usart_get_data(ptUsartBase);
					g_tUsartTran[byIdx].ptRingBuf->hwWrite = (g_tUsartTran[byIdx].ptRingBuf->hwWrite + 1) % g_tUsartTran[byIdx].ptRingBuf->hwSize;
					g_tUsartTran[byIdx].ptRingBuf->hwDataLen ++;
				}
			}
			else
			{
				csp_usart_cr_cmd(USART0, US_RSTRX | US_FIFO_EN | US_RXFIFO_1_2);	//reset rx 
				g_tUsartTran[byIdx].ptRingBuf->hwDataLen = 0;						//clear hwDataLen			
			}
			break;
		case US_TXRIS_INT:			//tx fifo interrupt								
			csp_usart_set_data(ptUsartBase, *g_tUsartTran[byIdx].pbyTxData);		//send data
			g_tUsartTran[byIdx].hwTxSize --;
			g_tUsartTran[byIdx].pbyTxData ++;
			
			if(g_tUsartTran[byIdx].hwTxSize == 0)
			{	
				//disable usart tx interrupt
				csp_usart_int_enable(ptUsartBase, US_TXRIS_INT, DISABLE);		
				g_tUsartTran[byIdx].bySendStat = USART_STATE_DONE;					//send complete
				
			}
			break;
		case US_TIMEOUT_INT:				//receive timeout interrupt
		
			if(g_tUsartTran[byIdx].ptRingBuf->hwDataLen < g_tUsartTran[byIdx].ptRingBuf->hwSize)	
			{
				while(csp_usart_get_sr(ptUsartBase) & US_RNE)	
				{
					g_tUsartTran[byIdx].ptRingBuf->pbyBuf[g_tUsartTran[byIdx].ptRingBuf->hwWrite] = csp_usart_get_data(ptUsartBase);
					g_tUsartTran[byIdx].ptRingBuf->hwWrite = (g_tUsartTran[byIdx].ptRingBuf->hwWrite + 1) % g_tUsartTran[byIdx].ptRingBuf->hwSize;
					g_tUsartTran[byIdx].ptRingBuf->hwDataLen ++;
				}
			}
			else
				csp_usart_cr_cmd(USART0, US_RSTRX | US_FIFO_EN | US_RXFIFO_1_2);	//reset rx 
			
			g_tUsartTran[byIdx].byRecvStat = USART_STATE_FULL;						//receive complete
			csp_usart_clr_isr(USART0,US_TIMEOUT_INT);								//clear interrupt status
			csp_usart_cr_cmd(USART0, US_STTTO | US_FIFO_EN | US_RXFIFO_1_2);		//enable receive timeover
			break;
			
		default:
			csp_usart_clr_isr(USART0, 0x7fff);										//clear all interrupt 
			break;
	}
}