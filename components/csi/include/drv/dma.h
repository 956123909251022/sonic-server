/***********************************************************************//** 
 * \file  dma.h
 * \brief  head file of csi dma
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-12-23 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_DMA_H_
#define _DRV_DMA_H_

#include <stdint.h>
#include <stdio.h>
#include <drv/etb.h>
#include <drv/common.h>
#include <csp.h>
#ifdef __cplusplus
extern "C" {
#endif

/****** DMA Event *****/
//typedef enum {
//    DMA_EVENT_TRANSFER_DONE       = 0,  ///< transfer complete
//    DMA_EVENT_TRANSFER_HALF_DONE,       ///< transfer half done
//    DMA_EVENT_TRANSFER_ERROR,           ///< transfer error
//} csi_dma_event_t;

/**
 * \enum     csi_dma_addr_inc_e
 * \brief    DMA addr inc mode
 */
typedef enum {
    DMA_ADDR_INC    	= 0,
    DMA_ADDR_CONSTANT
	//DMA_ADDR_DEC
} csi_dma_addr_inc_e;

/**
 * \enum     csi_dma_dsize_t
 * \brief    DMA transfer data size width
 */
typedef enum {
    DMA_DSIZE_8_BITS  	= 0,
    DMA_DSIZE_16_BITS,
    DMA_DSIZE_32_BITS
} csi_dma_dsize_e;

/**
 * \enum     csi_dma_reload_e
 * \brief    DMA auto reload enable
 */
typedef enum{
	DMA_RELOAD_ENABLE	= 0,
	DMA_RELOAD_DISABLE
}csi_dma_reload_e;

/**
 * \enum     csi_dma_smode_e
 * \brief    DMA serve mode 
 */
typedef enum{
	DMA_TRANS_ONCE		= 0,
	DMA_TRANS_CONTINU				
}csi_dma_smode_e;

/**
 * \enum     csi_dma_tsize_e
 * \brief    DMA Tsize mode 
 */
typedef enum{
	DMA_TSIZE_ONE_DSIZE	= 0,
	DMA_TSIZE_FOUR_DSIZE
}csi_dma_tsize_e;

/**
 * \enum     csi_dma_req_e
 * \brief    DMA request mode 
 */
typedef enum{
	DMA_REQ_SOFTWARE	= 0,
	DMA_REQ_HARDWARE
}csi_dma_req_e;

/**
 * \enum     csi_etb_chid_e
 * \brief    ETB channel id(number 0~31)
 */
typedef enum {
    DMA_CH0				= 0,	//dma channel 0 
	DMA_CH1,					//dma channel 1 
	DMA_CH2,					//dma channel 2 
	DMA_CH3,					//dma channel 3 
} csi_dma_ch_e;

/**
 * \enum     csi_dma_int_msg_e
 * \brief    DMA interrupt message
 */
typedef enum {
    DMA_CH0_LTCIT_MSG	= (0x01ul << 0),	//dma channel 0 LTCIT messsage
	DMA_CH1_LTCIT_MSG	= (0x01ul << 1),	//dma channel 1 LTCIT messsage
	DMA_CH2_LTCIT_MSG	= (0x01ul << 2),	//dma channel 2 LTCIT messsage
	DMA_CH3_LTCIT_MSG	= (0x01ul << 3),	//dma channel 3 LTCIT messsage
	DMA_CH0_TCIT_MSG	= (0x01ul << 4),	//dma channel 0 TCIT messsage
	DMA_CH1_TCIT_MSG	= (0x01ul << 5),	//dma channel 1 TCIT messsage
	DMA_CH2_TCIT_MSG	= (0x01ul << 6),	//dma channel 2 TCIT messsage
	DMA_CH3_TCIT_MSG	= (0x01ul << 7),	//dma channel 3 TCIT messsage
} csi_dma_int_msg_e;

/**
 * \enum     csi_dma_intsrc_e
 * \brief    DMA interrupt source 
 */
typedef enum
{
	DMA_INTSRC_NONE   	=	(0x00ul << 0), 		//NONE interrupt
	DMA_INTSRC_LTCIT  	=	(0x01ul << 29),		//LTICT interrupt
	DMA_INTSRC_TCIT   	=	(0x01ul << 30)		//LTICT interrupt
}csi_dma_intsrc_e;

