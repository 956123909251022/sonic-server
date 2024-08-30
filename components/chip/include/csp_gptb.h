/*********************************************************************** 
 * \file  csp_gptb.h
 * \brief  headfile for GPTB in csp level
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-24 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _CSP_GPTB_H
#define _CSP_GPTB_H

#include "csi_core.h"
#include "stdbool.h"
#include "csp_common.h"


/// \struct csp_gptb_t
/// \brief GPTB reg description
typedef struct
{
	__IOM uint32_t CEDR;			//0x0000	ID & Clock Enable/Disable Register                                              
	__IOM uint32_t RSSR;            //0x0004    RESET/START/STOP Register                                         
	__IOM uint32_t PSCR;            //0x0008    Counter Clock Prescaler Register                                          
	__IOM uint32_t CR;              //0x000c    General Control Register                                          
	__IOM uint32_t SYNCR;           //0x0010    Synchronization Control Register                                         
    __IOM uint32_t GLDCR;           //0x0014    Global Load Control Register                                         
    __IOM uint32_t GLDCFG;          //0x0018    Global Load Configuration Register                                        
    __IOM uint32_t GLDCR2;          //0x001c                                             
    __IOM uint32_t RESERVED;
    __IOM uint32_t PRDR;            //0x0024	Period Register                                             
    __IOM uint32_t PHSR;            //0x0028                                             
    __IOM uint32_t CMPA;            //0x002c	Compare A Register	                                             
    __IOM uint32_t CMPB;            //0x0030    Compare B Register                                         
    __IOM uint32_t RESERVED1[2];
    __IOM uint32_t CMPLDR;          //0x003c    Compare Data Load Control Register                                         
    __IOM uint32_t CNT;             //0x0040    Counter Register                                          
    __IOM uint32_t AQLDR;           //0x0044	Action Qualifier Load Control Register                                             
    __IOM uint32_t AQCRA;           //0x0048    Action Qualifier Control Register A                                         
    __IOM uint32_t AQCRB;           //0x004c    Action Qualifier Control Register B                                         
    __IOM uint32_t RESERVED2[3];
    __IOM uint32_t AQOSF;           //0x005c    Action Qualifier One Shot Force Register                                         
    __IOM uint32_t AQCSF;           //0x0060    Action Qualifier Continuous Force Register                                         
    __IOM uint32_t DBLDR;           //0x0064                                             
    __IOM uint32_t DBCR;            //0x0068                                             
    __IOM uint32_t DPSCR;           //0x006c                                             
    __IOM uint32_t DBDTR;           //0x0070                                             
    __IOM uint32_t DBDTF;           //0x0074                                             
    __IOM uint32_t RESERVED3;
    __IOM uint32_t EMSRC;           //0x007c                                             
    __IOM uint32_t EMSRC2;          //0x0080                                             
    __IOM uint32_t EMPOL;           //0x0084                                             
    __IOM uint32_t EMECR;           //0x0088                                             
    __IOM uint32_t EMOSR;           //0x008c                                             
    __IOM uint32_t RESERVED4;
    __IM  uint32_t EMSLSR;          //0x0094                                             
    __OM  uint32_t EMSLCLR;         //0x0098                                             
    __IM  uint32_t EMHLSR;          //0x009c                                             
    __OM  uint32_t EMHLCLR;         //0x00a0                                             
  __IOM  uint32_t EMFRCR;                    //
    __IM  uint32_t EMRISR;          //0x00a8                                             
    __IM  uint32_t EMMISR;          //0x00ac                                             
    __IOM uint32_t EMIMCR;          //0x00b0                                             
    __IOM uint32_t EMICR;           //0x00b4                                             
    __IOM uint32_t TRGFTCR;         //0x00b8	Digital Compare Filter Control Register                                             
    __IOM uint32_t TRGFTWR;         //0x00bc    Digital Compare Filter Window Register                                         
    __IOM uint32_t EVTRG;           //0x00c0    Event Generation Control Register                                         
    __IOM uint32_t EVPS;            //0x00c4	Event Counter Prescaler                                          
    __IOM uint32_t EVCNTINIT;       //0x00c8                                             
    __IOM uint32_t EVSWF;           //0x00cc 	Event Counter Load Control Register                                            
    __IOM uint32_t RISR;            //0x00d0    Raw Interrupt Status Register                                         
    __IOM uint32_t MISR;            //0x00d4    Masked Interrupt Status Register                                         
    __IOM uint32_t IMCR;            //0x00d8    Interrupt Masking Control Register                                         
    __IOM uint32_t ICR;             //0x00dc    Interrupt Clear Register                                         
    __IOM uint32_t REGLK;           //0x00e0                                             
    __IOM uint32_t REGLK2;          //0x00e4                                             
    __IOM uint32_t REGPROT;         //0x00e8   
    __IM uint32_t	RESERVED6[467-3];
	__IM uint32_t 	CMPAA;	        //0x082C	cmpa active reg for capture mode
    __IM uint32_t 	CMPBA;	        //0x0830	cmpb active reg for capture mode
} csp_gptb_t;

typedef enum 
{	GPTB_CHa	= 0,
	GPTB_CHb,
}csp_gptb_chtype_e;


///CEDR
#define GPTB_CLKEN           (1ul)
#define GPTB_DBGEN_POS		(1)
#define GPTB_DBGEN           (1ul << 1)

#define GPTB_CKSRC_POS		(3)
#define GPTB_CKSRC_MSK		(1ul << GPTB_CKSRC_POS)
#define GPTB_CKSRC_EXT       (1ul << GPTB_CKSRC_POS)
#define GPTB_CKSRC_PCLK		(0ul << GPTB_CKSRC_POS)

#define GPTB_TIN_POS			(4)
#define GPTB_TIN_MSK			(3ul << GPTB_TIN_POS)
#define GPTB_TIN_DIS			(0ul << GPTB_TIN_POS)
#define GPTB_TIN_BT0         (1ul << GPTB_TIN_POS)
#define GPTB_TIN_BT1         (2ul << GPTB_TIN_POS)

#define GPTB_START_POS		(6)
#define GPTB_START_MSK		(0x1ul << GPTB_START_POS)
#define GPTB_START_SHD       (0ul << GPTB_START_POS)
#define GPTB_START_IMMD      (1ul << GPTB_START_POS)
#define GPTB_FLTPRS_POS		(8)
#define GPTB_FLTPRS_MSK		(0xfful << GPTB_FLTPRS_POS)

///RSSR
#define GPTB_START		(0x1ul)
#define GPTB_RESET_POS	(12)
#define GPTB_RESET_MSK	(0xf << GPTB_RESET_POS)
#define GPTB_RESET		(0x5ul << GPTB_RESET_POS)


///CR
#define GPTB_CNTMD_POS  (0)
#define GPTB_CNTMD_MSK	(0x3ul << GPTB_CNTMD_POS)
typedef enum{
	GPTB_CNTMD_UP = 0,
	GPTB_CNTMD_DN,
	GPTB_CNTMD_UPDN
}csp_gptb_cntmd_e;


#define GPTB_STARTSRC_POS	(2)
#define GPTB_STARTSRC_MSK (0x1ul << GPTB_STARTSRC_POS)
typedef enum{
	GPTB_SW_START = 0,
	GPTB_SYNC_START
}csp_gptb_startsrc_e;

#define GPTB_STPST_POS	(3)
#define GPTB_STPST_MSK (0x1 << GPTB_STPST_POS)
typedef enum{
	GPTB_STPST_HZ = 0,
	GPTB_STPST_LOW
}csp_gptb_stpst_e;


#define GPTB_PRDLD_POS			(4)
#define GPTB_PRDLD_MSK			(0x3ul << GPTB_PRDLD_POS)
typedef enum {
	GPTB_LDPRDR_ZRO = 0,
	GPTB_LDPRDR_ZRO_SYNC,
	GPTB_LDPRDR_SYNC,
	GPTB_LDPRDR_IMMD
}csp_gptb_ldprdr_e;

#define GPTB_OPMD_POS	(6)
#define GPTB_OPMD_MSK	(0x1 << GPTB_OPMD_POS)
typedef enum{
	GPTB_OPMD_CONT = 0,
	GPTB_OPMD_OT,
}csp_gptb_opmd_e;




#define GPTB_INIT_AS_PHSEN	(0x0 << 7)
#define GPTB_INIT_DFLT		(0x1 << 7)

#define GPTB_CAPLD_EN   (1ul << 8)
#define GPTB_CAPLD_POS	(8)
#define GPTB_CAPLD_MSK	(0x1 << GPTB_CAPLD_POS)


#define GPTB_BURST_POS      (9)
#define GPTB_BURST_MSK      (1ul << GPTB_BURST_POS)

#define GPTB_FLT_INIT       (1ul << 10)
#define GPTB_FLT_INIT_POS      (10)
#define GPTB_FLT_INIT_MSK      (1ul << GPTB_FLT_INIT_POS)

#define GPTB_CGSRC_POS	(11)
#define GPTB_CGSRC_MSK	(0x3 << GPTB_CGSRC_POS)
typedef enum {
	GPTB_CGSRC_TIOA = 0,
	GPTB_CGSRC_TIOB,
//	GPTB_CGSRC_TIN
}csp_gptb_cgsrc_e;

#define GPTB_CGFLT_POS	(13)
#define GPTB_CGFLT_MSK	(0x7 << GPTB_CGFLT_POS)
typedef enum {
    GPTB_CGFLT_BP = 0,
	GPTB_CGFLT_2,
	GPTB_CGFLT_3,
	GPTB_CGFLT_4,
	GPTB_CGFLT_6,
	GPTB_CGFLT_8,
	GPTB_CGFLT_16,
	GPTB_CGFLT_32
}csp_gptb_cnflt_e;

#define GPTB_PSCLD_POS	(16)
#define GPTB_PSCLD_MSK	(0x3 << GPTB_PSCLD_POS)
typedef enum{
	GPTB_LDPSCR_ZRO = 0,
	GPTB_LDPSCR_PRD,
	GPTB_LDPSCR_ZROPRD
}csp_gptb_ldpscr_e;

#define GPTB_MODE_POS	(18)
#define GPTB_MODE_MSK     (1ul << GPTB_MODE_POS)
typedef enum{
	GPTB_MODE_CAP = 0,
	GPTB_MODE_OUT
}csp_gptb_md_e;


#define GPTB_CAPREARM   (1ul << 19)

#define GPTB_CAPMD_POS    (20)
#define GPTB_CAPMD_MSK	(0x1 << GPTB_CAPMD_POS)
typedef enum{
	GPTB_CAPMD_CONT = 0,
	GPTB_CAPMD_OT,
}csp_gptb_capmd_e;

#define GPTB_STOPWRAP_POS	(21)
#define GPTB_STOPWRAP_MSK	(0x3 << GPTB_STOPWRAP_POS)

#define GPTB_CMPA_RST_POS	(23)
#define GPTB_CMPA_RST_MSK	(0x1 << GPTB_CMPA_RST_POS) 	
#define GPTB_CMPB_RST_POS	(24)
#define GPTB_CMPB_RST_MSK	(0x1 << GPTB_CMPB_RST_POS) 	

#define GPTB_CMPAA_RST_POS	(25)
#define GPTB_CMPAA_RST_MSK	(0x1 << GPTB_CMPAA_RST_POS) 	
#define GPTB_CMPBA_RST_POS	(26)
#define GPTB_CMPBA_RST_MSK	(0x1 << GPTB_CMPBA_RST_POS) 	

#define GPTB_CMP_LDRST_POS(n) (23 + (n))
#define GPTB_CMP_LDRST_MSK(n) (0x1 << GPTB_CMP_LDRST_POS(n))	

	
///SYNCR Regiser
#define GPTB_SYNC_POS(ch)	(ch)
#define GPTB_SYNC_MSK(ch)	(1<<ch)
#define GPTB_SYNCEN(ch)		(1<<ch)

#define GPTB_OSTMD_POS(ch)	(ch+8)
#define GPTB_OSTMD_MSK(ch)	(1<<(GPTB_OSTMD_POS(ch)))

typedef enum {
	GPTB_OSTMD_CONT = 0,
	GPTB_OSTMD_OS
}csp_gptb_syncmd_e;

#define GPTB_REARM_POS(ch)	(ch+15)
#define GPTB_REARM_MSK(ch)	(1<<(ch+15))
#define GPTB_REARM(ch) 		(1<<(ch+15))

#define GPTB_TXREARM0_POS	(22)
#define GPTB_TXREARM0_MSK	(0x3 << GPTB_TXREARM0_POS)
typedef enum{
	GPTB_TXREARM0_DIS =0,
	GPTB_TX1REARM0,
	GPTB_TX2REARM0,
	GPTB_TX1_2REARM0
}csp_gptb_txrearm0_e;


#define GPTB_TRGO0SEL_POS	(24)
#define GPTB_TRGO0SEL_MSK	(0x7<<GPTB_TRGO0SEL_POS)

#define GPTB_TRGO1SEL_POS	(27)
#define GPTB_TRGO1SEL_MSK	(0x7<<GPTB_TRGO1SEL_POS)

#define GPTB_AREARM_POS		(30)
#define GPTB_AREARM_MSK		(0x3<<GPTB_AREARM_POS)
typedef enum{
	GPTB_AREARM_DIS = 0,
	GPTB_AREARM_ZRO,
	GPTB_AREARM_PRD,
	GPTB_AREARM_ZRO_PRD
}csp_gptb_arearm_e;

///GLDCR Regiser
#define GPTB_GLDEN_POS	(0)
#define GPTB_GLDEN_MSK	(0x1)
#define GPTB_GLDMD_POS	(1)
#define GPTB_GLDMD_MSK	(0xf << GPTB_GLDMD_POS)
typedef enum {
	GPTB_LDGLD_ZRO = 0,
	GPTB_LDGLD_PRD,
	GPTB_LDGLD_ZRO_PRD,
	GPTB_LDGLD_ZRO_LD_SYNC,
	GPTB_LDGLD_PRD_LD_SYNC,
	GPTB_LDGLD_ZRO_PRD_LD_SYNC,
	GPTB_LDGLD_SW = 0xf
}csp_gptb_ldgld_e;

#define GPTB_GLDCR_OSTMD_POS (5)
#define GPTB_GLDCR_OSTMD_MSK	(0x1 << GPTB_GLDCR_OSTMD_POS)


typedef enum{
	GPTB_LDMD_ANYTIME = 0,
	GPTB_LDMD_OS,
}csp_gptb_ldmd_e;

#define GPTB_GLDPRD_POS	(7)
#define GPTB_GLDPRD_MSK	(0x7 << GPTB_GLDPRD_POS)

#define GPTB_GLDCNT_POS	(10)
#define GPTB_GLDCNT_MSK	(0x7 << GPTB_GLDCNT_POS)

///CLDCFG
#define GPTB_LD_PRDR_POS (0)
#define GPTB_LD_PRDR_MSK	(0x1 << GPTB_LD_PRDR_POS)
#define GPTB_LD_CMPA_POS (1)
#define GPTB_LD_CMPA_MSK	(0x1 << GPTB_LD_CMPA_POS)
#define GPTB_LD_CMPB_POS (2)
#define GPTB_LD_CMPB_MSK	(0x1 << GPTB_LD_CMPB_POS)

#define GPTB_LD_AQCRA_POS (8)
#define GPTB_LD_AQCRA_MSK	(0x1 << GPTB_LD_AQCRA_POS)
#define GPTB_LD_AQCRB_POS (9)
#define GPTB_LD_AQCRB_MSK	(0x1 << GPTB_LD_AQCRB_POS)

#define GPTB_LD_AQCSF_POS (12)
#define GPTB_LD_AQCSF_MSK	(0x1 << GPTB_LD_AQCSF_POS)

typedef enum{
	GPTB_LD_NOTGLD = 0,
	GPTB_LD_GLD
}csp_gptb_gldmd_e;

///GLDCR2
#define GPTB_OSREARM_EN	(0x1)
#define GPTB_SW_GLD		(0x2)

///PHSR
#define GPTB_PHSR_POS	(0)
#define GPTB_PHSR_MSK	(0xffff)
#define GPTB_PHSDIR_POS	(31)
#define GPTB_PHSDIR_MSK	(0x1 << GPTB_PHSDIR_POS)
typedef enum{
	GPTB_PHSDIR_DN = 0,
	GPTB_PHSDIR_UP
}csp_gptb_phsdir_e;

///CMPA/B
#define GPTB_CMPDATA_MSK	(0xffff)	
#define GPTB_CMPDATA_OVWRT (0x1 << 31)


///CMPLDR
#define GPTB_CMP_LD_POS(n)	(n)
#define GPTB_CMP_LD_MSK(n)  (0x1 << GPTB_CMP_LD_POS(n))

#define GPTB_CMPA_LD_POS	(0)
#define GPTB_CMPA_LD_MSK    (0x1 << GPTB_CMPA_LD_POS)
#define GPTB_CMPB_LD_POS	(1)
#define GPTB_CMPB_LD_MSK    (0x1 << GPTB_CMPB_LD_POS)
typedef enum {
	GPTB_CMPLD_SHDW = 0,
	GPTB_CMPLD_IMM
}csp_gptb_cmpdata_ldmd_e;

#define GPTB_CMP_LDTIME_POS(n)	(4+ ((n)*3))
#define GPTB_CMP_LDTIME_MSK(n)	(0x7 << GPTB_CMP_LDTIME_POS(n))
#define GPTB_CMPA_LDTIME_POS	(4)
#define GPTB_CMPA_LDTIME_MSK	(0x7 << GPTB_CMPA_LDTIME_POS)
#define GPTB_CMPB_LDTIME_POS	(7)
#define GPTB_CMPB_LDTIME_MSK	(0x7 << GPTB_CMPB_LDTIME_POS)

typedef enum{
	GPTB_LDCMP_NEVER= 0,
	GPTB_LDCMP_ZRO ,
	GPTB_LDCMP_PRD,
	GPTB_LDCMP_LD_SYNC=4,
}csp_gptb_ldamd_e;

#define GPTB_SHDWAFULL 	(0x1 <<20)
#define GPTB_SHDWBFULL 	(0x1 <<21)

///AQLDR
#define GPTB_AQCR1_SHDWEN_POS	(0)
#define GPTB_AQCR1_SHDWEN_MSK	(0x1)
#define GPTB_AQCR2_SHDWEN_POS	(1)
#define GPTB_AQCR2_SHDWEN_MSK	(0x1 << GPTB_AQCR2_SHDWEN_POS)
typedef enum{
	GPTB_LD_IMM = 0,
	GPTB_LD_SHDW 
}csp_gptb_ld_e;
#define GPTB_LDAMD_POS		(2)
#define GPTB_LDAMD_MSK		(0x7 << GPTB_LDAMD_POS)
#define GPTB_LDBMD_POS		(5)
#define GPTB_LDBMD_MSK		(0x7 << GPTB_LDBMD_POS)


///AQCRA/B
#define GPTB_ACT_ZRO_POS	(0)
#define GPTB_ACT_ZRO_MSK	(0x3 << GPTB_ACT_ZRO_POS)
#define GPTB_ACT_PRD_POS	(2)
#define GPTB_ACT_PRD_MSK	(0x3 << GPTB_ACT_PRD_POS)
#define GPTB_ACT_C1U_POS	(4)
#define GPTB_ACT_C1U_MSK	(0x3 << GPTB_ACT_C1U_POS)
#define GPTB_ACT_C1D_POS	(6)
#define GPTB_ACT_C1D_MSK	(0x3 << GPTB_ACT_C1D_POS)
#define GPTB_ACT_C2U_POS	(8)
#define GPTB_ACT_C2U_MSK	(0x3 << GPTB_ACT_C2U_POS)
#define GPTB_ACT_C2D_POS	(10)
#define GPTB_ACT_C2D_MSK	(0x3 << GPTB_ACT_C2D_POS)
#define GPTB_ACT_T1U_POS	(12)
#define GPTB_ACT_T1U_MSK	(0x3 << GPTB_ACT_T1U_POS)
#define GPTB_ACT_T1D_POS	(14)
#define GPTB_ACT_T1D_MSK	(0x3 << GPTB_ACT_T1D_POS)
#define GPTB_ACT_T2U_POS	(16)
#define GPTB_ACT_T2U_MSK	(0x3 << GPTB_ACT_T2U_POS)
#define GPTB_ACT_T2D_POS	(18)
#define GPTB_ACT_T2D_MSK	(0x3 << GPTB_ACT_T2D_POS)

#define GPTB_C1SEL_POS	(20)
#define GPTB_C1SEL_MSK	(0x3<<GPTB_C1SEL_POS)
#define GPTB_C2SEL_POS	(22)
#define GPTB_C2SEL_MSK	(0x3<<GPTB_C2SEL_POS)
typedef enum {
	GPTB_CMPA= 0,
	GPTB_CMPB,
}csp_gptb_csrc_sel_e;

///AQOSF
#define GPTB_OSTSFA		(1)
#define GPTB_ACTA_POS	(1)
#define GPTB_ACTA_MSK	(0x3 << GPTB_ACTA_POS)
#define GPTB_OSTSFB		(0x1 << 4)
#define GPTB_ACTB_POS	(5)
#define GPTB_ACTB_MSK	(0x3 << GPTB_ACTB_POS)
#define GPTB_AQCSF_LDTIME_POS (16)
#define GPTB_AQCSF_LDTIME_MSK (0x3 << GPTB_AQCSF_LDTIME_POS)	
typedef enum{
	GPTB_LDAQCR_ZRO = 0,
	GPTB_LDAQCR_PRD,
	GPTB_LDAQCR_ZROPRD
}csp_gptb_ldaqcr_e;

typedef enum{
	GPTB_LDACTAB_NEVER= 0,
	GPTB_LDACTAB_LOW ,
	GPTB_LDACTAB_HIGH,
    GPTB_LDACTAB_TOGGLE	
}csp_gptb_ldosf_e;

//AQCSF
#define GPTB_CSFA_POS	(0)
#define GPTB_CSFA_MSK	(0x3)
#define GPTB_CSFB_POS	(2)
#define GPTB_CSFB_MSK	(0x3 << GPTB_CSFB_POS)

///DBLDR  
#define GPTB_DBLDR_CRSHDWEN_POS            0                                              /*!< GPTB DBLDR: CRSHDWEN Position */
#define GPTB_DBLDR_CRSHDWEN_MSK            (0x1UL << GPTB_DBLDR_CRSHDWEN_POS)             /*!< GPTB DBLDR: CRSHDWEN Mask */
#define GPTB_DBLDR_LDCRMODE_POS            1                                              /*!< GPTB DBLDR: LDCRMODE Position */
#define GPTB_DBLDR_LDCRMODE_MSK            (0x3UL << GPTB_DBLDR_LDCRMODE_POS)             /*!< GPTB DBLDR: LDCRMODE Mask */
#define GPTB_DBLDR_SHDWDTR_POS             3                                              /*!< GPTB DBLDR: SHDWDTR Position */
#define GPTB_DBLDR_SHDWDTR_MSK             (0x1UL << GPTB_DBLDR_SHDWDTR_POS)              /*!< GPTB DBLDR: SHDWDTR Mask */
#define GPTB_DBLDR_LDDTRMD_POS             4                                              /*!< GPTB DBLDR: LDDTRMD Position */
#define GPTB_DBLDR_LDDTRMD_MSK             (0x3UL << GPTB_DBLDR_LDDTRMD_POS)              /*!< GPTB DBLDR: LDDTRMD Mask */
#define GPTB_DBLDR_SHDWDTF_POS             6                                              /*!< GPTB DBLDR: SHDWDTF Position */
#define GPTB_DBLDR_SHDWDTF_MSK             (0x1UL << GPTB_DBLDR_SHDWDTF_POS)              /*!< GPTB DBLDR: SHDWDTF Mask */
#define GPTB_DBLDR_LDDTFMD_POS             7                                              /*!< GPTB DBLDR: LDDTFMD Position */
#define GPTB_DBLDR_LDDTFMD_MSK             (0x3UL << GPTB_DBLDR_LDDTFMD_POS)              /*!< GPTB DBLDR: LDDTFMD Mask */
#define GPTB_DBLDR_SHDWPSC_POS             9                                              /*!< GPTB DBLDR: SHDWPSC Position */
#define GPTB_DBLDR_SHDWPSC_MSK             (0x1UL << GPTB_DBLDR_SHDWPSC_POS)              /*!< GPTB DBLDR: SHDWPSC Mask */
#define GPTB_DBLDR_LDPSCMD_POS             10                                             /*!< GPTB DBLDR: LDPSCMD Position */
#define GPTB_DBLDR_LDPSCMD_MSK             (0x3UL << GPTB_DBLDR_LDPSCMD_POS)              /*!< GPTB DBLDR: LDPSCMD Mask */
typedef enum{
	GPTB_DBLDR_NEVER= 0,
	GPTB_DBLDR_ZRO ,
	GPTB_DBLDR_PRD,
	GPTB_DBLDR_ZRO_PRD,
}csp_gptb_dbldr_e;

