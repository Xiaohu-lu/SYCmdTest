/***************************************************************************************
** File name    : Locsc.c
** Function     : Get an image from CMOS sensor using LOCSC 		
** Author       : 
** Date         : 2014/10/11
** Version      : v1.0          
** Description  :                       
** ModifyRecord :    	
*****************************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09.h"

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

/**************************************************************************
* Local Functon Declaration
***************************************************************************/



/**************************************************************************
* Functon
***************************************************************************/


/***********************************************************************************
 * Subroutine:	LOCSC_Init
 * Function:	Initialize the LOSCS port with CMOS sensor signals' polarity
 * Input:		UINT8 nVsyncPol -- to specify the VSYNC signal polarity, VSYNC_ACTIVE_HIGH or VSYNC_ACTIVE_LOW
 *				UINT8 nHsyncPol -- to specify the HSYNC signal polarity, HSYNC_ACTIVE_HIGH or HSYNC_ACTIVE_LOW
 *				UINT8 nPclkPol -- to specify the PCLK signal polarity, PCLK_CAPTURE_RISING or PCLK_CAPTURE_FALLING
 *				UINT8 DataBits -- to specify LOCSC_D gpio number 
 *				UINT32 nIntType	-- to specify the interrupt
 * Output:		UINT8	--  Return result, RT_OK
 * Description:
 * Date:		2015/03/10
 * ModifyRecord:  
 * ********************************************************************************/
UINT8 LOCSC_Init(UINT8 nVsyncPol, UINT8 nHsyncPol, UINT8 nPclkPol, UINT8 DataBits, UINT32 nIntType)
{
	SCM_EnableModClk(ID_LOCSC);		// enable  clock
	SCM_ReleaseModRst(ID_LOCSC);	// Relinquish from reset

    switch(DataBits)
	{
		case 8:
            GpioRegs.GPAUSE0.bit.GPA0 = MUX_LOCSC;		//LOCSC_D0
        case 7:
            GpioRegs.GPAUSE0.bit.GPA1 = MUX_LOCSC;		//LOCSC_D1
        case 6:
            GpioRegs.GPAUSE0.bit.GPA2 = MUX_LOCSC;		//LOCSC_D2
        case 5:
            GpioRegs.GPAUSE0.bit.GPA3 = MUX_LOCSC;		//LOCSC_D3
        case 4:
            GpioRegs.GPAUSE0.bit.GPA4 = MUX_LOCSC;		//LOCSC_D4
        case 3:
           	GpioRegs.GPAUSE0.bit.GPA5 = MUX_LOCSC;		//LOCSC_D5
        case 2:
            GpioRegs.GPAUSE0.bit.GPA6 = MUX_LOCSC;		//LOCSC_D6
        case 1:
            GpioRegs.GPAUSE0.bit.GPA7 = MUX_LOCSC;		//LOCSC_D7
        default:
            break;
	}
	GpioRegs.GPAUSE1.bit.GPA8 = MUX_LOCSC;		//LOCSC_PCLK
	GpioRegs.GPAUSE1.bit.GPA9 = MUX_LOCSC;		//LOCSC_HSYNC
	GpioRegs.GPAUSE1.bit.GPA10 = MUX_LOCSC;		//LOCSC_VSYNC

    LocscRegs.CTRL.bit.VActive = nVsyncPol;
    LocscRegs.CTRL.bit.HActive = nHsyncPol;
    LocscRegs.CTRL.bit.PEdge = nPclkPol;
	if(nIntType != 0)
	{
		LocscRegs.INT_EN.all = nIntType;
		NVIC_EnableIRQ(LOCSC_IRQn);	// interrupt enabled
	}

	return RT_OK;	
}


/***********************************************************************************
 * Subroutine:	LOCSC_GetImage
 * Function:	get a image
 * Input:		UINT8 nPixelInv -- to specify if the pixel inverse or not, PIXEL_INV
 *				UINT8 nColMode -- to specify colum capture mode, NO_INTERVAL, INTERVAL_2, INTERVAL_4, INTERVAL_8
 *				UINT8 nRowMode -- to specify row capture mode, NO_INTERVAL, INTERVAL_2, INTERVAL_4, INTERVAL_8
 *				UINT16 nStCol --  to specify the start pixel in a row for capture
 *				UINT16 nStRow --  to specify the start row in a frame for capture
 *				UINT16 nCol -- to specify the number of column (width)
 *				UINT16 nRow -- to specify the number of row (height)
 *				UINT32 *pImage -- to specify the buffer for image data (4-byte-align) 
 * Output:		UINT8	--  Return result, RT_OK, RT_FAIL, RT_OVERTIME
 * Description:
 * Date:		2015/03/10
 * ModifyRecord:  
 * ********************************************************************************/
