/***********************************************************************//** 
 * \file  sio.c
 * \brief  csi sio driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>XB   <td>initial
 * <tr><td> 2021-1-03 <td>V0.0  <td>ZJY   <td>modify
 * <tr><td> 2021-12-03 <td>V0.1  <td>LQ   <td>modify
 * </table>
 * *********************************************************************
*/
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys_clk.h>
#include <drv/sio.h>
#include <drv/irq.h>
#include <drv/pin.h>
#include <drv/tick.h>

/* Private macro------------------------------------------------------*/
#define SIO_RESET_VALUE  (0x00000000)
#define SIO_RX_TIMEOUT		(0x10ff)
#define SIO_TX_TIMEOUT		(0x1FFF)
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/
csi_sio_trans_t g_tSioTran;	


/** \brief Init sio tx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptTxCfg: pointer of sio parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_tx_init(csp_sio_t *ptSioBase, csi_sio_tx_config_t *ptTxCfg)
{
	uint16_t hwClkDiv;
	
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_en(ptSioBase);					//enable clk
	csp_sio_set_mode(ptSioBase, SIO_MODE_TX);	//sio tx mode
	
	if(ptTxCfg->wTxFreq > 1)					//tx clk config
	{
		hwClkDiv = csi_get_pclk_freq()/ptTxCfg->wTxFreq - 1;
		csp_sio_set_tx_clkdiv(ptSioBase, hwClkDiv);
	}
	else
		return CSI_ERROR;
	
	//set TX CR0~1 reg
	csp_sio_set_txcr0(ptSioBase, ptTxCfg->byIdleLev, ptTxCfg->byTxDir, ptTxCfg->byTxBufLen - 1, ptTxCfg->byTxCnt - 1);	
	csp_sio_set_d0(ptSioBase, ptTxCfg->byD0Len - 1);					//set d0 clk len
	csp_sio_set_d1(ptSioBase, ptTxCfg->byD1Len - 1);					//set d1 clk len
	csp_sio_set_dl(ptSioBase, ptTxCfg->byDLLen - 1, ptTxCfg->byDLLsq);	//set dl clk len and lsq
	csp_sio_set_dh(ptSioBase, ptTxCfg->byDHLen - 1, ptTxCfg->byDHHsq);	//set dl clk len and hsq
	
	if(ptTxCfg->byInt)	
	{	
		g_tSioTran.bySendMode = SIO_TX_MODE_INT;						//interrupt, unsupport
		csi_irq_enable((uint32_t*)ptSioBase);							//enable sio irq 
		//csp_sio_int_enable(ptSioBase,ptTxCfg->byInt, ENABLE);       //this open in function[csi_sio_send_async]
	}
	else
		g_tSioTran.bySendMode = SIO_TX_MODE_POLL;						//polling mode

	return CSI_OK;
}

/** \brief Init sio rx, Initializes the resources needed for the sio instance 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] ptRxCfg: pointer of sio parameter config structure
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_rx_init(csp_sio_t *ptSioBase, csi_sio_rx_config_t *ptRxCfg)
{
	uint16_t hwClkDiv;
	
	csi_clk_enable((uint32_t *)ptSioBase);		//sio peripheral clk en
	csp_sio_clk_en(ptSioBase);					//enable clk
	csp_sio_set_mode(ptSioBase, SIO_MODE_RX);	//sio rx mode
	
	if(ptRxCfg->wRxFreq > 1)					//tx clk config
	{
		hwClkDiv = csi_get_pclk_freq()/ptRxCfg->wRxFreq - 1;
		csp_sio_set_rx_clkdiv(ptSioBase, hwClkDiv);
	}
	else
		return CSI_ERROR;
	
	//rx receive config
	csp_sio_set_rx_deb(ptSioBase, ptRxCfg->byDebPerLen - 1, ptRxCfg->byDebClkDiv - 1);								//set rx sampling debounce
	csp_sio_set_sample_mode(ptSioBase, ptRxCfg->byTrgEdge, ptRxCfg->byTrgMode, ptRxCfg->bySpMode);					//set rx samping mode
	csp_sio_set_sample(ptSioBase, ptRxCfg->bySpExtra, SIO_ALIGN_EN, ptRxCfg->bySpBitLen - 1, ptRxCfg->byHithr);		//set rx samping control
	csp_sio_set_recv(ptSioBase, ptRxCfg->byRxDir, ptRxCfg->byRxBufLen - 1, ptRxCfg->byRxCnt - 1);					//set receive para
	
	if(ptRxCfg->byInt)	
	{
		g_tSioTran.byRecvMode = SIO_RX_MODE_INT;						//interrupt mode						
		if(ptRxCfg->byInt & SIO_INTSRC_RXDNE)							//RXDONE  interrupt				
		{
			
			if(ptRxCfg->byRxCnt > 256)									//byRxCnt > 32 ,the mode work error
				return CSI_ERROR;
		}
		csp_sio_int_enable(ptSioBase, ptRxCfg->byInt, ENABLE);		//enable sio interrupt
		csi_irq_enable((uint32_t*)ptSioBase);							//enable sio irq 
	}
	else
		g_tSioTran.byRecvMode = SIO_RX_MODE_POLL;						//polling mode, unsupport

	return CSI_OK;
}
/** \brief sio receive break reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eBkLev: break reset detect level
 *  \param[in] byBkCnt: break reset detect period
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_break_rst(csp_sio_t *ptSioBase, csi_sio_bklev_e eBkLev, uint8_t byBkCnt, bool bEnable)
{
	if(byBkCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_break(ptSioBase, bEnable, (sio_breaklel_e)eBkLev, byBkCnt - 1);

	return CSI_OK; 
}

/** \brief sio receive timeout reset config
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] byBkCnt: break reset detect period
 *  \param[in] bEnable: ENABLE/DISABLE
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_sio_timeout_rst(csp_sio_t *ptSioBase, uint8_t byToCnt ,bool bEnable)
{
	if(byToCnt == 0)
		return CSI_ERROR;
		
	csp_sio_set_torst(ptSioBase, bEnable, byToCnt - 1);
	
	return CSI_OK; 
}
/** \brief sio transfer mode set,send(tx)/receive(rx)
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eWorkMd: sio working mode, send(tx)/receive(rx)
 *  \return none
 */
