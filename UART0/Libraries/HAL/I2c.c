/***************************************************************************
* File name    :	I2c.c
* Function     :	The functions for I2c
* Author       : 	Leixj
* Date         :	2012/07/12
* Version      :    v1.0
* Description  :    I2c Hardware Abstract Layer compatible with SY09
* ModifyRecord :
****************************************************************************/
#include "SY09.h"

extern uint32_t	SystemFrequency;

/**************************************************************************
* Local Macro Definition
***************************************************************************/
#define DUMMYBYTE 0xaa

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
#if(I2C_SLAVE)
static STRUCT_FIFO gI2cTxFIFO[I2C_NUM];	// the global for I2c transmitting
static STRUCT_FIFO gI2cRxFIFO[I2C_NUM];	// the global for I2c receiving
#endif
/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/
#if(I2C_SLAVE)
void I2C_SetRxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetRxFIFOValidSize(UINT8 bI2cNum);
void I2C_SetTxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size);
UINT16 I2C_GetTxFIFOValidSize(UINT8 bI2cNum);
void I2C_InitTxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pTxBuf);
void I2C_InitRxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pRxBuf);
#endif
/**************************************************************************
* Functon
***************************************************************************/
/****************************************************************************
 * Subroutine:	I2C_PortInit
 * Function:	I2C_Port initialization  
 * Input:       I2C_MAP mPortMap-- Select which group of GPIO port as I2C
 *                                 I2C0_MAP_A12_A13--GPIA[12]&GPIOA[13]
 *                                 I2C0_MAP_A16_A17--GPIA[16]&GPIOA[17]
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2015/08/28
 * ModifyRecord:
***************************************************************************/
UINT8 I2C_PortInit(I2C_MAP mPortMap)
{
    switch(mPortMap)
	{
		case I2C0_MAP_A12_A13:
			// select Pins
	        GpioRegs.GPAUSE1.bit.GPA12 = MUX_I2C;	// I2C_SCL
	        GpioRegs.GPAUSE1.bit.GPA13 = MUX_I2C;	// I2C_SDA
			break;
		case I2C0_MAP_A16_A17:
			// select Pins
	        GpioRegs.GPAUSE2.bit.GPA16 = MUX_I2C;	// I2C_SCL
	        GpioRegs.GPAUSE2.bit.GPA17 = MUX_I2C;	// I2C_SDA
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;	
}
/***************************************************************************
 * Subroutine:	I2C_GetRegs
 * Function:	Get I2c register file address
 * Input:		UINT8 bI2cNum -- 	I2c number, I2C0 only
 * 								if it is out of range, dead loops.
 * Output:		volatile struct USART_REGS * --	Return I2c register file address
 * Description:	Get I2C register file address.
 * Date:		2012.07.26
 * ModifyRecord:
 * *************************************************************************/
volatile struct USART_REGS * I2C_GetRegs(UINT8 bI2cNum)
{
	if(bI2cNum == I2C0)
		return &I2C0Regs;
	else
		return NULL;
}

/***************************************************************************
 * Subroutine:	WaitCycle
 * Function:	wait
 * Input:		UINT16 iCycle -- 	Cycle number;
 * Output:		none
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void WaitCycle(UINT16 iCycle)
{
	while(iCycle-=2){__NOP();}
}

/***************************************************************************
 * Subroutine:	I2C_Init
 * Function:	I2C mode initialization
 * Input:		UINT8 bI2cNum -- 	I2C number, I2c0 .
 *              I2C_MAP mPortMap-- Select which group of GPIO port as I2C
 *                                 I2C0_MAP_A12_A13--GPIA[12]&GPIOA[13]
 *                                 I2C0_MAP_A16_A17--GPIA[16]&GPIOA[17]
 * 				STRUCT_I2C_FORMAT sFrame --		I2C frame format;
 * 				STRUCT_DATA_BUF sRxBuf --
 * 									I2C receive buffer, to specify the buffer and size for slave receive;
 * 				STRUCT_DATA_BUF sTxBuf --
 * 									I2C transmit buffer, to specify the buffer and size for slave transmit;

 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT8 I2C_Init(UINT8 bI2cNum, I2C_MAP mPortMap, STRUCT_I2C_FORMAT sFrame, STRUCT_DATA_BUF *pRxBuf, STRUCT_DATA_BUF *pTxBuf)
{
	volatile struct USART_REGS * pI2c;
			
	if(bI2cNum == I2C0)
	{	//open global control
		ScmRegs.MODCLKCTRL |= 1<<ID_I2C0;
		ScmRegs.MODRSTCTRL |= 1<<ID_I2C0;  
		
		//2 to select Pins
		if(I2C_PortInit(mPortMap) != RT_OK) return RT_FAIL;
	}
	else return RT_PARAM_ERR;

	pI2c = I2C_GetRegs(bI2cNum);
	if(pI2c == NULL)  return RT_FAIL;

	pI2c->GCTL.bit.RxEn = 0;
	pI2c->GCTL.bit.TxEn = 0;

	//  disable all I2C interrupts and clear all interrupt flags
	pI2c->INTEN.all = 0x0000;
	pI2c->INTCLR.all = 0xffff;
	
	//  config I2C addressing bit mode
	switch(sFrame.bAddrBits)
	{
		case 7:		// 7-bit address mode
			pI2c->CCTL.bit.I2cXa = 0;
			break;
		case 10:	// 10-bit address mode
			pI2c->CCTL.bit.I2cXa = 1;
			break;
		default:
			return RT_FAIL;
	}
	
	//config I2C to master or slave
	switch(sFrame.bMasterEn)
	{
		case MASTER:
			pI2c->GCTL.bit.Mm = 1;
			//  config SPBRG for baud rate
			if(sFrame.lSckFreq == 0) return RT_FAIL;
			pI2c->SPBREG = SystemFrequency / (16 * sFrame.lSckFreq);
			return RT_OK;
		#if(I2C_SLAVE)
		case SLAVE:
			pI2c->GCTL.bit.Mm = 0;
			// config I2C address register
			pI2c->SI2CADD = sFrame.bSlaveAddr;
			
			I2C_InitTxFIFO(bI2cNum, pTxBuf);
			I2C_InitRxFIFO(bI2cNum, pRxBuf);

			if(pRxBuf == NULL)		// no buffer, disable receive
			{
				pI2c->GCTL.bit.RxEn = 0;
			}
			else
			{
				pI2c->INTEN.bit.RxOerriIEn = 1;
				pI2c->INTEN.bit.RxiEn = 1;
				pI2c->GCTL.bit.RxEn = 1;		// enable receive
			}

			if(pTxBuf == NULL)		// no buffer, disable send
			{
				pI2c->GCTL.bit.TxEn = 0;
			}
			else
			{
				pI2c->INTEN.bit.RtsiEn = 1;	// enable interrupt
				pI2c->INTEN.bit.TxiEn = 1;
				pI2c->GCTL.bit.TxEn = 1;		// enable send
			}
		    NVIC_EnableIRQ(I2C0_IRQn);
			return RT_OK;		//2013.1.5
		#endif
		default :
			return RT_FAIL;
	}
}

/***************************************************************************
* Subroutine: I2C_Stop
* Function:   stop I2C device
* Input:      UINT8 bI2cNum -- I2C number, I2C0;
* Output:                 
* Description: 
* Date:            2017.12.07
* ModifyRecord:
* *************************************************************************/
void I2C_Stop(UINT8 bI2cNum)
{
	volatile struct USART_REGS * pI2c;
	pI2c = I2C_GetRegs(bI2cNum);
	
	//  generate STOP
	pI2c->CCTL.bit.I2cStp = 1;
	WaitCycle(pI2c->SPBREG << 4);
	// 7 disable transmit and receive
	pI2c->GCTL.bit.TxEn = 0;
	pI2c->GCTL.bit.RxEn = 0;
}

/***************************************************************************
 * Subroutine:	I2C_MasterRead
 * Function:	Read one byte from I2C device
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * 				UINT8 bDevAddr --	I2C device address (7 bit address mode);
 * 				UINT16 bIndex -- 	High bit is flag that the index is use  
 *									low byte is the index of I2C device;
 * 				UINT8 *pData -- 	 The value to be received (pointer);
 *				UINT32 nLen -- 		to specify the length to be read
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL
 * Description:	The operation refer to I2C 2.1 Combined format transfer
 * Date:		2011.11.24
 * ModifyRecord: 
 * *************************************************************************/
UINT8 I2C_MasterRead(UINT8 bI2cNum, UINT8 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pI2c;
	UINT32 i = 0;
	UINT32 lWaitTime;
	pI2c = I2C_GetRegs(bI2cNum);
	lWaitTime = 16 * 8 * pI2c->SPBREG;	// 8 I2C clock transmit a byte

	// 1 enable transmit
	pI2c->GCTL.bit.TxEn = 1;
	// 2 generate START bit
	pI2c->CCTL.bit.I2cStt = 1;
	if(bIndex&0x8000)
	{
		// 3 write device address for write to I2C device
		pI2c->TXREG = bDevAddr;
		__NOP();
		while(!pI2c->CSTAT.bit.TxEpt)
		{
			if(++i == lWaitTime) 
			return RT_FAIL;
		}	  
		WaitCycle(pI2c->SPBREG << 4);
		// 4 write index
		pI2c->TXREG = bIndex;
		i = 0; 
		while(!pI2c->CSTAT.bit.TxEpt)
		{
			if(++i == lWaitTime) return RT_FAIL;
		}	
		WaitCycle(pI2c->SPBREG << 4);
		// 5 generate repeat START
		pI2c->CCTL.bit.I2cRstt = 1;
	}
	// 6 write device address for read
	pI2c->TXREG = bDevAddr + 1; 	// device address + read
	i = 0;
	while(!pI2c->CSTAT.bit.TxEpt)
	{
		if(++i == lWaitTime) return RT_FAIL;
	}	  
	WaitCycle(pI2c->SPBREG << 4);
	// 7 enable receive
	pI2c->GCTL.bit.RxEn = 1;
	// 8 wait for data valid, then read data
	while(nLen--)
	{
		i = 0;
		while(!pI2c->CSTAT.bit.RxAvl)
		{
			if(++i == lWaitTime) return RT_FAIL;
		}
		*pData++ = pI2c->RXREG;
	   if(nLen > 0)
	   		pI2c->CCTL.bit.NackDt = 0;	//acknowledge data bit
	}
	// 9 last acknowledge data bit
	if(bDevAddr == GC0303)
		pI2c->CCTL.bit.NackDt = 0;
	else pI2c->CCTL.bit.NackDt = 1;
	// 10 generate STOP
	pI2c->CCTL.bit.I2cStp= 1;
	WaitCycle(pI2c->SPBREG << 4);
	// 11 disable transmit and receive
	pI2c->GCTL.bit.TxEn = 0;
	pI2c->GCTL.bit.RxEn = 0;

	return RT_OK;
}

/***************************************************************************
 * Subroutine:	I2C_MasterWrite
 * Function:	Write one byte to I2C device
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * 				UINT8 bDevAddr --	I2C device address (7 bit address mode);
 * 				UINT8 bIndex -- 	High bit is flag that the index is use  
 *									low byte is the index of I2C device;
 * 				UINT8 bData -- 		the data write to;
 *				UINT32 nLen -- 		the length of data to be written
 * Output:		UINT8	--	Return the result, RT_OK or RT_FAIL
 * Description: The operation refer to I2C 2.1 Combined format transfer
 * Date:		2011.11.24
 * ModifyRecord:
 * *************************************************************************/
UINT8 I2C_MasterWrite(UINT8 bI2cNum, UINT8 bDevAddr, UINT16 bIndex, UINT8 *pData, UINT32 nLen)
{
	volatile struct USART_REGS * pI2c;
	UINT32 i = 0;
	UINT32 lWaitTime;
	pI2c = I2C_GetRegs(bI2cNum);
	lWaitTime = 16 * 8 * pI2c->SPBREG;	// 8 I2C clock transmit a byte
	
	// 1 enable transmit
	pI2c->GCTL.bit.TxEn = 1;
	// 2 generate START
	pI2c->CCTL.bit.I2cStt = 1;
	// 3 write device address for write to I2C device
	pI2c->TXREG = bDevAddr;
	i = 0;
	while(!pI2c->CSTAT.bit.TxEpt)
	{
		if(++i == lWaitTime) 
		return RT_FAIL;
	}	 
	WaitCycle(pI2c->SPBREG << 4);
	if(pI2c->INTSTAT.bit.NackIntf == 0)		// ack
	{
		// 4 write index
		if(bIndex&0x8000)
		{
			pI2c->TXREG = bIndex;
			i = 0;
			while(!pI2c->CSTAT.bit.TxEpt)
			{
				if(++i == lWaitTime) return RT_FAIL;
			}  
			WaitCycle(pI2c->SPBREG << 4);
			if(pI2c->INTSTAT.bit.NackIntf != 0)		// nack
			{
				pI2c->INTCLR.bit.NackiClr = 1;
				return RT_FAIL;
			}
		}
		while(nLen--)
		{
			pI2c->TXREG = *pData++;
			i = 0;
			while(!pI2c->CSTAT.bit.TxEpt)
			{
				if(++i == lWaitTime) return RT_FAIL;
			}	
			WaitCycle(pI2c->SPBREG << 4);
			if(pI2c->INTSTAT.bit.NackIntf != 0)		// nack
			{
				pI2c->INTCLR.bit.NackiClr = 1;
				return RT_FAIL;
			}
		}
	}
	else
	{
		pI2c->INTCLR.bit.NackiClr = 1;
		return RT_FAIL;
	}  
	// 6 generate STOP
	pI2c->CCTL.bit.I2cStp = 1;
	WaitCycle(pI2c->SPBREG << 4);
	// 7 disable transmit and receive
	pI2c->GCTL.bit.TxEn = 0;
	pI2c->GCTL.bit.RxEn = 0;
  
	return 	RT_OK;
}

#if(I2C_SLAVE)
/***************************************************************************
* Subroutine:	I2C_SlaveWriteBuf
* Function:		I2C send buffer
* Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
* 				UINT8 *pBuf --		The buffer to be sent
* 				UINT16 iLen --		The length of buffer
* Output:		UINT8	--			Return the result, RT_OK or RT_FAIL
* Description:
* Date:		2011.11.24
* ModifyRecord:
* **********************************************************************/
UINT8 I2C_SlaveWriteBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	volatile struct USART_REGS * pI2c;
	pI2c = I2C_GetRegs(bI2cNum);

	while(iLen--)
	{
		if(I2C_GetTxFIFOValidSize(bI2cNum) < gI2cTxFIFO[bI2cNum].FFDepth)// buffer is not full
		{
			gI2cTxFIFO[bI2cNum].pFFData[gI2cTxFIFO[bI2cNum].FFInOffset++] = *pBuf++;
			if(gI2cTxFIFO[bI2cNum].FFInOffset >= gI2cTxFIFO[bI2cNum].FFDepth)
				gI2cTxFIFO[bI2cNum].FFInOffset = 0;
			I2C_SetTxFIFOValidSize(bI2cNum, 1);	// valid size + 1 with interrupt disabled			
		}
		else
		{			
			UINT32 times = pI2c->SPBREG * 256;	   // wait
			while((I2C_GetTxFIFOValidSize(bI2cNum) >= gI2cTxFIFO[bI2cNum].FFDepth) && (times--))
                            {__NOP();}
			if(times == 0) return RT_FAIL;
		}
	}
	return RT_OK;
}