/// \struct csi_dma_ch_config_t
/// \brief  dma parameter configuration, open to users  
typedef struct
{
	uint8_t		bySrcLinc;		//low transfer count src addr inc control
	uint8_t		bySrcHinc;		//high transfer count src addr inc control
	uint8_t		byDetLinc;		//lowtransfer count det addr inc control
	uint8_t		byDetHinc;		//high transfer count det addr inc control
	uint8_t		byDataWidth;	//transfer data size width
	uint8_t		byReload;		//auto reload	
	uint8_t		byTransMode;	//dma serve(transfer) mode
	uint8_t		byTsizeMode;	//Tsize transfer mode
	uint8_t		byReqMode;		//request mode
	uint32_t	wInt;			//interrupt  
} csi_dma_ch_config_t;

/** 
  \brief 	   Init dma channel parameter config structure
  \param[in]   ptDmaBase	pointer of dma reg structure.
  \param[in]   eDmaCh		channel num of dma(4 channel: 0->3)
  \param[in]   ptChCfg		pointer of uart parameter config structure
  \return 	   error code \ref csi_error_t
 */ 
csi_error_t csi_dma_ch_init(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, csi_dma_ch_config_t *ptChCfg);

/**
  \brief       Start a dma channel
  \param[in]   eDmaCh       channel num of dma(4 channel: 0->3)
  \param[in]   pSrcAddr     transfer source address
  \param[in]   pDstAddr     transfer destination address
  \param[in]   hwHTranNum	high transfer num, hwHTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
  \param[in]   hwLTranNum	low transfer num,  hwLTranNum <= 0xfff; transfer number = hwHTranNum * hwLTranNum(TSIZE = ONCE)
			   transfer length (unit: bytes), if set data_width is 16, the length should be the multiple of 2, and
			   if set data_width is 32, the length should be the multiple of 4
  \return      error code \ref csi_error_t
*/
csi_error_t csi_dma_ch_start(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh, void *pSrcAddr, void *pDstAddr, uint16_t hwHTranNum, uint16_t hwLTranNum);

/** \brief dma channel transfer restart
 * 
 *  \param[in] ptDmaBase: pointer of dma reg structure.
 *  \param[in] eDmaCh: channel num of dma(4 channel: 0->3)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_dma_ch_restart(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh);

/** 
  \brief 	   enable/disable dma interrupt 
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel num of dma(4 channel: 0->3)
  \param[in]   eIntSrc		dma interrupt source
  \param[in]   bEnable		enable/disable interrupt
  \return none
 */
void csi_dma_int_enable(csp_dma_t *ptDmaBase,  csi_dma_ch_e eDmaCh, csi_dma_intsrc_e eIntSrc, bool bEnable);

/**
  \brief       Stop a dma channel
  \param[in]   ptDmaBase	pointer of dma register structure
  \param[in]   eDmaCh		channel num of dma(4 channel: 0->3)
  \return      none
*/
void csi_dma_ch_stop(csp_dma_t *ptDmaBase, csi_dma_ch_e eDmaCh);

/**
  \brief       Reset dma module
  \param[in]   ptDmaBase	pointer of dma register structure
  \return      none
*/
void csi_dma_soft_rst(csp_dma_t *ptDmaBase);


/** 
  \brief 	   get dma idx 
  \param[in]   ptDmaBase	pointer of uart register structure
  \return      dma id number(0~1) or error(0xff)
 */ 
//uint8_t csi_get_dma_idx(csp_dma_t *ptDmaBase);

/** 
  \brief 	   get dma interrupt message and (D0 not)clear message
  \param[in]   eDmaCh		dma channel number, channel 0->3
  \param[in]   bClrEn		bClrEn: clear dma interrupt message enable; ENABLE: clear , DISABLE: Do not clear
  \return 	   bool type true/false
 */ 
bool csi_dma_get_msg(csi_dma_ch_e eDmaCh, bool bClrEn);

/**
  \brief       enable dma power manage
  \param[in]   dma  dma handle to operate.
  \return      error code
*/
//csi_error_t csi_dma_enable_pm(csi_dma_t *dma);

/**
  \brief       disable dma power manage
  \param[in]   dma  dma handle to operate.
*/
//void        csi_dma_disable_pm(csi_dma_t *dma);

#ifdef __cplusplus
}
#endif

#endif /* _CSI_DMA_H_ */
