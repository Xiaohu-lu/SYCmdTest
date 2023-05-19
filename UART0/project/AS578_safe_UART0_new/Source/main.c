/****************************************************************************
* File name    :	main.c
* Function     :	the main application of the testing
* Author       : 	footman
* Date         :	2015/07/01
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/

/**********************************************************************************
* Include Header Files
**********************************************************************************/
#include "SYglobal.h"
#include "safecmd.h"
#include "led_task.h"
#include "operationkey.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
/**********************************************************************************
* Local Macro Definition
**********************************************************************************/


/**********************************************************************************
* Local Type Definition
**********************************************************************************/


/**********************************************************************************
* Global Variable Declaration
**********************************************************************************/
UINT32 AlgSramBuf[256/4];

/**********************************************************************************
* Local Functon Declaration
**********************************************************************************/
extern void xPortSysTickHandler(void);
void SysTick_Handler(void)
{
	#if (INCLUDE_xTaskGetSchedulerState == 1)
		if(xTaskGetSchedulerState() != taskSCHEDULER_NOT_STARTED)
		{
	#endif
			xPortSysTickHandler();
	#if (INCLUDE_xTaskGetSchedulerState == 1)
		}
	#endif
}

/**********************************************************************************
* Functon
**********************************************************************************/
void T0_Handler(void)
{	
	TMR_ClrIntFlag(T0);
	LED1_Toggle();
}
void T1_Handler(void)
{	
	TMR_ClrIntFlag(T1);
	RES_Timeout++;
	if(psInApp.nCommand == PS_WaitImage)
	{
		
		if(AutoE_I == 0)//ע��׶�
		{
			psInApp.nCommand = PS_GetEnrollImage;
			psInApp.cReStus = RE_OK;
		}	
		else //��֤�׶�
		{
			psInApp.nCommand = PS_GetImage;
			psInApp.cReStus = RE_OK;		
		}
			
	}
	if(Shake_One == 0)//û�����ֳɹ���,
	{
		if(RES_Timeout > 10)//300ms����һ������ָ���
		{
			RES_Timeout = 0;
			psInApp.cReStus = RE_OK;
			psInApp.nCommand = PS_HandShake;
		}
	}
	else
	{
		if(RES_Timeout > 3330)//10sģ��û������ͨ��
		{
			Shake_One = 0;//���·�����ָ��
			LED1_OFF;
			TMR_Stop(T0);
		}
	}

}
/****************************************************************************
 * Subroutine:	main
 * Function:	the main function
 * Input:		None
 * Output:		
 * Description:	 
 * Date:		2015/07/1
 * ModifyRecord:
 * *************************************************************************/ 
int main()
{  
	SF_Init(512,896);//��ȫ�洢����ʼ��,0x1004_0000--0x1008_0000
	
  SCM_SetSysClk(SYS_CLK_96M);
	print("System Clock = %d Hz \n",SCM_GetSysClk());
  
	/*����GPA10 ����
	 GPA10Ĭ������
	*/
	GpioRegs.GPAUSE1.bit.GPA10 = 0x0;
	if(GpioRegs.GPADATA.bit.GPA10 == 0)
	{
		printf("�͵�ƽ\r\n");
		EFC_ClrProgFlag(AREA_PARA_WE);
		SCB->VTOR = 0;
		NVIC_SystemReset();
	}
	else
	{
		printf("�ߵ�ƽ\r\n");
	}
		
	////�����㷨ǰ�����ȵ��ã����ڳ�ʼ���㷨��ر���
	InitAlgSram(AlgSramBuf);    
	//__enable_irq();//ʹ��ȫ���ж�
	
	/* ���񴴽�
	*/
	LedTask();
	
	
	vTaskStartScheduler();/*�������������*/
	
	
	Trng_Init(0);//��ʼ���������
	UartInit(6);
	TMR_Init(T0, SCM_GetSysClk()/1000*50, TCLK_SRC_FCLK, TMR_INT); //T0,ϵͳʱ�ӣ�50ms�ж�
	NVIC_EnableIRQ(T0_IRQn);
	
	TMR_Init(T1, SCM_GetSysClk()/1000*30, TCLK_SRC_FCLK, TMR_INT); //T1,ϵͳʱ�ӣ�30ms�ж�,
	NVIC_EnableIRQ(T1_IRQn);
	
	TMR_Start(T1);	
	LED_Config();
	ProtocolInit();
	Make_PackH();
	while( 1 )
	{ 	
		DoCommPkg();
    Get_Response();
	}
}





