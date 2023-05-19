#include "sy_uart_hal.h"
#include "sy09_uart0.h"

void HAL_SY_Send(char *buf,int len)
{
	USART0_Write(buf,len);
}

void HAL_SY_Secv(char *c,int timeout)
{
	/* 从环形缓冲区中得到数据
	 * 无数据则阻塞
	 */
	USART0_Read(c,timeout);
}

