/***********************************************************************//** 
 * \file  lcd.c
 * \brief  csi lcd driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version	<th>Author  <th>Description
 * <tr><td> 2021-12-03 <td>V0.0 <td>LQ      <td>initial
 * <tr><td> 2021-12-25 <td>V0.0 <td>ZJY     <td>initial
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/lcd.h>
#include <drv/irq.h>
#include "board_config.h"
#include "iostring.h"

/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
static uint16_t g_hwFckDiv = 0;


/** \brief lcd interrupt handle function
 * 
 *  \param[in] ptLcdBase: pointer of bt register structure
 *  \return none
 */ 
__attribute__((weak)) void lcd_irqhandler(csp_lcd_t *ptLcdBase)
{
   	if(csp_lcd_get_isr(ptLcdBase) & LCD_SOF_INT)
	{
		csp_lcd_clr_isr(ptLcdBase, LCD_SOF_INT);
	}
	
	if(csp_lcd_get_isr(ptLcdBase) & LCD_UDD_INT)
	{
		csp_lcd_clr_isr(ptLcdBase, LCD_UDD_INT);
	}
}
/** \brief initialize lcd data structure
 * 
 *  \param[in] ptLcdBase: pointer of lcd register structure
 *  \param[in] ptLcdCFg: pointer of lcd parameter config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_lcd_init(csp_lcd_t *ptLcdBase, csi_lcd_config_t *ptLcdCFg)
{
	uint8_t byDuty = 4;
	uint32_t wFreq = ISOSC_VALUE;
	uint16_t hwCpre = 1;
	volatile uint32_t wDelay = 0x7ffff;
	
	csi_clk_enable((uint32_t *)ptLcdBase);			//lcd clk enable
	csp_lcd_soft_rst(ptLcdBase);					//soft rst lcd
	csp_lcd_clk_en(ptLcdBase, ptLcdCFg->byClkSrc);		//lcd clk source		
	
	switch(ptLcdCFg->byDutyBias)					//get lcd duty
	{
		case LCD_DUTY1_8_BIAS1_4:
			byDuty = 8;
			break;
		case LCD_DUTY1_6_BIAS1_4:
			byDuty = 6;
			break;
		case LCD_DUTY1_5_BIAS1_3:
			byDuty = 5;
			break;
		case LCD_DUTY1_4_BIAS1_3:
			byDuty = 4;
			break;
		case LCD_DUTY1_3_BIAS1_3:
		case LCD_DUTY1_3_BIAS1_2:
			byDuty = 3;
			break;
		case LCD_DUTY1_2_BIAS1_2:
			byDuty = 2;
			break;
		default:
			return CSI_ERROR;
	}
	
	if(ptLcdCFg->byClkSrc == LCD_CLKSRC_ISOSC)		//get lcd clk source
		wFreq = ISOSC_VALUE;
	else
	{
		switch(csp_rtc_get_clksrc(RTC))
		{
			case 0:									
				wFreq = ISOSC_VALUE;
				break;
			case 1:
				switch(csp_get_imosc_fre(SYSCON))
				{
//					case 0:
//						wFreq = IMOSC_5M_VALUE >> 2;
//						break;
//					case 1:
//						wFreq = IMOSC_4M_VALUE >> 2;
//						break;
//					case 2:
//						wFreq = IMOSC_2M_VALUE >> 2;
//						break;
					case 3:
						wFreq = IMOSC_131K_VALUE >> 2;
						break;
					default:
						return CSI_ERROR;			//imosc = 2/4/5MHz, lcd  don't support	
				}
				break;
			case 2:
				wFreq = ESOSC_VALUE;				
				break;
			case 3:
				if(EMOSC_VALUE > 1000000)			//lcd clk <= 1MHz
					return CSI_ERROR;
				else
					wFreq = EMOSC_VALUE >> 2;
				break;
		}
	}
	
	hwCpre = (wFreq >> 3)/byDuty/ptLcdCFg->byFreq;
	if(hwCpre)
		hwCpre = hwCpre -1;
	
	g_hwFckDiv = (wFreq >> 3) / (hwCpre +1);		//backup FckDiv for Blink config
	csp_lcd_set_cdr(ptLcdBase, 2, hwCpre);				//lcdclk = wFreq/2^(2+1)/(hwCpre+1)); Fframe = lcdclk/byDuty
	
	//lcd driver network, power contorl,
	switch(ptLcdCFg->byDrvNet)
	{
		case LCD_DRVNET_PWLEV0:															
			ptLcdBase->CR = (ptLcdBase->CR & ~LCD_HDCYCLE_MSK) | LCD_HDPULSE_MSK | LCD_HD_MSK | (LCD_HDCYCLE_3 << LCD_HDCYCLE_POS);		//HD=EN,HDP=EN;(RL)
			break;
		case LCD_DRVNET_PWLEV1:
			ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_HDPULSE_MSK | LCD_HDCYCLE_MSK)) | LCD_HD_MSK | (LCD_HDCYCLE_3 << LCD_HDCYCLE_POS);	//HD=EN,HDP=DIS;(RL/RM)
			break;
		case LCD_DRVNET_PWLEV2:
			ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_HD_MSK | LCD_HDCYCLE_MSK)) | LCD_HDPULSE_MSK | (LCD_HDCYCLE_3 << LCD_HDCYCLE_POS);	//HD=DIS,HDP=EN;(RM/RH)
			break;
		case LCD_DRVNET_PWLEV3:
			ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_HD_MSK | LCD_HDPULSE_MSK | LCD_HDCYCLE_MSK)) | (LCD_HDCYCLE_3 << LCD_HDCYCLE_POS);	//HD=DIS,HDP=DIS;(RH)
			break;
		default:
			return CSI_ERROR;
	}
	
	//lcd config
	if(ptLcdCFg->byVlcd < LCD_VLCD_EX_VDD)						//vlcd = interior voltage
	{
		csp_lcd_set_mod(ptLcdBase, ptLcdCFg->byVlcd);
		ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_DISC_MSK |LCD_VLCD_MSK | LCD_BIAS_MSK | LCD_BP_MSK | LCD_DBSEL_MSK | LCD_DEAD_MSK));
		ptLcdBase->CR |= (LCD_DISC_NOR << LCD_DISC_POS) |  (LCD_PUMP_EN <<  LCD_PUMP_POS)| (ptLcdCFg->byDpEn << LCD_BP_POS) | 		
				(ptLcdCFg->byDutyBias << LCD_DBSEL_POS);
		
		wDelay = 0x7ffff;
		while(1)
		{
			while(!csp_lcd_pump_rdy(ptLcdBase) && --wDelay);	//vlcd inside pump ready 
			if(0 == wDelay)
			{
				if(ptLcdCFg->byVlcd > 0)
				{
					ptLcdCFg->byVlcd -= 1;
					csp_lcd_set_mod(ptLcdBase, ptLcdCFg->byVlcd);
					ptLcdBase->CR |= (LCD_PUMP_EN <<  LCD_PUMP_POS);
					wDelay = 0x7fff;
				}
				else
					return CSI_ERROR;
			}
			else
				break;
		}
		
		wDelay =  0x7ffff;
		ptLcdBase->CR |= (LCD_BIAS_EN << LCD_BIAS_POS);		//enable bias 
		while(!csp_lcd_bias_rdy(ptLcdBase) && --wDelay);
		if(0 == wDelay)
			return CSI_ERROR;
			
		if(ptLcdCFg->byDead)								//dead config
		{
			wDelay = 0x7ffff;
			csp_lcd_en(ptLcdBase);
			csp_lcd_clr_isr(ptLcdBase, LCD_SOF_INT);
			while(!(csp_lcd_get_sr(ptLcdBase) & LCD_SOF_SR) && --wDelay);
			if(wDelay)
				ptLcdBase->CR |= (ptLcdCFg->byDead << LCD_DEAD_POS);
		}
	}
	else if(ptLcdCFg->byVlcd == LCD_VLCD_EX_VDD)			//vlcd = exterior voltage
	{
		ptLcdBase->CR = (ptLcdBase->CR & ~(LCD_DISC_MSK | LCD_PUMP_MSK | LCD_BIAS_MSK | LCD_BP_MSK | LCD_DBSEL_MSK | LCD_DEAD_MSK));
		
		ptLcdBase->CR |= (LCD_VLCD_EX <<  LCD_VLCD_POS) | (LCD_DISC_NOR << LCD_DISC_POS) | (ptLcdCFg->byDpEn << LCD_BP_POS) | 
				(ptLcdCFg->byDutyBias << LCD_DBSEL_POS) | (LCD_BIAS_EN << LCD_BIAS_POS);
		//while(!csp_lcd_bias_rdy(ptLcdBase));
		
		if(ptLcdCFg->byDead)								//dead config
		{
			csp_lcd_en(ptLcdBase);
			csp_lcd_clr_isr(ptLcdBase, LCD_SOF_INT);
			while(!(csp_lcd_get_sr(ptLcdBase) & LCD_SOF_SR) && --wDelay);
			if(wDelay)
				ptLcdBase->CR |= (ptLcdCFg->byDead << LCD_DEAD_POS);
		}
	}
	else
		return CSI_ERROR;
	
	if(ptLcdCFg->byInt)
	{
		csp_lcd_int_enable(ptLcdBase, ptLcdCFg->byInt, ENABLE);
		csi_irq_enable((uint32_t *)ptLcdBase);							//enable bt irq
	}
	
	return CSI_OK;
}
/** \brief start(enable) lcd
 * 
 *  \param[in] ptLcdBase: pointer of lcd register structure
 *  \return none
 */ 
