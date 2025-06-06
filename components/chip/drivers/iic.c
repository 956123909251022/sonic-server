/***********************************************************************//** 
 * \file  iic.c
 * \brief  csi iic driver
 * \copyright Copyright (C) 2015-2020 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version  <th>Author  <th>Description
 * <tr><td> 2020-9-03 <td>V0.0  <td>ZJY   <td>initial
 * <tr><td> 2021-1-21 <td>V0.1  <td>ZJY   <td> modified in 110
  * <tr><td> 2021-1-22 <td>V0.2  <td>WNN   <td> move to 102
 * </table>
 * *********************************************************************
*/
#include <sys_clk.h>
#include <drv/iic.h>
#include <drv/irq.h>
#include <drv/gpio.h>
#include <drv/pin.h>
#include <drv/porting.h>
#include <drv/tick.h>



/* Private macro-----------------------------------------------------------*/
/* externs function--------------------------------------------------------*/
/* externs variablesr------------------------------------------------------*/
/* Private variablesr------------------------------------------------------*/
csi_iic_slave_t g_tSlave;
volatile uint32_t g_wIicErrorCont = 0;
volatile uint16_t g_hwReceivecnt = 0;
volatile uint8_t g_bySendIndex = 0;
volatile uint8_t g_byWriteIndex = 0;
volatile uint32_t g_wIicSlaveWriteAddress;


/** \brief deinit iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */  
void apt_iic_deinit(csp_i2c_t *ptIicBase)
{
	csp_i2c_disable(ptIicBase);
	csp_i2c_set_imcr(ptIicBase,0);
    csp_i2c_clr_all_isr(ptIicBase);
}

/** \brief enable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_enable(csp_i2c_t *ptIicBase)
{
	csp_i2c_enable(ptIicBase);
}

/** \brief disable iic 
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_disable(csp_i2c_t *ptIicBase)
{
	csp_i2c_disable(ptIicBase);
}

/** \brief SET iic Tsetup  
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] byTsetup: Tsetup value
 *  \return none
 */ 
void apt_iic_sda_tsetup_config(csp_i2c_t *ptIicBase,uint8_t byTsetup)
{
	csp_i2c_sda_setup(ptIicBase,byTsetup);
}

/** \brief SET iic SDA hold time  
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] byRxHold: RxHold value
 * 	\param[in] hwTxHold: TxHold value
 *  \return none
 */ 
void apt_iic_thold_config(csp_i2c_t *ptIicBase,uint8_t byRxHold,uint16_t hwTxHold)
{
	csp_i2c_sda_hold(ptIicBase,hwTxHold,byRxHold);
}

/** \brief SET iic timeout time  
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wSdaTimeout: SDA timeout value
 * 	\param[in] wSclTimeout: CSL timeout value
 *  \return none
 */ 
void apt_iic_set_timeout(csp_i2c_t *ptIicBase,uint32_t wSdaTimeout,uint32_t wSclTimeout)
{
	csp_i2c_set_sda_tout(ptIicBase,wSdaTimeout);
	csp_i2c_set_scl_tout(ptIicBase,wSclTimeout);
}

/** \brief SET iic rx flsel  
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] byRxFL: RxFL value
 *  \return none
 */ 
void apt_iic_set_rx_flsel(csp_i2c_t *ptIicBase,uint8_t byRxFL)
{
	csp_i2c_set_rx_flsel(ptIicBase, byRxFL);
}

/** \brief SET iic tx flsel  
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] byTxFL: TxFL value
 *  \return none
 */
void apt_iic_set_tx_flsel(csp_i2c_t *ptIicBase,uint8_t byTxFL)
{
	csp_i2c_set_tx_flsel(ptIicBase, byTxFL);
}

