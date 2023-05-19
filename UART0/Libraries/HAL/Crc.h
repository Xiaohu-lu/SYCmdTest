/***************************************************************************************
* File name    :	CRC.h
* Function     :	The header of CRC.c
* Author       : 	hong
* Date         :	2013/01/13
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef _CRC_H_
#define _CRC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define	CRC_CCITT	0
#define	CRC_16		1
#define CRC_LSB		0
#define CRC_MSB		1

/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void CRC_Init(UINT8 AlgSel, UINT8 InDataSeq, UINT8 OutDataSeq, UINT16 InitVal);
UINT16 CRC_Calulate(UINT8 *pData, UINT32 nLen);
#endif
