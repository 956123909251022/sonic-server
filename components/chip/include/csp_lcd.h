/***********************************************************************//** 
 * \file  csp_lcd.h
 * \brief  LCD description and static inline functions at register level 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-12-01 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-12-03 <td>V0.1  <td>LQ   <td>modify
 * </table>
 * *********************************************************************
*/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _CSP_LCD_H
#define _CSP_LCD_H

/* Includes ------------------------------------------------------------------*/
#include <soc.h>

/// \struct csp_lcd_t
/// \brief LCD Reg Description
typedef struct
{
	__IM  uint32_t  IDR;		//0x0000	ID Register                      
	__IOM uint32_t  CEDR;      	//0x0004    Clock Enable/Disable Register    
	__OM  uint32_t  SRR;       	//0x0008	Software Reset Register          
	__IOM uint32_t  CR;        	//0x000C    Control Register                 
	__IOM uint32_t  CDR;       	//0x0010    Clock Divider Register           
	__IOM uint32_t  MOD;       	//0x0014    LCD mode control register        
	__IOM uint32_t  SR;        	//0x0018    LCD status register              
	__IOM uint32_t  ICR;       	//0x001C    LCD Interrupt Clear Register     
	__IOM uint32_t  IER;       	//0x0020    LCD Interrupt Enable Register 
	__IM  uint32_t	MISR;       //0x0024    LCD Interrupt Flag Register
	__IOM uint32_t  CR2;        //0x0028    LCD Control Register2
	__IM  uint32_t	RSVD[245]; 	//0x002c->0x03fc 
	__IOM uint32_t  DMR[32];   	//0x0000	LCD Display Memory Register   
} csp_lcd_t;

/******************************************************************************
* IDR: LCD ID Code Register 
******************************************************************************/
#define LCD_IDCODE_POS  	(0)				//LCD IP IDCODE  						  
#define LCD_IDCODE_MSK  	(0xFFFFFFFF << LCD_IDCODE_POS)	

/******************************************************************************
* CEDR: LCD CLK Enable Control Registers
******************************************************************************/
#define LCD_CLKEN			(0x01ul << 0)	//LCD Clock Enable 
     
#define LCD_CLKSRC_POS		(1)				//LCD Clock Select
#define LCD_CLKSRC_MSK		(0x01ul << LCD_CLKSRC_POS)	
typedef enum{
	LCD_CLK_RTC		= 0,
	LCD_CLK_ISOSC
}lcd_clksrc_e;    
   
/******************************************************************************
* SRR: LCD Software Reset Registers
******************************************************************************/
#define LCD_SWRST			(0x01ul << 0)   //LCD Software Reset

/******************************************************************************
* CR: LCD Control Registers
******************************************************************************/
#define LCD_BIAS_RDY		(0x01ul << 31)	//Bias Module Ready
#define LCD_PUMP_RDY		(0x01ul << 30)	//Booster Pump Ready

#define LCD_EN_POS			(0)				//LCD Enable/Disable
#define LCD_EN_MSK			(0x01ul << LCD_EN_POS)
typedef enum{
	LCD_DIS			= 0,
	LCD_EN
}lcd_en_e;

#define LCD_DISC_POS		(1)				//LCD Display Control
#define LCD_DISC_MSK		(0x03ul << LCD_DISC_POS)
typedef enum{
	LCD_DISC_DIS	= 0,
	LCD_DISC_EN,
	LCD_DISC_NOR,
	LCD_DISC_NULL
}lcd_disc_e;

#define LCD_VLCD_POS		(3)			//VLCD Input Control
#define LCD_VLCD_MSK		(0x01ul << LCD_VLCD_POS)
typedef enum{
	LCD_VLCD_IN	= 0,
	LCD_VLCD_EX,
}lcd_vlcd_ex_e;

#define LCD_PUMP_POS		(4)				//Booster Pump Control
#define LCD_PUMP_MSK		(0x01ul << LCD_PUMP_POS)
typedef enum{
	LCD_PUMP_DIS	= 0,
	LCD_PUMP_EN,
}lcd_pump_e;

#define LCD_BIAS_POS		(5)				//LCD Bias Module Control
#define LCD_BIAS_MSK		(0x01ul << LCD_BIAS_POS)
typedef enum{
	LCD_BIAS_DIS	= 0,
	LCD_BIAS_EN,
}lcd_bias_e;

#define LCD_BP_POS			(6)				//Decoupling Capacitance Connet
#define LCD_BP_MSK			(0x01ul << LCD_BP_POS)
typedef enum{
	LCD_BP_DIS		= 0,
	LCD_BP_EN,
}lcd_bp_e;

