/***************************************************************************
* File name    :	Uart.h
* Function     :	Header of Uart.c
* Author       : 	coins
* Date         :	2009/03/31
* Version      :    v1.0
* Description  :    Header of UART Hardware Abstract Layer
* ModifyRecord :
****************************************************************************/
#ifndef UART_H_
#define UART_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
// Define UART Number
#ifndef	_UART_
#define	_UART_
#define 	UART_NUM	2
#define		UART0		0
#define		UART1		1
#endif

// Define UART Frame Format 
#ifndef	_STOP_BIT_
#define	_STOP_BIT_
#define		ONE_STOP	1
#define		TWO_STOP	2
#endif

// Define UART Char Bit 
#ifndef	_CHAR_BIT_
#define	_CHAR_BIT_
#define		CHAR_5BIT	5
#define		CHAR_6BIT	6
#define		CHAR_7BIT	7
#define		CHAR_8BIT	8
#endif

// Define UART Parity
#ifndef	_PARITY_
#define _PARITY_
#define		NONE		0
#define		ODD			1    
#define		EVEN		2   
#endif

// Define UART Transmit Mode
#ifndef	_UART_TX_MODE_
#define _UART_TX_MODE_
#define		UART_TX_POLLING		0
#define		UART_TX_INT			1
#endif

/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef	enum {
	UART0_MAP_A16_A17 = 0,	
			//GPA16 -- UART0_RXD	
			//GPA17 -- UART0_TXD		
	UART1_MAP_A27_A28 = 1,	
			//GPA27 -- UART1_RXD	
			//GPA28 -- UART1_TXD	
}UART_MAP;
// Define Data Buffer Structure Type 
#ifndef	_DATA_BUF_
#define	_DATA_BUF_
typedef	struct	{
	UINT8 *	pBuf;		// pointer to buffer start address
	UINT16	Size;		// buffer size
}STRUCT_DATA_BUF;
#endif

// Define Uart Frame Structure Type
#ifndef	_STRUCT_UART_FRAME_FORMAT_
#define	_STRUCT_UART_FRAME_FORMAT_
typedef	struct {
	UINT32	BaudRate;		// baud rate
	UINT8 	CharBit;		// character bit
	UINT8 	StopBit;		// stop bit
	UINT8 	Parity;			// parity
	UINT8 	TxMode;			// transmit mode, UART_TX_INT or UART_TX_POLLING
}STRUCT_UART_FORMAT;
#endif

// Define FIFO Structure Type 
#ifndef	_STRUCT_FIFO_
#define	_STRUCT_FIFO_
typedef	struct {
	UINT8 *	pFFData;		// data buffer pointer
	UINT16	FFDepth;		// FIFO depth
	UINT16 	FFValidSize;	// valid data size in FIFO
	UINT16 	FFInOffset;		// index of FIFO in
	UINT16 	FFOutOffset;	// index of FIFO out
}STRUCT_FIFO;
#endif

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
// Function Declare 
volatile struct USART_REGS * UART_GetRegs(UINT8 UARTx);
UINT8 UART_Init(UINT8 UARTx, UART_MAP mPortMap,STRUCT_UART_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf);
UINT8 UART_SendByte(UINT8 UARTx, UINT8 bData);
UINT8 UART_ReceByte(UINT8 UARTx, UINT8 *pData);
UINT8 UART_SendBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen);
UINT8 UART_ReceBuf(UINT8 UARTx, UINT8 *pBuf, UINT16 iLen);
UINT16 UART_GetRxFFValidSize(UINT8 UARTx);
void UARTx_IntProc(UINT8 UARTx);

#endif /*UART_H_*/

