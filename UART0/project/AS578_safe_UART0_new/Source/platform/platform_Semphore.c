#include "platform_Semphore.h"

/* ��ֵ�ź�����ʼ
 * ��ʼ��Ϊ0
*/
int platform_semphore_init(platform_semphr_t* s)
{
	s->Semphore = xSemaphoreCreateBinary(); //xSemaphoreCreateMutex();
	//xSemaphoreGive(s->Semphore);
	return 0;
}

/* ��ȡһ���ź���,���޵ȴ�
*/
int platform_semphore_take(platform_semphr_t* s)
{ 
	return xSemaphoreTake(s->Semphore,portMAX_DELAY);
}

/* ��ȡһ���ź���,���Ƶȴ�ʱ��
*/
int platform_semphore_take_timeout(platform_semphr_t* s,int timeout)
{ 
	return xSemaphoreTake(s->Semphore,timeout);
}


/* ���Ի�ȡһ���ź���,���ȴ�
*/
int platform_semphore_trytake(platform_semphr_t* s)
{ 
	return xSemaphoreTake(s->Semphore,0);
}

/* �ͷ�һ���ź���
*/
int platform_semphore_give(platform_semphr_t* s)
{ 
	return xSemaphoreGive(s->Semphore);
}


/* �ͷ�һ���ź���,���ж������ͷ�
*/
int platform_semphore_give_from_isr(platform_semphr_t* s)
{
		
	static BaseType_t xHigherPriorityTaskWoken = pdFALSE;
	/*�ͷŶ�ֵ�ź���*/
	xSemaphoreGiveFromISR(s->Semphore,&xHigherPriorityTaskWoken);
	/*�����Ҫ�Ļ�����һ�������л�,ϵͳ���ж��Ƿ���Ҫ�����л�*/
	portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
	return pdTRUE;
}


int platform_semphore_destroy(platform_semphr_t* s)
{
	vSemaphoreDelete(s->Semphore);
  return 0;
}