void csi_lcd_start(csp_lcd_t *ptLcdBase)
{
	csp_lcd_en(ptLcdBase);
}
/** \brief enable/disable lcd interrupt 
 * 
 *  \param[in] ptLcdBase: pointer of lcd register structure
 *  \param[in] eIntSrc: lcd interrupt source
 *  \param[in] bEnable: enable/disable interrupt
 *  \return none
 */
void csi_lcd_int_enable(csp_lcd_t *ptLcdBase, csi_lcd_intsrc_e eIntSrc, bool bEnable)
{
	csp_lcd_int_enable(ptLcdBase, eIntSrc, bEnable);
	
	if(bEnable)
		csi_irq_enable((uint32_t *)ptLcdBase);
	else
		csi_irq_disable((uint32_t *)ptLcdBase);
}
/** \brief lcd blink config
 * 
 *  \param[in] ptLcdBase: pointer of LCD reg structure
 *  \param[in] eBkMode: blink mode
 *  \param[in] eBkFre: blink frequence
 *  \param[in] hwF2Fre: eBkFre = LCD_BLINK_FRE_F2, blink frequence, unit : Hz
 *  \return none
 */ 
csi_error_t csi_lcd_set_blink(csp_lcd_t *ptLcdBase, csi_lcd_blink_md_e eBkMode, csi_lcd_blink_fre_e eBkFre, uint16_t hwF2Fre)
{
	if(eBkFre  > LCD_BLINK_FRE_F2)
		return CSI_ERROR;
	else
	{
		csp_lcd_set_blink(ptLcdBase, eBkMode, eBkFre);
		if(eBkFre == LCD_BLINK_FRE_F2)
		{
			uint16_t hwBkF2;
 
			if(hwF2Fre == 0)
				return CSI_ERROR;
		
			hwBkF2 = g_hwFckDiv / hwF2Fre;
			if(hwBkF2 < 8)
				hwBkF2 = 8;
			csp_lcd_set_blinkf2(ptLcdBase, hwBkF2);
			csp_lcd_set_blink(ptLcdBase, eBkMode, eBkFre);
		}
	}
		
	return CSI_OK;
}
/** \brief write(updata) lcd display data
 * 
 *  \param[in] ptLcdBase: pointer of LCD reg structure.
 *  \param[in] pbyData: pointer of write lcd data buf  
 *  \param[in] byStaPos: SEGx starting position, 0~31
 *  \param[in] bySegNum: SEGx number, 1~32
 *  \return none
 */ 
