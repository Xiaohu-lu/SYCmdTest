#include "operationkey.h"
#include "SYglobal.h"
#include <String.h>

/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
UINT8 SM4_KEY_A[16];		//SM4密钥对
UINT8 SM4_KEY_B[16];
UINT8 SM4_Q[16];			  //密文随机数
UINT8 SM4_M[16];				//握手信号、安全搜索响应
UINT8 SM4_R[16];				//随机数
UINT8 SM4_X[16];				//解密M的X

/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
UINT8 AES_KEY_A[16];		 //AES密钥A
UINT8 AES_KEY_B[16];		 //AES密钥B
UINT8 AES_Q[16];		 		//密文随机数
UINT8 AES_M[16];		 		//握手信号、安全搜索响应
UINT8 AES_R[16];		 		//随机数
UINT8 AES_X[16];	   		//解密M的X

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
UINT8 DES_KEY_A[16];		//DES密钥对
UINT8 DES_KEY_B[16];
UINT8 DES_Q[16];			  //密文随机数
UINT8 DES_M[16];				//握手信号、安全搜索响应
UINT8 DES_R[16];				//随机数
UINT8 DES_X[16];				//解密M的X

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
//以下所有变量，必须按四字节对齐 
ALIGN(4) UINT8 RSA_n[128], RSA_e[4];	//密钥对
ALIGN(4) UINT8 RSA_Q[128];		 //RSA私钥加密R后的Q,密文随机数
ALIGN(4) UINT8 RSA_M[128];		 //公钥解密Q的到128字节的后16字节为握手信号
ALIGN(4) UINT8 RSA_X[128];		 //公钥解密安全存储返回的响应Q的X
UINT8 RSA_R[16];							 //随机数

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
UINT8 ECC_KEY[32];	//私钥
UINT8 ECC_R[16];		//随机数R,密文随机数
UINT8 ECC_M[128];		//握手信号
UINT8 ECC_Q[128];		//安全搜索响应
UINT8 ECC_X[128];		//私钥解密Q得X


UINT8 Verify_T;			 //安全搜索返回数据包的确认码
UINT8 Verify_I;			 //安全搜索返回数据包的搜索ID号
UINT16 Verify_S;		 //安全搜索返回数据包的搜索得分

