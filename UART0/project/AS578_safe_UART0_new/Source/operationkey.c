#include "operationkey.h"
#include "SYglobal.h"
#include <String.h>

/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
UINT8 SM4_KEY_A[16];		//SM4��Կ��
UINT8 SM4_KEY_B[16];
UINT8 SM4_Q[16];			  //���������
UINT8 SM4_M[16];				//�����źš���ȫ������Ӧ
UINT8 SM4_R[16];				//�����
UINT8 SM4_X[16];				//����M��X

/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
UINT8 AES_KEY_A[16];		 //AES��ԿA
UINT8 AES_KEY_B[16];		 //AES��ԿB
UINT8 AES_Q[16];		 		//���������
UINT8 AES_M[16];		 		//�����źš���ȫ������Ӧ
UINT8 AES_R[16];		 		//�����
UINT8 AES_X[16];	   		//����M��X

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
UINT8 DES_KEY_A[16];		//DES��Կ��
UINT8 DES_KEY_B[16];
UINT8 DES_Q[16];			  //���������
UINT8 DES_M[16];				//�����źš���ȫ������Ӧ
UINT8 DES_R[16];				//�����
UINT8 DES_X[16];				//����M��X

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
//�������б��������밴���ֽڶ��� 
ALIGN(4) UINT8 RSA_n[128], RSA_e[4];	//��Կ��
ALIGN(4) UINT8 RSA_Q[128];		 //RSA˽Կ����R���Q,���������
ALIGN(4) UINT8 RSA_M[128];		 //��Կ����Q�ĵ�128�ֽڵĺ�16�ֽ�Ϊ�����ź�
ALIGN(4) UINT8 RSA_X[128];		 //��Կ���ܰ�ȫ�洢���ص���ӦQ��X
UINT8 RSA_R[16];							 //�����

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
UINT8 ECC_KEY[32];	//˽Կ
UINT8 ECC_R[16];		//�����R,���������
UINT8 ECC_M[128];		//�����ź�
UINT8 ECC_Q[128];		//��ȫ������Ӧ
UINT8 ECC_X[128];		//˽Կ����Q��X


UINT8 Verify_T;			 //��ȫ�����������ݰ���ȷ����
UINT8 Verify_I;			 //��ȫ�����������ݰ�������ID��
UINT16 Verify_S;		 //��ȫ�����������ݰ��������÷�

