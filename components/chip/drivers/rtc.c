/***********************************************************************//** 
 * \file  rtc.c
 * \brief  rtc driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2021-6-2 <td>V2.0 <td>WNN    <td>initial
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "csp.h"
#include "rtc.h"
#include "rtc_alg.h"
#include "irq.h"
#include "soc.h"
#include "board_config.h"

/* externs function--------------------------------------------------------*/

/* private function--------------------------------------------------------*/
static void apt_rtc_alm_set_time(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm, uint8_t wday, csi_rtc_ampm_e ePm, uint8_t byHor, uint8_t byMin,uint8_t bySec);
static csp_error_t apt_rtc_set_time(csp_rtc_t *ptRtc, csi_rtc_ampm_e ePm, uint8_t byHor, uint8_t byMin,uint8_t bySec);
static csp_error_t apt_rtc_set_date(csp_rtc_t *ptRtc, uint8_t byYear, uint8_t byMon, uint8_t byWday, uint8_t byDay);
csp_error_t apt_rtc_set_trgsrc(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csp_rtc_trgsrc_e eSrc);
csp_error_t apt_rtc_set_trgprd(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, uint8_t byPrd);

/* externs variablesr------------------------------------------------------*/

/* Private variablesr------------------------------------------------------*/
uint8_t s_hwRtcMsg = 0;

/** \brief Initialize RTC Interface. Initializes the resources needed for the RTC interface
 * 
 *  \param[in] ptRtc: pointer of rtc register structure
 *  \param[in] tConfig: rtc basic parameters \ref csi_rtc_config_t
 *  \return none
 */
csp_rtc_time_t tRtcAlarmTime;
void csi_rtc_init(csp_rtc_t *ptRtc, csi_rtc_config_t *tConfig)
{
    uint8_t byDiva = 0;
	uint16_t hwDivs = 0;
	csp_rtc_stop(ptRtc);
	soc_clk_enable(RTC_SYS_CLK);
	
	switch (tConfig->byClkSrc)
	{ 
		case (RTC_ISOSC):
			csi_isosc_enable();
			byDiva = 49;
			hwDivs = 269;
			break;
		case (RTC_ESOSC):
			csi_esosc_enable(ESOSC_VALUE);
			byDiva = 3;
			hwDivs = 4095;
//			hwDivs = 1;
			break;
		case (RTC_IMOSC_DIV4):
			switch(csp_get_imosc_fre(SYSCON))
			{
				case (0):	//5.556MHz
					byDiva = 124;
					hwDivs = 5555;
					break;
				case (1):	//4.194MHz
					byDiva = 124;
					hwDivs = 4193;
					break;
				case (2):	//2.097MHz
					byDiva = 124;
					hwDivs = 2096;
					break;
				case (3):	//131.072KHz
					byDiva = 3;
					hwDivs = 4095;
					break;
				default:
					break;
			}			
			break;
		case (RTC_EMOSC_DIV4):
			csi_emosc_enable(EMOSC_VALUE); //EMOSC_VALUE 值在board_config.h 文件定义
			byDiva = 99;                   //外部主晶振12MHz，如果不为12MHz请根据实际晶振大小再计算
			hwDivs = 14999;                //外部主晶振12MHz，如果不为12MHz请根据实际晶振大小再计算
			break;
		default:
			break;
	}
//	GPIOC0->CONLR = (GPIOC0->CONLR & 0xFFFFFFF0)|0x00000007;      //PC00   Pin37  调试DIVA、DIVS参数时用
//	SYSCON->OPT1  = (SYSCON->OPT1 & 0xFFFF80FF) | 6<<8 | 1<<12;   //CLO 输出RTCCLK ,不分频
	ptRtc->KEY = 0xCA53;
	RTC->CCR &= ~(0X01<<27);
	ptRtc->CCR = (ptRtc->CCR & (~RTC_CLKSRC_MSK) & (~RTC_DIVA_MSK)& (~RTC_DIVS_MSK)) | (tConfig->byClkSrc << RTC_CLKSRC_POS)|(byDiva << RTC_DIVA_POS)| (hwDivs << RTC_DIVS_POS) | (RTC_CLKEN);
	//ptRtc->KEY = 0x0;
	while((ptRtc->CCR & RTC_CLK_STABLE) == 0);
	
	//csp_rtc_ers_key(ptRtc);
	csp_rtc_rb_enable(ptRtc, ENABLE);
	csp_rtc_set_fmt(ptRtc, tConfig->byFmt);
	csp_rtc_alm_enable(ptRtc, RTC_ALMB, DISABLE);
	csp_rtc_alm_enable(ptRtc, RTC_ALMA, DISABLE);
	
	csp_rtc_int_enable(ptRtc, RTC_INTSRC_ALMA|RTC_INTSRC_ALMB|RTC_INTSRC_CPRD|RTC_INTSRC_TRGEV0|RTC_INTSRC_TRGEV1, DISABLE);
	csp_rtc_clr_isr(ptRtc, RTC_INTSRC_ALMA|RTC_INTSRC_ALMB|RTC_INTSRC_CPRD|RTC_INTSRC_TRGEV0|RTC_INTSRC_TRGEV1);
	
	csi_irq_enable((uint32_t *)ptRtc);	
}

