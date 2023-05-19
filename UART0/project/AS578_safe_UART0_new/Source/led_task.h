#ifndef _LED_TASK_H
#define _LED_TASK_H
#include "SY09_Device.h"

#define BSP_LED1_ON		GpioRegs.GPBDATA.bit.GPB15 = 0
#define BSP_LED1_OFF  GpioRegs.GPBDATA.bit.GPB15 = 1

#define BSP_LED2_ON		GpioRegs.GPBDATA.bit.GPB14 = 0
#define BSP_LED2_OFF  GpioRegs.GPBDATA.bit.GPB14 = 1


void LedTask(void);
#endif


