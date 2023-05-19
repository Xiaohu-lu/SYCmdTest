/***************************************************************************************
* File name    :	MD5.h
* Function     :	MD5.c
* Author       : 	Hongjz
* Date         :	2015/09/15
* Version      :    v1.0
* Description  :    
* ModifyRecord :
*****************************************************************************************/
#ifndef _MD5_H_
#define _MD5_H_
/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
typedef enum 
{
	MD5_Success = 0, 
	MD5_Err,
}MD5_RT;
/**************************************************************************
* Global Type Definition
***************************************************************************/
typedef struct {
	UINT32 state[4];
	UINT32 count[2];
	UINT8 buffer[64];
}MD5_CONTEXT;

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/

void MD5_Init(MD5_CONTEXT *context);

void MD5_Update(MD5_CONTEXT *context, UINT8 *pData, UINT32 lDataLen);

void MD5_Final(MD5_CONTEXT *context,UINT8 Result[16]);
 
void HMAC_MD5(UINT8 *pText,UINT32 nTextLen,UINT8* pKey,UINT32 nKeyLen,UINT8 *pOut);

void MD5Version(INT8 data[32]); 
 
#endif