/** \brief Read back function enable control. To save current, disable read back.
 * 
 *  \param[in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in] bEnable: ENABLE/DISABLE    
 *  \return none
 */
void csi_rtc_rb_enable(csp_rtc_t *ptRtc, bool bEnable)
{
	csp_rtc_rb_enable(ptRtc, bEnable);
}

/** \brief To set/change RTC format
 * 
 *  \param[in] ptRtc: pointer of rtc register structure pointer of rtc register structure to operate
 *  \param[in] eFmt: \ref rtc_fmt_e    
 *  \return  none
 */
void csi_rtc_change_fmt(csp_rtc_t *ptRtc,  rtc_fmt_e eFmt)
{
	csp_rtc_set_fmt(ptRtc, eFmt);
}

/** \brief To start RTC 
 * 
 *  \param[in] ptRtc: pointer of rtc register structure pointer of rtc register structure to operate
 *  \return none
 */
void csi_rtc_start(csp_rtc_t *ptRtc)
{
	csp_rtc_run(ptRtc);
}

/** \brief To stop RTC 
 * 
 *  \param[in] ptRtc: pointer of rtc register structure pointer of rtc register structure to operate
 *  \return none
 */
void csi_rtc_stop(csp_rtc_t *ptRtc)
{
	csp_rtc_stop(ptRtc);
}

/** \brief Set system date and run after setting
 * 
 *  week day will be calculated in this function
 *  \param[in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in] ptRtcTime: pointer to rtc time
 *  \return error code \ref csi_error_t
*/
csi_error_t csi_rtc_set_time(csp_rtc_t *ptRtc, csi_rtc_time_t *ptRtcTime)
{   
    csi_error_t ret = CSI_OK;
		
	do {
//		if (csp_rtc_get_fmt(ptRtc) == RTC_12FMT && ptRtcTime->iHour>12) {
//			ret = CSI_ERROR;
//			break;
//		}
		
		ret = (csi_error_t) clock_check_tm_ok((const struct tm *)ptRtcTime);
        if (ret < CSI_OK) {
            break;
		
        }
		csp_rtc_stop(ptRtc);
		
	
		
		ptRtcTime->iWday = get_week_by_date((struct tm *)ptRtcTime);
		
		
		apt_rtc_set_date(ptRtc, ptRtcTime->iYear, ptRtcTime->iMon, ptRtcTime->iWday, ptRtcTime->iMday);
		
	
		if ((ptRtcTime->iHour == 12) && (csp_rtc_get_fmt(ptRtc) == RTC_FMT_24))
			ret =  apt_rtc_set_time(ptRtc, RTC_PM, ptRtcTime->iHour, ptRtcTime->iMin,ptRtcTime->iSec);
		else
			ret =  apt_rtc_set_time(ptRtc, ptRtcTime->iPm, ptRtcTime->iHour, ptRtcTime->iMin,ptRtcTime->iSec);
		
		
		if (ret < CSI_OK) {
            break;
        }
	}while(0);
	
	//csp_rtc_run(ptRtc);

	return ret;
}

