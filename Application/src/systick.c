
#include "systick.h"

volatile unsigned int SystemTick;

/*******************************************************************************
* Function Name  : SysTick_Config
* Description    : Configures SysTick
* Input          : None
* Output         : None
* Return         : None
*******************************************************************************/
//SysTick设置
void SysTick_Config(void)
{
    /* Disable SysTick Counter */
    SysTick_CounterCmd(SysTick_Counter_Disable);
  
    /* Disable the SysTick Interrupt */
    SysTick_ITConfig(DISABLE);
  
    /* Configure HCLK clock as SysTick clock source */
    SysTick_CLKSourceConfig(SysTick_CLKSource_HCLK_Div8);  //时钟除于8

    SysTick_SetReload(9000);  //计数周期长度
   
    SysTick_CounterCmd(SysTick_Counter_Enable);   //启动定时器 
    SysTick_ITConfig(ENABLE);  //打开中断

}


/*******************************************************************************
* Function Name  : Delay_Ms
* Description    : Inserts a delay time.
* Input          : nTime: specifies the delay time length, in milliseconds.
* Output         : None
* Return         : None
*******************************************************************************/
void Delay_Ms(u32 nTime)
{
//    /* Enable the SysTick Counter */
//    SysTick_CounterCmd(SysTick_Counter_Enable);
//  
//    TimingDelay = nTime;

//    while(TimingDelay != 0);

//    /* Disable SysTick Counter */
//    SysTick_CounterCmd(SysTick_Counter_Disable);
//    /* Clear SysTick Counter */
//    SysTick_CounterCmd(SysTick_Counter_Clear);
    unsigned int StartTick = SystemTick;

    while(SystemTick <= StartTick + nTime);
}


/*******************************************************************************
* Function Name  : TimingDelayMs_Decrement
* Description    : Decrements the TimingDelay variable.
* Input          : None
* Output         : TimingDelay
* Return         : None
*******************************************************************************/
void TimingDelay_Decrement(void)
{
    SystemTick++;
}

