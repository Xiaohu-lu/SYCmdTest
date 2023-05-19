/***************************************************************************************
* File name    :	Otp.h
* Function     :	The header of Otp.c
* Author       : 	footman
* Date         :	2015/09/25
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef OTP_H_
#define OTP_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
// define the OTP user field ID and	user set field ID
typedef	enum	{
	EFC_OTP_USER_1		= 0x01,
	EFC_OTP_USER_2		= 0x02,
	EFC_OTP_USER_3		= 0x03,
	EFC_OTP_USER_4		= 0x04,
	EFC_OTP_USER_5		= 0x05,
	EFC_OTP_USER_6		= 0x06,
	EFC_OTP_USER_SET	= 0x07,
}EFC_OTP_USER_FIELD_ID;

// define the function name for old firmware library revision
#define	OTPWrite			EFC_OtpWrite
#define	OTPRead				EFC_OtpRead
#define	ReadChipSN			EFC_ReadChipSN
#define	JtagLock			EFC_LockJtag

/**************************************************************************
* Global Type Definition
***************************************************************************/

/**************************************************************************
* Global Variable Declaration
***************************************************************************/

/**************************************************************************
* Global Functon Declaration
***************************************************************************/
UINT8 EFC_OtpWrite(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32* pBuf);
UINT8 EFC_OtpDisWrite(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32* pBuf);
UINT8 EFC_OtpRead(EFC_OTP_USER_FIELD_ID nUserFieldID, UINT32 nAddress, UINT32 nNum, UINT32 *pBuf);
void EFC_ReadChipSN(UINT8 *pSN, UINT16* pLen);
UINT8 EFC_LockJtag(void);
UINT8 EFC_LockTest(void);

#endif
