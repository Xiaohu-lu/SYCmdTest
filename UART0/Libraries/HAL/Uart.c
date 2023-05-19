/***************************************************************************
* File name    :	Uart.c
* Function     :	The functions for USART0
* Author       : 	Leixj
* Date         :	2012.07.11
* Version      :    v1.0
* Description  :    UART Hardware Abstract Layer compatible with SY09
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09.h"


extern uint32_t	SystemFrequency;

/**************************************************************************
* Local Macro Definition
***************************************************************************/


/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static STRUCT_FIFO gUartTxFIFO[UART_NUM];	// the global for UART transmitting
static STRUCT_FIFO gUartRxFIFO[UART_NUM];	// the global for UART receiving
static const IRQn_Type gUartxIRQn[UART_NUM] = {UART0_IRQn,UART1_IRQn};
/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/
/****************************************************************************
 * Subroutine:	UART_PortInit
 * Function:	UART_Port initialization  
 * Input:       UART_MAP mPortMap-- Select which group of GPIO port as UART
 *                                 UART0_MAP_A16_A17
 *                                 UART1_MAP_A27_A28
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2015/08/28
 * ModifyRecord:
***************************************************************************/
UINT8 UART_PortInit(UART_MAP mPortMap)
{
    switch(mPortMap)
	{
		case UART0_MAP_A16_A17:
			// select Pins
	        GpioRegs.GPAUSE2.bit.GPA16 = MUX_UART;	// RXD
	        GpioRegs.GPAUSE2.bit.GPA17 = MUX_UART;	// TXD
			break;
		case UART1_MAP_A27_A28:
			// select Pins
            GpioRegs.GPAUSE3.bit.GPA27 = MUX_UART;	  //Select RxD pin for UART0	 GPA27
			GpioRegs.GPAUSE3.bit.GPA28 = MUX_UART;	  //Select TxD pin for UART0	 GPA28
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;	
}
/***************************************************************************
 * Subroutine:	UART_GetRegs
 * Function:	Get UART register file address
 * Input:		UINT8 UARTx -- 	UART number, should be less than UART_NUM;
 * 								if it is out of range, dead loops.
 * Output:		volatile struct USART_REGS * --	Return UARTx register file
 * 												address
 * Description:	Get UART register file address.
 * Date:		2012.07.11
 * ModifyRecord:
 * *************************************************************************/
volatile struct USART_REGS * UART_GetRegs(UINT8 UARTx)
{
    switch(UARTx)
	{
		case UART0:
			return &Uart0Regs;
		case UART1:
			return &Uart1Regs;
		default:
			return NULL;
	}
}

/***************************************************************************
 * Subroutine:	UART_Init
 * Function:	UART initialization
 * Input:		UINT8 UARTx -- 		UART number, UART0	 UART1
 *				UART_MAP mPortMap-- Select which group of GPIO port as UART
 *                                 UART0_MAP_A16_A17
 *                                 UART1_MAP_A27_A28 
 * 				STRUCT_UART_FORMAT sFrame --
 * 									UART frame format;
 * 				STRUCT_DATA_BUF *pRxBuf --
 * 									UART receive buffer, to specify the buffer and size for receive,
 * 									if pRxBuf is NULL, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * 				STRUCT_DATA_BUF *pTxBuf --
 * 									UART transmit buffer, to specify the buffer and size for transmit,
 *									if pRxBuf is NULL and sFrame.TxMode == UART_TX_INT, receiver will be disabled and its pin will be used
 * 									as default (GPIO)
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.07.11
 * ModifyRecord:
 * *************************************************************************/
UINT8 UART_Init(UINT8 UARTx, UART_MAP mPortMap,STRUCT_UART_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{
	volatile struct USART_REGS * pUart;

	// 1. to open global control
	switch(UARTx)
	{
		case UART0:
			if(mPortMap != UART0_MAP_A16_A17)
				return RT_PARAM_ERR;
   			ScmRegs.MODCLKCTRL |= 1<<ID_UART0;	 	// enable UART0 clock
			ScmRegs.MODRSTCTRL |= 1<<ID_UART0;	  	// Relinquish from reset
			break;
		case UART1:
			if(mPortMap == UART0_MAP_A16_A17)
				return RT_PARAM_ERR;
   			ScmRegs.MODCLKCTRL |= 1<<ID_UART1;	 	// enable UART0 clock
			ScmRegs.MODRSTCTRL |= 1<<ID_UART1;	  	// Relinquish from reset
			break;
		default:
			return RT_FAIL;
	}
	//2 to select Pins
	if(UART_PortInit(mPortMap) != RT_OK) return RT_FAIL;
			
	// 2. select UART register file
	pUart = UART_GetRegs(UARTx);
	// 3. disable UART transfer and receive
	pUart->GCTL.bit.RxEn = 0;
	pUart->GCTL.bit.TxEn = 0;
	// 4. disable all UART interrupts and clear all interrupt flags
	pUart->INTEN.all = 0;
	pUart->INTCLR.all = 0xffff;
	
	// 5. configure UART frame format
	switch(sFrame.CharBit)
	{
		case CHAR_5BIT:
		case CHAR_6BIT:
		case CHAR_7BIT:
		case CHAR_8BIT:
			pUart->CCTL.bit.Char = sFrame.CharBit-5;
			break;
		default:
			return RT_FAIL;
	}
	switch(sFrame.StopBit)
	{
		case ONE_STOP:
		case TWO_STOP:
			pUart->CCTL.bit.Spb = sFrame.StopBit-1;
			break;
		default:
			return RT_FAIL;
	}
	switch(sFrame.Parity)
	{
		case NONE:
			pUart->CCTL.bit.PEn = 0;
			break;
		case ODD:
			pUart->CCTL.bit.PEn = 1;
			pUart->CCTL.bit.PSel = 0;			
			break;
		case EVEN:
			pUart->CCTL.bit.PEn = 1;
			pUart->CCTL.bit.PSel = 1;			
			break;
		default:
			return RT_FAIL;
	}
	
	if(sFrame.BaudRate == 0) return RT_FAIL;
	pUart->SPBREG = SystemFrequency / (16 * sFrame.BaudRate);
	
	// 7. to initialize RX FIFO, enable RX interrupt and RX
	if(pRxBuf == NULL)		// no buffer, disable receive
	{
		pUart->GCTL.bit.RxEn = 0;
	}
	else
	{
		gUartRxFIFO[UARTx].pFFData = pRxBuf->pBuf;		// to initialize receive FIFO
		gUartRxFIFO[UARTx].FFDepth = pRxBuf->Size;
		gUartRxFIFO[UARTx].FFValidSize = 0;
		gUartRxFIFO[UARTx].FFInOffset = 0;
		gUartRxFIFO[UARTx].FFOutOffset = 0;
		pUart->INTEN.bit.RxiEn = 1;
		pUart->INTEN.bit.RxOerriIEn = 1;		// added by coins	2009.09.17
		pUart->INTEN.bit.RxBrkiEn = 1;
		pUart->INTEN.bit.RxFerriEn = 1;
		pUart->INTEN.bit.RxPerriEn = 1;
		pUart->GCTL.bit.RxEn = 1;				// enable receive
	}

	// 7. to initial TX FIFO, enable TX interrupt and TX
	if(sFrame.TxMode == UART_TX_INT)		// transmit by interrupt
	{
		if(pTxBuf == NULL)				// no buffer for transmit interrupt
		{
			pUart->GCTL.bit.TxEn = 0;		// disable transmit
		}
		else
		{
			gUartTxFIFO[UARTx].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
			gUartTxFIFO[UARTx].FFDepth = pTxBuf->Size;
			gUartTxFIFO[UARTx].FFValidSize = 0;
			gUartTxFIFO[UARTx].FFInOffset = 0;
			gUartTxFIFO[UARTx].FFOutOffset = 0;
			pUart->INTEN.bit.TxiEn = 1;
			pUart->GCTL.bit.TxEn = 1;		// enable transmit
		}
	}
	else
	{
		pUart->INTEN.bit.TxiEn = 0;
		pUart->GCTL.bit.TxEn = 1;		// enable transmit
	}
	return RT_OK;	
}

/***************************************************************************
 * Subroutine:	UART_SendByte
 * Function:	UART send byte
 * Input:		UINT8 UARTx -- 		UART number, UART0;
 * 				UINT8 bData --		The value to be transfered
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2010.05.26
 * ModifyRecord:
 * *************************************************************************/
UINT8 UART_SendByte(UINT8 UARTx, UINT8 bData)
{
	volatile struct USART_REGS * pUart;
	UINT32 	lWaitTime;
	UINT32  i = 0;
	if(UARTx >= UART_NUM) return RT_FAIL;
	pUart = UART_GetRegs(UARTx);
	lWaitTime  = 16 * 12 * pUart->SPBREG;	// 8 UART clock transmit a byte;

	__NOP();
	if(pUart->INTEN.bit.TxiEn==1)		// if transmit interrupt enabled
	{	
		NVIC_DisableIRQ(gUartxIRQn[UARTx]);   // disable UART0 interrupt
		if(gUartTxFIFO[UARTx].FFValidSize==0)	// TX FIFO is empty
		{
			if(!pUart->CSTAT.bit.TxFull)	// UART TXREG FIFO is not full
			{
				pUart->TXREG = bData;		// write to UART TXREG directly
				NVIC_EnableIRQ(gUartxIRQn[UARTx]);
				return RT_OK;
			}
		}
		// write to TX FIFO buffer
		if(gUartTxFIFO[UARTx].FFValidSize < gUartTxFIFO[UARTx].FFDepth)		// buffer is not full
		{
			gUartTxFIFO[UARTx].pFFData[gUartTxFIFO[UARTx].FFInOffset++] = bData;
			if(gUartTxFIFO[UARTx].FFInOffset >= gUartTxFIFO[UARTx].FFDepth)
				gUartTxFIFO[UARTx].FFInOffset = 0;
			gUartTxFIFO[UARTx].FFValidSize++;	// valid size + 1
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			return RT_OK;
		}
		else
		{
			NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			return RT_FAIL;
		}
	}
	else		// transmit mode is polling
	{
		while(pUart->CSTAT.bit.TxFull)	// wait FIFO is not full
		{
			if(++i == lWaitTime) return RT_FAIL;
		};
		pUart->TXREG = bData;
		return RT_OK;
	}
}
/***************************************************************************
 * Subroutine:	UART_ReceByte
 * Function:	UART receive byte
 * Input:		UINT8 UARTx -- 		UART number, UART0 ;
 * 				UINT8 *pData --		The value to be received (pointer)
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description: the received data is from receive buffer
 * Date:		2009.04.03
 * ModifyRecord:
 * ************************************************************************/
UINT8 UART_ReceByte(UINT8 UARTx, UINT8 *pData)
{
	if(gUartRxFIFO[UARTx].FFValidSize != 0)	// have data in FIFO
	{
		*(pData) = gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFOutOffset++];
		if(gUartRxFIFO[UARTx].FFOutOffset == gUartRxFIFO[UARTx].FFDepth)
		gUartRxFIFO[UARTx].FFOutOffset = 0;
		NVIC_DisableIRQ(gUartxIRQn[UARTx]);
		gUartRxFIFO[UARTx].FFValidSize--;	// valid size - 1
		NVIC_EnableIRQ(gUartxIRQn[UARTx]);
		return RT_OK;
	}
	else		// no data received, return RT_FAIL
	{		
		return RT_FAIL;
	}
}

/**************************************************************************
 * Subroutine:	UART_SendBuf
 * Function:	UART send buffer
 * Input:		UINT8 bUsartNum -- 	USART number, only USART0 for AS50x;
 * 				UINT8 *pBuf --		The buffer to be sent
 * 				UINT16 iLen --		The length of buffer
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
 * Description:
 * Date:		2009.07.31
 * ModifyRecord:
 * **********************************************************************/
UINT8 UART_SendBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen)
{
	UINT8 bRet = RT_OK;
	while(iLen--)
	{
		bRet = UART_SendByte(UARTx, *pBuf++);
		if(bRet != RT_OK) break;
	}
	return bRet;
}

