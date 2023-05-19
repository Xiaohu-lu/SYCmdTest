#include "safecmd.h"
#include "operationkey.h"
UINT8 gUartBuf[512];	//串口初始化中断接收buf

UINT8 Verify_flag[4];//密钥对状态标志


UINT8 User_Count = 0;


UINT16 CheekSum;		 			//校验和
UINT8 Cmd_Send[100];	 		//指令包
UINT8 Cmd_Recv[140];			//响应包
UINT8 Cmd_DataRecv[600];	//接收到的数据包
UINT8 Cmd_Len;			 			//指令包总长度


UINT16 SecurLevel = 2;	//加密等级


PKG_IN_APP psInApp;

/*响应超时判断*/
UINT16 RES_Timeout = 0;
/*注册时,手指录入次数*/
UINT8 CountEnroll = 0;
/*注册时,判断手指按下还是手指松开
 *0,要判断手指按下
 *1,要判断手指松开
*/
UINT8 Press_Down = 0;

/*判断当前是自动注册还是主动验证
 *0:自动注册,当前在自动注册
 *1:自动验证,当前在自动验证
*/
UINT8 AutoE_I = 0;

/*判断是否以及握手成功过
 *0:没有握手成功过,定时器300ms发一次握手指令
 *1:握手成功过,定时器中10s判断是否还能正常通信
*/
UINT8 Shake_One = 0;

/*握手信号长度*/
UINT8 Shake_Len = 0;
UINT8 Shake_Hands[64];	//握手信号缓冲区

/*安全搜索参数缓冲区*/
UINT8 Security_Rand[16];

/*包校验和是否正确*/
UINT8 CheekErr = 0;
/*收包有错标志*/
UINT8 CheekErr_flag = 0;
/*指令执行错误次数*/
UINT8 CountErr = 0;

extern UINT16 Verify_S;
/*---------------------------------------------------2--SM4(ECB)---------------------------------------------------*/
extern UINT8 SM4_KEY_A[16];		//SM4密钥对
extern UINT8 SM4_KEY_B[16];
extern UINT8 SM4_Q[16];			  //密文随机数
extern UINT8 SM4_M[16];				//握手信号、安全搜索响应
/*---------------------------------------------------3--AES(128bits,ECB)---------------------------------------------------*/
extern UINT8 AES_KEY_A[16];		 //AES密钥A
extern UINT8 AES_KEY_B[16];		 //AES密钥B
extern UINT8 AES_Q[16];		 		//密文随机数
extern UINT8 AES_M[16];		 		//握手信号、安全搜索响应
// UINT8 AES_R[16];		 		//随机数
// UINT8 AES_X[16];	   		//解密M的X

/*---------------------------------------------------4--3DES(16bytes,ECB)---------------------------------------------------*/
extern UINT8 DES_KEY_A[16];		//DES密钥对
extern UINT8 DES_KEY_B[16];
extern UINT8 DES_Q[16];			  //密文随机数
extern UINT8 DES_M[16];				//握手信号、安全搜索响应
// UINT8 DES_R[16];				//随机数
// UINT8 DES_X[16];				//解密M的X

/*---------------------------------------------------20--RSA(1024bits)---------------------------------------------------*/
//以下所有变量，必须按四字节对齐 
extern ALIGN(4) UINT8 RSA_n[256], RSA_e[4];	//密钥对
extern ALIGN(4) UINT8 RSA_Q[128];		 //RSA私钥加密R后的Q,密文随机数
extern ALIGN(4) UINT8 RSA_M[128];		 //公钥解密Q的到128字节的后16字节为握手信号
// ALIGN(4) UINT8 RSA_X[128];		 //公钥解密安全存储返回的响应Q的X
extern UINT8 RSA_R[16];							 //随机数

/*---------------------------------------------------21--ECC(256bits)---------------------------------------------------*/
extern UINT8 ECC_KEY[32];	//私钥
extern UINT8 ECC_R[16];		//随机数R,密文随机数
extern UINT8 ECC_M[64];		//握手信号
extern UINT8 ECC_Q[128];		//安全搜索响应
// UINT8 ECC_X[128];		//私钥解密Q得X




/***************************************************************************************************
** Subroutine  : LED_Config
** Function    : 初始化LED GPIO LED1-GPIOB14,LED2-GPIOB15 
** Input       :  
**                
** Output      : 
** Description : LED GPIO 初始化,熄灭LED  
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void LED_Config(void)
{
	GPIO_Init(GPIOA,1,OUTPUT);
// 	GPIO_Init(GPIOB,14,OUTPUT);
	/*熄灭LED*/
	LED1_OFF;
}

