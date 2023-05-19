/*******************************************************************************
* File name    :	Serial.c
* Function     :	The low level routines for retarget printf function
* Author       : 	coins
* Date         :	2015/05/28
* Version      :    v1.0
* Description  :	
* ModifyRecord :
*******************************************************************************/

/*******************************************************************************
* Include Header Files
*******************************************************************************/
#include <stdint.h>
#include "SY09.h"


/*******************************************************************************
* Local Macro Definition
*******************************************************************************/
//#define	PRT_BAUD_RATE	57600
#define	PRT_BAUD_RATE	115200

#define	PRT_UART0_PORT	0
#define	PRT_UART1_PORT	1
#define	PRT_UART_PORT_SEL	PRT_UART1_PORT
#define	PRT_UART1_PORT_GPA28	0
#define	PRT_UART1_PORT_GPB4		1
#define	PRT_UART1_PORT_IO_SEL	PRT_UART1_PORT_GPA28


#if(PRT_UART_PORT_SEL == PRT_UART0_PORT)	   
#define	PRT_UART	Uart0Regs		
#else
#define	PRT_UART	Uart1Regs
#endif
/*******************************************************************************
* Local Type Definition
*******************************************************************************/

/*******************************************************************************
* Local static Variable Declaration
*******************************************************************************/

/*******************************************************************************
* Global Variable Declaration
*******************************************************************************/
extern uint32_t	SystemFrequency;
volatile unsigned char SerInit = 0;


/*******************************************************************************
* Local Functon Declaration
*******************************************************************************/

/*******************************************************************************
* Functon
*******************************************************************************/


/***************************************************************************
* Subroutine:	SER_init
* Function:		to initialize serial port for printf.
* Input:		None
* Output:		None;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
void SER_init(void)
{
#if(PRT_UART_PORT_SEL == PRT_UART0_PORT)
	ScmRegs.MODCLKCTRL |= 1<<ID_UART0;
	ScmRegs.MODRSTCTRL |= 1<<ID_UART0;
	GpioRegs.GPAUSE2.bit.GPA17 = MUX_UART;
#else
	ScmRegs.MODCLKCTRL |= 1<<ID_UART1;
	ScmRegs.MODRSTCTRL |= 1<<ID_UART1;
	#if(PRT_UART1_PORT_IO_SEL == PRT_UART1_PORT_GPA28)
		GpioRegs.GPAUSE3.bit.GPA28 = MUX_UART;
	#else
		GpioRegs.GPBUSE0.bit.GPB4 = MUX_UART;
	#endif
#endif	
	PRT_UART.GCTL.all = 0;
	PRT_UART.INTEN.all = 0;
	PRT_UART.INTCLR.all = 0xFFFF;
	PRT_UART.CCTL.all = 0x0180; 	// 8bit, 1 stop, no parity
	#if(SIMULATE_FOR_IC)
		PRT_UART.SPBREG = 1; 	// baud rate
	#else
		PRT_UART.SPBREG =  SystemFrequency/(16 * PRT_BAUD_RATE);
	#endif
	PRT_UART.GCTL.all |= 1<<3;		//·¢ËÍÊ¹ÄÜ

	SerInit = 1;
  	return;
}

/***************************************************************************
* Subroutine:	sendchar
* Function:		Write character to Serial Port
* Input:		int c -- the character to be written
* Output:		return the character;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
int sendchar(int c)
{
	if(SerInit==0)
	{
		SER_init();
	}
	while(PRT_UART.CSTAT.bit.TxFull)	// wait FIFO is not full
	{
		__NOP();		
	};
	PRT_UART.TXREG = c;
	while(!(PRT_UART.CSTAT.all&0x1))	// wait FIFO is empty
	{
		__NOP();
	};
  	return (c);
}

/***************************************************************************
* Subroutine:	getkey
* Function:		Read character from Serial Port   (blocking read)
* Input:		None
* Output:		return the character received;
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
int getkey (void) 
{
	int c;
	if(SerInit==0)
	{
		SER_init();
	}
	while(!(PRT_UART.CSTAT.all&0x2))	// wait for receiving data valid
	{
		__NOP();
	}
	c = PRT_UART.RXREG;
   	return c;
}

/***************************************************************************
* Subroutine:	ExitSimulate
* Function:		Exit Simulate for IC Design
* Input:		None
* Output:		None
* Description:	
* Date:			2015/05/28
* ModifyRecord:
* *************************************************************************/
void ExitSimulate(void)
{
	#if(SIMULATE_FOR_IC)
	unsigned int i = 1000;
	while(i--);
	#endif	
}