/**************************************************************************
 * Subroutine:	UART_ReceBuf
 * Function:	UART receive buffer
 * Input:		UINT8 bUsartNum -- 	USART number, only USART0 for SY09;
 * 				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT16 iLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description:	if the pBuf is NULL, do not read out the data from the receive FIFO
 * Date:		2009.07.31
 * ModifyRecord:
 * ************************************************************************/
UINT8 UART_ReceBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen)
{
	if(gUartRxFIFO[UARTx].FFValidSize >= iLen)	// have data in FIFO
	{
		if(pBuf!=NULL)
		{
			while(iLen--)
			{
				*pBuf++ = gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFOutOffset++];
				if(gUartRxFIFO[UARTx].FFOutOffset == gUartRxFIFO[UARTx].FFDepth)
					gUartRxFIFO[UARTx].FFOutOffset = 0;
				NVIC_DisableIRQ(gUartxIRQn[UARTx]);
				gUartRxFIFO[UARTx].FFValidSize--;	// valid size - 1
				NVIC_EnableIRQ(gUartxIRQn[UARTx]);
			}
		}
		return RT_OK;
	}
	return RT_FAIL;
}

/***************************************************************************
 * Subroutine:	UART_GetRxFFValidSize
 * Function:	to get UART receive buffer valid data number
 * Input:		UINT8 UARTx -- 	UART number, UARTx for SY09;
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 UART_GetRxFFValidSize(UINT8 UARTx)
{
	UINT16 nLen;
// 	NVIC_DisableIRQ(gUartxIRQn[UARTx]);
	nLen=gUartRxFIFO[UARTx].FFValidSize;
// 	NVIC_EnableIRQ(gUartxIRQn[UARTx]);
	return nLen;
} 

/***************************************************************************
 * Subroutine:	UARTx_IntProc
 * Function:	UARTx interrupt process
 * Input:		UINT8 UARTx --		UART number, UART0 ;
 * Output:		none
 * Description: to process all UART interrupt
 * Date:		2010.05.27
 * ModifyRecord:
 * ************************************************************************/