typedef enum{
	GPTB_SHDW_IMMEDIATE =0,
	GPTB_SHDW_SHADOW      
}csp_gptb_shdw_e;

typedef enum{
	GPTB_LD_NEVER = 0,
	GPTB_LD_ZRO,
	GPTB_LD_PRD,
	GPTB_LD_ZRO_PRD	
}csp_gptb_lddb_e;

//DBCR
#define DB_CHA_OUTSEL_POS	(0)
#define DB_CHA_OUTSEL_MSK	(0x3) 
#define DB_CHB_OUTSEL_POS	(8)
#define DB_CHB_OUTSEL_MSK	(0x3 << DB_CHB_OUTSEL_POS) 
#define DB_CHC_OUTSEL_POS	(16)
#define DB_CHC_OUTSEL_MSK	(0x3 << DB_CHC_OUTSEL_POS) 
#define DB_CHA_POL_POS	(2)
#define DB_CHA_POL_MSK	(0x3 << DB_CHA_POL_POS)
#define DB_CHB_POL_POS	(10)
#define DB_CHB_POL_MSK	(0x3 << DB_CHB_POL_POS)
#define DB_CHC_POL_POS	(18)
#define DB_CHC_POL_MSK	(0x3 << DB_CHC_POL_POS)

typedef enum {
	B_NA = 0,
	B_LO,
	B_HI,
	B_TG	
}csp_gptb_action_e;

