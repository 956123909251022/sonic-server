/***********************************************************************//** 
 * \file  rtc.h
 * \brief  head file for RTC
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-8 <td>V0.0  <td>WNN   <td>initial
 * <tr><td> 2021-1-8 <td>V0.0  <td>WNN   <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_RTC_H_
#define _DRV_RTC_H_

#include <drv/common.h>
#include "csp.h"

#ifdef __cplusplus
extern "C" {
#endif  

typedef struct  {
	uint8_t		byClkSrc;	/// clock source
	uint8_t		byFmt;		//  timer format
} csi_rtc_config_t;

typedef struct {
	uint8_t		byAlmSt;	// 0: not yet
							// 1: time up
}csi_rtc_alm_t;

/****** RTC time *****/
typedef struct {
    int iSec;             ///< Second.      [0-59]
    int iMin;             ///< Minute.      [0-59]
    int iHour;            ///< Hour.        [0-23]
    int iMday;            ///< Day.         [1-31]
    int iMon;             ///< Month.       [1-12]
    int iYear;            ///< Year-1900.   [70- ]      !NOTE:100=2000    
	int iWday;			/// weekday		  [1-7]	     
    int iYday;            ///< Days in year.[0-365]     !NOTE:January 1st = 0
	int iIsdst;			/// Non-0 if daylight savings time is in effect
	int iPm;				///< PM.		  [0/1]
} csi_rtc_time_t;

/****** definition for rtc *****/
typedef struct csi_rtc csi_rtc_t;
struct csi_rtc {
    csi_dev_t           dev;
    void               (*callback)(csi_rtc_t *rtc, void *arg);
    void               *arg;
    void               *priv;
} ;

//typedef enum{
//	RTC_CLKSRC_ISOSC = 0,
//	RTC_CLKSRC_IMOSC_DIV4,
//	RTC_CLKSRC_EMOSC,
//	RTC_CLKSRC_EMOSC_DIV4
//}csi_rtc_clksrc_e;

typedef enum{
	RTC_CLKSRC_ISOSC = 0,
	RTC_CLKSRC_IMOSC_DIV4,
	RTC_CLKSRC_ESOSC,
	RTC_CLKSRC_EMOSC_DIV4
}csi_rtc_clksrc_e;

typedef enum {
	RTC_24FMT = 0,
	RTC_12FMT
}csi_rtc_fmt_e;

typedef enum {
	RTC_AM = 0,
	RTC_PM
}csi_rtc_ampm_e;

typedef enum {
	RTC_ALMA = 0,
	RTC_ALMB
}csi_rtc_alarm_e;

typedef enum {
	RTC_TIMER_DIS = 0,
	RTC_TIMER_0_5S,
	RTC_TIMER_1S,
	RTC_TIMER_1MIN,
	RTC_TIMER_1H,
	RTC_TIMER_1DAY,
	RTC_TIMER_1MON	
}csi_rtc_timer_e;

typedef enum {
	RTC_ALMA_P = 0,
	RTC_ALMA_H,
	RTC_ALMA_L,
	RTC_ALMB_P,
	RTC_ALMB_H,
	RTC_ALMB_L,
	
}csi_rtc_osel_e;

typedef enum{
	RTC_TRGOUT0 = 0,
	RTC_TRGOUT1
}csi_rtc_trgout_e;

typedef enum{
	RTC_TRGOUT_DIS = 0,
	RTC_TRGOUT_ALRMA,
	RTC_TRGOUT_ALRMB,
	RTC_TRGOUT_ALRMA_ALRMB,
	RTC_TRGOUT_CPRD
}csi_rtc_trgsrc_e;

typedef enum{
	RTC_INTSRC_ALMA   = 0x01,
	RTC_INTSRC_ALMB   = 0x02,
	RTC_INTSRC_CPRD   = 0x04,
	RTC_INTSRC_TRGEV0 = 0x08,
	RTC_INTSRC_TRGEV1 = 0x10
}csi_rtc_intsrc_e;

typedef enum{
//	EveryDay   = 0x00,
//	Sundays    = 0x01,
//	Saturdays  = 0x02,
//	Fridays    = 0x04,
//	Thursdays  = 0x08,
//	Wednesdays = 0x10,
//	Tuesdays   = 0x20,
//	Mondays    = 0x40
	
	EVERYDAY   = 0x00,
	SUNDAYS    = 0x01,
	SATURDAYS  = 0x02,
	FRIDAYS    = 0x04,
	THURSDAYS  = 0x08,
	WEDNESDAYS = 0x10,
	TUESDAYS   = 0x20,
	MONDAYS    = 0x40
}csi_rtc_weeksrc_e;



/**
  \brief       Initialize RTC Interface. Initializes the resources needed for the RTC interface
  \param       ptRtc    pointer of rtc register structure
  \param	   eOsc		clock source 
  \param       eFmt     rtc format: RTC_24FMT/RTC_12FMT
  \return      none
*/
void csi_rtc_init(csp_rtc_t *ptRtc, csi_rtc_config_t *tConfig);