/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : SM4_Get_M
** Function    : ���ܵȼ�Ϊ2ʱ,���㰲ȫ�洢ģ���е�16�ֽ������ź�M
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,��ԿA����Q��R',Ȼ��ʹ����ԿB����R'�õ�M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(SM4_R,0,sizeof(SM4_R));
	memset(SM4_M,0,sizeof(SM4_M));
	SMS4_Init(SM4_KEY_A);//��ʼ����ԿA
	
	/*ʹ��SM4_KEY_A����Q���R'
	*/
	if(SMS4_Run(SMS4_DECRYPT,SMS4_ECB,SM4_Q,SM4_R,16,NULL) != RT_OK)
	{
		nRet = 1;
	}

	SMS4_Init(SM4_KEY_B);//��ʼ����ԿB
	/*ʹ��SM4_KEY_B����R'���M
	*/
	if(SMS4_Run(SMS4_ENCRYPT,SMS4_ECB,SM4_R,SM4_M,16,NULL) != RT_OK)
	{
		nRet = 2;
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : SM4_Get_Q
** Function    : ���ܵȼ�Ϊ2ʱ,���㰲ȫ��ָ֤���е�16�ֽڲ���Q
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,����16�ֽ������R,ʹ����ԿA����R�õ�Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//��ʼ���������
	if(nRet != 0)
	{
		return nRet;
	}
	nRet = Trng_GenRandom(SM4_R,16);//��ȡ16�ֽ��������
	if(nRet != 0)
	{
		return nRet;
	}
	
	SMS4_Init(SM4_KEY_A);//��ʼ����ԿA
	/*ʹ��SM4_KEY_A����R���Q
	*/
	if(SMS4_Run(SMS4_ENCRYPT,SMS4_ECB,SM4_R,SM4_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : SM4_Verify_P
** Function    : ���ܵȼ�Ϊ2ʱ,У�鰲ȫ�������ص����ݰ�
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ2ʱ,��ԿB����M��X,��ȡ������ID�ź͵÷�
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 SM4_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	SMS4_Init(SM4_KEY_B);
	/*ʹ��KEY_B����M���X
	*/
	if(SMS4_Run(SMS4_DECRYPT,SMS4_ECB,SM4_M,SM4_X,16,NULL) != RT_OK)
	{
		nRet = 1;
		return nRet;
	}
	/*�ж�X�ĵ�11�ֽ���R�ĵ�11�ֽ��Ƿ����
	*/
	for(i=5;i<16;i++)
	{
		if(SM4_X[i] != SM4_R[i])
		{
			return 1;
		}
	}
	
	/*T�������
	 *I����ID
	 *S�����÷�
	 *R'ģ��ʹ����ԿA����Q�õ�R��
	 *P=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R'(11�ֽ�)
	 *X=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R(11�ֽ�)
	*/
	Verify_T = SM4_X[0];
	Verify_I = (SM4_X[1]<<8)&0xff00;
	Verify_I |= (SM4_X[2])&0xff;
	Verify_S = (SM4_X[3]<<8)&0xff00;
	Verify_S |= (SM4_X[4])&0xff;
	PRINTF_UART1("ȷ����:%x",Verify_T);
	PRINTF_UART1("����ID:%x %x",Verify_I/256,Verify_I%256);
	PRINTF_UART1("�����÷�:%x %x",Verify_S/256,Verify_S%256);
	return nRet;
}





/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : AES_Get_M
** Function    : ���ܵȼ�Ϊ3ʱ,���㰲ȫ�洢ģ���е�16�ֽ������ź�M
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,��ԿA����Q��R',Ȼ��ʹ����ԿB����R'�õ�M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(AES_R,0,sizeof(AES_R));
	memset(AES_M,0,sizeof(AES_M));
	AES_Init(AES_KEY_A,16);//��ʼ����ԿA
	
	/*ʹ��KEY_A����Q���R
	*/
	if(AES_Run(AES_DECRYPT,AES128_ECB,AES_Q,AES_R,16,NULL) != RT_OK)
	{
		nRet = 1;
	}

	AES_Init(AES_KEY_B,16);//��ʼ����ԿB
	/*ʹ��KEY_B����R1���M
	*/
	if(AES_Run(AES_ENCRYPT,AES128_ECB,AES_R,AES_M,16,NULL) != RT_OK)
	{
		nRet = 2;
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : AES_Get_Q
** Function    : ���ܵȼ�Ϊ3ʱ,���㰲ȫ��ָ֤���е�16�ֽڲ���Q
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,����16�ֽ������R,ʹ����ԿA����R�õ�Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//��ʼ���������
	if(nRet != 0)
	{
		return nRet;
	}
	nRet = Trng_GenRandom(AES_R,16);//��ȡ16�ֽ��������
	if(nRet != 0)
	{
		return nRet;
	}
	
	AES_Init(AES_KEY_A,16);//��ʼ����ԿA
	/*ʹ��KEY_A����R���Q
	*/
	if(AES_Run(AES_ENCRYPT,AES128_ECB,AES_R,AES_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : AES_Verify_P
** Function    : ���ܵȼ�Ϊ3ʱ,У�鰲ȫ�������ص����ݰ�
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,��ԿB����M��X,��ȡ������ID�ź͵÷�
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 AES_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	AES_Init(AES_KEY_B,16);
	/*ʹ��KEY_B����M���X
	*/
	if(AES_Run(AES_DECRYPT,AES128_ECB,AES_M,AES_X,16,NULL) != RT_OK)
	{
		nRet = 1;
		return nRet;
	}
	/*�ж�X�ĵ�11�ֽ���R�ĵ�11�ֽ��Ƿ����
	*/
	for(i=5;i<16;i++)
	{
		if(AES_X[i] != AES_R[i])
		{
			return 1;
		}
	}
	
	/*T�������
	 *I����ID
	 *S�����÷�
	 *R'ģ��ʹ����ԿA����Q�õ�R��
	 *P=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R'(11�ֽ�)
	 *X=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R(11�ֽ�)
	*/
	Verify_T = AES_X[0];
	Verify_I = (AES_X[1]<<8)&0xff00;
	Verify_I |= (AES_X[2])&0xff;
	Verify_S = (AES_X[3]<<8)&0xff00;
	Verify_S |= (AES_X[4])&0xff;
	PRINTF_UART1("ȷ����:%x",Verify_T);
	PRINTF_UART1("����ID:%x %x",Verify_I/256,Verify_I%256);
	PRINTF_UART1("�����÷�:%x %x",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : DES_Get_M
** Function    : ���ܵȼ�Ϊ4ʱ,���㰲ȫ�洢ģ���е�16�ֽ������ź�M
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,��ԿA����Q��R',Ȼ��ʹ����ԿB����R'�õ�M
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Get_M(void)
{
	UINT8 nRet = 0;
	
	memset(DES_R,0,sizeof(DES_R));
	memset(DES_M,0,sizeof(DES_M));
	DES_Init(DES_KEY_A,16);//��ʼ����ԿA
	
	/*ʹ��KEY_A����Q���R
	*/
	if(DES_Run(AES_DECRYPT,DES3_ECB,DES_Q,DES_R,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB ʹ��KEY_A����Q���Rʧ��\r\n");
	}

	DES_Init(DES_KEY_B,16);//��ʼ����ԿB
	/*ʹ��KEY_B����R1���M
	*/
	if(DES_Run(DES_ENCRYPT,DES3_ECB,DES_R,DES_M,16,NULL) != RT_OK)
	{
		nRet = 2;
// 		printf("3DES 16bytes ECB ʹ��KEY_B����R���Mʧ��\r\n");
	}
		
	return nRet;
}

/***************************************************************************************************
** Subroutine  : DES_Get_Q
** Function    : ���ܵȼ�Ϊ4ʱ,���㰲ȫ��ָ֤���е�16�ֽڲ���Q
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,����16�ֽ������R,ʹ����ԿA����R�õ�Q 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//��ʼ���������
	if(nRet != 0)
	{
// 		printf("���������ʼ��ʧ��\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(DES_R,16);//��ȡ16�ֽ��������
	if(nRet != 0)
	{
// 		printf("���������ʧ��\r\n");
		return nRet;
	}
	
	DES_Init(DES_KEY_A,16);//��ʼ����ԿA
	/*ʹ��DES_KEY_A����R���Q
	*/
	if(DES_Run(DES_ENCRYPT,DES3_ECB,DES_R,DES_Q,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB ʹ��KEY_A����R���Qʧ��\r\n");
	}
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : DES_Verify_P
** Function    : ���ܵȼ�Ϊ3ʱ,У�鰲ȫ�������ص����ݰ�
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,��ԿB����M��X,��ȡ������ID�ź͵÷�
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 DES_Verify_P(void)
{
	UINT8 nRet = 0;
	UINT8 i;
	DES_Init(DES_KEY_B,16);
	/*ʹ��KEY_B����M���X
	*/
	if(DES_Run(DES_DECRYPT,DES3_ECB,DES_M,DES_X,16,NULL) != RT_OK)
	{
		nRet = 1;
// 		printf("3DES 16bytes ECB ʹ��KEY_B����M���Xʧ��\r\n");
		return nRet;
	}
	/*�ж�X�ĵ�11�ֽ���R�ĵ�11�ֽ��Ƿ����
	*/
	for(i=5;i<16;i++)
	{
		if(DES_X[i] != DES_R[i])
		{
// 			printf("��ȫ��������ʧ��\r\n");
			return 1;
		}
	}
	
	/*T�������
	 *I����ID
	 *S�����÷�
	 *R'ģ��ʹ����ԿA����Q�õ�R��
	 *P=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R'(11�ֽ�)
	 *X=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R(11�ֽ�)
	*/
	Verify_T = DES_X[0];
	Verify_I = (DES_X[1]<<8)&0xff00;
	Verify_I |= (DES_X[2])&0xff;
	Verify_S = (DES_X[3]<<8)&0xff00;
	Verify_S |= (DES_X[4])&0xff;
	PRINTF_UART1("ȷ����:%x\r\n",Verify_T);
	PRINTF_UART1("����ID:%x %x\r\n",Verify_I/256,Verify_I%256);
	PRINTF_UART1("�����÷�:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : RSA_Get_M
** Function    : ���ܵȼ�Ϊ20ʱ,���㰲ȫ�洢ģ���е�16�ֽ������ź�M
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ20ʱ,ʹ�ù�Կ(n,e)����RSA_Q,��R'�ŵ�Text_M��
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
** Function    : ���ܵȼ�Ϊ3ʱ,���㰲ȫ��ָ֤���е�16�ֽڲ���Q
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ3ʱ,����16�ֽ������R,ֱ�ӷ���ģ�� 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 RSA_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//��ʼ���������
	if(nRet != 0)
	{
// 		printf("���������ʼ��ʧ��\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(RSA_R,16);//��ȡ16�ֽ��������
	if(nRet != 0)
	{
// 		printf("���������ʧ��\r\n");
		return nRet;
	}
	
	//���ܵȼ�Ϊ20,�������Rֱ�ӷ��͸�ģ��
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : RSA_Verify_P
** Function    : ���ܵȼ�Ϊ20ʱ,У�鰲ȫ�������ص����ݰ�
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ20,��Կ����M��X,��ȡ������ID�ź͵÷�
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
// 		printf("��Կ����RSA_Qʧ��\r\n");
		return nRet;
	}
// 	printf("��Կ����RSA_Q�ɹ�,RSA_M���ݳ���%d\r\n",nOutDataLen);
	for(i=0;i<16;i++)
		RSA_X[i] = RSA_Q[112+i];
		

	/*�ж�X�ĵ�11�ֽ���R�ĵ�11�ֽ��Ƿ����
	*/
	for(i=5;i<16;i++)
	{
		if(RSA_X[i] != RSA_R[i])
		{
// 			printf("��ȫ��������ʧ��\r\n");
			return 1;
		}
	}
	
	/*T�������
	 *I����ID
	 *S�����÷�
	 *R'ģ��ʹ����ԿA����Q�õ�R��
	 *P=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R'(11�ֽ�)
	 *X=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R(11�ֽ�)
	*/
	Verify_T = RSA_X[0];
	Verify_I = (RSA_X[1]<<8)&0xff00;
	Verify_I |= (RSA_X[2])&0xff;
	Verify_S = (RSA_X[3]<<8)&0xff00;
	Verify_S |= (RSA_X[4])&0xff;
// 	printf("ȷ����:%x\r\n",Verify_T);
// 	printf("����ID:%x %x\r\n",Verify_I/256,Verify_I%256);
// 	printf("�����÷�:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
/***************************************************************************************************
** Subroutine  : ECC_Get_M
** Function    : ���ܵȼ�Ϊ21ʱ,���㰲ȫ�洢ģ���е�16�ֽ������ź�M
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ21ʱ,����ʹ��˽Կǩ��R��Q,
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
** Function    : ���ܵȼ�Ϊ21ʱ,���㰲ȫ��ָ֤���е�16�ֽڲ���Q
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ21ʱ,����16�ֽ������R,ֱ�ӷ���ģ�� 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
UINT8 ECC_Get_Q(void)
{
	UINT32 nRet;
	nRet = Trng_Init(0);//��ʼ���������
	if(nRet != 0)
	{
// 		printf("���������ʼ��ʧ��\r\n");
		return nRet;
	}
	nRet = Trng_GenRandom(ECC_R,16);//��ȡ16�ֽ��������
	if(nRet != 0)
	{
// 		printf("���������ʧ��\r\n");
		return nRet;
	}
	
	//���ܵȼ�Ϊ21,�������Rֱ�ӷ��͸�ģ��
	
	return 0;
}

/***************************************************************************************************
** Subroutine  : ECC_Verify_P
** Function    : ���ܵȼ�Ϊ21ʱ,У�鰲ȫ�������ص����ݰ�
** Input       :  
**                
** Output      : ִ�гɹ�����0,ʧ�ܷ���1
** Description : ���ܵȼ�Ϊ21,˽Կ����Q�õ�X,��ȡ������ID�ź͵÷�
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
// 		printf("˽Կ����RSA_Qʧ��\r\n");
	}
// 	printf("˽Կ����RSA_Q�ɹ�,RSA_M���ݳ���%d\r\n",nOutDataLen);
	for(i=0;i<16;i++)
		ECC_X[i] = ECC_Q[i];
		

	/*�ж�X�ĵ�11�ֽ���R�ĵ�11�ֽ��Ƿ����
	*/
	for(i=5;i<16;i++)
	{
		if(ECC_X[i] != ECC_R[i])
		{
// 			printf("��ȫ��������ʧ��\r\n");
			return 1;
		}
	}
	
	/*T�������
	 *I����ID
	 *S�����÷�
	 *R'ģ��ʹ����ԿA����Q�õ�R��
	 *P=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R'(11�ֽ�)
	 *X=T(1�ֽ�)|I(2�ֽ�)|S(2�ֽ�)|R(11�ֽ�)
	*/
	Verify_T = ECC_X[0];
	Verify_I = (ECC_X[1]<<8)&0xff00;
	Verify_I |= (ECC_X[2])&0xff;
	Verify_S = (ECC_X[3]<<8)&0xff00;
	Verify_S |= (ECC_X[4])&0xff;
	PRINTF_UART1("ȷ����:%x\r\n",Verify_T);
	PRINTF_UART1("����ID:%x %x\r\n",Verify_I/256,Verify_I%256);
	PRINTF_UART1("�����÷�:%x %x\r\n",Verify_S/256,Verify_S%256);
	return nRet;
}



