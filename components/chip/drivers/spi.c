/***********************************************************************//** 
 * \file  spi.c
 * \brief  csi spi driver
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-8-28 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-11-29 <td>V0.1 <td>LQ     <td>modify
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/spi.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/porting.h>
#include <drv/tick.h>
#include <iostring.h>
#include <uart.h>

#include "csp_spi.h"
/* Private macro------------------------------------------------------*/
/* externs function---------------------------------------------------*/
/* externs variablesr-------------------------------------------------*/
/* Private variablesr-------------------------------------------------*/

csi_spi_transmit_t g_tSpiTransmit; 

/** \brief csi_spi_nss_high 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_spi_nss_high(pin_name_e ePinName)
{
	csi_pin_set_high(ePinName);
}

/** \brief csi_spi_nss_low 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_spi_nss_low(pin_name_e ePinName)
{
	csi_pin_set_low(ePinName);
}

/** \brief apt_spi_int_set 
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eSpiInt:spi interrupt source 
 *  \return none
 */ 
static void apt_spi_int_set(csp_spi_t *ptSpiBase,spi_int_e eSpiInt)
{
	if(eSpiInt != SPI_NONE_INT)
	{
		csi_irq_enable((uint32_t *)ptSpiBase);
		if(eSpiInt & SPI_RXIM_INT)
			csp_spi_int_enable(ptSpiBase, eSpiInt | SPI_RTIM_INT,true);
		else
			csp_spi_int_enable(ptSpiBase, eSpiInt,true);
	}
	else
	{
		csi_irq_disable((uint32_t *)ptSpiBase);
		csp_spi_int_enable(ptSpiBase, 0x0f,false);
	}
}

/** \brief initialize spi data structure
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] ptSpiCfg: pointer of user spi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_init(csp_spi_t *ptSpiBase,csi_spi_config_t *ptSpiCfg)
{
	csi_error_t tRet = CSI_OK;

	csi_clk_enable((uint32_t *)ptSpiBase);
	csp_spi_default_init(ptSpiBase);
	csp_spi_dis(ptSpiBase);
	csp_spi_set_rxifl(ptSpiBase, SPI_RXFIFO_1_2);
	csi_spi_mode(ptSpiBase, ptSpiCfg->bySpiMode);
	csi_spi_cp_format(ptSpiBase, ptSpiCfg->bySpiPolarityPhase);	
	csi_spi_frame_len(ptSpiBase, ptSpiCfg->bySpiFrameLen);
	csi_spi_baud(ptSpiBase, ptSpiCfg->wSpiBaud);
	apt_spi_int_set(ptSpiBase,ptSpiCfg->byInt);
	csi_spi_Internal_variables_init(ptSpiCfg);	
							
	csp_spi_en(ptSpiBase);
	
	return tRet;
}

/** \brief uninit spi data structure
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_uninit(csp_spi_t *ptSpiBase)
{
	csi_error_t tRet = CSI_OK;
	
	csi_clk_disable((uint32_t *)ptSpiBase);	
	csi_irq_disable((uint32_t *)ptSpiBase);
	csp_spi_default_init(ptSpiBase);
	g_tSpiTransmit.pbyRxData =NULL;
	g_tSpiTransmit.byRxSize =0;
	g_tSpiTransmit.pbyTxData =NULL;
	g_tSpiTransmit.byTxSize =0;
	g_tSpiTransmit.byRxFifoLength = SPI_RXFIFO_1_2;
	g_tSpiTransmit.byTxFifoLength = 0x04;//The spec specified
	g_tSpiTransmit.byInt = SPI_NONE_INT;
	g_tSpiTransmit.byWorkMode = (uint8_t)(SPI_MASTER);
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	g_tSpiTransmit.byReadable  = SPI_STATE_IDLE;
	g_tSpiTransmit.bySendMode = SPI_TX_MODE_POLL;
	g_tSpiTransmit.byRecvMode = SPI_RX_MODE_POLL;
	g_tSpiTransmit.bySendRecMode = SPI_TX_RX_MODE_POLL;
	
	return tRet;	
}
/** \brief set spi mode, master or slave
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eMode: master, slave
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_mode(csp_spi_t *ptSpiBase, csi_spi_mode_e eMode)
{
    csi_error_t   tRet = CSI_OK;
	
	switch (eMode) 			//configure spi mode
	{
        case SPI_MASTER:
            csp_spi_set_mode(ptSpiBase, SPI_MODE_MASTER);
            break;
        case SPI_SLAVE:
            csp_spi_set_mode(ptSpiBase, SPI_MODE_SLAVE);
            break;
        default:
            tRet = CSI_ERROR;
            break;
    }	
	return tRet;
}

/** \brief config spi cp format
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eFormat: spi cp format
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_cp_format(csp_spi_t *ptSpiBase, csi_spi_cp_format_e eFormat)
{
    csi_error_t   ret = CSI_OK;
	switch (eFormat)		//configure spi format 
	{
        case SPI_FORMAT_CPOL0_CPHA0:
			csp_spi_set_spo_sph(ptSpiBase, SPI_SPO0_SPH0);
            break;
        case SPI_FORMAT_CPOL0_CPHA1:
			csp_spi_set_spo_sph(ptSpiBase, SPI_SPO0_SPH1);
            break;
        case SPI_FORMAT_CPOL1_CPHA0:
			csp_spi_set_spo_sph(ptSpiBase, SPI_SPO1_SPH0);
            break;
        case SPI_FORMAT_CPOL1_CPHA1:
			csp_spi_set_spo_sph(ptSpiBase, SPI_SPO1_SPH1);
            break;
        default:
            ret = CSI_ERROR;
            break;
    }	
	return ret;
} 

/** \brief config spi work frequence
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] wBaud: spi work baud
 *  \return spi config frequency
 */
