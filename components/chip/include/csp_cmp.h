/***********************************************************************//** 
 * \file  csp_cmp.h
 * \brief CMP description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-11-18 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_CMP_H
#define _CSP_CMP_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_cmp_t
/// \brief CMP reg description   

typedef struct
{
   __IOM uint32_t  CEDR;  		//0x0000	Clock control & ID
   __IOM uint32_t  CR;		    //0x0004	Start & Stop Ctrl
   __IOM uint32_t  DFCR1;		//0x0008	Digital filter 1 control register 
   __IOM uint32_t  DFCR2;  		//0x000C	Digital filter 2 control register 
   __IOM uint32_t  WFCR;		//0x0010	captures the window control register
   __IOM uint32_t  INPCR;   	//0x0014	Input control register
   __IM  uint32_t  RISR;       	//0x0018    Interrupt RISR
   __IOM uint32_t  IMCR;       	//0x001c    Interrupt IMCR
   __IM  uint32_t  MISR;       	//0x0020    Interrupt MISR
   __OM  uint32_t  ICR;        	//0x0024    Interrupt clear
} csp_cmp_t;


#define CMP_INT_MSK         (0x01)

#define	CMP_INT_RAWDET		(16)
#define	CMP_INT_RAWDET_MSK	(0x01ul << CMP_INT_RAWDET)

typedef enum
{
	CMP_EDGEDET_INT  =  (0x01ul << 0),  
	CMP_RAWDET_INT   =  (0x01ul << 16),  
	
}
cmp_int_e;

#define	CMP_CLK_POS			(0)
#define	CMP_CLK_MSK			(0x01ul << CMP_CLK_POS)

#define	CMP_SWRST           (7)
#define CMP_SOFTRESET       (0x01<<CMP_SWRST)

#define	CMP_PHYSTPOL		    (1)
#define	CMP_PHYSTPOL_MSK		(0x07ul << CMP_PHYSTPOL)
typedef enum
{
	PHYST_0mv			=	0x00,
	PHYST_5mv,
	PHYST_10mv,
	PHYST_15mv,
	PHYST_20mv,
	PHYST_25mv,
	PHYST_40mv,	
	PHYST_60mv				
}cmp_phystpol_e;


#define	CMP_PHYSTSEL		    (4)
#define	CMP_PHYSTSEL_MSK		(0x03ul << CMP_PHYSTSEL)
typedef enum
{
	PHYSEL_DIS			=	0x00,
	PHYSEL_P,
	PHYSEL_N,
	PHYSEL_NP
			
}cmp_phystsel_e;

#define	CMP_POLARITY		    (6)
#define	CMP_POLARITY_MSK		(0x01ul << CMP_POLARITY)
typedef enum
{
    CMP_OUT_DIRECT  = 		(0x00ul),
	CMP_OUT_UNDIRECT = 	    (0x01ul)
}cmp_polarity_e;



#define	CMP_SYNCOE		    (7)
#define	CMP_SYNCOE_MSK		(0x01ul << CMP_SYNCOE)


#define	CMP_EVE_SEL		    (8)
#define	CMP_EVE_SEL_MSK		(0x03ul << CMP_EVE_SEL)
typedef enum
{
	EVE_SEL_DOWN			=	0x00,
	EVE_SEL_UP,
	EVE_SEL_DOWN_UP,
	EVE_SEL_UP1
			
}cmp_eve_sel_e;

#define	CMP_DFLT1EN		    (10)
#define	CMP_DFLT1EN_MSK		(0x01ul << CMP_DFLT1EN)

#define	CMP_DFLT2EN		    (11)
#define	CMP_DFLT2EN_MSK		(0x01ul << CMP_DFLT2EN)

#define	CMP_WFLTEN		    (12)
#define	CMP_WFLTEN_MSK		(0x01ul << CMP_WFLTEN)

#define	CMP_WFALIGN		    (13)
#define	CMP_WFALIGN_MSK		(0x01ul << CMP_WFALIGN)
typedef enum
{
	ALIGN_DISALLOW_LAST   = 0x00,
	ALIGN_ALLOW_LAST      = 0x01
	
}cmp_wfalign_e;

#define	CMP_WFOSET		    (14)
#define	CMP_WFOSET_MSK		(0x03ul << CMP_WFOSET)
typedef enum
{
	WFOSET_DIS			=	0x00,
	WFOSET_HIGH,
	WFOSET_LOW,
	WFOSET_DIS1
}cmp_wfoset_e;

#define	CMP_LPWKEN		    (20)
#define	CMP_LPWKEN_MSK		(0x01ul << CMP_LPWKEN)

#define	CMP_CPOSEL		    (23)
#define	CMP_CPOSEL_MSK		(0x01ul << CMP_CPOSEL)

typedef enum
{
	CPOS_DIRECT			=	0X00,
	CPOS_1				=	0X01
}cmp_cr_cpos_e;

#define	CMP_CPOUT0		    (24)
#define	CMP_CPOUT0_MSK		(0x01ul << CMP_CPOUT0)

#define	CMP_CPOUT1		    (25)
#define	CMP_CPOUT1_MSK		(0x01ul << CMP_CPOUT1)

//CMP_DFCR1 register
#define	CMP_DFCR1_DEPTH		    (0)
#define	CMP_DFCR1_DEPTH_MSK		(0x03ul << CMP_DFCR1_DEPTH)
typedef enum
{
	DEPTH1_8        =   0,
	DEPTH1_16       =   1,
	DEPTH1_32       =   2,
	DEPTH1_64       =   3,
	DEPTH1_128      =   4,
	DEPTH1_256      =   5,
	DEPTH1_512      =   6,
	DEPTH1_1024     =   7
			
}dfcr1_depth_e;

#define	CMP_DFCR1_DIVN		    (16)
#define	CMP_DFCR1_DIVN_MSK		(0x1ful << CMP_DFCR1_DIVN)

#define	CMP_DFCR1_DIVM		    (8)
#define	CMP_DFCR1_DIVM_MSK		(0xfful << CMP_DFCR1_DIVM)

//CMP_DFCR2 register
#define	CMP_DFCR2_DEPTH		    (0)
#define	CMP_DFCR2_DEPTH_MSK		(0x03ul << CMP_DFCR2_DEPTH)
typedef enum
{
	DEPTH2_16       =   0,
	DEPTH2_32,
	DEPTH2_64,
	DEPTH2_128,
	DEPTH2_256,
	DEPTH2_512,
	DEPTH2_1024,
	DEPTH2_2048
			
}dfcr2_depth_e;

#define	CMP_DFCR2_DIVN		    (16)
#define	CMP_DFCR2_DIVN_MSK		(0x1ful << CMP_DFCR2_DIVN)

#define	CMP_DFCR2_DIVM		    (8)
#define	CMP_DFCR2_DIVM_MSK		(0xfful << CMP_DFCR2_DIVM)

//CMP_WFCR register
#define	CMP_WFCR_WCNT		    (0)
#define	CMP_WFCR_WCNT_MSK		(0x3fful << CMP_WFCR_WCNT)

#define	CMP_WFCR_CLKDIV		    (10)
#define	CMP_WFCR_CLKDIV_MSK		(0x3ful << CMP_WFCR_CLKDIV)

#define	CMP_WFCR_DCNT		    (16)
#define	CMP_WFCR_DCNT_MSK		(0xfful << CMP_WFCR_DCNT)

//CMP_INPCR register
#define	CMP_INPCR_NSEL		    (0)
#define	CMP_INPCR_NSEL_MSK		(0xful << CMP_INPCR_NSEL)

#define	CMP_INPCR_1VBUF_ENABLE		    (16)
#define	CMP_INPCR_1VBUF_ENABLE_MSK		(0x1ul << CMP_INPCR_1VBUF_ENABLE)


typedef enum
{
	NSEL_CP0   =  0,
	NSEL_CP1,
	NSEL_CP2,
	NSEL_CP3,	
	NSEL_CP4,
	NSEL_CP5,
	NSEL_1VBUF =  7
}nsel_e;

#define	CMP_INPCR_PSEL		    (8)
#define	CMP_INPCR_PSEL_MSK		(0xful << CMP_INPCR_PSEL)
typedef enum
{
	PSEL_CP0   = 0,
	PSEL_CP1,
	PSEL_CP2,
	PSEL_CP3,	
	PSEL_CP4,
	PSEL_CP5,
	PSEL_CP6,
	PSEL_CP7
}psel_e;

/******************************************************************************
********************** CMP inline Functions Declaration ***********************
******************************************************************************/

