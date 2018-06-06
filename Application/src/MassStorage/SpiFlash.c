#include "stm32f10x_lib.h"
#include <stdint.h>
#include "SpiFlash.h"

#define FLASH_SPI			SPI2
#define SPI_FLASH_RX_DMA_CHANNAEL			DMA1_Channel4
#define SPI_FLASH_TX_DMA_CHANNAEL			DMA1_Channel5
#define SPI_DR_Address  0x4000380C
#define SPI_FLASH_CS_LOW()       GPIO_ResetBits(GPIOB, GPIO_Pin_12)
#define SPI_FLASH_CS_HIGH()      GPIO_SetBits(GPIOB, GPIO_Pin_12)

#define FLASH_STATUS_REG_WRITE_PROTECT		GPIO_ResetBits(GPIOC, GPIO_Pin_6)
#define FLASH_STATUS_REG_WRITE_UNPROTECT		GPIO_SetBits(GPIOC, GPIO_Pin_6);

#define SPI_FLASH_RX_READ_FLAG				DMA1_FLAG_TC4	
#define SPI_FLASH_RX_CLEAR_FLAG				(DMA1_FLAG_TC4 | DMA1_FLAG_GL4 | DMA1_FLAG_TE4 | DMA1_FLAG_HT4)

#define SPI_FLASH_TX_READ_FLAG				DMA1_FLAG_TC5	
#define SPI_FLASH_TX_CLEAR_FLAG				(DMA1_FLAG_TC5 | DMA1_FLAG_GL5 | DMA1_FLAG_TE5 | DMA1_FLAG_HT5)



#define WINBOND_8M_SSID					0xEF4017
#define WINBOND_8M_FLASH_BLOCK_CNT		128

#define WINBOND_4M_SSID					0xEF4016
#define WINBOND_4M_FLASH_BLOCK_CNT		64

#define GD_16M_SSID							0xC84018
#define GD_16M_FLASH_BLOCK_CNT				256

#define GD_8M_SSID							0xC84017
#define GD_8M_FLASH_BLOCK_CNT				128

#define GD_4M_SSID							0xC84016
#define GD_4M_FLASH_BLOCK_CNT				64

#define GD_2M_SSID							0xC84015
#define GD_2M_FLASH_BLOCK_CNT				32

#define GD_1M_SSID							0xC84014
#define GD_1M_FLASH_BLOCK_CNT				16


void SoftwareDisableWriteProtoct(void);
uint32_t SstReadId(void);



struct SpiFlashInfo SpiFlash;