uint32_t csi_spi_baud(csp_spi_t *ptSpiBase, uint32_t wBaud)
{
    uint32_t wDiv;
    uint32_t wFreq = 0U;

	wDiv = (csi_get_pclk_freq() >> 1) / wBaud;//baud = FPCLK/ CPSDVR / (1 + SCR))
	
	if(wDiv > 0)
		wDiv --;
	
	csp_spi_set_clk_div(ptSpiBase, wDiv, 2);

	if(wDiv > 0)
		wFreq = (csi_get_pclk_freq() >> 1) / (wDiv + 1);
	else
		wFreq = (csi_get_pclk_freq() >> 1) ;
		
    return wFreq;
}

/** \brief config spi frame length
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eLength: frame length
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_frame_len(csp_spi_t *ptSpiBase, csi_spi_frame_len_e eLength)
{	
    csi_error_t   tRet = CSI_OK;

    if ((eLength < SPI_FRAME_LEN_4) || (eLength > SPI_FRAME_LEN_16)) 
        tRet = CSI_ERROR;
	else 
	{
        csp_spi_set_data_size(ptSpiBase, (eLength -1));			 //configura data frame width
    }
    return tRet;
}

/** \brief get the tState of spi device
 * 
 *  \param[in] eWorkMode
 *  \return read/write state or CSI_ERROR/CSI_OK
 */ 
int8_t csi_spi_get_state(csi_spi_work_mode_e eWorkMode)
{
   if( !(SPI_SEND == eWorkMode || SPI_RECV == eWorkMode) )
	   return CSI_ERROR;
	switch(eWorkMode)
   {
	   case SPI_SEND: 
			return g_tSpiTransmit.byWriteable;
	   case SPI_RECV: 
			return g_tSpiTransmit.byReadable;
	   default:  
			return CSI_ERROR;
   }
}