/** \brief Config RTC alarm
 * 
 *  \param[in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in] eAlm: RTC_ALMA/RTC_ALMB
 *  \param[in] ptAlmTime: alarm time(s) 
 *  \param[in] byMode:	0: day       hour min sec
						1: weekday   hour min sec
						2:           hour min sec

 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_rtc_set_alarm(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm, csi_rtc_time_t *ptAlmTime)
{ 	
	bool bHmsk = 0;
	bool bMmsk = 0;
	bool bSmsk = 0;
	
	csi_rtc_ampm_e bFmt = ptAlmTime->iPm;
	
	if(eAlm > 1)
		return CSI_ERROR;

	if (ptAlmTime -> iHour == 0xff) {
		bHmsk = 1;
		ptAlmTime->iHour = 0;
	}
	if (ptAlmTime -> iMin == 0xff) {
		bMmsk = 1;
		ptAlmTime->iMin = 0;
	}
	if (ptAlmTime -> iSec == 0xff) {
		bSmsk = 1;	
		ptAlmTime->iSec = 0;
	}
	
	if(csp_rtc_get_fmt(RTC) == RTC_FMT_24) {
		if (ptAlmTime -> iHour == 12) 
			bFmt = RTC_PM;
		else
			bFmt = RTC_AM;
	}
	switch (eAlm)
	{
		case (RTC_ALMA): 	csp_rtc_clr_isr(ptRtc, RTC_INTSRC_ALMA);
							csp_rtc_int_enable(ptRtc, RTC_INTSRC_ALMA, ENABLE);
							break;
		case (RTC_ALMB):	csp_rtc_clr_isr(ptRtc, RTC_INTSRC_ALMB);
							csp_rtc_int_enable(ptRtc, RTC_INTSRC_ALMB, ENABLE);
							break;
		default:
			return CSI_ERROR;
	}
	
	csp_rtc_alm_enable(ptRtc, eAlm, DISABLE);
	apt_rtc_alm_set_time(ptRtc, eAlm, ptAlmTime->iWday, bFmt,ptAlmTime->iHour, ptAlmTime->iMin,ptAlmTime->iSec);
	csp_rtc_alm_set_mode(ptRtc, eAlm, bHmsk, bMmsk, bSmsk); 
	csp_rtc_alm_enable(ptRtc, eAlm, ENABLE);
	
	return CSI_OK;
}

/** \brief Cancel the rtc alarm, keep last alarm time and mode setting
 * 
 *  \param[in] ptRtc: pointer of rtc register structure to operate
 *  \param[in] eAlm: RTC_ALMA/RTC_ALMB
 *  \return none
 */
void csi_rtc_cancel_alarm(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm)
{
    
	csp_rtc_alm_enable(ptRtc, eAlm, DISABLE);
	switch (eAlm)
	{
		case (RTC_ALMA): 	csi_rtc_int_enable(ptRtc, RTC_INTSRC_ALMA, DISABLE);
							csp_rtc_clr_isr(ptRtc, RTC_INTSRC_ALMA);
							break;
		case (RTC_ALMB):	csi_rtc_int_enable(ptRtc, RTC_INTSRC_ALMB, DISABLE);
							csp_rtc_clr_isr(ptRtc, RTC_INTSRC_ALMB);
							break;
		default: break;
	}
}

/** \brief Config RTC alarm ture timer
 * 
 *  \param[in][in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in][in] eOut: rtc output
 *  \return none
 */
void csi_rtc_set_alarm_out(csp_rtc_t *ptRtc, csi_rtc_osel_e eOut)
{	
	csp_rtc_set_osel(ptRtc, eOut);
}

/** \brief use rtc as a timer
 * 
 *  \param[in] ptRtc: pointer of rtc register structure to operate
 *  \param[in] ePrd: time interval of the timer.
 *  \return none
 */
void csi_rtc_start_as_timer(csp_rtc_t *ptRtc, csi_rtc_timer_e ePrd)
{	
	
	csp_rtc_set_cprd(ptRtc, ePrd);
	csi_rtc_int_enable(ptRtc, RTC_INTSRC_CPRD , ENABLE);
	//csp_rtc_run(ptRtc);
}

/** \brief RTC interrupt enable/disable
 *  \param[in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in] eIntSrc: interrupt source	
 *  \param[in] bEnable: ENABLE/DISABLE 
 *  \return  none
 */
void csi_rtc_int_enable(csp_rtc_t *ptRtc, csi_rtc_intsrc_e eIntSrc, bool bEnable)
{
	csp_rtc_int_enable(ptRtc, eIntSrc, bEnable);	
	
	if (bEnable) {
		csi_irq_enable((uint32_t *)ptRtc);
	}
	else {
		if (eIntSrc == csp_rtc_get_imcr(ptRtc)) {
			csi_irq_disable((uint32_t *)ptRtc);
		}
	}
}


/** \brief get the RTC is interrupted
 * 
 *  \param[in] eIntSrc: RTC interrupt source
 *  \param[in] bClrEn: clean interrupt
 *  \return RTC interrupt register(RTC->MISR) value 
 */
bool csi_rtc_get_int_msg(csi_rtc_intsrc_e eIntSrc,bool bClrEn)
{	
	if(eIntSrc & s_hwRtcMsg)
	{
		if(bClrEn)
		{
			s_hwRtcMsg &= ~eIntSrc;
		}
		return true;
	}
	else
	{
		return false;
	}
}

