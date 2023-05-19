/**********************************************************************************
* File name    :	Efc.c
* Function     :	The base function for embedded flash controller
* Author       : 	coins
* Date         :	2012/09/13
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
//#define	EFC_ROM_FUNC_USED

/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
UINT32 nKeyPermission[4] = {AREA_KEY0_WE,AREA_KEY1_WE,AREA_KEY2_WE,AREA_KEY3_WE};

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
* Subroutine:	EFC_ConfigTiming
* Function:		to config EFC timing.
* Input:		UINT8 nReadMode -- 	to set read mode.
*						 			0/3 -- for Txa Timing;
*									1 -- for one cycle read;
*									2 -- for two cycle read;		
*				UINT32 nTC0	-- 	to specify the TC0 timing register value;
* 				UINT32 nTC1 --	to specify the TC1 timing register value;
* Output:		None;
* Description:	if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
void EFC_ConfigTiming(UINT32 nTC0, UINT32 nTC1, UINT32 nTC2, UINT32 nTC3)
{
	#ifndef	EFC_ROM_FUNC_USED
	EfcRegs.TC0.all = nTC0;
	EfcRegs.TC1.all = nTC1;
	EfcRegs.TC2.all = nTC2;
	EfcRegs.TC3.all = nTC3;
	#else
	_EFC_ConfigTiming(nTC0, nTC1, nTC2, nTC3);
	#endif
}

/***************************************************************************
* Subroutine:	EFC_Program
* Function:		flash program.
* Input:		UINT32 *pData	-- 	the data to be programmed, word aligned;
* 				UINT32 addr_align --	the offset address word-aligned;
* 				UINT32 nLen --		the length of data, size in word.
* Output:		RT_OK or RT_FAIL;
* Description:	Program nLen word into flash address addr_align.
* Date:			2015.09.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_Program(UINT32 *pData, UINT32 addr_align, UINT32 nLen)
{
	UINT8 nRet = RT_OK;
	UINT8 nRetry;
    UINT32 lRegStatus;
	VUINT32 * pSysAddr;

	addr_align &= 0xFFFFFFFC; 
	pSysAddr = (VUINT32 *)(FLASH_BASE_ADDR+addr_align);	// start address to program
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// close interrupt	

	while(nLen--)
	{
		EfcRegs.WEN.all = addr_align | 0x80000000;
		nRetry = PROG_MAX_TIMES;	
		while(nRetry)
		{
			*pSysAddr = *pData;	// program a word (32 bits)
			EFC_PROG_CMD();
			while(EfcRegs.STS.bit.Busy);
			if(*pSysAddr != *pData)
			{
				nRetry--;
				continue;
			}
			else break;
		}
		if(nRetry)	 // program ok
		{
			*pSysAddr = *pData;	// extra program once
			EFC_PROG_CMD();
			while(EfcRegs.STS.bit.Busy);
			pSysAddr++;	  	// point to next word
			pData++;
			addr_align += 4;	
		}
		else 	// program fail
		{
			nRet = RT_FAIL;
			break;
		}
	}
	EfcRegs.WEN.all = 0;
	__set_PRIMASK(lRegStatus); 			// recover interrupt
	return nRet;
}

/***************************************************************************
* Subroutine:	EFC_PageProg
* Function:		Embedded flash program a page.
* Input:		UINT32 addr_offset --	the flash offset address of byte;
* 				UINT32 *pData	-- 	the data to be programmed;
* Output:		RT_OK or RT_FAIL;
* Description:	to program a page that the addr_offset included.
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_PageProg(UINT32 addr_offset, UINT32 *pData)
{
	UINT8 extra=1, i, j, nRetry, nRet = RT_OK;
	VUINT32 *pSysAddr;
	UINT32 lRegStatus;
	addr_offset = addr_offset & (UINT32)(~(FLASH_PAGE_SIZE-1));	// align to FLASH_PAGE_SIZE
	lRegStatus = __get_PRIMASK();	// save global interrput
   	__disable_irq();	// close interrupt
	EfcRegs.WEN.all = addr_offset | 0x80000000;
	pSysAddr = (VUINT32 *)(FLASH_BASE_ADDR+addr_offset);
	do
	{
	   	for(i=0;i<(FLASH_PAGE_SIZE/4/FLASH_BUFF_LEN);i++)
		{	
			nRetry = PROG_MAX_TIMES;	
			while(nRetry)
			{
				for(j=0;j<FLASH_BUFF_LEN;j++)	// program 8 word (32 bits)
				{
					*pSysAddr++ = *pData++;	
				}
				EFC_PROG_CMD();
				while(EfcRegs.STS.bit.Busy);
				if(extra == 1)
				{
					if(memcmp((void *)(pSysAddr-FLASH_BUFF_LEN), pData-FLASH_BUFF_LEN, FLASH_BUFF_LEN*4))
					{
						nRetry--;
						pSysAddr -= FLASH_BUFF_LEN;
						pData -= FLASH_BUFF_LEN;
						continue;
					}
					else break;
				}
				else break;
			}
			if(nRetry==0)	// program fail
			{
				nRet = RT_FAIL;
				break;
			}	
		}
		if(nRet!=RT_OK) break;	// if error, do not need extra program
		pSysAddr -= FLASH_PAGE_SIZE/4;
		pData -= FLASH_PAGE_SIZE/4;
	}while(extra--);
	EfcRegs.WEN.all = 0;
	__set_PRIMASK(lRegStatus); 		// recover interrupt
	return nRet;	
}

/***************************************************************************
* Subroutine:	EFC_IsEraseOK
* Function:		Check flash page erase is successful. A page is 512-byte aligned.
* Input:		UINT32 addr_align	-- 	the offset address word-aligned;
* 				UINT32 nLen --		the length of data, size in word.
* Output:		RT_OK or RT_FAIL 
* Description:	check the page including from addr_align to (addr_align+nLen*4-1)
* Date:			2015.09.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_IsEraseOK(UINT32 addr_align, UINT32 nLen)
{
	UINT8 nRet = RT_OK;
    UINT32 lRegStatus;
	UINT32 i;
	UINT32 StrAddr = addr_align/FLASH_PAGE_SIZE;	// start page
	UINT32 EndAddr = (addr_align+nLen*4-1)/FLASH_PAGE_SIZE;	// last page

    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();	// close interrupt
	for(i=StrAddr;i<=EndAddr;i++)
	{
		EfcRegs.CTRL.bit.EV = 1;
		EfcRegs.PEA.all = i*FLASH_PAGE_SIZE;
		EFC_ERASE_CHK_CMD();
		while(EfcRegs.STS.bit.Busy);
		EfcRegs.CTRL.bit.EV = 0;
	    if(EfcRegs.STS.bit.Err_num!=0)
		{
			nRet = RT_FAIL;
			break;				
		}
	}
	__set_PRIMASK(lRegStatus); 	// recover interrupt
	return nRet;	
}

/***************************************************************************
* Subroutine:	EFC_Erase
* Function:		Embedded flash page erase. A page is 512-byte aligned.
* Input:		UINT32 addr_offset	-- 	the flash offset address of byte;
* 				UINT32 page_num --	page number n;
* Output:		None;
* Description:	to erase n pages from the page that the addr_offset included.				
* Date:			2015.09.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_Erase(UINT32 addr_offset, UINT32 page_num)
{
	UINT8 nRetry, nRet = RT_OK;
	UINT32 page;
	UINT32 lRegStatus;
	addr_offset = addr_offset & (UINT32)(~(FLASH_PAGE_SIZE-1));	// align to FLASH_PAGE_SIZE 
	for(page=0;page<page_num;page++)
	{
		nRetry = ERASE_MAX_TIMES;	// erase try times
		while(nRetry)
		{
			lRegStatus = __get_PRIMASK();	// save global interrput
   		 	__disable_irq();	// close interrupt
			EfcRegs.WEN.all = (addr_offset+page*FLASH_PAGE_SIZE) | 0x80000000;
			EfcRegs.PEA.all = addr_offset+page*FLASH_PAGE_SIZE;
			EFC_ERASE_CMD();
			while(EfcRegs.STS.bit.Busy);
			__set_PRIMASK(lRegStatus); 	// recover interrupt
			if(EFC_IsEraseOK(addr_offset, FLASH_PAGE_SIZE/4) != RT_OK) 
			{
				nRetry--;
				continue;
			}
			else break;	
		}
		if(!nRetry) 	// erase fail
		{
			nRet = RT_FAIL;
			break;		
		}	
	} 
	EfcRegs.WEN.all = 0;
	return nRet;
}

/***************************************************************************
* Subroutine:	EFC_ChipErase
* Function:		to erase the EFC flash chip which the specified
* 				byte address is in.
* Input:		None;
* Output:		None;
* Description:	to erase n pages from the page that the addr_offset included.
* 				if EFC_ROM_FUNC_USED is defined, ROM function will be called
* Date:			2014.09.21
* ModifyRecord:
* *************************************************************************/
#pragma arm section code = ".ram_code"
void EFC_ChipErase(void)
{
	#ifndef	EFC_ROM_FUNC_USED	
    UINT32 lRegStatus;
	EFC_ClrProgFlag(AREA_PARA_WE);
	EfcRegs.CHECK = AREA_CHIP_ERASE_WE;
    lRegStatus = __get_PRIMASK();	// save global interrput
    __disable_irq();				// close interrupt
	EfcRegs.WEN.all = ASA_PROG | 0x80000000;
	EfcRegs.PEA.all = ASA_PROG;
	EfcRegs.CTRL.bit.MAS1 = 1;
	EFC_ERASE_CMD();
	while(EfcRegs.STS.bit.Busy);
	__set_PRIMASK(lRegStatus); 		// recover interrupt
	EfcRegs.CHECK = AREA_PROT_DI;
	EfcRegs.CTRL.bit.MAS1 = 0;
	#else
	EFC_ClrProgFlag(AREA_PARA_WE);
	_EFC_ChipErase();	
	#endif	
}
#pragma arm section code