/** \brief cmp reg clk enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \param[in] bEnable: enable/disable cmp clk
 *  \return none
 */ 
static inline void csp_cmp_clk_enable(csp_cmp_t *ptCmpBase, bool bEnable)
{
	if(bEnable)
		ptCmpBase->CEDR |=  CMP_CLK_MSK;	
	else
		ptCmpBase->CEDR &= ~CMP_CLK_MSK;	
		
}

/** \brief cmp reg reset software
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */ 
static inline void  csp_cmp_software_reset(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CEDR |= CMP_SOFTRESET;

}

/** \brief cmp enable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
static inline void csp_cmp_enable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR|=0x01;;
}

/** \brief cmp disable
 * 
 *  \param[in] ptCmpBase: pointer of CMP reg structure.
 *  \return none
 */
static inline void csp_cmp_disable(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->CR&=0xfffffffe;	
}

static inline void  csp_cmp_hystpol(csp_cmp_t *ptCmpBase , cmp_phystpol_e ePhystpol,cmp_phystsel_e ePhystsel)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_PHYSTPOL_MSK|CMP_PHYSTSEL_MSK))|(ePhystpol<<CMP_PHYSTPOL)|(ePhystsel<<CMP_PHYSTSEL);
}

static inline void  csp_cmp_polarity(csp_cmp_t *ptCmpBase , cmp_polarity_e ePolarity)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_POLARITY_MSK))|(ePolarity<<CMP_POLARITY);
}

