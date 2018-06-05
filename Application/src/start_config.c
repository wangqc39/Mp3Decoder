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

    //�ı�ָ���ܽŵ�ӳ��  GPIO_Remap_SWJ_Disable SWJ ��ȫʧ�ܣ�JTAG+SW-DP��
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_Disable, ENABLE);
    
    //�ı�ָ���ܽŵ�ӳ��  GPIO_Remap_SWJ_JTAGDisable  JTAG-DP ʧ�� + SW-DPʹ��
    //GPIO_PinRemapConfig(GPIO_Remap_SWJ_JTAGDisable , ENABLE);

void RCC_Config(void)
{
  ErrorStatus HSEStartUpStatus;
  
  //������ RCC�Ĵ�������Ϊȱʡֵ
  RCC_DeInit();

  //�����ⲿ���پ���HSE��
  RCC_HSEConfig(RCC_HSE_ON);

  //�ȴ� HSE ���� 
  HSEStartUpStatus = RCC_WaitForHSEStartUp();

  if(HSEStartUpStatus == SUCCESS)
  {
    //Ԥȡָ����ʹ��
    FLASH_PrefetchBufferCmd(FLASH_PrefetchBuffer_Enable);

     //���ô�����ʱֵ
    //FLASH_Latency_2  2 ��ʱ����
    FLASH_SetLatency(FLASH_Latency_2);

    //���� AHB ʱ�ӣ�HCLK��
    //RCC_SYSCLK_Div1  AHB ʱ�� =  ϵͳʱ�� 
    RCC_HCLKConfig(RCC_SYSCLK_Div1);

     //���ø��� AHB ʱ�ӣ�PCLK2��
    //RCC_HCLK_Div2  APB1 ʱ��  = HCLK / 2 
    RCC_PCLK2Config(RCC_HCLK_Div1);

    //���õ��� AHB ʱ�ӣ�PCLK1��
    //RCC_HCLK_Div2  APB1 ʱ��  = HCLK / 2 
    RCC_PCLK1Config(RCC_HCLK_Div2);

    // PLLCLK = 8MHz * 9 = 72 MHz 
    //���� PLL ʱ��Դ����Ƶϵ��
    RCC_PLLConfig(RCC_PLLSource_HSE_Div1, RCC_PLLMul_6);

    //ʹ�ܻ���ʧ�� PLL
    RCC_PLLCmd(ENABLE);

    //�ȴ�ָ���� RCC ��־λ���óɹ� �ȴ�PLL��ʼ���ɹ�
    while(RCC_GetFlagStatus(RCC_FLAG_PLLRDY) == RESET)
    {
    }


    //����ϵͳʱ�ӣ�SYSCLK�� ����PLLΪϵͳʱ��Դ
    RCC_SYSCLKConfig(RCC_SYSCLKSource_PLLCLK);

    //�ȴ�PLL�ɹ�������ϵͳʱ�ӵ�ʱ��Դ
    //  0x00��HSI ��Ϊϵͳʱ�� 
    //  0x04��HSE��Ϊϵͳʱ�� 
    //  0x08��PLL��Ϊϵͳʱ��  
    while(RCC_GetSYSCLKSource() != 0x08)
    {
    }
  }

  
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);  
    
    //RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO, ENABLE);
    
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1, ENABLE);//�򿪴���1
   
    
    //RCC_APB1PeriphClockCmd(RCC_APB1Periph_WWDG, ENABLE);//ʹ��WWDG������Ź�
    
    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2,ENABLE);//ʹ�ܶ�ʱ��2  

  
    /* PD.08 used as USB pull-up */
  /*GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;//----------��ţ
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

 //---------------------����Ӧ���ж�---------------------------// 
  USART_ClearITPendingBit(USART1,USART_IT_RXNE);
  NVIC_InitStructure.NVIC_IRQChannel = USART1_IRQChannel;  //������1�ж�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 3;  //�ж�ռ�ȵȼ�0
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;  //���ж�
  NVIC_Init(&NVIC_InitStructure);  //��ʼ���ж�
 
  //SysTick��ʱ��ʱ�ж�
  NVIC_SystemHandlerPriorityConfig(SystemHandler_SysTick,1,0);  
  //�жϵȼ����á�һ�����õĸ�һЩ������������Ӱ��
  /*
  //WWDG������Ź��ж�
  NVIC_InitStructure.NVIC_IRQChannel = WWDG_IRQChannel;  //ͨ��
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;  //ռ���жϵȼ�
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  //��Ӧ�ж����ȼ�
  NVIC_Init(&NVIC_InitStructure);  //���ж�
  */
  //��ʱ��2�ж�
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
  //��ʱ��3�ж�
  /*
  NVIC_InitStructure.NVIC_IRQChannel = TIM3_IRQChannel;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;  
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);*/
}
//��ʱ��2��ʼ��
void TIM2_Config(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM2);//��λTIM2��ʱ��
    TIM_TimeBaseStructure.TIM_Period = 1200;//������ֵ0xffff      
    TIM_TimeBaseStructure.TIM_Prescaler = 59999;//��Ƶ       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;// ʱ�ӷָ�  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����������ϼ���
    TIM_TimeBaseInit(TIM2, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM2, TIM_FLAG_Update);//���TIM2����жϱ�־
    TIM_ITConfig(TIM2, TIM_IT_Update, ENABLE);//TIM2����ж�����  
    TIM_Cmd(TIM2, ENABLE);//����tim2����   
}

//��ʱ��3��ʼ��
/*
void TIM3_Config(void)
{   
    TIM_TimeBaseInitTypeDef  TIM_TimeBaseStructure;
    TIM_DeInit(TIM3);//��λTIM2��ʱ��
    TIM_TimeBaseStructure.TIM_Period = 35999;//������ֵ0xffff      
    TIM_TimeBaseStructure.TIM_Prescaler = 2000;//��Ƶ       
    TIM_TimeBaseStructure.TIM_ClockDivision = 0x0;// ʱ�ӷָ�  
    TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;//�����������ϼ���
    TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);
    
    TIM_ClearFlag(TIM3, TIM_FLAG_Update);//���TIM3����жϱ�־
    TIM_ITConfig(TIM3, TIM_IT_Update, ENABLE);//TIM3����ж�����  
    TIM_Cmd(TIM3, ENABLE);//����tim3����   
}
*/
//������Ź���ʼ��
void WWDG_Config(void)
{
    WWDG_SetPrescaler(WWDG_Prescaler_8); //ʱ��8��Ƶ4ms
    // (PCLK1/4096)/8= 244 Hz (~4 ms)
    WWDG_SetWindowValue(65);                    //��������ֵ
    WWDG_Enable(127);                   //����������������ι��ʱ��
    // WWDG timeout = ~4 ms * 64 = 262 ms
    WWDG_ClearFlag();                   //�����־λ
    WWDG_EnableIT();                    //�����ж�     
}

//����
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