#define LCD_DBSEL_POS		(8)				//Duty and Bias Mode
#define LCD_DBSEL_MSK		(0x07ul << LCD_DBSEL_POS)
typedef enum{
	LCD_D1_8_B1_4	= 0,	//1/8 Duty, 1/4 Bias Mode, Use 8 COM port	
	LCD_D1_6_B_4,			//1/6 Duty, 1/4 Bias Mode, Use 6 COM port
	LCD_D1_5_B1_3,			//1/5 Duty, 1/3 Bias Mode, Use 5 COM port
	LCD_D1_4_B1_3,			//1/4 Duty, 1/3 Bias Mode, Use 4 COM port
	LCD_D1_3_B1_3,			//1/3 Duty, 1/3 Bias Mode, Use 3 COM port
	LCD_D1_3_B1_2,			//1/3 Duty, 1/2 Bias Mode, Use 3 COM port
	LCD_D1_2_B1_2,			//1/2 Duty, 1/2 Bias Mode, Use 2 COM port
}lcd_dbsel_e;

#define LCD_BLINK_POS			(11)		//Blink Mode
#define LCD_BLINK_MSK			(0x03ul << LCD_BLINK_POS)
typedef enum{
	LCD_BK_DIS	= 0,
	LCD_BK_SEG8_COM0,
	LCD_BK_SEG8_COM,
	LCD_BK_ALL,
}lcd_blink_e;

#define LCD_BLINKF_POS			(13)		//Blink Frequency
#define LCD_BLINKF_MSK			(0x07ul << LCD_BLINKF_POS)
typedef enum{
	LCD_FRE_DIV8	= 0,
	LCD_FRE_DIV16,
	LCD_FRE_DIV32,
	LCD_FRE_DIV64,
	LCD_FRE_DIV128,
	LCD_FRE_DIV256,
	LCD_FRE_DIV512,
	LCD_FRE_F2
}lcd_blink_fre_e;

#define LCD_HDPULSE_POS			(16)		//Pulse High Enable
#define LCD_HDPULSE_MSK			(0x01ul << LCD_HDPULSE_POS)
typedef enum{
	LCD_HDPULSE_DIS	= 0,
	LCD_HDPULSE_EN
}lcd_hdpulse_e;

#define LCD_HDCYCLE_POS			(17)		//Pulse High Cycle
#define LCD_HDCYCLE_MSK			(0x07ul << LCD_HDCYCLE_POS)
typedef enum{
	LCD_HDCYCLE_0	= 0,
	LCD_HDCYCLE_1,
	LCD_HDCYCLE_2,
	LCD_HDCYCLE_3,
	LCD_HDCYCLE_4,
	LCD_HDCYCLE_5,
	LCD_HDCYCLE_6,
	LCD_HDCYCLE_7
}lcd_hdcycle_e;

#define LCD_DEAD_POS			(20)		//Dead Space Continue Timer
#define LCD_DEAD_MSK			(0x07ul << LCD_DEAD_POS)
typedef enum{
	LCD_DEAD_NULL	= 0,
	LCD_DEAD_1,
	LCD_DEAD_2,
	LCD_DEAD_3,
	LCD_DEAD_4,
	LCD_DEAD_5,
	LCD_DEAD_6,
	LCD_DEAD_7
}lcd_dead_e;

#define LCD_HD_POS				(28)		//High Driver Resistance
#define LCD_HD_MSK				(0x01ul << LCD_HD_POS)
typedef enum{
	LCD_HD_DIS		= 0,
	LCD_HD_EN,
}lcd_hd_e;

/******************************************************************************
* CDR: LCD Clock Division Registers
******************************************************************************/
#define LCD_CDIV_POS			(0)			//LCD Clock Division Value
#define LCD_CDIV_MSK			(0x07ul << LCD_CDIV_POS)
#define LCD_CDIV(val)  			((val) & 0x07) 

#define LCD_CDC_POS				(7)			//LCD Clock Division Control
#define LCD_CDC_MSK				(0x01ul << LCD_CDC_POS)
typedef enum{
	LCD_CDC_DIS		= 0,
	LCD_CDC_EN,
}lcd_cdc_e;

#define LCD_CPRE_POS			(8)			//LCD Clock Division Value
#define LCD_CPRE_MSK			(0xFFFFul << LCD_CPRE_POS)
#define LCD_CPRE(val)  			(((val) & 0xFFFF) << LCD_CPRE_POS)

/******************************************************************************
* MOD: LCD Mode Registers
******************************************************************************/
#define LCD_VLCD_INTER_POS			(0)			//LCD VLCD Voltage Select
#define LCD_VLCD_INTER_MSK			(0x07ul << LCD_VLCD_INTER_POS)
typedef enum{
	LCD_VLCD_2V6	= 0,
	LCD_VLCD_2V8,
	LCD_VLCD_3V0,
	LCD_VLCD_3V2,
	LCD_VLCD_3V4,
	LCD_VLCD_3V6,
	LCD_VLCD_3V8,
	LCD_VLCD_4V0,
	LCD_VLCD_EXTVDD	
}lcd_vlcd_e;

/******************************************************************************
* SR: LCD Status Registers
******************************************************************************/		
typedef enum{
	LCD_SOF_SR		= (0x01ul << 1),
	LCD_UDR_SR		= (0x01ul << 2),
	LCD_UDD_SR		= (0x01ul << 3)
}lcd_sr_e;

