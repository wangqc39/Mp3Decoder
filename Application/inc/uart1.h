#ifndef _UART1_H
#define _UART1_H


#include "main.h"

void UART1_Config(void);
void UART1_GPIO_Config(void);
void Uart1_SendByte(u8 ch);
void Uart1_SendStr(uc8 *str);

#endif