/** \brief clean the RTC is interrupted
 * 
 *  \param[in] eIntSrc: interrupt source
 *  \return none
 */
void csi_rtc_clr_int_msg(csi_rtc_intsrc_e eIntSrc)
{	
	s_hwRtcMsg &= ~eIntSrc;	
}

/** \brief Get system time
 * 
 *  \param[in] ptRtc: handle pointer of rtc register structure to operate
 *  \param[in] ptRtctime: pointer to rtc time
 *  \return none
 */
void csi_rtc_get_time(csp_rtc_t *ptRtc, csi_rtc_time_t *ptRtctime)
{
	ptRtctime->iSec = csp_rtc_read_sec(ptRtc);
	ptRtctime->iMin = csp_rtc_read_min(ptRtc);
	ptRtctime->iHour = csp_rtc_read_hour(ptRtc);
	ptRtctime->iPm = csp_rtc_read_pm(ptRtc);
	ptRtctime->iMday = csp_rtc_read_mday(ptRtc);
	ptRtctime->iWday = csp_rtc_read_wday(ptRtc);
	ptRtctime->iMon = csp_rtc_read_mon(ptRtc);
	ptRtctime->iYear = csp_rtc_read_year(ptRtc);
}

/** \brief Get alarm remaining time
 * 
 *  \param[in] ptRtc: pointer of rtc register structure to operate
 *  \param[in] eAlm: RTC_ALMA/RTC_ALMB
 *  \return the remaining time(s)
 */
uint32_t csi_rtc_get_alarm_remaining_time(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm)
{
    volatile csi_rtc_time_t tCurrentTime ;
	volatile csi_rtc_time_t tAlmTime;
	uint32_t wCurrentTime, wAlmTime;


	tCurrentTime.iMday = csp_rtc_read_mday(ptRtc); 
	tCurrentTime.iWday = csp_rtc_read_wday(ptRtc); 
	tCurrentTime.iHour = csp_rtc_read_hour(ptRtc);
	tCurrentTime.iMin = csp_rtc_read_min(ptRtc); 
	tCurrentTime.iSec = csp_rtc_read_sec(ptRtc); 

//	tAlmTime.iMday = csp_rtc_alm_read_mday(ptRtc, eAlm); 
	tAlmTime.iWday = csp_rtc_alm_read_wday(ptRtc, eAlm); 
	tAlmTime.iHour = csp_rtc_alm_read_hour(ptRtc, eAlm);
	tAlmTime.iMin = csp_rtc_alm_read_min(ptRtc, eAlm); 
	tAlmTime.iSec = csp_rtc_alm_read_sec(ptRtc, eAlm); 
	

	if (csp_rtc_alm_read_wdsel(ptRtc, eAlm) == 1) {
		wCurrentTime = tCurrentTime.iWday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
		wAlmTime = tAlmTime.iWday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
		if(wAlmTime < wCurrentTime)
			return (7*24*3600 - wCurrentTime + wAlmTime);
	}
	else {
		wCurrentTime = tCurrentTime.iMday * 86400 + tCurrentTime.iHour * 3600 + tCurrentTime.iMin * 60 + tCurrentTime.iSec;
		wAlmTime = tAlmTime.iMday * 86400 + tAlmTime.iHour * 3600 + tAlmTime.iMin * 60 + tAlmTime.iSec;
		if(wAlmTime < wCurrentTime)
			return CSI_UNSUPPORTED;	
	}
	
	
	return (wAlmTime - wCurrentTime);
}


/** \brief evtrg source output config  
 * 
 *  \param[in] ptRtc: RTC handle to operate
 *  \param[in] eTrgOut: rtc evtrg channel(0~1) 
 *  \param[in] eTrgSrc: rtc evtrg source
 *  \param[in] byTrgPrd: event count period 
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_rtc_set_evtrg(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrgOut, csi_rtc_trgsrc_e eTrgSrc, uint8_t byTrgPrd)
{
	
	csi_error_t ret = CSI_OK;
	
	if (apt_rtc_set_trgsrc(ptRtc, eTrgOut, eTrgSrc)<0)
		return CSI_ERROR;
	if (apt_rtc_set_trgprd(ptRtc, eTrgOut, byTrgPrd)<0)
		return CSI_ERROR;
	return ret;
}
/** \brief rtc evtrg output enable/disable
 * 
 *  \param[in] ptRtc: pointer of rtc register structure
 *  \param[in] eTrgOut: rtc evtrg out port (0~1)
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_rtc_evtrg_enable(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrgOut, bool bEnable)
{
	if(eTrgOut <= RTC_TRGOUT1)
		ptRtc->EVTRG = (ptRtc->EVTRG & ~RTC_TRGOE_MSK(eTrgOut)) | (bEnable << RTC_TRGOE_POS(eTrgOut));
	else
		return CSI_ERROR;
		
	return CSI_OK;
}
//*****************************************************************************//


