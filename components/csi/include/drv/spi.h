/***********************************************************************//** 
 * \file  spi.h
 * \brief  head file of csi spi
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version   <th>Author  <th>Description
 * <tr><td> 2020-8-28 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-1-12 <td>V0.1  <td>ZJY     <td>modify
 * <tr><td> 2021-11-29 <td>V0.2  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

#ifndef _DRV_SPI_H_
#define _DRV_SPI_H_

#include <stdint.h>

#include <drv/common.h>
#include <drv/gpio.h>
#include <drv/dma.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 *  \enum    csi_spi_mode_t
 *  \brief   Function mode of spi
 */
typedef enum {
    SPI_MASTER,             ///< SPI Master (Output on MOSI, Input on MISO)
    SPI_SLAVE               ///< SPI Slave  (Output on MISO, Input on MOSI)
} csi_spi_mode_e;

/**
 *  \enum     csi_spi_frame_len_t
 *  \brief    SPI data width (4bit ~ 16bit)
 */

typedef enum {
    SPI_FRAME_LEN_4  = 4,
    SPI_FRAME_LEN_5,
    SPI_FRAME_LEN_6,
    SPI_FRAME_LEN_7,
    SPI_FRAME_LEN_8,
    SPI_FRAME_LEN_9,
    SPI_FRAME_LEN_10,
    SPI_FRAME_LEN_11,
    SPI_FRAME_LEN_12,
    SPI_FRAME_LEN_13,
    SPI_FRAME_LEN_14,
    SPI_FRAME_LEN_15,
    SPI_FRAME_LEN_16
} csi_spi_frame_len_e;

/**
 *  \enum     csi_spi_format_t
 *  \brief    Timing format of spi
 */
typedef enum {
    SPI_FORMAT_CPOL0_CPHA0 = 0,  ///< Clock Polarity 0, Clock Phase 0
    SPI_FORMAT_CPOL0_CPHA1,      ///< Clock Polarity 0, Clock Phase 1
    SPI_FORMAT_CPOL1_CPHA0,      ///< Clock Polarity 1, Clock Phase 0
    SPI_FORMAT_CPOL1_CPHA1       ///< Clock Polarity 1, Clock Phase 1
} csi_spi_cp_format_e;

/**
 *  \enum     csi_spi_event_t
 *  \brief    Signaled event for user by driver
 */
typedef enum {
    SPI_EVENT_SEND_COMPLETE,           ///< Data Send completed. Occurs after call to csi_spi_send_async to indicate that all the data has been send over
    SPI_EVENT_RECEIVE_COMPLETE,        ///< Data Receive completed. Occurs after call to csi_spi_receive_async to indicate that all the data has been received
    SPI_EVENT_SEND_RECEIVE_COMPLETE,   ///< Data Send_receive completed. Occurs after call to csi_spi_send_receive_async to indicate that all the data has been send_received
    SPI_EVENT_RECEIVE_SLAVE_COMPLE,    ///< Data Receive completed. Occurs after call to csi_spi_receive_async to indicate that all the data has been received
	SPI_EVENT_ERROR_OVERFLOW,          ///< Data overflow: Receive overflow
    SPI_EVENT_ERROR_UNDERFLOW,         ///< Data underflow: Transmit underflow
    SPI_EVENT_ERROR                    ///< Master Mode Fault (SS deactivated when Master).Occurs in master mode when Slave Select is deactivated and indicates Master Mode Fault
} csi_spi_event_e;

/**
 *  \enum     csi_spi_int_e
 *  \brief    Interrupt source set of spi
 */
typedef enum
{
	SPI_INTSRC_NONE		= (0x00ul << 0),   		//none interrupt 
	SPI_INTSRC_ROTIM	= (0x01ul << 0), 		//Receive Overrun Interrupt              
	SPI_INTSRC_RTIM    	= (0x01ul << 1), 		//Receive Timeout Interrupt          
	SPI_INTSRC_RXIM   	= (0x01ul << 2),		//Receive FIFO Interrupt           
	SPI_INTSRC_TXIM		= (0x01ul << 3)    		//Transmit FIFO interrupt 
}csi_spi_intsrc_e;