#define GPTB_DBCR_CHA_OUTSEL_POS           0                                              /*!< GPTB DBCR: CHA_OUTSEL Position */
#define GPTB_DBCR_CHA_OUTSEL_MSK           (0x3UL << GPTB_DBCR_CHA_OUTSEL_POS)            /*!< GPTB DBCR: CHA_OUTSEL Mask */
typedef enum {
	B_DBOUT_DIS = 0,
	B_DBOUT_BF,
	B_DBOUT_AR,
	B_DBOUT_AR_BF
}csp_gptb_db_outsel_e;

#define GPTB_DBCR_CHA_POLARITY_POS         2                                              /*!< GPTB DBCR: CHA_POLARITY Position */
#define GPTB_DBCR_CHA_POLARITY_MSK         (0x3UL << GPTB_DBCR_CHA_POLARITY_POS)          /*!< GPTB DBCR: CHA_POLARITY Mask */
typedef enum {
	B_DB_POL_DIS = 0,
	B_DB_POL_A,
	B_DB_POL_B,
	B_DB_POL_AB
}csp_gptb_db_pol_e;

#define GPTB_DBCR_CHA_INSEL_POS            4                                              /*!< GPTB DBCR: CHA_INSEL Position */
#define GPTB_DBCR_CHA_INSEL_MSK            (0x3UL << GPTB_DBCR_CHA_INSEL_POS)             /*!< GPTB DBCR: CHA_INSEL Mask */
typedef enum {
	B_DBCHAIN_AR_AF = 0,
	B_DBCHAIN_BR_AF,
	B_DBCHAIN_AR_BF,
	B_DBCHAIN_BR_BF
}csp_gptb_dbcha_insel_e;
#define DB_CHA_INSEL_POS	(4)
#define DB_CHA_INSEL_MSK	(0x3 << DB_CHA_INSEL_POS)

#define GPTB_DBCR_CHA_OUTSWAP_POS          6                                              /*!< GPTB DBCR: CHA_OUTSWAP Position */
#define GPTB_DBCR_CHA_OUTSWAP_MSK          (0x3UL << GPTB_DBCR_CHA_OUTSWAP_POS)           /*!< GPTB DBCR: CHA_OUTSWAP Mask */                                           /*!< GPTB DBCR: CHA_OUTSWAP Position */
typedef enum {
	B_DBCHAOUT_OUTA_A_OUTB_B = 0,
	B_DBCHAOUT_OUTA_B_OUTB_B,
	B_DBCHAOUT_OUTA_A_OUTB_A,
	B_DBCHAOUT_OUTA_B_OUTB_A
}csp_gptb_dbcha_outswap_e;

#define DB_CHA_OUTSWAP_POS	(6)
#define DB_CHA_OUTSWAP_MSK	(0x3 << DB_CHA_OUTSWAP_POS)

#define DB_CHB_OUTSWAP_POS	(14)
#define DB_CHB_OUTSWAP_MSK	(0x3 << DB_CHB_OUTSWAP_POS)

#define DB_CHC_OUTSWAP_POS	(22)
#define DB_CHC_OUTSWAP_MSK	(0x3 << DB_CHC_OUTSWAP_POS)


#define DB_CHB_INSEL_POS	(12)
#define DB_CHB_INSEL_MSK	(0x3 << DB_CHB_INSEL_POS)

#define GPTB_DCKSEL_POS               24                                             /*!< GPTB DBCR: DCKSEL Position */
#define GPTB_DCKSEL_MSK               (0x1UL << GPTB_DCKSEL_POS)                /*!< GPTB DBCR: DCKSEL Mask */
typedef enum {
	GPTB_DB_TCLK = 0,
	GPTB_DB_DPSC
}csp_gptb_dbclksrc_e;


#define GPTB_CHA_DEDB_POS	(25)
#define GPTB_CHA_DEDB_MSK	(0x1 << GPTB_CHA_DEDB_POS)

#define GPTB_DBCR_CHA_EDEB_POS             25                                             /*!< GPTB DBCR: CHA_EDEB Position */
#define GPTB_DBCR_CHA_EDEB_MSK             (0x1UL << GPTB_DBCR_CHA_EDEB_POS)              /*!< GPTB DBCR: CHA_EDEB Mask */

typedef enum {
	B_DB_AR_BF = 0,
	B_DB_BR_BF
}csp_gptb_dedb_e;


//DPSCR
#define GPTB_DPSCR_DPSC_POS                0                                              /*!< GPTB DPSCR: DPSC Position */
#define GPTB_DPSCR_DPSC_MSK                (0xFFFFUL << GPTB_DPSCR_DPSC_POS)              /*!< GPTB DPSCR: DPSC Mask */

//DBDTR
#define GPTB_DBDTR_DTR_POS                 0                                              /*!< GPTB DBDTR: DTR Position */
#define GPTB_DBDTR_DTR_MSK                 (0xFFFFUL << GPTB_DBDTR_DTR_POS)               /*!< GPTB DBDTR: DTR Mask */

//DBDTF
#define GPTB_DBDTF_DTF_POS                 0                                              /*!< GPTB DBDTF: DTF Position */
#define GPTB_DBDTF_DTF_MSK                 (0xFFFFUL << GPTB_DBDTF_DTF_POS)               /*!< GPTB DBDTF: DTF Mask */

//EMSRC

#define GPTB_EMSRC_EP3_SEL_POS             12                                             /*!< GPTB EMSRC: EP3_SEL Position */
#define GPTB_EMSRC_EP3_SEL_MSK             (0xFUL << GPTB_EMSRC_EP3_SEL_POS)              /*!< GPTB EMSRC: EP3_SEL Mask */
#define GPTB_EMSRC_EP2_SEL_POS             8                                              /*!< GPTB EMSRC: EP2_SEL Position */
#define GPTB_EMSRC_EP2_SEL_MSK             (0xFUL << GPTB_EMSRC_EP2_SEL_POS)              /*!< GPTB EMSRC: EP2_SEL Mask */
#define GPTB_EMSRC_EP1_SEL_POS             4                                              /*!< GPTB EMSRC: EP1_SEL Position */
#define GPTB_EMSRC_EP1_SEL_MSK             (0xFUL << GPTB_EMSRC_EP1_SEL_POS)              /*!< GPTB EMSRC: EP1_SEL Mask */
#define GPTB_EMSRC_EP0_SEL_POS             0                                              /*!< GPTB EMSRC: EP0_SEL Position */
#define GPTB_EMSRC_EP0_SEL_MSK             (0xFUL << GPTB_EMSRC_EP0_SEL_POS)              /*!< GPTB EMSRC: EP0_SEL Mask */

