/***************************************************************************************
* File name    :	Timer.h
* Function     :	The header of timer.c
* Author       : 	Leixj
* Date         :	2012/06/26
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef TMR_H_
#define TMR_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/
// Define Timer0/1/2 Work Mode
#define TMR_CNT       	0x01		// mode count without interrupt
#define TMR_INT         0x02		// mode count with interrupt
#define TMR_WDT         0x03		// WDT

// Define Clock  Source
#define	TCLK_SRC_FCLK	0
#define	TCLK_SRC_LFROSC	1  	// Frequency of LFROSC/4 for counter
//#define	TCLK_SRC_HFROSC	2
#define	TCLK_SRC_TMRX	3

// Define Timer
#define T0		0
#define T1		1
#define T2		2

// define the function name for old firmware library revision
#define		TimerInit		TMR_Init
#define		StartTimer		TMR_Start
#define		StopTimer		TMR_Stop
#define		ClrTimerCnt		TMR_ClrCnt	


/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
//extern volatile struct TMR_REGS * TMR_GetRegs(UINT8 nTimerNum);
UINT8 TMR_Init(UINT8 nTimerNum, UINT32 nLimit, UINT8 nClkSrc, UINT8 nMode);
UINT8 TMR_Start(UINT8 nTimerNum);
UINT8 TMR_Stop(UINT8 nTimerNum);
UINT8 TMR_ClrCnt(UINT8 nTimerNum);
UINT8 TMR_GetCnt(UINT8 nTimerNum, UINT32 *pCnt);
UINT8 TMR_ClrIntFlag(UINT8 nTimerNum);


#endif /*TIMER_H_*/

