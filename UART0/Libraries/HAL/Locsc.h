/***************************************************************************************************
**  File name    : Locsc.h
**  Function     : Header of Locsc.c		
**  Author       : synochip
**  Date         : 2009/04/02
**  Version      : v1.0          
**  Description  : Header of Locsc Hardware Abstract Layer                   
**  ModifyRecord :          	
***************************************************************************************************/
#ifndef LOCSC_H_
#define LOCSC_H_

/***************************************************************************************************
** Global Macro Definition
***************************************************************************************************/
#ifndef	    _CAPTURE_MODE_
#define     _CAPTURE_MODE_
#define		CONTINUOUS		0
#define		INTERVAL		1
#endif

// define VSYNC polarity
#define	VSYNC_ACTIVE_HIGH	1
#define	VSYNC_ACTIVE_LOW	0

// define HSYNC polarity
#define	HSYNC_ACTIVE_HIGH	1
#define	HSYNC_ACTIVE_LOW	0

// define PCLK polarity
#define	PCLK_CAPTURE_RISING		1 	// rising edge
#define	PCLK_CAPTURE_FALLING	0  	// falling edge

// define the pixel inverse
#define	PIXEL_INV		1	// inverse
#define	PIXEL_ORI		0	// original

// define the row/column mode
#define	INTERVAL_NO		0
#define	INTERVAL_2		1 	// 1/2 row/column capture
#define	INTERVAL_4		2	// 1/4 row/column capture
#define	INTERVAL_8		3	// 1/8 row/column capture

// define the row/column mode
#define FIFOFULL_INT    	(1L<<0)  
#define FIFOEMPTY_INT    	(1L<<1) 
#define VSTART_INT    		(1L<<8)
#define VEND_INT    		(1L<<9)
#define HSTART_INT    		(1L<<16)
#define HEND_INT    		(1L<<17)
#define FIFOCNT_INT    		(1L<<24)


/***************************************************************************************************
** Global Functon Declaration
***************************************************************************************************/
UINT8 LOCSC_Init(UINT8 nVsyncPol, UINT8 nHsyncPol, UINT8 nPclkPol, UINT8 DataBits, UINT32 nIntType);
UINT8 LOCSC_GetImage(UINT8 nPixelInv, UINT8 nColMode, UINT8 nRowMode, UINT16 nStCol, UINT16 nStRow, 
					UINT16 nCol, UINT16 nRow, UINT32 *pImage);
UINT8 LOCSC_GetImageWithDMA(UINT8 nPixelInv, UINT8 nColMode, UINT8 nRowMode, UINT16 nStCol, UINT16 nStRow, 
					UINT16 nCol, UINT16 nRow, UINT32 *pImage, UINT8 nDmaChn);

#endif /*LOCSC_H_*/