/***************************************************************************************************
** Subroutine  : LED1_Toggle
** Function    : LED1状态翻转 
** Input       :  
**                
** Output      : 
** Description : LED1 GPIO 引脚电平翻转 
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
** Function    : 串口初始化 
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
	NVIC_EnableIRQ(UARTX_IRQn);//打开UARTX中断
}

/***************************************************************************************************
** Subroutine  : ProtocolInit
** Function    : 通讯包处理所需变量初始化 
** Input       :  
** Output      : 
** Description :   
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ProtocolInit(void)
{
	psInApp.cDoStatus = DO_OK;					//初始的时候不需要等待接收响应。
	psInApp.cReStus = RE_OK;						//初始的时候不需要等待解析响应,可以执行发送指令包
	psInApp.nCommand = PS_HandShake;		//初始第一条指令为握手指令
	psInApp.pata = (UINT8*)&Cmd_Recv[9];		//应答包的参数存放地址
	psInApp.cpataLen = 0;
	psInApp.pcDat = (UINT8*)&Cmd_DataRecv[0];	//数据包的存放地址
	psInApp.dwDatLen = 0;
}


/***************************************************************************************************
** Subroutine  : GenCheekSum
** Function    : 生成校验和
** Input       : UINT8 *cmd	要计算校验和的数据起始地址
**               UINT8 Len  要计算校验和的数据长度
** Output      : 
** Description : 校验和计算从包标识后边的所有数据,不包括校验和2字节
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
** Function    : 发送指令包给模组
** Input       : 
**               
** Output      : 
** Description : 通过串口把指令包发送给模组
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void SendCmd(void)
{
	UART_SendBuf(UARTX, Cmd_Send, Cmd_Len);
}

/***************************************************************************************************
** Subroutine  : Get_Response
** Function    : 接收模组响应包
** Input       : 
**               
** Output      : 
** Description : 接收模组响应包,并判断校验和是否正确,没接收到数据等待接收
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
	if (ValidLen<9)		//没有完整的包头 包头+芯片地址+包标识+包长度
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
		
	if (ValidLen<8)		//没有完整的包头 包头+芯片地址+包标识+包长度
	 return;
	tmp=0;
	
	/*先把包头接收到Cmd_Recv地址*/
	pRec=Cmd_Recv;
	memset(pRec,0,9); //包头+芯片地址+包标识+包长度 设为0 
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

	/*接收完包头,判断是响应数据包还是数据包*/
	if(pRec[6] == 0x07)//判断包标识,应答包
	{
		pRec = psInApp.pata; //更新接收的地址
		psInApp.cpataLen = nwPKOFCMDLen - 2; 
		//应答包,如果只需要接收应答包
		if(psInApp.cDatStus==COMM_INS)
		{
			Do_OKflag = 1;//把Do_OKflag = 1,到下面接收完包再把psInApp.cDoStatus设置为DO_OK，等待解析响应
		}
	}
	else if(pRec[6] == 0x02)//数据包,后续还有数据包
	{
		//一个包大小128字节数据,地址偏移Cmd_Recv+128;
		pack_Len++;//接收到一个数据包就++最后的值是多少个数据包
		pRec = (UINT8 *)(psInApp.pcDat + ((pack_Len-1)*128));//地址偏移Cmd_DataRecv+128*(pack_len-1)	
	}
	else if(pRec[6] == 0x08)//最后一个数据包,后续没有数据包了
	{
		//数据包,地址Cmd_DataRecv
		pack_Len++;//接收到一个数据包就++最后的值是多少个数据包
		pRec = (UINT8 *)(psInApp.pcDat + ((pack_Len-1)*128));//地址偏移Cmd_DataRecv+128*(pack_len-1)
		pack_Len=0;//清零,方便接收下一次循环数据包
		
		Do_OKflag = 1;	//接收完数据包,包Do_OKfalg = 1表示接收数据完成，等待解析响应
	}
	i = 0;
	RecTimOut = 0;
	LimTimOut = SCM_GetSysClk() / (6 * 9600) * 20 * 85 / 70;//while中一个循环70个系统时钟，一帧数据假设20B长度，超时时间定为85帧时间。
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
	/*判断校验和是否正确*/
	GenCheekSum((UINT8 *)&Cmd_Recv[6],3);
	GenSum = CheekSum;
	GenCheekSum(pRec,nwPKOFCMDLen-2);
	GenSum += CheekSum;
	
	GetSum = (pRec[nwPKOFCMDLen-2]<<8)&0xff00;
	GetSum |= (pRec[nwPKOFCMDLen-1])&0xff;
	if(GenSum != GetSum)//校验和不正确,接收响应包有错
	{
		CheekErr++;
	}
	
	/*包接收完成根据Do_OKflag = 1把psInApp.cDoStatus设置为DO_OK*/
	if(Do_OKflag == 1)
	{	
		/*完成判断CheekErr是否大于0,大于0表示收包有错*/
		if(CheekErr > 0)//收包有错
		{
			CheekErr_flag = 1;
		}
		else//收包无错
		{
			CheekErr_flag = 0;
		}
		CheekErr = 0;
		psInApp.cDoStatus = DO_OK;//等待解析响应
		/*把定时器T1的变量清零*/
		RES_Timeout = 0;
	}

	return;
}

