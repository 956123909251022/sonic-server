/***********************************************************************//** 
 * \file  spi_demo.c
 * \brief  spi示例代码。
 * w25q16jvsiq(32 block == 512 sector == 2M Byte)
 * 1 page = 256 bytes
 * 1 sector = 16 page (4KB)
 * 1 block = 16 sector (64KB)
 * erasable(1sector,1/2block,1block,chip)
 * Device ID: (M7->M0 = 0xEF), (ID7->ID0 = 0x14)
 * Spi Mode:  MODE0 or MODE3
 * 
 * \copyright Copyright (C) 2015-2021 @ APTCHIP
 * <table>
 * <tr><th> Date  <th>Version    <th>Author  <th>Description
 * <tr><td> 2020-11-24 <td>V0.0  <td>ZJY     <td>initial
 * <tr><td> 2021-11-29 <td>V0.1  <td>LQ      <td>modify
 * </table>
 * *********************************************************************
*/

/* include ----------------------------------------------------------------*/
#include "spi.h"
#include "pin.h"
#include <iostring.h>

//48LQFP SPI PIN
//MOSI		NSS		CLK		MISO
//PA0.6     PA0.7   PA0.8   PA0.9
//PA0.10
//PB0.11    PB0.8   PB0.9   PB0.10
//          PA1.13  

/** \brief spi master send a bunch of data; polling(sync,no interrupt)mode
 *  \brief spi 主机发送一串数据，TX使用轮询
 * 
 *  \param[in] none
 *  \return none
 */
void spi_master_send_demo(void)
{
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);								    //PA07 NSS init high
												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_MASTER;						 //作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE;					     //初始配置无中断
	t_SpiConfig.byTxMode = SPI_TX_MODE_POLL;                 //发送轮询模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi master send demo!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_send(SPI0, bySendData,16);
		csi_spi_nss_high(PA07);
		mdelay(100);
	}	
}

/** \brief spi master send a bunch of data; interrupt(async) mode
 *  \brief spi 主机发送一串数据，TX使用中断
 * 
 *  \param[in] none
 *  \return none
 */
void spi_master_send_int_demo(void)
{
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);								    //PA07 NSS init high
												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_MASTER;						 //作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆			
	t_SpiConfig.byInt= SPI_INTSRC_TXIM;					     //初始配置发送中断
	t_SpiConfig.byTxMode = SPI_TX_MODE_INT;                  //发送使用中断模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi master send int demo!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_send(SPI0, bySendData,16);
		while(SPI_STATE_BUSY == csi_spi_get_state(SPI_SEND));
		csi_spi_nss_high(PA07);
		mdelay(100);
	}	
}


/** \brief spi slave receive a bunch of data; interrupt(async) mode
 *  \brief spi 从机接收一串数据，RX使用中断
 *  \param[in] none
 *  \return none
 */
void spi_slave_receive_int_demo(void)
{
	uint8_t byReceData[16] = {0};
	uint8_t byIndex = 0;
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA07,PA07_SPI0_NSS);					//PA07 = SPI0_NSS							    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_SLAVE;						 //作为从机	
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆,此参数取决于主机			
	t_SpiConfig.byInt=  SPI_INTSRC_RXIM;					 //初始配置接收中断	
	t_SpiConfig.byRxMode = SPI_RX_MODE_INT;                  //接收使用中断模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi slave receive int demo!\n");
	mdelay(100);

	while(1)
	{
		csi_spi_receive(SPI0,byReceData,16);
		while( SPI_STATE_BUSY==csi_spi_get_state(SPI_RECV) );
		my_printf("\nslave receive data is:");
		for(byIndex = 0;byIndex<16;byIndex++)
		{
			my_printf(" %d", byReceData[byIndex]);
		}
	}
}

/** \brief spi master send/receive a bunch of data; polling(sync,no interrupt)mode
 *  \brief spi 主机收发一串数据，收发使用轮询
 *  \param[in] none
 *  \return none
 */
void spi_master_send_receive_demo(void)
{
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byReceData[16] = {0};
	uint8_t byIndex = 0;
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	for(byIndex = 0;byIndex < 16; byIndex++)
	{
		bySendData[byIndex]=byIndex+1;
	}
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);									//PA07 NSS init high												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_MASTER;						 //作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE; 					 //初始配置无中断
	t_SpiConfig.byTxRxMode = SPI_TX_RX_MODE_POLL;            //收发使用轮询模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi master send receive demo!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_send_receive(SPI0, bySendData,byReceData,16);
		csi_spi_nss_high(PA07);
		
		mdelay(100);
	}	
}