/***************************************************************************
* Subroutine:	FlashWriteBlock
* Function:		to write a Flash Block(page), Block(page) is 512-byte aligned.
* Input:		UINT8 *pBuf	-- 	to specify the data to be written;
* 				UINT32 lBlock -- to specify the block (page) to be written;
* 				UINT32 lOffset -- to specify the offset in block for the data
* 								  to be written start;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:
* Date:			2010.09.02
* ModifyRecord:
* *************************************************************************/
#ifndef	EFC_ROM_FUNC_USED
static UINT8 FlashWriteBlock(UINT8 *pBuf, UINT32 lBlock, UINT32 lOffset, UINT32 lLen)
{
	UINT8 *pTmp;
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	UINT8 bMustErase;
	UINT32 lStartAddr;

 	if((lOffset+lLen)>FLASH_PAGE_SIZE)
	    return RT_PARAM_ERR;

 	lStartAddr=lBlock*FLASH_PAGE_SIZE;
 	// first read back the data
 	pTmp=(UINT8 *)Buf;
 	memcpy(pTmp,(void *)(FLASH_BASE_ADDR+lStartAddr),FLASH_PAGE_SIZE);
	bMustErase=0;
	if(EFC_IsEraseOK(lStartAddr, FLASH_PAGE_SIZE/4) != RT_OK)
		bMustErase=1;
	if(bMustErase)//	//must erase;
	{
	    //erase the block
	   if(EFC_Erase(lStartAddr,1) != RT_OK)
	   	  return RT_FAIL;
	}
	memcpy((pTmp+lOffset), pBuf, lLen);
    //write to flash
    return EFC_PageProg(lStartAddr, (UINT32 *)Buf);
}
#endif