static inline void  csp_cmp_evtrg(csp_cmp_t *ptCmpBase , bool bEnable,cmp_eve_sel_e eEveSel)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_SYNCOE_MSK|CMP_EVE_SEL_MSK))|(bEnable<<CMP_SYNCOE)|(eEveSel<<CMP_EVE_SEL);
}

static inline void  csp_cmp_dflt_enable(csp_cmp_t *ptCmpBase , bool bEnable1,bool bEnable2)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_DFLT1EN_MSK|CMP_DFLT2EN_MSK))|(bEnable1<<CMP_DFLT1EN)|(bEnable2<<CMP_DFLT2EN);
}

static inline  void csp_cmp_lpwken_enable(csp_cmp_t *ptCmpBase,bool bEnable)
{
	ptCmpBase->CR = (ptCmpBase->CR & ~CMP_LPWKEN_MSK) | (bEnable<<CMP_LPWKEN);
}

static inline void  csp_cmp_wf_set(csp_cmp_t *ptCmpBase , bool bEnable,cmp_wfalign_e eWfalign,cmp_wfoset_e eWfoset)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_WFLTEN_MSK|CMP_WFALIGN_MSK|CMP_WFOSET_MSK))|(bEnable<<CMP_WFLTEN)|(eWfalign<<CMP_WFALIGN)|(eWfoset<<CMP_WFOSET);
}

static inline void  csp_cmp_out(csp_cmp_t *ptCmpBase , cmp_cr_cpos_e ePos_Sel)
{
	ptCmpBase->CR = (ptCmpBase->CR&~(CMP_CPOSEL_MSK))|(ePos_Sel<<CMP_CPOSEL);
}

static inline uint8_t csp_cmp_get_out0(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT0_MSK) >> CMP_CPOUT0);
}

static inline uint8_t csp_cmp_get_out1(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase->CR & CMP_CPOUT1_MSK) >> CMP_CPOUT1);
}