/** \brief sending data to spi transmitter
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
int32_t csi_spi_send(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{
	uint32_t wCount = 0U;
	uint32_t wSendStart;
	
	if(NULL == pData || 0 == wSize)
		return CSI_ERROR;
		
	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY))						
		return CSI_BUSY;
		
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.byTxSize = wSize;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pData;
	csi_spi_clr_rxfifo(ptSpiBase);
	csp_spi_en(ptSpiBase);
	
	switch(g_tSpiTransmit.bySendMode)
	{
		case SPI_TX_MODE_POLL:			
			while(g_tSpiTransmit.byTxSize > 0)
			{
				wSendStart = SPI_SEND_TIMEOUT;
				while(!csp_spi_write_ready(ptSpiBase) && wSendStart --); 
				if(wSendStart == 0)
				{
					break;
				}
				csp_spi_set_data(ptSpiBase, *g_tSpiTransmit.pbyTxData);
				g_tSpiTransmit.pbyTxData ++;
				wCount ++;
				g_tSpiTransmit.byTxSize --;
			}
			
			wSendStart = SPI_SEND_TIMEOUT;
			while(csp_spi_busy(ptSpiBase))
			{
				wSendStart --;
				if(wSendStart == 0)
					break;
			}

			g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;

			return wCount;
		
		case SPI_TX_MODE_INT:
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_TXIM_INT,true);
			return CSI_OK;
		default:
			return CSI_ERROR;
	}
}

/** \brief sending data to spi transmitter, non-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_async(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{	
	csi_error_t tRet = CSI_OK;
	
	if(NULL == pData || 0 == wSize)
		tRet = CSI_ERROR;
		
	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY))						
		tRet = CSI_BUSY;
		
	if(tRet == CSI_OK)
	{
		g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
		g_tSpiTransmit.byTxSize = wSize;
		g_tSpiTransmit.pbyTxData = (uint8_t *)pData;
		csi_spi_clr_rxfifo(ptSpiBase);
		csp_spi_en(ptSpiBase);
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_TXIM_INT,true);
	}
	
	return tRet;
}

/** \brief  receiving data from spi receiver
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to receive
 *  \param[in] wSize: number of data to receive(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_receive(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{
	uint32_t wCount = 0;
	uint32_t wTimeStart;
	if(NULL == pData || 0 == wSize)
		return CSI_ERROR;
		
	if((g_tSpiTransmit.byReadable == SPI_STATE_BUSY))						
		return CSI_BUSY;
	
	g_tSpiTransmit.byReadable = SPI_STATE_BUSY;
	g_tSpiTransmit.byRxSize = wSize;
	g_tSpiTransmit.pbyRxData = (uint8_t *)pData;
	csi_spi_clr_rxfifo(ptSpiBase);
	csp_spi_en(ptSpiBase);
	switch(g_tSpiTransmit.byRecvMode)
	{
		case SPI_RX_MODE_POLL:
			while(g_tSpiTransmit.byRxSize)
			{
				wTimeStart = SPI_RECV_TIMEOUT;
				while(!csp_spi_read_ready(ptSpiBase) && wTimeStart --);
				if(wTimeStart ==0)
				{
					break;
				}
				
				*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);
				g_tSpiTransmit.pbyRxData++;
				wCount ++;
				
				g_tSpiTransmit.byRxSize --;
			}
			g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
			return wCount;
			
		case SPI_RX_MODE_INT:
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RXIM_INT,true);
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RTIM_INT,true);
			return CSI_OK;
		default:
			return CSI_ERROR;
	}
}

/** \brief  receiving data from spi receiver, not-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_receive_async(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize)
{	
	csi_error_t tRet = CSI_OK;
	
	if(NULL == pData || 0 == wSize)
		tRet = CSI_ERROR;
		
	if((g_tSpiTransmit.byReadable == SPI_STATE_BUSY))						
		tRet = CSI_BUSY;
	
	if(tRet == CSI_OK)
	{
		g_tSpiTransmit.byReadable = SPI_STATE_BUSY;
		g_tSpiTransmit.byRxSize = wSize;
		g_tSpiTransmit.pbyRxData = (uint8_t *)pData;
		csp_spi_en(ptSpiBase);
		csi_spi_clr_rxfifo(ptSpiBase);
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RXIM_INT,true);
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RTIM_INT,true);
	}
	
	return tRet;
}

/** \brief  sending and receiving data from spi receiver
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataout: pointer to buffer with data to send to spi transmitter
 *  \param[in] pDatain: number of data to receive(byte)
 *  \param[in] wSize: number of data to receive(byte)
 *  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
int32_t csi_spi_send_receive(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize)
{
	uint32_t wCount = 0;
	uint32_t wTimeStart;
	if( (0 == wSize) || ( (NULL == pDataout) && (NULL == pDatain)  ) )
		return CSI_ERROR;
		
	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY)) 						
		return CSI_BUSY;
	
	g_tSpiTransmit.byTxSize = wSize;
	g_tSpiTransmit.byRxSize = wSize;
	
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pDataout;
	g_tSpiTransmit.pbyRxData = (uint8_t *)pDatain;
	csp_spi_en(ptSpiBase);
	
	switch(g_tSpiTransmit.bySendRecMode)
	{
		case SPI_TX_RX_MODE_POLL:
			while((g_tSpiTransmit.byTxSize > 0U) || (g_tSpiTransmit.byRxSize > 0U))
			{
				if(g_tSpiTransmit.byTxSize > 0U)
				{
					wTimeStart = SPI_SEND_TIMEOUT;
					while(!csp_spi_write_ready(ptSpiBase) && wTimeStart --);
					if(wTimeStart ==0)
					{
						break;
					}
					if(g_tSpiTransmit.pbyTxData)
					{
						csp_spi_set_data(ptSpiBase,*g_tSpiTransmit.pbyTxData);
						g_tSpiTransmit.pbyTxData ++;
					}
					else
					{
						csp_spi_set_data(ptSpiBase,0x00);
					}
					wCount ++;
					g_tSpiTransmit.byTxSize --;
				}
				
				if(g_tSpiTransmit.byRxSize > 0U)
				{
					wTimeStart = SPI_RECV_TIMEOUT;
					while(!csp_spi_read_ready(ptSpiBase) && wTimeStart --);
					if(wTimeStart ==0)
					{
						break;
					}
					if(g_tSpiTransmit.pbyRxData)
					{
						*g_tSpiTransmit.pbyRxData  = csp_spi_get_data(ptSpiBase);
						g_tSpiTransmit.pbyRxData ++;
					}
					else
					{
						csp_spi_get_data(ptSpiBase);
					}
					g_tSpiTransmit.byRxSize --;
				}
			}
			
			wTimeStart = SPI_RECV_TIMEOUT;
			while((csp_spi_get_sr(ptSpiBase) & SPI_BSY) && (wTimeStart --) );
			
			g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
			g_tSpiTransmit.byReadable =  SPI_STATE_IDLE;
			return wCount;
			
		case SPI_TX_RX_MODE_INT:
			csi_spi_clr_rxfifo(ptSpiBase);
			g_tSpiTransmit.byTxFifoLength = 0x02;
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RXIM_INT,true);
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RTIM_INT,true);
			csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_TXIM_INT,true);		
			return CSI_OK;
			
		default: return CSI_ERROR;
	}
}

/** \brief  receiving data from spi receiver, not-blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataout: pointer to buffer with data to send to spi transmitter
 *  \param[in] pDatain: number of data to receive(byte)
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_receive_async(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize)
{
    csi_error_t tRet = CSI_OK;
	
	if( (0 == wSize) || ( (NULL == pDataout) && (NULL == pDatain)  ) )
		tRet = CSI_ERROR;
	
	if((g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY))
	{ 
        tRet = CSI_BUSY;
	}
		
	if(tRet == CSI_OK) 
	{
		g_tSpiTransmit.byTxSize = wSize;
		g_tSpiTransmit.byRxSize = wSize;
	
		g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
		g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
		g_tSpiTransmit.pbyTxData = (uint8_t *)pDataout;
		g_tSpiTransmit.pbyRxData = (uint8_t *)pDatain;
		csp_spi_en(ptSpiBase);
		csi_spi_clr_rxfifo(ptSpiBase);
	
		g_tSpiTransmit.byTxFifoLength = 0x02;
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RXIM_INT,true);
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RTIM_INT,true);
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_TXIM_INT,true);
    } 
	return tRet;
}

/** \brief  transmission variables init ,user not change it
 * 
 *  \param[in] ptSpiCfg:pointer of user spi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_Internal_variables_init(csi_spi_config_t *ptSpiCfg)
{
	g_tSpiTransmit.pbyRxData =NULL;
	g_tSpiTransmit.byRxSize =0;
	g_tSpiTransmit.pbyTxData =NULL;
	g_tSpiTransmit.byTxSize =0;
	g_tSpiTransmit.byRxFifoLength = SPI_RXFIFO_1_2;
	g_tSpiTransmit.byTxFifoLength = 0x04;
	if(ptSpiCfg->byInt & SPI_RXIM_INT)
		g_tSpiTransmit.byInt = ptSpiCfg->byInt | SPI_RTIM_INT;
	else
	g_tSpiTransmit.byInt = ptSpiCfg->byInt;
	g_tSpiTransmit.byWorkMode = (uint8_t)(ptSpiCfg->bySpiMode);
	g_tSpiTransmit.byWriteable = SPI_STATE_IDLE;
	g_tSpiTransmit.byReadable  = SPI_STATE_IDLE;
	g_tSpiTransmit.bySendMode = ptSpiCfg->byTxMode;
	g_tSpiTransmit.byRecvMode = ptSpiCfg->byRxMode;
	g_tSpiTransmit.bySendRecMode = ptSpiCfg->byTxRxMode;
	
    return CSI_OK;
}

/** \brief clr spi rx fifo
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */
void csi_spi_clr_rxfifo(csp_spi_t *ptSpiBase)
{
	uint8_t byIndex=8;
	while(byIndex--)
	{
		ptSpiBase->DR;//fifo deep is 8,read 8 times is clear rx fifo
	}
}


