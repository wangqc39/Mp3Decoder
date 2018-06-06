/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "ff.h"
#include "SpiFlash.h"
#include "Mp3Convert.h"




FATFS  fs;            // Work area (file system object) for logical drive
FIL    fsrc, 
       fdst;      // file objects

FRESULT   res;         // FatFs function common result code


volatile int usb_flag = 5;
volatile unsigned int time_now = 0;


volatile int last_time = 0;


void CopyFile(void);
void ConvertTest(void);
       
/*******************************************************************************
* Function Name  : main
* Description    : Main program.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

int ConvertRes;

int main (void)
{   
//可调试用----------------------------------------------------------------------
    #ifdef DEBUG
    debug();
    #endif
    GPIO_InitTypeDef  GPIO_InitStructure ;

//以下为配置--------------------------------------------------------------------    
    RCC_Config();  //配置系统时钟
    NVIC_Config();  //配置  NVIC 和 Vector Table

    SysTick_Config();  //配置延时时钟
    
    TIM2_Config();  //配置定时器2

    SpiFlashHwInit();

    
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
    GPIO_Init(GPIOA, &GPIO_InitStructure);
    GPIO_SetBits(GPIOA, GPIO_Pin_10);

    Set_USBClock();
    Get_Medium_Characteristics();
    USB_Init();
    


 

  //===============================================  
       
//===============================================  

    
    

    




   
      
/*******************************************************************************
**                               主循环              **
*******************************************************************************/

    
//      Delay_Ms(1000);

//      Delay_Ms(1000);
      res = f_mount(0, &fs);
      Delay_Ms(1000);


     //CopyFile();
     ConvertTest();


     GPIO_ResetBits(GPIOA, GPIO_Pin_10);

      while (1)
      {
//          if(time_now != last_time && usb_flag == 0)
//          {
//              name_buff[8] = time_now / 100 % 10 + 0x30;
//              name_buff[9] = time_now /10 % 10 + 0x30;
//              name_buff[10] = time_now % 10 + 0x30;
//              res = f_open(&fsrc, name_buff, FA_CREATE_NEW | FA_WRITE);
//              res = f_write(&fsrc, name_buff, 15, &bw);
//              f_close(&fsrc);

//              last_time = time_now;
//          }
      }
}

uint8_t ConvertFlag;
void ConvertTest()
{
    if(ConvertFlag)
    {
        ConvertRes = ConvertMp3("1.mp3", "1.wav");
    }
}


/***********For Test************/
//uint8_t CopyFlag;
//uint8_t BuffTmp[FS_SECTOR_SIZE];
//uint32_t ReadCnt;
//uint32_t WriteCnt;
//void CopyFile()
//{

//    if(CopyFlag == 0)
//        return;

//    res = f_open(&fsrc, "1.bin", FA_READ);
//    res = f_open(&fdst, "2.bin", FA_CREATE_NEW | FA_WRITE);

//    do
//    {
//        res = f_read(&fsrc, BuffTmp, FS_SECTOR_SIZE, &ReadCnt);
//        if(res != FR_OK || ReadCnt <= 0)
//        {
//            break;
//        }
//        res = f_write(&fdst, BuffTmp, ReadCnt, &WriteCnt);
//        
//    }while(ReadCnt == FS_SECTOR_SIZE);

//    f_close(&fsrc);
//    f_close(&fdst);
//}


