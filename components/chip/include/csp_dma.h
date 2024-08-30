/***********************************************************************//** 
 * \file  csp_dma.h
 * \brief DMA description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-01 <td>V0.0  <td>ZJY   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_DMA_H
#define _CSP_DMA_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_dma_t
/// \brief DMA reg description      
typedef struct
{
	__IOM uint32_t	ISRX;  			//0x0000	Initial Source Addr Register
	__IOM uint32_t	ISCRX;			//0x0004	Initial Source Control Register
	__IOM uint32_t	IDRX;			//0x0008	Initial Target Addr Register
	__IOM uint32_t	IDCRX; 			//0x000C	Initial Target Control Register
	__IOM uint32_t	CRX;			//0x0010	Channelx Control Register
	__IM  uint32_t	SRX;			//0x0014	Channelx Status Register
	__IM  uint32_t	CSRX;			//0x0018	current Source Register
	__IM  uint32_t	CDRX;   		//0x001C	current Target Register
    __IOM uint32_t	MTRX;  			//0x0020	Mask Trigger Register
	__IOM uint32_t  RSRX;      		//0x0024    Request Select Register
	__IM  uint32_t	RSVD1[182];		//0x0028 
	__IM  uint32_t	RSVD2[128];		//0x04fc
	__IM  uint32_t	IDR;  			//0x0500	ID Register
	__OM  uint32_t	SRR;			//0x0504	Soft Reset Register
	__IM  uint32_t	CESR;			//0x0508	Status Register
	__IM  uint32_t	ISR; 			//0x050C	Interrupt Status Register
	__OM  uint32_t	ICR;			//0x0510	Interrupt Clear Register
} csp_dma_t; 


//dma reg base
#define	DMA_REG_BASE(dma, ch)	((uint32_t)dma + ch * 0x80)	

/******************************************************************************
* ISRX: Channelx Initial Source Addr Register
******************************************************************************/
#define DMA_SADDR_POS  		(0)				//Channel Init Source Addr 						  
#define DMA_SADDR_MSK  		(0xFFFFFFFF << DMA_SADDR_POS)	

/******************************************************************************
* IDRX: Channelx Initial Target Addr Register
******************************************************************************/
#define DMA_DADDR_POS  		(0)				// 					  
#define DMA_DADDR_MSK  		(0xFFFFFFFF << DMA_DADDR_POS)	

/******************************************************************************
* ISCRX: IDCRX: Channelx Initial Target Control Register
******************************************************************************/
#define DMA_LINC_POS		(0) 			//     					  
#define DMA_LINC_MSK		(0x01ul << DMA_LINC_POS)	
typedef enum{
	DMA_LINC_INC	= 0,
	DMA_LINC_CONST
}dma_linc_e;

#define DMA_HINC_POS		(1) 			//    					  
#define DMA_HINC_MSK		(0x01ul << DMA_DHINC_POS)	
typedef enum{
	DMA_HINC_INC	= 0,
	DMA_HINC_CONST
}dma_hinc_e;

/******************************************************************************
* CRX: Channelx Control Register 
******************************************************************************/
#define DMA_LTC_POS			(0) 			//     					  
#define DMA_LTC_MSK			(0xFFFul << DMA_LTC_POS)	
#define DMA_LTC(val)		(((val) & 0xFFF) << DMA_LTC_POS)

#define DMA_HTC_POS			(12) 			//     					  
#define DMA_HTC_MSK			(0xFFFul << DMA_HTC_POS)	
#define DMA_HTC(val)		(((val) & 0xFFF) << DMA_HTC_POS)

#define DMA_DSIZE_POS		(24) 			//    					  
#define DMA_DSIZE_MSK		(0x03ul << DMA_DSIZE_POS)	
typedef enum{
	DMA_DSIZE_BY	= 0,
	DMA_DSIZE_HW,
	DMA_DSIZE_W
}dma_dsize_e;

#define DMA_RELOAD_POS		(26) 			//    					  
#define DMA_RELOAD_MSK		(0x01ul << DMA_RELOAD_POS)	
typedef enum{
	DMA_RELOAD_EN	= 0,
	DMA_RELOAD_DIS
}dma_reload_e;

#define DMA_SMODE_POS		(27) 			//    					  
#define DMA_SMODE_MSK		(0x01ul << DMA_SMODE_POS)	
typedef enum{
	DMA_SMODE_ONCE	= 0,
	DMA_SMODE_CNTNU				//continuous
}dma_smode_e;

#define DMA_TSIZE_POS		(28) 			//    					  
#define DMA_TSIZE_MSK		(0x01ul << DMA_TSIZE_POS)	
typedef enum{
	DMA_TSIZE_NOR	= 0,
	DMA_TSIZE_MULT
}dma_tsize_e;

typedef enum{
	DMA_LTCIT		= (0x01ul << 29),
	DMA_TCIT		= (0x01ul << 30)
}dma_int_e;

