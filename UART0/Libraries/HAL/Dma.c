/***************************************************************************************
* File name    :	Dma.c
* Function     :	The functions for DMA
* Author       : 	Coins
* Date         :	2015/03/08	
* Version      :    v1.0
* Description  :    DMA Hardware Abstract Layer
* ModifyRecord :
*****************************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09.h"
/**************************************************************************
* Local Macro Definition
***************************************************************************/


/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
// define the peripheral address for the DMA
const UINT32 gDmaPerAddrList[] = {
	0x40028060,		// SSI TX
	0x40028064,	   	// SSI RX
	0x4006801C,	 	// LOCSC
	0x40048014,	   	// VPWM
	0x40014000,	  	// SPI0 TX
	0x40014004,		// SPI0 RX
	0x4006C000,		// SPI1 TX
	0x4006C004,		// SPI1 RX	
};

static FP_DMA_CH_ISR_CB fpDmaCHxIsrCb[DMA_NUM_CHANNELS];

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
 * Subroutine:	DMA_Init
 * Function:	Init DMA for specified channel
 * Input:		UINT8  nChn -- DMA channel, values 0 ~ DMA_NUM_CHANNELS-1
 * 				UINT8  nPrio -- DMA priority for the channel, values 0 ~ DMA_NUM_CHANNELS
 *				STRUCT_DMA_DEV *pSrcDev -- the source device for the DMA channel
 *				STRUCT_DMA_DEV *pDestDev --  the destination device for the DMA channel
 *				UINT8 nIntType -- to specify the interrupt, OR-locgic can be used 
 *								to the items: DMA_INT_TFR, DMA_INT_BLOCK, 
 *								DMA_INT_SRC_TRAN, DMA_INT_DST_TRAN, DMA_INT_ERR 
 *				FP_DMA_CH_ISR_CB fp -- to specify the interrupt service routine for the channel
 * Output:		UINT8 -- 	RT_OK, RT_FAIL, RT_PARAM_ERR
 * Description:
 * Date:		2014/10/15
 * ModifyRecord:  
 * 1. Add nohandshaking mode   2015.10.30, by coins	 
 * 2. Add DMA interrupt call back to the channel for application processing
 * ********************************************************************************/
