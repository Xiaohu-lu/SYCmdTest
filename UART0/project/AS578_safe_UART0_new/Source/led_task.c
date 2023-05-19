#include "led_task.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"



static TaskHandle_t AppTaskCreate_Handle = NULL;
static TaskHandle_t LED1_Task_Handle = NULL;
static TaskHandle_t LED2_Task_Handle = NULL;
static TaskHandle_t UART_Task_Handle = NULL;

static void LED1_Task(void *parameter)
{
	for(;;)
	{
		printf("LED1任务达到\r\n");
		BSP_LED1_ON;
		vTaskDelay(500);
		BSP_LED1_OFF;
		vTaskDelay(500);
	}
}

static void LED2_Task(void *parameter)
{
	for(;;)
	{
		printf("LED2任务达到\r\n");
		BSP_LED2_ON;
		vTaskDelay(1000);
		BSP_LED2_OFF;
		vTaskDelay(1000);
	}
}


static void UART_Task(void *parameter)
{
	for(;;)
	{
		printf("Uart任务达到\r\n");
		vTaskDelay(500);
		
	}
}


static void AppTaskCreate(void *parameter)
{
	BaseType_t xReturn;
	/*进入临界区*/
	taskENTER_CRITICAL();
	xReturn = xTaskCreate((TaskFunction_t)LED1_Task,(const char*)"LED1_Task",(uint32_t)128,(void*)NULL,(UBaseType_t)4,(TaskHandle_t*)&LED1_Task_Handle);
	if(pdPASS == xReturn)
	{
		printf("LED1任务创建成功\r\n");
	}
	xReturn = xTaskCreate((TaskFunction_t)LED2_Task,(const char*)"LED2_Task",(uint32_t)128,(void*)NULL,(UBaseType_t)3,(TaskHandle_t*)&LED2_Task_Handle);
	if(pdPASS == xReturn)
	{
		printf("LED1任务创建成功\r\n");
	}
	xReturn = xTaskCreate((TaskFunction_t)UART_Task,(const char*)"Uart_Task",(uint32_t)128,(void*)NULL,(UBaseType_t)5,(TaskHandle_t*)&UART_Task_Handle);
	if(pdPASS == xReturn)
	{
		printf("Uart任务创建成功\r\n");
	}
	vTaskDelete(AppTaskCreate_Handle);
	taskEXIT_CRITICAL();
}


void LedTask(void)
{
	BaseType_t xReturn;
	xReturn = xTaskCreate((TaskFunction_t)AppTaskCreate,(const char*)"AppTaskCreate",(uint32_t)128,(void*)NULL,(UBaseType_t)3,(TaskHandle_t*)&AppTaskCreate_Handle);
	if(pdPASS == xReturn)
	{
		printf("LED Task Create error\r\n");
	}
	else
	{
		printf("LED Task Create success\r\n");
	}

}