void UARTx_IntProc(UINT8 UARTx)
{
	union USART_INTSTAT_REG IntFlag;
	volatile struct USART_REGS * pUart;
	pUart = UART_GetRegs(UARTx);

	IntFlag = pUart->INTSTAT;		// get interrupt flags
	pUart->INTCLR.all = IntFlag.all;	// clear interrupt flags for next interrupt

	if(IntFlag.bit.RxIntf || IntFlag.bit.RxOerrIntf)	// receive interrupt
	{
		while(pUart->CSTAT.bit.RxAvl)	// added by coins, 2009.09.16
		{
			if(gUartRxFIFO[UARTx].FFValidSize < gUartRxFIFO[UARTx].FFDepth)	// not full
			{
				gUartRxFIFO[UARTx].pFFData[gUartRxFIFO[UARTx].FFInOffset++] = pUart->RXREG;
				if(gUartRxFIFO[UARTx].FFInOffset == gUartRxFIFO[UARTx].FFDepth) gUartRxFIFO[UARTx].FFInOffset = 0;
				gUartRxFIFO[UARTx].FFValidSize++;
			}
			else	// if USART RX FIFO is full, dummy read.	added by coins, 2009.09.17
			{
				pUart->RXREG;
			}
		}
	}
	__NOP();
	if(IntFlag.bit.TxIntf)		// transmit interrupt
	{
		while(!pUart->CSTAT.bit.TxFull)	// till FIFO is full
		{
			if(gUartTxFIFO[UARTx].FFValidSize != 0)	// not empty
			{
				pUart->TXREG = gUartTxFIFO[UARTx].pFFData[gUartTxFIFO[UARTx].FFOutOffset++];
				if(gUartTxFIFO[UARTx].FFOutOffset == gUartTxFIFO[UARTx].FFDepth) gUartTxFIFO[UARTx].FFOutOffset = 0;
				gUartTxFIFO[UARTx].FFValidSize--;
			}
			else break;
		}
	}	
}
/***************************************************************************
 * Subroutine:	UART0_Handler
 * Function:	UART0 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: UART0 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
#if 0
void UART0_Handler(void)
{
	UARTx_IntProc(UART0);
}
#endif
/***************************************************************************
 * Subroutine:	UART1_Handler
 * Function:	UART1 Interrupt Service Routine
 * Input:		None
 * Output:		none
 * Description: UART1 interrupt processing
 * Date:		2012.10.26
 * ModifyRecord:
 * ************************************************************************/
void UART1_Handler(void)
{
	UARTx_IntProc(UART1);
}
