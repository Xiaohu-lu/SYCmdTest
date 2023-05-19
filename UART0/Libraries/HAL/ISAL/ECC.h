/***************************************************************************************
* File name    :	ECC.h
* Function     :	The header of ECC.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _ECC_H
#define _ECC_H

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/


/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef enum 
{
	ECC_Success = 0,	
	ECC_OVERTIME_Err,
	ECC_POINT_Err,
	ECC_PARA_Err,
	ECC_VERIFY_Err,
}ECC_RT;

typedef struct 
{
	UINT32      nBits;		
    UINT8*		PrivateKey;
}ECC_PriKey;

typedef struct 
{
	UINT32      nBits;		
    UINT8*		X;
	UINT8*		Y;
}ECC_PubKey; 

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 InitCurveEcc(UINT32 nBits,Ep_Curve **EC);

UINT8 ECDSA_GenKey(ECC_PriKey *pPriKey,ECC_PubKey *pPubKey,UINT32 nBits,BOOL bPriKeyExist);

UINT8 ECDSA_Sign(ECC_PriKey* PriKey,UINT8 *pHash,UINT32 HashLen,UINT8 *pResult,UINT32 *pResultLen);

UINT8 ECDSA_Verify(ECC_PubKey* PubKey,UINT8 *pHash, UINT32 HashLen,UINT8 *pResult, UINT32 ResultLen);

UINT8 ECDSA_PubKeyEncrypt(ECC_PubKey* PubKey,UINT8 *Buf,UINT32 *pBufLen, UINT8 *pM, UINT32 nMLen);

UINT8 ECDSA_PriKeyDecrypt(ECC_PriKey* PriKey,UINT8   *Buf, UINT32  *pBufLen, UINT8   *pEM, UINT32  nEMLen);

UINT8 ECDSA_KeyExchange(ECC_PubKey* PubKey,ECC_PriKey* PriKey,UINT8   *pKey, UINT32  *pKeyLen);

void EccVersion(INT8 data[32]);


#endif
