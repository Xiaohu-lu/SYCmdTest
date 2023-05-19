/**********************************************************************************
* File name    :	crc.c
* Function     :	The base function for CRC calculation
* Author       : 	hong
* Date         :	2013/01/13
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


/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Local static Variable Declaration
**********************************************************************************/


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
* Subroutine:	CRC_Init
* Function:		crc Initialize
* Input:		UINT8 AlgSel	-- 	the algorithm select, CRC_16 or CRC_CCITT;
* 				UINT8 InDataSeq --	the data input sequence, CRC_LSB or CRC_MSB;
* 				UINT8 OutDataSeq --	the data output sequence, CRC_LSB or CRC_MSB.
* 				UINT16 InitVal --	the CRC initial value.
* Output:		None;
* Description:	
* Date:			2013.10.17
* ModifyRecord:
* *************************************************************************/
void CRC_Init(UINT8 AlgSel, UINT8 InDataSeq, UINT8 OutDataSeq, UINT16 InitVal)
{
    SCM_DisableModClk(ID_CRC);
    SCM_KeepModRst(ID_CRC);
    SCM_EnableModClk(ID_CRC);
    SCM_ReleaseModRst(ID_CRC);

    CrcRegs.CTRL.all = ((AlgSel)|(InDataSeq<<1)|(OutDataSeq<<2))&0x07; 					
    CrcRegs.INITVAL = InitVal;
}
/***************************************************************************
* Subroutine:	CRC_Calulate
* Function:		Calculating the CRC data
* Input:		UINT8 *pData	-- 	The first address of the input data;
* 				UINT32 nLen --	data len;
* Output:		CRC result;
* Description:	
* Date:			2013.01.21
* ModifyRecord:
* *************************************************************************/
UINT16 CRC_Calulate(UINT8 *pData, UINT32 nLen)
{
    while(nLen--)
    {
		CrcRegs.DATA = *pData++;
    }

    return CrcRegs.RESULT;
}
