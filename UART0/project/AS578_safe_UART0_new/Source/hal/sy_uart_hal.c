#include "sy_uart_hal.h"
#include "sy09_uart0.h"

void HAL_SY_Send(char *buf,int len)
{
	USART0_Write(buf,len);
}

void HAL_SY_Secv(char *c,int timeout)
{
	/* �ӻ��λ������еõ�����
	 * ������������
	 */
	USART0_Read(c,timeout);
}