void csi_sio_set_mode(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWorkMd)
{
	csp_sio_set_mode(ptSioBase, (sio_mode_e)eWorkMd);
}
/** \brief enable/disable sio interrupt 
 * 
 *  \param[in] ptSioBase: pointer of sio register structure
 *  \param[in] eIntSrc: sio interrupt source
 *  \param[in] bEnable: enable/disable interrupt
 *  \return none
 */
void csi_sio_int_enable(csp_sio_t *ptSioBase, csi_sio_intsrc_e eIntSrc, bool bEnable)
{
	csp_sio_int_enable(ptSioBase, (sio_int_e)eIntSrc, bEnable);
	
	if(bEnable)
		csi_irq_enable((uint32_t *)ptSioBase);
	else
		csi_irq_disable((uint32_t *)ptSioBase);
}
/** \brief send data from sio, this function is polling mode(sync mode)
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \return error code \ref csi_error_t or receive data size
 */
int32_t csi_sio_send(csp_sio_t *ptSioBase, const uint32_t *pwData, uint16_t hwSize)
{
	uint16_t  i;

	switch(g_tSioTran.bySendMode)
	{
		case SIO_TX_MODE_POLL:											//sio send polling mode
		
			for(i = 0; i < hwSize; i++)									
			{
				csp_sio_set_txbuf(ptSioBase,pwData[i]);
				while(!(csp_sio_get_risr(ptSioBase) & SIO_TXBUFEMPT));
			}
			while(!(csp_sio_get_risr(ptSioBase) & SIO_TXDNE));
			csp_sio_clr_isr(ptSioBase, SIO_TXDNE);
			return i;
		case SIO_TX_MODE_INT:
			if(SIO_STATE_SEND == g_tSioTran.byTxStat)
			{
				return CSI_ERROR;
			}
			g_tSioTran.pwData 	 = (uint32_t *)pwData;
			g_tSioTran.hwSize 	 = hwSize;
			g_tSioTran.hwTranLen = 0;
			g_tSioTran.byTxStat  = SIO_STATE_SEND;
			csp_sio_int_enable(SIO0,SIO_TXBUFEMPT, ENABLE);
			return CSI_OK;	
		default:
			return CSI_UNSUPPORTED;
	}
}

/** \brief send data from sio, this function is intrrupt mode(async mode)
 * 
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \return error code \ref csi_error_t or receive data size
 */
int32_t csi_sio_send_async(uint32_t *pwData, uint16_t hwSize)
{
	if(SIO_STATE_SEND == g_tSioTran.byTxStat)
	{
		return CSI_ERROR;
	}
	g_tSioTran.pwData 	 = pwData;
	g_tSioTran.hwSize 	 = hwSize;
	g_tSioTran.hwTranLen = 0;
	g_tSioTran.byTxStat  = SIO_STATE_SEND;
	csp_sio_int_enable(SIO0,SIO_TXBUFEMPT, ENABLE);
	return CSI_OK;
}