/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicSlaveCfg: pointer of iic slave config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_slave_init(csp_i2c_t *ptIicBase, csi_iic_slave_config_t *ptIicSlaveCfg)
{
	uint32_t wIicClk;
	uint16_t hwSclL;
	uint16_t hwSclH;
	if((ptIicBase == NULL)||(ptIicSlaveCfg == NULL))
		return CSI_ERROR;
	csp_pcer0_clk_en(SYSCON,22);
	apt_iic_deinit(ptIicBase);
	
	csp_i2c_set_saddr(ptIicBase, ptIicSlaveCfg->hwSlaveAddr >> 1);

	wIicClk = csi_get_pclk_freq() / 1000000U;
	
	if(ptIicSlaveCfg->bySpeedMode == IIC_BUS_SPEED_STANDARD)
	{
		if(wIicClk<3)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_STANDARD<<1)|(ptIicSlaveCfg->byAddrMode<<3)| I2C_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk *4;
			hwSclL = wIicClk *47/10;
		}
		csp_i2c_set_ss_sclh(ptIicBase,hwSclH);
		csp_i2c_set_ss_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0xf);
		csp_i2c_sda_hold(ptIicBase,0xf,0xf);
		
		
	}else if(ptIicSlaveCfg->bySpeedMode == IIC_BUS_SPEED_FAST){
		if(wIicClk<12)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicSlaveCfg->byAddrMode<<3)| I2C_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 60 / 100 + 1;			
			hwSclL = wIicClk  * 130 / 100 + 1;
			
		}
		
		csp_i2c_set_fs_sclh(ptIicBase,hwSclH);
		csp_i2c_set_fs_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0x2);
		csp_i2c_sda_hold(ptIicBase,0x8,0x3);
	}else if(ptIicSlaveCfg->bySpeedMode == IIC_BUS_SPEED_FAST_PLUS){
		if(wIicClk<32)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicSlaveCfg->byAddrMode<<3)| I2C_SLAVE);
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 26 / 100 + 1;			
			hwSclL = wIicClk  * 50 / 100 + 1;
			
		}
		
		csp_i2c_set_fs_sclh(ptIicBase,hwSclH);
		csp_i2c_set_fs_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0x2);
		csp_i2c_sda_hold(ptIicBase,0x8,0x3);
	}
	apt_iic_set_rx_flsel(ptIicBase,0x0);
	apt_iic_set_tx_flsel(ptIicBase,0x7);
	apt_iic_set_timeout(ptIicBase,ptIicSlaveCfg->wSdaTimeout,ptIicSlaveCfg->wSclTimeout);
	csp_i2c_set_imcr(ptIicBase,ptIicSlaveCfg->hwInt);
	csi_irq_enable((uint32_t *)ptIicBase);
	csi_iic_enable(ptIicBase);
    return CSI_OK;
}