/**************************************************************************
 * Subroutine:	I2C_SlaveReadBuf
 * Function:	I2C read buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09
 * 				UINT8 *pBuf --		The buffer used for storing received data
 * 				UINT16 iLen --		The length will be receive
 * Output:		UINT8	--			Return the result, RT_OK or RT_FAIL(no enough data)
 * Description: same as UART_ReceBuf
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
UINT8 I2C_SlaveReadBuf(UINT8 bI2cNum, UINT8 *pBuf, UINT16 iLen)
{
	if(I2C_GetRxFIFOValidSize(bI2cNum) >= iLen)		// have enough data in FIFO
	{
		while(iLen--)
		{
			*pBuf++ = gI2cRxFIFO[bI2cNum].pFFData[gI2cRxFIFO[bI2cNum].FFOutOffset++];
			if(gI2cRxFIFO[bI2cNum].FFOutOffset == gI2cRxFIFO[bI2cNum].FFDepth)
				gI2cRxFIFO[bI2cNum].FFOutOffset = 0;
			I2C_SetRxFIFOValidSize(bI2cNum, 1);
		}
		return RT_OK;
	}
	return RT_FAIL;
}

/***************************************************************************
 * Subroutine:	I2C_InitTxFIFO
 * Function:	Initialize I2c transmit buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * Output:		none
 * Description: clear memory of gI2cTxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and pFFData
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
void I2C_InitTxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pTxBuf)
{
	gI2cTxFIFO[bI2cNum].pFFData = pTxBuf->pBuf;		// to initialize transmit FIFO
	gI2cTxFIFO[bI2cNum].FFDepth = pTxBuf->Size;
	gI2cTxFIFO[bI2cNum].FFValidSize = 0;
	gI2cTxFIFO[bI2cNum].FFInOffset = 0;
	gI2cTxFIFO[bI2cNum].FFOutOffset = 0;
}
/***************************************************************************
 * Subroutine:	I2C_InitRxFIFO
 * Function:	Initialize I2c Receive buffer
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * Output:		none
 * Description: clear memory of gI2cRxFIFO[bSpiNum], include FFValidSize, FFInOffset,
 * 				FFOutOffset and pFFData
 * Date:		2011.11.24
 * ModifyRecord:
 * ************************************************************************/
