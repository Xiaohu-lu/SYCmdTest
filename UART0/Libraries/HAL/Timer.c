/***************************************************************************************
* File name    :	Timer.c
* Function     :	The functions for Timer0/1/2
* Author       : 	Leixj
* Date         :	2012/06/26
* Version      :    v1.0
* Description  :    Timer0/1/2 Hardware Abstract Layer
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


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/


/**************************************************************************
* Functon
***************************************************************************/

/***************************************************************************
 * Subroutine:	TIMER_GetRegs
 * Function:	Get Timer register file base address 
 * Input:		UINT8 nTimerNum -- 	to specify the timer number, T0,T1,T2.
 * Output:		UINT32 --	Return Tx register file address;
 * Description:	Get timer register file base address pointer.
 * Date:		2012.06.26
 * ModifyRecord:
 * *************************************************************************/
static volatile struct TMR_REGS * TMR_GetRegs(UINT8 nTimerNum)
{
	switch(nTimerNum)
	{
		case T0:
			return &T0Regs;
		case T1:
			return &T1Regs;
		case T2:
			return &T2Regs;
		default:
			return NULL;  
	}
}

/***********************************************************************************
 * Subroutine:	TIMER_Init
 * Function:	set timer0/1/2 work mode
 * Input:		UINT8 nTimerNum --  timer number, T0,T1,T2
 * 				UINT32 nLimit -- 	limit of counter
 * 				UINT8 nClkSrc --    to specify the timer clock for counting
 * 				UINT8  nMode  --    configure timer work mode
 *                                  can be configured as TMR_CNT ,TMR_INT, TMR_WDT
 * Output:		UINT8 --			RT_OK or RT_FAIL
 * Description:
 * Date:		2012.06.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Init(UINT8 nTimerNum, UINT32 nLimit, UINT8 nClkSrc, UINT8 nMode)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

   	switch(nTimerNum)
	{
		case T0:
			ScmRegs.MODCLKCTRL |= 1<<ID_T0;
			ScmRegs.MODRSTCTRL |= 1<<ID_T0;
			break;
		case T1:
			ScmRegs.MODCLKCTRL |= 1<<ID_T1;
			ScmRegs.MODRSTCTRL |= 1<<ID_T1;
			break;
		case T2:
			ScmRegs.MODCLKCTRL |= 1<<ID_T2;
			ScmRegs.MODRSTCTRL |= 1<<ID_T2;
			break;
		default:
			return RT_FAIL;
	}

	pTimer->CONTROL.all = (CONTROL_WKEY | CONTROL_WUFLAG);
	pTimer->LIMIT = nLimit;
	pTimer->CONTROL.all = (CONTROL_WKEY | CONTROL_WUFLAG);
	pTimer->COUNT = 0x00000000;

	switch(nMode)
	{
		case TMR_CNT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL;
			break;
		case TMR_INT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL | (1<<CONTROL_IE);
			break;
		case TMR_WDT:
			pTimer->CONTROL.all = CONTROL_WKEY | (UINT32)nClkSrc<<CONTROL_CLKSEL | (1<<CONTROL_WDT);
			break;
		default :
			return RT_FAIL;
	}
	
	if(nClkSrc == TCLK_SRC_TMRX)		//2012.11.21
	{
		if(nTimerNum == T0)
			GpioRegs.GPAUSE1.bit.GPA8 = MUX_TMR;
		else if(nTimerNum == T1)
			GpioRegs.GPAUSE1.bit.GPA9 = MUX_TMR;
		else
			GpioRegs.GPAUSE1.bit.GPA10 = MUX_TMR;
	} 
	
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_Start
 * Function:	start timer to count
 * Input:		UINT8 nTimerNum --  	timer number, T0, T1, T2
 * Output:		UINT8 --			RT_OK or RT_FAIL
 * Description:
 * Date:		2012.06.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Start(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = CONTROL_WKEY | (1<<CONTROL_EN) |(pTimer->CONTROL.all); 
	
	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_Stop
 * Function:	stop timer counting
 * Input:		UINT8 nTimerNum --  	timer number, T0, T1, T2
 * Output:		UINT8 --			  RT_OK or RT_FAIL
 * Description:
 * Date:		2012.06.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_Stop(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = (CONTROL_WKEY|(pTimer->CONTROL.all) )& ~(UINT32)(1<<CONTROL_EN);	

	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_ClrCnt
 * Function:	clear counter
 * Input:		UINT8 nTimerNum -- 	timer number, T0, T1, T2
 * Output:		UINT8 --		RT_OK or RT_FAIL
 * Description:
 * Date:		2012.06.26			  
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_ClrCnt(UINT8 nTimerNum)
{	
	UINT32 RegControlVal;
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;
		
	RegControlVal = pTimer->CONTROL.all;
	pTimer->CONTROL.all = (CONTROL_WKEY | CONTROL_WUFLAG | RegControlVal)& ~(UINT32)(1<<CONTROL_EN);
	pTimer->COUNT = 0x00000000;
	pTimer->CONTROL.all = CONTROL_WKEY | RegControlVal;

	return RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_GetCnt
 * Function:	to get counter
 * Input:		UINT8 nTimerNum -- 	timer number, T0, T1, T2
 *				UINT32	*pCnt --	to return current value	of counter
 * Output:		UINT32 Count --		the value of timer
 * Description:
 * Date:		2012.06.26
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_GetCnt(UINT8 nTimerNum, UINT32 *pCnt)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	*pCnt = pTimer->COUNT;

	return	RT_OK;
}

/***********************************************************************************
 * Subroutine:	TIMER_ClrIntFlag
 * Function:	to clear timer interrupt flag
 * Input:		UINT8 nTimerNum -- 	timer number, T0, T1
 * Output:		UINT8 --		RT_OK or RT_FAIL
 * Description:
 * Date:		2012.10.27
 * ModifyRecord:
 * ********************************************************************************/
UINT8 TMR_ClrIntFlag(UINT8 nTimerNum)
{
	volatile struct TMR_REGS * pTimer;
	pTimer = TMR_GetRegs(nTimerNum);
	if(pTimer == NULL) return RT_FAIL;

	pTimer->CONTROL.all = CONTROL_WKEY | (pTimer->CONTROL.all) | (1<<CONTROL_IP);
	return	RT_OK;
}
