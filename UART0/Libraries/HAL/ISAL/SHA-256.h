/***************************************************************************************
* File name    :	SHA-256.h
* Function     :	The header of SHA-256.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA256_H_
#define _SHA256_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	SHA256_Success = 0, 
	SHA256_Err,
}SHA256_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct
{
    UINT32 total[2];
    UINT32 state[8];
    UINT8 buffer[64];
}
SHA256_CONTEXT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void SHA256_Init( SHA256_CONTEXT *ctx);

void SHA256_Update( SHA256_CONTEXT *ctx, UINT8 *input, INT32 ilen );

void SHA256_Final( SHA256_CONTEXT *ctx, UINT8 output[32] );

void HMAC_SHA256(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA256Version(INT8 data[32]);  
 
#endif