static inline void  csp_cmp_dfcr1(csp_cmp_t *ptCmpBase , dfcr1_depth_e eDepth,uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR1 = (ptCmpBase->DFCR1&~(CMP_DFCR1_DEPTH_MSK|CMP_DFCR1_DIVN|CMP_DFCR1_DIVM))|(eDepth<<CMP_DFCR1_DEPTH)|(byDivn<<CMP_DFCR1_DIVN)|(byDivm<<CMP_DFCR1_DIVM);
}

static inline void  csp_cmp_dfcr2(csp_cmp_t *ptCmpBase , dfcr2_depth_e eDepth,uint8_t byDivn,uint8_t byDivm)
{
	ptCmpBase->DFCR2 = (ptCmpBase->DFCR2&~(CMP_DFCR2_DEPTH_MSK|CMP_DFCR2_DIVN|CMP_DFCR2_DIVM))|(eDepth<<CMP_DFCR2_DEPTH)|(byDivn<<CMP_DFCR2_DIVN)|(byDivm<<CMP_DFCR2_DIVM);
}

static inline void  csp_cmp_wfcr(csp_cmp_t *ptCmpBase, uint16_t hwWcnt,uint8_t byClkDiv,uint8_t byDcnt)
{
	ptCmpBase->WFCR = (ptCmpBase->WFCR&~(CMP_WFCR_WCNT_MSK|CMP_WFCR_CLKDIV_MSK|CMP_WFCR_DCNT_MSK))|(hwWcnt<<CMP_WFCR_WCNT)|(byClkDiv<<CMP_WFCR_CLKDIV)|(byDcnt<<CMP_WFCR_DCNT);
}

static inline void  csp_cmp_inpcr(csp_cmp_t *ptCmpBase ,nsel_e eNsel,psel_e ePsel)
{
	ptCmpBase->INPCR = (ptCmpBase->INPCR&~(CMP_INPCR_NSEL_MSK|CMP_INPCR_PSEL_MSK))|(eNsel<<CMP_INPCR_NSEL)|(ePsel<<CMP_INPCR_PSEL);
}

static inline void  csp_cmp_inpcr_1vbuf_enable(csp_cmp_t *ptCmpBase ,bool bEnable)
{
	if(bEnable)
		ptCmpBase->INPCR |= CMP_INPCR_1VBUF_ENABLE_MSK; 
	else
	    ptCmpBase->INPCR &= ~CMP_INPCR_1VBUF_ENABLE_MSK; 	
}

static inline uint8_t csp_cmp_get_risr(csp_cmp_t *ptCmpBase)
{
	return (uint8_t)((ptCmpBase-> RISR)&0x03);
}

static inline void csp_cmp_int_enable(csp_cmp_t *ptCmpBase, cmp_int_e eCmpInt,bool bEnable)
{
	if(bEnable)
		ptCmpBase->IMCR |= eCmpInt; 
	else
	    ptCmpBase->IMCR &= ~eCmpInt; 
}

static inline void csp_cmp_int_clear(csp_cmp_t *ptCmpBase,cmp_int_e eCmpInt)
{
	ptCmpBase->ICR|= eCmpInt;
}

static inline  void csp_cmp_edgedet_int_enable(csp_cmp_t *ptCmpBase,bool bEnable)
{
	ptCmpBase->IMCR = (ptCmpBase->IMCR & ~CMP_INT_MSK) | bEnable;
}

static inline  void csp_cmp_rawdet_int_enable(csp_cmp_t *ptCmpBase,bool bEnable)
{
	ptCmpBase->IMCR = (ptCmpBase->IMCR & ~CMP_INT_RAWDET_MSK) | (bEnable<<CMP_INT_RAWDET);
}

static inline uint32_t csp_cmp_get_imcr(csp_cmp_t *ptCmpBase)
{
	return (uint32_t)(ptCmpBase-> IMCR);
}

static inline uint32_t csp_cmp_get_misr(csp_cmp_t *ptCmpBase)
{
	return (uint32_t)(ptCmpBase-> MISR);
}

static inline void csp_cmp_edgedet_int_clear(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->ICR|= 0x01;
}

static inline void csp_cmp_rawdet_int_clear(csp_cmp_t *ptCmpBase)
{
	ptCmpBase->ICR|= (0x01<<16);
}

#endif