///EMSRC
#define GPTB_SEL_POS_EP(n)	((n) << 2)
#define GPTB_SEL_MSK_EP(n)	(0xf << GPTB_SEL_POS_EP(n))
typedef enum{
	B_EP0 = 0,
	B_EP1,
	B_EP2,
	B_EP3,
}csp_gptb_ep_e;
typedef enum {
	B_EBI0 = 1,
	B_EBI1,
	B_EBI2,
	B_EBI3,
	B_EBI4,
	B_ORL0 = 0xe,
	B_ORL1,
}csp_gptb_ebi_e;

#define B_ORLx_EP0  1
#define B_ORLx_EP1  1<<1
#define	B_ORLx_EP2  1<<2
#define	B_ORLx_EP3  1<<3

//EMSRC2
#define GPTB_EMSRC2_ORL1_EP3_POS          19                                             /*!< GPTB EMSRC2: ORL1_EP3 Position */
#define GPTB_EMSRC2_ORL1_EP3_MSK          (0x1UL << GPTB_EMSRC2_ORL1_EP3_POS)           /*!< GPTB EMSRC2: ORL1_EP3 Mask */
#define GPTB_EMSRC2_ORL1_EP2_POS          18                                             /*!< GPTB EMSRC2: ORL1_EP2 Position */
#define GPTB_EMSRC2_ORL1_EP2_MSK          (0x1UL << GPTB_EMSRC2_ORL1_EP2_POS)           /*!< GPTB EMSRC2: ORL1_EP2 Mask */
#define GPTB_EMSRC2_ORL1_EP1_POS          17                                             /*!< GPTB EMSRC2: ORL1_EP1 Position */
#define GPTB_EMSRC2_ORL1_EP1_MSK          (0x1UL << GPTB_EMSRC2_ORL1_EP1_POS)           /*!< GPTB EMSRC2: ORL1_EP1 Mask */
#define GPTB_EMSRC2_ORL1_EP0_POS          16                                             /*!< GPTB EMSRC2: ORL1_EP0 Position */
#define GPTB_EMSRC2_ORL1_EP0_MSK          (0x1UL << GPTB_EMSRC2_ORL1_EP0_POS)           /*!< GPTB EMSRC2: ORL1_EP0 Mask */
#define GPTB_EMSRC2_FLT_PACE0_POS          8                                              /*!< GPTB EMSRC2: FLT_PACE0 Position */
#define GPTB_EMSRC2_FLT_PACE0_MSK          (0xFUL << GPTB_EMSRC2_FLT_PACE0_POS)           /*!< GPTB EMSRC2: FLT_PACE0 Mask */

#define GPTB_EMSRC2_ORL0_EP3_POS          3                                              /*!< GPTB EMSRC2: ORL0_EP3 Position */
#define GPTB_EMSRC2_ORL0_EP3_MSK          (0x1UL << GPTB_EMSRC2_ORL0_EP3_POS)           /*!< GPTB EMSRC2: ORL0_EP3 Mask */
#define GPTB_EMSRC2_ORL0_EP2_POS          2                                              /*!< GPTB EMSRC2: ORL0_EP2 Position */
#define GPTB_EMSRC2_ORL0_EP2_MSK          (0x1UL << GPTB_EMSRC2_ORL0_EP2_POS)           /*!< GPTB EMSRC2: ORL0_EP2 Mask */
#define GPTB_EMSRC2_ORL0_EP1_POS          1                                              /*!< GPTB EMSRC2: ORL0_EP1 Position */
#define GPTB_EMSRC2_ORL0_EP1_MSK          (0x1UL << GPTB_EMSRC2_ORL0_EP1_POS)           /*!< GPTB EMSRC2: ORL0_EP1 Mask */
#define GPTB_EMSRC2_ORL0_EP0_POS          0                                              /*!< GPTB EMSRC2: ORL0_EP0 Position */
#define GPTB_EMSRC2_ORL0_EP0_MSK          (0x1UL << GPTB_EMSRC2_ORL0_EP0_POS)           /*!< GPTB EMSRC2: ORL0_EP0 Mask */

///EMSRC2 config ORL0, ORL1 and input Filters
#define GPTB_ORL0_POS	(0)
#define GPTB_ORL0_MSK	(0xff)
#define GPTB_ORL1_POS	(16)
#define GPTB_ORL1_MSK	(0xff << GPTB_ORL1_POS)
#define GPTB_ORL0_NO_EPI(n)		(0x1<< n)
#define GPTB_ORL1_NO_EPI(n)		(0x1<<(n+16))
#define GPTB_EPPACE0_POS	(8)
#define GPTB_EPPACE0_MSK	(0xf << GPTB_EPPACE0_POS)
#define GPTB_EPPACE1_POS	(12)
#define GPTB_EPPACE1_MSK	(0xf << GPTB_EPPACE1_POS)

#define POL_POS_EBI(n)	(n)
#define POL_MSK_EBI(n)	(0x1 << POL_POS_EBI(n))


//_EMPOL

#define GPTB_EMPOL_EBI4_POL_POS            4                                              /*!< GPTB EMPOL: EPI4_POL Position */
#define GPTB_EMPOL_EBI4_POL_MSK            (0x1UL << GPTB_EMPOL_EBI4_POL_POS)             /*!< GPTB EMPOL: EPI4_POL Mask */
#define GPTB_EMPOL_EBI3_POL_POS            3                                              /*!< GPTB EMPOL: EPI3_POL Position */
#define GPTB_EMPOL_EBI3_POL_MSK            (0x1UL << GPTB_EMPOL_EBI3_POL_POS)             /*!< GPTB EMPOL: EPI3_POL Mask */
#define GPTB_EMPOL_EBI2_POL_POS            2                                              /*!< GPTB EMPOL: EPI2_POL Position */
#define GPTB_EMPOL_EBI2_POL_MSK            (0x1UL << GPTB_EMPOL_EBI2_POL_POS)             /*!< GPTB EMPOL: EPI2_POL Mask */
#define GPTB_EMPOL_EBI1_POL_POS            1                                              /*!< GPTB EMPOL: EPI1_POL Position */
#define GPTB_EMPOL_EBI1_POL_MSK            (0x1UL << GPTB_EMPOL_EBI1_POL_POS)             /*!< GPTB EMPOL: EPI1_POL Mask */
#define GPTB_EMPOL_EBI0_POL_POS            0                                              /*!< GPTB EMPOL: EPI0_POL Position */
#define GPTB_EMPOL_EBI0_POL_MSK            (0x1UL << GPTB_EMPOL_EBI0_POL_POS)             /*!< GPTB EMPOL: EPI0_POL Mask */

typedef enum {
	B_EBI_POL_H = 0,
	B_EBI_POL_L
}csp_gptb_ebipol_e;

//EMECR
#define GPTB_EMECR_MEM_FAULT_POS           30                                             /*!< GPTB EMECR: MEM_FAULT Position */
#define GPTB_EMECR_MEM_FAULT_MSK           (0x1UL << GPTB_EMECR_MEM_FAULT_POS)            /*!< GPTB EMECR: MEM_FAULT Mask */
#define GPTB_EMECR_EOM_FAULT_POS           29                                             /*!< GPTB EMECR: EOM_FAULT Position */
#define GPTB_EMECR_EOM_FAULT_MSK           (0x1UL << GPTB_EMECR_EOM_FAULT_POS)            /*!< GPTB EMECR: EOM_FAULT Mask */
#define GPTB_EMECR_CPU_FAULT_POS           28                                             /*!< GPTB EMECR: CPU_FAULT Position */
#define GPTB_EMECR_CPU_FAULT_MSK           (0x1UL << GPTB_EMECR_CPU_FAULT_POS)            /*!< GPTB EMECR: CPU_FAULT Mask */
#define GPTB_EMECR_EMASYNC_POS             26                                             /*!< GPTB EMECR: EMASYNC Position */
#define GPTB_EMECR_EMASYNC_MSK             (0x1UL << GPTB_EMECR_EMASYNC_POS)              /*!< GPTB EMECR: EMASYNC Mask */
#define GPTB_EMECR_SLCLRMD_POS             24                                             /*!< GPTB EMECR: SLCLRMD Position */
#define GPTB_EMECR_SLCLRMD_MSK             (0x3UL << GPTB_EMECR_SLCLRMD_POS)              /*!< GPTB EMECR: SLCLRMD Mask */
#define GPTB_EMECR_OSRLDMD_POS             22                                             /*!< GPTB EMECR: OSRLDMD Position */
#define GPTB_EMECR_OSRLDMD_MSK             (0x3UL << GPTB_EMECR_OSRLDMD_POS)              /*!< GPTB EMECR: OSRLDMD Mask */
#define GPTB_EMECR_OSRSHDW_POS             21                                             /*!< GPTB EMECR: OSRSHDW Position */
#define GPTB_EMECR_OSRSHDW_MSK             (0x1UL << GPTB_EMECR_OSRSHDW_POS)              /*!< GPTB EMECR: OSRSHDW Mask */
#define GPTB_EMECR_EP3_LCKMD_POS           6                                              /*!< GPTB EMECR: EP3_LCKMD Position */
#define GPTB_EMECR_EP3_LCKMD_MSK           (0x3UL << GPTB_EMECR_EP3_LCKMD_POS)            /*!< GPTB EMECR: EP3_LCKMD Mask */
#define GPTB_EMECR_EP2_LCKMD_POS           4                                              /*!< GPTB EMECR: EP2_LCKMD Position */
#define GPTB_EMECR_EP2_LCKMD_MSK           (0x3UL << GPTB_EMECR_EP2_LCKMD_POS)            /*!< GPTB EMECR: EP2_LCKMD Mask */
#define GPTB_EMECR_EP1_LCKMD_POS           2                                              /*!< GPTB EMECR: EP1_LCKMD Position */
#define GPTB_EMECR_EP1_LCKMD_MSK           (0x3UL << GPTB_EMECR_EP1_LCKMD_POS)            /*!< GPTB EMECR: EP1_LCKMD Mask */
#define GPTB_EMECR_EP0_LCKMD_POS           0                                              /*!< GPTB EMECR: EP0_LCKMD Position */
#define GPTB_EMECR_EP0_LCKMD_MSK           (0x3UL << GPTB_EMECR_EP0_LCKMD_POS)            /*!< GPTB EMECR: EP0_LCKMD Mask */

#define GPTB_LCKMD_POS_EP(n)	((n) << 1)
#define GPTB_LCKMD_MSK_EP(n)	(0x3 << GPTB_LCKMD_POS_EP(n))


