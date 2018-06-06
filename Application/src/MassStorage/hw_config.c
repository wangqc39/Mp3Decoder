/******************** (C) COPYRIGHT 2007 STMicroelectronics ********************
* File Name          : hw_config.c
* Author             : MCD Application Team
* Version            : V1.0
* Date               : 10/08/2007
* Description        : Hardware Configuration & Setup
********************************************************************************
* THE PRESENT SOFTWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH SOFTWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/
#include "stm32f10x_it.h"
#include "hw_config.h"
#include "usb_lib.h"
#include "SpiFlash.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
u32 Mass_Memory_Size;
u32 Mass_Block_Size;
u32 Mass_Block_Count;
//sMSD_CSD MSD_csd;
//ErrorStatus HSEStartUpStatus;

/* Extern variables ----------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/



/*******************************************************************************
* Function Name  : Set_USBClock
* Description    : Configures USB Clock input (48MHz)
* Input          : None.
* Return         : None.
*******************************************************************************/
void Set_USBClock(void)
{
  /* USBCLK = PLLCLK */
  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_1Div5);      //8MHz
//  RCC_USBCLKConfig(RCC_USBCLKSource_PLLCLK_Div1);

  /* Enable USB clock */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_USB, ENABLE);
}

/*******************************************************************************
* Function Name  : Enter_LowPowerMode
* Description    : Power-off system clocks and power while entering suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Enter_LowPowerMode(void)
{
}

/*******************************************************************************
* Function Name  : Leave_LowPowerMode
* Description    : Restores system clocks and power while exiting suspend mode
* Input          : None.
* Return         : None.
*******************************************************************************/
void Leave_LowPowerMode(void)
{
}



/*******************************************************************************
* Function Name  : Led_Config
* Description    : configure the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;

  /* Enable GPIOC clock */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);

  /* Configure PC.06, PC.07, PC.08 and PC.09 as output push-pull */
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_6 | GPIO_Pin_7 /*| GPIO_Pin_8 | GPIO_Pin_9*/;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_Init(GPIOC, &GPIO_InitStructure);
}

/*******************************************************************************
* Function Name  : Led_RW_ON
* Description    : Turn ON the Read/Write LEDs.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_ON(void)
{
  GPIO_SetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 /*| GPIO_Pin_8 | GPIO_Pin_9*/);
}

/*******************************************************************************
* Function Name  : Led_RW_OFF
* Description    : Turn off the Read/Write LEDs. 
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Led_RW_OFF(void)
{
  GPIO_ResetBits(GPIOC, GPIO_Pin_6 | GPIO_Pin_7 /*| GPIO_Pin_8 | GPIO_Pin_9*/);
}

/*******************************************************************************
* Function Name  : USB_Cable_Config
* Description    : Software Connection/Disconnection of USB Cable.
* Input          : None.
* Return         : Status
*******************************************************************************/
void USB_Cable_Config (FunctionalState NewState)
{
 /* if (NewState != DISABLE)//--------------ÍòÀû
  {
    GPIO_ResetBits(GPIOD, GPIO_Pin_8);
  }
  else
  {
    GPIO_SetBits(GPIOD, GPIO_Pin_8);
  }*/

  if (NewState != DISABLE)//-----------»ðÅ£
  {
    GPIO_ResetBits(GPIOA, GPIO_Pin_10);
  }
  else
  {
    GPIO_SetBits(GPIOA, GPIO_Pin_10);
  }


  
}

/*******************************************************************************
* Function Name  : Get_Medium_Characteristics.
* Description    : Get the microSD card size.
* Input          : None.
* Output         : None.
* Return         : None.
*******************************************************************************/
void Get_Medium_Characteristics(void)
{

  //MSD_GetCSDRegister(&MSD_csd);

  //temp1 = MSD_csd.DeviceSize + 1;
  //temp2 = 1 << (MSD_csd.DeviceSizeMul + 2);

//  Mass_Block_Count = SpiFlash.FlashSize / 512;//temp1 * temp2;

//  Mass_Block_Size =  512;// 1 << MSD_csd.RdBlockLen;

//  Mass_Memory_Size = (Mass_Block_Count * Mass_Block_Size);

  Mass_Block_Count = SpiFlash.FsSectorCnt;//temp1 * temp2;

  Mass_Block_Size =  FS_SECTOR_SIZE;// 1 << MSD_csd.RdBlockLen;

  Mass_Memory_Size = (Mass_Block_Count * Mass_Block_Size);

}

/******************* (C) COPYRIGHT 2007 STMicroelectronics *****END OF FILE****/