/***************************************************************************
* Subroutine:	EFC_WriteInfo
* Function:		to write Embedded Flash data 
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be written;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:	
* 				
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	#ifndef	EFC_ROM_FUNC_USED
	UINT32 nStart;
	UINT16 nNum;
	UINT16 nStartBlock,nStartOffset;

	UINT8 *pBuf;
	UINT8 nRet;	   

	//check the data range;
	if(lLen<=0)
		return RT_PARAM_ERR;
	nStart=lAddress;
	nStartBlock=(UINT16)(nStart/FLASH_PAGE_SIZE);
	pBuf=pDataBuf;
	nNum=lLen;
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nStart%FLASH_PAGE_SIZE);
		if(nStartOffset)
		{
			
			if(FLASH_PAGE_SIZE-nStartOffset>lLen)
			{		 
				return FlashWriteBlock(pBuf,nStartBlock,nStartOffset,lLen);
			}
			nRet=FlashWriteBlock(pBuf,nStartBlock,nStartOffset,FLASH_PAGE_SIZE-nStartOffset);
			   
			if(nRet!=RT_OK) return nRet;
			    
			pBuf+=FLASH_PAGE_SIZE-nStartOffset;
			nStart+=FLASH_PAGE_SIZE-nStartOffset;
			nNum-=FLASH_PAGE_SIZE-nStartOffset;
			nStartBlock++;
		}
		else if(nNum>=FLASH_PAGE_SIZE)
		{
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,FLASH_PAGE_SIZE);
				
			if(nRet!=RT_OK) return nRet;
						    
		    pBuf+=FLASH_PAGE_SIZE;
		    nStart+=FLASH_PAGE_SIZE;
		    nNum-=FLASH_PAGE_SIZE;
			nStartBlock++;			  	
		}			 
		else
		{
			nRet=FlashWriteBlock(pBuf,nStartBlock,0,nNum);			 
	   	    if(nRet!=RT_OK) return nRet;
			nNum=0;
		}
	}while(nNum>0);
			    
	return RT_OK;
	#else
	return _EFC_WriteInfo(pDataBuf, lAddress, lLen);
	#endif
}

/***************************************************************************
* Subroutine:	EFC_ReadInfo
* Function:		to Read Embedded Flash data 
* Input:		UINT8 *pDataBuf	-- 	to specify the data to be Read;
* 				UINT32 lAddress -- to specify the Flash address;
* 				UINT32 lLen --	to specify the length data to be read
* Output:		UINT8 --	return the result;
* Description:	
* Date:			2013.01.29
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{ 
	VUINT8* pAddr;
	pAddr=(VUINT8*)(lAddress+FLASH_BASE_ADDR);
	memcpy(pDataBuf,(void*)pAddr,lLen);
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	EFC_ClrProgFlag
 * Function:	Clear SY09 code programmed flag and parameter page (parameter page1  &parameter page2)
 * Input:		UINT32 Key -- 	AREA_PARA_WE(0x4444_5678);
 * Output:		None
 * Description: It is recommended the parameter Key is get from USB, UART or
 * 				other communication interface for parameter security.
 * Date:		2012.11.05
 * ModifyRecord:
 * ********************************************************************************/
