/***********************************************************************//** 
 * \file  sio.c
 * \brief  csi sio driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>XB   <td>initial
 * <tr><td> 2021-1-03 <td>V0.0  <td>ZJY   <td>modify
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
 @brief  tkey Variable Definition
***********************************************************************/
uint8_t  	byTkeyGlobalSens;	
uint8_t  	byTkeyGlobalTrigger;		
uint8_t 	byTkeyGlobalIcon;	
uint8_t 	byPressDebounce;		
uint8_t 	byReleaseDebounce;	
uint8_t 	byMultiTimesFilter;			
uint8_t 	byValidKeyNum;				
uint8_t 	byKeyMode;					
uint8_t 	byBaseUpdateSpeed;
uint8_t 	byTkeyRebuildTime;
uint8_t 	byTkeyFineTurn;
uint8_t 	byTkeyIcon[32];
uint8_t 	byTkeyChxSens[32];		
uint8_t		byTkeyChxTrigger[32];							
uint8_t		byTkeyChxIcon[32];

uint16_t 	hwTkeyBaseCnt;	
uint16_t 	hwTkeyEcLevel;
uint16_t 	hwTkeyPselMode;		
uint16_t 	hwTkeyFvrLevel;
uint16_t 	hwTkeySenprd[32];
uint16_t 	hwTkeyTriggerLevel[32];	

uint32_t 	dwTkeyScanFreq;
uint32_t 	dwTkeyIoEnable;


extern volatile uint32_t dwKey_Map;
/*********************************************************************** 
 @brief  Tkey extern functions                  
***********************************************************************/ 
extern void tkey_sampling_prog(void);
extern void	tkey_keymap_prog(void);
extern void	tkey_baseline_tracking(void);
extern void	tkey_result_prog(void);
extern void	tkey_trigger_auto_adjust(void);
extern void tkey_error_detect(void);
extern void tkey_baseline_prog(void);
extern void csi_tkey_init(void);
extern void csi_tkey_main_prog(void);
extern void csi_tkey_process(void); 
extern void csi_tkey_int_process(void); 


#ifdef __cplusplus
}
#endif

#endif /* _DRV_tkey_H_ */