void csi_lcd_write_data(csp_lcd_t *ptLcdBase, uint8_t *pbyData, uint8_t byStaPos, uint8_t bySegNum)
{
	uint8_t i;
	uint16_t hwCnt = 0x7fff;
	
	for(i = byStaPos; i < (bySegNum + byStaPos); i++)
	{
		ptLcdBase->DMR[i] = pbyData[i];
	}
	
	csp_lcd_set_udr(ptLcdBase);
	//while(!(csp_lcd_get_sr(ptLcdBase) & LCD_UDD_SR) && --hwCnt);
	while(hwCnt --)
	{
		if(csp_lcd_get_sr(ptLcdBase) & LCD_UDD_SR)
			break;
	}
	
	csp_lcd_clr_sr(ptLcdBase, LCD_UDD_SR);
}
/** \brief lcd gpio init
 * 
 *  \param[in] ptLcdBase: pointer of LCD reg structure.
 *  \param[in] wSegMask:  lcd SEG(0->31) Mask
 *  \param[in] byComMask: lcd COM(0->8) Mask
 *  \return none
 */ 
void csi_lcd_gpio_init(uint32_t wSegMask, uint8_t byComMask)
{
	uint8_t i;
	for(i = 0; i < 32; i++)
	{
		if(wSegMask & (0x01ul << i))
		{
			switch(i)
			{
				case 0:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFFFF0) | 0x00000004;break;		//SEG0  PA0.08
				case 1:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFFF0F) | 0x00000040;break;		//SEG1  PA0.09
				case 2:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFF0FF) | 0x00000400;break;		//SEG2  PA0.10
				case 3:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFF0FFF) | 0x00004000;break;		//SEG3  PA0.11
				case 4:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFF0FFFF) | 0x00040000;break;		//SEG4  PA0.12
				case 5:GPIOA0->CONHR=(GPIOA0->CONHR&0xFF0FFFFF) | 0x00400000;break;		//SEG5  PA0.13
				case 6:GPIOA0->CONHR=(GPIOA0->CONHR&0xF0FFFFFF) | 0x04000000;break;		//SEG6  PA0.14
				case 7:GPIOA0->CONHR=(GPIOA0->CONHR&0x0FFFFFFF) | 0x40000000;break;		//SEG7  PA0.15
				case 8:GPIOB0->CONLR=(GPIOB0->CONLR&0xFFFF0FFF) | 0x00004000;break;		//SEG8  PB0.03
				case 9:GPIOC0->CONLR=(GPIOC0->CONLR&0xFFFFFF0F) | 0x00000040;break;		//SEG9  PC0.01
				
				case 10:GPIOB0->CONLR=(GPIOB0->CONLR&0xFFF0FFFF) | 0x00040000;break;	//SEG10 PB0.04
				case 11:GPIOA1->CONLR=(GPIOA1->CONLR&0xFFFFFFF0) | 0x00000004;break;	//SEG11 PA1.00			
				case 12:GPIOA1->CONLR=(GPIOA1->CONLR&0xFFFFFF0F) | 0x00000040;break;	//SEG12 PA1.01			
				case 13:GPIOA1->CONLR=(GPIOA1->CONLR&0xFFFFF0FF) | 0x00000400;break;	//SEG13 PA1.02		
				case 14:GPIOA1->CONLR=(GPIOA1->CONLR&0xFFFF0FFF) | 0x00004000;break;	//SEG14 PA1.03		
				case 15:GPIOA1->CONLR=(GPIOA1->CONLR&0xFFF0FFFF) | 0x00040000;break;	//SEG15 PA1.04		
				case 16:GPIOA1->CONLR=(GPIOA1->CONLR&0xFF0FFFFF) | 0x00400000;break;	//SEG16 PA1.05		
				case 17:GPIOA1->CONLR=(GPIOA1->CONLR&0xF0FFFFFF) | 0x04000000;break;	//SEG17 PA1.06		
				case 18:GPIOB0->CONLR=(GPIOB0->CONLR&0xFF0FFFFF) | 0x00400000;break;	//SEG18 PB0.05		
				//case 19:GPIOA1->CONLR=(GPIOA1->CONLR&0x0FFFFFFF) | 0x40000000;break;	//SEG19 PA1.07		//swd
				
				case 20:GPIOB0->CONLR=(GPIOB0->CONLR&0xF0FFFFFF) | 0x04000000;break;	//SEG20 PB0.06	
				case 21:GPIOB0->CONLR=(GPIOB0->CONLR&0x0FFFFFFF) | 0x40000000;break;	//SEG21 PB0.07	
				//case 22:GPIOA1->CONHR=(GPIOA1->CONHR&0xFFFFFFF0) | 0x00000004;break;	//SEG22 PA1.08		//swc	  
				case 23:GPIOB0->CONHR=(GPIOB0->CONHR&0xFFFFFFF0) | 0x00000004;break;	//SEG23 PB0.08	
				case 24:GPIOB0->CONHR=(GPIOB0->CONHR&0xFFFFFF0F) | 0x00000040;break;	//SEG24 PB0.09	
				case 25:GPIOB0->CONHR=(GPIOB0->CONHR&0xFFFFF0FF) | 0x00000400;break;	//SEG25 PB0.10	
				case 26:GPIOB0->CONHR=(GPIOB0->CONHR&0xFFFF0FFF) | 0x00004000;break;	//SEG26 PB0.11	
				case 27:GPIOA1->CONHR=(GPIOA1->CONHR&0xFFFFFF0F) | 0x00000040;break;	//SEG27 PA1.09	
				case 28:GPIOA1->CONHR=(GPIOA1->CONHR&0xFFFFF0FF) | 0x00000400;break;	//SEG28 PA1.10	
				case 29:GPIOA1->CONHR=(GPIOA1->CONHR&0xFFFF0FFF) | 0x00004000;break;	//SEG29 PA1.11
				
				case 30:GPIOA1->CONHR=(GPIOA1->CONHR&0xFFF0FFFF) | 0x00040000;break;	//SEG30 PA1.12	
				case 31:GPIOA1->CONHR=(GPIOA1->CONHR&0xFF0FFFFF) | 0x00400000;break;	//SEG31 PA1.13	
			}
		}
	}
	
	for(i = 0; i < 8; i++)
	{
		if(byComMask & (0x01ul << i))
		{
			switch(i)
			{
				case 0:GPIOA0->CONLR=(GPIOA0->CONLR&0xF0FFFFFF) | 0x04000000;break;		//COM0 PA0.06
				case 1:GPIOA0->CONLR=(GPIOA0->CONLR&0x0FFFFFFF) | 0x40000000;break;		//COM1 PA0.07
				case 2:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFFFF0) | 0x00000004;break;		//COM2 PA0.08
				case 3:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFFF0F) | 0x00000040;break;		//COM3 PA0.09	
					
				case 4:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFFF0FF) | 0x00000400;break;		//COM4 PA0.10	
				case 5:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFFF0FFF) | 0x00004000;break;		//COM5 PA0.11	
				case 6:GPIOA0->CONHR=(GPIOA0->CONHR&0xFFF0FFFF) | 0x00040000;break;		//COM6 PA0.12
				case 7:GPIOA0->CONHR=(GPIOA0->CONHR&0xFF0FFFFF) | 0x00400000;break;		//COM7 PA0.13	
			}
		}
	}
}