UINT8 DMA_Init(UINT8 nChn, UINT8 nPrio, STRUCT_DMA_DEV *pSrcDev, STRUCT_DMA_DEV *pDestDev, \
		UINT8 nIntType, FP_DMA_CH_ISR_CB fp)
{
	UINT32 nChnMask = (1<<nChn);
	UINT32 nCtlL = 0, nCfgH = 0;
	UINT8 i;
	// 1. check parameters
	if((nChn>DMA_NUM_CHANNELS) || (nPrio>DMA_NUM_CHANNELS)) return RT_PARAM_ERR;
	// 2. enable module
	SCM_EnableModClk(ID_DMA);
	SCM_ReleaseModRst(ID_DMA);
	// 3. read the Channel Enable Register for free, a enabled channel can not be configed
	if(DmaRegs.CH_EN_REG.all & nChnMask)	return RT_FAIL;	// the channel is busy
	// 4. clear any pending interrupts
	DmaRegs.CLEAR_TFR.all = nChnMask;
	DmaRegs.CLEAR_BLOCK.all = nChnMask;
	DmaRegs.CLEAR_SRC_TRAN.all = nChnMask;
	DmaRegs.CLEAR_DST_TRAN.all = nChnMask;
	DmaRegs.CLEAR_ERR.all = nChnMask;
	// 5. Program the channel registers
	DmaRegs.CH[nChn].SAR = pSrcDev->nAddr;
	DmaRegs.CH[nChn].DAR = pDestDev->nAddr;
	DmaRegs.CH[nChn].LLP = 0;

	for(i=0;i<sizeof(gDmaPerAddrList)/4;i++)	// hardware handshaking
	{
		if(gDmaPerAddrList[i] == pSrcDev->nAddr)
		{
			nCfgH |= i<<DMA_SRC_PER;	
		}
		if(gDmaPerAddrList[i] == pDestDev->nAddr)
		{
			nCfgH |= i<<DMA_DEST_PER;	
		}
	}	
	nCfgH |= (0x1<<DMA_PROTCTL) | (0<<DMA_FIFO_MODE) | (0<<DMA_FCMODE);

	DmaRegs.CH[nChn].CFG_L.all = (nPrio<<DMA_CH_PRIOR) | (pSrcDev->mHandshaking<<DMA_HS_SEL_SRC) | (pDestDev->mHandshaking<<DMA_HS_SEL_DST);
	DmaRegs.CH[nChn].CFG_H.all = nCfgH; 

	if((((pSrcDev->nAddr>>28)==4) || ((pSrcDev->nAddr>>28)==5)) && (pSrcDev->mHandshaking != NOHANDSHAKE)) 	  //add nohandshaking mode ,2015.10.30
	{
		nCtlL |= 2<<(DMA_TT_FC);	// source peripheral
	}
	if((((pDestDev->nAddr>>28)==4) || ((pDestDev->nAddr>>28)==5)) && (pSrcDev->mHandshaking != NOHANDSHAKE)) 
	{
		nCtlL |= 1<<(DMA_TT_FC);	// destination peripheral
	}

	nCtlL |= (pSrcDev->mTranWidth<<DMA_SRC_TR_WIDTH) | (pDestDev->mTranWidth<<DMA_DST_TR_WIDTH) | \
			 (pSrcDev->mAddrMode<<DMA_SINC) | (pDestDev->mAddrMode<<DMA_DINC) | \
			 (pSrcDev->mBurstLen<<DMA_SRC_MSIZE) | (pDestDev->mBurstLen<<DMA_DEST_MSIZE);

	DmaRegs.CH[nChn].CTL_L.all = nCtlL;
	DmaRegs.CH[nChn].CTL_H.all = 0;

	// 6. interrupt setting
	if(nIntType) 
	{
		if(nIntType&DMA_INT_TFR) 	DmaRegs.MASK_TFR.all |= (1<<nChn) | (0x100<<nChn);
		if(nIntType&DMA_INT_BLOCK) 	DmaRegs.MASK_BLOCK.all |= (1<<nChn) | (0x100<<nChn);
		if(nIntType&DMA_INT_SRC_TRAN) DmaRegs.MASK_SRC_TRAN.all |= (1<<nChn) | (0x100<<nChn);
		if(nIntType&DMA_INT_DST_TRAN) DmaRegs.MASK_DST_TRAN.all |= (1<<nChn) | (0x100<<nChn);
		if(nIntType&DMA_INT_ERR) DmaRegs.MASK_ERR.all |= (1<<nChn) | (0x100<<nChn);
		DmaRegs.CH[nChn].CTL_L.all |= 1<<DMA_INT_EN;	// interrupt enabled
		fpDmaCHxIsrCb[nChn] = fp;
		NVIC_EnableIRQ(DMA_IRQn);	// interrupt enabled
	}
	else
	{
		DmaRegs.CH[nChn].CTL_L.all &= ~(1<<DMA_INT_EN);	// interrupt disable	
	}

 	return RT_OK;	
}

/***********************************************************************************
 * Subroutine:	DMA_Start
 * Function:	Start DMA transfer
 * Input:		UINT8  nChn -- DMA channel, values 0 ~ DMA_NUM_CHANNELS-1
 *				UINT32 nSrcAddr -- DMA source transfer address
 *				UINT32 nDstAddr -- DMA destination transfer address
 *				UINT32 nTranTimes -- DMA transfer times (1~4095)
 * Output:		None
 * Description:	Note that the total transfer bytes is the value that
 *				DMA transfer times mutiplied by the source transfer width.
 *				The maximum transfer is DMA_MAX_TRAN_SIZE.
 * Date:		2015/03/08
 * ModifyRecord:  
 * ********************************************************************************/