//------------------------------------------------------------------------------------------
//interrupt process,just for reference
//------------------------------------------------------------------------------------------

/** \brief spi receive data,interrupt call 
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
static void apt_spi_intr_recv_data(csp_spi_t *ptSpiBase)
{
	if((g_tSpiTransmit.pbyRxData == NULL) || (g_tSpiTransmit.byRxSize == 0U))//use interrupt transceiver, but does not care about receiving
	{
		csi_spi_clr_rxfifo(ptSpiBase); 
		g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
		csp_spi_int_enable(ptSpiBase, g_tSpiTransmit.byInt & SPI_RXIM_INT,false);
		csp_spi_int_enable(ptSpiBase, SPI_RTIM_INT,false);
	}
	else
	{
		for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength; byIdx++)
		{
			*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);
			g_tSpiTransmit.pbyRxData++;
			g_tSpiTransmit.byRxSize--;
			if(g_tSpiTransmit.byRxSize == 0)
			{
				break;
			}
		}
		
		if(g_tSpiTransmit.byRxSize == 0)
		{
			g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
			csp_spi_int_enable(ptSpiBase, SPI_RXIM_INT | SPI_RTIM_INT, false);
		}
	}
}

/** \brief spi send data,interrupt call
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
static void apt_spi_intr_send_data(csp_spi_t *ptSpiBase)
{	
	uint8_t byCount = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;
	if( (ptSpiBase->CR1 & SPI_SSE_MSK) && (g_tSpiTransmit.byTxSize) )//Make sure that spi is enabled (if SPI is not enabled,just enable Tx int,Tx interrupts also come in )
	{
		
		if(g_tSpiTransmit.byTxSize >= (g_tSpiTransmit.byTxFifoLength) )
		{
			for(byCount = 0; byCount < g_tSpiTransmit.byTxFifoLength;byCount++)
			{
				if(g_tSpiTransmit.pbyTxData)//care your send data
				{
					csp_spi_set_data(ptSpiBase,*g_tSpiTransmit.pbyTxData);//send data
					g_tSpiTransmit.pbyTxData ++;
				}	
				else //just for generate the receive clock
				{
					csp_spi_set_data(ptSpiBase,0x00);
				}
			}
			g_tSpiTransmit.byTxSize = g_tSpiTransmit.byTxSize - g_tSpiTransmit.byTxFifoLength;	
		}
		else
		{
			for(byCount = 0; byCount < g_tSpiTransmit.byTxSize;byCount++)
			{
				if(g_tSpiTransmit.pbyTxData)
				{
					csp_spi_set_data(ptSpiBase,*g_tSpiTransmit.pbyTxData);
					g_tSpiTransmit.pbyTxData ++;
				}	
				else
				{
					csp_spi_set_data(ptSpiBase,0x00);
				}
			}
			g_tSpiTransmit.byTxSize = 0;	
		}
		
		if (g_tSpiTransmit.byTxSize <= 0U) 
		{
			while((csp_spi_get_sr(ptSpiBase) & SPI_BSY) && (wTimeStart --) );
			g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
			csp_spi_int_enable(ptSpiBase,SPI_TXIM_INT, false);
		}
	}
	else
	{
		while((csp_spi_get_sr(ptSpiBase) & SPI_BSY) && (wTimeStart --) );
		g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
		csp_spi_int_enable(ptSpiBase, SPI_TXIM_INT, false);	
	}
}

/** \brief spi interrupt handle weak function
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */ 
__attribute__((weak)) void spi_irqhandler(csp_spi_t *ptSpiBase)
{	
	uint32_t wStatus = csp_spi_get_isr(ptSpiBase);
	//fifo rx 
	if(wStatus & SPI_RXIM_INT)
	{
		//for reference
		apt_spi_intr_recv_data(ptSpiBase);
	}
	//fifo tx 
	if(wStatus & SPI_TXIM_INT)		
	{
		//for reference
		apt_spi_intr_send_data(ptSpiBase);
	}
	
	//fifo overflow
	if(wStatus & SPI_ROTIM_INT)
	{	
		//for reference
		csi_spi_clr_rxfifo(ptSpiBase);
		csp_spi_clr_isr(ptSpiBase, SPI_ROTIM_INT);
	}
	
	//fifo rx timeout
	if(wStatus & SPI_RTIM_INT)		
	{	
		//for reference
		csp_spi_clr_isr(ptSpiBase, SPI_RTIM_INT);
		
		for(uint8_t byIdx = 0; byIdx < g_tSpiTransmit.byRxFifoLength; byIdx++)
		{
			if(csp_spi_get_sr(ptSpiBase) & SPI_RNE)
			{
				*g_tSpiTransmit.pbyRxData = csp_spi_get_data(ptSpiBase);
				g_tSpiTransmit.pbyRxData++;
			}
			else
			{
				break;
			}			
		}		
		g_tSpiTransmit.byRxSize = 0;
		g_tSpiTransmit.byReadable = SPI_STATE_IDLE;
		csp_spi_int_enable(ptSpiBase, SPI_RXIM_INT | SPI_RTIM_INT, false);

	}
}

