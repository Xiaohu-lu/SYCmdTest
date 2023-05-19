#include "safecmd.h"
#include "operationkey.h"
UINT8 gUartBuf[512];	//���ڳ�ʼ���жϽ���buf

UINT8 Verify_flag[4];//��Կ��״̬��־


UINT8 User_Count = 0;


UINT16 CheekSum;		 			//У���
UINT8 Cmd_Send[100];	 		//ָ���
UINT8 Cmd_Recv[140];			//��Ӧ��
UINT8 Cmd_DataRecv[600];	//���յ������ݰ�
UINT8 Cmd_Len;			 			//ָ����ܳ���


UINT16 SecurLevel = 2;	//���ܵȼ�


PKG_IN_APP psInApp;

/*��Ӧ��ʱ�ж�*/
UINT16 RES_Timeout = 0;
/*ע��ʱ,��ָ¼�����*/
UINT8 CountEnroll = 0;
/*ע��ʱ,�ж���ָ���»�����ָ�ɿ�
 *0,Ҫ�ж���ָ����
 *1,Ҫ�ж���ָ�ɿ�
*/
UINT8 Press_Down = 0;

/*�жϵ�ǰ���Զ�ע�ỹ��������֤
 *0:�Զ�ע��,��ǰ���Զ�ע��
 *1:�Զ���֤,��ǰ���Զ���֤
*/
UINT8 AutoE_I = 0;

/*�ж��Ƿ��Լ����ֳɹ���
 *0:û�����ֳɹ���,��ʱ��300ms��һ������ָ��
 *1:���ֳɹ���,��ʱ����10s�ж��Ƿ�������ͨ��
*/
UINT8 Shake_One = 0;

/*�����źų���*/
UINT8 Shake_Len = 0;
UINT8 Shake_Hands[64];	//�����źŻ�����

/*��ȫ��������������*/
UINT8 Security_Rand[16];

/*��У����Ƿ���ȷ*/
UINT8 CheekErr = 0;
/*�հ��д��־*/
UINT8 CheekErr_flag = 0;
/*ָ��ִ�д������*/
UINT8 CountErr = 0;

extern UINT16 Verify_S;
/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
extern UINT8 SM4_KEY_A[16];		//SM4��Կ��
extern UINT8 SM4_KEY_B[16];
extern UINT8 SM4_Q[16];			  //���������
extern UINT8 SM4_M[16];				//�����źš���ȫ������Ӧ
/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
extern UINT8 AES_KEY_A[16];		 //AES��ԿA
extern UINT8 AES_KEY_B[16];		 //AES��ԿB
extern UINT8 AES_Q[16];		 		//���������
extern UINT8 AES_M[16];		 		//�����źš���ȫ������Ӧ
// UINT8 AES_R[16];		 		//�����
// UINT8 AES_X[16];	   		//����M��X

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
extern UINT8 DES_KEY_A[16];		//DES��Կ��
extern UINT8 DES_KEY_B[16];
extern UINT8 DES_Q[16];			  //���������
extern UINT8 DES_M[16];				//�����źš���ȫ������Ӧ
// UINT8 DES_R[16];				//�����
// UINT8 DES_X[16];				//����M��X

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
//�������б��������밴���ֽڶ��� 
extern ALIGN(4) UINT8 RSA_n[256], RSA_e[4];	//��Կ��
extern ALIGN(4) UINT8 RSA_Q[128];		 //RSA˽Կ����R���Q,���������
extern ALIGN(4) UINT8 RSA_M[128];		 //��Կ����Q�ĵ�128�ֽڵĺ�16�ֽ�Ϊ�����ź�
// ALIGN(4) UINT8 RSA_X[128];		 //��Կ���ܰ�ȫ�洢���ص���ӦQ��X
extern UINT8 RSA_R[16];							 //�����

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
extern UINT8 ECC_KEY[32];	//˽Կ
extern UINT8 ECC_R[16];		//�����R,���������
extern UINT8 ECC_M[64];		//�����ź�
extern UINT8 ECC_Q[128];		//��ȫ������Ӧ
// UINT8 ECC_X[128];		//˽Կ����Q��X




/***************************************************************************************************
** Subroutine  : LED_Config
** Function    : ��ʼ��LED GPIO LED1-GPIOB14,LED2-GPIOB15 
** Input       :  
**                
** Output      : 
** Description : LED GPIO ��ʼ��,Ϩ��LED  
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void LED_Config(void)
{
	GPIO_Init(GPIOA,1,OUTPUT);
// 	GPIO_Init(GPIOB,14,OUTPUT);
	/*Ϩ��LED*/
	LED1_OFF;
}

