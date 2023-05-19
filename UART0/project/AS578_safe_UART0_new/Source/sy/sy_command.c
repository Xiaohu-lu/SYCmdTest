#include "sy_command.h"
#include "sy_uart_hal.h"
#include "platform_Semphore.h"

#define SY_RESP_LEN			100

static platform_semphr_t sy_ret_semphore;

static int	g_sy_status;
static char g_sy_resp[SY_RESP_LEN];

void SetSYStatus(int status)
{
	g_sy_status = status;
}

int GetSYStatus(void)
{
	return g_sy_status;
}

int SY_Init(void)
{
	platform_semphore_init(&sy_ret_semphore);
	
	return 0;
}


int SYSendCmd(char *buf,int cmd_len,char *resp,int resp_len,int timeout)
{
	int ret;
	int err;
	/* ����ָ�� */
	HAL_SY_Send(buf,cmd_len);
	
	/* �ȴ���� 
	 * 1 ���ɹ��õ��ź���
	 * 0 ����ʱ����
	 */
	ret = platform_semphore_take_timeout(&sy_ret_semphore,timeout);
	if(ret)
	{
		err = GetSYStatus();
		if(!err && resp)
		{
			
		}
	}
}