/**
 * \enum     csi_spi_work_e
 * \brief    SPI tx/rx work mode
 */
typedef enum{
	//send mode
	SPI_TX_MODE_POLL	=	0,			//polling mode, no interrupt
	SPI_TX_MODE_INT		=	1,			//tx use interrupt mode
	//receive
	SPI_RX_MODE_POLL	=	0,			//polling mode, no interrupt
	SPI_RX_MODE_INT	    =	1,			//rx use interrupt mode	
	
	//send and receive
	SPI_TX_RX_MODE_POLL	=	0,			//tx/rx polling mode, no interrupt
	SPI_TX_RX_MODE_INT	=	3			//tx/rx use interrupt mode	
	
}csi_spi_work_e;

/**
 * \enum     csi_spi_work_mode_e
 * \brief    SPI tx/rx work mode
 */	
typedef enum{
	
	SPI_SEND = 0,
	SPI_RECV
}csi_spi_work_mode_e;

/**
 * \enum     csi_spi_state_e
 * \brief    SPI working status
 */
typedef enum {
	SPI_STATE_IDLE		= 0,			//spi idle(rx/tx)
	SPI_STATE_BUSY                      //spi busy(rx/tx)
}csi_spi_state_e;


typedef struct 
{	
	uint32_t     wSpiBaud;			  //spi clk
	uint8_t      bySpiMode;	    	  //0:master   1:slave
	uint8_t 	 bySpiPolarityPhase;  //0:(0 0)   1:(0 1)  2:(1 0) 3:(1 1)
	uint8_t 	 bySpiFrameLen;       //4-16 bit
	uint8_t      byInt;            	  //int source
	uint8_t      byTxMode;            //send mode: polling/interrupt
	uint8_t      byRxMode;            //recv mode: polling/interrupt
	uint8_t      byTxRxMode;          //send/receive mode: polling/interrupt
}csi_spi_config_t;

typedef struct
{
	uint8_t             *pbyTxData;      //< Output data buf
	uint8_t             *pbyRxData;      //< Input  data buf
	uint8_t             bySendMode;      // send mode
	uint8_t             byRecvMode;      // receive mode
    uint16_t            byTxSize;        //< Output data size specified by user
    uint16_t            byRxSize;        //< Input  data size specified by user
	uint8_t             byRxFifoLength;  //< receive fifo length
	uint8_t             byTxFifoLength;  // send fifo  threshold
	uint8_t             byInt;  		 //< interrupt
	uint8_t             byWorkMode;      //< master or slave
	
    //csi_state_t         tState;          //< Peripheral state
	uint8_t             byReadable;
	uint8_t             byWriteable;
	uint8_t             bySendRecMode;
	
}csi_spi_transmit_t;
extern csi_spi_transmit_t g_tSpiTransmit; 

/** \brief initialize spi data structure
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] ptSpiCfg: pointer ofuser spi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_init(csp_spi_t *ptSpiBase,csi_spi_config_t *ptSpiCfg);

/** \brief uninit spi data structure
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_uninit(csp_spi_t *ptSpiBase);

/** \brief set spi mode, master or slave
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eMode: master, slave
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_mode(csp_spi_t *ptSpiBase, csi_spi_mode_e eMode);

/** \brief config spi cp format
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eFormat: spi cp format
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_cp_format(csp_spi_t *ptSpiBase, csi_spi_cp_format_e eFormat);

/** \brief config spi work frequence
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] wBaud: spi work baud
 *  \return spi config frequency
 */
uint32_t csi_spi_baud(csp_spi_t *ptSpiBase, uint32_t wBaud);

/** \brief config spi frame length
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] eLength: frame length
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_frame_len(csp_spi_t *ptSpiBase, csi_spi_frame_len_e eLength);

/** \brief csi_spi_nss_high 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_spi_nss_high(pin_name_e ePinName);

/** \brief csi_spi_nss_low 
 * 
 *  \param[in] ePinName:which pin use as nss
 *  \return none
 */ 
void csi_spi_nss_low(pin_name_e ePinName);