/******************************************************************************
* SRX: Channelx Status Register
******************************************************************************/
#define DMA_CURR_LTC_POS	(0) 			//    					  
#define DMA_CURR_LTC_MSK	(0xFFFul << DMA_CURR_LTC_POS)	

#define DMA_CURR_HTC_POS	(12) 			//    					  
#define DMA_CURR_HTC_MSK	(0xFFFul << DMA_CURR_HTC_POS)	

#define DMA_LTCST_POS		(31) 			//    					  
#define DMA_LTCST_MSK		(0x01ul << DMA_LTCST_POS)	
#define DMA_LTCST			(0x01ul << DMA_LTCST_POS)	

/******************************************************************************
* CSRX: Channelx current Source Addr Register
******************************************************************************/
#define DMA_CSADDR_POS  	(0)				// 					  
#define DMA_CSADDR_MSK  	(0xFFFFFFFF << DMA_CSADDR_POS)	

/******************************************************************************
* CDRX: Channelx current Target Addr Register
******************************************************************************/
#define DMA_CDADDR_POS  	(0)				// 					  
#define DMA_CDADDR_MSK  	(0xFFFFFFFF << DMA_CDADDR_POS)	

/******************************************************************************
* MTRX: Mask Trigger Register
******************************************************************************/
#define DMA_SWTRIG_POS		(0) 			//    					  
#define DMA_SWTRIG_MSK		(0x01ul << DMA_SWTRIG_POS)
#define DMA_SWTRIG			(0x01ul) 	 	

#define DMA_STOP_POS		(2) 			//    					  
#define DMA_STOP_MSK		(0x01ul << DMA_STOP_POS)
#define DMA_STOP			(0x01ul) 					  

#define DMA_CHEN_POS		(1) 			//    					  
#define DMA_CHEN_MSK		(0x01ul << DMA_CHEN_POS)	
typedef enum{
	DMA_CHEN_DIS	= 0,
	DMA_CHEN_EN
}dma_chen_e;

/******************************************************************************
* RSRX: Channelx Request Select Register
******************************************************************************/
#define DMA_REQ_POS			(0) 			//    					  
#define DMA_REQ_MSK			(0x01ul << DMA_REQ_POS)	
typedef enum{
	DMA_REQ_SW		= 0,
	DMA_REQ_HW
}dma_req_e;

/******************************************************************************
* IDR: ID Register
******************************************************************************/
#define DMA_IDCODE_POS  	(0)				//DMA IP IDCODE  						  
#define DMA_IDCODE_MSK  	(0xFFFFFFFF << DMA_IDCODE_POS)	

/******************************************************************************
* SRR: Soft Reset Register
******************************************************************************/
#define DMA_SWRST_POS  		(0)				//DMA IP IDCODE  						  
#define DMA_SWRST_MSK  		(0x01ul << DMA_SWRST_POS)	
#define DMA_SWRST  			(0x01ul)	

/******************************************************************************
* CESR: Channel Enable Status Register
******************************************************************************/
typedef enum{
	DMA_CH0EN_SR	= (0x01ul << 0),
	DMA_CH1EN_SR	= (0x01ul << 1),
	DMA_CH2EN_SR	= (0x01ul << 2),
	DMA_CH3EN_SR	= (0x01ul << 3)
}dma_cesr_e;

///******************************************************************************
//* ISR: Interrupt Status Register
//******************************************************************************/
typedef enum{
	DMA_CH0_LTCIT_SR	= (0x01ul << 0),
	DMA_CH1_LTCIT_SR	= (0x01ul << 1),
	DMA_CH2_LTCIT_SR	= (0x01ul << 2),
	DMA_CH3_LTCIT_SR	= (0x01ul << 3),
	DMA_CH0_TCIT_SR		= (0x01ul << 16),
	DMA_CH1_TCIT_SR		= (0x01ul << 17),
	DMA_CH2_TCIT_SR		= (0x01ul << 18),
	DMA_CH3_TCIT_SR		= (0x01ul << 19),
}dma_isr_e;
/******************************************************************************
* ICR: Interrupt Clear Register
******************************************************************************/
typedef enum{
	DMA_CH0_IT		= (0x01ul << 0),
	DMA_CH1_IT		= (0x01ul << 1),
	DMA_CH2_IT		= (0x01ul << 2),
	DMA_CH3_IT		= (0x01ul << 3),
}dma_icr_e;

/******************************************************************************
********************** DMA inline Functions Declaration ***********************
******************************************************************************/
static inline void csp_dma_soft_rst(csp_dma_t *ptDmaBase) 
{
	ptDmaBase->SRR = DMA_SWRST_MSK;		
}

static inline void csp_dma_ch_swtrig(csp_dma_t *ptDmaBase) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->MTRX |= DMA_SWTRIG_MSK;
}

static inline void csp_dma_ch_stop(csp_dma_t *ptDmaBase) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->MTRX |= DMA_STOP_MSK;
}