/***************************************************************************************************
** Subroutine  : DoCommPkg
** Function    : 发送指令包,以及解析模组返回的响应包
** Input       : 
**               
** Output      : 
** Description : 先发送指令包,等待接收完响应包,解析响应包
** Date        : 2021/9/26
** ModifyRecord:
***************************************************************************************************/
void DoCommPkg(void)
{
	if(psInApp.cReStus == RE_OK)//可以发送
	{
		switch(psInApp.nCommand)
		{
			case PS_HandShake:						//握手指令
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_HandShake();
				break;
			case PS_ReadINFpage:					//读系统参数页
				/*要接收数据包*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_ReadINFpage();
				break;
			case PS_WriteReg:							//写系统寄存器
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_WriteReg();
			case PS_GetKeyt:							//获取密钥对
				/*要接收数据包*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_GetKeyt();
				break;
			case PS_GetEnrollImage:				//注册获取图像
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GetEnrollImage();
				break;
			case PS_WaitImage:						//等待发送获取图像
				/*等待定时器中断,什么也不做*/
				break;
			case PS_GenChar:							//提取特征
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GenChar(CountEnroll+1);
				break;
			case PS_RegModel:							//合成模板
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_RegModel();
				break;
			case PS_GetCiphertext:				//获取密文随机数
				/*要接收数据包*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_GetCiphertext();
				break;
			case PS_SecurityStoreChar:		//安全存储
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_SecurityStoreChar();
				break;
			case PS_GetImage:							//验证获取图像
				/*只需接收应答包*/
				psInApp.cDatStus = COMM_INS;
				Cmd_GetImage();
				break;
			case PS_SecuritySearch:				//安全搜索
				/*要接收数据包*/
				psInApp.cDatStus = COMM_DATA_REC;
				Cmd_SecuritySearch();
				break;
				
		}
		/*指令发送完成等待接收响应和解析响应*/
		psInApp.cReStus = RE_WAIT;
		psInApp.cDoStatus = DO_WAIT;
	}
	if(psInApp.cDoStatus == DO_OK)//接收完响应,解析响应
	{
		switch(psInApp.nCommand)
		{
			case PS_HandShake:				//握手响应
				Res_HandShake();
				break;
			case PS_ReadINFpage:			//获取系统参数页响应
				Res_ReadINFpage();
				break;
			case PS_WriteReg:					//写系统寄存器响应
				Res_WriteReg();
				break;
			case PS_GetKeyt:					//获取密钥对响应
				Res_GetKeyt();
				break;	
			case PS_GetEnrollImage:		//注册获取图像响应
				Res_GetEnrollImage();
				break;
			case PS_WaitImage:				//等待发送获取图像响应
				/*等待定时器中断,什么也不做*/
				break;
			case PS_GenChar:					//提取特征响应
				Res_GenChar();
				break;
			case PS_RegModel:					//合成模板响应	
				Res_RegModel();
				break;
			case PS_GetCiphertext:		//获取密文随机数响应
				Res_GetCiphertext();
				break;
			case PS_SecurityStoreChar://安全存储响应	
				Res_SecurityStoreChar();
				break;
			case PS_GetImage:					//验证获取图像响应
				Res_GetImage();
				break;
			case PS_SecuritySearch:		//安全搜索响应
				Res_SecuritySearch();
				break;
			
		}
		
		if(CountErr > 5)	//接收包错误次数>5次,开始执行握手指令,或者指令执行错误大于5次
		{
			CountErr = 0;
			Shake_One = 0;
			psInApp.nCommand = PS_HandShake;
		}
		
		/*解析响应完成,更新指令完成,可以发送下一个指令包*/
		psInApp.cReStus = RE_OK;
		
		/*把定时器T1的变量清零*/
		RES_Timeout = 0;
	}
}
/***************************************************************************************************
** Subroutine  : Make_PackH
** Function    : 初始化指令包的前7字节数据
** Input       : 
**               
** Output      : 
** Description : 初始化包的包头、设备地址和包标识
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
** Function    : 验证获取图像
** Input       : 
**               
** Output      : 
** Description : 发送验证获取图像指令
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
** Function    : 生成特征
** Input       : UINT8 BufferID --- 缓冲区ID号
**               
** Output      : 
** Description : 发送生成特征指令
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
** Function    : 合成模板
** Input       : 
**               
** Output      : 
** Description : 发送合成模板指令
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
** Function    : 设置加密等级
** Input       : 
**               
** Output      : 
** Description : 写系统寄存器7，加密等级SecurLevel
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
** Function    : 读参数页
** Input       : 
**               
** Output      : 
** Description : 发送读参数页指令,获取第一个数据包的加密等级
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
** Function    : 注册获取图像
** Input       : 
**               
** Output      : 
** Description : 发送注册获取图像指令
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
** Function    : 获取密钥对
** Input       : 
**               
** Output      : 
** Description : 发送获取密钥对指令
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
	printf("发送获取密钥对指令\r\n");
}

/***************************************************************************************************
** Subroutine  : Cmd_GetCiphertext
** Function    : 获取密文随机数
** Input       : 
**               
** Output      : 
** Description : 发送获取密文随机数指令
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
** Function    : 安全存储模板
** Input       : 
**               
** Output      : 
** Description : 发送安全存储模板指令,buffID=1,PageID=00 01
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
	Cmd_Send[10] = 0x01;//缓冲区号
	Cmd_Send[11] = 0x00;//位置号
	Cmd_Send[12] = 0x01;//位置号
	
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
** Function    : 安全搜索指纹
** Input       : 
**               
** Output      : 
** Description : 发送安全搜索指纹指令,BufferID=1,StartPage=00 00,PageNum=ff ff
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
	Cmd_Send[10] = 0x01;//缓冲区号
	Cmd_Send[11] = 0x00;//起始页
	Cmd_Send[12] = 0x00;//起始页
	Cmd_Send[13] = 0xFF;//页数
	Cmd_Send[14] = 0xFF;//页数
	
	/*去执行安全搜索,获取指令包中的16字节参数*/
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
** Function    : 握手指令
** Input       : 
**               
** Output      : 
** Description : 发送握手指令包
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
** Function    : 握手响应数据包处理
** Input       : 
**               
** Output      : 返回确认码
** Description : 接收握手响应数据包,返回确认码
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_HandShake(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//收包无错
	{	
		if(nRet == 0x00)
		{
			PRINTF_UART1("握手信号执行成功");
			/*握手信号执行成功，下面执行读取参数页命令,获取加密等级*/
			psInApp.nCommand = PS_ReadINFpage;
			
			/*握手成功*/
			Shake_One = 1;
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("握手信号确认码为%d",psInApp.pata[0]);
			CountErr++;//指令执行错误次数++
		}	
	}
	else//收包有错
	{
		CountErr++;	//收包错误次数次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_ReadINFpage
** Function    : 读参数页的响应数据包处理
** Input       : 
**               
** Output      : 返回确认码
** Description : 读取加密等级,加密等级为0,发送写系统寄存器指令-设置加密等级,否则根据flash中的标志
**							 判断是否有密钥,有从flahs中读取密钥,无发送获取密钥对指令
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
			PRINTF_UART1("读取参数页执行成功");
			
			/*读取加密等级*/
			SecurLevel = psInApp.pcDat[20]&0xff00;
			SecurLevel |= (psInApp.pcDat[21]&0xff);
			
			if(SecurLevel == 0)
			{
				PRINTF_UART1("还没设置过加密等级,开始设置加密等级");
				/*读取加密等级执行成功，下面执行设置加密等级指令,设置加密等级为2*/
				SecurLevel = 3;
				psInApp.nCommand = PS_WriteReg;
				
			}
			else
			{
				PRINTF_UART1("已经设置过加密等级,加密等级为%d",SecurLevel);
				PRINTF_UART1("从flash中读取密钥对");
				/*根据主控flash地址的标志判断是否有密钥对*/
				R_KeyFormFlash();
			}
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
		}
		else
		{
			PRINTF_UART1("读取参数页确认码为%d",nRet);
			CountErr++;//指令执行错误次数++
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_WriteReg
** Function    : 写系统寄存器的响应数据包处理
** Input       : 
**               
** Output      : 返回确认码
** Description : 设置加密等级成功,下一个命令为获取密钥对,失败继续执行设置加密等级,失败次,开始重新执行握手指令
**							 
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_WriteReg(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(CheekErr_flag == 0)//收包无错
	{	
		if(nRet == 0)//设置加密等级成功
		{
			PRINTF_UART1("设置加密等级成功,加密等级为%d",SecurLevel);
			/*接下来执行获取密钥对指令*/
			psInApp.nCommand = PS_GetKeyt;
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("设置加密等级失败,失败码为%d",nRet);
			CountErr++;//指令执行错误次数++
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}
/***************************************************************************************************
** Subroutine  : Res_GetKeyt
** Function    : 处理获取密钥对的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 根据加密等级把数据包保存到对应的缓存以及存储到对应的flash
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetKeyt(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//收包无错
	{	
		if(nRet == 0)
		{
			PRINTF_UART1("获取密钥对成功");
			/*根据加密等级保存密钥对*/
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
			/*更新密钥flag*/
			Verify_flag[3] = 0x23;
			Verify_flag[2] = 0x45;
			Verify_flag[1] = 0x67;
			Verify_flag[0] = 0x89;
			SF_WriteData(Verify_flag,Verify_flag_ADDR,Verify_flag_SIZE);//写Verify_flag到flash中
			
			/*获取完密钥对,开始注册指纹*/
			/*注册采图*/
			psInApp.nCommand = PS_GetEnrollImage;
		
			/*开启定期T0,准备自动注册*/
			TMR_Start(T0);
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("获取密钥对失败,%d",nRet);
			CountErr++;//指令执行错误次数++
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetEnrollImage
** Function    : 处理注册获取图像的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 注册4次录入,先等待手指按下,提取特征成功,等待手指松开
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetEnrollImage(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(AutoE_I == 1)//自动注册阶段
		AutoE_I = 0;
	
	if(CheekErr_flag == 0)//收包无错
	{	
		if(CountEnroll < 4)//4次录入还没完成
		{
			if(Press_Down == 0)//一次录入手指,先等待手指按下
			{
				if(nRet == 0x00)//手指按下采图成功
				{
					Press_Down = 1;	//等待手松开
					/*提取特征*/
					psInApp.nCommand = PS_GenChar;
					PRINTF_UART1("第%d次获取图像成功",CountEnroll+1);
					
					
					if(CountErr != 0)
					{
						CountErr = 0;//指令执行错误次数清零
					}
					
				}
				else if(nRet == 0x02)//重新发获取图像指令
				{
					/*等待30ms定时器中断中再次发获取图像指令*/
					psInApp.nCommand = PS_WaitImage;
					PRINTF_UART1("第%d次等待手指按下",CountEnroll+1);
					
					if(CountErr != 0)
					{
						CountErr = 0;//指令执行错误次数清零
					}
					
				}
				else//收包有误
				{
					CountErr++;	//指令执行错误次数++
				}
				
			}
			else	//Press_Down = 1等待手指离开,一次提取特征成功
			{
				if(nRet == 0x02)//手指松开
				{
					Press_Down = 0;	//等待下次采图手指按下
					/*注册采图*/
					psInApp.nCommand = PS_GetEnrollImage;
					PRINTF_UART1("第%d次手指离开成功",CountEnroll+1);
					//CountEnroll++;
					
					if(CountErr != 0)
					{
						CountErr = 0;//指令执行错误次数清零
					}
					
				}
				else if(nRet == 0x00)//等待手指离开
				{
					/*等待30ms定时器中断中再次发获取图像指令*/
					psInApp.nCommand = PS_WaitImage;
					PRINTF_UART1("第%d次等待手指离开",CountEnroll+1);
					
					if(CountErr != 0)
					{
						CountErr = 0;//指令执行错误次数清零
					}
					
				}
				else//指令收包有误
				{
					CountErr++;	//指令执行错误次数++
				}
			}
		}
		else	//4次录入完成
		{
			/*发送合并模板指令包*/
			psInApp.nCommand = PS_RegModel;
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GenChar
** Function    : 处理提取特征的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 注册指纹阶段：提取特征成功后,CountEnroll++;不管成功还是失败,后面都要等待手指离开
**							 验证指纹阶段：提取特征成功,执行安全搜索,失败重新采图提取特征
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GenChar(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//收包无错
	{	
		if(AutoE_I == 0)//当前在自动注册阶段
		{
			if(nRet == 0)//提取特征成功
			{
				PRINTF_UART1("第%d次提取特征成功",CountEnroll+1);
				CountEnroll++;	
				
				if(CountErr != 0)
				{
					CountErr = 0;//指令执行错误次数清零
				}
				
			}
			else
			{
				PRINTF_UART1("提取特征失败,失败码%x",nRet);
				CountErr++;//指令执行错误次数++
			}
			psInApp.nCommand = PS_GetEnrollImage;
		}
		else//当前在自动验证阶段
		{
			if(nRet == 0)//提取特征成功
			{
				PRINTF_UART1("自动验证提取特征成功");
				/*提取特征成功,去执行安全搜索*/
				psInApp.nCommand = PS_SecuritySearch;
				if(CountErr != 0)
				{
					CountErr = 0;//指令执行错误次数清零
				}
				
				TMR_Stop(T0);
			}
			else
			{
				PRINTF_UART1("自动验证提取特征失败,失败码%x",nRet);
				psInApp.nCommand = PS_GetImage;
				CountErr++;	//指令执行错误次数++
			}
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_RegModel
** Function    : 处理合并特征的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 4次录入成功,合并特征
**							 
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_RegModel(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//收包无错
	{
		if(nRet == 0)//合并特征成功
		{
			PRINTF_UART1("合并特征成功");
			/*准备安全存储,先发获取密文随机数*/
			psInApp.nCommand = PS_GetCiphertext;
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("合并特征失败,失败码为%d",nRet);
			/*重新注册*/			
			Press_Down = 0;
			psInApp.nCommand = PS_GetEnrollImage;
			CountErr++;//指令执行错误次数++
		}
		CountEnroll = 0;
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetCiphertext
** Function    : 处理获取密文随机数的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 根据加密等级,从数据缓冲区中,读取密文随机数到对应的缓冲区,
**							 成功去执行安全存储,失败继续执行获取密文随机数,5次失败,握手指令重新开始
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetCiphertext(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	if(CheekErr_flag == 0)//收包无错
	{
		if(nRet == 0)//获取密文随机数成功
		{
			PRINTF_UART1("获取密文随机数成功");
			/*读取密文随机数,读取到对应加密等级的缓冲区中*/
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
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("获取密文随机数失败,失败码为%d",nRet);
			
			/*重新发送获取密文随机数*/
			psInApp.nCommand = PS_GetCiphertext;
			CountErr++;//指令执行错误次数++
			
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_SecurityStoreChar
** Function    : 处理安全存储模板的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 安全存储成功,发送采图指令,进行验证指纹流程
**							 失败,从发送获取密钥对开始执行
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_SecurityStoreChar(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	/*关闭定时器T0*/
	TMR_Stop(T0);	
	
	if(CheekErr_flag == 0)//收包无错
	{
		if(nRet == 0)//安全存储模板成功
		{
			PRINTF_UART1("第%d次，安全存储模板成功",User_Count);
			/*到下面的自动验证,获取图像*/
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
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			PRINTF_UART1("安全存储模板失败,失败码为%d",nRet);
			LED1_OFF;
			_delay_ms(1000);
			/*重新发送获取密文随机数*/
			psInApp.nCommand = PS_GetKeyt;
			
			CountErr++;//指令执行错误次数++
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_GetImage
** Function    : 处理验证采图响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 采图成功去提取特征,
**							 采图失败,继续采图
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_GetImage(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];
	
	if(AutoE_I == 0)//当前在自动注册
		AutoE_I = 1;
	if(CheekErr_flag == 0)//收包无错
	{
		if(nRet == 0)//获取图像成功
		{
			PRINTF_UART1("自动验证获取图像成功");
			/*到下面的自动验证,生成特征*/
			psInApp.nCommand = PS_GenChar;
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}	
			
		}
		else if(nRet == 0x02)
		{
			PRINTF_UART1("自动验证获取图像失败,失败码为%d",nRet);
			PRINTF_UART1("AutoE_I=%d",AutoE_I);
			/*等待定时器,重新发送获取图像*/
			psInApp.nCommand = PS_WaitImage;
			
			if(CountErr != 0)
			{
				CountErr = 0;//指令执行错误次数清零
			}
			
		}
		else
		{
			CountErr++;//指令执行错误次数++
		}
		
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : Res_SecuritySearch
** Function    : 处理安全搜索的响应
** Input       : 
**               
** Output      : 返回确认码
** Description : 从数据缓冲区中读取安全搜索返回的数据包,解析数据包
**							 得分大于0,LED灯亮5s,否则LED灭5s,重新采图,验证指纹
** Date        : 2021/10/18
** ModifyRecord:
***************************************************************************************************/
UINT8 Res_SecuritySearch(void)
{
	UINT8 nRet;
	nRet = psInApp.pata[0];

	if(CheekErr_flag == 0)//收包无错
	{	
		if(nRet == 0)//安全搜索成功
		{
			PRINTF_UART1("安全搜索成功");
			/*读取密文随机数,读取到对应加密等级的缓冲区中*/
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
				CountErr = 0;//指令执行错误次数清零
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
			PRINTF_UART1("安全搜索失败,失败码为%d",nRet);
			
			/*重新发送安全搜索*/
			psInApp.nCommand = PS_SecuritySearch;
			CountErr++;//指令执行错误次数++
		}
	}
	else
	{
		CountErr++;	//收包错误次数++
	}
	return nRet;
}

/***************************************************************************************************
** Subroutine  : R_KeyFormFlash
** Function    : 根据标志,从flash读取密钥对
** Input       : 
**               
** Output      : 
** Description : 如果标志为0x23456789从flash读取密钥对,否则发送获取密钥对指令
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
		/*主控flash中没有密钥对,发送获取密钥对指令*/
		PRINTF_UART1("flash中没有密钥对");
		psInApp.nCommand = PS_GetKeyt;
		PRINTF_UART1("准备获取密钥对");
	}
	else//从flash中获取密钥对
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
		/*读取密钥对后,自动注册指令,获取图像*/
		psInApp.nCommand = PS_GetEnrollImage;
		PRINTF_UART1("读取密钥对完成");
		TMR_Start(T0);
	}	
}

/***************************************************************************************************
** Subroutine  : SM4_RKFormflash
** Function    : SM4,从flash中读取密钥A和密钥B
** Input       : 
**               
** Output      : 
** Description : SM4从flash中读取密钥A和密钥B
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_RKFormflash(void)
{
	PRINTF_UART1("SM4读取密钥对");
	SF_ReadData(SM4_KEY_A,SM4_KEY_A_ADDR,SM4_KEY_A_SIZE);
	SF_ReadData(SM4_KEY_B,SM4_KEY_B_ADDR,SM4_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : AES_RKFormflash
** Function    : AES,从flash中读取密钥A和密钥B
** Input       : 
**               
** Output      : 
** Description : AES从flash中读取密钥A和密钥B
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_RKFormflash(void)
{
	PRINTF_UART1("AES读取密钥对");
	SF_ReadData(AES_KEY_A,AES_KEY_A_ADDR,AES_KEY_A_SIZE);
	SF_ReadData(AES_KEY_B,AES_KEY_B_ADDR,AES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : DES_RKFormflash
** Function    : DES,从flash中读取密钥A和密钥B
** Input       : 
**               
** Output      : 
** Description : DES从flash中读取密钥A和密钥B
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_RKFormflash(void)
{
	PRINTF_UART1("DES读取密钥对");
	SF_ReadData(DES_KEY_A,DES_KEY_A_ADDR,DES_KEY_A_SIZE);
	SF_ReadData(DES_KEY_B,DES_KEY_B_ADDR,DES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : RSA_RKFormflash
** Function    : RSA,从flash中读取密钥A和密钥B
** Input       : 
**               
** Output      : 
** Description : RSA从flash中读取公钥(n,e)
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_RKFormflash(void)
{
	PRINTF_UART1("RSA读取密钥对");
	SF_ReadData(RSA_e,RSA_E_ADDR,RSA_E_SIZE);
	SF_ReadData(RSA_n,RSA_N_ADDR,RSA_N_SIZE);
}

/***************************************************************************************************
** Subroutine  : ECC_RKFormflash
** Function    : ECC,从flash中读取密钥A和密钥B
** Input       : 
**               
** Output      : 返回确认码
** Description : ECC从flash中私钥32字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_RKFormflash(void)
{
	PRINTF_UART1("ECC读取密钥对");
	SF_ReadData(ECC_KEY,ECC_KEY_ADDR,ECC_KEY_SIZE);
}

/***************************************************************************************************
** Subroutine  : SM4_WriteKey
** Function    : SM4,从数据包中读取密钥A和密钥B
** Input       : 
**               
** Output      : 返回确认码
** Description : SM4,读取密钥A到SM4_KEY_A,密钥B到SM4_KEY_B,写入flash
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

	/*把密钥写入flash*/
	SF_WriteData(SM4_KEY_A,SM4_KEY_A_ADDR,SM4_KEY_A_SIZE);
	SF_WriteData(SM4_KEY_B,SM4_KEY_B_ADDR,SM4_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : AES_WriteKey
** Function    : AES,从数据包中读取密钥A和密钥B
** Input       : 
**               
** Output      : 返回确认码
** Description : AES,读取密钥A到AES_KEY_A,密钥B到AES_KEY_B,写入flash
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

	/*把密钥写入flash*/
	SF_WriteData(AES_KEY_A,AES_KEY_A_ADDR,AES_KEY_A_SIZE);
	SF_WriteData(AES_KEY_B,AES_KEY_B_ADDR,AES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : DES_WriteKey
** Function    : DES,从数据包中读取密钥A和密钥B
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,读取密钥A到DES_KEY_A,密钥B到DES_KEY_B,写入flash
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

	/*把密钥写入flash*/
	SF_WriteData(DES_KEY_A,DES_KEY_A_ADDR,DES_KEY_A_SIZE);
	SF_WriteData(DES_KEY_B,DES_KEY_B_ADDR,DES_KEY_B_SIZE);
}

/***************************************************************************************************
** Subroutine  : RSA_WriteKey
** Function    : RSA,从数据包中读取公钥,n和e
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,读取e到RSA_e,读取n到RSA_n
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

	/*把密钥写入flash*/
	SF_WriteData(RSA_e,RSA_E_ADDR,RSA_E_SIZE);
	SF_WriteData(RSA_n,RSA_N_ADDR,RSA_N_SIZE);
}

/***************************************************************************************************
** Subroutine  : ECC_WriteKey
** Function    : ECC,从数据包中读取私钥
** Input       : 
**               
** Output      : 返回确认码
** Description : ECC,读取私钥到ECC_KEY
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_WriteKey(void)
{
	UINT8 i;
	for(i=0;i<32;i++)
		ECC_KEY[i] = psInApp.pcDat[i];

	/*把密钥写入flash*/
	SF_WriteData(ECC_KEY,ECC_KEY_ADDR,ECC_KEY_SIZE);
}

/***************************************************************************************************
** Subroutine  : SM4_ReadQ
** Function    : SM4,从数据包中读取密文随机数Q
** Input       : 
**               
** Output      : 返回确认码
** Description : SM4,获取Q,然后获得握手信号,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		SM4_Q[i] = psInApp.pcDat[i];
	
	/*使用密钥A解密Q得R',然后使用密钥B加密R'得M,*/
	nRet = SM4_Get_M();
	if(nRet == 0)//获取握手信号成功
	{
		PRINTF_UART1("获取握手信号成功");
		Shake_Len = 16;//握手信号长度为16字节
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = SM4_M[i];
		/*处理数据,获取握手信号发送安全存储模板*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("获取握手信号失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_ReadQ
** Function    : AES,从数据包中读取密文随机数Q
** Input       : 
**               
** Output      : 返回确认码
** Description : AES,获取Q,然后获得握手信号,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		AES_Q[i] = psInApp.pcDat[i];
	
	/*使用密钥A解密Q得R',然后使用密钥B加密R'得M,*/
	nRet = AES_Get_M();
	if(nRet == 0)//获取握手信号成功
	{
		PRINTF_UART1("获取握手信号成功");
		Shake_Len = 16;//握手信号长度为16字节
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = AES_M[i];
		/*处理数据,获取握手信号发送安全存储模板*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("获取握手信号失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_ReadQ
** Function    : DES,从数据包中读取密文随机数Q
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,获取Q,然后获得握手信号,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		DES_Q[i] = psInApp.pcDat[i];
	
	/*使用密钥A解密Q得R',然后使用密钥B加密R'得M,*/
	nRet = DES_Get_M();
	if(nRet == 0)//获取握手信号成功
	{
		PRINTF_UART1("获取握手信号成功");
		Shake_Len = 16;//握手信号长度为16字节
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = DES_M[i];
		/*处理数据,获取握手信号发送安全存储模板*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("获取握手信号失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_ReadQ
** Function    : RSA,从数据包中读取密文随机数Q
** Input       : 
**               
** Output      : 返回确认码
** Description : RSA,获取Q,然后获得握手信号,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		RSA_Q[i] = psInApp.pcDat[i];
	
	/*使用密钥A解密Q得R',然后使用密钥B加密R'得M,*/
	nRet = RSA_Get_M();
	if(nRet == 0)//获取握手信号成功
	{
		PRINTF_UART1("获取握手信号成功");
		Shake_Len = 16;//握手信号长度为16字节,RSA_M的后16字节
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = RSA_M[i+112];
		/*处理数据,获取握手信号发送安全存储模板*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("获取握手信号失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_Q
** Function    : ECC,从数据包中读取密文随机数Q
** Input       : 
**               
** Output      : 返回确认码
** Description : ECC,获取Q,然后获得握手信号,
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_ReadQ(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		ECC_Q[i] = psInApp.pcDat[i];
	
	/*使用使用私钥签名R得M,*/
	nRet = ECC_Get_M();
	if(nRet == 0)//获取握手信号成功
	{
		PRINTF_UART1("获取握手信号成功");
		Shake_Len = 64;//握手信号长度为16字节,RSA_M的后16字节
		for(i=0;i<Shake_Len;i++)
			Shake_Hands[i] = ECC_M[i];
		/*处理数据,获取握手信号发送安全存储模板*/
		psInApp.nCommand = PS_SecurityStoreChar;
		
	}
	else
	{
		PRINTF_UART1("获取握手信号失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : SM4_GetRand
** Function    : SM4,计算安全搜索的参数16字节
** Input       : 
**               
** Output      : 返回确认码
** Description : SM4,生成16字节随机数R,使用密钥A加密R得Q,参数16字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*生成16字节随机数R,使用密钥A加密R得Q*/
	nRet = SM4_Get_Q();
	if(nRet == 0)//获取安全存储参数Q成功
	{
		PRINTF_UART1("获取安全存储参数Q成功");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = SM4_Q[i];
	}
	else
	{
		PRINTF_UART1("获取安全存储参数Q失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_GetRand
** Function    : AES,计算安全搜索的参数16字节
** Input       : 
**               
** Output      : 返回确认码
** Description : AES,生成16字节随机数R,使用密钥A加密R得Q,参数16字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*生成16字节随机数R,使用密钥A加密R得Q*/
	nRet = AES_Get_Q();
	if(nRet == 0)//获取安全存储参数Q成功
	{
		PRINTF_UART1("获取安全存储参数Q成功");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = AES_Q[i];	
	}
	else
	{
		PRINTF_UART1("获取安全存储参数Q失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_GetRand
** Function    : DES,计算安全搜索的参数16字节
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,生成16字节随机数R,使用密钥A加密R得Q,参数16字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*生成16字节随机数R,使用密钥A加密R得Q*/
	nRet = DES_Get_Q();
	if(nRet == 0)//获取安全存储参数Q成功
	{
		PRINTF_UART1("获取安全存储参数Q成功");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = DES_Q[i];
	}
	else
	{
		PRINTF_UART1("获取安全存储参数Q失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_GetRand
** Function    : RSA,计算安全搜索的参数16字节
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,生成16字节随机数R,参数16字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*生成16字节随机数R,*/
	nRet = RSA_Get_Q();
	if(nRet == 0)//获取安全存储参数Q成功
	{
		PRINTF_UART1("获取安全存储参数Q成功");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = RSA_R[i];	
	}
	else
	{
		PRINTF_UART1("获取安全存储参数Q失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_GetRand
** Function    : ECC,计算安全搜索的参数16字节
** Input       : 
**               
** Output      : 返回确认码
** Description : ECC,生成16字节随机数R,参数16字节
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_GetRand(void)
{
	UINT8 i;
	UINT8 nRet;
	
	/*生成16字节随机数R,*/
	nRet = ECC_Get_Q();
	if(nRet == 0)//获取安全存储参数Q成功
	{
		PRINTF_UART1("获取安全存储参数Q成功");
		
		for(i=0;i<16;i++)
			Security_Rand[i] = ECC_R[i];
	}
	else
	{
		PRINTF_UART1("获取安全存储参数Q失败,失败返回为%d",nRet);
		/*从获取密钥对开始执行*/
// 		psInApp.nCommand = PS_SecurityStoreChar;
	}
	
}

/***************************************************************************************************
** Subroutine  : SM4_ReadM
** Function    : SM4,从数据包中读取安全搜索的返回值,M
** Input       : 
**               
** Output      : 返回确认码
** Description : SM4,获取M,使用密钥B解密M得X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void SM4_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		SM4_M[i] = psInApp.pcDat[i];
	
	/*分析安全搜索返回数据包*/
	nRet = SM4_Verify_P();
	if(nRet == 0)//安全搜索成功
	{
		PRINTF_UART1("安全搜索成功");
		/*一次安全搜索执行成功*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("安全搜索失败,失败返回为%d",nRet);
		/*一次安全搜索执行失败*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : AES_ReadM
** Function    : AES,从数据包中读取安全搜索的返回值,M
** Input       : 
**               
** Output      : 返回确认码
** Description : AES,获取M,使用密钥B解密M得X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void AES_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		AES_M[i] = psInApp.pcDat[i];
	
	/*分析安全搜索返回数据包*/
	nRet = AES_Verify_P();
	if(nRet == 0)//安全搜索成功
	{
		PRINTF_UART1("安全搜索成功");
		/*一次安全搜索执行成功*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("安全搜索失败,失败返回为%d",nRet);
		/*一次安全搜索执行失败*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : DES_ReadM
** Function    : DES,从数据包中读取安全搜索的返回值,M
** Input       : 
**               
** Output      : 返回确认码
** Description : DES,获取M,使用密钥B解密M得X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void DES_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<16;i++)
		DES_M[i] = psInApp.pcDat[i];
	
	/*分析安全搜索返回数据包*/
	nRet = DES_Verify_P();
	if(nRet == 0)//安全搜索成功
	{
		PRINTF_UART1("安全搜索成功");
		/*一次安全搜索执行成功*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("安全搜索失败,失败返回为%d",nRet);
		/*一次安全搜索执行失败*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : RSA_ReadM
** Function    : RSA,从数据包中读取安全搜索的返回值,M
** Input       : 
**               
** Output      : 返回确认码
** Description : RSA,获取M,使用公钥解密M得X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void RSA_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		RSA_M[i] = psInApp.pcDat[i];
	
	/*分析安全搜索返回数据包*/
	nRet = RSA_Verify_P();
	if(nRet == 0)//安全搜索成功
	{
		PRINTF_UART1("安全搜索成功");
		/*一次安全搜索执行成功*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("安全搜索失败,失败返回为%d",nRet);
		/*一次安全搜索执行失败*/
		psInApp.nCommand = PS_GetImage;
	}
	
}

/***************************************************************************************************
** Subroutine  : ECC_ReadM
** Function    : ECC,从数据包中读取安全搜索的返回值,M
** Input       : 
**               
** Output      : 返回确认码
** Description : ECC,获取M,使用公钥解密M得X
** Date        : 2021/10/15
** ModifyRecord:
***************************************************************************************************/
void ECC_ReadM(void)
{
	UINT8 i;
	UINT8 nRet;
	for(i=0;i<128;i++)
		ECC_M[i] = psInApp.pcDat[i];
	
	/*分析安全搜索返回数据包*/
	nRet = ECC_Verify_P();
	if(nRet == 0)//安全搜索成功
	{
		PRINTF_UART1("安全搜索成功");
		/*一次安全搜索执行成功*/
		psInApp.nCommand = PS_GetImage;
		
	}
	else
	{
		PRINTF_UART1("安全搜索失败,失败返回为%d",nRet);
		/*一次安全搜索执行失败*/
		psInApp.nCommand = PS_GetImage;
	}
}

