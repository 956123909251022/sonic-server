/***********************************************************************//** 
 * \file  demo.h
 * \brief  demo description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-11 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _DEMO_H
#define _DEMO_H

/* Includes ------------------------------------------------------------------*/
#include "common.h"


//uart demo
//uart send
int uart_char_demo(void);
int uart_send_demo(void);
int uart_send_int_demo(void);
//uart receive
int uart_receive_demo(void);
int uart_recv_rx_int_demo(void);
int uart_recv_rxfifo_int_demo(void);
int uart_send_dma_demo(void);
int uart_recv_dma_demo(void);

//usart
int usart_char_demo(void);
int usart_send_demo(void);
int usart_recv_demo(void);
int usart_send_int_demo(void);
int usart_recv_rx_int_demo(void);
int usart_recv_rxfifo_int_demo(void);
int usart_send_dma_demo(void);
int usart_recv_dma_demo(void);


//adc demo
//normal mode(no interrupt)
int adc_samp_oneshot_demo(void);
int adc_samp_oneshot_demo2(void);
int adc_samp_continuous_demo(void);
//interrupt mode
int adc_samp_oneshot_int_demo(void);
int adc_samp_continuous_int_demo(void);
//DMA传输
int adc_samp_continuous_dma_transfer_demo(void);



#endif