/***********************************************************************//** 
 * \file  tkey.h
 * \brief  csi tkey driver
 * \copyright Copyright (C) 2020-2025 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2022-9-03 <td>V0.0  <td>APT AE   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_TKEY_H_
#define _DRV_TKEY_H_

#include <stdint.h>
#include <stdbool.h>
#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif

/*********************************************************************** 
 @brief  tkey clock control register                
***********************************************************************/ 
typedef enum
{
    TK_CLK_EN = 1<<0,
	TK_CLK_DIS = 0<<0,
}TK_CLK_TypeDef;

/*********************************************************************** 
 @brief  tkey dch clock select               
***********************************************************************/ 
typedef enum
{
    TK_DCHCKSEL_REFCLK  = 0<<3,
	TK_DCHCKSEL_PCLK = 1<<3,
}TK_DCHCKSEL_TypeDef;

/*********************************************************************** 
 @brief  tkey dch DIV               
***********************************************************************/ 
typedef enum
{
    TK_DCHCKDIV_2 = 0<<8,
	TK_DCHCKDIV_4 = 1<<8,
	TK_DCHCKDIV_6 = 2<<8,
	TK_DCHCKDIV_8 = 3<<8,
}TK_DCHCKDIV_TypeDef;

/*********************************************************************** 
 @brief  tkey TRIM EN/DIS             
***********************************************************************/ 
typedef enum
{
    TK_ALDTRIM_EN  = 1<<15,
	TK_ALDTRIM_DIS = 0<<15,
}TK_ALDTRIM_TypeDef;
/*********************************************************************** 
 @brief  tkey trim select              
***********************************************************************/ 
typedef enum
{
    TK_TRMTARSEL_TRIM  = 0<<16,
	TK_TRMTARSEL_FSEL = 1<<16,
}TK_TRMTARSEL_TypeDef;

/***********************************************************************
 @brief  tkey haed EN/DIS
***********************************************************************/
typedef enum
{
    TK_HM_EN = 1<<0,
	TK_HM_DIS = 0<<0,
}TK_HMEN_TypeDef;

/***********************************************************************
 @brief  tkey scan mode
***********************************************************************/
typedef enum
{
    TK_MODE_SINGLE  = 0<<1,
	TK_MODE_SEQ = 1<<1,
}TK_MODE_TypeDef;

/***********************************************************************
 @brief  tkey ec level
***********************************************************************/
typedef enum
{
    TK_ECLVL_1V = 0<<7,
	TK_ECLVL_2V = 1<<7,
	TK_ECLVL_3V = 2<<7,
	TK_ECLVL_3V6 = 3<<7,
}TK_ECLVL_TypeDef;

/***********************************************************************
 @brief  tkey scan timeout
***********************************************************************/
typedef enum
{
    TK_SCTIMLMT_5ms = 0<<9,
	TK_SCTIMLMT_10ms = 1<<9,
	TK_SCTIMLMT_50ms = 2<<9,
	TK_SCTIMLMT_100ms = 3<<9,
}TK_SCTIMLMT_TypeDef;

/***********************************************************************
 @brief  tkey scan timeout EN/DIS
***********************************************************************/
typedef enum
{
    TK_SCTLIM_EN = 0<<11,
	TK_SCTLIM_DIS = 1<<11,
}TK_SCTLIM_TypeDef;

/***********************************************************************
 @brief  tkey scan intervals
***********************************************************************/
typedef enum
{
    TK_INTVTIM_None = 0<<12,
	TK_INTVTIM_16ms = 1<<12,
	TK_INTVTIM_32ms = 2<<12,
	TK_INTVTIM_64ms = 3<<12,
	TK_INTVTIM_128ms = 4<<12,
	TK_INTVTIM_186ms = 5<<12,
}TK_INTVTIM_TypeDef;

/***********************************************************************
 @brief  tkey power source
***********************************************************************/
typedef enum
{
    TK_PWRSRC_FVR = 0<<15,
	TK_PWRSRC_AVDD = 1<<15,
}TK_PWRSRC_TypeDef;

/***********************************************************************
 @brief  tkey internal resistance switch
***********************************************************************/
typedef enum
{
    TK_INTREN_EN = 1<<16,
	TK_INTREN_DIS = 0<<16,
}TK_INTREN_TypeDef;

