#ifndef FREERTOS_CONFIG_H
#define FREERTOS_CONFIG_H
#include "SYglobal.h"
//��Բ�ͬ�ı��������ò�ͬ��stdint.h�ļ�
#if defined(__ICCARM__) || defined(__CC_ARM) || defined(__GNUC__)
#include <stdint.h>
extern uint32_t SystemFrequency;
#endif

//����
//#define vAssertCalled(char,int) printf("Error:%s,%d\r\n",char,int)
//#define configASSERT(x) if((x)==0)	vAssertCalled(__FILE__,__LINE__)

/***********************************FreeRTOS������������ѡ��************************************************/
/*��1��RTOSʹ����ռʽ������,
 *��0��RTOSʹ��Э��ʽ������(ʱ��Ƭ)
 *ע���ڶ�������������,����ϵͳ���Է�Ϊ��ռʽ��Э��ʽ���֡�
 *Э��ʽ����ϵͳ�����������ͷ�CPU��,�л�����һ������
 *�����л���ʱ����ȫȡ�����������е�����
*/
#define configUSE_PREEMPTION		1

/*1ʹ��ʱ��Ƭ����,�����ȼ���ͬʱʹ��ʱ��Ƭ����*/
#define configUSE_TIME_SLICING	1

/*ĳЩ����FreeRTOS��Ӳ�������ַ���ѡ����һ��Ҫִ�е�����
 *ͨ�÷������ض���Ӳ���ķ���
 *
 *ͨ�÷�����
 *	1.configUSE_PORT_OPTIMISED_TASK_SELECTIONΪ0����Ӳ����֧�����ⷽ��
 *	2.������������FreeRTOS֧�ֵ�Ӳ��
 *	3.��ȫ��Cʵ��,Ч���Ե������ⷽ��
 *	4.��ǿ��Ҫ���������������ȼ���Ŀ
 *���ⷽ����
 *	1.���뽫configUSE_PORT_OPTIMISED_TASK_SELECTION����Ϊ1
 *	2.����һ�������ض��ܹ��Ļ��ָ��(һ�������Ƽ���ǰ����[CLZ]ָ��).
 *	3.��ͨ�÷�������Ч
 *	4.һ��ǿ���޶����������ȼ���ĿΪ32
 һ����Ӳ������ǰ����ָ��,�����ʹ�õ�,MCUû����ЩӲ��ָ��Ļ��κ�Ӧ������Ϊ0
*/
#define configUSE_PORT_OPTIMISED_TASK_SELECTION	1

/*��1��ʹ�ܵ͹���ticklessģʽ;��0������ϵͳ����(tick)�ж�һֱ����*/
#define configUSE_TICKLESS_IDLE							1

/*
 *д��ʵ�ʵ�CPU�ں�ʱ��Ƶ��,Ҳ����CPUָ��ִ��Ƶ��,ͨ����ΪFclk
 *FclkΪ����CPU�ں˵�ʱ���ź�,������˵��CPU��ƵΪXX MHZ
 *����ָ�����ʱ���ź�,��Ӧ��,1/Fclk��ΪCPU��ʱ������
*/
#define configCPU_CLOCK_HZ			( SystemFrequency )

/*RTOSϵͳ�����жϵ�Ƶ��.��1s���жϵĴ���,ÿ���ж�RTOS��������������*/
#define configTICK_RATE_HZ			( ( TickType_t ) 1000 )

/*���õ�������ȼ�*/
#define configMAX_PRIORITIES		( 32 )

/*��������ʹ�õĶ�ջ��С*/
#define configMINIMAL_STACK_SIZE	( ( unsigned short ) 128 )

/*���������ַ�������*/
#define configMAX_TASK_NAME_LEN		( 16 )

/*ϵͳ���ļ�����������������,1��ʾΪ16λ�޷�������,0��ʾ32λ�޷�������*/
#define configUSE_16_BIT_TICKS	0

/*�����������CPUʹ��Ȩ������ͬ���ȼ����û�����*/
#define configIDLE_SHOULD_YIELD	1

/*���ö���*/
#define configUSE_QUEUE_SETS		1

/*��������֪ͨ����*/
#define configUSE_TASK_NOTIFICATIONS	1

/*ʹ�û����ź���*/
#define configUSE_MUTEXES				1

/*ʹ�õݹ黥���ź���*/
#define configUSE_RECURSIVE_MUTEXES		1

/*Ϊ1ʱʹ�ü����ź���*/
#define configUSE_COUNTING_SEMAPHORES		1

/*���ÿ���ע����ź�������Ϣ���и���*/
#define configQUEUE_REGISTRY_SIZE				10

#define configUSE_APPLICATION_TASK_TAG	0


/***********************************FreeRTOS���ڴ������й�����ѡ��************************************************/
/*֧�ֶ�̬�ڴ�����*/
#define configSUPPORT_DYNAMIC_ALLOCATION		1

/*֧�־�̬�ڴ�*/
#define configSUPPORT_STATIC_ALLOCATION			0

/*ϵͳ�����ܵĶѴ�С*/
#define configTOTAL_HEAP_SIZE								( ( size_t ) ( 14 * 1024 ) )