/**
  \brief       De-initialize RTC Interface. stops operation and releases the software resources used by the interface
  \param[in]   rtc    pointer of rtc register structure to operate
  \return      None
*/
void csi_rtc_uninit(csi_rtc_t *rtc);

/**
  \brief       Set system date and wait for synchro
  \param[in]   rtc        pointer of rtc register structure to operate
  \param[in]   rtctime    pointer to rtc time
  \return      error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_time(csp_rtc_t *rtc, csi_rtc_time_t *rtctime);

/**
  \brief       To set/change RTC format
  \param       pointer of rtc register structure pointer of rtc register structure to operate
  \param       eFmt \ref rtc_fmt_e    
  \return      none
*/

/**
  \brief       To start RTC 
  \param       pointer of rtc register structure pointer of rtc register structure to operate
  \return      none
*/
void csi_rtc_start(csp_rtc_t *ptRtc);

/**
  \brief       To stop RTC 
  \param       pointer of rtc register structure pointer of rtc register structure to operate
  \return      none
*/
void csi_rtc_stop(csp_rtc_t *ptRtc);


/**
  \brief       To set/change RTC format
  \param       pointer of rtc register structure pointer of rtc register structure to operate
  \param       eFmt \ref rtc_fmt_e    
  \return      none
*/
void csi_rtc_change_fmt(csp_rtc_t *ptRtc,  csi_rtc_fmt_e eFmt);

/**
  \brief   RTC interrupt enable/disable
  \param   ptRtc      handle pointer of rtc register structure to operate
  \param   eIntSrc	  interrupt source	
  \param   bEnable    ENABLE/DISABLE 
  \return  none
*/
void csi_rtc_int_enable(csp_rtc_t *ptRtc, csi_rtc_intsrc_e eIntSrc, bool bEnable);

/**
  \brief       get the RTC is interrupted
  \param[in]   eIntSrc RTC interrupt source
  \param[in]   bClrEn  clean interrupt
  \return      RTC interrupt register(RTC->MISR) value 
*/
bool csi_rtc_get_int_msg(csi_rtc_intsrc_e eIntSrc,bool bClrEn);


/**
  \brief       clean the RTC is interrupted
  \param[in]   eIntSrc      interrupt source
  \return      none
*/
void csi_rtc_clr_int_msg(csi_rtc_intsrc_e eIntSrc);

/**
  \brief       Get system date
  \param[in]   rtc        pointer of rtc register structure to operate
  \param[out]  rtctime    pointer to rtc time
  \return      none
*/
void csi_rtc_get_time(csp_rtc_t *rtc, csi_rtc_time_t *ptRtctime);

/**
  \brief       Get alarm remaining time
  \param[in]   rtc    pointer of rtc register structure to operate
  \param	   eAlm  RTC_ALMA/RTC_ALMB
  \return      the remaining time(s)
*/
uint32_t csi_rtc_get_alarm_remaining_time(csp_rtc_t *rtc, csi_rtc_alarm_e eAlm);

/**
  \brief   Config RTC alarm
  \param   ptRtc      handle pointer of rtc register structure to operate
  \param   eAlm	  RTC_ALMA/RTC_ALMB
  \param   rtctime    alarm time(s) 
  \param   byMode	  	0: day       hour min sec
						1: weekday   hour min sec
						2:           hour min sec

  \return  error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_alarm(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm, csi_rtc_time_t *tpRtcTime);

/**
  \brief       Cancel the rtc alarm
  \param       ptRtc    pointer of rtc register structure to operate
  \param       eAlm	RTC_ALMA/RTC_ALMB
  \return      none
*/
void csi_rtc_cancel_alarm(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm);

/**
  \brief       Judge rtc is running
  \param[in]   rtc    pointer of rtc register structure to operate
  \return      
               true  - rtc is running 
               false - rtc is not running 
*/
bool csi_rtc_is_running(csi_rtc_t *rtc);


/**
  \brief    use rtc as a timer
  \param	callback  callback function to be called when time = ePrd
  \param 	ePrd    time interval of the timer.
  \param[in]   rtc    pointer of rtc register structure to operate
*/
void csi_rtc_start_as_timer(csp_rtc_t *ptRtc, csi_rtc_timer_e ePrd);

/**
  \brief       Config RTC alarm ture timer
  \param[in]   ptRtc      handle pointer of rtc register structure to operate
  \param[in]   eOut     rtc output
  \return      none
*/
void csi_rtc_set_alarm_out(csp_rtc_t *ptRtc, csi_rtc_osel_e eOut);

/** \brief evtrg source output config  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrgOut: rtc evtrg channel(0~1) 
 *  \param[in] eTrgSrc: rtc evtrg source
 *  \param[in] trg_prd: event count period 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_rtc_set_evtrg(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrgOut, csi_rtc_trgsrc_e eTrgSrc, uint8_t byTrgPrd);

/** \brief rtc evtrg output enable/disable
 * 
 *  \param[in] ptRtc: pointer of rtc register structure
 *  \param[in] eTrgOut: rtc evtrg out port (0~1)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_rtc_evtrg_enable(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrgOut, bool bEnable);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_RTC_H_ */