/***********************************************************************
 @brief  tkey edge detection stability conditions
***********************************************************************/
typedef enum
{
    TK_STBLCNT_4 = 0<<17,
	TK_STBLCNT_8 = 1<<17,
	TK_STBLCNT_12 = 2<<17,
	TK_STBLCNT_16 = 3<<17,
}TK_STBLCNT_TypeDef;

/***********************************************************************
 @brief  tkey continue counting after scan counter overflow EN/DIS
***********************************************************************/
typedef enum
{
    TK_CYCSCNT_EN = 1<<19,
	TK_CYCSCNT_DIS = 0<<19,
}TK_CYCSCNT_TypeDef;

/***********************************************************************
 @brief  tkey deepsleep EN/DIS
***********************************************************************/
typedef enum
{
    TK_STPATDSL_EN = 0<<20,
	TK_STPATDSL_DIS = 1<<20,
}TK_STPATDSL_TypeDef;

/***********************************************************************
 @brief  tkey not activated TCH as the current scan channel Status
***********************************************************************/  
typedef enum
{
    TK_DST_Z = 0<<0,
	TK_DST_HIGH = 1<<0,
	TK_DST_LOW = 2<<0,
}TK_DST_TypeDef;

/***********************************************************************
 @brief  tkey Single line debug data EN/DIS
***********************************************************************/  
typedef enum
{
    TK_DBGSDO_EN = 1<<4,
	TK_DBGSDO_DIS = 0<<4,
}TK_DBGSDO_TypeDef;

/***********************************************************************
 @brief  tkey Single line debug data bit number
***********************************************************************/  
typedef enum
{
    TK_DBGDLEN_16bit = 0<<5,
	TK_DBGDLEN_24bit = 1<<5,
}TK_DBGDLEN_TypeDef;

/***********************************************************************
 @brief  tkey Single line debug clock div
***********************************************************************/  
typedef enum
{
    TK_DBGDOCKDIV_2 = 0<<6,
	TK_DBGDOCKDIV_4 = 1<<6,
	TK_DBGDOCKDIV_8 = 2<<6,
	TK_DBGDOCKDIV_16 = 3<<6,
}TK_DBGDOCKDIV_TypeDef;

/***********************************************************************
 @brief  tkey Phase shift repeat sequence cycle control
***********************************************************************/  
typedef enum
{
    TK_PHS_SHFT_SEQ_0 = 0<<20,
	TK_PHS_SHFT_SEQ_2 = 1<<20,
	TK_PHS_SHFT_SEQ_3 = 2<<20,
	TK_PHS_SHFT_SEQ_4 = 3<<20,
	TK_PHS_SHFT_SEQ_5 = 4<<20,
	TK_PHS_SHFT_SEQ_6 = 5<<20,
	TK_PHS_SHFT_SEQ_7 = 6<<20,
	TK_PHS_SHFT_SEQ_8 = 7<<20,
}TK_PHS_SHFT_SEQ_TypeDef;

/***********************************************************************
 @brief  tkey scan deviation threshold setting
***********************************************************************/
typedef enum
{
    TK_THRSEL_CMP_BIT_23_4 = 0<<0,
	TK_THRSEL_CMP_BIT_23_5 = 1<<0,
	TK_THRSEL_CMP_BIT_23_6 = 2<<0,
	TK_THRSEL_CMP_BIT_23_7 = 3<<0,
	TK_THRSEL_CMP_BIT_23_8 = 4<<0,
	TK_THRSEL_CMP_BIT_23_9 = 5<<0,
	TK_THRSEL_CMP_BIT_23_10 = 6<<0,
	TK_THRSEL_CMP_BIT_23_11 = 7<<0,
}TK_THRSEL_TypeDef;

/***********************************************************************
 @brief  tkey scan deviation threshold EN/DIS
***********************************************************************/
typedef enum
{
	TK_THR_EN = 1<<8,
	TK_THR_DIS = 0<<8,
}TK_THREN_TypeDef;