/***************************************************************************************************
** Subroutine  : LED1_Toggle
** Function    : LED1״̬��ת 
** Input       :  
**                
** Output      : 
** Description : LED1 GPIO ���ŵ�ƽ��ת 
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void LED1_Toggle(void)
{
	GPIO_BitTog(GPIOA,1);
// 	GPIO_BitTog(GPIOB,14);
}

/***************************************************************************************************
** Subroutine  : UartInit
** Function    : ���ڳ�ʼ�� 
** Input       :  
**                
** Output      : 
** Description :  
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void UartInit(UINT8 nBaudrate)
{
	STRUCT_UART_FORMAT tPRT_FRAME;
	STRUCT_DATA_BUF RXfifi; 
	RXfifi.pBuf= gUartBuf;
	RXfifi.Size=sizeof(gUartBuf);
	tPRT_FRAME.BaudRate=nBaudrate*9600;
	tPRT_FRAME.CharBit=8;
	tPRT_FRAME.Parity=NONE;
	tPRT_FRAME.StopBit=2;
	tPRT_FRAME.TxMode=UART_TX_POLLING;
	UART_Init(UARTX,UARTX_MAP,tPRT_FRAME,&RXfifi,NULL);
	NVIC_EnableIRQ(UARTX_IRQn);//��UARTX�ж�
}

/***************************************************************************************************
** Subroutine  : ProtocolInit
** Function    : ͨѶ���������������ʼ�� 
** Input       :  
** Output      : 
** Description :   
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ProtocolInit(void)
{
	psInApp.cDoStatus = DO_OK;					//��ʼ��ʱ����Ҫ�ȴ�������Ӧ��
	psInApp.cReStus = RE_OK;						//��ʼ��ʱ����Ҫ�ȴ�������Ӧ,����ִ�з���ָ���
	psInApp.nCommand = PS_HandShake;		//��ʼ��һ��ָ��Ϊ����ָ��
	psInApp.pata = (UINT8*)&Cmd_Recv[9];		//Ӧ����Ĳ�����ŵ�ַ
	psInApp.cpataLen = 0;
	psInApp.pcDat = (UINT8*)&Cmd_DataRecv[0];	//���ݰ��Ĵ�ŵ�ַ
	psInApp.dwDatLen = 0;
}


/***************************************************************************************************
** Subroutine  : GenCheekSum
** Function    : ����У���
** Input       : UINT8 *cmd	Ҫ����У��͵�������ʼ��ַ
**               UINT8 Len  Ҫ����У��͵����ݳ���
** Output      : 
** Description : У��ͼ���Ӱ���ʶ��ߵ���������,������У���2�ֽ�
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void GenCheekSum(UINT8 *cmd,UINT8 Len)
{
	UINT16 Cmd_Cheek=0;
	UINT8 i;
	for(i=0;i<Len;i++)
	{
		Cmd_Cheek += cmd[i];
	}
	CheekSum = Cmd_Cheek;
}

/***************************************************************************************************
** Subroutine  : SendCmd
** Function    : ����ָ�����ģ��
** Input       : 
**               
** Output      : 
** Description : ͨ�����ڰ�ָ������͸�ģ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void SendCmd(void)
{
	UART_SendBuf(UARTX, Cmd_Send, Cmd_Len);
}

/***************************************************************************************************
** Subroutine  : Get_Response
** Function    : ����ģ����Ӧ��
** Input       : 
**               
** Output      : 
** Description : ����ģ����Ӧ��,���ж�У����Ƿ���ȷ,û���յ����ݵȴ�����
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void Get_Response(void)
{
	UINT8 Do_OKflag = 0;
	static UINT8 tmp=0;
	static UINT8 pack_Len = 0;
	UINT16 ValidLen;
	UINT32 RecTimOut,LimTimOut,nwPKOFCMDLen,i;
	UINT8 *pRec;
	UINT16 GenSum,GetSum;
	UINT8 cHbuf[2];
	
	ValidLen=UART_GetRxFFValidSize(UARTX);
	if (ValidLen<9)		//û�������İ�ͷ ��ͷ+оƬ��ַ+����ʶ+������
	{
		 return;
	}
	cHbuf[0]=PKG_HEAD&0xff;
	cHbuf[1]=(PKG_HEAD>>8)&0xff;
	while (tmp!=cHbuf[0])	
	{
		UART_ReceByte(UARTX,&tmp);
		if (ValidLen==0) return;
		else ValidLen--;
	} 
		
	if (ValidLen<8)		//û�������İ�ͷ ��ͷ+оƬ��ַ+����ʶ+������
	 return;
	tmp=0;
	
	/*�ȰѰ�ͷ���յ�Cmd_Recv��ַ*/
	pRec=Cmd_Recv;
	memset(pRec,0,9); //��ͷ+оƬ��ַ+����ʶ+������ ��Ϊ0 
	pRec[0]=cHbuf[0];
	UART_ReceBuf(UARTX,&pRec[1],1);
	if (pRec[0]!=cHbuf[0]||pRec[1]!=cHbuf[1])
	{
		if (pRec[1]==cHbuf[0])
			tmp=pRec[1];	
		return;
	} 
	UART_ReceBuf(UARTX,&pRec[2],7);	

	nwPKOFCMDLen = (((UINT16)pRec[7]& 0xff) << 8) + (pRec[8] & 0xff);		
	if(nwPKOFCMDLen > (128+2))
		return;

	/*�������ͷ,�ж�����Ӧ���ݰ��������ݰ�*/
	if(pRec[6] == 0x07)//�жϰ���ʶ,Ӧ���
	{
		pRec = psInApp.pata; //���½��յĵ�ַ
		psInApp.cpataLen = nwPKOFCMDLen - 2; 
		//Ӧ���,���ֻ��Ҫ����Ӧ���
		if(psInApp.cDatStus==COMM_INS)
		{
			Do_OKflag = 1;//��Do_OKflag = 1,�������������ٰ�psInApp.cDoStatus����ΪDO_OK���ȴ�������Ӧ
		}
	}
	else if(pRec[6] == 0x02)//���ݰ�,�����������ݰ�
	{
		//һ������С128�ֽ�����,��ַƫ��Cmd_Recv+128;
		pack_Len++;//���յ�һ�����ݰ���++����ֵ�Ƕ��ٸ����ݰ�
		pRec = (UINT8 *)(psInApp.pcDat + ((pack_Len-1)*128));//��ַƫ��Cmd_DataRecv+128*(pack_len-1)	
	}
	else if(pRec[6] == 0x08)//���һ�����ݰ�,����û�����ݰ���
	{
		//���ݰ�,��ַCmd_DataRecv
		pack_Len++;//���յ�һ�����ݰ���++����ֵ�Ƕ��ٸ����ݰ�
		pRec = (UINT8 *)(psInApp.pcDat + ((pack_Len-1)*128));//��ַƫ��Cmd_DataRecv+128*(pack_len-1)
		pack_Len=0;//����,���������һ��ѭ�����ݰ�
		
		Do_OKflag = 1;	//���������ݰ�,��Do_OKfalg = 1��ʾ����������ɣ��ȴ�������Ӧ
	}
	i = 0;
	RecTimOut = 0;
	LimTimOut = SCM_GetSysClk() / (6 * 9600) * 20 * 85 / 70;//while��һ��ѭ��70��ϵͳʱ�ӣ�һ֡���ݼ���20B���ȣ���ʱʱ�䶨Ϊ85֡ʱ�䡣
	while (i<nwPKOFCMDLen)
	{
			if (UART_ReceByte(UARTX,&pRec[i])==RT_OK)
			{
					i++;
					RecTimOut=0;	
			}	
			else
			{
					if (RecTimOut++>LimTimOut)
							return;
			}											
	}		
	/*�ж�У����Ƿ���ȷ*/
	GenCheekSum((UINT8 *)&Cmd_Recv[6],3);
	GenSum = CheekSum;
	GenCheekSum(pRec,nwPKOFCMDLen-2);
	GenSum += CheekSum;
	
	GetSum = (pRec[nwPKOFCMDLen-2]<<8)&0xff00;
	GetSum |= (pRec[nwPKOFCMDLen-1])&0xff;
	if(GenSum != GetSum)//У��Ͳ���ȷ,������Ӧ���д�
	{
		CheekErr++;
	}
	
	/*��������ɸ���Do_OKflag = 1��psInApp.cDoStatus����ΪDO_OK*/
	if(Do_OKflag == 1)
	{	
		/*����ж�CheekErr�Ƿ����0,����0��ʾ�հ��д�*/
		if(CheekErr > 0)//�հ��д�
		{
			CheekErr_flag = 1;
		}
		else//�հ��޴�
		{
			CheekErr_flag = 0;
		}
		CheekErr = 0;
		psInApp.cDoStatus = DO_OK;//�ȴ�������Ӧ
		/*�Ѷ�ʱ��T1�ı�������*/
		RES_Timeout = 0;
	}

	return;
}