/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : SM4_Get_M
** Function    : 加密等级为2时,计算安全存储模板中的16字节握手信号M
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,密钥A解密Q得R',然后使用密钥B加密R'得到M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(SM4_R,0,sizeof(SM4_R));
	memset(SM4_M,0,sizeof(SM4_M));
	SMS4_Init(SM4_KEY_A);//初始化密钥A
	
	/*使用SM4_KEY_A解密Q获得R'
	*/
	if(SMS4_Run(SMS4_DECRYPT,SMS4_ECB,SM4_Q,SM4_R,16,NULL) != RT_OK)
	{
		nRet = 1;
	}

	SMS4_Init(SM4_KEY_B);//初始化密钥B
	/*使用SM4_KEY_B加密R'获得M
	*/
	if(SMS4_Run(SMS4_ENCRYPT,SMS4_ECB,SM4_R,SM4_M,16,NULL) != RT_OK)
	{
		nRet = 2;
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : SM4_Get_Q
** Function    : 加密等级为2时,计算安全验证指纹中的16字节参数Q
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,生成16字节随机数R,使用密钥A加密R得到Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//初始化真随机数
	if(nRet != 0)
	{
		return nRet;
	}
	nRet = Trng_GenRandom(SM4_R,16);//获取16字节真随机数
	if(nRet != 0)
	{
		return nRet;
	}
	
	SMS4_Init(SM4_KEY_A);//初始化密钥A
	/*使用SM4_KEY_A加密R获得Q
	*/
	if(SMS4_Run(SMS4_ENCRYPT,SMS4_ECB,SM4_R,SM4_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : SM4_Verify_P
** Function    : 加密等级为2时,校验安全搜索返回的数据包
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为2时,密钥B解密M得X,获取搜索的ID号和得分
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	SMS4_Init(SM4_KEY_B);
	/*使用KEY_B解密M获得X
	*/
	if(SMS4_Run(SMS4_DECRYPT,SMS4_ECB,SM4_M,SM4_X,16,NULL) != RT_OK)
	{
		nRet = 1;
		return nRet;
	}
	/*判断X的低11字节与R的低11字节是否相等
	*/
	for(i=5;i<16;i++)
	{
		if(SM4_X[i] != SM4_R[i])
		{
			return 1;
		}
	}
	
	/*T搜索结果
	 *I搜索ID
	 *S搜索得分
	 *R'模组使用密钥A解密Q得到R’
	 *P=T(1字节)|I(2字节)|S(2字节)|R'(11字节)
	 *X=T(1字节)|I(2字节)|S(2字节)|R(11字节)
	*/
	Verify_T = SM4_X[0];
	Verify_I = (SM4_X[1]<<8)&0xff00;
	Verify_I |= (SM4_X[2])&0xff;
	Verify_S = (SM4_X[3]<<8)&0xff00;
	Verify_S |= (SM4_X[4])&0xff;
	PRINTF_UART1("确认码:%x",Verify_T);
	PRINTF_UART1("搜索ID:%x %x",Verify_I/256,Verify_I%256);
	PRINTF_UART1("搜索得分:%x %x",Verify_S/256,Verify_S%256);
	return nRet;
}





/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : AES_Get_M
** Function    : 加密等级为3时,计算安全存储模板中的16字节握手信号M
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,密钥A解密Q得R',然后使用密钥B加密R'得到M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(AES_R,0,sizeof(AES_R));
	memset(AES_M,0,sizeof(AES_M));
	AES_Init(AES_KEY_A,16);//初始化密钥A
	
	/*使用KEY_A解密Q获得R
	*/
	if(AES_Run(AES_DECRYPT,AES128_ECB,AES_Q,AES_R,16,NULL) != RT_OK)
	{
		nRet = 1;
	}

	AES_Init(AES_KEY_B,16);//初始化密钥B
	/*使用KEY_B加密R1获得M
	*/
	if(AES_Run(AES_ENCRYPT,AES128_ECB,AES_R,AES_M,16,NULL) != RT_OK)
	{
		nRet = 2;
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : AES_Get_Q
** Function    : 加密等级为3时,计算安全验证指纹中的16字节参数Q
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,生成16字节随机数R,使用密钥A加密R得到Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//初始化真随机数
	if(nRet != 0)
	{
		return nRet;
	}
	nRet = Trng_GenRandom(AES_R,16);//获取16字节真随机数
	if(nRet != 0)
	{
		return nRet;
	}
	
	AES_Init(AES_KEY_A,16);//初始化密钥A
	/*使用KEY_A加密R获得Q
	*/
	if(AES_Run(AES_ENCRYPT,AES128_ECB,AES_R,AES_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : AES_Verify_P
** Function    : 加密等级为3时,校验安全搜索返回的数据包
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,密钥B解密M得X,获取搜索的ID号和得分
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	AES_Init(AES_KEY_B,16);
	/*使用KEY_B解密M获得X
	*/
	if(AES_Run(AES_DECRYPT,AES128_ECB,AES_M,AES_X,16,NULL) != RT_OK)
	{
		nRet = 1;
		return nRet;
	}
	/*判断X的低11字节与R的低11字节是否相等
	*/
	for(i=5;i<16;i++)
	{
		if(AES_X[i] != AES_R[i])
		{
			return 1;
		}
	}
	
	/*T搜索结果
	 *I搜索ID
	 *S搜索得分
	 *R'模组使用密钥A解密Q得到R’
	 *P=T(1字节)|I(2字节)|S(2字节)|R'(11字节)
	 *X=T(1字节)|I(2字节)|S(2字节)|R(11字节)
	*/
	Verify_T = AES_X[0];
	Verify_I = (AES_X[1]<<8)&0xff00;
	Verify_I |= (AES_X[2])&0xff;
	Verify_S = (AES_X[3]<<8)&0xff00;
	Verify_S |= (AES_X[4])&0xff;
	PRINTF_UART1("确认码:%x",Verify_T);
	PRINTF_UART1("搜索ID:%x %x",Verify_I/256,Verify_I%256);
	PRINTF_UART1("搜索得分:%x %x",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : DES_Get_M
** Function    : 加密等级为4时,计算安全存储模板中的16字节握手信号M
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,密钥A解密Q得R',然后使用密钥B加密R'得到M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(DES_R,0,sizeof(DES_R));
	memset(DES_M,0,sizeof(DES_M));
	DES_Init(DES_KEY_A,16);//初始化密钥A
	
	/*使用KEY_A解密Q获得R
	*/
	if(DES_Run(AES_DECRYPT,DES3_ECB,DES_Q,DES_R,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB 使用KEY_A解密Q获得R失败\r\n");
	}

	DES_Init(DES_KEY_B,16);//初始化密钥B
	/*使用KEY_B加密R1获得M
	*/
	if(DES_Run(DES_ENCRYPT,DES3_ECB,DES_R,DES_M,16,NULL) != RT_OK)
	{
		nRet = 2;
// 		printf("3DES 16bytes ECB 使用KEY_B加密R获得M失败\r\n");
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : DES_Get_Q
** Function    : 加密等级为4时,计算安全验证指纹中的16字节参数Q
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,生成16字节随机数R,使用密钥A加密R得到Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//初始化真随机数
	if(nRet != 0)
	{
// 		printf("真随机数初始化失败\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(DES_R,16);//获取16字节真随机数
	if(nRet != 0)
	{
// 		printf("产生随机数失败\r\n");
		return nRet;
	}
	
	DES_Init(DES_KEY_A,16);//初始化密钥A
	/*使用DES_KEY_A加密R获得Q
	*/
	if(DES_Run(DES_ENCRYPT,DES3_ECB,DES_R,DES_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB 使用KEY_A加密R获得Q失败\r\n");
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : DES_Verify_P
** Function    : 加密等级为3时,校验安全搜索返回的数据包
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,密钥B解密M得X,获取搜索的ID号和得分
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	DES_Init(DES_KEY_B,16);
	/*使用KEY_B解密M获得X
	*/
	if(DES_Run(DES_DECRYPT,DES3_ECB,DES_M,DES_X,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB 使用KEY_B解密M获得X失败\r\n");
		return nRet;
	}
	/*判断X的低11字节与R的低11字节是否相等
	*/
	for(i=5;i<16;i++)
	{
		if(DES_X[i] != DES_R[i])
		{
// 			printf("安全搜索解密失败\r\n");
			return 1;
		}
	}
	
	/*T搜索结果
	 *I搜索ID
	 *S搜索得分
	 *R'模组使用密钥A解密Q得到R’
	 *P=T(1字节)|I(2字节)|S(2字节)|R'(11字节)
	 *X=T(1字节)|I(2字节)|S(2字节)|R(11字节)
	*/
	Verify_T = DES_X[0];
	Verify_I = (DES_X[1]<<8)&0xff00;
	Verify_I |= (DES_X[2])&0xff;
	Verify_S = (DES_X[3]<<8)&0xff00;
	Verify_S |= (DES_X[4])&0xff;
	PRINTF_UART1("确认码:%x\r\n",Verify_T);
	PRINTF_UART1("搜索ID:%x %x\r\n",Verify_I/256,Verify_I%256);
	PRINTF_UART1("搜索得分:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : RSA_Get_M
** Function    : 加密等级为20时,计算安全存储模板中的16字节握手信号M
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为20时,使用公钥(n,e)解密RSA_Q,得R'放到Text_M中
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 RSA_Get_M(void)
{
	UINT8 nRet = 0;		
	UINT32 nOutDataLen,nInDataLen;
	R_RSA_PUBLIC_KEY  PublicKey;
	
	PublicKey.modulus = RSA_n;
	PublicKey.exponent = RSA_e;
	PublicKey.bits = 1024;
		
	nInDataLen = 128;
	nRet=RSA_PubKeyOpt(RSA_M,&nOutDataLen,RSA_Q,nInDataLen,&PublicKey);
	
	return nRet;
}

/***************************************************************************************************
** Subroutine  : RSA_Get_Q
** Function    : 加密等级为3时,计算安全验证指纹中的16字节参数Q
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为3时,生成16字节随机数R,直接发给模组 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 RSA_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//初始化真随机数
	if(nRet != 0)
	{
// 		printf("真随机数初始化失败\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(RSA_R,16);//获取16字节真随机数
	if(nRet != 0)
	{
// 		printf("产生随机数失败\r\n");
		return nRet;
	}
	
	//加密等级为20,把随机数R直接发送给模组
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : RSA_Verify_P
** Function    : 加密等级为20时,校验安全搜索返回的数据包
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为20,公钥解密M的X,获取搜索的ID号和得分
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 RSA_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	UINT32 nOutDataLen;
	R_RSA_PUBLIC_KEY  PublicKey;
	
	PublicKey.modulus = RSA_n;
	PublicKey.exponent = RSA_e;
	PublicKey.bits = 1024;

		
	memset(RSA_Q,0,sizeof(RSA_Q));
	
	nRet = RSA_PubKeyOpt(RSA_Q,&nOutDataLen,RSA_M,128,&PublicKey);
	if(nRet != 0)
	{
// 		printf("公钥解密RSA_Q失败\r\n");
		return nRet;
	}
// 	printf("公钥解密RSA_Q成功,RSA_M数据长度%d\r\n",nOutDataLen);
	for(i=0;i<16;i++)
		RSA_X[i] = RSA_Q[112+i];
		

	/*判断X的低11字节与R的低11字节是否相等
	*/
	for(i=5;i<16;i++)
	{
		if(RSA_X[i] != RSA_R[i])
		{
// 			printf("安全搜索解密失败\r\n");
			return 1;
		}
	}
	
	/*T搜索结果
	 *I搜索ID
	 *S搜索得分
	 *R'模组使用密钥A解密Q得到R’
	 *P=T(1字节)|I(2字节)|S(2字节)|R'(11字节)
	 *X=T(1字节)|I(2字节)|S(2字节)|R(11字节)
	*/
	Verify_T = RSA_X[0];
	Verify_I = (RSA_X[1]<<8)&0xff00;
	Verify_I |= (RSA_X[2])&0xff;
	Verify_S = (RSA_X[3]<<8)&0xff00;
	Verify_S |= (RSA_X[4])&0xff;
// 	printf("确认码:%x\r\n",Verify_T);
// 	printf("搜索ID:%x %x\r\n",Verify_I/256,Verify_I%256);
// 	printf("搜索得分:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : ECC_Get_M
** Function    : 加密等级为21时,计算安全存储模板中的16字节握手信号M
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为21时,主控使用私钥签名R得Q,
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 ECC_Get_M(void)
{
	UINT8 nRet = 0;		
	UINT32 nOutDataLen;
	ECC_PriKey ecPriKey;
	
	ecPriKey.PrivateKey = ECC_KEY;
	ecPriKey.nBits = 256;
	nRet = ECDSA_Sign(&ecPriKey, ECC_Q, 16, ECC_M,&nOutDataLen);
	return nRet;
}

/***************************************************************************************************
** Subroutine  : ECC_Get_Q
** Function    : 加密等级为21时,计算安全验证指纹中的16字节参数Q
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为21时,生成16字节随机数R,直接发给模组 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 ECC_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//初始化真随机数
	if(nRet != 0)
	{
// 		printf("真随机数初始化失败\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(ECC_R,16);//获取16字节真随机数
	if(nRet != 0)
	{
// 		printf("产生随机数失败\r\n");
		return nRet;
	}
	
	//加密等级为21,把随机数R直接发送给模组
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : ECC_Verify_P
** Function    : 加密等级为21时,校验安全搜索返回的数据包
** Input       :  
**                
** Output      : 执行成功返回0,失败返回1
** Description : 加密等级为21,私钥解密Q得到X,获取搜索的ID号和得分
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 ECC_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	UINT32 nOutDataLen;
	ECC_PriKey ecPriKey;
	
	ecPriKey.PrivateKey = (UINT8*)ECC_KEY;
	ecPriKey.nBits = 256;

		
	memset(ECC_Q,0,sizeof(ECC_Q));
	nRet = ECDSA_PriKeyDecrypt(&ecPriKey, ECC_Q, &nOutDataLen,ECC_M,128);
	if(nRet != 0)
	{
		return nRet;
// 		printf("%d \r\n",nRet);
// 		printf("私钥解密RSA_Q失败\r\n");
	}
// 	printf("私钥解密RSA_Q成功,RSA_M数据长度%d\r\n",nOutDataLen);
	for(i=0;i<16;i++)
		ECC_X[i] = ECC_Q[i];
		

	/*判断X的低11字节与R的低11字节是否相等
	*/
	for(i=5;i<16;i++)
	{
		if(ECC_X[i] != ECC_R[i])
		{
// 			printf("安全搜索解密失败\r\n");
			return 1;
		}
	}
	
	/*T搜索结果
	 *I搜索ID
	 *S搜索得分
	 *R'模组使用密钥A解密Q得到R’
	 *P=T(1字节)|I(2字节)|S(2字节)|R'(11字节)
	 *X=T(1字节)|I(2字节)|S(2字节)|R(11字节)
	*/
	Verify_T = ECC_X[0];
	Verify_I = (ECC_X[1]<<8)&0xff00;
	Verify_I |= (ECC_X[2])&0xff;
	Verify_S = (ECC_X[3]<<8)&0xff00;
	Verify_S |= (ECC_X[4])&0xff;
	PRINTF_UART1("确认码:%x\r\n",Verify_T);
	PRINTF_UART1("搜索ID:%x %x\r\n",Verify_I/256,Verify_I%256);
	PRINTF_UART1("搜索得分:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}



