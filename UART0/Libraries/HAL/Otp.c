/**********************************************************************************
* File name    :	Otp.c
* Function     :	The base function for One time programmer
* Author       : 	footman
* Date         :	2015/09/25
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY09.h"


/**********************************************************************************
* Local Macro Definition
**********************************************************************************/

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/


/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/



/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/


/**********************************************************************************
* Functon
**********************************************************************************/

/***************************************************************************
* Subroutine:	EFC_OtpWrite
* Function:		to write OTP user field.
* Input:		EFC_OTP_USER_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID
*										from 1 to 6;
*				UINT32 nAddress	-- 	to specify the start address of the OTP user field
* 									from 1 to 31;
* 				UINT32 nNum --	the number of data to be written, size of word;
* 				UINT32* pBuf -- to specify the data 
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpWrite(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32* pBuf)
{
	UINT32 nStartOffset;

	if((nUserFieldID<1)||(nUserFieldID>6)) return RT_PARAM_ERR;
	if((nAddress<1)||(nAddress>31)) return RT_PARAM_ERR;
	if((nAddress+nNum)>32) return RT_PARAM_ERR;

	nStartOffset = ASA_OTP + nUserFieldID*0x80;	
	if(*((UINT32 *)(FLASH_BASE_ADDR+nStartOffset)) != 0xFFFFFFFF)
		return RT_FAIL;
	EFC_WrProtect(AREA_OTP_WE);
	while(nNum--)
	{
		EfcRegs.WEN.all = 0x80000000 + nAddress*4 + nStartOffset;
		if(EFC_Program(pBuf, nAddress*4+nStartOffset, 1) != RT_OK)
		{
		 	EFC_WrProtect(AREA_PROT_DI);
			return RT_FAIL;
		}
		nAddress++;
		pBuf++;         
	}	
	EFC_WrProtect(AREA_PROT_DI);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_OtpDisWrite
* Function:		to disable Write otp function.
* Input:		EFC_OTP_USER_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID
*										from 1 to 7;
* 				UINT32* pBuf -- to specify the data 
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpDisWrite(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32* pBuf)
{
	UINT32 nAddr;

	if((nUserFieldID<1)||(nUserFieldID>7)) return RT_PARAM_ERR;
	if(*pBuf == 0xFFFFFFFF) return RT_PARAM_ERR;

	nAddr = ASA_OTP + nUserFieldID*0x80;
	if(*((UINT32 *)(FLASH_BASE_ADDR+nAddr)) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program(pBuf, nAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(*pBuf == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_OtpRead
* Function:		to read OTP user field.
* Input:		EFC_OTP_USER_FIELD_ID nUserFieldID -- 	to specify the number of the OTP user field ID
*										from 1 to 6;
*				UINT32 nAddress	-- 	to specify the start address of the OTP user field
* 									from 0 to 31;
* 				UINT32 nNum --	the number of data to be read;
* 				UINT8* pBuf -- to return the data
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_OtpRead(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32 *pBuf)
{
	if((nUserFieldID<1)||(nUserFieldID>6)) return RT_PARAM_ERR;
	if(nAddress>31) return RT_PARAM_ERR;
	if((nAddress+nNum)>32) return RT_PARAM_ERR;

	memcpy((void *)pBuf, (void *)(nAddress*4+FLASH_BASE_ADDR + ASA_OTP + nUserFieldID*0x80), nNum*4);
	return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_ReadChipSN
* Function:		to read OTP chip Serial Number.
* Input:		UINT8 *pSN	-- 	to return SN data;
* 				UINT16* pLen -- to return the length of SN, unit in byte
* Output:		None
* Description:	if pSN is NULL, only the length of SN will return by pLen
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen)
{
	if(pSN == NULL)
	{
		*pLen = 0x20;
		return;
	}
	memcpy((void *)pSN, (void *)(FLASH_BASE_ADDR + ASA_OTP), 0x20);
	*pLen = 0x20;
}


/***************************************************************************
* Subroutine:	EFC_LockJtag
* Function:		to lock JTAG
* Input:		None
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2013.01.29
* ModifyRecord: 
* *************************************************************************/
UINT8 EFC_LockJtag(void)
{
	UINT32 nAddr = 0x807F8;
	UINT32 lock = 0;
	
    if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&lock, nAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(lock == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_OK;
}

/***************************************************************************
* Subroutine:	EFC_LockTest
* Function:		to lock test
* Input:		None
* Output:		UINT8 --	return the result;
* Description:
* Date:			2013.01.18
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_LockTest(void)
{
	UINT32 nAddr = 0x807FC;
	UINT32 nVal = 0x23581321;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nAddr) == 0xFFFFFFFF)
	{
		EFC_WrProtect(AREA_OTP_WE);
		EfcRegs.WEN.all = 0x80000000 | nAddr;
		EFC_Program((UINT32 *)&nVal, nAddr, 1);
		if(nVal == *(UINT32 *)(FLASH_BASE_ADDR+nAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}