uint8_t  apt_dec2bcd(uint32_t wData)
{	
	return (((wData / 10) << 4) | (wData % 10));
}


static csp_error_t apt_rtc_set_date(csp_rtc_t *ptRtc, uint8_t byYear, uint8_t byMon, uint8_t byWday, uint8_t byDay)
{
	uint8_t byVal;
	csp_error_t ret = CSP_SUCCESS;
	if (csp_rtc_is_running(ptRtc))
		return CSP_FAIL;
		
	csp_rtc_wr_key(ptRtc);
	byVal = apt_dec2bcd(byYear);
	csp_rtc_set_date_year(ptRtc, byVal);
	byVal = apt_dec2bcd(byMon);
	csp_rtc_set_date_mon(ptRtc, byVal);
	byVal = apt_dec2bcd(byWday);
	csp_rtc_set_date_wday(ptRtc, byVal);
	byVal = apt_dec2bcd(byDay);
	csp_rtc_set_date_day(ptRtc, byVal);	
	csp_rtc_ers_key(ptRtc);
	
	return ret;
}


static csp_error_t apt_rtc_set_time(csp_rtc_t *ptRtc, csi_rtc_ampm_e ePm, uint8_t byHor, uint8_t byMin,uint8_t bySec)
{
	uint8_t byVal;
	csp_error_t ret = CSP_SUCCESS;

	if (csp_rtc_is_running(ptRtc))
		return CSP_FAIL;
			
//	if (ePm == RTC_AM && byHor > 12)
//		ret = CSP_FAIL;
//	else {
	
		csp_rtc_wr_key(ptRtc);
		byVal = apt_dec2bcd(byHor);
		csp_rtc_set_time_hour(ptRtc, ePm, byVal);
		byVal = apt_dec2bcd(byMin);
		csp_rtc_set_time_min(ptRtc, byVal);
		byVal = apt_dec2bcd(bySec);
		csp_rtc_set_time_sec(ptRtc, byVal);
		csp_rtc_ers_key(ptRtc);
//	}
	
	return ret;
}


static void apt_rtc_alm_set_time(csp_rtc_t *ptRtc, csi_rtc_alarm_e eAlm, uint8_t wday, csi_rtc_ampm_e ePm, uint8_t byHor, uint8_t byMin,uint8_t bySec)
{
	uint8_t byVal;
		
	csp_rtc_wr_key(ptRtc);
		
	csp_rtc_alm_set_wday(ptRtc, eAlm, wday);
	byVal = apt_dec2bcd(byHor);
	csp_rtc_alm_set_hour(ptRtc, eAlm, ePm, byVal);
	byVal = apt_dec2bcd(byMin);
	csp_rtc_alm_set_min(ptRtc, eAlm, byVal);
	byVal = apt_dec2bcd(bySec);
	csp_rtc_alm_set_sec(ptRtc, eAlm, byVal);
	
	csp_rtc_ers_key(ptRtc);
}



csp_error_t apt_rtc_set_trgsrc(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, csp_rtc_trgsrc_e eSrc)
{
	if(eTrg == RTC_TRGOUT0)
		ptRtc -> EVTRG = (ptRtc->EVTRG & ~(RTC_TRGSEL0_MSK)) | eSrc | RTC_TRGSEL0_EN;
	else if(eTrg == RTC_TRGOUT1)
		ptRtc -> EVTRG = (ptRtc->EVTRG & ~(RTC_TRGSEL1_MSK)) | (eSrc << RTC_TRGSEL1_POS) | RTC_TRGSEL1_EN;
	else
		return CSP_FAIL;
	return CSP_SUCCESS;
}


csp_error_t apt_rtc_set_trgprd(csp_rtc_t *ptRtc, csi_rtc_trgout_e eTrg, uint8_t byPrd)
{	
	if(eTrg == RTC_TRGOUT0)
		ptRtc -> EVPS = (ptRtc->EVPS & ~(RTC_TRGEV0PRD_MSK)) | byPrd ;
	else if(eTrg == RTC_TRGOUT1)
		ptRtc -> EVPS = (ptRtc->EVPS & ~(RTC_TRGSEL1_MSK)) | (byPrd << RTC_TRGEV1PRD_POS);
	else
		return CSP_FAIL;
		
	return CSP_SUCCESS;
}