/** \brief get the tState of spi device
 * 
 *  \param[in] eWorkMode
 *  \return read/write state or CSI_ERROR/CSI_OK
 */ 
int8_t csi_spi_get_state(csi_spi_work_mode_e eWorkMode);

/** \brief sending data to spi transmitter
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
int32_t csi_spi_send(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);

/** \brief sending data to spi transmitter, non-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to send(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_async(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);


/** \brief  receiving data from spi receiver
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to receive
 *  \param[in] wSize: number of data to receive(byte)
 *  \return return the num of data or  return Error code
 */
int32_t csi_spi_receive(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);

/** \brief  receiving data from spi receiver, not-blocking mode(interrupt mode)
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pData: pointer to buffer with data to send to spi transmitter
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_receive_async(csp_spi_t *ptSpiBase, void *pData, uint32_t wSize);

/** \brief  sending and receiving data from spi receiver
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataout: pointer to buffer with data to send to spi transmitter
 *  \param[in] pDatain: number of data to receive(byte)
 *  \param[in] wSize: number of data to receive(byte)
 *  \return  the num of data which is send successfully or CSI_ERROR/CSI_OK
 */
int32_t csi_spi_send_receive(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize);

/** \brief  receiving data from spi receiver, not-blocking mode
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataout: pointer to buffer with data to send to spi transmitter
 *  \param[in] pDatain: number of data to receive(byte)
 *  \param[in] wSize: number of data to receive(byte)
 *  \return error code \ref csi_error_t
 */
csi_error_t csi_spi_send_receive_async(csp_spi_t *ptSpiBase, void *pDataout, void *pDatain, uint32_t wSize);

/** \brief  transmission variables init ,user not change it
 * 
 *  \param[in] ptSpiCfg:pointer of user spi parameter config
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_Internal_variables_init(csi_spi_config_t *ptSpiCfg);

//interrupt
/** \brief spi interrupt handle function
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */ 
void spi_irqhandler(csp_spi_t *ptSpiBase);

/** \brief clr spi rx fifo
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \return none
 */
void csi_spi_clr_rxfifo(csp_spi_t *ptSpiBase);

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
csi_error_t csi_spi_send_fast(csp_spi_t *ptSpiBase,void *pDataOut,uint8_t bySize);

/** \brief spi send and receive fast
 * 
 *  \param[in] ptSpiBase: pointer of spi register structure
 *  \param[in] pDataOut :send data buffer pointer 
 *  \param[in] pDataIn  :receive data buffer pointer 
 *  \param[in] wSize ：length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_spi_send_receive_fast(csp_spi_t *ptSpiBase, uint8_t *pDataOut,uint8_t *pDataIn, uint32_t wSize);
//-------------------------------------------------------------------------------------------------------------------------------------
//DMA
/** \brief send data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pData: pointer to buffer data of SPI transmitter.
 *  \param[in] hwSize: number of data to send (byte), hwSize <= 0xfff.
 *  \param[in] byDmaCh: channel of DMA(0 -> 3)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_send_dma(csp_spi_t *ptSpiBase, const void *pData, uint16_t hwSize, uint8_t byDmaCh);

/** \brief receive data of spi by DMA
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \param[in] pbyRecv: pointer to buffer data of SPI receive.
 *  \param[in] hwSize: number of data to receive (byte), hwSize <= 0xfff.
 *  \param[in] byDmaCh: channel of DMA(0 -> 3)
 *  \return  error code \ref csi_error_t
 */
csi_error_t csi_spi_recv_dma(csp_spi_t *ptSpiBase, void *pbyRecv, uint16_t hwSize,  uint8_t byDmaCh);

/** \brief spi receive data,interrupt call 
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
void apt_spi_intr_recv_data(csp_spi_t *ptSpiBase);

/** \brief spi send data,interrupt call
 * 
 *  \param[in] ptSpiBase: pointer of SPI reg structure.
 *  \return none
 */ 
void apt_spi_intr_send_data(csp_spi_t *ptSpiBase);

#ifdef __cplusplus
}
#endif

#endif /* _DRV_SPI_H_ */
