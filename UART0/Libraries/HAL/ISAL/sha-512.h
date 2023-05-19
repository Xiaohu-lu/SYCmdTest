/***************************************************************************************
* File name    :	SHA-512.h
* Function     :	The header of SHA-512.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA4_H
#define _SHA4_H 
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	SHA512_Success = 0, 
	SHA512_Err,
}SHA512_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct
{
    UINT64 total[2];
    UINT64 state[8];
    UINT8 buffer[128];
    INT32 is384;
}SHA4_CONTEXT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void SHA4_Init( SHA4_CONTEXT *ctx, INT32 is384 );

void SHA4_Update( SHA4_CONTEXT *ctx, UINT8 *input, INT32 ilen );

void HMAC_SHA384(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void HMAC_SHA512(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA4_Final( SHA4_CONTEXT *ctx, UINT8 output[64] );	 

void SHA4Version(INT8 data[32]); 
 
#endif