/** \brief initialize iic slave
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] ptIicMasterCfg: pointer of iic master config structure
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_master_init(csp_i2c_t *ptIicBase, csi_iic_master_config_t *ptIicMasterCfg)
{
	
	uint32_t wIicClk;
	uint16_t hwSclL;
	uint16_t hwSclH ;
	if((ptIicBase == NULL)||(ptIicMasterCfg == NULL))
		return CSI_ERROR;
	csp_pcer0_clk_en(SYSCON,22);
	apt_iic_deinit(ptIicBase);
	wIicClk = csi_get_pclk_freq() / 1000000U;
	
	if(ptIicMasterCfg->bySpeedMode == IIC_BUS_SPEED_STANDARD)
	{
		if(wIicClk<3)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_STANDARD<<1)|(ptIicMasterCfg->byAddrMode<<4) | I2C_MASTER |(ptIicMasterCfg->byReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk *4;
			hwSclL = wIicClk *47/10;
		}
		csp_i2c_set_ss_sclh(ptIicBase,hwSclH);
		csp_i2c_set_ss_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0xf);
		csp_i2c_sda_hold(ptIicBase,0xf,0xf);
		
	}else if(ptIicMasterCfg->bySpeedMode == IIC_BUS_SPEED_FAST)
	{
		if(wIicClk<12)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicMasterCfg->byAddrMode<<4) | I2C_MASTER |(ptIicMasterCfg->byReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 60 / 100 + 1;			
			hwSclL = wIicClk  * 130 / 100 + 1;
			
		}
		
		csp_i2c_set_fs_sclh(ptIicBase,hwSclH);
		csp_i2c_set_fs_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0x2);
		csp_i2c_sda_hold(ptIicBase,0x8,0x3);
	}else if(ptIicMasterCfg->bySpeedMode == IIC_BUS_SPEED_FAST_PLUS){
		if(wIicClk<32)
			return CSI_ERROR;
		csp_i2c_set_cr(ptIicBase,(IIC_BUS_SPEED_FAST<<1)|(ptIicMasterCfg->byAddrMode<<4) | I2C_MASTER |(ptIicMasterCfg->byReStart<<5));
		if(0 == wIicClk)
		{
			hwSclH = 8;
			hwSclL = 8;
		}
		else
		{
			hwSclH = wIicClk  * 26 / 100 + 1;			
			hwSclL = wIicClk  * 50 / 100 + 1;
			
		}
		
		csp_i2c_set_fs_sclh(ptIicBase,hwSclH);
		csp_i2c_set_fs_scll(ptIicBase,hwSclL);
		csp_i2c_sda_setup(ptIicBase,0x2);
		csp_i2c_sda_hold(ptIicBase,0x8,0x3);
	}
	
	apt_iic_set_rx_flsel(ptIicBase,0x7);
	apt_iic_set_tx_flsel(ptIicBase,0x7);
	apt_iic_set_timeout(ptIicBase,ptIicMasterCfg->wSdaTimeout,ptIicMasterCfg->wSclTimeout);
	
	csp_i2c_set_imcr(ptIicBase,ptIicMasterCfg->hwInt);
	csi_irq_enable((uint32_t *)ptIicBase);
	
    return CSI_OK;
}


/** \brief  iic  master  write n byte data
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wWriteAdds: Write address
 * 	\param[in] byWriteAddrNumByte: Write address length (unit byte)
 * 	\param[in] pbyIicData: pointer of Write data
 * 	\param[in] wNumByteToWrite: Write data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_write_nbyte(csp_i2c_t *ptIicBase,uint32_t wDevAddr, uint32_t wWriteAdds, uint8_t byWriteAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteToWrite)
{
	uint16_t hwEerorCont=0;
	uint8_t byWriteAdds = 0;
	uint32_t i;
	
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteToWrite == 0)||(byWriteAddrNumByte == 0))
		return CSI_ERROR;
	
	csi_iic_disable(ptIicBase);
	csp_i2c_set_taddr(ptIicBase,wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	
	switch(byWriteAddrNumByte)
	{
		case 1:
				byWriteAdds = wWriteAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
			break;
		case 2:
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
			break;
		case 3:
				byWriteAdds = (wWriteAdds>>16)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				
			break;
		case 4:
				byWriteAdds = (wWriteAdds>>24)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>16)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = (wWriteAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
				byWriteAdds = wWriteAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byWriteAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteToWrite;i++)
	{
		if(i>=wNumByteToWrite-1)
		{
			csp_i2c_set_data_cmd(ptIicBase,*(pbyIicData+i) |I2C_CMD_STOP);
		}
		else
		{
			csp_i2c_set_data_cmd(ptIicBase,*(pbyIicData+i));
		}
		do
		{
			if(hwEerorCont++>=10000)
			{
				csi_iic_disable(ptIicBase);
				csi_iic_enable(ptIicBase);
				return CSI_ERROR;
			}
		}
		while( (csp_i2c_get_status(ptIicBase) & I2C_BUSY) != I2C_BUSY ); 		//Wait for FSM working
		do
		{
			if(hwEerorCont++>=10000)
			{
				csi_iic_disable(ptIicBase);
				csi_iic_enable(ptIicBase);
				return CSI_ERROR;
			}
		}
		while((csp_i2c_get_status(ptIicBase) & I2C_TFNF) != I2C_TFNF);
		hwEerorCont=0;
	}
	
	return CSI_OK;
}

/** \brief  iic  master  read n byte data
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 * 	\param[in] wDevAddr: Addrress of slave device
 *  \param[in] wReadAdds: Read address
 * 	\param[in] byReadAddrNumByte: Read address length (unit byte)
 * 	\param[in] pbyIicData: Read the address pointer of the data storage array
 * 	\param[in] wNumByteRead: Read data length
 *  \return error code \ref csi_error_t
 */ 