/** \brief spi master send/receive a bunch of data; polling(sync,no interrupt)mode
 *  \brief spi 主机收发一串数据，收发使用轮询
 *  \param[in] none
 *  \return none
 */
void spi_master_send_receive_fast_demo(void)
{
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byReceData[16] = {0};
	uint8_t byIndex = 0;
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	for(byIndex = 0;byIndex < 16; byIndex++)
	{
		bySendData[byIndex]=byIndex+1;
	}
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);									//PA07 NSS init high												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_MASTER;						 //作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE; 					 //初始配置无中断
	t_SpiConfig.byTxRxMode = SPI_TX_RX_MODE_POLL;            //收发使用轮询模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi master send receive demo!\n");
	mdelay(500);
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_send_receive_fast(SPI0, bySendData,byReceData,16);
		//csi_spi_send_fast(SPI0, bySendData,16);
		csi_spi_nss_high(PA07);
		
		mdelay(100);
	}	
}

/** \brief spi slave send/receive a bunch of data; interrupt(async) mode
 *  \brief spi 从机收发一串数据，收发使用中断
 *  \param[in] none
 *  \return none
 */
void spi_slave_send_receive_int_demo(void)
{
	uint8_t bySendData[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byReceData[16] = {0};
	uint8_t byIndex = 0;
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA07,PA07_SPI0_NSS);					//PA07 = SPI0_NSS							    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_SLAVE;						 //作为从机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆,此参数取决于主机			
	t_SpiConfig.byInt= SPI_INTSRC_RXIM | SPI_INTSRC_TXIM;	 //初始配置发送和接收中断
	t_SpiConfig.byTxRxMode = SPI_TX_RX_MODE_INT;             //收发使用中断模式
	csi_spi_init(SPI0,&t_SpiConfig);				

	my_printf("the spi slave send receive int demo!\n");
	mdelay(100);
	while(1)
	{
		for(byIndex = 0;byIndex < 16; byIndex++)
		{
			bySendData[byIndex]=byReceData[byIndex];
			
		}
		csi_spi_send_receive(SPI0, bySendData, byReceData, 16);
		while(SPI_STATE_BUSY==csi_spi_get_state(SPI_SEND) || SPI_STATE_BUSY==csi_spi_get_state(SPI_RECV));
	}
}

//----------------------------------------------------------------------------------------------
//w25q16jvsiq demo  (32 block == 512 sector == 2M Byte)
// 1 page = 256 bytes
// 1 sector = 16 page (4KB)
// 1 block = 16 sector (64KB)

//erasable(1sector,1/2block,1block,chip)


#define		PAGE_SIZE				256
#define     W25Q16_CHIPID           0xef14      //chip id
//flash cmd
#define		WREN_CMD				0x06		//write enable
#define		WRDI_CMD				0x04		//write disable
#define		RDSR0_CMD				0x05		//Read Status0
#define		RDSR1_CMD				0x35		//Read Status1
#define		WRSR_CMD				0x01		//Write Status 
#define		READ_CMD				0x03		//ReadData
#define		FREAD_CMD				0x0B		//FastReadData  
#define		PGPRO_CMD				0x02		//PageProgram
#define		FPGPRO_CMD				0x32		//Fast PageProgram
#define		SECTORERASE_CMD			0x20		//setor erase
#define		BKERASE0_CMD			0xD8		//block erase0(64k)
#define		BKERASE1_CMD			0x52		//block erase1(32k)
#define		CHIPERASE_CMD			0xc7		//chip erase
#define		RDDEVICEID_CMD			0x90		//read chip ID ReadElectricManufacturerDeviceID 
#define		RDCHIPID_CMD			0x9F		//read chip ID
	
//flash status reg
//BIT7    6     5     4     3     2     1     0
//SPR0    SEC   TB   BP2    BP1  BP0   WEL   BUSY
//SPR0:Status Protect
//SEC: Sector/Block Protect
//TB:  TOP/BOTTOM Block Protect
//BP3, BP2,BP1,BP0:FLASH Block Protect
//WEL: Write Enable Latch
//BUSY:Busy flage（1 busy：0 idle）
//defualt:0x00

/** \brief flash write enable 
 * 
 *  \param[in] none
 *  \return none
 */