//static void delay_ums(unsigned int t)
//{
//	volatile unsigned int i,j ,k=0;
//	j = 20* t;
//	for ( i = 0; i < j; i++ )
//	{
//		k++;
//	}
//}
/** \brief send data from sio, this function is polling mode(sync mode), with delay between bytes 
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwData: pointer to buffer with data to send 
 * \param[in] hwSize: send data size
 * \param[in] hwDelay: delay between bytes 
 * \return error code \ref csi_error_t or receive data size
*/
//int32_t csi_sio_send_space(csp_sio_t *ptSioBase, const uint32_t *pwData, uint16_t hwSize, uint16_t hwDelay)
//{
//	uint16_t  i;
//	
//	for(i = 0; i < hwSize; i++)									
//	{
//		csp_sio_set_txbuf(ptSioBase,pwData[i]);
//		while(!(csp_sio_get_risr(ptSioBase) & SIO_TXBUFEMPT));	
//		delay_ums(hwDelay);
//	}
//	while(!(csp_sio_get_risr(ptSioBase) & SIO_TXDNE));
//
//	return i;
//}
/** \brief set sio receive data buffer and length
 * 
 *  \param[in] pwData: pointer of sio transport data buffer
 *  \param[in] hwLen: sio transport data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_sio_set_buffer(uint32_t *pwData, uint16_t hwLen)
{
	if(NULL == pwData || hwLen == 0)
		return CSI_ERROR;
	
	g_tSioTran.pwData 	 = pwData;
	g_tSioTran.hwSize 	 = hwLen;
	g_tSioTran.hwTranLen = 0;
	g_tSioTran.byRxStat  = SIO_STATE_IDLE;
	g_tSioTran.byTxStat  = SIO_STATE_IDLE;
	
	return CSI_OK;
}
/** \brief receive data to sio transmitter, asynchronism mode
 * 
 * \param[in] ptSioBase: pointer of sio register structure
 * \param[in] pwRecv: pointer of sio receive data
 * \param[in] hwLen: length receive data
 * \return error code \ref csi_error_t or receive data len
 */
int32_t csi_sio_receive(csp_sio_t *ptSioBase, uint32_t *pwRecv, uint16_t hwLen)
{
	switch(g_tSioTran.byRecvMode)
	{
		case SIO_RX_MODE_INT:
			if(g_tSioTran.hwTranLen >= hwLen)		//sio receive interrupt 		
			{
				memcpy((void *)pwRecv, (void *)g_tSioTran.pwData, 4 * hwLen);
				g_tSioTran.hwTranLen = 0;
				g_tSioTran.byRxStat  = SIO_STATE_IDLE;
				return hwLen;
			}
			else
				return 0;
			case SIO_RX_MODE_POLL:	
				return CSI_UNSUPPORTED;;			//sio receive polling mode, unsupport
		default:
			return CSI_UNSUPPORTED;;
	}
}
/** \brief get sio receive/send complete message and (Do not) clear message
 * 
 *  \param[in] ptSioBase: pointer of sio reg structure.
 *  \param[in] eWkMode: tx or rx mode
 *  \param[in] bClrEn: clear sio receive/send complete message enable; ENABLE: clear , DISABLE: Do not clear
 *  \return  bool type true/false
 */ 
bool csi_sio_get_msg(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWkMode, bool bClrEn)
{
	bool bRet = false;
	
	switch(eWkMode)
	{
		case SIO_SEND:
			if(g_tSioTran.byTxStat == SIO_STATE_DONE)
			{
				if(bClrEn)
					g_tSioTran.byTxStat = SIO_STATE_IDLE;		//clear send status
				bRet = true;
			}
			break;
		case SIO_RECV:
			if(g_tSioTran.byRxStat == SIO_STATE_FULL)
			{
				if(bClrEn)
					g_tSioTran.byRxStat = SIO_STATE_IDLE;		//clear receive status
				bRet = true;
			}
			break;
		default:
			break;
	}
	
	return bRet;
}
/** \brief clr sio receive/send status message (set sio recv/send status idle) 
 * 
 *  \param[in] ptSioBase: pointer of sio reg structure.
 *  \param[in] eMode: tx or rx mode
 *  \return none
 */ 
void csi_sio_clr_msg(csp_sio_t *ptSioBase, csi_sio_wkmode_e eWkMode)
{
	
	if(eWkMode == SIO_SEND)
		g_tSioTran.byTxStat = SIO_STATE_IDLE;		//clear send status
	else
		g_tSioTran.byRxStat = SIO_STATE_IDLE;		//clear receive status		
}