void EFC_ClrProgFlag(UINT32 Key)
{
	EFC_WrProtect(Key); 
	EFC_Erase(1024*FLASH_PAGE_SIZE, 2);
	EFC_WrProtect(AREA_PROT_DI);
}

/***********************************************************************************
 * Subroutine:	EFC_ClrProgFlagOnly
 * Function:	Clear SY09 code programmed flag and parameter page1(do not clear parameter page2)
 * Input:		UINT32 Key -- 	AREA_PARA_WE(0x4444_5678);
 * Output:		None
 * Description: It is recommended the parameter Key is get from USB, UART or
 * 				other communication interface for parameter security.
 * Date:		2012.11.05
 * ModifyRecord:
 * ********************************************************************************/
void EFC_ClrProgFlagOnly(UINT32 Key)
{
	EFC_WrProtect(Key); 
	EFC_Erase(1024*FLASH_PAGE_SIZE, 1);
	EFC_WrProtect(AREA_PROT_DI);
}
/***********************************************************************************
 * Subroutine:	EFC_ClrDESFlag
 * Function:	Clear SY09 parameter page2(do not clear parameter page1)
 * Input:		UINT32 Key -- 	AREA_PARA_WE(0x4444_5678);
 * Output:		None
 * Description: It is recommended the parameter Key is get from USB, UART or
 * 				other communication interface for parameter security.
 * Date:		2012.11.05
 * ModifyRecord:
 * ********************************************************************************/
