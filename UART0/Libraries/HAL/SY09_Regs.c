/****************************************************************************
* File name    :	SY09_Regs.c
* Function     :	the peripheral registers viarible defined
* Author       : 	coins
* Date         :	2015/05/27
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09_Device.h"


/**************************************************************************
* Local Macro Definition
***************************************************************************/

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
#if defined ( __CC_ARM   )      /*------------------RealView Compiler -----------------*/
__NO_INIT volatile struct SCM_REGS 		ScmRegs 		__AT(SCM_BASE);
__NO_INIT volatile struct GPIO_REGS 	GpioRegs 		__AT(GPIO_BASE);
__NO_INIT volatile struct TMR_REGS 		T0Regs			__AT(T0_BASE);
__NO_INIT volatile struct USART_REGS 	Spi0Regs		__AT(SPI0_BASE);
__NO_INIT volatile struct RNG_REGS 		RngRegs			__AT(TRNG_BASE);
__NO_INIT volatile struct TMR_REGS 		T1Regs			__AT(T1_BASE);
__NO_INIT volatile struct TMR_REGS 		T2Regs			__AT(T2_BASE);
__NO_INIT volatile struct SSI_REGS 		SsiRegs			__AT(SSI_BASE);
__NO_INIT volatile struct USART_REGS 	Uart0Regs		__AT(UART0_BASE);
__NO_INIT volatile struct USART_REGS 	I2C0Regs		__AT(I2C0_BASE);
__NO_INIT volatile struct SWPS_REGS 	SwpsRegs		__AT(SWPS_BASE);
__NO_INIT volatile struct ISO7816_REGS 	Iso7816Regs		__AT(ISO7816_BASE);
__NO_INIT volatile struct SCI_REGS 		SciRegs			__AT(SCI_BASE);
__NO_INIT volatile struct VPWM_REGS 	VpwmRegs		__AT(VPWM_BASE);
__NO_INIT volatile struct USART_REGS 	Uart1Regs		__AT(UART1_BASE);
__NO_INIT volatile struct DMA_REGS 		DmaRegs			__AT(DMA_BASE);
__NO_INIT volatile struct EFC_REGS 		EfcRegs			__AT(EFC_BASE);
__NO_INIT volatile struct SQI_REGS 		SqiRegs			__AT(SQI_BASE);
__NO_INIT volatile struct CRC_REGS 		CrcRegs			__AT(CRC_BASE);
__NO_INIT volatile struct BVD_REGS 		BvdRegs			__AT(BVD_BASE);
__NO_INIT volatile struct LOCSC_REGS 	LocscRegs		__AT(LOCSC_BASE);
__NO_INIT volatile struct USART_REGS 	Spi1Regs		__AT(SPI1_BASE);
__NO_INIT volatile struct USB_REGS 		UsbRegs			__AT(USB_BASE);
#endif



/**************************************************************************
* Local Functon Declaration
***************************************************************************/

/**************************************************************************
* Functon
***************************************************************************/