typedef enum {
	B_EM_INT_EP0 = 0x1,
	B_EM_INT_EP1 = 0x1 << 1,
	B_EM_INT_EP2 = 0x1 << 2,
	B_EM_INT_EP3 = 0x1 << 3,
	B_EM_INT_CPUF= 0x1 << 8,
	B_EM_INT_MEMF= 0x1 << 9,
	B_EM_INT_EOMF= 0x1 << 10
}csp_gptb_emint_e;


typedef enum{
	B_EPFLT0_DIS = 0,
	B_EPFLT0_2P,
	B_EPFLT0_3P,
	B_EPFLT0_4P
}csp_gptb_epflt0_e;
typedef enum{
	B_EPFLT1_1P = 0,
	B_EPFLT1_2P,
	B_EPFLT1_3P,
	B_EPFLT1_4P
}csp_gptb_epflt1_e;

typedef enum{
	B_EP_DIS = 0,
	B_EP_SLCK,
	B_EP_HLCK,
	B_EP_DISABLE
}csp_gptb_ep_lckmd_e;

typedef enum{
             B_IMMEDIATE=0,
			 B_SHADOW
}csp_gptb_Osrshdw_e;



//EMOSR
#define GPTB_EMOSR_EM_COAY_POS             8                                              /*!< GPTB EMOSR: EM_COAY Position */
#define GPTB_EMOSR_EM_COAY_MSK             (0x3UL << GPTB_EMOSR_EM_COAY_POS)              /*!< GPTB EMOSR: EM_COAY Mask */
#define GPTB_EMOSR_EM_COBX_POS             2                                              /*!< GPTB EMOSR: EM_COBX Position */
#define GPTB_EMOSR_EM_COBX_MSK             (0x3UL << GPTB_EMOSR_EM_COBX_POS)              /*!< GPTB EMOSR: EM_COBX Mask */
#define GPTB_EMOSR_EM_COAX_POS             0                                              /*!< GPTB EMOSR: EM_COAX Position */
#define GPTB_EMOSR_EM_COAX_MSK             (0x3UL << GPTB_EMOSR_EM_COAX_POS)              /*!< GPTB EMOSR: EM_COAX Mask */

typedef enum {
	B_EM_OUT_HZ,
	B_EM_OUT_H,
	B_EM_OUT_L,
	B_EM_OUT_NONE
}csp_gptb_emout_e;

//EMSLSR
#define GPTB_EMSLSR_EP3_POS                3                                              /*!< GPTB EMSLSR: EP3 Position */
#define GPTB_EMSLSR_EP3_MSK                (0x1UL << GPTB_EMSLSR_EP3_POS)                 /*!< GPTB EMSLSR: EP3 Mask */
#define GPTB_EMSLSR_EP2_POS                2                                              /*!< GPTB EMSLSR: EP2 Position */
#define GPTB_EMSLSR_EP2_MSK                (0x1UL << GPTB_EMSLSR_EP2_POS)                 /*!< GPTB EMSLSR: EP2 Mask */
#define GPTB_EMSLSR_EP1_POS                1                                              /*!< GPTB EMSLSR: EP1 Position */
#define GPTB_EMSLSR_EP1_MSK                (0x1UL << GPTB_EMSLSR_EP1_POS)                 /*!< GPTB EMSLSR: EP1 Mask */
#define GPTB_EMSLSR_EP0_POS                0                                              /*!< GPTB EMSLSR: EP0 Position */
#define GPTB_EMSLSR_EP0_MSK                (0x1UL << GPTB_EMSLSR_EP0_POS)                 /*!< GPTB EMSLSR: EP0 Mask */
//EMSLCLR
#define GPTB_EMSLCLR_EP3_POS               3                                              /*!< GPTB EMSLCLR: EP3 Position */
#define GPTB_EMSLCLR_EP3_MSK               (0x1UL << GPTB_EMSLCLR_EP3_POS)                /*!< GPTB EMSLCLR: EP3 Mask */
#define GPTB_EMSLCLR_EP2_POS               2                                              /*!< GPTB EMSLCLR: EP2 Position */
#define GPTB_EMSLCLR_EP2_MSK               (0x1UL << GPTB_EMSLCLR_EP2_POS)                /*!< GPTB EMSLCLR: EP2 Mask */
#define GPTB_EMSLCLR_EP1_POS               1                                              /*!< GPTB EMSLCLR: EP1 Position */
#define GPTB_EMSLCLR_EP1_MSK               (0x1UL << GPTB_EMSLCLR_EP1_POS)                /*!< GPTB EMSLCLR: EP1 Mask */
#define GPTB_EMSLCLR_EP0_POS               0                                              /*!< GPTB EMSLCLR: EP0 Position */
#define GPTB_EMSLCLR_EP0_MSK               (0x1UL << GPTB_EMSLCLR_EP0_POS)                /*!< GPTB EMSLCLR: EP0 Mask */
//EMHLSR
#define GPTB_EMHLSR_EOM_FAULT_POS          10                                             /*!< GPTB EMHLSR: EOM_FAULT Position */
#define GPTB_EMHLSR_EOM_FAULT_MSK          (0x1UL << GPTB_EMHLSR_EOM_FAULT_POS)           /*!< GPTB EMHLSR: EOM_FAULT Mask */
#define GPTB_EMHLSR_MEM_FAULT_POS          9                                              /*!< GPTB EMHLSR: MEM_FAULT Position */
#define GPTB_EMHLSR_MEM_FAULT_MSK          (0x1UL << GPTB_EMHLSR_MEM_FAULT_POS)           /*!< GPTB EMHLSR: MEM_FAULT Mask */
#define GPTB_EMHLSR_CPU_FAULT_POS          8                                              /*!< GPTB EMHLSR: CPU_FAULT Position */
#define GPTB_EMHLSR_CPU_FAULT_MSK          (0x1UL << GPTB_EMHLSR_CPU_FAULT_POS)           /*!< GPTB EMHLSR: CPU_FAULT Mask */
#define GPTB_EMHLSR_EP3_POS                3                                              /*!< GPTB EMHLSR: EP3 Position */
#define GPTB_EMHLSR_EP3_MSK                (0x1UL << GPTB_EMHLSR_EP3_POS)                 /*!< GPTB EMHLSR: EP3 Mask */
#define GPTB_EMHLSR_EP2_POS                2                                              /*!< GPTB EMHLSR: EP2 Position */
#define GPTB_EMHLSR_EP2_MSK                (0x1UL << GPTB_EMHLSR_EP2_POS)                 /*!< GPTB EMHLSR: EP2 Mask */
#define GPTB_EMHLSR_EP1_POS                1                                              /*!< GPTB EMHLSR: EP1 Position */
#define GPTB_EMHLSR_EP1_MSK                (0x1UL << GPTB_EMHLSR_EP1_POS)                 /*!< GPTB EMHLSR: EP1 Mask */
#define GPTB_EMHLSR_EP0_POS                0                                              /*!< GPTB EMHLSR: EP0 Position */
#define GPTB_EMHLSR_EP0_MSK                (0x1UL << GPTB_EMHLSR_EP0_POS)                 /*!< GPTB EMHLSR: EP0 Mask */
//EMHLCLR
#define GPTB_EMHLCLR_MEM_FAULT_POS         10                                             /*!< GPTB EMHLCLR: MEM_FAULT Position */
#define GPTB_EMHLCLR_MEM_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_MEM_FAULT_POS)          /*!< GPTB EMHLCLR: MEM_FAULT Mask */
#define GPTB_EMHLCLR_EOM_FAULT_POS         9                                              /*!< GPTB EMHLCLR: EOM_FAULT Position */
#define GPTB_EMHLCLR_EOM_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_EOM_FAULT_POS)          /*!< GPTB EMHLCLR: EOM_FAULT Mask */
#define GPTB_EMHLCLR_CPU_FAULT_POS         8                                              /*!< GPTB EMHLCLR: CPU_FAULT Position */
#define GPTB_EMHLCLR_CPU_FAULT_MSK         (0x1UL << GPTB_EMHLCLR_CPU_FAULT_POS)          /*!< GPTB EMHLCLR: CPU_FAULT Mask */
#define GPTB_EMHLCLR_EP3_POS               3                                              /*!< GPTB EMHLCLR: EP3 Position */
#define GPTB_EMHLCLR_EP3_MSK               (0x1UL << GPTB_EMHLCLR_EP3_POS)                /*!< GPTB EMHLCLR: EP3 Mask */
#define GPTB_EMHLCLR_EP2_POS               2                                              /*!< GPTB EMHLCLR: EP2 Position */
#define GPTB_EMHLCLR_EP2_MSK               (0x1UL << GPTB_EMHLCLR_EP2_POS)                /*!< GPTB EMHLCLR: EP2 Mask */
#define GPTB_EMHLCLR_EP1_POS               1                                              /*!< GPTB EMHLCLR: EP1 Position */
#define GPTB_EMHLCLR_EP1_MSK               (0x1UL << GPTB_EMHLCLR_EP1_POS)                /*!< GPTB EMHLCLR: EP1 Mask */
#define GPTB_EMHLCLR_EP0_POS               0                                              /*!< GPTB EMHLCLR: EP0 Position */
#define GPTB_EMHLCLR_EP0_MSK               (0x1UL << GPTB_EMHLCLR_EP0_POS)                /*!< GPTB EMHLCLR: EP0 Mask */
//_EMRISR
#define GPTB_EMRISR_MEM_FAULT_POS          10                                             /*!< GPTB EMRISR: MEM_FAULT Position */
#define GPTB_EMRISR_MEM_FAULT_MSK          (0x1UL << GPTB_EMRISR_MEM_FAULT_POS)           /*!< GPTB EMRISR: MEM_FAULT Mask */
#define GPTB_EMRISR_EOM_FAULT_POS          9                                              /*!< GPTB EMRISR: EOM_FAULT Position */
#define GPTB_EMRISR_EOM_FAULT_MSK          (0x1UL << GPTB_EMRISR_EOM_FAULT_POS)           /*!< GPTB EMRISR: EOM_FAULT Mask */
#define GPTB_EMRISR_CPU_FAULT_POS          8                                              /*!< GPTB EMRISR: CPU_FAULT Position */
#define GPTB_EMRISR_CPU_FAULT_MSK          (0x1UL << GPTB_EMRISR_CPU_FAULT_POS)           /*!< GPTB EMRISR: CPU_FAULT Mask */
#define GPTB_EMRISR_EP3_POS                3                                              /*!< GPTB EMRISR: EP3 Position */
#define GPTB_EMRISR_EP3_MSK                (0x1UL << GPTB_EMRISR_EP3_POS)                 /*!< GPTB EMRISR: EP3 Mask */
#define GPTB_EMRISR_EP2_POS                2                                              /*!< GPTB EMRISR: EP2 Position */
#define GPTB_EMRISR_EP2_MSK                (0x1UL << GPTB_EMRISR_EP2_POS)                 /*!< GPTB EMRISR: EP2 Mask */
#define GPTB_EMRISR_EP1_POS                1                                              /*!< GPTB EMRISR: EP1 Position */
#define GPTB_EMRISR_EP1_MSK                (0x1UL << GPTB_EMRISR_EP1_POS)                 /*!< GPTB EMRISR: EP1 Mask */
#define GPTB_EMRISR_EP0_POS                0                                              /*!< GPTB EMRISR: EP0 Position */
#define GPTB_EMRISR_EP0_MSK                (0x1UL << GPTB_EMRISR_EP0_POS)                 /*!< GPTB EMRISR: EP0 Mask */
//EMMISR
#define GPTB_EMMISR_EOM_FAULT_POS          10                                             /*!< GPTB EMMISR: EOM_FAULT Position */
#define GPTB_EMMISR_EOM_FAULT_MSK          (0x1UL << GPTB_EMMISR_EOM_FAULT_POS)           /*!< GPTB EMMISR: EOM_FAULT Mask */
#define GPTB_EMMISR_MEM_FAULT_POS          9                                              /*!< GPTB EMMISR: MEM_FAULT Position */
#define GPTB_EMMISR_MEM_FAULT_MSK          (0x1UL << GPTB_EMMISR_MEM_FAULT_POS)           /*!< GPTB EMMISR: MEM_FAULT Mask */
#define GPTB_EMMISR_CPU_FAULT_POS          8                                              /*!< GPTB EMMISR: CPU_FAULT Position */
#define GPTB_EMMISR_CPU_FAULT_MSK          (0x1UL << GPTB_EMMISR_CPU_FAULT_POS)           /*!< GPTB EMMISR: CPU_FAULT Mask */
#define GPTB_EMMISR_EP3_POS                3                                              /*!< GPTB EMMISR: EP3 Position */
#define GPTB_EMMISR_EP3_MSK                (0x1UL << GPTB_EMMISR_EP3_POS)                 /*!< GPTB EMMISR: EP3 Mask */
#define GPTB_EMMISR_EP2_POS                2                                              /*!< GPTB EMMISR: EP2 Position */
#define GPTB_EMMISR_EP2_MSK                (0x1UL << GPTB_EMMISR_EP2_POS)                 /*!< GPTB EMMISR: EP2 Mask */
#define GPTB_EMMISR_EP1_POS                1                                              /*!< GPTB EMMISR: EP1 Position */
#define GPTB_EMMISR_EP1_MSK                (0x1UL << GPTB_EMMISR_EP1_POS)                 /*!< GPTB EMMISR: EP1 Mask */
#define GPTB_EMMISR_EP0_POS                0                                              /*!< GPTB EMMISR: EP0 Position */
#define GPTB_EMMISR_EP0_MSK                (0x1UL << GPTB_EMMISR_EP0_POS)                 /*!< GPTB EMMISR: EP0 Mask */
//_EMIMCR
#define GPTB_EMIMCR_MEM_POS                10                                             /*!< GPTB EMIMCR: MEM Position */
#define GPTB_EMIMCR_MEM_MSK                (0x1UL << GPTB_EMIMCR_MEM_POS)                 /*!< GPTB EMIMCR: MEM Mask */
#define GPTB_EMIMCR_EOM_POS                9                                              /*!< GPTB EMIMCR: EOM Position */
#define GPTB_EMIMCR_EOM_MSK                (0x1UL << GPTB_EMIMCR_EOM_POS)                 /*!< GPTB EMIMCR: EOM Mask */
#define GPTB_EMIMCR_CPU_FAULT_POS          8                                              /*!< GPTB EMIMCR: CPU_FAULT Position */
#define GPTB_EMIMCR_CPU_FAULT_MSK          (0x1UL << GPTB_EMIMCR_CPU_FAULT_POS)           /*!< GPTB EMIMCR: CPU_FAULT Mask */
#define GPTB_EMIMCR_EP3_POS                3                                              /*!< GPTB EMIMCR: EP3 Position */
#define GPTB_EMIMCR_EP3_MSK                (0x1UL << GPTB_EMIMCR_EP3_POS)                 /*!< GPTB EMIMCR: EP3 Mask */
#define GPTB_EMIMCR_EP2_POS                2                                              /*!< GPTB EMIMCR: EP2 Position */
#define GPTB_EMIMCR_EP2_MSK                (0x1UL << GPTB_EMIMCR_EP2_POS)                 /*!< GPTB EMIMCR: EP2 Mask */
#define GPTB_EMIMCR_EP1_POS                1                                              /*!< GPTB EMIMCR: EP1 Position */
#define GPTB_EMIMCR_EP1_MSK                (0x1UL << GPTB_EMIMCR_EP1_POS)                 /*!< GPTB EMIMCR: EP1 Mask */
#define GPTB_EMIMCR_EP0_POS                0                                              /*!< GPTB EMIMCR: EP0 Position */
#define GPTB_EMIMCR_EP0_MSK                (0x1UL << GPTB_EMIMCR_EP0_POS)                 /*!< GPTB EMIMCR: EP0 Mask */
//EMICR
#define GPTB_EMICR_MEM_POS                 10                                             /*!< GPTB EMICR: MEM Position */
#define GPTB_EMICR_MEM_MSK                 (0x1UL << GPTB_EMICR_MEM_POS)                  /*!< GPTB EMICR: MEM Mask */
#define GPTB_EMICR_EOM_FAULT_POS           9                                              /*!< GPTB EMICR: EOM_FAULT Position */
#define GPTB_EMICR_EOM_FAULT_MSK           (0x1UL << GPTB_EMICR_EOM_FAULT_POS)            /*!< GPTB EMICR: EOM_FAULT Mask */
#define GPTB_EMICR_CPU_FAULT_POS           8                                              /*!< GPTB EMICR: CPU_FAULT Position */
#define GPTB_EMICR_CPU_FAULT_MSK           (0x1UL << GPTB_EMICR_CPU_FAULT_POS)            /*!< GPTB EMICR: CPU_FAULT Mask */
#define GPTB_EMICR_EP3_POS                 3                                              /*!< GPTB EMICR: EP3 Position */
#define GPTB_EMICR_EP3_MSK                 (0x1UL << GPTB_EMICR_EP3_POS)                  /*!< GPTB EMICR: EP3 Mask */
#define GPTB_EMICR_EP2_POS                 2                                              /*!< GPTB EMICR: EP2 Position */
#define GPTB_EMICR_EP2_MSK                 (0x1UL << GPTB_EMICR_EP2_POS)                  /*!< GPTB EMICR: EP2 Mask */
#define GPTB_EMICR_EP1_POS                 1                                              /*!< GPTB EMICR: EP1 Position */
#define GPTB_EMICR_EP1_MSK                 (0x1UL << GPTB_EMICR_EP1_POS)                  /*!< GPTB EMICR: EP1 Mask */
#define GPTB_EMICR_EP0_POS                 0                                              /*!< GPTB EMICR: EP0 Position */
#define GPTB_EMICR_EP0_MSK                 (0x1UL << GPTB_EMICR_EP0_POS)                  /*!< GPTB EMICR: EP0 Mask */

