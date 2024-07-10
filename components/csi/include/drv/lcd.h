/***********************************************************************//** 
 * \file  lcd.h
 * \brief  head file of csi lcd
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2021-12-03 <td>V0.0  <td>LQ&&ZJY     <td>initial
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_LCD_H_
#define _DRV_LCD_H_

#include <stdint.h>
#include <drv/common.h>

#include "csp.h"

#ifdef __cplusplus
extern "C" 
{
#endif

/**
 * \enum     csi_lcd_vol_e
 * \brief    lcd voltage select
 */
typedef enum {
	LCD_CLKSRC_RTC	= 0,
	LCD_CLKSRC_ISOSC,
} csi_lcd_clksrc_e;

/**
 * \enum     csi_lcd_vol_e
 * \brief    lcd voltage select
 */
typedef enum {
	LCD_VLCD_IN_2V6	= 0,
	LCD_VLCD_IN_2V8,
	LCD_VLCD_IN_3V0,
	LCD_VLCD_IN_3V2,
	LCD_VLCD_IN_3V4,
	LCD_VLCD_IN_3V6,
	LCD_VLCD_IN_3V8,
	LCD_VLCD_IN_4V0,
	LCD_VLCD_EX_VDD
} csi_lcd_vol_e;

/**
 * \enum     csi_lcd_duty_bias_e
 * \brief    lcd duty and bias select
 */
typedef enum {
	LCD_DUTY1_8_BIAS1_4	= 0,	//1/8 Duty, 1/4 Bias Mode, Use 8 COM port	
	LCD_DUTY1_6_BIAS1_4,		//1/6 Duty, 1/4 Bias Mode, Use 6 COM port
	LCD_DUTY1_5_BIAS1_3,		//1/5 Duty, 1/3 Bias Mode, Use 5 COM port
	LCD_DUTY1_4_BIAS1_3,		//1/4 Duty, 1/3 Bias Mode, Use 4 COM port
	LCD_DUTY1_3_BIAS1_3,		//1/3 Duty, 1/3 Bias Mode, Use 3 COM port
	LCD_DUTY1_3_BIAS1_2,		//1/3 Duty, 1/2 Bias Mode, Use 3 COM port
	LCD_DUTY1_2_BIAS1_2,		//1/2 Duty, 1/2 Bias Mode, Use 2 COM port
} csi_lcd_duty_bias_e;

/**
 * \enum     csi_lcd_dead_e
 * \brief    lcd dead phase select
 */
typedef enum {
	LCD_DEAD_NONE	= 0,
	LCD_DEAD_1PHASE,
	LCD_DEAD_2PHASE,
	LCD_DEAD_3PHASE,
	LCD_DEAD_4PHASE,
	LCD_DEAD_5PHASE,
	LCD_DEAD_6PHASE,
	LCD_DEAD_7PHASE
} csi_lcd_dead_e;

/**
 * \enum     csi_lcd_dead_e
 * \brief    lcd dead phase select
 */
typedef enum {
	LCD_DRVNET_PWLEV0	= 0,	//low resistance(RL), high power
	LCD_DRVNET_PWLEV1,			//low and normal resistance(RL and RM), normal power 
	LCD_DRVNET_PWLEV2,			//normal and high resistance(RH and RM), low power 
	LCD_DRVNET_PWLEV3,			//high resistance(RH), low power 
} csi_lcd_drvnet_e;

/**
 * \enum     csi_lcd_blink_md_e
 * \brief    lcd blink mode 
 */
typedef enum
{
	LCD_BLINK_DIS 		= 0x00ul,	//LCD none interrupt
	LCD_BLINK_SEG8_COM0 = 0x01uL, 	//SEG8 of COM0
	LCD_BLINK_SEG8   	= 0x02ul,	//SEG8 of all COM 
	LCD_BLINK_ALL   	= 0x03ul	//all SEG and COM
}csi_lcd_blink_md_e;

/**
 * \enum     csi_lcd_blink_md_e
 * \brief    lcd blink mode 
 */
