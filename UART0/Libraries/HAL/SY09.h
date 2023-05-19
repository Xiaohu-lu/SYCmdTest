/***************************************************************************
* File name    :	SY09.h
* Function     :	Header of SY09 
* Author       : 	coins
* Date         :	2014/06/25
* Version      :    v1.0
* Description  :	to include all header files about SY09, such as memory-map-
*					register header, drive
* ModifyRecord :
****************************************************************************/
#ifndef SY09_H_
#define SY09_H_

/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09_Device.h"
#include "core_cm3.h"
#include "stdio.h"
#include "string.h"

#include "Efc.h"
#include "Otp.h"
#include "KFlash.h"
#include "SFlash.h"
#include "XFlash.h"
#include "Sci.h"
#include "Scm.h"
#include "Sqi.h"
#include "Ssi.h"
#include "Swps.h"
#include "Gpio.h"
#include "Timer.h"
#include "Spi.h"
#include "Uart.h"
#include "I2c.h"
#include "Iso7816.h"
#include "Vpwm.h"
#include "Crc.h"
#include "Bvd.h"
#include "Usb.h"
#include "Alg.h"
#include "Dma.h"
#include "Locsc.h"
#include "SYFP.h"

/**************************************************************************
* Global Macro Definition
***************************************************************************/

#define	FWLIB_VERSION		0x09020130	// Version, BCD code, nn.mm	


// define the function name for old firmware library revision
#define print printf
#define _nop  __nop


/**************************************************************************
* Global Type Definition
***************************************************************************/


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/
/****************************************************************************
 * Subroutine:	FWLIB_GetVersion
 * Function:	to get firmware lib version 
 * Input:		None 
 * Output:		unsigned short	
 * Description:	the version is BCD code, such as 0x0010, is version 0.10 
 * Date:		2014.06.25
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE unsigned long FWLIB_GetVersion(void)
{
	return (FWLIB_VERSION);
}

#endif /*SY09_H_*/

