
/***********************************************************************//** 
 * \file  drv_config.h
 * \brief  csi drivers head file 
 * \copyright Copyright (C) 2015-2022 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-4-2 <td>V0.0   <td>ZJY    <td>initial
 * </table>
 * *********************************************************************
*/
#ifndef _CSI_DRV_H_
#define _CSI_DRV_H_

 

//drv
#include <drv/clk.h>
#include <drv/irq.h>
#include <drv/ifc.h>
#include <drv/tick.h>
#include <drv/crc.h>
#include <drv/iwdt.h>
#include <drv/wwdt.h>
#include <drv/adc.h>
#include <drv/rtc.h>
#include <drv/pm.h>
#include <drv/hwdiv.h>
#include <drv/reliability.h>
#include <drv/pin.h>
#include <drv/gpio.h>
#include <drv/dma.h>
#include <drv/etb.h>
#include <drv/bt.h>
#include <drv/lpt.h>
#include <drv/cnta.h>
#include <drv/ept.h>
#include <drv/gpta.h>
#include <drv/gptb.h>
#include <drv/iic.h>
#include <drv/sio.h>
#include <drv/spi.h>
#include <drv/lin.h>
#include <drv/uart.h>
#include <drv/usart.h>
//
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
#include <drv/cmp.h>
#endif
//
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
#include <drv/led.h>
#endif
//
#if	defined(IS_CHIP_1103) || defined(IS_CHIP_1104)
#include <drv/lcd.h>
#endif
//
#if	defined(IS_CHIP_1101) || defined(IS_CHIP_1103)
#include <drv/tkey.h>
#endif


#ifdef __cplusplus
extern "C" {
#endif

#ifdef __cplusplus
}
#endif

#endif /* _CSI_DRV_H_ */