///TRGFTCR
#define GPTB_FLTSRC_POS	    (0)
#define GPTB_FLTSRC_MSK	    (0x7)
#define GPTB_FLTBLKINV_POS  (4)
#define GPTB_FLTBLKINV_MSK	(0x1 << GPTB_FLTBLKINV_POS)
#define GPTB_ALIGNMD_POS	(5)
#define GPTB_ALIGNMD_MSK	(0x3 << GPTB_ALIGNMD_POS)
typedef enum{
	GPTB_ALIGN_PRD =0,
	GPTB_ALIGN_ZRO,
	GPTB_ALIGN_ZRO_PRD,
	GPTB_ALIGN_T1
}csp_gptb_alignmd_e;

#define GPTB_CROSSMD_POS	(7)
#define GPTB_CROSSMD_MSK	(0x1 << GPTB_CROSSMD_POS)


///TRGFTWR
#define GPTB_FLT_OFFSET_POS	(0)
#define GPTB_FLT_OFFSET_MSK	(0xffff)
#define GPTB_FLT_WDW_POS	(16)
#define GPTB_FLT_WDW_MSK	(0xffff << GPTB_FLT_WDW_POS)

///EVTRG
#define GPTB_SEL_POS_TRG(n)	(n << 2)
#define GPTB_SEL_MSK_TRG(n)	(0xf << GPTB_SEL_POS_TRG(n))
typedef enum{
	GPTB_TRG01_DIS = 0,
	GPTB_TRG01_ZRO,
	GPTB_TRG01_PRD,
	GPTB_TRG01_ZRO_PRD,
	GPTB_TRG01_CMPA_R,
	GPTB_TRG01_CMPA_F,
	GPTB_TRG01_CMPB_R,
	GPTB_TRG01_CMPB_F,
	
	GPTB_TRG01_SYNC = 0xc,
	GPTB_TRG01_PE0,
	GPTB_TRG01_PE1,
	GPTB_TRG01_PE2
}csp_gptb_trgsrc0_e;



#define GPTB_INITEN_POS_CNT(n)	(16+n)
#define GPTB_INITEN_MSK_CNT(n)	(1 <<(16+n))
#define GPTB_OUTEN_POS_TRG(n)	(20+n)
#define GPTB_OUTEN_MSK_TRG(n)	(1 <<(20+n))
#define GPTB_SWTRG_EV(n)		(n+24)

///EVPS
#define GPTB_PRD_POS_EV(n)	(n*4)
#define GPTB_PRD_MSK_EV(n)	(1 << (n*4))
#define GPTB_CNT_POS_EV(n)	(16 + n*4)	
#define GPTB_CNT_MSKS_EV(n)	(1 <<(16 + n*4))

///EVSWF
#define GPTB_SWF_EV(n)	(0x1 << n)

///EVCNTINIT
#define GPTB_CNT_INIT_POS_EV(n)	(n<<2)
#define GPTB_CNT_INIT_MSK_EV(n) 	(0xf << GPTB_CNT_INIT_POS_EV(n))
#define GPTB_CNT_INIT(val, n)   	((0xf & val) << GPTB_CNT_INIT_POS_EV(n))

//REGLK 
#define GPTB_PRDR_POS	(0)
#define GPTB_PRDR_MSK   (0xf << GPTB_PRDR_POS)
#define GPTB_CMPA_POS	(4)
#define GPTB_CMPA_MSK   (0xf << GPTB_CMPA_POS)
#define GPTB_CMPB_POS	(8)
#define GPTB_CMPB_MSK   (0xf << GPTB_CMPB_POS)
#define GPTB_GLD2_POS	(20)
#define GPTB_GLD2_MSK   (0xf << GPTB_GLD2_POS)	
#define GPTB_RSSR_POS	(24)
#define GPTB_RSSR_MSK   (0xf << GPTB_RSSR_POS)	

