/**********************************************************************************
* File name    :	Vpwm.c
* Function     :	The base function for Voice PWM
* Author       : 	hong
* Date         :	2013/01/13
* Version      :    v1.0
* Description  :	
* ModifyRecord :
**********************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include"SY09.h"

/**********************************************************************************
* Local Macro Definition
**********************************************************************************/
#define	VPWM_OUTPUT_BITS	8		// define the VPWM output bit width for voice

/**********************************************************************************
* Local Type Definition
**********************************************************************************/

/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/

static 	const UINT8 *gpVoiceDataBuf;		// the voice data buffer for current play
static	UINT32 gnSizeOfVoiceDataBuf;  	// the size of the voice data buffer for current play
static	UINT32 gnCurrPosition;		// the position of current play

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
* Subroutine:	VPWM_VoiceInit
* Function:		VPWM module voice Initialization, the voice volume is fixed level 2
* Input:		UINT8 nBitPerSample -- the voice data bit width, 8 or 16 bit.
* 				UINT16 nSamplesPerSec -- the voice data sample rate, such as 11025.
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_VoiceInit(UINT8 nBitPerSample, UINT16 nSamplesPerSec)
{
	if((nBitPerSample!=8) && (nBitPerSample!=16)) return RT_FAIL;
	if(nSamplesPerSec>(SCM_GetSysClk()/(1<<VPWM_OUTPUT_BITS)/4))  return RT_FAIL;
	SCM_DisableModClk(ID_VPWM);
	SCM_KeepModRst(ID_VPWM);
    SCM_EnableModClk(ID_VPWM);
	SCM_ReleaseModRst(ID_VPWM);
		
	GpioRegs.GPAUSE3.bit.GPA26 = MUX_VPWM;
	VpwmRegs.TIMER.bit.FsPeriod = (SCM_GetSysClk()/nSamplesPerSec);  // system cycles for a sample
	if(nBitPerSample == 8)
	{
		VpwmRegs.CFG.all = 0x18A;	// Voice functon, right shift 8 bit, 8-bit width WAV data input	, FIFO used
	}
	else
	{
		VpwmRegs.CFG.all = 0x18E;	// Voice functon, right shift 8 bit, 16-bit width WAV data input, FIFO used
	}
	VpwmRegs.PR.bit.Period = 1<<VPWM_OUTPUT_BITS;	// output PWM period is fixed 
	VpwmRegs.CFG.bit.ShiftBits = 16-VPWM_OUTPUT_BITS+4-2;
	return RT_OK;
}

/***************************************************************************
* Subroutine:	VPWM_VoicePlay
* Function:		Voice play with VPWM output
* Input:		UINT8 nWorkMode	-- 		Poll or Interrupt, VPWM_POLLING_MODE or VPWM_INTTERRUPT_MODE;
* 				UINT8 *pVoiceDataBuf --	the voice data buffer.
* 				UINT32 nSizeOfVoiceDataBuf -- 	the size of the voice data buffer, unit in byte.
* 				UINT32 nPositionPlayStart -- 	the position of the voice data buffer to start playing.
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_VoicePlay(UINT8 nWorkMode, const UINT8 *pVoiceDataBuf, UINT32 nSizeOfVoiceDataBuf, UINT32 nPositionPlayStart)
{
	if(VpwmRegs.CFG.bit.Sample_b) 	// if the voice data is 16 bit width, the address and size must even
	{
		if((UINT32)pVoiceDataBuf&0x1) return RT_FAIL;
		if(nSizeOfVoiceDataBuf&0x1) return RT_FAIL;
		if(nPositionPlayStart&0x1) return RT_FAIL;
	}
	gpVoiceDataBuf = pVoiceDataBuf;
	gnSizeOfVoiceDataBuf = nSizeOfVoiceDataBuf;
	gnCurrPosition = nPositionPlayStart;

	if(VPWM_POLLING_MODE == nWorkMode)
	{
		VpwmRegs.CFG.bit.En = 1;
		while(gnCurrPosition<gnSizeOfVoiceDataBuf)
		{
			if(VpwmRegs.CFG.bit.Sample_b)	// 16-bit-width data
			{				
				VpwmRegs.FIFO.all = ((UINT16 *)gpVoiceDataBuf)[gnCurrPosition/2];
				gnCurrPosition+=2;
			}
			else 
			{
				VpwmRegs.FIFO.all = gpVoiceDataBuf[gnCurrPosition];
				gnCurrPosition+=1;
			}
			while(VpwmRegs.CFG.bit.Full)
			{
				__NOP();
			}
		}
		VpwmRegs.CFG.bit.En = 0;  	// close VPWM output
	}
	else
	{
		VpwmRegs.INTCTRL.bit.NoFullEn = 1;	// enable not full interrupt
		VpwmRegs.CFG.bit.En = 1;
		NVIC_EnableIRQ(VPWM_IRQn);	
		__enable_irq();			
	}
	return RT_OK;
}

/***************************************************************************
* Subroutine:	VPWM_GetPlayStatus
* Function:		Playing status query used to interrupt mode
* Input:		None
* Output:		play status, VPWM_PLAY_BUSY or VPWM_PLAY_IDLE;
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
VPWM_PLAY_STATUS VPWM_GetPlayStatus(void)
{
	if(VpwmRegs.CFG.bit.En) return VPWM_PLAY_BUSY;
	else return VPWM_PLAY_IDLE;
}

/***************************************************************************
* Subroutine:	VPWM_SetVolume
* Function:		to set voice volume level
* Input:		UINT8 nLev --  the volume level for the voice (0 ~ 4)
* Output:		RT_OK or RT_FAIL
* Description:	the level is lower, the volume is little
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_SetVolume(UINT8 nLev)
{
	if(nLev>=5) return RT_FAIL;
	VpwmRegs.CFG.bit.ShiftBits = 16-VPWM_OUTPUT_BITS+4-nLev;
	return RT_OK;
}

/****************************************************************************
 * Subroutine:	VPWM_Handler
 * Function:	to handle the VPWM interrupt for voice output
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.16
 * ModifyRecord:
 * *************************************************************************/