/***********************************************************************
 @brief  tkey fuction define
***********************************************************************/
#define TK_START 0X1
#define TK_STOP 0X0
#define TK_SRR (0X5<<12)
#define TK_PCKDIV(n) ((n&0xf)<<4)
#define TK_TRIMCONFIG0(n) ((n&0x1f)<<17)
#define TK_TRIMCONFIG1(n) ((n&0x1f)<<22)
#define TK_TRIMCONFIG2(n) ((n&0x1f)<<27)
#define TK_SHFT_STEP(n) ((n&0xff)<<24)
#define TCH_EN(val)   (0x01<<val)
#define TK_SEQLEN(n) (((n-1)&0x1f)<<2)


/***********************************************************************
 @brief  tkey status and int define
***********************************************************************/
#define TK_BUSY 	(0X1<<12)
#define TK_SEQ_DNE	(0x01ul << 0)
#define TK_CH_DNE	(0x01ul << 1)
#define TK_CH_ERR	(0x01ul << 2)
#define TK_TIMOVF	(0x01ul << 3)
#define TK_REFOVF	(0x01ul << 4)
#define TK_THROVR	(0x01ul << 5)
#define TK_FVR_2048V 0
#define	TK_FVR_4096V 1
#define TK_FVR_NONE  2
/***********************************************************************
 @brief  tkey Variable Definition
***********************************************************************/
unsigned char	byTkeyGlobalSens;
unsigned char	byTkeyGlobalIcon;	
unsigned char	byPressDebounce;		
unsigned char	byReleaseDebounce;	
unsigned char 	byMultiTimesFilter;			
unsigned char 	byValidKeyNum;				
unsigned char 	byKeyMode;					
unsigned char 	byBaseUpdateSpeed;
unsigned char 	byTkeyRebuildTime;
unsigned char 	byTkeyFineTurn;
unsigned char 	byTkeyIcon[32];
unsigned char 	byTkeyChxSens[32];		
unsigned char	byTkeyChxTrigger[32];							
unsigned char	byTkeyChxIcon[32];
unsigned char   dwTkeyWakeupLevel;
unsigned char 	byTkeyLowPowerMode;
unsigned char 	byTkeyLowPowerLevel;

unsigned short  hwTkeyGlobalTrigger;
unsigned short 	hwTkeyBaseCnt;	
unsigned short 	hwTkeyEcLevel;
unsigned short 	hwTkeyPselMode;		
unsigned short 	hwTkeyFvrLevel;
unsigned short 	hwTkeySenprd[32];
unsigned short 	hwTkeyTriggerLevel[32];	

unsigned long  	dwTkeyScanFreq;
unsigned long  	dwTkeyIoEnable;	
unsigned long  	dwTkeyWakeupIoEnable;
/*
unsigned char 	byTkeyLowPowerMode;
unsigned char 	byTkeyLowPowerLevel;	
unsigned char 	byAutoAdjustDis;
*/
/***********************************************************************
 @brief  Tkey Hidden Variable Definition
***********************************************************************/
unsigned char 	byTkeyDstStatus;		
//Used to modify the default state when TCHx is not enabled;0/3=Z,1=high,2=low(Default)
unsigned char 	byTkeyIntrStatus;		
//Used to enable the default internal resistor of TCHx;0=disable(Default),1=enable
unsigned char   byTkeyImtvtimTime;		
//Used to modify the scan interval;0=none(Default),1=16ms,2=32ms,3=64ms,4=128ms,>4=186ms
unsigned char 	byAutoAdjustDis;		
//Used to enable or disable automatic TriggerLevel adjustment on the first pressd of each TCHx;1=disable,0=enable(Default)
unsigned char 	byTkeyNegBuildBounce;	
//Used to modify the Bounce of the forced negative Rebuild;>=1 (Default=3)
//The higher the number, the longer it takes to trigger the update mechanism
unsigned char 	byTkeyPosBuildBounce;
//Used to modify the Bounce of the forced postive Rebuild;>=1 (Default=3)
//The higher the number, the longer it takes to trigger the update mechanism


extern volatile unsigned long dwKey_Map;
extern volatile unsigned char byForceReBuild;
/*********************************************************************** 
 @brief  Tkey extern functions                  
***********************************************************************/ 
extern void csi_tkey_init(void);
extern void csi_tkey_main_prog(void);
extern void csi_tkey_int_process(void);
extern void csi_tkey_basecnt_process(void);
extern void csi_tkey_parameter_init(void);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_tkey_H_ */