/***************************************************************************************************
** Subroutine  : DoCommPkg
** Function    : ����ָ���,�Լ�����ģ�鷵�ص���Ӧ��
** Input       : 
**               
** Output      : 
** Description : �ȷ���ָ���,�ȴ���������Ӧ��,������Ӧ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void DoCommPkg(void)
{
	if(psInApp.cReStus == RE_OK)//���Է���
	{
		switch(psInApp.nCommand)
		{
			case PS_HandShake:						//����ָ��
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_HandShake();
				break;
			case PS_ReadINFpage:					//��ϵͳ����ҳ
				/*Ҫ�������ݰ�*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_ReadINFpage();
				break;
			case PS_WriteReg:							//дϵͳ�Ĵ���
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_WriteReg();
			case PS_GetKeyt:							//��ȡ��Կ��
				/*Ҫ�������ݰ�*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_GetKeyt();
				break;
			case PS_GetEnrollImage:				//ע���ȡͼ��
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GetEnrollImage();
				break;
			case PS_WaitImage:						//�ȴ����ͻ�ȡͼ��
				/*�ȴ���ʱ���ж�,ʲôҲ����*/
				break;
			case PS_GenChar:							//��ȡ����
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GenChar(CountEnroll+1);
				break;
			case PS_RegModel:							//�ϳ�ģ��
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_RegModel();
				break;
			case PS_GetCiphertext:				//��ȡ���������
				/*Ҫ�������ݰ�*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_GetCiphertext();
				break;
			case PS_SecurityStoreChar:		//��ȫ�洢
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_SecurityStoreChar();
				break;
			case PS_GetImage:							//��֤��ȡͼ��
				/*ֻ�����Ӧ���*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GetImage();
				break;
			case PS_SecuritySearch:				//��ȫ����
				/*Ҫ�������ݰ�*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_SecuritySearch();
				break;
				
		}
		/*ָ�����ɵȴ�������Ӧ�ͽ�����Ӧ*/
		psInApp.cReStus = RE_WAIT;
		psInApp.cDoStatus = DO_WAIT;
	}
	if(psInApp.cDoStatus == DO_OK)//��������Ӧ,������Ӧ
	{
		switch(psInApp.nCommand)
		{
			case PS_HandShake:				//������Ӧ
				Res_HandShake();
				break;
			case PS_ReadINFpage:			//��ȡϵͳ����ҳ��Ӧ
				Res_ReadINFpage();
				break;
			case PS_WriteReg:					//дϵͳ�Ĵ�����Ӧ
				Res_WriteReg();
				break;
			case PS_GetKeyt:					//��ȡ��Կ����Ӧ
				Res_GetKeyt();
				break;	
			case PS_GetEnrollImage:		//ע���ȡͼ����Ӧ
				Res_GetEnrollImage();
				break;
			case PS_WaitImage:				//�ȴ����ͻ�ȡͼ����Ӧ
				/*�ȴ���ʱ���ж�,ʲôҲ����*/
				break;
			case PS_GenChar:					//��ȡ������Ӧ
				Res_GenChar();
				break;
			case PS_RegModel:					//�ϳ�ģ����Ӧ	
				Res_RegModel();
				break;
			case PS_GetCiphertext:		//��ȡ�����������Ӧ
				Res_GetCiphertext();
				break;
			case PS_SecurityStoreChar://��ȫ�洢��Ӧ	
				Res_SecurityStoreChar();
				break;
			case PS_GetImage:					//��֤��ȡͼ����Ӧ
				Res_GetImage();
				break;
			case PS_SecuritySearch:		//��ȫ������Ӧ
				Res_SecuritySearch();
				break;
			
		}
		
		if(CountErr > 5)	//���հ��������>5��,��ʼִ������ָ��,����ָ��ִ�д������5��
		{
			CountErr = 0;
			Shake_One = 0;
			psInApp.nCommand = PS_HandShake;
		}
		
		/*������Ӧ���,����ָ�����,���Է�����һ��ָ���*/
		psInApp.cReStus = RE_OK;
		
		/*�Ѷ�ʱ��T1�ı�������*/
		RES_Timeout = 0;
	}
}
/***************************************************************************************************
** Subroutine  : Make_PackH
** Function    : ��ʼ��ָ�����ǰ7�ֽ�����
** Input       : 
**               
** Output      : 
** Description : ��ʼ�����İ�ͷ���豸��ַ�Ͱ���ʶ
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Make_PackH(void)
{
	memset(Cmd_Send,0,sizeof(Cmd_Send));
	
	Cmd_Send[0] = 0xEF;
	Cmd_Send[1] = 0x01;
	Cmd_Send[2] = 0xFF;
	Cmd_Send[3] = 0xFF;
	Cmd_Send[4] = 0xFF;
	Cmd_Send[5] = 0xFF;
	Cmd_Send[6] = 0x01;
}

/***************************************************************************************************
** Subroutine  : Cmd_GetImage
** Function    : ��֤��ȡͼ��
** Input       : 
**               
** Output      : 
** Description : ������֤��ȡͼ��ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_GetImage(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0x01;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_GenChar
** Function    : ��������
** Input       : UINT8 BufferID --- ������ID��
**               
** Output      : 
** Description : ������������ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_GenChar(UINT8 BufferID)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x04;
	Cmd_Send[9] = 0x02;
	Cmd_Send[10] = BufferID;
	GenCheekSum(&Cmd_Send[6],5);
	Cmd_Send[11] = (CheekSum>>8)&0xff;
	Cmd_Send[12] = (CheekSum&0xff);
	Cmd_Len = 13;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_RegModel
** Function    : �ϳ�ģ��
** Input       : 
**               
** Output      : 
** Description : ���ͺϳ�ģ��ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_RegModel(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0x05;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_WriteReg
** Function    : ���ü��ܵȼ�
** Input       : 
**               
** Output      : 
** Description : дϵͳ�Ĵ���7�����ܵȼ�SecurLevel
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_WriteReg(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x05;
	Cmd_Send[9] = 0x0E;
	Cmd_Send[10] = 0x07;
	Cmd_Send[11] = SecurLevel;
	GenCheekSum(&Cmd_Send[6],6);
	Cmd_Send[12] = (CheekSum>>8)&0xff;
	Cmd_Send[13] = (CheekSum&0xff);
	Cmd_Len = 14;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_ReadINFpage
** Function    : ������ҳ
** Input       : 
**               
** Output      : 
** Description : ���Ͷ�����ҳָ��,��ȡ��һ�����ݰ��ļ��ܵȼ�
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_ReadINFpage(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0x16;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_GetEnrollImage
** Function    : ע���ȡͼ��
** Input       : 
**               
** Output      : 
** Description : ����ע���ȡͼ��ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_GetEnrollImage(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0x29;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_GetKeyt
** Function    : ��ȡ��Կ��
** Input       : 
**               
** Output      : 
** Description : ���ͻ�ȡ��Կ��ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_GetKeyt(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0xE0;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
	printf("���ͻ�ȡ��Կ��ָ��\r\n");
}

/***************************************************************************************************
** Subroutine  : Cmd_GetCiphertext
** Function    : ��ȡ���������
** Input       : 
**               
** Output      : 
** Description : ���ͻ�ȡ���������ָ��
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_GetCiphertext(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0xE2;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_SecurityStoreChar
** Function    : ��ȫ�洢ģ��
** Input       : 
**               
** Output      : 
** Description : ���Ͱ�ȫ�洢ģ��ָ��,buffID=1,PageID=00 01
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_SecurityStoreChar(void)
{
	UINT8 i;
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = ((Shake_Len+6) & 0xff00);
	Cmd_Send[8] = ((Shake_Len+6) & 0xff);
	Cmd_Send[9] = 0xE3;
	Cmd_Send[10] = 0x01;//��������
	Cmd_Send[11] = 0x00;//λ�ú�
	Cmd_Send[12] = 0x01;//λ�ú�
	
	for(i=0;i<Shake_Len;i++)
		Cmd_Send[13+i] = Shake_Hands[i];
	
	printf("%d\r\n",Shake_Len);
	for(i=0;i<15+Shake_Len;i++)
		printf("%x ",Cmd_Send[i]);
	
	GenCheekSum(&Cmd_Send[6],7+Shake_Len);
	Cmd_Send[13+Shake_Len] = (CheekSum>>8)&0xff;
	Cmd_Send[14+Shake_Len] = (CheekSum&0xff);
	Cmd_Len = 15+Shake_Len;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_SecuritySearch
** Function    : ��ȫ����ָ��
** Input       : 
**               
** Output      : 
** Description : ���Ͱ�ȫ����ָ��ָ��,BufferID=1,StartPage=00 00,PageNum=ff ff
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_SecuritySearch(void)
{
	UINT8 i;
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x18;
	Cmd_Send[9] = 0xE4;
	Cmd_Send[10] = 0x01;//��������
	Cmd_Send[11] = 0x00;//��ʼҳ
	Cmd_Send[12] = 0x00;//��ʼҳ
	Cmd_Send[13] = 0xFF;//ҳ��
	Cmd_Send[14] = 0xFF;//ҳ��
	
	/*ȥִ�а�ȫ����,��ȡָ����е�16�ֽڲ���*/
	switch(SecurLevel)
	{
		case 2:
			SM4_GetRand();
			break;
		case 3:
			AES_GetRand();
			break;
		case 4:
			DES_GetRand();
			break;
		case 20:
			RSA_GetRand();
			break;
		case 21:
			ECC_GetRand();
			break;
	}
	
	
	for(i=0;i<16;i++)
	{
		Cmd_Send[15+i] = Security_Rand[i];
	}
	GenCheekSum(&Cmd_Send[6],25);
	Cmd_Send[31] = (CheekSum>>8)&0xff;
	Cmd_Send[32] = (CheekSum&0xff);
	Cmd_Len = 33;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Cmd_HandShake