void SpiFlashHwInit(void)
{
    SPI_InitTypeDef  SPI_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
    DMA_InitTypeDef  DMA_InitStructure;
  
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_SPI2, ENABLE);
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);

    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13 | GPIO_Pin_14 | GPIO_Pin_15;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  								  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_12;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOB, &GPIO_InitStructure);
  
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_6;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOC, &GPIO_InitStructure);
    GPIO_SetBits(GPIOC, GPIO_Pin_6);


    DMA_StructInit(&DMA_InitStructure);
    DMA_DeInit(SPI_FLASH_RX_DMA_CHANNAEL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SPI_DR_Address;
    //DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPI2_Buffer_Rx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    DMA_InitStructure.DMA_BufferSize = 512;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_VeryHigh;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SPI_FLASH_RX_DMA_CHANNAEL, &DMA_InitStructure);
  
    //SPI TX DMA Configure
    DMA_DeInit(SPI_FLASH_TX_DMA_CHANNAEL);
    DMA_InitStructure.DMA_PeripheralBaseAddr = (u32)SPI_DR_Address;
    //DMA_InitStructure.DMA_MemoryBaseAddr = (u32)SPI2_Buffer_Rx;
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralDST;
    DMA_InitStructure.DMA_BufferSize = 0;
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Disable;
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_Byte;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_Byte;
    DMA_InitStructure.DMA_Mode = DMA_Mode_Normal;
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    DMA_Init(SPI_FLASH_TX_DMA_CHANNAEL, &DMA_InitStructure);
    
    SPI_FLASH_CS_HIGH();
								  
    SPI_InitStructure.SPI_Direction = SPI_Direction_2Lines_FullDuplex;
    SPI_InitStructure.SPI_Mode = SPI_Mode_Master;
    SPI_InitStructure.SPI_DataSize = SPI_DataSize_8b;
    SPI_InitStructure.SPI_CPOL = SPI_CPOL_High;
    SPI_InitStructure.SPI_CPHA = SPI_CPHA_2Edge;
    SPI_InitStructure.SPI_NSS = SPI_NSS_Soft;
    SPI_InitStructure.SPI_BaudRatePrescaler = SPI_BaudRatePrescaler_2;
    SPI_InitStructure.SPI_FirstBit = SPI_FirstBit_MSB;
    SPI_InitStructure.SPI_CRCPolynomial = 7;
    SPI_Init(FLASH_SPI, &SPI_InitStructure);
    SPI_I2S_DMACmd(FLASH_SPI, SPI_I2S_DMAReq_Rx, ENABLE);
    SPI_I2S_DMACmd(FLASH_SPI, SPI_I2S_DMAReq_Tx, ENABLE);
    SPI_Cmd(FLASH_SPI, ENABLE); 

  
    SpiFlash.Ssid = SstReadId();
    if(SpiFlash.Ssid == WINBOND_8M_SSID)
    {
        SpiFlash.FlashBlockCnt = WINBOND_8M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;  
    }
    else if(SpiFlash.Ssid == WINBOND_4M_SSID)
    {
        SpiFlash.FlashBlockCnt = WINBOND_4M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else if(SpiFlash.Ssid == GD_16M_SSID)
    {
        SpiFlash.FlashBlockCnt = GD_16M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else if(SpiFlash.Ssid == GD_8M_SSID)
    {
        SpiFlash.FlashBlockCnt = GD_8M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else if(SpiFlash.Ssid == GD_4M_SSID)
    {
        SpiFlash.FlashBlockCnt = GD_4M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else if(SpiFlash.Ssid == GD_2M_SSID)
    {
        SpiFlash.FlashBlockCnt = GD_2M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else if(SpiFlash.Ssid == GD_1M_SSID)
    {
        SpiFlash.FlashBlockCnt = GD_1M_FLASH_BLOCK_CNT;
        SpiFlash.FlashStatus = ENABLE;
    }
    else
    {
        //ReportError(ERROR_FLASH_INIT_ERROR);
        SpiFlash.FlashStatus = DISABLE;
    }
    SpiFlash.FsSectorCnt = SpiFlash.FlashBlockCnt * (SPI_FLASH_BLOCK_SIZE / FS_SECTOR_SIZE);
    SpiFlash.FlashBlockSize = SPI_FLASH_BLOCK_SIZE;
    SpiFlash.FlashSize = SpiFlash.FlashBlockCnt * SpiFlash.FlashBlockSize;
  
    SoftwareDisableWriteProtoct();
}

int32_t CheckEmpty(uint8_t *buff, int32_t cnt)
{
    int32_t i;
    for(i = 0; i < cnt; i++)
    {
        if(buff[i] != 0xFF)
        {
            return 1;
        }
    }
    return 0;
}

u8 spi_write(u8 data)
{
  int i = 0xfff;
  /* Loop while DR register in not emplty */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == RESET);

  /* Send byte through the SPI1 peripheral */
  SPI_I2S_SendData(FLASH_SPI, data);

  /* Wait to receive a byte */
  while (SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_RXNE) == RESET && i)
  {
      i--;
  }

  if(i == 0)
  {
      return SPI_I2S_ReceiveData(FLASH_SPI);
  }

  /* Return the byte read from the SPI bus */
  return SPI_I2S_ReceiveData(FLASH_SPI);
}

void WriteEnable()
{
    SPI_FLASH_CS_LOW();  
    spi_write(0x06);
    SPI_FLASH_CS_HIGH();
}


void WaitFlashFree()
{
    uint8_t ucResult;

    //ClearSpiRxFifo();
    SPI_FLASH_CS_LOW();  
    do
    {
   	spi_write(0x05);
	ucResult = spi_write(0);
    }while(ucResult & 0x01);
    SPI_FLASH_CS_HIGH();
}

void SoftwareDisableWriteProtoct()
{
     FLASH_STATUS_REG_WRITE_UNPROTECT;
    WaitFlashFree();
    WriteEnable();

    SPI_FLASH_CS_LOW();  
    spi_write(0x01);
    spi_write(0x80); //Status Reg 1
    spi_write(0x00);//Status Reg 2
    SPI_FLASH_CS_HIGH();
    FLASH_STATUS_REG_WRITE_PROTECT;
}

uint32_t SstReadId()
{
    uint8_t IdBuff[3];
    int32_t SstId;
    WaitFlashFree();	
    SPI_FLASH_CS_LOW();
    spi_write(0x9F);   
    IdBuff[0] = spi_write(0);   
    IdBuff[1] = spi_write(0);   
    IdBuff[2] = spi_write(0);  
    SPI_FLASH_CS_HIGH();

    SstId = ((IdBuff[0] << 16) | (IdBuff[1] << 8) | IdBuff[2]);
    return SstId;
}

void SectorErase(unsigned int Addr)
{
    uint8_t bytes[2];

    WaitFlashFree();
    WriteEnable();
    
    bytes[0] = (uint8_t)(Addr >> 16);				
    bytes[1] = (uint8_t)((Addr >> 8) & 0xF0);	
	
    SPI_FLASH_CS_LOW();
    spi_write(0x20);    
    spi_write(bytes[0]);         				
    spi_write(bytes[1]);         			
    spi_write(0x00);    
    SPI_FLASH_CS_HIGH();
    WaitFlashFree();
}

void BlockErase(unsigned int Addr)
{
    uint8_t bytes[1];

    WaitFlashFree();
    WriteEnable();
    
    bytes[0] = (unsigned char)(Addr >> 16);				
	
    SPI_FLASH_CS_LOW();
    spi_write(0xD8);    
    spi_write(bytes[0]);         				
    spi_write(0);         			
    spi_write(0x00);    
    SPI_FLASH_CS_HIGH();

    WaitFlashFree();
}

void ChipErase()
{

    WaitFlashFree();
    WriteEnable();
   		
	
    SPI_FLASH_CS_LOW();
    spi_write(0x60);      
    SPI_FLASH_CS_HIGH();

    WaitFlashFree();
}

uint32_t DataFlashReadData(uint32_t StartAddress, uint8_t *ucRdDataBuff, uint32_t ReadCnt)
{
    uint8_t ZeroData = 0;
    uint8_t Cmd[4];
    
    Cmd[0] = 0x03;
    Cmd[1] = (StartAddress >> 16) & 0xFF;
    Cmd[2] = (StartAddress >> 8) & 0xFF;
    Cmd[3] = StartAddress & 0xFF;
    
    WaitFlashFree();
    SPI_FLASH_CS_LOW();

    spi_write(Cmd[0]);              
    spi_write(Cmd[1]);             
    spi_write(Cmd[2]);             
    spi_write(Cmd[3]);         
    


    SPI_FLASH_TX_DMA_CHANNAEL->CCR &= ~DMA_MemoryInc_Enable;
    SPI_FLASH_RX_DMA_CHANNAEL->CNDTR = ReadCnt;
    SPI_FLASH_RX_DMA_CHANNAEL->CMAR = (u32)ucRdDataBuff;
    SPI_FLASH_TX_DMA_CHANNAEL->CNDTR = ReadCnt;
    SPI_FLASH_TX_DMA_CHANNAEL->CMAR = (u32)&ZeroData;
    DMA_Cmd(SPI_FLASH_RX_DMA_CHANNAEL, ENABLE);
    DMA_Cmd(SPI_FLASH_TX_DMA_CHANNAEL, ENABLE);
 
    while (!DMA_GetFlagStatus(SPI_FLASH_TX_READ_FLAG));
    DMA_ClearFlag(SPI_FLASH_TX_CLEAR_FLAG);
    while (!DMA_GetFlagStatus(SPI_FLASH_RX_READ_FLAG));
    DMA_ClearFlag(SPI_FLASH_RX_CLEAR_FLAG);
 
    DMA_Cmd(SPI_FLASH_TX_DMA_CHANNAEL, DISABLE);
    DMA_Cmd(SPI_FLASH_RX_DMA_CHANNAEL, DISABLE);
    
    SPI_FLASH_CS_HIGH();
    return 0;
}

static void DataFlashDirectWriteData(uint32_t StartAddress, uint8_t *ucWrDataBuff, uint32_t WriteCnt)
{
    uint32_t ThisWriteCnt;
    uint8_t *WritePtr;

    SPI_FLASH_TX_DMA_CHANNAEL->CCR |= DMA_MemoryInc_Enable;
    //DMA_CHCTL(FLASH_DMA, SPI_FLASH_TX_DMA_CHANNAEL) |= DMA_MEMORY_INCREASE_ENABLE;
    WritePtr = ucWrDataBuff;
    while(WriteCnt > 0)
    {
        if((StartAddress + WriteCnt) > ((StartAddress & 0xFFFFFF00) + SPI_FLASH_PAGE_SIZE))
        {
            ThisWriteCnt = ((StartAddress & 0xFFFFFF00) + SPI_FLASH_PAGE_SIZE) - StartAddress;
        }
        else
        {
            ThisWriteCnt = WriteCnt;
        }
        
        WaitFlashFree();
        WriteEnable();
        SPI_FLASH_CS_LOW();
 
        spi_write(0x02);          
        spi_write((StartAddress >> 16) & 0xFF);         				
        spi_write((StartAddress >> 8) & 0xFF);         			
        spi_write(StartAddress & 0xFF);   

        SPI_FLASH_TX_DMA_CHANNAEL->CNDTR = ThisWriteCnt;
        SPI_FLASH_TX_DMA_CHANNAEL->CMAR = (u32)(WritePtr);
        DMA_Cmd(SPI_FLASH_TX_DMA_CHANNAEL, ENABLE);

        while (!DMA_GetFlagStatus(SPI_FLASH_TX_READ_FLAG));
        DMA_ClearFlag(SPI_FLASH_TX_CLEAR_FLAG);
    
        DMA_Cmd(SPI_FLASH_TX_DMA_CHANNAEL, DISABLE);
        //while(SPI_I2S_GetFlagStatus(FLASH_SPI, SPI_I2S_FLAG_TXE) == SET);
 	             
        SPI_FLASH_CS_HIGH();

        WritePtr += ThisWriteCnt;
        WriteCnt -= ThisWriteCnt;
        StartAddress += ThisWriteCnt;
    }
    WaitFlashFree();
}

//返回剩余要写入数据的数量
uint32_t DataFlashWriteData(uint32_t Addr, uint8_t *ucWrDataBuff, uint32_t DataCnt)
{
    int32_t i;
    int32_t BuffOffset;
    uint8_t *WritePtr;
    uint8_t ReadTemp[SPI_FLASH_SECTOR_SIZE];
    int32_t ThisWriteCnt;
    WritePtr = ucWrDataBuff;
    while(DataCnt)
    {
        //一个扇区范围内的处理
        BuffOffset = Addr & 0xFFF;
        if(BuffOffset + DataCnt <= SPI_FLASH_SECTOR_SIZE)
        {
            //写入的数据在一个扇区范围内
            ThisWriteCnt = DataCnt;
        }
        else
        {
            //写入的数据超出一个扇区范围外
            ThisWriteCnt = SPI_FLASH_SECTOR_SIZE - BuffOffset;
        }

        DataFlashReadData(Addr, ReadTemp, ThisWriteCnt);
        if(CheckEmpty(ReadTemp, ThisWriteCnt))
        {
            //写入的区域不为空，将整个扇区读取出来，再将需要写入的数据填充到缓冲中，再整个扇区写入
            DataFlashReadData((Addr & 0xFFFFF000), ReadTemp, SPI_FLASH_SECTOR_SIZE);
            SectorErase(Addr);
            for(i = 0; i < ThisWriteCnt; i++)
            {
                ReadTemp[BuffOffset + i] = WritePtr[i];
            }
            DataFlashDirectWriteData((Addr & 0xFFFFF000),  ReadTemp, SPI_FLASH_SECTOR_SIZE);
        }
        else
        {
            //写入的区域为空，直接进行写入操作
            DataFlashDirectWriteData(Addr, WritePtr, ThisWriteCnt);
        }
        WritePtr += ThisWriteCnt;
        Addr += ThisWriteCnt;
        DataCnt -= ThisWriteCnt;
    }

    return DataCnt;
}