//REGLK2 
#define GPTB_EMSLCLR_POS   (0)
#define GPTB_EMSLCLR_MSK   (0xf << GPTB_EMSLCLR_POS)
#define GPTB_EMHLCLR_POS   (4)
#define GPTB_EMHLCLR_MSK   (0xf << GPTB_EMHLCLR_POS)
#define GPTB_EMICR_POS	   (8)
#define GPTB_EMICR_MSK     (0xf << GPTB_EMICR_POS)
#define GPTB_EMFRCR_POS	   (12)
#define GPTB_EMFRCR_MSK    (0xf << GPTB_EMFRCR_POS)	
#define GPTB_AQOSF_POS	   (16)
#define GPTB_AQOSF_MSK     (0xf << GPTB_AQOSF_POS)
#define GPTB_AQCSF_POS	   (20)
#define GPTB_AQCSF_MSK     (0xf << GPTB_AQCSF_POS)
	
/******************************************************************************
/// Interrupt Related
******************************************************************************/
typedef enum{
	GPTB_INT_TRGEV0 = 1,
	GPTB_INT_TRGEV1 = 2,
//	GPTB_INT_TRGEV2 = 4,
//	GPTB_INT_TRGEV3 = 8,
	GPTB_INT_CAPLD0 = 1 << 4,
	GPTB_INT_CAPLD1 = 1 << 5,
	GPTB_INT_CAPLD2 = 1 << 6,
	GPTB_INT_CAPLD3 = 1 << 7,
	GPTB_INT_CAU = 1 << 8,
	GPTB_INT_CAD = 1 << 9,
	GPTB_INT_CBU = 1 << 10,
	GPTB_INT_CBD = 1 << 11,
	GPTB_INT_PEND = 1 << 16	
}csp_gptb_int_e;

#define GPTB_INT_EV(n)		(0x1 << n)
#define GPTB_INT_CAPLD(n)	(0x1 << (n+4))


///REGPROT
#define GPTB_REGPROT			(0xa55a << 16 | 0xc73a)




static inline void csp_gptb_wr_key(csp_gptb_t *ptGptbBase)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
}

 
static inline void csp_gptb_clken(csp_gptb_t *ptGptbBase)
 {
	ptGptbBase -> CEDR |=  GPTB_CLKEN;
 }
 
//static inline void csp_gptb_reset(csp_gptb_t *ptGptbBase)
// {
//	ptGptbBase -> CEDR |= GPTB_RESET;
// }
 static inline void csp_gptb_reset(csp_gptb_t *ptGptbBase)
 {  
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> RSSR |= GPTB_RESET;
 }
static inline void csp_gptb_start(csp_gptb_t *ptGptbBase)
{
	ptGptbBase->RSSR |= GPTB_START ;
} 
static inline void csp_gptb_stop(csp_gptb_t *ptGptbBase)
{
	ptGptbBase->RSSR &= ~ GPTB_START ;
}
 
static inline void csp_gptb_dbg_enable(csp_gptb_t *ptGptbBase, bool bEnable)
{
	ptGptbBase -> CEDR = (ptGptbBase -> CEDR & (~GPTB_DBGEN)) | (bEnable << GPTB_DBGEN_POS);
}
static inline void csp_gptb_set_clksrc(csp_gptb_t *ptGptbBase, uint8_t byVal)
{
	ptGptbBase -> CEDR = (ptGptbBase -> CEDR & (~GPTB_CKSRC_MSK)) | (byVal << GPTB_CKSRC_POS);
}

static inline void csp_gptb_set_pscr(csp_gptb_t *ptGptbBase, uint16_t wVal)
{
	ptGptbBase -> PSCR = wVal;
}

static inline uint16_t csp_gptb_get_prdr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> PRDR);
}

static inline void csp_gptb_set_src(csp_gptb_t *ptGptbBase,uint32_t byVal )
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMSRC = byVal;
}
static inline uint32_t csp_gptb_get_src(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMSRC );
}

static inline void csp_gptb_set_src2(csp_gptb_t *ptGptbBase,uint32_t byVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMSRC2  =byVal;
}
static inline uint32_t csp_gptb_get_src2(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMSRC2 );
}
static inline void csp_gptb_set_empol(csp_gptb_t *ptGptbBase, uint32_t byVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMPOL = byVal;
}
static inline uint32_t csp_gptb_get_empol(csp_gptb_t *ptGptbBase )
{	
	return ( ptGptbBase -> EMPOL );
}
static inline  uint32_t csp_gptb_get_emecr(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMECR);
}

static inline void csp_gptb_set_emecr(csp_gptb_t *ptGptbBase,uint32_t byVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMECR |= byVal;
}

static inline void csp_gptb_set_emosr(csp_gptb_t *ptGptbBase,uint32_t byVal)
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMOSR = byVal;
}
static inline  uint32_t csp_gptb_get_emosr(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMOSR);
}

static inline void csp_gptb_Emergency_emimcr(csp_gptb_t *ptGptbBase, csp_gptb_emint_e eInt)
{
	ptGptbBase -> EMIMCR  |=  eInt;
}

static inline uint32_t csp_gptb_get_emmisr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> EMMISR);
}

static inline void csp_gptb_clr_emint(csp_gptb_t *ptGptbBase, csp_gptb_emint_e eInt)
{
	ptGptbBase -> EMICR = eInt;
}


static inline uint16_t csp_gptb_get_emHdlck(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase ->EMHLSR &0x70f);
}

static inline void csp_gptb_clr_emHdlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEp)
{  // ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMHLCLR =( 0x01 << eEp);
}

static inline  uint16_t csp_gptb_get_emSdlck(csp_gptb_t *ptGptbBase)				
{
	return (ptGptbBase->EMSLSR&0x0f);
}

static inline void csp_gptb_clr_emSdlck(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEp)
{
	ptGptbBase -> EMSLCLR = 0x1 << eEp;
}
static inline void csp_gptb_force_em(csp_gptb_t *ptGptbBase, csp_gptb_ep_e eEp)//B_EP3
{
	ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> EMFRCR = 0x1 << eEp;
}
static inline void csp_gptb_sync_enable(csp_gptb_t *ptGptbBase, uint8_t byCh, bool bEnable)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR = (ptGptbBase -> SYNCR & ~(GPTB_SYNC_MSK(byCh))) | bEnable << (GPTB_SYNC_POS(byCh));
}

static inline void csp_gptb_set_sync_mode(csp_gptb_t *ptGptbBase, uint8_t byCh, csp_gptb_syncmd_e eMode)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR = (ptGptbBase -> SYNCR & ~(GPTB_OSTMD_MSK(byCh)) ) | eMode << GPTB_OSTMD_POS(byCh);
}

static inline void csp_gptb_set_auto_rearm(csp_gptb_t *ptGptbBase,  csp_gptb_arearm_e eHWMode)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR = (ptGptbBase -> SYNCR & ~(GPTB_AREARM_MSK)) | eHWMode << GPTB_AREARM_POS;
}

static inline void csp_gptb_sync_trg0sel(csp_gptb_t *ptGptbBase, uint8_t byTrgin)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR   =(uint32_t)((ptGptbBase -> SYNCR & ~GPTB_TRGO0SEL_MSK) | (byTrgin << GPTB_TRGO0SEL_POS));
}

static inline void csp_gptb_sync_trg1sel(csp_gptb_t *ptGptbBase, uint8_t byTrgin)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR   = (ptGptbBase -> SYNCR & ~GPTB_TRGO1SEL_MSK) | (byTrgin << GPTB_TRGO1SEL_POS);
}

static inline void csp_gptb_rearm_sync(csp_gptb_t *ptGptbBase, uint8_t byCh)
{   ptGptbBase -> REGPROT = GPTB_REGPROT;
	ptGptbBase -> SYNCR  = ptGptbBase -> SYNCR | GPTB_REARM(byCh);
}

static inline void csp_gptb_set_gldcr(csp_gptb_t *ptGptbBase, uint32_t byCh)
{
	ptGptbBase -> GLDCR   =  byCh ;
}
static inline void csp_gptb_set_gldcr2(csp_gptb_t *ptGptbBase, uint32_t byCh)
{    ptGptbBase -> REGPROT = GPTB_REGPROT;
	 ptGptbBase -> GLDCR2   |=  byCh ;
}

static inline void csp_gptb_set_trgftcr(csp_gptb_t *ptGptbBase, uint32_t byPrd)
{
	ptGptbBase ->  TRGFTCR = byPrd;
}
static inline void csp_gptb_set_trgftwr(csp_gptb_t *ptGptbBase, uint32_t byPrd)
{
	ptGptbBase ->  TRGFTWR = byPrd;
}

static inline void csp_gptb_trg_cntxiniten_enable(csp_gptb_t *ptGptbBase, uint8_t byCh, bool bEnable)
{
	ptGptbBase -> EVTRG = (uint32_t)((ptGptbBase -> EVTRG & ~GPTB_INITEN_MSK_CNT(byCh)) | (bEnable << GPTB_INITEN_POS_CNT(byCh)));
}

static inline void csp_gptb_set_trgprd(csp_gptb_t *ptGptbBase, uint8_t byCh,uint8_t byPrd)
{
	ptGptbBase ->  EVPS = (uint32_t)(ptGptbBase ->  EVPS & (~GPTB_PRD_MSK_EV(byCh))) | (byPrd << GPTB_PRD_POS_EV(byCh));
}

static inline uint32_t csp_gptb_get_trgtime(csp_gptb_t *ptGptbBase, uint8_t byCh)
{
	return (uint32_t) (ptGptbBase -> EVPS & GPTB_CNT_MSKS_EV(byCh) >> GPTB_CNT_POS_EV(byCh));
 
}

static inline void csp_gptb_swf_trg(csp_gptb_t *ptGptbBase, uint8_t byCh)
{
	ptGptbBase -> EVSWF |= 0x1 << byCh; 
}

static inline void csp_gptb_trg_cntxinit(csp_gptb_t *ptGptbBase, uint8_t byCh, uint8_t byVal)
{
	ptGptbBase -> EVCNTINIT = (ptGptbBase -> EVCNTINIT & ~GPTB_CNT_INIT_MSK_EV(byCh)) | GPTB_CNT_INIT(byVal,byCh);
}


static inline void csp_gptb_set_cr(csp_gptb_t *ptGptbBase, uint32_t wCr)
{
	ptGptbBase->CR = wCr;
}

static inline void csp_gptb_set_mode(csp_gptb_t *ptGptbBase, csp_gptb_md_e eMode)
{
	ptGptbBase -> CR = (ptGptbBase -> CR &(~GPTB_MODE_MSK)) | eMode << GPTB_MODE_POS;
}

static inline void csp_gptb_set_cntmd(csp_gptb_t *ptGptbBase, csp_gptb_cntmd_e eMode)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_CNTMD_MSK)) | (eMode << GPTB_CNTMD_POS);
}