csi_error_t csi_iic_read_nbyte(csp_i2c_t *ptIicBase,uint32_t wDevAddr, uint32_t wReadAdds, uint8_t byReadAddrNumByte,volatile uint8_t *pbyIicData,uint32_t wNumByteRead)
{
	uint16_t hwEerorCont=0;
	uint32_t i;
	uint8_t byreadnum = 1;
	uint8_t byReadAdds = 0;
	if((ptIicBase == NULL)||(pbyIicData == NULL)||(wNumByteRead == 0)||(byReadAddrNumByte == 0))
		return CSI_ERROR;
	
	
	csi_iic_disable(ptIicBase);
	csp_i2c_set_taddr(ptIicBase,wDevAddr >> 1);
	csi_iic_enable(ptIicBase);
	
	switch(byReadAddrNumByte)
	{
		case 1:
				byReadAdds = wReadAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds|I2C_CMD_RESTART1);
			break;
		case 2:
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds|I2C_CMD_RESTART1);
				byReadAdds = wReadAdds;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
			break;
		case 3:
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds|I2C_CMD_RESTART1);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
				
			break;
		case 4:
				byReadAdds = (wReadAdds>>24)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds|I2C_CMD_RESTART1);
				byReadAdds = (wReadAdds>>16)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
				byReadAdds = (wReadAdds>>8)&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
				byReadAdds = wReadAdds&0xff;
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_WRITE|byReadAdds);
			break;	
		default:
			break;
	}
	for(i=0;i<wNumByteRead;i++)
	{
		
		if(byreadnum == 1)
		{	
			if(wNumByteRead > 1)
			{
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_READ);
				byreadnum = 0;
			}				
		}	
		if(wNumByteRead == 1)
		{
			csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_READ|I2C_CMD_STOP);
		}else{
		
			if(i>=wNumByteRead-2)
			{
				if(i == wNumByteRead-2)
					csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_READ|I2C_CMD_STOP);
			}
			else
			{
				csp_i2c_set_data_cmd(ptIicBase,I2C_CMD_READ);
			}
		}

		do
		{
			if(hwEerorCont++>=10000)
			{
				return CSI_ERROR;
			}
		}
		while( (csp_i2c_get_status(ptIicBase) & I2C_BUSY) != I2C_BUSY ); 		//Wait for FSM working
		do
		{
			if(hwEerorCont++>=10000)
			{
				return CSI_ERROR;
			}
		}
		while( (csp_i2c_get_status(ptIicBase) & I2C_RFNE) != I2C_RFNE ); 		//Wait for RX done
		*(pbyIicData+i)=csp_i2c_get_data(ptIicBase);

	}
	return CSI_OK;
}

/** \brief  set iic slave  tx/rx buffer
 * 
 * 	\param[in] pbyIicRxBuf: pointer of iic RX data buffer
 *  \param[in] hwIicRxSize: Rx buffer size
 * 	\param[in] pbyIicTxBuf: pointer of iic TX data buffer
 * 	\param[in] hwIicTxSize: Tx buffer size
 *  \return none
 */ 
void csi_iic_set_slave_buffer(volatile uint8_t *pbyIicRxBuf,uint16_t hwIicRxSize,volatile uint8_t *pbyIicTxBuf,uint16_t hwIicTxSize)
{
	g_tSlave.pbySlaveRxBuf = pbyIicRxBuf;
	g_tSlave.pbySlaveTxBuf = pbyIicTxBuf;
	g_tSlave.hwRxSize = hwIicRxSize;
	g_tSlave.hwTxSize = hwIicTxSize;
}



/** \brief  IIC slave sends after receiving request (to be called in IIC IRQhandler)
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \return none
 */ 