//-----------------------------------------------------------------------------------------------------------
//high speed spi function for reference
//-----------------------------------------------------------------------------------------------------------

/** \brief (this funtion will ignore the receive)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer
 *  \param[in] bySize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_fast(csp_spi_t *ptSpiBase,void *pDataOut,uint8_t bySize)
{		
	csi_error_t tRet = CSI_OK;
	uint8_t byIcount = 0;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;
	
	if( (0 == bySize) || (NULL == pDataOut) ) 
	{
		tRet = CSI_ERROR;
		return tRet;
	}
	
	if( g_tSpiTransmit.byWriteable == SPI_STATE_BUSY ) 
	{
		tRet = CSI_BUSY;
		return tRet;
	}
	
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pDataOut;
	
	for(byIcount = 0;byIcount <bySize;byIcount++)
	{
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ( !((uint32_t)(ptSpiBase->SR) & SPI_TNF) ) && (wTimeStart --) ){;}
		ptSpiBase->DR = *(g_tSpiTransmit.pbyTxData+byIcount);
	}
	
	wTimeStart = SPI_SEND_TIMEOUT;
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;}
	
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	
	return tRet;
}

/** \brief spi send and receive fast
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer 
 *  \param[in] pDataIn  :receive data buffer pointer 
 *  \param[in] wSize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_receive_fast(csp_spi_t *ptSpiBase, uint8_t *pDataOut,uint8_t *pDataIn, uint32_t wSize)
{
	csi_error_t tRet = CSI_OK;
	uint8_t byTxsize = wSize;
	uint32_t wTimeStart = SPI_SEND_TIMEOUT;

	uint8_t byCount = 0;
	
	uint8_t byOutidx = 0;
	uint8_t byIdx;
	uint8_t byRemainder = 0;
	uint8_t byZheng = 0;
	uint8_t byLast8Times = 0;
	uint8_t byLastTxBuff[8] = {0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
	
	if( (0 == byTxsize) ||  ( (NULL == pDataOut) && (NULL == pDataIn) ) ) 
	{
		tRet = CSI_ERROR;
		return tRet;
	}

	if( (g_tSpiTransmit.byWriteable == SPI_STATE_BUSY) || (g_tSpiTransmit.byReadable == SPI_STATE_BUSY) ) 
	{
		tRet = CSI_BUSY;
		return tRet;
	}
	
	g_tSpiTransmit.byWriteable = SPI_STATE_BUSY;
	g_tSpiTransmit.byReadable  = SPI_STATE_BUSY;
	g_tSpiTransmit.pbyTxData = (uint8_t *)pDataOut;
	g_tSpiTransmit.pbyRxData = (uint8_t *)pDataIn;	
	csi_spi_clr_rxfifo(ptSpiBase);
	
	
	if(byTxsize > 8)
	{	
		byZheng = (wSize >> 3);
		byRemainder = wSize & 0x07;
		byLast8Times = byZheng << 3;
		
		memcpy((void *)byLastTxBuff,(void *)&g_tSpiTransmit.pbyTxData[byLast8Times], byRemainder);
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 

		while(byTxsize >=8U)
		{
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+1];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+2];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+3];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+4];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+5];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+6];
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byOutidx+7];

			wTimeStart = SPI_SEND_TIMEOUT;
			while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 		

			byTxsize -= 8;
			g_tSpiTransmit.pbyRxData[byOutidx] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+1] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+2] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+3] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+4] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+5] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+6] = ptSpiBase->DR;
			g_tSpiTransmit.pbyRxData[byOutidx+7] = ptSpiBase->DR;
			
			byOutidx += 8;	
		}
		
		if(byRemainder != 0)
		{	
			switch(byRemainder)
			{
				case 1:
					ptSpiBase->DR = byLastTxBuff[0];
					break;
				case 2:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];
					break;
				case 3:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];
					break;
				case 4:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
					break;	
				case 5:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
					ptSpiBase->DR = byLastTxBuff[4];
					break;
				case 6:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
					ptSpiBase->DR = byLastTxBuff[4];ptSpiBase->DR = byLastTxBuff[5];
					break;
				case 7:
					ptSpiBase->DR = byLastTxBuff[0];ptSpiBase->DR = byLastTxBuff[1];ptSpiBase->DR = byLastTxBuff[2];ptSpiBase->DR = byLastTxBuff[3];
					ptSpiBase->DR = byLastTxBuff[4];ptSpiBase->DR = byLastTxBuff[5];ptSpiBase->DR = byLastTxBuff[6];
					break;	
				default:
					break;
			}
			wTimeStart = SPI_SEND_TIMEOUT;
			while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;} 

			for(byIdx=0;byIdx<byRemainder;byIdx++)		//read buffer data
				g_tSpiTransmit.pbyRxData[byOutidx+byIdx] = ptSpiBase->DR;		
		}

	}
	else
	{
		while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY ) && (wTimeStart --) ){;} 
		ptSpiBase->CR1 &= ~SPI_SSE_MSK;
		for(byCount = 0;byCount<byTxsize;byCount++)
		{
			ptSpiBase->DR = g_tSpiTransmit.pbyTxData[byCount];
		}
		ptSpiBase->CR1 |= (SPI_EN << SPI_SSE_POS);
		wTimeStart = SPI_SEND_TIMEOUT;
		while( ( (uint32_t)(ptSpiBase->SR) & SPI_BSY ) && (wTimeStart --) ){;} 
		
		switch(byTxsize)
		{
			case 1:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;
				break;
			case 2:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				break;
			case 3:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;
				break;
			case 4:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[3]=ptSpiBase->DR;
				break;
			case 5:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[3]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[4]=ptSpiBase->DR;
				break;
			case 6:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[3]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[4]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[5]=ptSpiBase->DR;
				break;
			case 7:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[3]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[4]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[5]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[6]=ptSpiBase->DR;
				break;
			case 8:
				g_tSpiTransmit.pbyRxData[0]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[1]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[2]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[3]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[4]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[5]=ptSpiBase->DR;
				g_tSpiTransmit.pbyRxData[6]=ptSpiBase->DR;g_tSpiTransmit.pbyRxData[7]=ptSpiBase->DR;
				break;
			default:
				break;
		}
	}
		
	wTimeStart = SPI_SEND_TIMEOUT;
	while( ((uint32_t)(ptSpiBase->SR) & SPI_BSY) && (wTimeStart --) ){;}
	
	g_tSpiTransmit.byWriteable =  SPI_STATE_IDLE;
	g_tSpiTransmit.byReadable =  SPI_STATE_IDLE;
	
	return tRet;		
}

//---------------------------------------------DMA-------------------------------------------------
/** \brief send data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pData: pointer to buffer data of SPI transmitter.
 *  \param[in] hwSize: number of data to send (byte), hwSize <= 0xfff.
 *  \param[in] byDmaCh: channel of DMA(0 -> 3)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_send_dma(csp_spi_t *ptSpiBase, const void *pData, uint16_t hwSize, uint8_t byDmaCh)
{	
	if(hwSize > 0xfff)
		return CSI_ERROR;
	csi_dma_ch_start(DMA, byDmaCh, (void *)pData, (void *)&(ptSpiBase->DR), hwSize, 1);
	csp_spi_set_txdma(ptSpiBase, SPI_TDMA_EN, SPI_TDMA_FIFO_NFULL);
	
	return CSI_OK;
}

/** \brief receive data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of SPI receive.
 *  \param[in] hwSize: number of data to receive (byte), hwSize <= 0xfff.
 *  \param[in] byDmaCh: channel of DMA(0 -> 3)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_recv_dma(csp_spi_t *ptSpiBase, void *pbyRecv, uint16_t hwSize ,uint8_t byDmaCh)
{
	if(hwSize > 0xfff)
		return CSI_ERROR;
	csp_spi_set_rxdma(ptSpiBase, SPI_RDMA_EN, SPI_RDMA_FIFO_NSPACE);
	csi_dma_ch_start(DMA, byDmaCh, (void *)&(ptSpiBase->DR),(void *)pbyRecv, hwSize, 1);
	
	return CSI_OK;
}
