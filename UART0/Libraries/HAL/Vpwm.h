/***************************************************************************************
* File name    :	Vpwm.h
* Function     :	The header of Vpwm.c
* Author       : 	Hongjz
* Date         :	2013/01/26
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef VPWM_H_
#define VPWM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define VPWM_POLLING_MODE		0x11	//VPWM work in the query mode
#define VPWM_INTTERRUPT_MODE	0x22	//VPWM work in the interrupt mode

/**************************************************************************
* Global Type Definition
***************************************************************************/

typedef enum{
	VPWM_PLAY_IDLE = 0,		// Voice play finish
	VPWM_PLAY_BUSY = 1,		// Voice playing
}VPWM_PLAY_STATUS;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 VPWM_VoiceInit(UINT8 nBitPerSample, UINT16 nSamplesPerSec);
UINT8 VPWM_VoicePlay(UINT8 nWorkMode, const UINT8 *pVoiceDataBuf, UINT32 nSizeOfVoiceDataBuf, UINT32 nPositionPlayStart);
VPWM_PLAY_STATUS VPWM_GetPlayStatus(void);
UINT8 VPWM_SetVolume(UINT8 nLev);
UINT8 VPWM_PwmInit(void);
UINT8 VPWM_PwmWaveOutput(UINT16 nDuty, UINT16 nPeriod);
UINT8 VPWM_VoicePlayWithDMA(UINT8 nDmaCh, UINT32 nMemAddr, UINT8* pData, UINT32 nLen);

#endif

