/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : memory.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Memory management layer
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "SpiFlash.h"
#include "memory.h"
#include "usb_scsi.h"
#include "usb_bot.h"
#include "usb_regs.h"
#include "hw_config.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
vu32 Memory_Offset;                   /* Memory Offset */
u32 Transfer_Length;                  /* Transfer Length */
vu32 Block_Read_count = 0;
vu32 Block_offset;
vu32 Counter = 0;
u32 i;
u8 Data_Buffer[SPI_FLASH_SECTOR_SIZE]; /* 512 bytes*/

/* Extern variables ----------------------------------------------------------*/
extern u8 Bulk_Data_Buff[BULK_MAX_PACKET_SIZE];  /* data buffer*/
extern u16 Data_Len;
extern u8 Bot_State;
extern Bulk_Only_CBW CBW;
extern Bulk_Only_CSW CSW;
extern u32 Mass_Memory_Size;
extern u32 Mass_Block_Size;

/* Private function prototypes -----------------------------------------------*/
/* Extern function prototypes ------------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/*******************************************************************************
* Function Name  : Read_Memory
* Description    : Handle the Read operation from the microSD card. 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Read_Memory(void)
{
  if (!Block_Read_count)
  {
    //MSD_ReadBlock(Data_Buffer, Memory_Offset, 512);
    DataFlashReadData(Memory_Offset, Data_Buffer, SPI_FLASH_SECTOR_SIZE);
    UserToPMABufferCopy(Data_Buffer, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
    Block_Read_count = SPI_FLASH_SECTOR_SIZE - BULK_MAX_PACKET_SIZE;
    Block_offset = BULK_MAX_PACKET_SIZE;
  }
  else
  {
    UserToPMABufferCopy(Data_Buffer + Block_offset, ENDP1_TXADDR, BULK_MAX_PACKET_SIZE);
    Block_Read_count -= BULK_MAX_PACKET_SIZE;
    Block_offset += BULK_MAX_PACKET_SIZE;
  }

  SetEPTxCount(ENDP1, BULK_MAX_PACKET_SIZE);
  SetEPTxStatus(ENDP1, EP_TX_VALID);


  Memory_Offset += BULK_MAX_PACKET_SIZE;
  Transfer_Length -= BULK_MAX_PACKET_SIZE;

  CSW.dDataResidue -= BULK_MAX_PACKET_SIZE;
  Led_RW_ON();

  if (Transfer_Length == 0)
  {
    Block_Read_count = 0;
    Block_offset = 0;
    Memory_Offset = 0;
    Bot_State = BOT_DATA_IN_LAST;
    Led_RW_OFF();
  }
}

/*******************************************************************************
* Function Name  : Write_Memory
* Description    : Handle the Write operation to the microSD card.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Write_Memory(void)
{
  u32 temp =  Counter + 64;


  i = 0;
  for (; Counter < temp; Counter++)
  {
    Data_Buffer[Counter] = Bulk_Data_Buff[i];
    i++;
  }

  Memory_Offset += Data_Len;
  Transfer_Length -= Data_Len;

  if (!(Transfer_Length % SPI_FLASH_SECTOR_SIZE))
  {
    Counter = 0;
    //MSD_WriteBlock(Data_Buffer, Memory_Offset - 512, 512);
    DataFlashWriteData(Memory_Offset - SPI_FLASH_SECTOR_SIZE, Data_Buffer, SPI_FLASH_SECTOR_SIZE);
  }

  CSW.dDataResidue -= Data_Len;
  SetEPRxStatus(ENDP2, EP_RX_VALID); /* enable the next transaction*/

  Led_RW_ON();

  if ((Transfer_Length == 0) || (Bot_State == BOT_CSW_Send))
  {
    Counter = 0;
    Set_CSW (CSW_CMD_PASSED, SEND_CSW_ENABLE);
    Led_RW_OFF();
  }
}

/*******************************************************************************
* Function Name  : Address_Management_Test
* Description    : Test the received address. 
* Input          : u8 Cmd : the command can be SCSI_READ10 or SCSI_WRITE10.
* Output         : None.
* Return         : Read\Write status (bool).
*******************************************************************************/
bool Address_Management_Test(u8 Cmd)
{
  vu32 temp1;
  vu32 temp2;

  /* Logical Block Address of First Block */
  temp1 = (CBW.CB[2] << 24) |
          (CBW.CB[3] << 16) |
          (CBW.CB[4] <<  8) |
          (CBW.CB[5] <<  0);
  /* Number of Blocks to transfer */
  temp2 = (CBW.CB[7] <<  8) |
          (CBW.CB[8] <<  0);

  Memory_Offset = temp1 * Mass_Block_Size;
  Transfer_Length = temp2 * Mass_Block_Size;

  if ((Memory_Offset + Transfer_Length) > Mass_Memory_Size)
  {
    if (Cmd == SCSI_WRITE10)
    {
      Bot_Abort(BOTH_DIR);
    }
    Bot_Abort(DIR_IN);
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, ADDRESS_OUT_OF_RANGE);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    return (FALSE);
  }


  if (CBW.dDataLength != Transfer_Length)
  {
    if (Cmd == SCSI_WRITE10)
    {
      Bot_Abort(BOTH_DIR);
    }
    else
    {
      Bot_Abort(DIR_IN);
    }
    Set_Scsi_Sense_Data(ILLEGAL_REQUEST, INVALID_FIELED_IN_COMMAND);
    Set_CSW (CSW_CMD_FAILED, SEND_CSW_DISABLE);
    return (FALSE);
  }
  return (TRUE);
}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/