void EFC_ClrDESFlag(UINT32 Key)
{
	EFC_WrProtect(Key); 
	EFC_Erase(1025*FLASH_PAGE_SIZE, 1);
	EFC_WrProtect(AREA_PROT_DI);
}

/***************************************************************************
* Subroutine:	EFC_KEYWrite
* Function:		to write Embedded Flash key area
* Input:		EFC_AREA_KEY_ID nKeyAreaID	-- 	to specify the number of the KEY AREA ID
*				UINT8 *pDataBuf	-- 	to specify the data to be written;
* 				UINT32 lAddress -- the offset address in sub key flash area;
* 				UINT32 lLen --	to specify the length data to be written
* Output:		UINT8 --	return the result;
* Description:	this function is used to write KEY area , the address from 
* 				(ASA_KEY-ASA_PARA).
* 				If the address out of the range, RT_PARAM_ERR will be returned.
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_KEYWrite(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen)
{
	UINT32 nStartOffset;
	UINT8 nRet;

	if(nKeyAreaID>EFC_AREA_KEY_3) return RT_PARAM_ERR;
	if(lAddress>0xFFF) return RT_PARAM_ERR;
	if((lAddress+lLen)>0x1000) return RT_PARAM_ERR;

	nStartOffset = ASA_KEY + nKeyAreaID*0x1000;	
	EFC_WrProtect(AREA_KEY_WE);
	EFC_KEYWrProtect(nKeyPermission[nKeyAreaID]);
	nRet = EFC_WriteInfo(pDataBuf,nStartOffset + lAddress, lLen);
	EFC_WrProtect(AREA_PROT_DI);
	return nRet;
}

/***************************************************************************
* Subroutine:	EFC_SetEoSt
* Function:		to enable execute only area 
* Input:		UINT32 nPage  -- Page number;
* Output:		UINT8 --	return the result;
* Description:
* Date:			2012.10.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_SetEoSt(UINT32 nPage)
{
	UINT32 nEoStConfigAddr = AREA_EO_EN;

	if(EfcRegs.STS.bit.Exe_en)
		return RT_FAIL;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr) == 0xFFFFFFFF)
	{
		nPage *= FLASH_PAGE_SIZE;
		nPage += FLASH_BASE_ADDR;
		EFC_WrProtect(AREA_EO_WE); //AREA_OTP_WE
		EfcRegs.WEN.all = 0x80000000 | nEoStConfigAddr;
		EFC_Program((UINT32 *)&nPage, nEoStConfigAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nPage == *(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}
/***************************************************************************
* Subroutine:	EFC_SetKeySt
* Function:		to enable KEY area 
* Input:		UINT32 nPage  -- Page number;
* Output:		UINT8 --	return the result;
* Description:
* Date:			2012.10.17
* ModifyRecord:
* *************************************************************************/
UINT8 EFC_SetKeySt(UINT32 nPage)
{
	UINT32 nEoStConfigAddr = AREA_KEY_EN;

	if((EfcRegs.STS.bit.Key_en)||(EfcRegs.STS.bit.Exe_en))
		return RT_FAIL;

	if(*(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr) == 0xFFFFFFFF)
	{
		nPage *= FLASH_PAGE_SIZE;
		nPage += FLASH_BASE_ADDR;
		EFC_WrProtect(AREA_EO_WE); //AREA_OTP_WE
		EfcRegs.WEN.all = 0x80000000 | nEoStConfigAddr;
		EFC_Program((UINT32 *)&nPage, nEoStConfigAddr, 1);
		EFC_WrProtect(AREA_PROT_DI);
		if(nPage == *(UINT32 *)(FLASH_BASE_ADDR+nEoStConfigAddr)) return RT_OK;
		else return RT_FAIL;
	}
	else return RT_FAIL;
}