** Function    : ����ָ��
** Input       : 
**               
** Output      : 
** Description : ��������ָ���
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void Cmd_HandShake(void)
{
	memset(&Cmd_Send[7],0,(sizeof(Cmd_Send)-7));
	
	Cmd_Send[7] = 0x00;
	Cmd_Send[8] = 0x03;
	Cmd_Send[9] = 0x35;
	GenCheekSum(&Cmd_Send[6],4);
	Cmd_Send[10] = (CheekSum>>8)&0xff;
	Cmd_Send[11] = (CheekSum&0xff);
	Cmd_Len = 12;
	SendCmd();
}

/***************************************************************************************************
** Subroutine  : Res_HandShake
** Function    : ������Ӧ���ݰ�����
** Input       : 
**               
** Output      : ����ȷ����
** Description : ����������Ӧ���ݰ�,����ȷ����
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_HandShake(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(nRet == 0x00)
		{
			PRINTF_UART1("�����ź�ִ�гɹ�");
			/*�����ź�ִ�гɹ�������ִ�ж�ȡ����ҳ����,��ȡ���ܵȼ�*/
			psInApp.nCommand = PS_ReadINFpage;
			
			/*���ֳɹ�*/
			Shake_One = 1;
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("�����ź�ȷ����Ϊ%d",psInApp.pata[0]);
			CountErr++;//ָ��ִ�д������++
		}	
	}
	else//�հ��д�
	{
		CountErr++;	//�հ������������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_ReadINFpage
** Function    : ������ҳ����Ӧ���ݰ�����
** Input       : 
**               
** Output      : ����ȷ����
** Description : ��ȡ���ܵȼ�,���ܵȼ�Ϊ0,����дϵͳ�Ĵ���ָ��-���ü��ܵȼ�,�������flash�еı�־
**							 �ж��Ƿ�����Կ,�д�flahs�ж�ȡ��Կ,�޷��ͻ�ȡ��Կ��ָ��
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_ReadINFpage(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)
	{
		if(nRet == 0x00)
		{
			PRINTF_UART1("��ȡ����ҳִ�гɹ�");
			
			/*��ȡ���ܵȼ�*/
			SecurLevel = psInApp.pcDat[20]&0xff00;
			SecurLevel |= (psInApp.pcDat[21]&0xff);
			
			if(SecurLevel == 0)
			{
				PRINTF_UART1("��û���ù����ܵȼ�,��ʼ���ü��ܵȼ�");
				/*��ȡ���ܵȼ�ִ�гɹ�������ִ�����ü��ܵȼ�ָ��,���ü��ܵȼ�Ϊ2*/
				SecurLevel = 3;
				psInApp.nCommand = PS_WriteReg;
				
			}
			else
			{
				PRINTF_UART1("�Ѿ����ù����ܵȼ�,���ܵȼ�Ϊ%d",SecurLevel);
				PRINTF_UART1("��flash�ж�ȡ��Կ��");
				/*��������flash��ַ�ı�־�ж��Ƿ�����Կ��*/
				R_KeyFormFlash();
			}
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
		}
		else
		{
			PRINTF_UART1("��ȡ����ҳȷ����Ϊ%d",nRet);
			CountErr++;//ָ��ִ�д������++
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_WriteReg
** Function    : дϵͳ�Ĵ�������Ӧ���ݰ�����
** Input       : 
**               
** Output      : ����ȷ����
** Description : ���ü��ܵȼ��ɹ�,��һ������Ϊ��ȡ��Կ��,ʧ�ܼ���ִ�����ü��ܵȼ�,ʧ�ܴ�,��ʼ����ִ������ָ��
**							 
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_WriteReg(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(nRet == 0)//���ü��ܵȼ��ɹ�
		{
			PRINTF_UART1("���ü��ܵȼ��ɹ�,���ܵȼ�Ϊ%d",SecurLevel);
			/*������ִ�л�ȡ��Կ��ָ��*/
			psInApp.nCommand = PS_GetKeyt;
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("���ü��ܵȼ�ʧ��,ʧ����Ϊ%d",nRet);
			CountErr++;//ָ��ִ�д������++
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}
/***************************************************************************************************
** Subroutine  : Res_GetKeyt
** Function    : �����ȡ��Կ�Ե���Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ���ݼ��ܵȼ������ݰ����浽��Ӧ�Ļ����Լ��洢����Ӧ��flash
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetKeyt(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(nRet == 0)
		{
			PRINTF_UART1("��ȡ��Կ�Գɹ�");
			/*���ݼ��ܵȼ�������Կ��*/
			switch(SecurLevel)
			{
				case 2:
					SM4_WriteKey();
					break;
				case 3:
					AES_WriteKey();
					break;
				case 4:
					DES_WriteKey();
					break;
				case 20:
					RSA_WriteKey();
					break;
				case 21:
					ECC_WriteKey();
					break;
			}
			/*������Կflag*/
			Verify_flag[3] = 0x23;
			Verify_flag[2] = 0x45;
			Verify_flag[1] = 0x67;
			Verify_flag[0] = 0x89;
			SF_WriteData(Verify_flag,Verify_flag_ADDR,Verify_flag_SIZE);//дVerify_flag��flash��
			
			/*��ȡ����Կ��,��ʼע��ָ��*/
			/*ע���ͼ*/
			psInApp.nCommand = PS_GetEnrollImage;
		
			/*��������T0,׼���Զ�ע��*/
			TMR_Start(T0);
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("��ȡ��Կ��ʧ��,%d",nRet);
			CountErr++;//ָ��ִ�д������++
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetEnrollImage
** Function    : ����ע���ȡͼ�����Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ע��4��¼��,�ȵȴ���ָ����,��ȡ�����ɹ�,�ȴ���ָ�ɿ�
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetEnrollImage(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(AutoE_I == 1)//�Զ�ע��׶�
		AutoE_I = 0;
	
	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(CountEnroll < 4)//4��¼�뻹û���
		{
			if(Press_Down == 0)//һ��¼����ָ,�ȵȴ���ָ����
			{
				if(nRet == 0x00)//��ָ���²�ͼ�ɹ�
				{
					Press_Down = 1;	//�ȴ����ɿ�
					/*��ȡ����*/
					psInApp.nCommand = PS_GenChar;
					PRINTF_UART1("��%d�λ�ȡͼ��ɹ�",CountEnroll+1);
					
					
					if(CountErr != 0)
					{
						CountErr = 0;//ָ��ִ�д����������
					}
					
				}
				else if(nRet == 0x02)//���·���ȡͼ��ָ��
				{
					/*�ȴ�30ms��ʱ���ж����ٴη���ȡͼ��ָ��*/
					psInApp.nCommand = PS_WaitImage;
					PRINTF_UART1("��%d�εȴ���ָ����",CountEnroll+1);
					
					if(CountErr != 0)
					{
						CountErr = 0;//ָ��ִ�д����������
					}
					
				}
				else//�հ�����
				{
					CountErr++;	//ָ��ִ�д������++
				}
				
			}
			else	//Press_Down = 1�ȴ���ָ�뿪,һ����ȡ�����ɹ�
			{
				if(nRet == 0x02)//��ָ�ɿ�
				{
					Press_Down = 0;	//�ȴ��´β�ͼ��ָ����
					/*ע���ͼ*/
					psInApp.nCommand = PS_GetEnrollImage;
					PRINTF_UART1("��%d����ָ�뿪�ɹ�",CountEnroll+1);
					//CountEnroll++;
					
					if(CountErr != 0)
					{
						CountErr = 0;//ָ��ִ�д����������
					}
					
				}
				else if(nRet == 0x00)//�ȴ���ָ�뿪
				{
					/*�ȴ�30ms��ʱ���ж����ٴη���ȡͼ��ָ��*/
					psInApp.nCommand = PS_WaitImage;
					PRINTF_UART1("��%d�εȴ���ָ�뿪",CountEnroll+1);
					
					if(CountErr != 0)
					{
						CountErr = 0;//ָ��ִ�д����������
					}
					
				}
				else//ָ���հ�����
				{
					CountErr++;	//ָ��ִ�д������++
				}
			}
		}
		else	//4��¼�����
		{
			/*���ͺϲ�ģ��ָ���*/
			psInApp.nCommand = PS_RegModel;
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GenChar
** Function    : ������ȡ��������Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ע��ָ�ƽ׶Σ���ȡ�����ɹ���,CountEnroll++;���ܳɹ�����ʧ��,���涼Ҫ�ȴ���ָ�뿪
**							 ��ָ֤�ƽ׶Σ���ȡ�����ɹ�,ִ�а�ȫ����,ʧ�����²�ͼ��ȡ����
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GenChar(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(AutoE_I == 0)//��ǰ���Զ�ע��׶�
		{
			if(nRet == 0)//��ȡ�����ɹ�
			{
				PRINTF_UART1("��%d����ȡ�����ɹ�",CountEnroll+1);
				CountEnroll++;	
				
				if(CountErr != 0)
				{
					CountErr = 0;//ָ��ִ�д����������
				}
				
			}
			else
			{
				PRINTF_UART1("��ȡ����ʧ��,ʧ����%x",nRet);
				CountErr++;//ָ��ִ�д������++
			}
			psInApp.nCommand = PS_GetEnrollImage;
		}
		else//��ǰ���Զ���֤�׶�
		{
			if(nRet == 0)//��ȡ�����ɹ�
			{
				PRINTF_UART1("�Զ���֤��ȡ�����ɹ�");
				/*��ȡ�����ɹ�,ȥִ�а�ȫ����*/
				psInApp.nCommand = PS_SecuritySearch;
				if(CountErr != 0)
				{
					CountErr = 0;//ָ��ִ�д����������
				}
				
				TMR_Stop(T0);
			}
			else
			{
				PRINTF_UART1("�Զ���֤��ȡ����ʧ��,ʧ����%x",nRet);
				psInApp.nCommand = PS_GetImage;
				CountErr++;	//ָ��ִ�д������++
			}
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_RegModel
** Function    : ����ϲ���������Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : 4��¼��ɹ�,�ϲ�����
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_RegModel(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//�հ��޴�
	{
		if(nRet == 0)//�ϲ������ɹ�
		{
			PRINTF_UART1("�ϲ������ɹ�");
			/*׼����ȫ�洢,�ȷ���ȡ���������*/
			psInApp.nCommand = PS_GetCiphertext;
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("�ϲ�����ʧ��,ʧ����Ϊ%d",nRet);
			/*����ע��*/			
			Press_Down = 0;
			psInApp.nCommand = PS_GetEnrollImage;
			CountErr++;//ָ��ִ�д������++
		}
		CountEnroll = 0;
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetCiphertext
** Function    : �����ȡ�������������Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ���ݼ��ܵȼ�,�����ݻ�������,��ȡ�������������Ӧ�Ļ�����,
**							 �ɹ�ȥִ�а�ȫ�洢,ʧ�ܼ���ִ�л�ȡ���������,5��ʧ��,����ָ�����¿�ʼ
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetCiphertext(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//�հ��޴�
	{
		if(nRet == 0)//��ȡ����������ɹ�
		{
			PRINTF_UART1("��ȡ����������ɹ�");
			/*��ȡ���������,��ȡ����Ӧ���ܵȼ��Ļ�������*/
			switch(SecurLevel)
			{
				case 2:
					SM4_ReadQ();
					break;
				case 3:
					AES_ReadQ();
					break;
				case 4:
					DES_ReadQ();
					break;
				case 20:
					RSA_ReadQ();
					break;
				case 21:
					ECC_ReadQ();
					break;
			}	
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("��ȡ���������ʧ��,ʧ����Ϊ%d",nRet);
			
			/*���·��ͻ�ȡ���������*/
			psInApp.nCommand = PS_GetCiphertext;
			CountErr++;//ָ��ִ�д������++
			
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_SecurityStoreChar
** Function    : ����ȫ�洢ģ�����Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ��ȫ�洢�ɹ�,���Ͳ�ͼָ��,������ָ֤������
**							 ʧ��,�ӷ��ͻ�ȡ��Կ�Կ�ʼִ��
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_SecurityStoreChar(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	/*�رն�ʱ��T0*/
	TMR_Stop(T0);	
	
	if(CheekErr_flag == 0)//�հ��޴�
	{
		if(nRet == 0)//��ȫ�洢ģ��ɹ�
		{
			PRINTF_UART1("��%d�Σ���ȫ�洢ģ��ɹ�",User_Count);
			/*��������Զ���֤,��ȡͼ��*/
			User_Count++;
			if(User_Count<30)
			{
				psInApp.nCommand = PS_GetEnrollImage;
			}
			else
			{
				psInApp.nCommand = PS_GetImage;
			}
			LED1_ON;	
			_delay_ms(1000);
			
			TMR_Start(T0);
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			PRINTF_UART1("��ȫ�洢ģ��ʧ��,ʧ����Ϊ%d",nRet);
			LED1_OFF;
			_delay_ms(1000);
			/*���·��ͻ�ȡ���������*/
			psInApp.nCommand = PS_GetKeyt;
			
			CountErr++;//ָ��ִ�д������++
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetImage
** Function    : ������֤��ͼ��Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ��ͼ�ɹ�ȥ��ȡ����,
**							 ��ͼʧ��,������ͼ
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetImage(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(AutoE_I == 0)//��ǰ���Զ�ע��
		AutoE_I = 1;
	if(CheekErr_flag == 0)//�հ��޴�
	{
		if(nRet == 0)//��ȡͼ��ɹ�
		{
			PRINTF_UART1("�Զ���֤��ȡͼ��ɹ�");
			/*��������Զ���֤,��������*/
			psInApp.nCommand = PS_GenChar;
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}	
			
		}
		else if(nRet == 0x02)
		{
			PRINTF_UART1("�Զ���֤��ȡͼ��ʧ��,ʧ����Ϊ%d",nRet);
			PRINTF_UART1("AutoE_I=%d",AutoE_I);
			/*�ȴ���ʱ��,���·��ͻ�ȡͼ��*/
			psInApp.nCommand = PS_WaitImage;
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			
		}
		else
		{
			CountErr++;//ָ��ִ�д������++
		}
		
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_SecuritySearch
** Function    : ����ȫ��������Ӧ
** Input       : 
**               
** Output      : ����ȷ����
** Description : �����ݻ������ж�ȡ��ȫ�������ص����ݰ�,�������ݰ�
**							 �÷ִ���0,LED����5s,����LED��5s,���²�ͼ,��ָ֤��
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_SecuritySearch(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];

	if(CheekErr_flag == 0)//�հ��޴�
	{	
		if(nRet == 0)//��ȫ�����ɹ�
		{
			PRINTF_UART1("��ȫ�����ɹ�");
			/*��ȡ���������,��ȡ����Ӧ���ܵȼ��Ļ�������*/
			switch(SecurLevel)
			{
				case 2:
					SM4_ReadM();
					break;
				case 3:
					AES_ReadM();
					break;
				case 4:
					DES_ReadM();
					break;
				case 20:
					RSA_ReadM();
					break;
				case 21:
					ECC_ReadM();
					break;
				default:
					break;
			}
			if(Verify_S > 0)
			{
				LED1_ON;
				_delay_ms(500);
			}
			else
			{
				LED1_OFF;
				_delay_ms(500);
			}
			
			if(CountErr != 0)
			{
				CountErr = 0;//ָ��ִ�д����������
			}
			TMR_Start(T0);
		}
		else
		{
			if(nRet == 0x35)
			{
				while(1)
				{
					LED1_Toggle();
					_delay_ms(500);
				}
			}
			PRINTF_UART1("��ȫ����ʧ��,ʧ����Ϊ%d",nRet);
			
			/*���·��Ͱ�ȫ����*/
			psInApp.nCommand = PS_SecuritySearch;
			CountErr++;//ָ��ִ�д������++
		}
	}
	else
	{
		CountErr++;	//�հ��������++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : R_KeyFormFlash
** Function    : ���ݱ�־,��flash��ȡ��Կ��
** Input       : 
**               
** Output      : 
** Description : �����־Ϊ0x23456789��flash��ȡ��Կ��,�����ͻ�ȡ��Կ��ָ��
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void R_KeyFormFlash(void)
{
	UINT32 flag = 0;
	SF_ReadData(Verify_flag,0,4); 
	flag = (Verify_flag[3]<<24)&0xFF000000;
	flag |= (Verify_flag[2]<<16)&0xff0000;
	flag |= (Verify_flag[1]<<8)&0xff00;
	flag |= (Verify_flag[0])&0xff;
	
	if(flag != 0x23456789)
	{
		/*����flash��û����Կ��,���ͻ�ȡ��Կ��ָ��*/
		PRINTF_UART1("flash��û����Կ��");
		psInApp.nCommand = PS_GetKeyt;
		PRINTF_UART1("׼����ȡ��Կ��");
	}
	else//��flash�л�ȡ��Կ��
	{
		switch(SecurLevel)
		{
			case 2:
				SM4_RKFormflash();
				break;
			case 3:
				AES_RKFormflash();
				break;
			case 4:
				DES_RKFormflash();
				break;
			case 20:
				RSA_RKFormflash();
				break;
			case 21:
				ECC_RKFormflash();
				break;
		}
		/*��ȡ��Կ�Ժ�,�Զ�ע��ָ��,��ȡͼ��*/
		psInApp.nCommand = PS_GetEnrollImage;
		PRINTF_UART1("��ȡ��Կ�����");
		TMR_Start(T0);
	}	
}

/***************************************************************************************************
** Subroutine  : SM4_RKFormflash
** Function    : SM4,��flash�ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : 
** Description : SM4��flash�ж�ȡ��ԿA����ԿB
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_RKFormflash(void)
{
	PRINTF_UART1("SM4��ȡ��Կ��");
	SF_ReadData(SM4_KEY_A,SM4_KEY_A_ADDR,SM4_KEY_A_SIZE);
	SF_ReadData(SM4_KEY_B,SM4_KEY_B_ADDR,SM4_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : AES_RKFormflash
** Function    : AES,��flash�ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : 
** Description : AES��flash�ж�ȡ��ԿA����ԿB
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_RKFormflash(void)
{
	PRINTF_UART1("AES��ȡ��Կ��");
	SF_ReadData(AES_KEY_A,AES_KEY_A_ADDR,AES_KEY_A_SIZE);
	SF_ReadData(AES_KEY_B,AES_KEY_B_ADDR,AES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : DES_RKFormflash
** Function    : DES,��flash�ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : 
** Description : DES��flash�ж�ȡ��ԿA����ԿB
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_RKFormflash(void)
{
	PRINTF_UART1("DES��ȡ��Կ��");
	SF_ReadData(DES_KEY_A,DES_KEY_A_ADDR,DES_KEY_A_SIZE);
	SF_ReadData(DES_KEY_B,DES_KEY_B_ADDR,DES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : RSA_RKFormflash
** Function    : RSA,��flash�ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : 
** Description : RSA��flash�ж�ȡ��Կ(n,e)
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_RKFormflash(void)
{
	PRINTF_UART1("RSA��ȡ��Կ��");
	SF_ReadData(RSA_e,RSA_E_ADDR,RSA_E_SIZE);
	SF_ReadData(RSA_n,RSA_N_ADDR,RSA_N_SIZE);
}

/***************************************************************************************************
** Subroutine  : ECC_RKFormflash
** Function    : ECC,��flash�ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : ����ȷ����
** Description : ECC��flash��˽Կ32�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_RKFormflash(void)
{
	PRINTF_UART1("ECC��ȡ��Կ��");
	SF_ReadData(ECC_KEY,ECC_KEY_ADDR,ECC_KEY_SIZE);
}

/***************************************************************************************************
** Subroutine  : SM4_WriteKey
** Function    : SM4,�����ݰ��ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : ����ȷ����
** Description : SM4,��ȡ��ԿA��SM4_KEY_A,��ԿB��SM4_KEY_B,д��flash
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<16;i++)
		SM4_KEY_A[i] = psInApp.pcDat[i];
	for(i=0;i<16;i++)
		SM4_KEY_B[i] = psInApp.pcDat[i+16];

	/*����Կд��flash*/
	SF_WriteData(SM4_KEY_A,SM4_KEY_A_ADDR,SM4_KEY_A_SIZE);
	SF_WriteData(SM4_KEY_B,SM4_KEY_B_ADDR,SM4_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : AES_WriteKey
** Function    : AES,�����ݰ��ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : ����ȷ����
** Description : AES,��ȡ��ԿA��AES_KEY_A,��ԿB��AES_KEY_B,д��flash
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<16;i++)
		AES_KEY_A[i] = psInApp.pcDat[i];
	for(i=0;i<16;i++)
		AES_KEY_B[i] = psInApp.pcDat[i+16];

	/*����Կд��flash*/
	SF_WriteData(AES_KEY_A,AES_KEY_A_ADDR,AES_KEY_A_SIZE);
	SF_WriteData(AES_KEY_B,AES_KEY_B_ADDR,AES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : DES_WriteKey
** Function    : DES,�����ݰ��ж�ȡ��ԿA����ԿB
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,��ȡ��ԿA��DES_KEY_A,��ԿB��DES_KEY_B,д��flash
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<16;i++)
		DES_KEY_A[i] = psInApp.pcDat[i];
	for(i=0;i<16;i++)
		DES_KEY_B[i] = psInApp.pcDat[i+16];

	/*����Կд��flash*/
	SF_WriteData(DES_KEY_A,DES_KEY_A_ADDR,DES_KEY_A_SIZE);
	SF_WriteData(DES_KEY_B,DES_KEY_B_ADDR,DES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : RSA_WriteKey
** Function    : RSA,�����ݰ��ж�ȡ��Կ,n��e
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,��ȡe��RSA_e,��ȡn��RSA_n
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<4;i++)
		RSA_e[i] = psInApp.pcDat[i];
	for(i=0;i<128;i++)
		RSA_n[i] = psInApp.pcDat[i+4];

	/*����Կд��flash*/
	SF_WriteData(RSA_e,RSA_E_ADDR,RSA_E_SIZE);
	SF_WriteData(RSA_n,RSA_N_ADDR,RSA_N_SIZE);
}

/***************************************************************************************************
** Subroutine  : ECC_WriteKey
** Function    : ECC,�����ݰ��ж�ȡ˽Կ
** Input       : 
**               
** Output      : ����ȷ����
** Description : ECC,��ȡ˽Կ��ECC_KEY
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<32;i++)
		ECC_KEY[i] = psInApp.pcDat[i];

	/*����Կд��flash*/
	SF_WriteData(ECC_KEY,ECC_KEY_ADDR,ECC_KEY_SIZE);
}

/***************************************************************************************************
** Subroutine  : SM4_ReadQ
** Function    : SM4,�����ݰ��ж�ȡ���������Q
** Input       : 
**               
** Output      : ����ȷ����
** Description : SM4,��ȡQ,Ȼ���������ź�,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		SM4_Q[i] = psInApp.pcDat[i];
	
	/*ʹ����ԿA����Q��R',Ȼ��ʹ����ԿB����R'��M,*/
	nRet = SM4_Get_M();
	if(nRet == 0)//��ȡ�����źųɹ�
	{
		PRINTF_UART1("��ȡ�����źųɹ�");
		Shake_Len = 16;//�����źų���Ϊ16�ֽ�
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = SM4_M[i];
		/*��������,��ȡ�����źŷ��Ͱ�ȫ�洢ģ��*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("��ȡ�����ź�ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_ReadQ
** Function    : AES,�����ݰ��ж�ȡ���������Q
** Input       : 
**               
** Output      : ����ȷ����
** Description : AES,��ȡQ,Ȼ���������ź�,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		AES_Q[i] = psInApp.pcDat[i];
	
	/*ʹ����ԿA����Q��R',Ȼ��ʹ����ԿB����R'��M,*/
	nRet = AES_Get_M();
	if(nRet == 0)//��ȡ�����źųɹ�
	{
		PRINTF_UART1("��ȡ�����źųɹ�");
		Shake_Len = 16;//�����źų���Ϊ16�ֽ�
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = AES_M[i];
		/*��������,��ȡ�����źŷ��Ͱ�ȫ�洢ģ��*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("��ȡ�����ź�ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_ReadQ
** Function    : DES,�����ݰ��ж�ȡ���������Q
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,��ȡQ,Ȼ���������ź�,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		DES_Q[i] = psInApp.pcDat[i];
	
	/*ʹ����ԿA����Q��R',Ȼ��ʹ����ԿB����R'��M,*/
	nRet = DES_Get_M();
	if(nRet == 0)//��ȡ�����źųɹ�
	{
		PRINTF_UART1("��ȡ�����źųɹ�");
		Shake_Len = 16;//�����źų���Ϊ16�ֽ�
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = DES_M[i];
		/*��������,��ȡ�����źŷ��Ͱ�ȫ�洢ģ��*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("��ȡ�����ź�ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_ReadQ
** Function    : RSA,�����ݰ��ж�ȡ���������Q
** Input       : 
**               
** Output      : ����ȷ����
** Description : RSA,��ȡQ,Ȼ���������ź�,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		RSA_Q[i] = psInApp.pcDat[i];
	
	/*ʹ����ԿA����Q��R',Ȼ��ʹ����ԿB����R'��M,*/
	nRet = RSA_Get_M();
	if(nRet == 0)//��ȡ�����źųɹ�
	{
		PRINTF_UART1("��ȡ�����źųɹ�");
		Shake_Len = 16;//�����źų���Ϊ16�ֽ�,RSA_M�ĺ�16�ֽ�
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = RSA_M[i+112];
		/*��������,��ȡ�����źŷ��Ͱ�ȫ�洢ģ��*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("��ȡ�����ź�ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_Q
** Function    : ECC,�����ݰ��ж�ȡ���������Q
** Input       : 
**               
** Output      : ����ȷ����
** Description : ECC,��ȡQ,Ȼ���������ź�,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		ECC_Q[i] = psInApp.pcDat[i];
	
	/*ʹ��ʹ��˽Կǩ��R��M,*/
	nRet = ECC_Get_M();
	if(nRet == 0)//��ȡ�����źųɹ�
	{
		PRINTF_UART1("��ȡ�����źųɹ�");
		Shake_Len = 64;//�����źų���Ϊ16�ֽ�,RSA_M�ĺ�16�ֽ�
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = ECC_M[i];
		/*��������,��ȡ�����źŷ��Ͱ�ȫ�洢ģ��*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("��ȡ�����ź�ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : SM4_GetRand
** Function    : SM4,���㰲ȫ�����Ĳ���16�ֽ�
** Input       : 
**               
** Output      : ����ȷ����
** Description : SM4,����16�ֽ������R,ʹ����ԿA����R��Q,����16�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*����16�ֽ������R,ʹ����ԿA����R��Q*/
	nRet = SM4_Get_Q();
	if(nRet == 0)//��ȡ��ȫ�洢����Q�ɹ�
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Q�ɹ�");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = SM4_Q[i];
	}
	else
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Qʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_GetRand
** Function    : AES,���㰲ȫ�����Ĳ���16�ֽ�
** Input       : 
**               
** Output      : ����ȷ����
** Description : AES,����16�ֽ������R,ʹ����ԿA����R��Q,����16�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*����16�ֽ������R,ʹ����ԿA����R��Q*/
	nRet = AES_Get_Q();
	if(nRet == 0)//��ȡ��ȫ�洢����Q�ɹ�
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Q�ɹ�");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = AES_Q[i];	
	}
	else
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Qʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_GetRand
** Function    : DES,���㰲ȫ�����Ĳ���16�ֽ�
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,����16�ֽ������R,ʹ����ԿA����R��Q,����16�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*����16�ֽ������R,ʹ����ԿA����R��Q*/
	nRet = DES_Get_Q();
	if(nRet == 0)//��ȡ��ȫ�洢����Q�ɹ�
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Q�ɹ�");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = DES_Q[i];
	}
	else
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Qʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_GetRand
** Function    : RSA,���㰲ȫ�����Ĳ���16�ֽ�
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,����16�ֽ������R,����16�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*����16�ֽ������R,*/
	nRet = RSA_Get_Q();
	if(nRet == 0)//��ȡ��ȫ�洢����Q�ɹ�
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Q�ɹ�");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = RSA_R[i];	
	}
	else
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Qʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_GetRand
** Function    : ECC,���㰲ȫ�����Ĳ���16�ֽ�
** Input       : 
**               
** Output      : ����ȷ����
** Description : ECC,����16�ֽ������R,����16�ֽ�
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*����16�ֽ������R,*/
	nRet = ECC_Get_Q();
	if(nRet == 0)//��ȡ��ȫ�洢����Q�ɹ�
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Q�ɹ�");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = ECC_R[i];
	}
	else
	{
		PRINTF_UART1("��ȡ��ȫ�洢����Qʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*�ӻ�ȡ��Կ�Կ�ʼִ��*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : SM4_ReadM
** Function    : SM4,�����ݰ��ж�ȡ��ȫ�����ķ���ֵ,M
** Input       : 
**               
** Output      : ����ȷ����
** Description : SM4,��ȡM,ʹ����ԿB����M��X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		SM4_M[i] = psInApp.pcDat[i];
	
	/*������ȫ�����������ݰ�*/
	nRet = SM4_Verify_P();
	if(nRet == 0)//��ȫ�����ɹ�
	{
		PRINTF_UART1("��ȫ�����ɹ�");
		/*һ�ΰ�ȫ����ִ�гɹ�*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("��ȫ����ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*һ�ΰ�ȫ����ִ��ʧ��*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_ReadM
** Function    : AES,�����ݰ��ж�ȡ��ȫ�����ķ���ֵ,M
** Input       : 
**               
** Output      : ����ȷ����
** Description : AES,��ȡM,ʹ����ԿB����M��X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		AES_M[i] = psInApp.pcDat[i];
	
	/*������ȫ�����������ݰ�*/
	nRet = AES_Verify_P();
	if(nRet == 0)//��ȫ�����ɹ�
	{
		PRINTF_UART1("��ȫ�����ɹ�");
		/*һ�ΰ�ȫ����ִ�гɹ�*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("��ȫ����ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*һ�ΰ�ȫ����ִ��ʧ��*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_ReadM
** Function    : DES,�����ݰ��ж�ȡ��ȫ�����ķ���ֵ,M
** Input       : 
**               
** Output      : ����ȷ����
** Description : DES,��ȡM,ʹ����ԿB����M��X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		DES_M[i] = psInApp.pcDat[i];
	
	/*������ȫ�����������ݰ�*/
	nRet = DES_Verify_P();
	if(nRet == 0)//��ȫ�����ɹ�
	{
		PRINTF_UART1("��ȫ�����ɹ�");
		/*һ�ΰ�ȫ����ִ�гɹ�*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("��ȫ����ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*һ�ΰ�ȫ����ִ��ʧ��*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_ReadM
** Function    : RSA,�����ݰ��ж�ȡ��ȫ�����ķ���ֵ,M
** Input       : 
**               
** Output      : ����ȷ����
** Description : RSA,��ȡM,ʹ�ù�Կ����M��X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		RSA_M[i] = psInApp.pcDat[i];
	
	/*������ȫ�����������ݰ�*/
	nRet = RSA_Verify_P();
	if(nRet == 0)//��ȫ�����ɹ�
	{
		PRINTF_UART1("��ȫ�����ɹ�");
		/*һ�ΰ�ȫ����ִ�гɹ�*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("��ȫ����ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*һ�ΰ�ȫ����ִ��ʧ��*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_ReadM
** Function    : ECC,�����ݰ��ж�ȡ��ȫ�����ķ���ֵ,M
** Input       : 
**               
** Output      : ����ȷ����
** Description : ECC,��ȡM,ʹ�ù�Կ����M��X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		ECC_M[i] = psInApp.pcDat[i];
	
	/*������ȫ�����������ݰ�*/
	nRet = ECC_Verify_P();
	if(nRet == 0)//��ȫ�����ɹ�
	{
		PRINTF_UART1("��ȫ�����ɹ�");
		/*һ�ΰ�ȫ����ִ�гɹ�*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("��ȫ����ʧ��,ʧ�ܷ���Ϊ%d",nRet);
		/*һ�ΰ�ȫ����ִ��ʧ��*/
		psInApp.nCommand = PS_GetImage;
	}
}