static void spi_flash_write_enable(void)
{
	uint8_t byCmd = WREN_CMD;		//write enable cmd = 0x06
	
	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0,(void *)&byCmd, NULL, 1);	
	csi_spi_nss_high(PA07);
}

/** \brief flash read status reg
 * 
 *  \param[in] none
 *  \return flash status
 */
//BIT7    6     5     4     3     2     1     0
//SPR0    SEC   TB   BP2    BP1  BP0   WEL   BUSY
//SPR0:Status Protect
//SEC: Sector/Block Protect
//TB:  TOP/BOTTOM Block Protect
//BP3, BP2,BP1,BP0:FLASH Block Protect
//WEL: Write Enable Latch
//BUSY:Busy flage（1 busy：0 idle）
//defualt:0x00

static uint8_t spi_flash_read_status(void)
{
	uint8_t bySend[2] = {RDSR0_CMD, 0x00};		//read status cmd0 = 0x05
	uint8_t byRecv[2];

	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0, (void *)bySend, (void *)byRecv, 2);
	csi_spi_nss_high(PA07);
	
	return byRecv[1];
}
/** \brief flash wait idle
 * 
 *  \param[in] none
 *  \return none
 */
static void spi_flash_wait_busy(void)
{
	while((spi_flash_read_status() & 0x01) == 0x01);	// 1: busy, 0: idle
}

/** \brief flash write status 
 * 
 *  \param[in] byStatus: flash status of write
 *  \return none
 */
//SPR0,TB,BP2,BP1,BP0(bit 7,5,4,3,2) 
static void spi_flash_write_Status(uint8_t byStatus)
{
	uint8_t bySend[2] = {WRSR_CMD, byStatus};		//write status cmd = 0x01

	spi_flash_write_enable();		//write enable cmd
	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 2);
	csi_spi_nss_high(PA07);
	spi_flash_wait_busy();
}
/** \brief flash read chip id
 * 
 *  \param[in] none
 *  \return chip id (chip id = 0xef13)
 */
static uint32_t spi_flash_read_id(void)
{
	uint32_t hwId = 0x00;
	uint8_t bySend[6] = {RDDEVICEID_CMD, 0, 0, 0};
	uint8_t byRecv[2];
	
	csi_spi_nss_low(PA07); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);	//send read id cmd and three bytes addr	
	csi_spi_send_receive(SPI0, NULL, (void *)byRecv, 2);	//read id value; id value = 0xef14
	hwId = ((byRecv[0] << 8) |  byRecv[1]);
	csi_spi_nss_high(PA07);
   
	return hwId;
}
/** \brief flash chip erase
 * 
 *  \param[in] none
 *  \return none
 */
static void spi_flash_chip_erase(void)
{
	uint8_t byCmd = CHIPERASE_CMD;
	
	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0, (void *)&byCmd, NULL, 1);		//send chip erase cmd
	csi_spi_nss_high(PA07);
	spi_flash_wait_busy();
}
/** \brief flash sector erase (sector = 4k bytes)
 * 
 *  \param[in] wAddr: flash sector addr
 *  \return none
 */
static void spi_flash_sector_erase(uint32_t wAddr)
{
	wAddr = wAddr & 0xfff000;
	uint8_t bySend[4] = {SECTORERASE_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send sector erase cmd and data three bytes addr
	csi_spi_nss_high(PA07);
	spi_flash_wait_busy();
}

/** \brief flash read bytes
 * 
 *  \param[in] pbyBuf: pointer of read data buf 
 *  \param[in] wAddr: start addr of read data
 *  \param[in] hwNum: length of read data
 *  \return none
 */
static void spi_flash_read_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {READ_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};
	
	csi_spi_nss_low(PA07); 
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send read bytes cmd and data three bytes addr
	csi_spi_send_receive(SPI0, NULL,(void *)pbyBuf, hwNum);		//read hwNum bytes 
	csi_spi_nss_high(PA07);
}

/** \brief flash write enable 
 * 
 *  \param[in] pbyBuf: pointer of write data buf 
 *  \param[in] wAddr: start addr of write data
 *  \param[in] hwNum: length of write data
 *  \return none
 */
