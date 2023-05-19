/***************************************************************************************
* File name    :	Gpio.c
* Function     :	the function of GPIO
* Author       : 	Leixj
* Date         :	2012/09/03
* Version      :    v1.0
* Description  :    GPIO Hardware Abstract Layer for SY09
* ModifyRecord :
*****************************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09.h"

/**************************************************************************
* Local Macro Definition
***************************************************************************/
#define		GPIO_REG_SIZE	(12)

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/
/***********************************************************************************
 * Subroutine:	GPIO_Init
 * Function:	Initialize GPIO function
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * 				UINT8 nMode -- 		GPIO mode, according to _GPIO_MODE_
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_Init(UINT8 nPort,UINT8 nBitNum,UINT8 nMode)
{
	UINT8 temp;
	UINT32 lRegStatus;
	volatile UINT32 *pUse;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	pUse = (volatile UINT32 *)&(GpioRegs.GPAUSE0);
	pUse += GPIO_REG_SIZE*nPort;				// to point to specified register
	temp  = nBitNum/8;
	pUse += temp;
	temp = (nBitNum&7)*4;
	lRegStatus = __get_PRIMASK();	// save PRIMASK
	__disable_irq();						// disable interrupt
	*pUse = (*pUse & (~((UINT32)0xf << temp))) | ((UINT32)nMode << temp);
	__set_PRIMASK(lRegStatus);		// recover
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitIn
 * Function:	Read GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB, GPIOD, GPIOE supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return GPIOXn bit value 0 or 1
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitIn(UINT8 nPort,UINT8 nBitNum)
{
	volatile UINT32 *pData;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	pData += GPIO_REG_SIZE*nPort;
	return (*pData >> nBitNum) & 0x1;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitOut
 * Function:	output GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * 				UINT8 nBitVal --	the value of GPIO bit output, 0 or 1
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:	to set the value of specified bit
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitOut(UINT8 nPort,UINT8 nBitNum,UINT8 nBitVal)
{
	if(nBitVal)
	{
		return GPIO_BitSet(nPort, nBitNum);
	}
	else
	{
	 	return GPIO_BitClr(nPort, nBitNum);
	}
}

/***********************************************************************************
 * Subroutine:	GPIO_BitTog
 * Function:	Toggle GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitTog(UINT8 nPort,UINT8 nBitNum)
{
 	volatile UINT32 *pData;
 	pData = (volatile UINT32 *)&(GpioRegs.GPATOG);
 	pData += GPIO_REG_SIZE*nPort;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	(*pData) = (1<<nBitNum);		// the GPxTOG register is valid by writing 1
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitSet
 * Function:	Set GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitSet(UINT8 nPort,UINT8 nBitNum)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPASET);
	pData += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	(*pData) = (1<<nBitNum);		// the GPxSET register is valid by writing 1
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitClr
 * Function:	Clear GPIO bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitClr(UINT8 nPort,UINT8 nBitNum)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPACLR);
	pData += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	(*pData) = (1<<nBitNum);		// the GPxCLR register is valid by writing 1
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitIntClr
 * Function:	Clear GPIO interrupt flag bit
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitIntClr(UINT8 nPort,UINT8 nBitNum)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
	pData += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	(*pData) = (1<<nBitNum);		// the GPxINT register is valid by writing 1
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_GetBitUse
 * Function:	Read GPIO bit use value
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return GPIOXn use value
 * Description:
 * Date:		2013.04.09
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_GetBitUse(UINT8 nPort,UINT8 nBitNum)
{
	UINT8 temp;
	volatile UINT32 *pUse;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	pUse = (volatile UINT32 *)&(GpioRegs.GPAUSE0);
	pUse += GPIO_REG_SIZE*nPort;				// to point to specified register
	temp  = nBitNum/8;
	pUse += temp;
	temp = (nBitNum&7)*4;
	return (*pUse>>temp)&0xf;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitPullSet
 * Function:	Set the bit of GPIOxPE Register 
 * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2014.05.21
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitPullSet(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPAPE);
	pData += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	(*pData) |= (1<<nBitNum);		
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_BitPullClr
 * Function:	Clear the bit of GPIOxPE register 
 * Input:		UINT8 bPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 bBitNum -- 	GPIO bit number of each port, according to each port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2014.05.21
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitPullClr(UINT8 nPort, UINT8 nBitNum)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPAPE);
	pData += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
	(*pData) &= ~(1<<nBitNum);		
	return RT_OK;	
}

/***********************************************************************************
 * Subroutine:	GPIO_BitDriveConfig
 * Function:	Config GPIO bit Drive
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT8 nBitNum -- 	GPIO bit number of each port, according to each port;
 *				UINT8 nBitVal --	the value of GPIO Driver Register bit 0~3
 * Output:		UINT8	--			RT_OK or RT_FAIL
 * Description:
 * Date:		2014.05.21
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_BitDriveConfig(UINT8 nPort, UINT8 nBitNum, UINT8 nBitVal)
{
	UINT8 temp;
	UINT32 lRegStatus;
	volatile UINT32 *pDrive;
	pDrive = (volatile UINT32 *)&(GpioRegs.GPADRIVE0);
	pDrive += GPIO_REG_SIZE*nPort;
	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;

	temp  = (nBitNum/16);
	pDrive += temp;
	temp = (nBitNum&0xf)*2;
	lRegStatus = __get_PRIMASK();	  // save STATUS32					
	__disable_irq();			  	// disable interrupt
	*pDrive = (*pDrive & (~((UINT32)0x3 << temp))) | ((UINT32)nBitVal << temp);
	__set_PRIMASK(lRegStatus);		// recover
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortIn
 * Function:	read whole GPIO port
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * Output:		UINT32	--			Return specific port value
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT32 GPIO_PortIn(UINT8 nPort)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
	pData += GPIO_REG_SIZE*nPort;
	return *pData;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortIntStat
 * Function:	read GPIO port interrupt state
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * Output:		UINT32	--			Return specific port value
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT32 GPIO_PortIntStat(UINT8 nPort)
{
	volatile UINT32 *pData;
	pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
	pData += GPIO_REG_SIZE*nPort;
	return *pData;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortOut
 * Function:	output whole GPIO port
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nPortVal --  the value of GPIO port;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortOut(UINT8 nPort, UINT32 nPortVal)
{
	volatile UINT32 *pData;
 	UINT32 lRegStatus;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	pData = (volatile UINT32 *)&(GpioRegs.GPADATA);
 	pData += GPIO_REG_SIZE*nPort;
 	lRegStatus = __get_PRIMASK();	// save PRIMASK
	__disable_irq();						// disable interrupt
 	(*pData) = nPortVal;
 	__set_PRIMASK(lRegStatus);		// recover
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortTog
 * Function:	Toggle GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nSpecBits -- to specify GPIO bits to be toggled;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortTog(UINT8 nPort, UINT32 nSpecBits)
{
 	volatile UINT32 *pData;
 	pData = (volatile UINT32 *)&(GpioRegs.GPATOG);
 	pData += GPIO_REG_SIZE*nPort;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	(*pData) = nSpecBits;	// the GPxTOG register is valid by writing 1
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortSet
 * Function:	Set GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nSpecBits -- to specify GPIO bits to be set;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortSet(UINT8 nPort, UINT32 nSpecBits)
{
 	volatile UINT32 *pData;
 	pData = (volatile UINT32 *)&(GpioRegs.GPASET);
 	pData += GPIO_REG_SIZE*nPort;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	(*pData) = nSpecBits;	// the GPxSET register is valid by writing 1
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortClr
 * Function:	Clear GPIO port with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB supported;
 * 				UINT32 nSpecBits -- to specify GPIO bits to be cleared;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortClr(UINT8 nPort, UINT32 nSpecBits)
{
 	volatile UINT32 *pData;
 	pData = (volatile UINT32 *)&(GpioRegs.GPACLR);
 	pData += GPIO_REG_SIZE*nPort;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	(*pData) = nSpecBits;	// the GPxCLR register is valid by writing 1
 	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	GPIO_PortIntClr
 * Function:	Clear GPIO port interrupt flag with specified bits
 * Input:		UINT8 nPort -- 		GPIO port, GPIOA, GPIOB, GPIOD, GPIOE supported;
 * 				UINT32 nSpecBits -- to specify GPIO interrupt to be cleared;
 * Output:		UINT8	--			Return result, RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 GPIO_PortIntClr(UINT8 nPort, UINT32 nSpecBits)
{
 	volatile UINT32 *pData;
 	pData = (volatile UINT32 *)&(GpioRegs.GPAINT);
 	pData += GPIO_REG_SIZE*nPort;
 	if(GPIO_OUT_OF_PORT(nPort)) return RT_FAIL;
 	(*pData) = nSpecBits;	// the GPxINT register is valid by writing 1
 	return RT_OK;
}