UINT8 LOCSC_GetImage(UINT8 nPixelInv, UINT8 nColMode, UINT8 nRowMode, UINT16 nStCol, UINT16 nStRow, 
					UINT16 nCol, UINT16 nRow, UINT32 *pImage)
{ 
	UINT32 nLen = ((UINT32)nCol * nRow)/4;		//  Total pixels in word
	UINT32 nTimeCnt = 0;
	UINT8 nFifoCnt = 0;
	LocscRegs.CTRL.all = (LocscRegs.CTRL.all & 0xF) | ((UINT32)nColMode<<16) | ((UINT32)nRowMode<<8) | ((UINT32)nPixelInv<<24);
	LocscRegs.PCOUNTER.all=((UINT32)nCol<<16|nStCol);
	LocscRegs.HCOUNTER.all=((UINT32)nRow<<16|nStRow);

	LocscRegs.CTRL.bit.VEn = 1;	
	while(nLen)
    {
		nFifoCnt = LocscRegs.FIFO_CNT;
		if(nFifoCnt>nLen) nFifoCnt = nLen;
		nLen -= nFifoCnt;
		while(nFifoCnt--)
		{
			*pImage++ = LocscRegs.FIFO;
		}
		
		if(LocscRegs.CTRL.bit.VEn==0)
        {
			break;
       	}
		if (nTimeCnt++>8000000)   
	    {
	        return RT_OVERTIME;
	    }           
    }
	if(nLen == 0) return RT_OK;
	else return RT_FAIL;
}


/***********************************************************************************
 * Subroutine:	LOCSC_GetImageWithDMA
 * Function:	get a image with DMA
 * Input:		UINT8 nPixelInv -- to specify if the pixel inverse or not, PIXEL_INV
 *				UINT8 nColMode -- to specify colum capture mode, NO_INTERVAL, INTERVAL_2, INTERVAL_4, INTERVAL_8
 *				UINT8 nRowMode -- to specify row capture mode, NO_INTERVAL, INTERVAL_2, INTERVAL_4, INTERVAL_8
 *				UINT16 nStCol --  to specify the start pixel in a row for capture
 *				UINT16 nStRow --  to specify the start row in a frame for capture
 *				UINT16 nCol -- to specify the number of column (width)
 *				UINT16 nRow -- to specify the number of row (height)
 *				UINT32 *pImage -- to specify the buffer for image data (4-byte-align) 
 *				UINT8 nDmaChn -- to specify the DMA channel
 * Output:		UINT8	--  Return result, RT_OK, RT_FAIL, RT_OVERTIME
 * Description:
 * Date:		2015/03/10
 * ModifyRecord:  
 * ********************************************************************************/
UINT8 LOCSC_GetImageWithDMA(UINT8 nPixelInv, UINT8 nColMode, UINT8 nRowMode, UINT16 nStCol, UINT16 nStRow, 
					UINT16 nCol, UINT16 nRow, UINT32 *pImage, UINT8 nDmaChn)
{ 
	UINT32 nLen = ((UINT32)nCol * nRow)/4;		//  Total pixels in word
	UINT32 nDmaTs;
	UINT8 nRet;

	STRUCT_DMA_DEV sSrcDev = {0, FIX, BYTE_4, BURST_1, HARD};
	STRUCT_DMA_DEV sDestDev = {0, INC, BYTE_4, BURST_1, HARD};

	LocscRegs.CTRL.all = (LocscRegs.CTRL.all & 0xF) | ((UINT32)nColMode<<16) | ((UINT32)nRowMode<<8) | ((UINT32)nPixelInv<<24);
	LocscRegs.PCOUNTER.all=((UINT32)nCol<<16|nStCol);
	LocscRegs.HCOUNTER.all=((UINT32)nRow<<16|nStRow);
	LocscRegs.INT_CNT = 1;

   	sSrcDev.nAddr = (UINT32)&LocscRegs.FIFO;
	sDestDev.nAddr = (UINT32)pImage;
	DMA_Init(nDmaChn, nDmaChn, &sSrcDev, &sDestDev, 0, NULL);

	LocscRegs.CTRL.bit.VEn = 1;	
	while(nLen)
    {
		nDmaTs = (nLen>DMA_MAX_TRAN_SIZE)? DMA_MAX_TRAN_SIZE : nLen;
		DMA_Start(nDmaChn, sSrcDev.nAddr, (UINT32)pImage, nDmaTs); 
		nRet = DMA_CheckDone(nDmaChn, 10000000); 
		if(nRet!=RT_OK) return nRet;
		pImage += nDmaTs;
		nLen -= nDmaTs;
    }
	return RT_OK;
}



   