static void spi_flash_write_bytes(uint8_t *pbyBuf, uint32_t wAddr, uint16_t hwNum)
{
	uint8_t bySend[4] = {PGPRO_CMD, (wAddr >> 16), (wAddr >> 8), wAddr};

	spi_flash_write_enable();		//write enable
	csi_spi_nss_low(PA07);
	csi_spi_send_receive(SPI0, (void *)bySend, NULL, 4);		//send write bytes cmd and data three bytes addr
	csi_spi_send_receive(SPI0, (void *)pbyBuf, NULL, hwNum);	//write hwNum bytes
	csi_spi_nss_high(PA07);
	spi_flash_wait_busy();
}

/** \brief spi示例代码
 * 
 *  \param[in] none
 *  \return chip id
 */
int16_t spi_w25q16jvsiq_write_read_demo(void)
{
	uint8_t byWrBuf[16] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16};
	uint8_t byRdBuf[16];
	uint8_t byIndex;
	int iRet = 0;	
	csi_spi_config_t t_SpiConfig;  //spi初始化参数配置结构体
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);									//PA07 NSS init high												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	t_SpiConfig.bySpiMode = SPI_MASTER;						 //作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA0; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 2000000; 						 //通讯速率2兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE;					     //初始配置无中断	
    t_SpiConfig.byTxRxMode =  SPI_TX_RX_MODE_POLL;           //收发使用轮询方式 
	csi_spi_init(SPI0,&t_SpiConfig);							

	my_printf("the spi w25q16 test!\r\n");
	
	while(1)
	{
		mdelay(1000);
		iRet = spi_flash_read_id();					//read chip id, chip id = 0xef14	
		if(W25Q16_CHIPID == iRet)
		{
		
			//iRet = spi_flash_read_status();		//read status reg
			my_printf("\r\nflash chip id right:0x%x",iRet);
			
			spi_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 16);
			my_printf("\r\nbefore sector erase read data is:" );
			for(byIndex=0;byIndex<16;byIndex++)
			{
				my_printf("  0x%x",byRdBuf[byIndex]);
			}
			
			spi_flash_sector_erase(0x1000);			//erase sector 1; start addr = 0x1000

			spi_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 16);	//read data = 0xff
			my_printf("\r\nafter sector erase read data is:" );
			for(byIndex=0;byIndex<16;byIndex++)
			{
				my_printf("  0x%x",byRdBuf[byIndex]);
			}
		
			spi_flash_write_bytes(byWrBuf,0x1000,16);				//write 16 bytes
			my_printf("\r\nwill write data is:" );
			for(byIndex=0;byIndex<16;byIndex++)
			{
				my_printf("  0x%x",byWrBuf[byIndex]);
			} 
			
			spi_flash_read_bytes((uint8_t *)byRdBuf, 0x1000, 16);	//read 16 bytes, read bytes = write bytes
			my_printf("\r\nread back data is:" );
			for(byIndex=0;byIndex<16;byIndex++)
			{
				my_printf("  0x%x",byRdBuf[byIndex]);
			}
			break;
			
		}
		else
		{
			my_printf("\r\nflash chip id error:0x%x",iRet);
		}
	}
	return iRet;
}

//spi dma demo
//spi_etcb_dma send
void spi_etcb_dma_send(void)
{
	uint8_t bySdData[100]={1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17,18,19,20};

	csi_dma_ch_config_t tDmaConfig;		//DMA 配置初始化
	csi_etb_config_t 	tEtbConfig;		//ETB 参数配置结构体	
	csi_spi_config_t t_SpiConfig;  		//spi初始化参数配置结构体
	
	for(uint8_t i=0;i<100;i++)
	{
		bySdData[i] = i;
	}
	
	//dma para config
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_INC;				//高位传输原地址自增
	//tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	//tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt			= DMA_INTSRC_NONE;			//不使用中断
	
	//etcb para config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	= ETB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfig.byDstIp 	= ETB_DMA_CH0;					//ETB DMA0通道0作为目标实际
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样硬件触发
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);									//PA07 NSS init high												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	//spi para config
	t_SpiConfig.bySpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 12000000; 						//通讯速率12兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE;					//初始配置无中断	  
					
	
	csi_etb_init();									//使能ETB模块
	csi_etb_ch_config(ETB_CH8, &tEtbConfig);		//初始化ETB，DMA ETB CHANNEL 8-11
	
	
	csi_dma_soft_rst(DMA);
	csi_dma_ch_init(DMA, 0, &tDmaConfig);				//初始化DMA
	
	csi_spi_init(SPI0,&t_SpiConfig);					//初始化并启动spi
	
	csi_spi_nss_low(PA07);
	csi_spi_send_dma(SPI0,(void *)bySdData,100, 0);  //使能dma通道，等待对应dma请求并传输
	csp_dma_t *ptDmaChBase = (csp_dma_t *)DMA_REG_BASE(DMA, 0);
	while(csp_dma_get_curr_htc(ptDmaChBase));//等待直到dma传输完成
	while(csp_spi_get_sr(SPI0) & SPI_BSY );		//wait for transmition finish
    csi_spi_nss_high(PA07);
	nop;
	
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_send_dma(SPI0,(void *)bySdData,100, 0);
		while(csp_dma_get_curr_htc(ptDmaChBase));//等待直到dma传输完成
		while(csp_spi_get_sr(SPI0) & SPI_BSY );	 //wait for transmition finish
		csi_spi_nss_high(PA07);
		nop;
	}

}

