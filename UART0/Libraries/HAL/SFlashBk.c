 /**********************************************************************************
* File name    :	SFlashBk.c
* Function     :	Safe flash operate by backup mode.
* Author       : 	Emil
* Date         :	2016/07/02
* Version      :    v1.1
* Description  :
* The structure of Save Flash Area:
*
*   |<------- SF_Init:	  nEndBlock - nStartBlock	        ---------->|
*									|<-- BK_PAGE_NUM -->|<-- 1_PAGE -->|
* 	--------------------------------------------------------------------
*	|*********数据存储区************|***写操作备份区****|**备份状态区**|
* 	-------------------------------------------------------------------------
*  /|\							   /|\				   /|\            /|\
*	|  								|					|              |
* gSF_Start_Block			  gSF_End_Block	                
* nStartBlock				  BK_START_PAGE       gBK_Status_Page   nEndBlock
* ModifyRecord :
* 1. 将STRUCT_BK_STATUS结构体成员lStatus移至最后一个，解决在写状态页时掉电存在的标志
*    已经写入，而其它参数未正确写入的隐患。	2013.04.08	Coins
* 2. 修改写入数据范围越界的比较方式，将>=比较改为>比较，解决写数据存储区的
*	 最后一页时返回参数错误的bug。	2013/05/13	coins
* 3. 擦除前增加判断是否已经擦除，以减少对已擦除页的写入时间。2013/05/13	coins 
* 4. 修改备份方式，将原来备份状态及标志放置在每个备份页中，是写一次的擦除操作由原来的3次
*	 改为2次，时间由89ms降为69ms。	2013/05/14	coins
* 5. 修改函数SF_WriteData，解决当写入数据长度小于当页偏移后部分的长度时，
*	 写入操作返回RT_PARAM_ERR的bug。 2013/05/15	coins
* 6. 修改备份恢复bug, 备份恢复时，原来的方式是每页都检查是否需要恢复备份，
*	 这样可能存在问题，当新数据写入后在擦除备份页时掉电，可能只擦除了备份的起始页，
*	 重新上电恢复时，会恢复后面的备份页，导致写入的新数据被就数据覆盖。
*	 现在修改成按次序恢复，恢复到备份标志无效页为止，对后续页将不再检查。
*	 2015/01/16	coins
* 7. 将STRUCT_BK_STATUS结构体成员lStatus移至单独备份状态区页，解决在写备份页时掉电存在的标志
*    已经写入，而其它参数未正确写入的隐患。
*	 2015/10/12	Emil
**********************************************************************************/
/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SY09.h"

#ifdef	SFLASH_BACKUP_MODE_USED 
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/ 
//#define	EO_KEY_USED_FOR_SF			// if EO and KEY AREA is used for SF, please open EO_KEY_USED_FOR_SF

#ifdef	EO_KEY_USED_FOR_SF
#define	AREA_EO_PARA_ADD	0x1007BE00
#define	AREA_EO_PARA_PAGE	991						
#endif

#define BK_PAGE_NUM   			8 					//备份区页数 
#define BK_STATUS_PAGE_NUM   	1 					//备份状态页数 
#define	BK_START_PAGE			(gBK_Start_Page)	//备份区起始页就是存储区结束页
#define STATUS_BK 				0x22339876 			//说明是备份状态. 
#define	SF_PAGE_SIZE			FLASH_PAGE_SIZE		//安全存储数据页大小（512字节）

/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 	// 2015.10.12	Coins
	UINT32 lStatus;			//状态标志 
    UINT32 lStartPage;		//((~备份数据起始页地址)<<16)|备份数据起始页地址
	UINT32 lStartPageCHK;	//lStartPage校验值
	UINT32 lPageNum;		//((~备份的页数)<<16)|备份的页数
	UINT32 lPageNumCHK;		//lPageNum校验值
}STRUCT_BK_STATUS; 	

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/
static UINT16 gSF_Start_Block, gSF_End_Block;
static UINT16 gBK_Start_Page, gBK_Status_Page;

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
* Subroutine:	SF_IsBufAllOne
* Function:		Check buffer all 1 
* Input:	 	void *pData -- the buffer
*				UINT32 nSize -- the buffer size (word number)
* Output:		RT_OK
* Description:	 
* Date:			2013.05.14
* ModifyRecord:
* *************************************************************************/
//static UINT8 SF_IsBufAllOne(void *pData, UINT32 nSize)
//{
//	UINT32 i;
//	UINT32 *p = pData;
//	for(i=0;i<nSize;i++) 
//	{
//		if(p[i] != 0xFFFFFFFF) return FALSE;
//	}
//	return TRUE;
//}

