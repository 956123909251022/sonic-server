/***********************************************************************//** 
 * \file  cmp.h
 * \brief header file for cmp driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-11-22 <td>V0.0  <td>YYM   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CMP_H_
#define _DRV_CMP_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

#define CMP0_OUT     0            //out low
#define CMP1_OUT     1            //out high

 
#define CMP0_EDGEDET0_INT         (1<<0)
#define CMP1_EDGEDET1_INT         (1<<1)

#define CMP0_RAWDET0_INT          (1<<16)
#define CMP1_RAWDET1_INT          (1<<17)

typedef enum
{
	CMP_INTSRC_NONE     =  (0x00ul << 0), 
	CMP_INTSRC_EDGEDET  =  (0x01ul << 0),  
	CMP_INSRCT_RAWDET   =  (0x01ul << 16),  	
}csi_cmp_intsrc_e;

typedef enum
{
	CMP_PHYST_POL_0mv			=	0x00,
	CMP_PHYST_POL_5mv,
	CMP_PHYST_POL_10mv,
	CMP_PHYST_POL_15mv,
	CMP_PHYST_POL_20mv,
	CMP_PHYST_POL_25mv,
	CMP_PHYST_POL_40mv,	
	CMP_PHYST_POL_60mv				
}csi_phystpol_e;

typedef enum
{
	CMP_PHYST_POL_DIS			=	0x00,
	CMP_PHYST_POL_P,
	CMP_PHYST_POL_N,
	CMP_PHYST_POL_NP
}csi_phystpol_sel_e;

typedef enum
{
    CMP_POL_OUT_DIRECT          = 0x00,
	CMP_POL_OUT_UNDIRECT 
}csi_polarity_e;

typedef enum
{
	CMP_CPOS_OUT_DIRECT			=	0x00,
	CMP_CPOS_OUT_IN				=	0x01
}csi_cr_cpos_e;

typedef enum{
	CMP_TRGOUT					= 0,		
}csi_cmp_trgout_e;

typedef enum
{
	CMP_EVE_DOWN		    	=	0x00,
	CMP_EVE_UP,
	CMP_EVE_DOWN_UP,
	CMP_EVE_UP1
}csi_cmp_trgsrc_e;

typedef enum
{
	CMP_DFCR_DEPTH1_8        =   0,
	CMP_DFCR_DEPTH1_16       =   1,
	CMP_DFCR_DEPTH1_32       =   2,
	CMP_DFCR_DEPTH1_64       =   3,
	CMP_DFCR_DEPTH1_128      =   4,
	CMP_DFCR_DEPTH1_256      =   5,
	CMP_DFCR_DEPTH1_512      =   6,
	CMP_DFCR_DEPTH1_1024     =   7
			
}csi_dfcr1_depth_e;

typedef enum
{
	CMP_DFCR_DEPTH2_16       =   0x00,
	CMP_DFCR_DEPTH2_32,
	CMP_DFCR_DEPTH2_64,
	CMP_DFCR_DEPTH2_128,
	CMP_DFCR_DEPTH2_256,
	CMP_DFCR_DEPTH2_512,
	CMP_DFCR_DEPTH2_1024,
	CMP_DFCR_DEPTH2_2048
			
}csi_dfcr2_depth_e;


typedef enum
{
	CMP_WFCR_ALIGN_DISALLOW   = 0x00,
	CMP_WFCR_ALIGN_ALLOW      = 0x01
}csi_wfcr_align_e;

typedef enum
{
	CMP_WFCR_OSET_DIS			=	0x00,
	CMP_WFCR_OSET_HIGH,
	CMP_WFCR_OSET_LOW,
	CMP_WFCR_OSET_DIS1
}csi_wfcr_oset_e;

typedef enum
{
	CMP_N_SEL_CP0        =  0x00,
	CMP_N_SEL_CP1,
	CMP_N_SEL_CP2,
	CMP_N_SEL_CP3,	
	CMP_N_SEL_CP4,
	CMP_N_SEL_CP5,
	CMP_N_SEL_1VBUF      =  0x07     
}csi_nsel_e;

typedef enum
{
	CMP_P_SEL_CP0       = 0x00,
	CMP_P_SEL_CP1,
	CMP_P_SEL_CP2,
	CMP_P_SEL_CP3,	
	CMP_P_SEL_CP4,
	CMP_P_SEL_CP5,
	CMP_P_SEL_CP6,
	CMP_P_SEL_CP7
}csi_psel_e;


typedef struct
{
	uint8_t  byNsel;                  //N- pin
	uint8_t  byPsel;	             //P+ pin
	uint8_t  byPhystpol;		     
	uint8_t  byPhystsel;		
	uint8_t  byPolarity;		
	uint8_t  byCpoSel;
	uint32_t wInt;
	
}csi_cmp_config_t;

typedef struct
{
	uint8_t byDepth1;
	uint8_t byDivn1;
	uint8_t byDivm1;	
	uint8_t byRev;
}csi_cmp_dflt1_config_t;

typedef struct
{	
	uint8_t byDepth2;
	uint8_t byDivn2;
	uint8_t byDivm2;
	uint8_t byRev;	
}csi_cmp_dflt2_config_t;

typedef struct
{
	uint8_t  byWfalign;
	uint8_t  byWoset;
	uint8_t  byClkDiv;	
	uint8_t  byDcnt;
	uint16_t hwWcnt;
	
}csi_cmp_wfcr_config_t;


/**
 *  \brief       Enable cmp power manage
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   eIntSrc:cmp interrupt source
 *  \param[in]   bEnable:cmp irq enable or disable
 */
