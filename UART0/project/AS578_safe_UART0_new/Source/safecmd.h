#ifndef _SAFECMD_H
#define _SAFECMD_H
#include "SYglobal.h"

#define PKG_HEAD                0x01ef 

#define UARTX				UART0
#define UARTX_MAP		UART0_MAP_A16_A17
#define UARTX_IRQn	UART0_IRQn

// #define LED1_ON 		(GPIO_BitSet(GPIOB,14))
// #define LED1_OFF		(GPIO_BitClr(GPIOB,14))

#define LED1_OFF 		(GPIO_BitSet(GPIOA,1))
#define LED1_ON			(GPIO_BitClr(GPIOA,1))

#define COMM_INS				0					//只需要接收应答包
#define COMM_DATA_REC		1					//接收应答包和数据包


#define DO_OK				0					//已经接收完响应,可以解析响应
#define DO_WAIT			1					//在等待接收响应

#define RE_OK				0					//解析完响应，可以发送下一个指令包
#define RE_WAIT			1					//等待解析响应包完成

#define PS_GetImage						0x01		//验证获取图像
#define PS_GenChar						0x02		//提取特征
#define PS_RegModel						0x05		//合并模板
#define	PS_WriteReg						0x0e		//写系统寄存器
#define PS_ReadINFpage				0x16		//读取参数页
#define PS_GetEnrollImage			0x29		//注册获取图像
#define PS_HandShake		    	0x35		//握手指令
#define PS_GetKeyt						0xE0		//获取秘钥对			
#define PS_GetCiphertext			0xE2		//获取密文		
#define PS_SecurityStoreChar	0xE3		//安全存储模板
#define PS_SecuritySearch			0xE4		//安全搜索指纹

#define PS_WaitImage				0xFF		//等待发送获取图像
#define Verify_flag_ADDR		0
#define Verify_flag_SIZE		4

/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
#define SM4_KEY_A_ADDR					(Verify_flag_ADDR+Verify_flag_SIZE)
#define SM4_KEY_A_SIZE					16

#define SM4_KEY_B_ADDR					(SM4_KEY_A_ADDR+SM4_KEY_A_SIZE)
#define SM4_KEY_B_SIZE					16

/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
#define AES_KEY_A_ADDR					(Verify_flag_ADDR+Verify_flag_SIZE)
#define AES_KEY_A_SIZE					16

#define AES_KEY_B_ADDR					(AES_KEY_A_ADDR+AES_KEY_A_SIZE)
#define AES_KEY_B_SIZE					16

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
#define DES_KEY_A_ADDR					(Verify_flag_ADDR+Verify_flag_SIZE)
#define DES_KEY_A_SIZE					16

#define DES_KEY_B_ADDR					(DES_KEY_A_ADDR+DES_KEY_A_SIZE)
#define DES_KEY_B_SIZE					16

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
#define RSA_E_ADDR					(Verify_flag_ADDR+Verify_flag_SIZE)
#define RSA_E_SIZE					4

#define RSA_N_ADDR					(RSA_E_ADDR+RSA_E_SIZE)
#define RSA_N_SIZE					128

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
#define ECC_KEY_ADDR					(Verify_flag_ADDR+Verify_flag_SIZE)
#define ECC_KEY_SIZE					32








typedef struct _PKG_IN_APP
{
	UINT8 cDoStatus;			//是否接收完成,可以解析响应
	UINT8 cReStus;				//是否解析完响应,可以发送指令包
	
	UINT8 nCommand;				//要发送的指令码
	
	UINT8 cACK;						//接收到的确认码
	UINT8 *pata;					//接收到的应答包的应答参数存放地址,&Cmd_Recv[9]
	UINT16 cpataLen;			//应答包的参数长度
	
	UINT8 cDatStus;				//是否需要接收数据包
	UINT8 *pcDat;					//接收到数据包的存放地址,&Cmd_DataRecv[0]
	UINT32 dwDatLen;			//数据包的长度	
}PKG_IN_APP;


extern PKG_IN_APP psInApp;

extern UINT8 AutoE_I;
extern UINT8 Shake_One;
extern UINT16 RES_Timeout;

void LED_Config(void);
void LED1_Toggle(void);
void UartInit(UINT8 nBaudrate);
void ProtocolInit(void);
void GenCheekSum(UINT8 *cmd,UINT8 Len);
void SendCmd(void);

void Get_Response(void);
void DoCommPkg(void);

void Make_PackH(void);
void Cmd_GetImage(void);
void Cmd_GenChar(UINT8 BufferID);
void Cmd_RegModel(void);
void Cmd_WriteReg(void);
void Cmd_ReadINFpage(void);
void Cmd_GetEnrollImage(void);
void Cmd_GetKeyt(void);
void Cmd_GetCiphertext(void);
void Cmd_SecurityStoreChar(void);
void Cmd_SecuritySearch(void);
void Cmd_HandShake(void);

UINT8 Res_HandShake(void);
UINT8 Res_ReadINFpage(void);
UINT8 Res_WriteReg(void);
UINT8 Res_GetKeyt(void);
UINT8 Res_GetEnrollImage(void);
UINT8 Res_GenChar(void);
UINT8 Res_RegModel(void);
UINT8 Res_GetCiphertext(void);
UINT8 Res_SecurityStoreChar(void);
UINT8 Res_GetImage(void);
UINT8 Res_SecuritySearch(void);


void R_KeyFormFlash(void);
void SM4_RKFormflash(void);
void AES_RKFormflash(void);
void DES_RKFormflash(void);
void RSA_RKFormflash(void);
void ECC_RKFormflash(void);
void SM4_WriteKey(void);
void AES_WriteKey(void);
void DES_WriteKey(void);
void RSA_WriteKey(void);
void ECC_WriteKey(void);
void SM4_ReadQ(void);
void AES_ReadQ(void);
void DES_ReadQ(void);
void RSA_ReadQ(void);
void ECC_ReadQ(void);
void SM4_GetRand(void);
void AES_GetRand(void);
void DES_GetRand(void);
void RSA_GetRand(void);
void ECC_GetRand(void);
void SM4_ReadM(void);
void AES_ReadM(void);
void DES_ReadM(void);
void RSA_ReadM(void);
void ECC_ReadM(void);


#endif


