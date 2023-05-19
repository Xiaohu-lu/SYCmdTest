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
*	|*********���ݴ洢��************|***д����������****|**����״̬��**|
* 	-------------------------------------------------------------------------
*  /|\							   /|\				   /|\            /|\
*	|  								|					|              |
* gSF_Start_Block			  gSF_End_Block	                
* nStartBlock				  BK_START_PAGE       gBK_Status_Page   nEndBlock
* ModifyRecord :
* 1. ��STRUCT_BK_STATUS�ṹ���ԱlStatus�������һ���������д״̬ҳʱ������ڵı�־
*    �Ѿ�д�룬����������δ��ȷд���������	2013.04.08	Coins
* 2. �޸�д�����ݷ�ΧԽ��ıȽϷ�ʽ����>=�Ƚϸ�Ϊ>�Ƚϣ����д���ݴ洢����
*	 ���һҳʱ���ز��������bug��	2013/05/13	coins
* 3. ����ǰ�����ж��Ƿ��Ѿ��������Լ��ٶ��Ѳ���ҳ��д��ʱ�䡣2013/05/13	coins 
* 4. �޸ı��ݷ�ʽ����ԭ������״̬����־������ÿ������ҳ�У���дһ�εĲ���������ԭ����3��
*	 ��Ϊ2�Σ�ʱ����89ms��Ϊ69ms��	2013/05/14	coins
* 5. �޸ĺ���SF_WriteData�������д�����ݳ���С�ڵ�ҳƫ�ƺ󲿷ֵĳ���ʱ��
*	 д���������RT_PARAM_ERR��bug�� 2013/05/15	coins
* 6. �޸ı��ݻָ�bug, ���ݻָ�ʱ��ԭ���ķ�ʽ��ÿҳ������Ƿ���Ҫ�ָ����ݣ�
*	 �������ܴ������⣬��������д����ڲ�������ҳʱ���磬����ֻ�����˱��ݵ���ʼҳ��
*	 �����ϵ�ָ�ʱ����ָ�����ı���ҳ������д��������ݱ������ݸ��ǡ�
*	 �����޸ĳɰ�����ָ����ָ������ݱ�־��ЧҳΪֹ���Ժ���ҳ�����ټ�顣
*	 2015/01/16	coins
* 7. ��STRUCT_BK_STATUS�ṹ���ԱlStatus������������״̬��ҳ�������д����ҳʱ������ڵı�־
*    �Ѿ�д�룬����������δ��ȷд���������
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

#define BK_PAGE_NUM   			8 					//������ҳ�� 
#define BK_STATUS_PAGE_NUM   	1 					//����״̬ҳ�� 
#define	BK_START_PAGE			(gBK_Start_Page)	//��������ʼҳ���Ǵ洢������ҳ
#define STATUS_BK 				0x22339876 			//˵���Ǳ���״̬. 
#define	SF_PAGE_SIZE			FLASH_PAGE_SIZE		//��ȫ�洢����ҳ��С��512�ֽڣ�

/**********************************************************************************
* Local Type Definition
**********************************************************************************/
typedef struct { 	// 2015.10.12	Coins
	UINT32 lStatus;			//״̬��־ 
    UINT32 lStartPage;		//((~����������ʼҳ��ַ)<<16)|����������ʼҳ��ַ
	UINT32 lStartPageCHK;	//lStartPageУ��ֵ
	UINT32 lPageNum;		//((~���ݵ�ҳ��)<<16)|���ݵ�ҳ��
	UINT32 lPageNumCHK;		//lPageNumУ��ֵ
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
	// �ж��Ƿ��Ѳ��� 2013.05.13 coins
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
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
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
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
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
*	1.�޸ı��ݻָ�bug, ���ݻָ�ʱ��ԭ���ķ�ʽ��ÿҳ������Ƿ���Ҫ�ָ����ݣ�
*	�������ܴ������⣬��������д����ڲ�������ҳʱ���磬����ֻ�����˱��ݵ���ʼҳ��
*	�����ϵ�ָ�ʱ����ָ�����ı���ҳ������д��������ݱ������ݸ��ǡ�
*	�����޸ĳɰ�����ָ����ָ������ݱ�־��ЧҳΪֹ���Ժ���ҳ�����ټ�顣
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
*	1. �޸�д�����ݷ�ΧԽ��ıȽϷ�ʽ����>=�Ƚϸ�Ϊ>�Ƚϣ����д���ݴ洢����
*	���һҳʱ���ز��������bug��	2013/05/13	coins
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
 	
 	//����Ҫ���ݵ�ҳ�� 
 	if(lOffset==0)
 	   lBlockNum=(lLen+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	else
 	   lBlockNum=(lOffset+SF_PAGE_SIZE-1)/SF_PAGE_SIZE+(lLen-(SF_PAGE_SIZE-lOffset)+SF_PAGE_SIZE-1)/SF_PAGE_SIZE;
 	
 	if(lBlockNum>BK_PAGE_NUM)//����������ܳ�����������ҳ�� 
 	    return RT_PARAM_ERR;
 	// 2013/05/13 coins �޸�bug����>=�Ƚϸ�Ϊ>�Ƚϣ�����д���ݴ洢�������һҳʱ���ز�������    
    if((lStartBlock+lBlockNum)>gSF_End_Block)	// ���ܳ�����ȫ�洢��.
 	    return RT_PARAM_ERR; 	
 	
 	//1---�����ݰ��Ƶ��������������ñ��ݱ�־
     if(SF_BackupPage(lStartBlock,BK_START_PAGE,lBlockNum, (UINT32 *)Buf) != RT_OK)
		return RT_FAIL;
      
    //2---д������  
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
		// �ж��Ƿ��Ѳ��� 2013.05.13 coins
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

    // 3--�������ݱ�־
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
   
 	// ��ȫFlash�洢������λ��MPU�ı����������ռ�
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
* 1. �޸ĵ�д�����ݳ���С�ڵ�ҳƫ�ƺ󲿷ֵĳ���ʱ��д���������RT_PARAM_ERR��bug��	2013/05/15	coins
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
