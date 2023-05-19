#include "sy09_uart0.h"
#include "ring_buffer.h"
#include "Uart.h"
#include "platform_Semphore.h"

static ring_buffer uart0_buffer;
static platform_semphr_t uart_recv_semphore;

/* UART0中断处理函数 */
void UART0_Handler(void)
{
	union USART_INTSTAT_REG IntFlag;
	volatile struct USART_REGS * pUart;
	char c;
	pUart = UART_GetRegs(HARD_UARTX);
	
	IntFlag = pUart->INTSTAT;		// get interrupt flags
	pUart->INTCLR.all = IntFlag.all;	// clear interrupt flags for next interrupt
	if(IntFlag.bit.RxIntf || IntFlag.bit.RxOerrIntf)	// receive interrupt
	{
		c = pUart->RXREG;	/*接收数据*/
		ring_buffer_write(c,&uart0_buffer);
		/*释放一个二值信号量*/
		platform_semphore_give_from_isr(&uart_recv_semphore);
		return;
	}
	
}


void UART0_Lock_init(void)
{
	ring_buffer_init(&uart0_buffer);
	platform_semphore_init(&uart_recv_semphore);
}


void USART0_Write(char *buf,int len)
{
	int i = 0;
	volatile struct USART_REGS * pUart;
	pUart = UART_GetRegs(HARD_UARTX);
	while(i < len)
	{
		while(pUart->CSTAT.bit.TxFull);
		pUart->TXREG = buf[i];
		i++;
	}
}

void USART0_Read(char *c,int timeout)
{
	while(1)
	{
		if(0 == ring_buffer_read((unsigned char*)c,&uart0_buffer))
			return;
		else/*阻塞*/
		{
			platform_semphore_take_timeout(&uart_recv_semphore,timeout);
		}
	}
}