void I2C_InitRxFIFO(UINT8 bI2cNum, STRUCT_DATA_BUF *pRxBuf)
{
	gI2cRxFIFO[bI2cNum].pFFData = pRxBuf->pBuf;		// to initialize receive FIFO
	gI2cRxFIFO[bI2cNum].FFDepth = pRxBuf->Size;
	gI2cRxFIFO[bI2cNum].FFValidSize = 0;
	gI2cRxFIFO[bI2cNum].FFInOffset = 0;
	gI2cRxFIFO[bI2cNum].FFOutOffset = 0;
}

/***************************************************************************
 * Subroutine:	I2C_GetTxFIFOValidSize
 * Function:	to get I2c transmit buffer valid data number
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 I2C_GetTxFIFOValidSize(UINT8 bI2cNum)
{
	UINT32 status32;
	UINT32 size;
	status32 = __get_PRIMASK();
	__disable_irq();
	size = gI2cTxFIFO[bI2cNum].FFValidSize;
	__set_PRIMASK(status32);
	return size;
}

/***************************************************************************
 * Subroutine: I2cSetTxFIFOValidSize
 * Function:	to set I2c transmit buffer valid data number (increase current value)
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C_SetTxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size)
{
	UINT32 status32;
	status32 = __get_PRIMASK();
	__disable_irq();
	gI2cTxFIFO[bI2cNum].FFValidSize += offset_size;
	__set_PRIMASK(status32);
}

/***************************************************************************
 * Subroutine:	I2C_GetRxFIFOValidSize
 * Function:	to get I2c receive buffer valid data number
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * Output:		UINT16	-- the buffer valid data size;
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
UINT16 I2C_GetRxFIFOValidSize(UINT8 bI2cNum)
{
	UINT32 status32;
	UINT32 size;
	status32 = __get_PRIMASK();
	__disable_irq();
	size = gI2cRxFIFO[bI2cNum].FFValidSize;
	__set_PRIMASK(status32);
	return size;
}

/***************************************************************************
 * Subroutine: I2C_SetRxFIFOValidSize
 * Function:	to set I2c receive buffer valid data number (increase current value)
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * 				UINT16 offset_size --	specifies the size offset;
 * Output:		none
 * Description:
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C_SetRxFIFOValidSize(UINT8 bI2cNum, UINT16 offset_size)
{
	UINT32 status32;
	status32 = __get_PRIMASK();
	__disable_irq();
	gI2cRxFIFO[bI2cNum].FFValidSize -= offset_size;
	__set_PRIMASK(status32);
}

/***************************************************************************
 * Subroutine:	I2C_IntProc
 * Function:	I2c interrupt process
 * Input:		UINT8 bI2cNum -- 	I2C number, I2C0 for SY09;
 * Output:		none
 * Description: to process all I2C interrupt except NackIntf
 * Date:		2012.07.26
 * ModifyRecord:
 *	1.if TX FIFO is empty, dummy send.
 * ************************************************************************/