/******************************************************************************
* ICR : LCD Interrupt Clear Registers/ Enable Registers/Status Registers
******************************************************************************/
typedef enum{
	LCD_NONE_INT    = (0x00ul << 0),
	LCD_SOF_INT		= (0x01ul << 1),
	LCD_UDD_INT		= (0x01ul << 3)
}lcd_int_e; 

/******************************************************************************
* CR2 : LCD Control Registers2
******************************************************************************/
typedef enum{
	LCD_UDR_EN   = (0x01ul << 0)
}lcd_udr_e;

#define LCD_BLINKF2_POS			(16)			
#define LCD_BLINKF2_MSK			(0xFFFul << LCD_BLINKF2_POS)
#define LCD_BLINKF2(val)  		(((val) & 0xFFF) << LCD_BLINKF2_POS)

/******************************************************************************
* DMRx : LCD Display Registers
******************************************************************************/
typedef enum{
	LCD_SEG_COM0	= (0x01ul << 0),
	LCD_SEG_COM1	= (0x01ul << 1),
	LCD_SEG_COM2	= (0x01ul << 2),
	LCD_SEG_COM3	= (0x01ul << 3),
	LCD_SEG_COM4	= (0x01ul << 4),
	LCD_SEG_COM5	= (0x01ul << 5),
	LCD_SEG_COM6	= (0x01ul << 6),
	LCD_SEG_COM7	= (0x01ul << 7)
}lcd_com_e; 

/******************************************************************************
********************** LCD inline Functions Declaration ***********************
******************************************************************************/
static inline void csp_lcd_soft_rst(csp_lcd_t *ptLcdBase)
{
	ptLcdBase->SRR = LCD_SWRST;
}

static inline void csp_lcd_clk_en(csp_lcd_t *ptLcdBase, lcd_clksrc_e eClkSrc)
{
	ptLcdBase->CEDR = (ptLcdBase->CEDR & ~ LCD_CLKSRC_MSK) | (eClkSrc << LCD_CLKSRC_POS) | LCD_CLKEN;
}

static inline void csp_lcd_en(csp_lcd_t *ptLcdBase)
{
	ptLcdBase->CR |= LCD_EN;
}
static inline void csp_lcd_set_cdr(csp_lcd_t *ptLcdBase, uint8_t byCdiv, uint16_t hwCpre)
{
	if((byCdiv == 0) && (hwCpre == 0))
		ptLcdBase->CDR = 0x00000000;
	else
		ptLcdBase->CDR = (LCD_CDIV(byCdiv) | LCD_CPRE(hwCpre) | (LCD_CDC_EN << LCD_CDC_POS));
}
static inline void csp_lcd_set_mod(csp_lcd_t *ptLcdBase, lcd_vlcd_e eVlcd)
{
	ptLcdBase->MOD = eVlcd;
}
//
static inline void csp_lcd_set_blink(csp_lcd_t *ptLcdBase, lcd_blink_e eBkMode, lcd_blink_fre_e eBkFre)
{
	ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_BLINK_MSK | LCD_BLINKF_MSK)) | (eBkMode << LCD_BLINK_POS) | (eBkFre << LCD_BLINKF_POS);
}

static inline void csp_lcd_set_blinkf2(csp_lcd_t *ptLcdBase, uint16_t hwBkF2)
{
	ptLcdBase->CR2 = (ptLcdBase->CR2 & ~LCD_BLINKF2_MSK) | LCD_BLINKF2(hwBkF2);
}

//update display request
static inline void csp_lcd_set_udr(csp_lcd_t *ptLcdBase)
{
	ptLcdBase->CR2 |= LCD_UDR_EN;
}

static inline void csp_lcd_clr_sr(csp_lcd_t *ptLcdBase, lcd_sr_e eSr)
{
	ptLcdBase->ICR = eSr;
}

static inline void csp_lcd_set_bp(csp_lcd_t *ptLcdBase, lcd_bp_e eBp)
{
	ptLcdBase->CR = (ptLcdBase->CR & ~LCD_BP_MSK) | (eBp << LCD_BP_POS);
}

static inline uint32_t csp_lcd_pump_rdy(csp_lcd_t *ptLcdBase)
{
	return (uint32_t)(ptLcdBase->CR & LCD_PUMP_RDY);
}

static inline uint32_t csp_lcd_bias_rdy(csp_lcd_t *ptLcdBase)
{
	return (uint32_t)(ptLcdBase->CR & LCD_BIAS_RDY);
}

static inline uint8_t csp_lcd_get_sr(csp_lcd_t *ptLcdBase)
{
	return (uint8_t)ptLcdBase->SR;
}

static inline void csp_lcd_int_enable(csp_lcd_t *ptLcdBase, lcd_int_e eLcdInt, bool bEnable)
{
	if(bEnable)
		ptLcdBase->IER |= eLcdInt;
	else
		ptLcdBase->IER &= ~eLcdInt;

}

static inline uint8_t csp_lcd_get_isr(csp_lcd_t *ptLcdBase)
{
	return (uint8_t)(ptLcdBase->MISR);
}

static inline void csp_lcd_clr_isr(csp_lcd_t *ptLcdBase, lcd_int_e eLcdInt)
{
	ptLcdBase->ICR = eLcdInt;
}


#endif

