/***************************************************************************
* File name    :	Bvd.c
* Function     :	The functions for BVD
* Author       : 	Leixj
* Date         :	2012/07/09
* Version      :    v1.0
* Description  :    BVD Hardware Abstract Layer
* ModifyRecord :
****************************************************************************/
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
 * Subroutine:	Bvd_Init
 * Function:	BVD initialization
 * Input:		UINT8 bVinSel --    select the range of the analog input voltage 
 *									0: 3.0 - 4.2 V
 *									1: 2.1 - 3.3 V
 * Output:		NONE
 * Description:	BVD initialization. 
 * Date:		2012.07.09
 * ModifyRecord:
 * *************************************************************************/
void BVD_Init(UINT8 VinSel)
{
	UINT32 ClkDiv1 = SCM_GetSysClk()/32000/2;
	ScmRegs.MODCLKCTRL &= ~(UINT32)(1<<ID_BVD);
	ScmRegs.MODRSTCTRL &= ~(UINT32)(1<<ID_BVD);
	ScmRegs.MODCLKCTRL |= 1<<ID_BVD;
	ScmRegs.MODRSTCTRL |= 1<<ID_BVD;
        
	BvdRegs.CTRL.bit.RstAn = 0;	  										//复位ADC模拟电路

	BvdRegs.CTRL.all |= (ClkDiv1<<CTRL_CLKDIV)|(VinSel<<CTRL_VINSEL);	//配置ClkDiv、VinSel		 
	BvdRegs.CTRL.all |= (1<<CTRL_RSTAN)|(1<<CTRL_SOC); 					//模拟电路工作、开始转换
	SCM_AnModPowerEnable(AN_BVD);
}

/***************************************************************************
 * Subroutine:	BVD_ReadResult
 * Function:	to read ADC conversion result
 * Input:		
 * Output:		ADC_VALUE -- return the ADC conversion result
 * Description:
 * Date:		2012.07.09
 * ModifyRecord:
 * *************************************************************************/
UINT32 BVD_ReadResult(void)
{
    while(BvdRegs.STAT.bit.Eoclf == 0)
    {
		__NOP();
    }
    BvdRegs.STAT.bit.Eoclf = 1;
    while(BvdRegs.STAT.bit.Eoclf == 1)      //wait clear
    {
		__NOP();
    }
	return BvdRegs.STAT.bit.Result;
}