void DMA_Start(UINT8 nChn, UINT32 nSrcAddr,UINT32 nDstAddr, UINT32 nTranTimes)
{
	DmaRegs.CH[nChn].SAR = 	nSrcAddr;
	DmaRegs.CH[nChn].DAR = 	nDstAddr;
	DmaRegs.CH[nChn].CTL_H.bit.BlockTs = nTranTimes; 
	DmaRegs.CFG_REG.all = 0x1;
	DmaRegs.CH_EN_REG.all |= ((0x100 << nChn)|(1<<nChn));
}

/***********************************************************************************
 * Subroutine:	DMA_CheckDone
 * Function:	to check the specified DMA channel transfer complete
 * Input:		UINT8  nChn -- DMA channel, values 0 ~ DMA_NUM_CHANNELS-1
 *				UINT32 nTimeout -- to specified the timeout for wait DMA transfer complete.
 *								When the value is 0, don't return untill DMA transfer complete.
 *								When the value is not 0, wait for timeout or DMA transfer complete.
 * Output:		UINT8 -- 		RT_OVERTIME, RT_OK
 * Description:	
 * Date:		2015/03/08
 * ModifyRecord:
 * ********************************************************************************/
UINT8 DMA_CheckDone(UINT8 nChn, UINT32 nTimeout)
{
	UINT32 nTimeCnt = 0;
	while(!(DmaRegs.RAW_TFR.all & (1<<nChn)))
	{
		if(nTimeout)
		{
			if(nTimeCnt++ == nTimeout) 
			{
				DmaRegs.CH_EN_REG.all = (0x100 << nChn);	// disable DMA channel to terminal DMA transfer
				while(DmaRegs.CH_EN_REG.all&(1<<nChn)) 	// wait for DMA transfer terminal
				{
				}
				return RT_OVERTIME;
			}
		}	
	}
	
	DmaRegs.CLEAR_TFR.all = (1<<nChn);
	return RT_OK;		
}

/***********************************************************************************
 * Subroutine:	DMA_Handler
 * Function:	to dispatch DMA interrupt for each channel
 * Input:		None
 * Output:		None
 * Description:	
 * Date:		2015/11/03
 * ModifyRecord:
 * ********************************************************************************/
void DMA_Handler(void)
{
	UINT8 nChInt, nCh; 
	UINT32 nChMask;
	UINT32 nTfrInt = DmaRegs.STATUS_TFR.all;
	UINT32 nBlockInt = DmaRegs.STATUS_BLOCK.all;
	UINT32 nSrcTranInt = DmaRegs.STATUS_SRC_TRAN.all;
	UINT32 nDestTranInt = DmaRegs.STATUS_DST_TRAN.all;
	UINT32 nErrInt = DmaRegs.STSTUS_ERR.all;
	// clear interrupt
	DmaRegs.CLEAR_TFR.all = nTfrInt;
	DmaRegs.CLEAR_BLOCK.all = nBlockInt;
	DmaRegs.CLEAR_SRC_TRAN.all = nSrcTranInt;
	DmaRegs.CLEAR_DST_TRAN.all = nDestTranInt;
	DmaRegs.CLEAR_ERR.all = nErrInt;

	for(nCh=0;nCh<DMA_NUM_CHANNELS;nCh++)
	{
		nChMask = 1<<nCh;
		nChInt = (nTfrInt&nChMask) | ((nBlockInt&nChMask)<<1) | ((nSrcTranInt&nChMask)<<2) | \
					((nDestTranInt&nChMask)<<3) | ((nDestTranInt&nChMask)<<4);
		nChInt >>= nCh;
		if(nChInt && (fpDmaCHxIsrCb[nCh]!=NULL)) fpDmaCHxIsrCb[nCh](nChInt);	
	}
}





