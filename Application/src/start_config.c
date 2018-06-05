#include "start_config.h"

TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
TIM_OCInitTypeDef  TIM_OCInitStructure;
TIM_ICInitTypeDef  TIM_ICInitStructure;
 
ErrorStatus HSEStartUpStatus;


/*******************************************************************************
* Function Name  : RCC_Configuration
* Description    : Configures the different system clocks.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/

    //改变指定管脚的映射  GPIO_Remap_SWJ_Disable SWJ 完全失能（JTAG+SW-DP）
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    
    //改变指定管脚的映射  GPIO_Remap_SWJ_JTAGDisable  JTAG-DP 失能 + SW-DP使能
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

void RCC_Config(void)
{
  ErrorStatus HSEStartUpStatus;
  
  //将外设 RCC寄存器重设为缺省值
  RCC_DeInit();

  //设置外部高速晶振（HSE）
  RCC_HSEConfig(RCC_HSE_ON);

  //等待 HSE 起振 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    //预取指缓存使能
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     //设置代码延时值
    //FLASH_Latency_2  2 延时周期
    FLASH_SetLatency(FLASH_Latency_2);

    //设置 AHB 时钟（HCLK）
    //RCC_SYSCLK_Div1  AHB 时钟 =  系统时钟 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

     //设置高速 AHB 时钟（PCLK2）
    //RCC_HCLK_Div2  APB1 时钟  = HCLK / 2 
    RCC_PCLK2Config(RCC_HCLK_Div1);

    //设置低速 AHB 时钟（PCLK1）
    //RCC_HCLK_Div2  APB1 时钟  = HCLK / 2 
    RCC_PCLK1Config(RCC_HCLK_Div2);

    // PLLCLK = 8MHz * 9 = 72 MHz 
    //设置 PLL 时钟源及倍频系数
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    //使能或者失能 PLL
    RCC_PLLCmd(ENABLE);

    //等待指定的 RCC 标志位设置成功 等待PLL初始化成功
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }


    //设置系统时钟（SYSCLK） 设置PLL为系统时钟源
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    //等待PLL成功用作于系统时钟的时钟源
    //  0x00：HSI 作为系统时钟 
    //  0x04：HSE作为系统时钟 
    //  0x08：PLL作为系统时钟  
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
    
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//打开串口1
   
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);//使能WWDG软件看门狗
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//使能定时器2  

  
    /* PD.08 used as USB pull-up */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//----------火牛
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  //GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
  GPIO_Init(GPIOC, &GPIO_InitStructure);*/





    
}



/*******************************************************************************
* Function Name  : NVIC_Configuration
* Description    : Configures NVIC and Vector Table base location.
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
void NVIC_Config(void)
{
  NVIC_InitTypeDef NVIC_InitStructure;
  
#ifdef  VECT_TAB_RAM
  /* Set the Vector Table base location at 0x20000000 */
  NVIC_SetVectorTable(NVIC_VectTab_RAM, 0x0);
#else  /* VECT_TAB_FLASH  */
  /* Set the Vector Table base location at 0x08000000 */
  NVIC_SetVectorTable(NVIC_VectTab_FLASH, 0x0);
#endif
  
  /* Configure the NVIC Preemption Priority Bits */  
  NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2);

 //---------------------开相应的中断---------------------------// 
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;  //开串口1中断
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //中断占先等级0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //开中断
  NVIC_Init(&NVIC_InitStructure);  //初始化中断
 
  //SysTick定时延时中断
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,1,0);  
  //中断等级设置。一般设置的高一些会少受其他的影响
  /*
  //WWDG软件看门狗中断
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQChannel;  //通道
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //占先中断等级
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //响应中断优先级
  NVIC_Init(&NVIC_InitStructure);  //打开中断
  */
  //定时器2中断
  TIM_ClearFlag(TIM2, TIM_FLAG_Update);
  NVIC_InitStructure.NVIC_IRQChannel = TIM2_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 2;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);


  NVIC_InitStructure.NVIC_IRQChannel = USB_LP_CAN_RX0_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  
  NVIC_InitStructure.NVIC_IRQChannel = USB_HP_CAN_TX_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
  //定时器3中断
  /*
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}
//定时器2初始化
void TIM2_Config(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM2);//复位TIM2定时器
    TIM_TimeBaseStructure.TIM_Period = 1200;//最大计数值0xffff      
    TIM_TimeBaseStructure.TIM_Prescaler = 59999;//分频       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;// 时钟分割  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方向向上计数
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//清除TIM2溢出中断标志
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//TIM2溢出中断允许  
    TIM_Cmd(TIM2, ENABLE);//允许tim2计数   
}

//定时器3初始化
/*
void TIM3_Config(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM3);//复位TIM2定时器
    TIM_TimeBaseStructure.TIM_Period = 35999;//最大计数值0xffff      
    TIM_TimeBaseStructure.TIM_Prescaler = 2000;//分频       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;// 时钟分割  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//计数方向向上计数
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);//清除TIM3溢出中断标志
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//TIM3溢出中断允许  
    TIM_Cmd(TIM3, ENABLE);//允许tim3计数   
}
*/
//软件看门狗初始化
void WWDG_Config(void)
{
    WWDG_SetPrescaler(WWDG_Prescaler_8); //时钟8分频4ms
    // (PCLK1/4096)/8= 244 Hz (~4 ms)
    WWDG_SetWindowValue(65);                    //计数器数值
    WWDG_Enable(127);                   //启动计数器，设置喂狗时间
    // WWDG timeout = ~4 ms * 64 = 262 ms
    WWDG_ClearFlag();                   //清除标志位
    WWDG_EnableIT();                    //启动中断     
}

//调试
#ifdef  DEBUG
/*******************************************************************************
* Function Name  : assert_failed
* Description    : Reports the name of the source file and the source line number
*                  where the assert_param error has occurred.
* Input          : - file: pointer to the source file name
*                  - line: assert_param error line source number
* Output         : None
* Return         : None
*******************************************************************************/
void assert_failed(u8* file, u32 line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
