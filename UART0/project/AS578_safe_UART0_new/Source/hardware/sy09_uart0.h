#ifndef _SY09_UART0_H
#define _SY09_UART0_H
#include "SY09_Device.h"

#define HARD_UARTX					UART0


void UART0_Lock_init(void);
void USART0_Write(char *buf,int len);
void USART0_Read(char *c,int timeout);
#endif


