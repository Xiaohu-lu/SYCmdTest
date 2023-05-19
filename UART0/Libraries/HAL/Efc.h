/***************************************************************************************
* File name    :	Efc.h
* Function     :	The header of Efc.c
* Author       : 	coins
* Date         :	2012/09/13
* Version      :    v1.0
* Description  :
* ModifyRecord :
*****************************************************************************************/
#ifndef EFC_H_
#define EFC_H_

/***************************************************************************
* Include Header Files
***************************************************************************/

/**************************************************************************
* Global Macro Definition
***************************************************************************/
#define 	EFC_ERASE_CMD() 	{*(UINT32 *)0x11005678 = 0x0000FEDC;}
#define 	EFC_PROG_CMD()		{*(UINT32 *)0x11001234 = 0x0000CDEF;}
#define		EFC_ERASE_CHK_CMD()	{*(UINT32 *)0x11009ABC = 0x0000ABCD;}
#define 	EFC_PROG_CHK_CMD()	{*(UINT32 *)0x1100DEF0 = 0x0000DCBA;}

#define		FLASH_BASE_ADDR		0x10000000
#define		FLASH_PAGE_SIZE		512
#define		FLASH_BUFF_LEN		8

//------ Define Check Code for Write/Erase Protect --------------------------
#define		AREA_EO_WE			0x11115678
#define		AREA_PROG_WE		0x11115678
#define		AREA_DATAPRO_WE		0x22225678
#define		AREA_KEY_WE			0x22225678
#define		AREA_PARA_WE		0x44445678
#define		AREA_OTP_WE			0x55555678
#define		AREA_CHIP_ERASE_WE	0x11115678
#define		AREA_PROT_DI		0x0

#define		AREA_KEY0_WE		0x01234567
#define		AREA_KEY1_WE		0x12345678
#define		AREA_KEY2_WE		0x23456789
#define		AREA_KEY3_WE		0x3456789A
//------ Define the Different Area Start Address ---------------------------
#define		ASA_PARA			0x00080000				// size 1*2 page, fixed
#define		ASA_OTP				0x00080400				// size 1*2 page, fixed
#define		ASA_PROG			0x00000000				// size 256*2 pages by default, alterable
#define		ASA_DATAPRO			0x00040000				// size (192)*2 pages by default, alterable
#define		ASA_EO				0x00070000				// size 48*2 pages by default, alterable
#define		ASA_KEY				0x0007C000				// size 16*2 pages by default, alterable
#define		AREA_EO_ST_ADD		(FLASH_BASE_ADDR+ASA_EO)		
#define		AREA_EO_END_ADD		(FLASH_BASE_ADDR+ASA_KEY)	
#define		AREA_EO_EN			0x0007BFF8						
#define		AREA_EO_EN_ADD		(FLASH_BASE_ADDR+AREA_EO_EN)	
#define		AREA_KEY_EN			0x0007BFFC						
#define		AREA_KEY_EN_ADD		(FLASH_BASE_ADDR+AREA_KEY_EN)	

//------ Define the para for TC0 and TC1
#define		TC0_6M		0x02020305
#define		TC0_12M		0x0505060A
#define		TC0_24M		0x0A0A0C14
#define		TC0_48M		0x14141828
#define		TC0_96M		0x28283050
#define		TC0_18M		0x0707090F
#define		TC0_36M		0x0E0E121E
#define		TC0_72M		0x1C1C243C
#define		TC0_144M	0x38384878
#define		TC0_1_5M	0x01010101
#define		TC0_3M		0x01010202

#define		TC1_6M		0x0605060B
#define		TC1_12M		0x0C0A0C16
#define		TC1_24M		0x1914192D
#define		TC1_48M		0x3228325A		
#define		TC1_96M		0x645064B4
#define		TC1_18M		0x120F1221
#define		TC1_36M		0x241E2442
#define		TC1_72M		0x483C4884
#define		TC1_144M	0x907890FF
#define		TC1_1_5M	0x02020203
#define		TC1_3M		0x03030305

#define		TC2_6M		0x00000001
#define		TC2_12M		0x00000001
#define		TC2_24M		0x00111112
#define		TC2_48M		0x00222223
#define		TC2_96M		0x00444446
#define		TC2_18M		0x00111111
#define		TC2_36M		0x00222223
#define		TC2_72M		0x00333335
#define		TC2_144M	0x00555559
#define		TC2_1_5M	0x00000000
#define		TC2_3M		0x00000000

#define		TC3_6M		0x00000100
#define		TC3_12M		0x00000110
#define		TC3_24M		0x00111220
#define		TC3_48M		0x00222330
#define		TC3_96M		0x00443770	
#define		TC3_18M		0x00111220
#define		TC3_36M		0x00222330
#define		TC3_72M		0x00333550
#define		TC3_144M	0x00555AA0
#define		TC3_1_5M	0x00000000
#define		TC3_3M		0x00000000