void csi_cmp_int_enable(csp_cmp_t *ptCmpBase, csi_cmp_intsrc_e eIntSrc,bool bEnable);

/**
 *  \brief       init cmp
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   ptCmpCfg:pointer of cmp parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_init(csp_cmp_t *ptCmpBase,csi_cmp_config_t *ptCmpCfg);

/**
 *  \brief       start cmp
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \return none
 */
void csi_cmp_start(csp_cmp_t *ptCmpBase);

/** \brief stop cmp
 * 
 *  \param[in] ptBtBase: pointer of cmp register structure
 *  \return none
 */ 
void csi_cmp_stop(csp_cmp_t *ptCmpBase);

/** \brief cmp dflt1 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt1 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt1_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt1_config_t *ptCmpDfltCfg);

/** \brief cmp dflt2 config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] bEnable: dflt2 enable or disable
 *  \param[in] ptCmpDfltCfg: pointer of cmp dlft config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_dflt2_config(csp_cmp_t *ptCmpBase,bool bEnable,csi_cmp_dflt2_config_t *ptCmpDfltCfg);

/**
 *  \brief       cmp wflt config
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   ptCmpWfcrCfg:pointer of cmp wflt config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_wfcr_config(csp_cmp_t *ptCmpBase,csi_cmp_wfcr_config_t *ptCmpWfcrCfg);

/** \brief cmp evtrg output config
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eTrgOut: adc evtrg out port (0)
 *  \param[in] eTrgSrc: cmp evtrg source(0~3) 
 *  \param[in] bEnable: cmp evtrg enable or disable
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_set_evtrg(csp_cmp_t *ptCmpBase, csi_cmp_trgout_e eTrgOut, csi_cmp_trgsrc_e eTrgSrc);

/** \brief cmp evtrg output enable/disable
 * 
 *  \param[in] ptCmpBase: pointer of cmp register structure
 *  \param[in] eTrgOut: cmp evtrg out port (0)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cmp_evtrg_enable(csp_cmp_t *ptCmpBase, csi_cmp_trgout_e eTrgOut, bool bEnable);

/** \brief cmp out status
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \param[in] byOutCh: cmp out ch sel(cmp0~cmp1)
 *  \return out status(0~1)
 */
uint8_t csi_cmp_get_out(csp_cmp_t *ptCmpBase,uint8_t byOutCh);

/** \brief clear cmp interrupt status 
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \param[in] eIntMode:EDGEDET_MODE or RAWDET_MODE
 *  \return none
 */
void csi_cmp_clr_isr(csp_cmp_t *ptCmpBase,csi_cmp_intsrc_e eIntMode);

/** \brief get cmp interrupt status
 * 
 *  \param[in] ptCmpBase:pointer of cmp register structure
 *  \return cmp int status
 */
uint32_t csi_cmp_get_isr(csp_cmp_t *ptCmpBase);

/**
 *  \brief       Enable cmp power manage
 *  \param[in]   ptCmpBase:pointer of cmp register structure
 *  \param[in]   bEnable:cmp lpwken enable or disable
 */
void csi_cmp_lpwken_enable(csp_cmp_t *ptCmpBase, bool bEnable);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_CMP_H_ */