static inline void csp_dma_set_ch(csp_dma_t *ptDmaBase, dma_dsize_e eDsize, dma_reload_e eReload, dma_smode_e eSmode, dma_tsize_e eTsize) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	
	ptDmaBase->CRX = ptDmaBase->CRX & ~(DMA_DSIZE_MSK | DMA_RELOAD_MSK | DMA_SMODE_MSK | DMA_TSIZE_MSK);
	ptDmaBase->CRX |= (eDsize << DMA_DSIZE_POS) | (eReload << DMA_RELOAD_POS) | (eSmode << DMA_SMODE_POS) | (eTsize << DMA_TSIZE_POS);
}

static inline void csp_dma_ch_en(csp_dma_t *ptDmaBase) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->MTRX = DMA_CHEN_MSK;
}
static inline void csp_dma_ch_dis(csp_dma_t *ptDmaBase) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->MTRX &= ~DMA_CHEN_MSK;
}

static inline void csp_dma_set_ch_req(csp_dma_t *ptDmaBase, dma_req_e eReq) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->RSRX = (ptDmaBase->RSRX & ~DMA_REQ_MSK) | (eReq << DMA_REQ_POS);
}

static inline void csp_dma_set_ch_src_addr(csp_dma_t *ptDmaBase, uint32_t wSrcAddr) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->ISRX = wSrcAddr;
}

static inline void csp_dma_set_ch_saddr_mode(csp_dma_t *ptDmaBase, dma_linc_e eLinc, dma_hinc_e eHinc) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->ISCRX = (eLinc << DMA_LINC_POS) | (eHinc << DMA_HINC_POS);
}

static inline void csp_dma_set_ch_dst_addr(csp_dma_t *ptDmaBase, uint32_t wDetAddr) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->IDRX = wDetAddr;
}

static inline void csp_dma_set_ch_daddr_mode(csp_dma_t *ptDmaBase, dma_linc_e eLinc, dma_hinc_e eHinc) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	ptDmaBase->IDCRX = (eLinc << DMA_LINC_POS) | (eHinc << DMA_HINC_POS);
}


static inline void csp_dma_set_ch_saddr(csp_dma_t *ptDmaBase, uint8_t byChnl, uint32_t wSrcAddr, dma_linc_e eLinc, dma_hinc_e eHinc) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	
	ptDmaBase->ISRX = wSrcAddr;
	ptDmaBase->ISCRX = (eLinc << DMA_LINC_POS)  | (eHinc << DMA_HINC_POS);
}

static inline void csp_dma_set_ch_daddr(csp_dma_t *ptDmaBase, uint8_t byChnl, uint32_t wDetAddr, dma_linc_e eLinc, dma_hinc_e eHinc) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	
	ptDmaBase->IDRX = wDetAddr;
	ptDmaBase->IDCRX = (eLinc << DMA_LINC_POS)  | (eHinc << DMA_HINC_POS);
	
}

static inline void csp_dma_set_ch_trans_num(csp_dma_t *ptDmaBase, uint16_t hwLtc, uint16_t hwHtc) 
{
	//csp_dma_t *ptDmaChBase = (csp_dma_t *)((uint32_t)ptDmaBase + byChnl * 0x80);
	
	ptDmaBase->CRX = (ptDmaBase->CRX & ~(DMA_LTC_MSK | DMA_HTC_MSK)) | DMA_LTC(hwLtc) | DMA_HTC(hwHtc);
}

static inline uint16_t csp_dma_get_curr_ltc(csp_dma_t *ptDmaBase)
{
	return (uint16_t)(ptDmaBase->SRX & DMA_CURR_LTC_MSK);
}

static inline uint16_t csp_dma_get_curr_htc(csp_dma_t *ptDmaBase)
{
	return (uint16_t)((ptDmaBase->SRX & DMA_CURR_HTC_MSK) >> DMA_CURR_HTC_POS);
}

static inline uint8_t csp_dma_get_ltcst(csp_dma_t *ptDmaBase)
{
	return (uint8_t)((ptDmaBase->SRX & DMA_LTCST_MSK) >>  DMA_LTCST_POS);
}

static inline uint32_t csp_dma_get_crx(csp_dma_t *ptDmaBase)
{
	return (uint32_t)(ptDmaBase->CRX);
}

static inline uint8_t csp_dma_get_rsrx(csp_dma_t *ptDmaBase)
{
	return (uint8_t)(ptDmaBase->RSRX & 0x01);
}

static inline uint32_t csp_dma_get_esr(csp_dma_t *ptDmaBase)
{
	return (uint32_t)(ptDmaBase->CESR);
}

static inline uint32_t csp_dma_get_isr(csp_dma_t *ptDmaBase)
{
	return (uint32_t)(ptDmaBase->ISR);
}


static inline void csp_dma_clr_isr(csp_dma_t *ptDmaBase, dma_icr_e eDmaIcr)
{
	ptDmaBase->ICR = eDmaIcr;
}

static inline void csp_dma_int_enable(csp_dma_t *ptDmaBase, dma_int_e eDmaInt, bool bEnable) 
{
	if(bEnable)
		ptDmaBase->CRX |= eDmaInt; 
	else
		ptDmaBase->CRX &= ~eDmaInt;
}

#endif