#define ERASE_MAX_TIMES	18
#define PROG_MAX_TIMES	4

// define the KEY area ID 
typedef	enum	{
	EFC_AREA_KEY_0		= 0x00,
	EFC_AREA_KEY_1		= 0x01,
	EFC_AREA_KEY_2		= 0x02,
	EFC_AREA_KEY_3		= 0x03,
}EFC_AREA_KEY_ID;

// define the function name for old firmware library revision
#define	Flash_TimConfig(TC0, TC1, TC2, TC3)	EFC_ConfigTiming(TC0, TC1, TC2, TC3)	
#define	Flash_Program		EFC_Program
#define	Flash_Erase			EFC_Erase
#define	Flash_WriteInfo		EFC_WriteInfo
#define	Flash_ReadInfo		EFC_ReadInfo
#define	ClrProgFlag			EFC_ClrProgFlag
#define	Flash_PageProg		EFC_PageProg		
#define	SetCodeSt			EFC_SetEoSt
/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
void EFC_ConfigTiming(UINT32 nTC0, UINT32 nTC1, UINT32 nTC2, UINT32 nTC3);
UINT8 EFC_Program(UINT32 *pData, UINT32 addr_align, UINT32 nLen);
UINT8 EFC_Erase(UINT32 addr_offset, UINT32 page_num);
void EFC_ChipErase(void);
UINT8 EFC_IsEraseOK(UINT32 addr_align, UINT32 nLen);
UINT8 EFC_PageProg(UINT32 addr_offset, UINT32 *pData);
UINT8 EFC_WriteInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_ReadInfo(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
void EFC_ClrProgFlag(UINT32 Key);
void EFC_ClrProgFlagOnly(UINT32 Key);
void EFC_ClrDESFlag(UINT32 Key);

UINT8 EFC_KEYWrite(EFC_AREA_KEY_ID nKeyAreaID, UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);
UINT8 EFC_SetEoSt(UINT32 nPage);
UINT8 EFC_SetKeySt(UINT32 nPage);
/**************************************************************************
* Functon
***************************************************************************/
/***************************************************************************
* Subroutine:	EFC_WrProtect
* Function:		Embedded flash write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
* 				0x1111_5678 -- 	program area writable and erasable;
* 				0x1111_5678 -- 	E0 area writable and erasable;
* 				0x2222_5678 -- 	protected data area writable and erasable;
* 				0x2222_5678 -- 	KEY area writable and erasable;
* 				0x4444_5678 -- 	Parameter area writable and erasable;
*				0x5555_5678 -- 	OTP area un-writable;
* Date:		2012.09.21
* ModifyRecord:
* *************************************************************************/
__STATIC_INLINE void EFC_WrProtect(UINT32 permission)
{
	EfcRegs.CHECK = permission;
}

/***************************************************************************
* Subroutine:	EFC_KEYWrProtect
* Function:		Embedded flash KEY area write and erase protect enable or disable with proper
* 				check code.
* Input:		UINT32 permission	-- 	the permission for an operation;
* Output:		None
* Description:	the permission value defined as following:
*				0x0123_4567 -- 	KEY AREA 0 area writable and erasable;
* 				0x1234_5678 -- 	KEY AREA 1 area writable and erasable;
* 				0x2345_6789 -- 	KEY AREA 2 area writable and erasable;
* 				0x3456_789A -- 	KEY AREA 3 area writable and erasable;
* Date:		2012.09.21
* ModifyRecord:
* *************************************************************************/
__STATIC_INLINE void EFC_KEYWrProtect(UINT32 permission)
{
	EfcRegs.KEY_CHK = permission;
}

///**************************************************************************
//* Subroutine:	EFC_DisEncrypt
//* Function:		to disable flash encrypt operation
//* Input:		None
//* Output:		None
//* Description:
//* Date:			2012.09.21
//* ModifyRecord:
//* *************************************************************************/
//__STATIC_INLINE void EFC_DisEncrypt(void)
//{
//	EfcRegs.CTRL.all &= ~(UINT32)EFC_CTRL_ENCRYPT;
//}
//
/**************************************************************************
* Subroutine:	EFC_ConfigCodeSz
* Function:		to config code area size
* Input:		UINT32 nPage --	to specify the page numbers for code area
* Output:		None
* Description:
* Date:			2012.09.21
* ModifyRecord:
* *************************************************************************/
__STATIC_INLINE void EFC_ConfigCodeSz(UINT32 nPage)
{
	EfcRegs.DATAPRO = FLASH_BASE_ADDR + nPage*FLASH_PAGE_SIZE;
}

#endif /* EFC_H_ */