void csi_iic_slave_receive_send(csp_i2c_t *ptIicBase)
{
	if((csp_i2c_get_isr(ptIicBase)&I2C_SCL_SLOW_INT)||(csp_i2c_get_isr(ptIicBase)&I2C_TX_ABRT_INT))			 //SCLK锁死,IIC发送中止))
	{
		csi_iic_disable(ptIicBase);
		csp_i2c_set_data_cmd(ptIicBase, 0x00);
		csi_iic_enable(ptIicBase);
		g_bySendIndex=0;
		csp_i2c_clr_isr(ptIicBase,I2C_SCL_SLOW_INT|I2C_TX_ABRT_INT);
		g_wIicErrorCont=0;
		csp_i2c_imcr_disable(ptIicBase,I2C_TX_EMPTY_INT);
		
	}else
	{
		if(csp_i2c_get_isr(ptIicBase)&I2C_RX_FULL_INT)        //有接收到数据
		{
			
			if(g_hwReceivecnt == 0)
			{
				g_wIicSlaveWriteAddress = csp_i2c_get_data(ptIicBase);
				g_byWriteIndex = g_wIicSlaveWriteAddress;
			}
			else
			{
				if(g_wIicSlaveWriteAddress<g_tSlave.hwRxSize)
				{
					*(g_tSlave.pbySlaveRxBuf+g_wIicSlaveWriteAddress)= csp_i2c_get_data(ptIicBase);
					g_byWriteIndex = *(g_tSlave.pbySlaveRxBuf+g_wIicSlaveWriteAddress);
				}else
				{
					g_byWriteIndex = csp_i2c_get_data(ptIicBase);
				}
				g_wIicSlaveWriteAddress++;
			}
			csp_i2c_clr_isr(ptIicBase,I2C_RX_FULL_INT);
			g_wIicErrorCont=0;
			g_hwReceivecnt++;
		}
		
		if(csp_i2c_get_isr(ptIicBase)&I2C_RD_REQ_INT)			//读请求产生
		{
			if(g_bySendIndex==0)
			{
				g_bySendIndex=1;
				csp_i2c_imcr_enable(ptIicBase,I2C_TX_EMPTY_INT);
				if(g_byWriteIndex<g_tSlave.hwTxSize)
				{
					csp_i2c_set_data_cmd(ptIicBase, *(g_tSlave.pbySlaveTxBuf+g_byWriteIndex));
					
				}else{
					csp_i2c_set_data_cmd(ptIicBase, 0xFF);
				}
				g_byWriteIndex++;
				
			}

			csp_i2c_clr_isr(ptIicBase,I2C_RD_REQ_INT);
			g_wIicErrorCont=0;
		} 

		if(csp_i2c_get_isr(ptIicBase)&I2C_TX_EMPTY_INT)				//IIC发送为空
		{
			if(g_bySendIndex==1)
			{
				if(g_byWriteIndex<g_tSlave.hwTxSize)
				{
					csp_i2c_set_data_cmd(ptIicBase, *(g_tSlave.pbySlaveTxBuf+g_byWriteIndex));
				}
				else{
					csp_i2c_set_data_cmd(ptIicBase, 0xFF);
				}
				g_byWriteIndex++;
			}
			else
			{
				csp_i2c_imcr_disable(ptIicBase,I2C_TX_EMPTY_INT);
				if(g_wIicErrorCont>10000)
				{
					csi_iic_disable(ptIicBase);
					csp_i2c_set_data_cmd(ptIicBase, 0x00);
					csi_iic_enable(ptIicBase);
					g_wIicErrorCont=0;
				}
				else
				{
					g_wIicErrorCont++;
				}
			}
			csp_i2c_clr_isr(ptIicBase,I2C_TX_EMPTY_INT);
		}
		if(csp_i2c_get_isr(ptIicBase)&I2C_STOP_DET_INT)
		{
			csp_i2c_imcr_disable(ptIicBase,I2C_TX_EMPTY_INT);
			csp_i2c_clr_isr(ptIicBase,I2C_STOP_DET_INT);
			if(g_bySendIndex!=0)
			{
				g_bySendIndex=0;
			}
			g_wIicErrorCont=0;
			g_hwReceivecnt = 0;
		}
	
	
	}
	
}

/** \brief set iic SPKLEN
 * 
 *  \param[in] ptIicBase: pointer of iic register structure
 *  \param[in] bySpklenCfg: filter set 
 *  \return none
 */ 
void csi_iic_spklen_set(csp_i2c_t *ptIicBase, uint8_t bySpklen)
{
	csp_i2c_set_spklen(ptIicBase,bySpklen);
}



