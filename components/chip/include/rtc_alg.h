/***********************************************************************//** 
 * \file  rtc_alg.h
 * \brief  headfile for RTC
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-4 <td>V0.0  <td>WNN   <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _RTC_ALG_H_
#define _RTC_ALG_H_

#include <time.h>
#include <drv/rtc.h>
//#include <csi_config.h>

#ifdef    __cplusplus
extern "C" {
#endif

/* ########################## Date Clock Default value ############################## */

#define RTC_TIME_BASE_YEAR                          (2000)                                 //< Year,      Effective range[1970,2099]

#define RTC_TIME_MAX_VAL_YEAR                       (159)                                  ///< Year,     Maximum value
#define RTC_TIME_MAX_VAL_MON                        (12)                                   ///< Month,    Maximum value
#define RTC_TIME_MAX_VAL_DAY                        (31)                                   ///< Day,      Maximum value
#define RTC_TIME_MAX_VAL_HOUR                       (23)                                   ///< Hour,     Maximum value
#define RTC_TIME_MAX_VAL_MIN                        (59)                                   ///< Minute,   Maximum value
#define RTC_TIME_MAX_VAL_SEC                        (59)                                   ///< Second,   Maximum value

#define RTC_TIME_MIN_VAL_YEAR                       (0)                                   ///< Year,     Minimum value
#define RTC_TIME_MIN_VAL_MON                        (1)                                    ///< Month,    Minimum value    
#define RTC_TIME_MIN_VAL_DAY                        (1)                                    ///< Day,      Minimum value
#define RTC_TIME_MIN_VAL_HOUR                       (0)                                    ///< Hour,     Minimum value
#define RTC_TIME_MIN_VAL_MIN                        (0)                                    ///< Minute,   Minimum value    
#define RTC_TIME_MIN_VAL_SEC                        (0)                                    ///< Second,   Minimum value  

#define RTC_TIME_DEFAULT_VAL_YEAR                   (70)                                   ///< Year,     Dafault min value
#define RTC_TIME_DEFAULT_VAL_MON                    (1)                                    ///< Month,    Dafault min value
#define RTC_TIME_DEFAULT_VAL_DAY                    (1)                                    ///< Day,      Dafault min value
#define RTC_TIME_DEFAULT_VAL_HOUR                   (0)                                    ///< Hour,     Dafault min value
#define RTC_TIME_DEFAULT_VAL_MIN                    (0)                                    ///< Minute,   Dafault min value
#define RTC_TIME_DEFAULT_VAL_SEC                    (0)                                    ///< Second,   Dafault min value

#define RTC_TIME_TEMP_LEAP_YEAR                     (72)                                   ///< Year,     Dafault min value
#define RTC_TIME_TEMP_NONLEAP_YEAR                  (70)                                   ///< Year,     Dafault min value

/**
 * \brief Determine if it is a leap year
 * 0 - this is not leapyear 
 * 1  - this is leapyear 
*/
#define RTC_IS_LEAPYEAR(_year_)                  (((_year_) % 400) ? (((_year_) % 100) ? (((_year_) % 4) ? (int)0 : (int)1) : (int)0) : (int)1)

#define RTC_SEC_PER_MIN                          ((time_t)60)
#define RTC_SEC_PER_HOUR                         ((time_t)60 * RTC_SEC_PER_MIN)
#define RTC_SEC_PER_DAY                          ((time_t)24 * RTC_SEC_PER_HOUR)

#define RTC_PARA_UNSUPPORT                       (0)

/**
 * week:                 tm_wday:
 * 0 is Monday           0 is Sunday
 * 1 is Tuesday          1 is Monday
 * 2 is Wednesday        2 is Tuesday
 * 3 is Thursday         3 is Wednesday
 * 4 is Friday           4 is Thursday
 * 5 is Saturday         5 is Friday
 * 6 is Sunday           6 is Saturday
*/
#define RTC_CALCULATE_WEEK(_week_)               ((_week_ == 6) ? 0 : (_week_ + 1))

/**
  \brief   calculate overall seconds from the min year supported by HW RTC
  \param   tp      handler to operate
  \return  time_t	
*/
time_t   mktime(struct tm *tp);

/**
  \brief   calculate days before some specific month of one year
  \param   timer      pointer to 
  \param   result	  pointer to 
  \return  result	  pointer to result time structure
*/
struct tm *gmtime_r(const time_t *timer, struct tm *result);

/**
  \brief   calculate days before some specific month of one year
  \param   month      month
  \param   leapyear	  0: non leap year; 1: leap year

  \return  retval
*/
int      clock_daysbeforemonth(int month, int leapyear);

/**
  \brief   calculate past days since the min year supported by HW RTC
  \param   month      month
  \param   year	      year
  \param   day	      day
  \return  days		
*/
time_t   clock_calendar2utc(int year, int month, int day);

/**
  \brief   calculate date according to past days since the min year supported by HW RTC
  \param   days       past days
  \param   year	      pointer to result year
  \param   month      pointer to result month
  \param   day	      pointer to result day
  \return  days		
*/
void     clock_utc2calendar(time_t days, int *year, int *month, int *day);

/**
  \brief   check if any member of the input time structure is valid
  \param   rtctime    pointer to time structure
  \return  ret    0: pass; 1: OK
*/
int32_t  clock_check_tm_ok(const struct tm *rtctime);

/**
  \brief   update year_to_secs(cache), return total secs
  \param   rtctime    pointer to time structure
  \return  total_sec  total seconds
*/
int32_t clock_update_basetime(const struct tm *rtctime);

/**
  \brief   calculate weekday from date
  \param   rtctime    pointer to time structure
  \return             weekday
*/
int get_week_by_date(const struct tm *rtctime);

#ifdef __cplusplus
}
#endif

#endif /* _RTC_ALG_H_ */
