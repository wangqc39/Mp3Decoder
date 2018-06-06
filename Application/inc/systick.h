
#ifndef _SYSTICK_H
#define _SYSTICK_H

#include "main.h"

void SysTick_Config(void);
void Delay_Ms(u32 nTime);
void TimingDelay_Decrement(void);


extern volatile unsigned int SystemTick;

#endif

