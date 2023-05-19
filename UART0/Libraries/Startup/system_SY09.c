/****************************************************************************
* File name    :	system_SY09.c
* Function     :	the system initialization functions
* Author       : 	coins
* Date         :	2014/06/25
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
#include "SY09.h"

//#define	ENABLE_SRAM_ENC	 	//if SRAM encrypt function is used, please open ENABLE_SRAM_ENC	
/*----------------------------------------------------------------------------
  Clock Variable definitions
 *----------------------------------------------------------------------------*/
uint32_t SystemFrequency = F_HFROSC/2;
extern uint32_t __Vectors;

UINT32 SYS_GenSramKey(void)
{
	UINT32 rand = 0;
#ifdef	ENABLE_SRAM_ENC
	Trng_Init(0);
	while(rand==0)
		Trng_GenRandom((UINT8 *)&rand, 4);
#endif
	return rand;			
}
/****************************************************************************
 * Subroutine:	SystemInit
 * Function:	to initialize the system according to the core/board
 * Input:		None
 * Output:		None
 * Description:	to initialize the system clock, power, reset, memory and so on 
 * Date:		2012.06.15
 * ModifyRecord:
 * *************************************************************************/
void SystemInit (void)
{
	SCB->VTOR = (uint32_t)&__Vectors;
	SCM_LoadHFRoscTrim();
	SCM_EnableRst(RST_VDD18BODSEL);	
  	return;
}