static inline uint8_t csp_gptb_get_cntmd(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase->CR & GPTB_CNTMD_MSK);
}
static inline void csp_gptb_set_capmd(csp_gptb_t *ptGptbBase, csp_gptb_capmd_e eMode)//csp_gpta_capmd_e
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_CAPMD_MSK)) | (eMode << GPTB_CAPMD_POS);
}

static inline void csp_gptb_set_stopwrap(csp_gptb_t *ptGptbBase, uint8_t byTime)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & ~(GPTB_STOPWRAP_MSK)) | (byTime << GPTB_STOPWRAP_POS);
}

static inline void csp_gptb_set_burst(csp_gptb_t *ptGptbBase,csp_gptb_cgsrc_e eCgsrc, bool bEnable)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & ~(GPTB_BURST_MSK)) | (bEnable << GPTB_BURST_POS);
	if(bEnable == ENABLE)
	{
		ptGptbBase -> CR =(ptGptbBase -> CR & ~(GPTB_CGSRC_MSK))|(eCgsrc<<GPTB_CGSRC_POS);
	}
}

static inline void csp_gptb_flt_init(csp_gptb_t *ptGptbBase, csp_gptb_cnflt_e eCgflt,bool bEnable)
{
	ptGptbBase -> CR = (ptGptbBase -> CR & ~(GPTB_FLT_INIT_MSK)) | (bEnable << GPTB_FLT_INIT_POS);
	if(bEnable == ENABLE)
	{
		ptGptbBase -> CR=(ptGptbBase -> CR & ~(GPTB_CGFLT_MSK))|(eCgflt<<GPTB_CGFLT_POS);
	}
}

static inline void csp_gptb_set_prdr(csp_gptb_t *ptGptbBase, uint16_t bwVal)
{
	ptGptbBase -> PRDR = bwVal;
}

static inline void csp_gptb_set_start_src(csp_gptb_t *ptGptbBase, csp_gptb_startsrc_e eVal)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_STARTSRC_MSK))| (eVal <<GPTB_STARTSRC_POS);
}

static inline void csp_gptb_set_stop_st(csp_gptb_t *ptGptbBase, csp_gptb_stpst_e eVal)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_STPST_MSK))| (eVal <<GPTB_STPST_POS);
}

static inline void csp_gptb_set_opmd(csp_gptb_t *ptGptbBase, csp_gptb_opmd_e eVal)
{
	ptGptbBase -> CR = (ptGptbBase->CR & (~GPTB_OPMD_MSK))| (eVal <<GPTB_OPMD_POS);
}

static inline void csp_gptb_capld_enable(csp_gptb_t *ptGptbBase, bool bEnable)
{
	ptGptbBase -> CR = (ptGptbBase->CR & ~(GPTB_CAPLD_MSK)) | (bEnable << GPTB_CAPLD_POS);
}
//
static inline void csp_gptb_prdld_mod(csp_gptb_t *ptGptbBase, csp_gptb_ldprdr_e Val)
{
	ptGptbBase -> CR = (ptGptbBase->CR & ~(GPTB_PRDLD_MSK)) | (Val << GPTB_PRDLD_POS);
}

static inline void csp_gptb_set_aqcra(csp_gptb_t *ptGptbBase, uint32_t bwVal)
{
	ptGptbBase -> AQCRA = bwVal;
}
static inline void csp_gptb_set_aqcrb(csp_gptb_t *ptGptbBase, uint32_t bwVal)
{
	ptGptbBase -> AQCRB = bwVal;
}
static inline uint32_t csp_gptb_get_dbldr(csp_gptb_t *ptGptbBase)
{
 return ( ptGptbBase -> DBLDR);
}

static inline void csp_gptb_set_dbldr(csp_gptb_t *ptGptbBase, uint32_t w_Val)
{
  ptGptbBase -> DBLDR = w_Val;
}

static inline void csp_gptb_set_dbcr(csp_gptb_t *ptGptbBase, uint32_t w_VAL)
{
	ptGptbBase -> DBCR = w_VAL;
}

static inline uint32_t csp_gptb_get_dbcr(csp_gptb_t *ptGptbBase)
{
	return ( ptGptbBase -> DBCR);	
}
static inline void csp_gptb_set_dpscr(csp_gptb_t *ptGptbBase, uint16_t wVal)
{
	ptGptbBase ->DPSCR = wVal;
}
static inline void csp_gptb_set_dbdtr(csp_gptb_t *ptGptbBase, uint16_t wVal)
{
	ptGptbBase ->DBDTR = wVal;
}
static inline void csp_gptb_set_dbdtf(csp_gptb_t *ptGptbBase, uint16_t wVal)
{
	ptGptbBase ->DBDTF = wVal;
}

//static inline void csp_gptb_set_cpcr(csp_gptb_t *ptGptbBase, uint32_t byVal)
//{
//	ptGptbBase ->CPCR = byVal ;
//}
//static inline uint32_t csp_gptb_get_cpcr(csp_gptb_t *ptGptbBase)
//{
//	return ( ptGptbBase ->CPCR );
//}

static inline void csp_gptb_set_cmpa(csp_gptb_t *ptGptbBase, uint16_t bwVal)
{
	ptGptbBase -> CMPA = bwVal;
}
static inline uint16_t csp_gptb_get_cmpa(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPA);
}
static inline uint16_t csp_gptb_get_cmpaa(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPAA);
}
static inline void csp_gptb_set_cmpb(csp_gptb_t *ptGptbBase, uint16_t bwVal)
{
	ptGptbBase -> CMPB = bwVal;
}
static inline uint16_t csp_gptb_get_cmpb(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPB);
}
static inline uint16_t csp_gptb_get_cmpba(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> CMPBA);
}
static inline void csp_gptb_set_prd(csp_gptb_t *ptGptbBase, uint16_t bwVal)
{
	ptGptbBase -> PRDR = bwVal;
}
static inline uint16_t csp_gptb_get_prd(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> PRDR);
}
static inline void csp_gptb_set_phsr(csp_gptb_t *ptGptbBase, uint16_t bwVal)
{
	ptGptbBase -> PHSR = bwVal;
}
static inline uint16_t csp_gptb_get_phsr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> PHSR);
}


static inline void csp_gptb_int_enable(csp_gptb_t *ptGptbBase, csp_gptb_int_e byInt, bool bEnable)
{
	ptGptbBase -> IMCR = ptGptbBase -> IMCR & ( ~byInt);
	if (bEnable)
		ptGptbBase ->IMCR |= byInt;
	
}

static inline uint32_t csp_gptb_get_risr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> RISR);
}

static inline uint32_t csp_gptb_get_misr(csp_gptb_t *ptGptbBase)
{
	return (ptGptbBase -> MISR);
}
static inline void csp_gptb_clr_int(csp_gptb_t *ptGptbBase, csp_gptb_int_e byInt)
{
	ptGptbBase -> ICR = byInt;
}
static inline void csp_gptb_set_aqosf(csp_gptb_t *ptGptbBase, uint32_t bwVal)
{
	ptGptbBase -> AQOSF = bwVal;
}


static inline void csp_gptb_trg_xoe_enable(csp_gptb_t *ptGptbBase, uint8_t byCh, bool bEnable)
{
	ptGptbBase -> EVTRG = (ptGptbBase -> EVTRG & (~GPTB_OUTEN_MSK_TRG(byCh))) | (bEnable << GPTB_OUTEN_POS_TRG(byCh));
}

static inline void csp_gptb_set_trgsel01(csp_gptb_t *ptGptbBase, uint8_t byCh,  uint8_t eSrc)
{ 
	
	ptGptbBase -> EVTRG = (ptGptbBase -> EVTRG & (~GPTB_SEL_MSK_TRG(byCh))) | (eSrc << GPTB_SEL_POS_TRG(byCh));
	
}

static inline void csp_gptb_set_feglk(csp_gptb_t *ptGptbBase, uint32_t byCh)
{
	ptGptbBase -> REGLK = byCh;
}
static inline void csp_gptb_set_feglk2(csp_gptb_t *ptGptbBase, uint32_t byCh)
{
	ptGptbBase -> REGLK2 = byCh;
}


///function declaration 

csp_error_t csp_gptb_set_outfreq(csp_gptb_t *ptGptbBase, uint32_t wHz);
csp_error_t csp_gptb_set_dualout(csp_gptb_t *ptGptbBase,  uint32_t wDutyCycle, uint32_t wTns);

csp_error_t csp_gptb_set_ch_duty(csp_gptb_t *ptGptbBase, csp_gptb_chtype_e eCh, uint32_t wActiveTime);
csp_error_t csp_gptb_set_ch_singleout(csp_gptb_t *ptGptbBase, csp_gptb_chtype_e eCh, uint32_t wDutyCycle);
void csp_gptb_sw_start(csp_gptb_t *ptGptbBase);
void csp_gptb_sw_stop(csp_gptb_t *ptGptbBase);
void csp_gptb_aqcra_config(csp_gptb_t *ptGptbBase, csp_gptb_csrc_sel_e eCaSel, csp_gptb_action_e eZro, csp_gptb_action_e eCau, csp_gptb_action_e ePrd, csp_gptb_action_e eCad);
void csp_gptb_aqcrb_config(csp_gptb_t *ptGptbBase, csp_gptb_csrc_sel_e eCaSel, csp_gptb_action_e eZro, csp_gptb_action_e eCau, csp_gptb_action_e ePrd, csp_gptb_action_e eCad);
csp_error_t csp_gptb_set_sync_enable(csp_gptb_t *ptGptbBase, uint8_t bySync, bool bEnable);
uint32_t csp_gptb_get_cap_tick(csp_gptb_t *ptGptbBase);
csp_error_t csp_gptb_ldrst_enable(csp_gptb_t *ptGptbBase, uint8_t cmp, bool bEnable);
//csp_error_t csp_gptb_set_cmp_ldmd(csp_gptb_t *ptGptbBase, uint8_t cmp, csp_gptb_cmpdata_ldmd_e eLdmd, csp_gptb_ldamd_e eLdT);
void csp_gptb_aqosf_config(csp_gptb_t *ptGptbBase, csp_gptb_ldosf_e acta,csp_gptb_ldosf_e actb, csp_gptb_ldaqcr_e  rldcsf);
void csp_gptb_ostsfa_sw(csp_gptb_t *ptGptbBase);
void csp_gptb_ostsfb_sw(csp_gptb_t *ptGptbBase);
void csp_gptb_cont_software_a_sw(csp_gptb_t *ptGptbBase);
csp_error_t csp_gptb_set_sync_ostmd_enable(csp_gptb_t *ptGptbBase, uint8_t bySync, bool bEnable);
csp_error_t csp_gptb_set_sync_ostmd_rearm(csp_gptb_t *ptGptbBase, uint8_t bySync);
void csp_gptb_phs_enable(csp_gptb_t *ptGptbBase, csp_gptb_phsdir_e val,uint16_t hwVal);



#endif   /* csp_gptb_H */