typedef enum
{
	LCD_BLINK_FRE_DIV8 	= 0,		//Fblink = Fck_div/8, Fck_div = duty * Refresh rate	
	LCD_BLINK_FRE_DIV16, 	
	LCD_BLINK_FRE_DIV32,		
	LCD_BLINK_FRE_DIV64,
	LCD_BLINK_FRE_DIV128,		
	LCD_BLINK_FRE_DIV256,
	LCD_BLINK_FRE_DIV512,
	LCD_BLINK_FRE_F2,
}csi_lcd_blink_fre_e;

/**
 * \enum     csi_lcd_intsrc_e
 * \brief    lcd interrupt source 
 */
typedef enum
{
	LCD_INTSRC_NONE 	= (0x00ul << 0),		//LCD none interrupt
	LCD_INTSRC_SOF   	= (0x01ul << 1), 		//SOF interrupt
	LCD_INTSRC_UDD  	= (0x01ul << 3),		//UDD interrupt
}csi_lcd_intsrc_e;

/// \struct csi_lcd_config_t
/// \brief  lcd parameter configuration, open to users  
typedef struct {
	uint8_t		byClkSrc;	//lcd clk source 
	uint8_t     byVlcd;		//vlcd = in/ex
	uint8_t     byDutyBias;	//duty and bias select
	uint8_t		byDead;     //dead zone 
	uint8_t		byInt;		//Interrupt
	uint8_t		byDrvNet;	//lcd Drive network 
	uint8_t		byFreq;		//Refresh rate 
	bool		byDpEn;		//decoupling capacitor enable
} csi_lcd_config_t;

/**
  \brief       lcd init
  \param[in]   ptLcdBase	pointer of lcd register structure
  \param[in]   ptLcdCFg		pointer of lcd parameter structure
  \return      error code \ref csi_error_t
*/
csi_error_t csi_lcd_init(csp_lcd_t *ptLcdBase, csi_lcd_config_t *ptLcdCFg);

/** 
  \brief start(enable) lcd
  \param[in] ptLcdBase: pointer of lcd register structure
  \return none
 */ 
void csi_lcd_start(csp_lcd_t *ptLcdBase);

/** 
  \brief 	   enable/disable lcd interrupt 
  \param[in]   ptLcdBase	pointer of lcd register structure
  \param[in]   eIntSrc		lcd interrupt source
  \param[in]   bEnable		enable/disable interrupt
  \return 	   none
 */
void csi_lcd_int_enable(csp_lcd_t *ptLcdBase, csi_lcd_intsrc_e eIntSrc, bool bEnable);

/** 
  \brief lcd blink config
  \param[in] ptLcdBase		pointer of LCD reg structure
  \param[in] eBkMode		blink mode
  \param[in] eBkFre			blink frequence
  \param[in] hwF2Fre		eBkFre = LCD_BLINK_FRE_F2, blink frequence
  \return none
 */ 
csi_error_t csi_lcd_set_blink(csp_lcd_t *ptLcdBase, csi_lcd_blink_md_e eBkMode, csi_lcd_blink_fre_e eBkFre, uint16_t hwF2Fre);

/** 
  \brief set lcd display data
  \param[in] ptLcdBase: pointer of LCD reg structure.
  \param[in] pbyData: pointer of write lcd data buf  
  \param[in] byStaPos: SEGx starting position, 0~31
  \param[in] bySegNum: SEGx number, 1~32
  \return none
 */ 
void csi_lcd_write_data(csp_lcd_t *ptLcdBase, uint8_t *pbyData, uint8_t byStaPos, uint8_t bySegNum);

/** 
  \brief lcd gpio init
  \param[in] ptLcdBase: pointer of LCD reg structure.
  \param[in] wSegMask:  lcd SEG(0->31) Mask
  \param[in] byComMask: lcd COM(0->8) Mask
  \return none
 */ 
void csi_lcd_gpio_init(uint32_t wSegMask, uint8_t byComMask);


#ifdef __cplusplus
}
#endif

#endif /* _DRV_LCD_H_ */