void VPWM_Handler(void)
{
	VpwmRegs.INTCTRL.all = VpwmRegs.INTCTRL.all;	// clear all interrupt flag

	if(gnCurrPosition>=gnSizeOfVoiceDataBuf) 	// not data to fill
	{
		if(VpwmRegs.CFG.bit.Empty) 	// FIFO is empty
		{
			VpwmRegs.INTCTRL.bit.EmptyEn = 0;	// close empty interrupt
			VpwmRegs.CFG.bit.En = 0;	// close VPWM output
		}
		else
		{
			VpwmRegs.INTCTRL.bit.NoFullEn = 0;	// disable not full interrupt
			VpwmRegs.INTCTRL.bit.EmptyEn = 1; 	// enable empty interrupt
		}
	}
	else
	{
		while((!VpwmRegs.CFG.bit.Full) && ((gnCurrPosition<gnSizeOfVoiceDataBuf)))
		{
			if(VpwmRegs.CFG.bit.Sample_b)	// 16-bit-width data
			{				
				VpwmRegs.FIFO.all = ((UINT16 *)gpVoiceDataBuf)[gnCurrPosition/2];
				gnCurrPosition+=2;
			}
			else 
			{
				VpwmRegs.FIFO.all = gpVoiceDataBuf[gnCurrPosition];
				gnCurrPosition+=1;
			}	
		}
	}
}

//------------------------------------------------------------------------------
// the following function is for normal	PWM
//-----------------------------------------------------------------------------
/***************************************************************************
* Subroutine:	VPWM_PwmInit
* Function:		VPWM initialization for output fixed duty and period wave 
* Input:		None
* Output:		RT_OK 
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_PwmInit(void)
{
	SCM_DisableModClk(ID_VPWM);
	SCM_KeepModRst(ID_VPWM);
    SCM_EnableModClk(ID_VPWM);
	SCM_ReleaseModRst(ID_VPWM);	
	return RT_OK;	
}

/***************************************************************************
* Subroutine:	VPWM_PwmWaveOutput
* Function:		VPWM output fixed duty and period wave
* Input:		UINT16 nDuty -- PWM wave duty.
* 				UINT16 nPeriod -- PWM wave period.
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2013.10.16
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_PwmWaveOutput(UINT16 nDuty, UINT16 nPeriod)
{
	if(nDuty>nPeriod) return RT_FAIL;
	VpwmRegs.PR.bit.Period = nPeriod;
	VpwmRegs.DR.bit.Duty = nDuty;
	GpioRegs.GPAUSE3.bit.GPA26 = MUX_VPWM;
	VpwmRegs.CFG.bit.En = 1;
	return RT_OK;
}

/***************************************************************************
* Subroutine:	VPWM_VoicePlayWithDMA
* Function:		Voice play with VPWM output with DMA
* Input:		UINT8 nDmaCh -- DMA channel , 0~3
*				UINT32 nMemAddr -- the memory address for data
* 				UINT8 *pData --	the voice data buffer.
* 				UINT32 nLen -- 	the size of the voice data buffer, unit in byte.
* Output:		RT_OK or RT_FAIL
* Description:	
* Date:			2015.10.22
* ModifyRecord:
* *************************************************************************/
UINT8 VPWM_VoicePlayWithDMA(UINT8 nDmaCh, UINT32 nMemAddr, UINT8* pData, UINT32 nLen)
{
	UINT32 nDmaTs;
	UINT8 nRet;

	STRUCT_DMA_DEV sSrcDev = {0, INC, BYTE_1, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDev = {0, FIX, BYTE_1, BURST_1, HARD};
	
	memcpy((UINT8 *)nMemAddr, pData, nLen);
	sSrcDev.nAddr = (UINT32)nMemAddr;
	sDestDev.nAddr = (UINT32)(&(VpwmRegs.FIFO.all));
	DMA_Init(nDmaCh, nDmaCh, &sSrcDev, &sDestDev, 0, NULL);
	VpwmRegs.CFG.bit.En = 1;
	while(nLen)
	{
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;
		DMA_Start(nDmaCh, sSrcDev.nAddr, sDestDev.nAddr, nDmaTs);
		nRet = DMA_CheckDone(nDmaCh, 10000000); 
		if(nRet!=RT_OK) return nRet;
		sSrcDev.nAddr += nDmaTs;
		nLen -= nDmaTs;
	}
	return RT_OK;
}