void I2C_IntProc(UINT8 bI2cNum)
{	
	union USART_INTSTAT_REG IntFlag;
	volatile struct USART_REGS * pI2c;
	pI2c = I2C_GetRegs(bI2cNum);	// get usart registers address

	IntFlag = pI2c->INTSTAT;	// get interrupt flags
	pI2c->INTCLR.all = IntFlag.all;	// clear interrupt flags for next interrupt

	if(IntFlag.bit.RxIntf || IntFlag.bit.RxOerrIntf)	// receive interrupt
	{
		while(pI2c->CSTAT.bit.RxAvl)	// added by coins, 2009.09.16
		{
			if(gI2cRxFIFO[bI2cNum].FFValidSize < gI2cRxFIFO[bI2cNum].FFDepth)	// not full
			{
				gI2cRxFIFO[bI2cNum].pFFData[gI2cRxFIFO[bI2cNum].FFInOffset++] = pI2c->RXREG;
				if(gI2cRxFIFO[bI2cNum].FFInOffset == gI2cRxFIFO[bI2cNum].FFDepth)
					gI2cRxFIFO[bI2cNum].FFInOffset = 0;
				gI2cRxFIFO[bI2cNum].FFValidSize++;
				__NOP();
			}
			else	// if USART RX FIFO is full, dummy read.	added by coins, 2009.09.17
			{
				UINT8 temp;
				temp = pI2c->RXREG;
			}
		}
	}

	if(IntFlag.bit.RtsIntf || (IntFlag.bit.TxIntf))	// transmit
	{
		if(pI2c->CSTAT.bit.Si2cBusy && pI2c->CSTAT.bit.Si2cRw && (!pI2c->CSTAT.bit.Si2cNack))
		{
            UINT8 temp;
			if(IntFlag.bit.RtsIntf)			  //added by coins, 2015.10.21
			{
				pI2c->GCTL.bit.RxEn = 0;
				pI2c->GCTL.bit.TxEn = 1;
				if(gI2cTxFIFO[bI2cNum].FFValidSize == 0)	//if TX FIFO is empty, dummy send.
				{
					pI2c->TXREG = DUMMYBYTE;
				}
			}
			while(!pI2c->CSTAT.bit.TxFull)	// till FIFO is full
			{
				if(gI2cTxFIFO[bI2cNum].FFValidSize != 0)	// not empty
				{
                    temp= gI2cTxFIFO[bI2cNum].pFFData[gI2cTxFIFO[bI2cNum].FFOutOffset++];
					pI2c->TXREG = temp;
					if(gI2cTxFIFO[bI2cNum].FFOutOffset == gI2cTxFIFO[bI2cNum].FFDepth)
						gI2cTxFIFO[bI2cNum].FFOutOffset = 0;
					gI2cTxFIFO[bI2cNum].FFValidSize--;
				}
				else
				{
					UINT32 times = pI2c->SPBREG * 256 * 16;
					while(!pI2c->CSTAT.bit.TxEpt && (times--)){__NOP();} 	// wait transmit complete
					pI2c->GCTL.bit.RxEn = 1;
					pI2c->GCTL.bit.TxEn = 0;
					break;
				}
			}
		}		
		if(!pI2c->CSTAT.bit.Si2cBusy || pI2c->CSTAT.bit.Si2cNack)	// received stop bit
		{
			pI2c->GCTL.bit.RxEn = 1;
			pI2c->GCTL.bit.TxEn = 0;			
		}
		__NOP();		
	}  
}

/***************************************************************************
 * Subroutine:	I2C0_IntProc
 * Function:	use for I2C0 ISR call back
 * Input:		None
 * Output:		none
 * Description: I2C0 interrupt processing
 * Date:		2012.07.26
 * ModifyRecord:
 * ************************************************************************/
void I2C0_Handler()
{
	I2C_IntProc(I2C0);
}
#endif