//spi dma demo
//spi_etcb_dma send_and_receive
void spi_etcb_dma_send_receive(void)
{	
	uint8_t byChnl = 0;
	uint8_t byChnl1 = 1;
	volatile uint8_t bySrcBuf[104] = {0x02,0x00,0x00,0x00,1, 2 ,3, 4, 5, 6, 7,8,9,10,11,12,13,14,15,16,17};
	volatile uint8_t byDesBuf[104] = {0};
	csi_dma_ch_config_t tDmaConfig; //发送dma配置
	csi_etb_config_t 	tEtbConfig;	//发送ETB参数配置结构体
	csi_dma_ch_config_t tDmaConfig1;//接收dma配置
	csi_etb_config_t 	tEtbConfig1;//接收ETB参数配置结构体	
	csi_spi_config_t t_SpiConfig;   //spi初始化参数配置结构体
	
	for(uint8_t i=4;i<104;i++)
	{
		bySrcBuf[i]=i-3;
	}
	
	//send dma para config
	//tDmaConfig.bySrcLinc 	= DMA_ADDR_INC;				//低位传输原地址自增
	//tDmaConfig.bySrcHinc 	= DMA_ADDR_CONSTANT;		//高位传输原地址固定不变
	tDmaConfig.bySrcLinc 	= DMA_ADDR_CONSTANT;		//低位传输原地址固定不变
	tDmaConfig.bySrcHinc 	= DMA_ADDR_INC;		        //高位传输原地址自增
	tDmaConfig.byDetLinc 	= DMA_ADDR_CONSTANT;		//低位传输目标地址固定不变
	tDmaConfig.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig.byDataWidth 	= DMA_DSIZE_8_BITS;			//传输数据宽度8bit
	tDmaConfig.byReload 	= DMA_RELOAD_DISABLE;		//禁止自动重载
	tDmaConfig.byTransMode 	= DMA_TRANS_ONCE;			//DMA服务模式(传输模式)，连续服务
	tDmaConfig.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;		//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig.byReqMode	= DMA_REQ_HARDWARE;			//DMA请求模式，硬件请求（硬件触发）
	tDmaConfig.wInt			= DMA_INTSRC_NONE;			//不使用中断
	
	//receive dma para config
	tDmaConfig1.bySrcLinc 	= DMA_ADDR_CONSTANT;			//低位传输原地址固定不变
	tDmaConfig1.bySrcHinc 	= DMA_ADDR_CONSTANT;			//高位传输原地址固定不变
	//tDmaConfig1.byDetLinc 	= DMA_ADDR_INC;				//低位传输目标地址自增
	//tDmaConfig1.byDetHinc 	= DMA_ADDR_CONSTANT;		//高位传输目标地址固定不变
	tDmaConfig1.byDetLinc 	= DMA_ADDR_CONSTANT;			//低位传输目标地址固定不变
	tDmaConfig1.byDetHinc 	= DMA_ADDR_INC;					//高位传输目标地址自增
	tDmaConfig1.byDataWidth = DMA_DSIZE_8_BITS;				//传输数据宽度8bit
	tDmaConfig1.byReload 	= DMA_RELOAD_DISABLE;			//禁止自动重载
	tDmaConfig1.byTransMode = DMA_TRANS_ONCE;				//DMA服务模式(传输模式)，连续服务
	tDmaConfig1.byTsizeMode  = DMA_TSIZE_ONE_DSIZE;			//传输数据大小，一个 DSIZE , 即DSIZE定义大小
	tDmaConfig1.byReqMode	= DMA_REQ_HARDWARE;				//DMA请求模式，硬件请求
	tDmaConfig1.wInt		= DMA_INTSRC_NONE;				//不使用中断
	
	//send etcb para config
	tEtbConfig.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig.bySrcIp 	= ETB_SPI0_TXSRC;				//SPI0 TXSRC作为触发源
	tEtbConfig.byDstIp 	= ETB_DMA_CH0;					//ETB DMA通道0作为目标实际
	tEtbConfig.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//receive etcb para config
	tEtbConfig1.byChType = ETB_ONE_TRG_ONE_DMA;			//单个源触发单个目标，DMA方式
	tEtbConfig1.bySrcIp 	= ETB_SPI0_RXSRC;				//SPI0 RXSRC作为触发源
	tEtbConfig1.byDstIp 	= ETB_DMA_CH1;					//ETB DMA通道1作为目标实际
	tEtbConfig1.byTrgMode = ETB_HARDWARE_TRG;			//通道触发模式采样软件触发
	
	//端口配置
	csi_pin_set_mux(PA07, PA07_OUTPUT);                     //PA07 as output
	csi_pin_output_mode(PA07, GPIO_PUSH_PULL);              //PA07 push pull mode
	csi_spi_nss_high(PA07);									//PA07 NSS init high												    
	csi_pin_set_mux(PA08,PA08_SPI0_SCK);					//PA08 = SPI0_SCK
	csi_pin_set_mux(PA09,PA09_SPI0_MISO);					//PA09 = SPI0_MISO
	csi_pin_set_mux(PA06,PA06_SPI0_MOSI);					//PA06 = SPI0_MOSI
	
	//spi para config
	t_SpiConfig.bySpiMode = SPI_MASTER;						//作为主机
	t_SpiConfig.bySpiPolarityPhase = SPI_FORMAT_CPOL0_CPHA1; //clk空闲电平为0，相位为在第二个边沿采集数据
	t_SpiConfig.bySpiFrameLen = SPI_FRAME_LEN_8;             //帧数据长度为8bit
	t_SpiConfig.wSpiBaud = 12000000; 						//通讯速率12兆			
	t_SpiConfig.byInt= SPI_INTSRC_NONE;			//初始配置无中断	  
	
	
	csi_etb_init();									//使能ETB模块
	csi_etb_ch_config(ETB_CH8, &tEtbConfig);		//初始化ETB，通道8用来发送触发
	csi_etb_ch_config(ETB_CH9, &tEtbConfig1);		//初始化ETB，通道9用来接收触发
	
	csi_dma_soft_rst(DMA);
	csi_dma_ch_init(DMA, byChnl, &tDmaConfig);     //dma通道0初始化
	csi_dma_ch_init(DMA, byChnl1, &tDmaConfig1);   //dma通道1初始化

	
	csi_spi_init(SPI0,&t_SpiConfig);				//初始化并启动spi
	
	
	csi_spi_nss_low(PA07);
	csi_spi_recv_dma(SPI0,(void *)byDesBuf,104, byChnl1);
	csi_spi_send_dma(SPI0,(void *)bySrcBuf,104, byChnl);

	csp_dma_t *ptDmaChBase_01 = (csp_dma_t *)DMA_REG_BASE(DMA, 1);
	csp_dma_t *ptDmaChBase_00 = (csp_dma_t *)DMA_REG_BASE(DMA, 0);
	
	while(csp_dma_get_curr_htc(ptDmaChBase_00));	//等待直到dma发送完成
	while(csp_dma_get_curr_htc(ptDmaChBase_01));	//等待直到dma接收完成
	while( (SPI0->SR & SPI_BSY) );					//等到spi传输完成
	csi_spi_nss_high(PA07);

	
	while(1)
	{
		csi_spi_nss_low(PA07);
		csi_spi_recv_dma(SPI0,(void *)byDesBuf,104, 1);
		csi_spi_send_dma(SPI0,(void *)bySrcBuf,104, 0);
		while(csp_dma_get_curr_htc(ptDmaChBase_00));			//等待直到dma发送完成
		while(csp_dma_get_curr_htc(ptDmaChBase_01));			//等待直到dma接收完成
		while( (SPI0->SR & SPI_BSY) );							//等到spi传输完成
		csi_spi_nss_high(PA07);
		nop;
	}

}