#ifdef	EO_KEY_USED_FOR_SF
/**************************************************************************
* Subroutine:	SF_MpuWrEnable
* Function:		Embedded flash write and erase protect enable or disable with proper check code.
* Input:		nOffsetAddr -- Offset address
* Output:		
* Description:
* Date:			2013.01.16
* ModifyRecord:
* *************************************************************************/
static UINT8 SF_MpuWrEnable(UINT32 nOffsetAddr)
{
	if(nOffsetAddr<(EfcRegs.DATAPRO-FLASH_BASE_ADDR))		   
	{
		EFC_WrProtect(AREA_PROG_WE);
	}
	else if(nOffsetAddr<ASA_EO) 							
	{
		EFC_WrProtect(AREA_DATAPRO_WE);	
	}
	else if(nOffsetAddr<ASA_KEY) 							
	{
		if(EfcRegs.STS.bit.Exe_en)
			return RT_FAIL;
		EFC_WrProtect(AREA_EO_WE);	
	}
	else if(nOffsetAddr<ASA_PARA) 							
	{
		if(EfcRegs.STS.bit.Key_en)
			return RT_FAIL;
		EFC_WrProtect(AREA_KEY_WE);	
	}
	else
	{
	   return RT_FAIL;
	}
	return RT_OK;
}
#endif
/***************************************************************************
* Subroutine:	SF_WriteBKStatus
* Function:		Write Save Flash data in backup status.
* Input:	 	UINT32 lStartPage -- the Save Flash data Start page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK RT_FAIL
* Description:	 
* Date:			2015.10.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 SF_WriteBKStatus(UINT32 lStartPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	STRUCT_BK_STATUS tBk;

	#ifdef	EO_KEY_USED_FOR_SF
	if(SF_MpuWrEnable(gBK_Status_Page*FLASH_PAGE_SIZE) != RT_OK)
		return RT_FAIL;
	#endif 
	// 判断是否已擦除 2013.05.13 coins
	//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
	if(EFC_IsEraseOK(gBK_Status_Page*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4) != RT_OK)	
	{		
		if(EFC_Erase(gBK_Status_Page*FLASH_PAGE_SIZE,1) != RT_OK)	
		{
			#ifdef	EO_KEY_USED_FOR_SF		
			EFC_WrProtect(AREA_PROT_DI);
			#endif
			return RT_FAIL;
		}
	}
	tBk.lStartPage = ((~lStartPage)<<16)|lStartPage;
	tBk.lStartPageCHK = ~tBk.lStartPage;
	tBk.lPageNum = ((~lPageNum)<<16)|lPageNum;
	tBk.lPageNumCHK = ~tBk.lPageNum;
	tBk.lStatus = STATUS_BK;
   	memcpy((UINT8 *)pPageBuf, (UINT8 *)&tBk, sizeof(STRUCT_BK_STATUS));
	if(EFC_PageProg(gBK_Status_Page*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
	{
		#ifdef	EO_KEY_USED_FOR_SF		
		EFC_WrProtect(AREA_PROT_DI);
		#endif
		return RT_FAIL;
	}
	#ifdef	EO_KEY_USED_FOR_SF		
	EFC_WrProtect(AREA_PROT_DI);
	#endif				
	return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_BackupPage
* Function:		Move several pages from data area to backup area.
* Input:	 	UINT32 lSrcPage -- the source flash page number;
* 				UINT32 lDstPage --	the destination flash page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2013.05.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 SF_BackupPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;
	UINT8 nOffsetSrcPage = 0;
	UINT8 nOffsetDstPage = 0;
	for(i=0;i<lPageNum;i++)
	{
		#ifdef	EO_KEY_USED_FOR_SF
		nOffsetSrcPage = 0;
		nOffsetDstPage = 0;
		if((lSrcPage + i) >= AREA_EO_PARA_PAGE)
			nOffsetSrcPage = 1;
		if((lDstPage + i) >= AREA_EO_PARA_PAGE)
			nOffsetDstPage = 1;
		if(SF_MpuWrEnable((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE) != RT_OK)
			return RT_FAIL;
		#endif 
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+nOffsetSrcPage+i)*FLASH_PAGE_SIZE, SF_PAGE_SIZE);	
		// 判断是否已擦除 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(EFC_IsEraseOK((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4) != RT_OK)	
		{		
			if(EFC_Erase((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE,1) != RT_OK)	
			{
				#ifdef	EO_KEY_USED_FOR_SF		
				EFC_WrProtect(AREA_PROT_DI);
				#endif
				return RT_FAIL;
			}
		}
		if(EFC_PageProg((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
		{
			#ifdef	EO_KEY_USED_FOR_SF		
			EFC_WrProtect(AREA_PROT_DI);
			#endif
			return RT_FAIL;
		}	
	}
	return SF_WriteBKStatus(lSrcPage, lPageNum, pPageBuf);	
}

/***************************************************************************
* Subroutine:	SF_RecoverPage
* Function:		Move several pages from backup area to data area.
* Input:	 	UINT32 *lSrcPage -- the source flash page number;
* 				UINT32 lDstPage --	the destination flash page number;
* 				UINT32 lPageNum  --	the numbers to move;
*				UINT32 *pPageBuf -- to specify a page buffer (FLASH_PAGE_SIZE size).
* Output:		RT_OK.
* Description:	the destination pages and source pages must be not overlapped.	 
* Date:			2013.05.14
* ModifyRecord: 
* *************************************************************************/
static UINT8 SF_RecoverPage(UINT32 lSrcPage, UINT32 lDstPage, UINT32 lPageNum, UINT32 *pPageBuf)
{
	UINT8 i;
	UINT8 nOffsetSrcPage = 0;
	UINT8 nOffsetDstPage = 0;
	for(i=0;i<lPageNum;i++)
	{
		#ifdef	EO_KEY_USED_FOR_SF
		nOffsetSrcPage = 0;
		nOffsetDstPage = 0;
		if((lSrcPage + i) >= AREA_EO_PARA_PAGE)
			nOffsetSrcPage = 1;
		if((lDstPage + i) >= AREA_EO_PARA_PAGE)
			nOffsetDstPage = 1;
		if(SF_MpuWrEnable((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE) != RT_OK)
			return RT_FAIL;
		#endif 
		EFC_ReadInfo((UINT8 *)pPageBuf,(lSrcPage+nOffsetSrcPage+i)*FLASH_PAGE_SIZE, SF_PAGE_SIZE);	
		// 判断是否已擦除 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)((lDstPage+i)*FLASH_PAGE_SIZE+FLASH_BASE_ADDR), FLASH_PAGE_SIZE/4))
		if(EFC_IsEraseOK((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4) != RT_OK)	
		{	
			if(EFC_Erase((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE,1) != RT_OK)
			{
				return RT_FAIL;
			}
		}
		if(EFC_PageProg((lDstPage+nOffsetDstPage+i)*FLASH_PAGE_SIZE, pPageBuf) != RT_OK)
		{
			return RT_FAIL;
		}
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	SF_CheckBKStatus
* Function:		Check Save Flash data in backup status. when in back up, to recover data. 
* Input:	 	None
* Output:		RT_OK
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
*	1.修改备份恢复bug, 备份恢复时，原来的方式是每页都检查是否需要恢复备份，
*	这样可能存在问题，当新数据写入后在擦除备份页时掉电，可能只擦除了备份的起始页，
*	重新上电恢复时，会恢复后面的备份页，导致写入的新数据被就数据覆盖。
*	现在修改成按次序恢复，恢复到备份标志无效页为止，对后续页将不再检查。
*	2015/01/16	Coins
* *************************************************************************/
static UINT8 SF_CheckBKStatus(void)
{
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	STRUCT_BK_STATUS tBK; 
    UINT8 nRet = RT_OK;
	UINT32 lStartPage; 
	UINT32 lPageNum;
	// to read back up status from backup page
	EFC_ReadInfo((UINT8*)&tBK,gBK_Status_Page*FLASH_PAGE_SIZE,sizeof(tBK));	
	if(((tBK.lStartPage^tBK.lStartPageCHK) == 0xFFFFFFFF)&&((tBK.lPageNum^tBK.lPageNumCHK) == 0xFFFFFFFF)&&(tBK.lStatus == STATUS_BK))	// the page have been backup, need to recover
	{
		lStartPage = (tBK.lStartPage&0x0000FFFF);
		lPageNum = (tBK.lPageNum&0x0000FFFF);
		if(((lStartPage^(tBK.lStartPage>>16)) == 0x0000FFFF)&&((lPageNum^(tBK.lPageNum>>16)) == 0x0000FFFF))
		{
			#ifndef	EO_KEY_USED_FOR_SF
	        EFC_WrProtect(AREA_DATAPRO_WE);
			#endif
			nRet = SF_RecoverPage(BK_START_PAGE, lStartPage, lPageNum, Buf);	// copy back from backup pages					 			
			EFC_WrProtect(AREA_PROT_DI);
			if(nRet != RT_OK)
				return RT_FAIL;
		}	      	
	}
	// erase backup Status page	
	if(EFC_IsEraseOK(gBK_Status_Page*FLASH_PAGE_SIZE, FLASH_PAGE_SIZE/4) != RT_OK)
	{
		#ifdef	EO_KEY_USED_FOR_SF
		if(SF_MpuWrEnable(gBK_Status_Page*FLASH_PAGE_SIZE) != RT_OK)
			return RT_FAIL;
		#else
		EFC_WrProtect(AREA_DATAPRO_WE);
		#endif 
		nRet = EFC_Erase(gBK_Status_Page*FLASH_PAGE_SIZE,1);	// erase current backup page
		EFC_WrProtect(AREA_PROT_DI);
		if(nRet != RT_OK)
			return RT_FAIL;
	}
	EFC_WrProtect(AREA_PROT_DI);				
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_WriteBlocks
* Function:		Multi-page save write, the page number should be less than
*				or equal to BK_PAGE_NUM. 
* Input:	 	UINT8 *pBuf -- the data to be written;
*             	UINT32 lStartBlock -- the start page;
*             	UINT32 lOffset -- the offset in the page; 
*				UINT32 lLen -- the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:	
*	1. 修改写入数据范围越界的比较方式，将>=比较改为>比较，解决写数据存储区的
*	最后一页时返回参数错误的bug。	2013/05/13	coins
* *************************************************************************/
UINT8 SF_WriteBlocks(UINT8 *pBuf, UINT32 lStartBlock, UINT32 lOffset, UINT32 lLen)
{
	UINT32 j;
	UINT8 *pAddr;
	UINT8 *pTmp;
	UINT32 Buf[FLASH_PAGE_SIZE/sizeof(UINT32)];
	//UINT8 bMustErase;
	UINT32 lStartAddr,lBlockNum,lWriteLen;
	UINT32 lStartAddrTemp;
	  
 	lStartAddr=lStartBlock*FLASH_PAGE_SIZE;
 	
 	//计算要备份的页数 
 	if(lOffset==0)
 	   lBlockNum=(lLen+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	else
 	   lBlockNum=(lOffset+SF_PAGE_SIZE-1)/SF_PAGE_SIZE+(lLen-(SF_PAGE_SIZE-lOffset)+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	
 	if(lBlockNum>BK_PAGE_NUM)//跨过的区域不能超过备份区的页数 
 	    return RT_PARAM_ERR;
 	// 2013/05/13 coins 修改bug，将>=比较改为>比较，否则写数据存储区的最后一页时返回参数错误    
    if((lStartBlock+lBlockNum)>gSF_End_Block)	// 不能超过安全存储区.
 	    return RT_PARAM_ERR; 	
 	
 	//1---把数据搬移到备份区，并设置备份标志
     if(SF_BackupPage(lStartBlock,BK_START_PAGE,lBlockNum, (UINT32 *)Buf) != RT_OK)
		return RT_FAIL;
      
    //2---写新数据  
    pAddr=pBuf;
	pTmp = (UINT8 *)Buf;
    for(j=0;j<lBlockNum;j++)
    {
		lStartAddrTemp = lStartAddr;
		#ifdef	EO_KEY_USED_FOR_SF
		if(lStartAddrTemp >= (AREA_EO_PARA_ADD-FLASH_BASE_ADDR))
			lStartAddrTemp += FLASH_PAGE_SIZE;
		if(SF_MpuWrEnable(lStartAddrTemp) != RT_OK)
			return RT_FAIL;
		#endif  	 
		EFC_ReadInfo(pTmp, lStartAddrTemp, FLASH_PAGE_SIZE);// modify SF_PAGE_SIZE to FLASH_PAGE_SIZE, 2015/01/16, coins 
		//erase the block
		// 判断是否已擦除 2013.05.13 coins
		//if(!SF_IsBufAllOne((void *)Buf, SF_PAGE_SIZE/4))
		if(EFC_IsEraseOK(lStartAddrTemp, FLASH_PAGE_SIZE/4) != RT_OK)// modify SF_PAGE_SIZE to FLASH_PAGE_SIZE, 2015/01/16, coins
		{
        	if(EFC_Erase(lStartAddrTemp,1) != RT_OK)
			{
				#ifdef	EO_KEY_USED_FOR_SF		
				EFC_WrProtect(AREA_PROT_DI);
				#endif
				return RT_FAIL;
			}
		}
    	// copy data to write buffer
    	if((lLen+lOffset)>SF_PAGE_SIZE)
    	     lWriteLen=SF_PAGE_SIZE-lOffset;    	    
    	else
    	     lWriteLen=lLen;
    	     
        memcpy((pTmp+lOffset), pAddr, lWriteLen); 
    
        //write to flash
        if(EFC_PageProg(lStartAddrTemp, (UINT32 *)Buf) != RT_OK)
		{
			#ifdef	EO_KEY_USED_FOR_SF		
			EFC_WrProtect(AREA_PROT_DI);
			#endif
			return RT_FAIL;
		}
		// next page
        lStartAddr+=FLASH_PAGE_SIZE;
        pAddr+=lWriteLen;
        lOffset=0;
        lLen-=lWriteLen;
    }

    // 3--擦除备份标志
	#ifdef	EO_KEY_USED_FOR_SF
	if(SF_MpuWrEnable(gBK_Status_Page*FLASH_PAGE_SIZE) != RT_OK)
		return RT_FAIL;
	#endif  
	if(EFC_Erase(gBK_Status_Page*FLASH_PAGE_SIZE,1) != RT_OK)
	{
		#ifdef	EO_KEY_USED_FOR_SF		
		EFC_WrProtect(AREA_PROT_DI);
		#endif
		return RT_FAIL;
	}
	#ifdef	EO_KEY_USED_FOR_SF		
	EFC_WrProtect(AREA_PROT_DI);
	#endif	 
    return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_BkInit
* Function:		Init Safe flash block
* Input:		UINT16 nStartBlock --	the start block(page).
                UINT16 nEndBlock --	the end block(page).
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_BkInit(UINT16 nStartBlock, UINT16 nEndBlock)
{
#ifndef	EO_KEY_USED_FOR_SF
	if(nEndBlock>896) return RT_PARAM_ERR;
	if(nEndBlock<nStartBlock) return RT_PARAM_ERR; 
    if((nEndBlock-nStartBlock)>896 || (nEndBlock-nStartBlock)<(2*BK_PAGE_NUM+BK_STATUS_PAGE_NUM))
    	return RT_PARAM_ERR; 

	gSF_Start_Block=nStartBlock;
    gSF_End_Block=nEndBlock-BK_PAGE_NUM-BK_STATUS_PAGE_NUM;
	gBK_Start_Page=gSF_End_Block;
    gBK_Status_Page=gBK_Start_Page+BK_PAGE_NUM;
#else
	if(nEndBlock>1024) return RT_PARAM_ERR;
	if(nEndBlock<nStartBlock) return RT_PARAM_ERR; 
    if((nEndBlock-nStartBlock)>1000 || (nEndBlock-nStartBlock)<(2*BK_PAGE_NUM+BK_STATUS_PAGE_NUM+1))
    	return RT_PARAM_ERR;
		
	gSF_Start_Block=nStartBlock;
    gSF_End_Block=nEndBlock-BK_PAGE_NUM-BK_STATUS_PAGE_NUM-1;
	gBK_Start_Page=gSF_End_Block;
    gBK_Status_Page=gBK_Start_Page+BK_PAGE_NUM; 
	if(gBK_Status_Page >= AREA_EO_PARA_PAGE)
		gBK_Status_Page += 1;
#endif
   
 	// 安全Flash存储区必须位于MPU的保护数据区空间
	if(EfcRegs.DATAPRO>FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE) 
 		EfcRegs.DATAPRO=FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE;

    return SF_CheckBKStatus();    
}
 
/***************************************************************************
* Subroutine:	SF_BkWriteData
* Function:		write data to save flash area.
* Input:		UINT8* pDataBuf --	the data to be written save flash area;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK or RT_PARAM_ERR;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* 1. 修改当写入数据长度小于当页偏移后部分的长度时，写入操作返回RT_PARAM_ERR的bug。	2013/05/15	coins
* *************************************************************************/
UINT8 SF_BkWriteData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
 
	UINT32 lWriteLen;
	UINT16 nNum;
	UINT16 nStartBlock,nStartOffset;

	UINT8 *pBuf;
	UINT8 nRet;
	
	//check the data range;
	if(nDataNum<=0)
		return RT_PARAM_ERR;

	pBuf=pDataBuf;
	nNum=nDataNum;

#ifndef	EO_KEY_USED_FOR_SF	
	EFC_WrProtect(AREA_DATAPRO_WE);
#endif
	//loop to write data to flash
	do{
		nStartOffset=(UINT16)(nOffset%SF_PAGE_SIZE);
		nStartBlock=(UINT16)(gSF_Start_Block+nOffset/SF_PAGE_SIZE);
		 	
    	if(nNum>(BK_PAGE_NUM*SF_PAGE_SIZE-nStartOffset)) 	// 201/05/15	coins
    	{
    		lWriteLen=BK_PAGE_NUM*SF_PAGE_SIZE-nStartOffset;
    	}
        else
           	lWriteLen=nNum;
 
        nRet=SF_WriteBlocks(pBuf,nStartBlock,nStartOffset,lWriteLen);    	
 
		if(nRet!=RT_OK) 
		{
			#ifndef	EO_KEY_USED_FOR_SF	
			EFC_WrProtect(AREA_PROT_DI);
			#endif     
            return nRet;
		}        

		pBuf+=lWriteLen;
		nOffset+=lWriteLen;
		nNum-=lWriteLen;
		   
	}while(nNum>0);
#ifndef	EO_KEY_USED_FOR_SF		
	EFC_WrProtect(AREA_PROT_DI);
#endif     
	return RT_OK;
}
 
/***************************************************************************
* Subroutine:	SF_BkReadData
* Function:		read data from save flash area
* Input:		UINT8* pDataBuf -- 	the buffer to save data for read;
*				UINT32 nOffset --	the offset address in save flash area;
*				UINT16 nDataNum --	the length to be written.
* Output:		RT_OK;
* Description:	 
* Date:			2013.03.14
* ModifyRecord:
* *************************************************************************/
UINT8 SF_BkReadData(UINT8* pDataBuf, UINT32 nOffset, UINT16 nDataNum)
{
	VUINT8* pAddr;
	UINT16 nStartBlock,nStartOffset; 

	if((nOffset+nDataNum)>(gSF_End_Block-gSF_Start_Block)*SF_PAGE_SIZE) return RT_PARAM_ERR;
	while(nDataNum)
	{
		nStartOffset=(UINT16)(nOffset%SF_PAGE_SIZE);
		nStartBlock=(UINT16)(gSF_Start_Block+nOffset/SF_PAGE_SIZE);
		pAddr = (VUINT8*)FLASH_BASE_ADDR+nStartBlock*FLASH_PAGE_SIZE+nStartOffset;
		#ifdef	EO_KEY_USED_FOR_SF
		if((UINT32)pAddr >= AREA_EO_PARA_ADD)
			pAddr += FLASH_PAGE_SIZE;
		#endif
		if(nDataNum>SF_PAGE_SIZE-nStartOffset)
		{
			memcpy(pDataBuf,(void*)pAddr,SF_PAGE_SIZE-nStartOffset);
			nDataNum -= SF_PAGE_SIZE-nStartOffset;
			nOffset += SF_PAGE_SIZE-nStartOffset;
			pDataBuf += SF_PAGE_SIZE-nStartOffset;	
		}
		else
		{
			memcpy(pDataBuf,(void*)pAddr,nDataNum);
			nDataNum = 0;
		}
	}	
	return RT_OK;
}

#endif
