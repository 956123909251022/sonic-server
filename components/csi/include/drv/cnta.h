/***********************************************************************//** 
 * \file  cnta.h
 * \brief header file for cnta driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-9-09 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-11-23 <td>V0.1  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_CNTA_H_
#define _DRV_CNTA_H_

#include <drv/common.h>
#include <csp.h>

#ifdef __cplusplus
extern "C" {
#endif


typedef enum
{
	CNTA_CLK_DIV1		= 0,
	CNTA_CLK_DIV2, 
	CNTA_CLK_DIV4,  
	CNTA_CLK_DIV8  
}csi_cnta_ckdiv_e;

typedef enum
{
	CNTA_CNT_ONCE	= 0,	//once	
	CNTA_CNT_CONTINU		//continuous	
}csi_cnta_cntmode_e;

typedef enum
{
	CNTA_INTSRC_NONE		= (0x00ul << 0),
	CNTA_INTSRC_PENDL		= (0x01ul << 0),
	CNTA_INTSRC_PENDH		= (0x01ul << 1),     
	CNTA_INTSRC_PENDHL		= (0x03ul << 0)
}csi_cnta_intsrc_e;

/// \struct csi_conta_pwm_config_t
/// \brief  conta pwm parameter configuration
typedef struct {
	uint8_t		byStartLevel;
	uint8_t		byStopLevel;
	uint8_t     byInt;
	uint8_t		byDutyCycle;
	uint32_t	wFreq;
} csi_cnta_pwm_config_t;

typedef enum
{
	CNTA_POLAR_LOW	= 0,
    CNTA_POLAR_HIGH,       
	CNTA_STOP_LOW,	
    CNTA_STOP_HIGH   	    
}csi_cnta_pwmlev_e;

typedef enum
{
	CNTA_PWM_CARRIER	= 0,
	CNTA_PWM_ENVELOPE    
}csi_cnta_pwmout_e;

typedef enum
{
	CNTA_MATCH_CARR_DIS	= 0,
	CNTA_MATCH_CARR_CLR, 
	CNTA_MATCH_CARR_SET       
}csi_cnta_tcmatch_e;

typedef enum
{
	CNTA_PEND_CARR_DIS	= 0,
	CNTA_PEND_CARR_CLR, 
	CNTA_PEND_CARR_SET      
}csi_cnta_tcpend_e;

typedef enum
{
	CNTA_SW_DIS			= (0x00ul),
	CNTA_SW_EN			= (0x01ul)  
}csi_cnta_sw_updata_e;

typedef enum
{
	CNTA_HW_DIS				= (0x00ul),
	CNTA_HW_TCMATCH			= (0x01ul), 
	CNTA_HW_TCPEND			= (0x02ul),
    CNTA_HW_TCMATCH_TCPEND	= (0x03ul)   
}csi_cnta_hw_updata_e;

/** \brief initialize cnta data structure
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] wTimeOut:the timeout for cnta, pclk = 48000000hz，1-21845 us
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_timer_init(csp_cnta_t *ptCntaBase,uint32_t wTimeOut);

/** \brief start cnta
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return none
 */ 
void csi_cnta_start(csp_cnta_t *ptCntaBase);

/**
  \brief       Stop cnta
  \param[in]   ptCntaBase    pointer of cnta register structure
  \return      None
*/
void csi_cnta_stop(csp_cnta_t *ptCntaBase);

/**
  \brief       Get cnta datah value
  \param[in]   ptCntaBase    pointer of cnta register structure
  \return      cnta datah load value
*/
uint32_t csi_cnta_get_datah_value(csp_cnta_t *cnta);

/** \brief get cnta datal load value
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \return cnta datal load value
 */ 
uint32_t csi_cnta_get_datal_value(csp_cnta_t *ptCntaBase);

/** \brief set cnta count mode
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] eCntMode: cnta count mode, one pulse/continuous
 *  \return none
 */ 
void csi_cnta_count_mode(csp_cnta_t *ptCntaBase, csi_cnta_cntmode_e eCntMode);

/** \brief cnta pwm init 
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] ptCntaPwmCfg:point of pwm parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_cnta_pwm_init(csp_cnta_t *ptCntaBase,csi_cnta_pwm_config_t *ptCntaPwmCfg);

/** \brief updata cnta pwm freq para: (datah and datal value)
 * 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] hwDatah: rem ouput high time long 
 *  \param[in] hwDatal: rem ouput low time long  
 *  \param[in] eUpdata: data reg soft updata control
 *  \return none
 */
void csi_cnta_pwm_para_updata(csp_cnta_t *ptCntaBase, uint16_t hwDatah, uint16_t hwDatal, csi_cnta_sw_updata_e eUpdata);

/** \brief set cnta tc sync 
 *  \param[in] ptCntaBase: pointer of cnta register structure
 *  \param[in] tcpend_rem: carrieron control of tc pend end 
 *  \param[in] tcmatch_rem: carrieron control of tc pulse match 
 *  \param[in] hw_updata: cadata reg auto updata control
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_cnta_bt0_sync(csp_cnta_t *ptCntaBase, csi_cnta_tcpend_e eTcPendRem, csi_cnta_tcmatch_e eTcMatchRem,csi_cnta_hw_updata_e eHwUpdata);


//---------------------------------------------------------------------------------------------------------------------------------------------

#ifdef __cplusplus
}
#endif

#endif /* _DRV_TIMER_H_ */