/***********************************FreeRTOS�빳�Ӻ����й�����ѡ��************************************************/
/*��1��ʹ�ÿ��й���(Idle Hook�����ڻص�����);��0�����Կ��й���
 *
 *������������һ������,����������û���ʵ��
 *FreeRTOS�涨�˺��������ֺͲ�����void vApplicationIdleHook(void)
 *���������ÿ�������������ڶ��ᱻ����
 *��������ɾ����RTOS����,������������ͷŷ�������ǵĶ�ջ�ڴ�
 *��˱��뱣֤����������ñ�CPUִ��
 *ʹ�ÿ��й��Ӻ�������CPU����ʡ��ģʽ�Ǻܳ�����
 *�����Ե��û������������������API����
*/
#define configUSE_IDLE_HOOK			0

/*��1��ʹ��ʱ��Ƭ����(Tick Hook);��0������ʱ��Ƭ����
 *
 *ʱ��Ƭ������һ������,����������û���ʵ��
 *FreeRTOS�涨�˺��������ֺͲ�����void vApplicationTickHook(void)
 *ʱ��Ƭ�жϿ��������Եĵ���
 *��������ǳ���С,���ܴ���ʹ�ö�ջ
 *���ܵ�����"FromISR"����"From_ISR"��β��API����
*/
#define configUSE_TICK_HOOK			0

/*ʹ���ڴ�����ʧ�ܹ��Ӻ���*/
#define configUSE_MALLOC_FAILED_HOOK		0

/*
 *����0ʱ���ö�ջ�����⹦��,���ʹ�ô˹���
 *�û������ṩһ��ջ������Ӻ���,���ʹ�õĻ�
 *��ֵ����Ϊ1����Ϊ2,��Ϊ������ջ�����ⷽ��
*/
#define configCHECK_FOR_STACK_OVERFLOW			0


/***********************************FreeRTOS������ʱ�������״̬�ռ��й�����ѡ��************************************************/
/*��������ʱ��ͳ�ƹ���*/
#define configGENERATE_RUN_TIME_STATS		0

/*���ÿ��ӻ����ٵ���*/
#define configUSE_TRACE_FACILITY				0

/*���configUSE_TRACE_FACILITYͬʱΪ1ʱ���������3������
 *prvWriteNameToBuffer()
 *vTaskList()
 *vTaskGetRunTimeStats()
*/
#define configUSE_STATS_FORMATTING_FUNCTIONS		1

/***********************************FreeRTOS��Э���й�����ѡ��************************************************/
/*����Э��,����Э���Ժ��������ļ�croutine.c*/
#define configUSE_CO_ROUTINES 		0
/*Э�̵���Ч���ȼ���Ŀ*/
#define configMAX_CO_ROUTINE_PRIORITIES ( 2 )

/***********************************FreeRTOS�������ʱ���й�����ѡ��************************************************/
/*���������ʱ��*/
#define configUSE_TIMERS						1

/*�����ʱ�����ȼ�*/
#define configTIMER_TASK_PRIORITY		( configMAX_PRIORITIES - 1 )

/*�����ʱ�����г���*/
#define configTIMER_QUEUE_LENGTH		10

/*�����ʱ�������ջ��С*/
#define configTIMER_TASK_STACK_DEPTH	( configMINIMAL_STACK_SIZE * 2 )


/***********************************FreeRTOS��ѡ��������ѡ��************************************************/
#define INCLUDE_xTaskGetSchedulerState	1
#define INCLUDE_vTaskPrioritySet		1
#define INCLUDE_uxTaskPriorityGet		1
#define INCLUDE_vTaskDelete				1
#define INCLUDE_vTaskCleanUpResources	1
#define INCLUDE_vTaskSuspend			1
#define INCLUDE_vTaskDelayUntil			1
#define INCLUDE_vTaskDelay				1
#define INCLUDE_eTaskGetState			1
#define INCLUDE_xTimerPendFunctionCall	1

/***********************************FreeRTOS���ж��йص�����ѡ��************************************************/
#ifdef __NVIC_PRIO_BITS
#define configPRIO_BITS		__NVIC_PRIO_BITS
#else
#define configPRIO_BITS		4
#endif

/*�ж�������ȼ�*/
#define configLIBRARY_LOWEST_INTERRUPT_PRIORITY		15

/*ϵͳ�ɹ��������ж����ȼ�*/
#define configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY	15

#define configKERNEL_INTERRUPT_PRIORITY	( configLIBRARY_LOWEST_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )

#define configMAX_SYSCALL_INTERRUPT_PRIORITY ( configLIBRARY_MAX_SYSCALL_INTERRUPT_PRIORITY << ( 8 - configPRIO_BITS ) )


/***********************************FreeRTOS���жϷ�������ص�����ѡ��************************************************/
#define xPortPendSVHandler  PendSV_Handler
#define vPortSVCHandler			SVC_Handler

/*����Ϊʹ��Percepio Tracealyzer��Ҫ�Ķ���,����ҪʱҪ��configUSE_TRACE_FACILITY����Ϊ0
*/
#if ( configUSE_TRACE_FACILITY == 1 )
#include "trcRecorder.h"
#define INCLUDE_xTaskGetCurrentTaskHandler 0
#endif
#endif /* FREERTOS_CONFIG_H */

