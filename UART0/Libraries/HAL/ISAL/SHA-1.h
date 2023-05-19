/***************************************************************************************
* File name    :	SHA-1.h
* Function     :	The header of SHA-1.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _SHA1_H_
#define _SHA1_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	SHA1_Success = 0, 
	SHA1_Err,
}SHA1_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct 
{
    UINT32  h0,h1,h2,h3,h4;
    UINT32  nblocks;
    UINT8 buf[64];
    UINT32  count;
}SHA1_CONTEXT;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void SHA1_Init( SHA1_CONTEXT *hd );

void SHA1_Update( SHA1_CONTEXT *hd, UINT8 *inbuf, UINT32 inlen);

void SHA1_Final(SHA1_CONTEXT *hd,UINT8 output[20]);

void HMAC_SHA1(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void SHA1Version(INT8 data[32]);
 
#endif

