/***************************************************************************
* File name    :	SY09_Device.h
* Function     :	Header of SY09 device to include all register struct
* 					and data type header
* Author       : 	coins
* Date         :	2015/05/25
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
#ifndef SY09_DEVICE_H_
#define SY09_DEVICE_H_

/**************************************************************************
* Global Macro Definition
***************************************************************************/

/* locate variable at ABS_ADDR */
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
	#define	__AT(ABS_ADDR)	__attribute__((at(ABS_ADDR)))	
#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
	#define	__AT(ABS_ADDR)	@ (ABS_ADDR)					
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif

/* non-initialization variable keyword */
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
	#define	__NO_INIT
#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
	#define	__NO_INIT	__no_init					
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif

/* Compiler Related Definitions */
#if defined (__CC_ARM)                  /* ARM Compiler */
    #define ALIGN(n)                    __attribute__((aligned(n)))
#elif defined (__ICCARM__)     			/* for IAR Compiler */
    #define ALIGN(n)                    PRAGMA(data_alignment=n)
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	
#endif /* Compiler Related Definitions */

/* Configuration of the Cortex-M3 Processor and Core Peripherals */
#define __MPU_PRESENT				1		/*!< devices provide an MPU      					*/
#define __CM3_REV					0x0200	/*!< Core Revision r2p0                           */
#define __NVIC_PRIO_BITS			3		/*!< uses 3 Bits for the Priority Levels    		*/
#define __Vendor_SysTickConfig		0		/*!< Set to 1 if different SysTick Config is used */

/* HFROSC output clock frequency definition */
#define	F_HFROSC_0		12000000UL		/* Grade = 0 */
#define	F_HFROSC_1		48000000UL		/* Grade = 1 */
#define	F_HFROSC_2		96000000UL		/* Grade = 2 */
#define	F_HFROSC_3		144000000UL		/* Grade = 3 */
#define	F_HFROSC		F_HFROSC_2		/* HFROSC default output frequency */

/* LFROSC output clock frequency definition */
#define	F_LFROSC		32000UL	

//--------------------------------------------------------------------------
// define the peripheral ID
//--------------------------------------------------------------------------
#define	ID_SCM		0
#define	ID_GPIO		1
#define	ID_T0		2
#define	ID_SEA		3
#define	ID_AEA		4
#define	ID_SPI0		5
#define	ID_TRNG		6
#define	ID_USB		7
#define	ID_T1		8
#define	ID_T2		9
#define	ID_SSI		10
#define	ID_UART0	11
#define	ID_I2C0		12
#define	ID_SWPS		13
#define	ID_RSVD14	14
#define	ID_ISO7816	15
#define	ID_SCI		16
#define	ID_FPA		17
#define	ID_VPWM		18
#define	ID_UART1	19
#define	ID_HASH		20
#define	ID_EFC		21
#define	ID_SQI		22
#define	ID_CRC		23
#define	ID_BVD		24
#define	ID_RSVD25	25
#define	ID_LOCSC	26
#define	ID_SPI1		27
#define	ID_DMA		28

// -------------------------------------------------------------------------
// define the memory-map-register base address
#define	MMR_BASE		0x40000000
#define	APB_BASE		MMR_BASE
#define	AHB_BASE		(MMR_BASE + 0x10000000)
#define	APB_SPACE		0x4000
#define	AHB_SPACE		0x10000

#define	APB_PERI_BASE(id)	(APB_BASE + APB_SPACE * (id))	
#define	AHB_PERI_BASE(id)	(AHB_BASE + AHB_SPACE * (id))		

// APB
#define	SCM_BASE		APB_PERI_BASE(ID_SCM)
#define	GPIO_BASE		APB_PERI_BASE(ID_GPIO)
#define	T0_BASE			APB_PERI_BASE(ID_T0)
#define	SEA_BASE		APB_PERI_BASE(ID_SEA)
#define	AEA_BASE		APB_PERI_BASE(ID_AEA)
#define	SPI0_BASE		APB_PERI_BASE(ID_SPI0)
#define	TRNG_BASE		APB_PERI_BASE(ID_TRNG)

#define	T1_BASE			APB_PERI_BASE(ID_T1)
#define	T2_BASE			APB_PERI_BASE(ID_T2)
#define	SSI_BASE		APB_PERI_BASE(ID_SSI)
#define	UART0_BASE		APB_PERI_BASE(ID_UART0)
#define	I2C0_BASE		APB_PERI_BASE(ID_I2C0)
#define	SWPS_BASE		APB_PERI_BASE(ID_SWPS)

#define	ISO7816_BASE	APB_PERI_BASE(ID_ISO7816)
#define	SCI_BASE		APB_PERI_BASE(ID_SCI)
#define	FPA_BASE		APB_PERI_BASE(ID_FPA)
#define	VPWM_BASE		APB_PERI_BASE(ID_VPWM)
#define	UART1_BASE		APB_PERI_BASE(ID_UART1)
#define	DMA_BASE		APB_PERI_BASE(ID_DMA)
#define	EFC_BASE		APB_PERI_BASE(ID_EFC)
#define	SQI_BASE		APB_PERI_BASE(ID_SQI)
#define	CRC_BASE		APB_PERI_BASE(ID_CRC)
#define	BVD_BASE		APB_PERI_BASE(ID_BVD)
#define	RSVD_BASE		APB_PERI_BASE(ID_RSVD)
#define	LOCSC_BASE		APB_PERI_BASE(ID_LOCSC)
#define	SPI1_BASE		APB_PERI_BASE(ID_SPI1)
#define	HASH_BASE		APB_PERI_BASE(ID_HASH)
// AHB
#define	USB_BASE		AHB_PERI_BASE(1)	


/*General Return Code */
#define RT_OK            0x00 //success
#define RT_FAIL          0x01  //fail
#define RT_COMMAND_ERR   0x02  //command error
#define RT_PARAM_ERR     0x03  //param error
#define RT_OVERTIME      0x04  //over time
#define RT_ECC_ERR       0x05  //ecc error
#define RT_WRITE_ERR     0x06  //write flash err
#define RT_READ_ERR      0x07  //read flash err

// define BOOL value
#define FALSE	0
#define TRUE	1
#define false 	0
#define true  	1

#ifndef NULL
#define NULL 	0
#endif


//the function of GPIO to be configured
//the value from 0 to 15, but not all mode are used
#ifndef _GPIO_MODE_
#define _GPIO_MODE_
#define INPUT     			0
#define FUNC1				1
#define FUNC2				2
#define FUNC3				3
#define OUTPUT    			8
#define	INT_RISE_EDGE		12
#define INT_FALL_EDGE		13

// Define the pheriphals' MUX 
#define MUX_SPI         FUNC1
#define MUX_I2C         FUNC3
#define MUX_UART        FUNC1
#define MUX_SQI         FUNC1
#define	MUX_TMR			FUNC1
#define MUX_SWD        	FUNC1
#define MUX_SSI         FUNC2
#define	MUX_SCI			FUNC1
#define	MUX_ISO7816		FUNC1
#define	MUX_SWPS		FUNC1
#define	MUX_VPWM		FUNC1
#define MUX_CLKOUT      FUNC1
#define	MUX_SDIN		FUNC1
#define	MUX_LOCSC		FUNC2
#endif	

#define	_SCM_SetSysClk 		((FP_SCM_SET_SYSCLK)0x000013fd)
#define	_SCM_LoadHFRoscTrim	((FP_SCM_LOAD_HFROSC_TRIM)0x00001439)
#define	_EFC_Program		((FP_EFC_PROGRAM)0x00000893)
#define	_EFC_IsEraseOK		((FP_EFC_ISERASEOK)0x00000a3d)
#define	_EFC_Erase			((FP_EFC_ERASE)0x00000a07) //
#define	_EFC_PageProg		((FP_EFC_PAGE_PROG)0x00000c9f)
#define	_EFC_ConfigTiming	((FP_EFC_CONFIG_TIMING)0x0000073d)
#define	_EFC_WriteInfo		((FP_FLASH_WRITE_INFO)0x00000d45)
#define	_EFC_ChipErase		((EFC_CHIP_ERASE)0x00000c1b)

#define	_SQI_PageProg		((FP_SQI_PAGE_PROG)0x0000107b)
#define	_SQI_Init			((SQI_INIT)0x000011b3)
#define	_SQI_ReadInfo		((SQI_READ_INFO)0x000013a7)
#define	_SQI_WriteInfo		((SQI_WRITE_INFO)0x0000131d)	

/**************************************************************************
* Global Type Definition
***************************************************************************/
/* Firmware Library General Type */
//------- BOOL type definition --------------
#ifndef	_BOOL_TYPE_
#define	_BOOL_TYPE_
typedef	unsigned char BOOL;
typedef	unsigned char bool;
#endif

//------- UINT8 type definition --------------
#ifndef	_UINT8_TYPE_
#define _UINT8_TYPE_
typedef unsigned char UINT8;
#endif

//------- UINT16 type definition --------------
#ifndef	_UINT16_TYPE_
#define _UINT16_TYPE_
typedef unsigned short UINT16;
#endif

//------- UINT32 type definition --------------
#ifndef	_UINT32_TYPE_
#define _UINT32_TYPE_
typedef unsigned long UINT32;
#endif

//------- UINT64 type definition --------------
#ifndef	_UINT64_TYPE_
#define _UINT64_TYPE_
typedef unsigned long long UINT64;
#endif

//-------- INT8 type definition --------------
#ifndef	_INT8_TYPE_
#define _INT8_TYPE_
typedef signed char  INT8;
#endif

//-------- INT16 type definition --------------
#ifndef	_INT16_TYPE_
#define _INT16_TYPE_
typedef signed short INT16;
#endif

//-------- INT32 type definition --------------
#ifndef	_INT32_TYPE_
#define _INT32_TYPE_
typedef signed long INT32;
#endif

//------- INT64 type definition --------------
#ifndef	_INT64_TYPE_
#define _INT64_TYPE_
typedef signed long long INT64;
#endif

//------- VUINT8 type definition --------------
#ifndef	_VUINT8_TYPE_
#define _VUINT8_TYPE_
typedef volatile unsigned char VUINT8;
#endif

//------- VUINT16 type definition --------------
#ifndef	_VUINT16_TYPE_
#define _VUINT16_TYPE_
typedef volatile unsigned short VUINT16;
#endif

//------- VUINT32 type definition --------------
#ifndef	_VUINT32_TYPE_
#define _VUINT32_TYPE_
typedef volatile unsigned long VUINT32;
#endif

//------- VINT8 type definition --------------
#ifndef	_VINT8_TYPE_
#define _VINT8_TYPE_
typedef volatile signed char VINT8;
#endif

//------- VINT16 type definition --------------
#ifndef	_VINT16_TYPE_
#define _VINT16_TYPE_
typedef volatile signed short VINT16;
#endif

//------- VUINT32 type definition --------------
#ifndef	_VINT32_TYPE_
#define _VINT32_TYPE_
typedef volatile signed long VINT32;
#endif

// define the ROM function type
typedef void (*FP_EFC_PROGRAM)(UINT32 *pData, UINT32 addr_align, UINT32 nLen);
typedef void (*FP_EFC_ERASE)(UINT32 addr_offset, UINT32 page_num);
typedef BOOL (*FP_EFC_ISERASEOK)(UINT32 StrAddr, UINT32 EndAddr);
typedef void (*FP_EFC_PAGE_PROG)(UINT32 addr_offset, UINT32 *pData);
typedef void (*FP_EFC_CONFIG_TIMING)(UINT32 nTC0, UINT32 nTC1, UINT32 nTC2, UINT32 nTC3);
typedef UINT8 (*FP_SCM_SET_SYSCLK)(UINT8 nClk); 
typedef	UINT8 (*FP_SCM_LOAD_HFROSC_TRIM)(void);
typedef UINT8 (*FP_FLASH_WRITE_INFO)(UINT8 *pDataBuf,UINT32 lAddress,UINT32 lLen);
typedef void (*EFC_CHIP_ERASE)(UINT32 StrAddr, UINT32 EndAddr);

typedef UINT8 (*FP_SQI_PAGE_PROG)(UINT32 byte_addr, UINT32 *pData);
typedef UINT8 (*SQI_INIT)(UINT8 clk_div, UINT8 read_mode);
typedef UINT8 (*SQI_READ_INFO)(UINT8* pDataBuf,UINT32 nOffset,UINT16 nDataNum);
typedef UINT8 (*SQI_WRITE_INFO)(UINT8 *pDataBuf, UINT32 lAddress, UINT32 lLen);

/*
 * ==========================================================================
 * ---------- Interrupt Number Definition -----------------------------------
 * ==========================================================================
 */
typedef enum IRQn
{
/******  Cortex-M0 Processor Exceptions Numbers ***************************************************/
	NonMaskableInt_IRQn     = -14,    /*!< 2 Non Maskable Interrupt                             */
	HardFault_IRQn	      	= -13,    /*!< 3 Cortex-M0 Hard Fault Interrupt                     */
	SVCall_IRQn             = -5,     /*!< 11 Cortex-M0 SV Call Interrupt                       */
	PendSV_IRQn             = -2,     /*!< 14 Cortex-M0 Pend SV Interrupt                       */
	SysTick_IRQn            = -1,     /*!< 15 Cortex-M0 System Tick Interrupt                   */

/******  CM0IKMCU Cortex-M0 specific Interrupt Numbers ********************************************/
	SCM_IRQn                = ID_SCM,      /*!< SCM Interrupt                                       */
	                                  /*!< maximum of 32 Interrupts are possible                */
	GPIO_IRQn         		= ID_GPIO,
	T0_IRQn         		= ID_T0,
	SEA_IRQn         		= ID_SEA,
	AEA_IRQn         		= ID_AEA,
	SPI0_IRQn      			= ID_SPI0,
	TRNG_IRQn         		= ID_TRNG,
	USB_IRQn         		= ID_USB,
	T1_IRQn         		= ID_T1,
	T2_IRQn         		= ID_T2,
	SSI_IRQn         		= ID_SSI,
	UART0_IRQn         		= ID_UART0,
	I2C0_IRQn        		= ID_I2C0, 
	SWPS_IRQn         		= ID_SWPS,	
	ISO7816_IRQn         	= ID_ISO7816,
	SCI_IRQn         		= ID_SCI,
	FPA_IRQn         		= ID_FPA,
	VPWM_IRQn         		= ID_VPWM,
	UART1_IRQn         		= ID_UART1,
	HASH_IRQn         		= ID_HASH,
	EFC_IRQn         		= ID_EFC,
	LOCSC_IRQn         		= ID_LOCSC,
	SPI1_IRQn         		= ID_SPI1,
	DMA_IRQn         		= ID_DMA,
} IRQn_Type;

/*
 * ==========================================================================
 * ---------- Perpheral Registers Definition --------------------------------
 * ==========================================================================
 */

/*
*************************** SCM ******************************************
*/
// System Clock Control Register bit field
struct	SCM_SYSCLKCTRL_BITS	{
	UINT32	FclkSrc:2;		//1:0	
	UINT32	FclkSel:2;		//3:2
	UINT32  AutoClkSwitch:1;	//4
	UINT32	Rsvd1:19;		//23:5
	UINT32	UsbClk48mEn:1;	//24
	UINT32	UsbClk48mSel:1;	//25
	UINT32	Rsvd2:6;		//31:26
};
union	SCM_SYSCLKCTRL_REG	{
	UINT32	all;
	struct	SCM_SYSCLKCTRL_BITS	bit;
};

// System Reset Control Register bit field
struct	SCM_SYSRSTCTRL_BITS	{
	UINT32	IoBod:1;
	UINT32	IoVd:1;
	UINT32 	Rsvd1:6;
	UINT32	Td:1;	
	UINT32	Rsvd2:1;
	UINT32	Vdd18Vd:1;
	UINT32 	Vdd18Bod:1;
	UINT32	Rsvd3:4;
	UINT32	CoreVd:1;
	UINT32	Shield:1;
	UINT32	Rsvd4:6;
	UINT32	Sdi1:1;
	UINT32	Sdi0:1;
	UINT32	Sram:1;	
	UINT32	Rsvd5:5; 
};
union	SCM_SYSRSTCTRL_REG	{
	UINT32	all;
	struct	SCM_SYSRSTCTRL_BITS	bit;
};

// System Reset Status Register bit field
struct	SCM_SYSRSTSTATUS_BITS	{
	UINT32	IoBod:1;
	UINT32	IoVd:1;
	UINT32	Wdt:1;
	UINT32	Soft:1;
	UINT32 	Rsvd1:4;
	UINT32	Td:1;	
	UINT32	Rsvd2:1;
	UINT32	Vdd18Vd:1;
	UINT32	Vdd18Bod:1;
	UINT32	Rsvd3:4;
	UINT32	CoreVd:1;
	UINT32	Rsvd4:15;	 
};
union	SCM_SYSRSTSTATUS_REG	{
	UINT32	all;
	struct	SCM_SYSRSTSTATUS_BITS	bit;
};

// High Frequency ROSC Control Register bit field
struct	SCM_HFROSCCTRL_BITS	{
	UINT32 	HFROscCRE:1;
	UINT32 	Rsvd1:7;  			
	UINT32	HFROscTrim:10;
	UINT32	HFROscGrade:2;
	UINT32	TrimHLimit:10;
	UINT32	Rsvd2:2;
};
union	SCM_HFROSCCTRL_REG	{
	UINT32	all;
	struct	SCM_HFROSCCTRL_BITS	bit;
};

// TPR Control Register bit field
struct	SCM_TPRCTRL_BITS	{
	UINT32 	TDLTrim:3;
	UINT32 	Rsvd1:5;  			
	UINT32 	TDHTrim:3;
	UINT32 	Rsvd2:21;	
};
union	SCM_TPRCTRL_REG	{
	UINT32	all;
	struct	SCM_TPRCTRL_BITS	bit;
};

// Analog Module Power Enable Control Register bit field
struct	SCM_AMODENCTRL_BITS	{
	UINT32	Hfrosc:1;
	UINT32	Rsvd0:3;
	UINT32	Rom:1;
	UINT32	Rsvd:1;
	UINT32	TrngRing:1;
	UINT32	UsbPhy:1;
	UINT32	Tpr:1;
	UINT32	Rsvd1:7;
	UINT32	BvdAn:1;
	UINT32	Rsvd2:1; 
	UINT32	SWPPowerInLel:1;
	UINT32  AutoHFRoscPowerSwitch:1;
	UINT32	Rsvd3:12; 
};
union	SCM_AMODENCTRL_REG	{
	UINT32	all;
	struct	SCM_AMODENCTRL_BITS	bit;
};

// Clock Output Control Register bit field
struct	SCM_CLKOUTCTRL_BITS	{
	UINT32	ClkOutDiv:8;		
	UINT32	ClkOutEn:1;
	UINT32	Rsvd1:7;
	UINT32	ClkSqiDiv:7;		
	UINT32	ClkSqiDivEn:1;	
	UINT32	ClkSqiEn:1;
	UINT32	Rsvd2:7;
};
union	SCM_CLKOUTCTRL_REG	{
	UINT32	all;
	struct 	SCM_CLKOUTCTRL_BITS	bit;
};

// Active Shield Control Register bit field
struct	SCM_SHIELDCTRL_BITS	{
	UINT32	TbusDiv:4;	
	UINT32	Rsvd1:4;	
	UINT32	Active:1;
	UINT32	Rsvd:23;	
};
union	SCM_SHIELDCTRL_REG	{
	UINT32	all;
	struct 	SCM_SHIELDCTRL_BITS	bit;
};

// System Self-Destruct Control Register bit field	 
struct	SCM_SYSSDCTRL_BITS	{
	UINT32	Rsvd1:1;
	UINT32	IoVd:1;
	UINT32 	Rsvd2:6;
	UINT32	Td:1;
	UINT32	Vdd18Vd:1;	
	UINT32	Rsvd3:6;
	UINT32	CoreVd:1;
	UINT32	Shield:1;
	UINT32	Rsvd4:6;
	UINT32	Sdi0:1;
	UINT32	Sdi1:1;
	UINT32	Sram:1;
	UINT32	Rom:1;	
	UINT32	Rsvd7:4; 
};
union	SCM_SYSSDCTRL_REG	{
	UINT32	all;
	struct 	SCM_SYSSDCTRL_BITS	bit;
};

// USB PHY	 
struct	SCM_USBPHYCTRL_BITS	{
	UINT32	UsbPuRes:1;
	UINT32 	Rsvd1:6;
	UINT32	PhyMod:1;	
	UINT32	Rsvd2:24;
};
union	SCM_USBPHYCTRL_REG	{
	UINT32	all;
	struct 	SCM_USBPHYCTRL_BITS	bit;
};
// System Control Moudle Registers
struct	SCM_REGS{
	UINT32	SOCID;									//0x00
	UINT32	MODCLKCTRL;								//0x04
	UINT32	MODRSTCTRL;	 							//0x08
	union	SCM_SYSCLKCTRL_REG		SYSCLKCTRL;		//0x0C
	union	SCM_SYSRSTCTRL_REG		SYSRSTCTRL;		//0x10
	union	SCM_HFROSCCTRL_REG		HFROSCCTRL;		//0x14
	union	SCM_TPRCTRL_REG			TPRCTRL;		//0x18
	union	SCM_AMODENCTRL_REG		AMODENCTRL;		//0x1C
	UINT32	Rsvd1[1];								//0x20
	UINT32	MEMREMAP;
	union	SCM_CLKOUTCTRL_REG 		CLKOUTCTRL;		//0x28
	union	SCM_SYSRSTSTATUS_REG	SYSRSTSTATUS;	//0x2C
	UINT32	Rsvd2[1];								//0x30
	union	SCM_SHIELDCTRL_REG		SHIELDCTRL;		//0x34
	UINT32	SHIELDINITVAL;							// 0x38
	union	SCM_SYSSDCTRL_REG		SYSSDCTRL;		// 0x3C
	UINT32	Rsvd3[2];
	UINT32	REGWP0;									//0x48
	union	SCM_USBPHYCTRL_REG  	USBPHYCTRL;		//0x4C
	UINT32 SRAM_ADDRKEY;						//0x50
	UINT32 SRAM_DATAKEY;						//0x54
	UINT32 SRAM_EXT_INHIBIT;					//0x58
	UINT32 REGWP1;								//0x5C						
	UINT32 USB_INT_EN;							//0x60
	UINT32 USB_INT_CTRL;
	UINT32 USB_ADJ;
	UINT32 Rsvd4[1];							//0x6C
	UINT32 KEY_ACCESS_INHIBIT;					//0x70
	UINT32 Rsvd5[23];							//0x74
	UINT32 GPREGx[12];							//0xD0
};

/*
**************************** GPIO ******************************************
*/
// GPIOA DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPADATA_BITS	{
	UINT32	GPA0:1;
	UINT32	GPA1:1;
	UINT32	GPA2:1;
	UINT32	GPA3:1;
	UINT32	GPA4:1;
	UINT32	GPA5:1;
	UINT32	GPA6:1;
	UINT32	GPA7:1;
	UINT32	GPA8:1;
	UINT32	GPA9:1;
	UINT32	GPA10:1;
	UINT32	GPA11:1;
	UINT32	GPA12:1;
	UINT32	GPA13:1;
	UINT32	GPA14:1;
	UINT32	GPA15:1;
	UINT32	GPA16:1;
	UINT32	GPA17:1;
	UINT32	GPA18:1;
	UINT32	GPA19:1;
	UINT32	GPA20:1;
	UINT32	GPA21:1;
	UINT32	GPA22:1;
	UINT32	GPA23:1;
	UINT32	GPA24:1;
	UINT32	GPA25:1;
	UINT32	GPA26:1;
	UINT32	GPA27:1;
	UINT32	GPA28:1;
	UINT32	GPA29:1;
	UINT32	GPA30:1;
	UINT32	GPA31:1;
};
union	GPIO_GPADATA_REG	{
	UINT32	all;
	struct	GPIO_GPADATA_BITS	bit;
};

// GPIOA USE0 Bits Definition
struct	GPIO_GPAUSE0_BITS	{	// bits	description
	UINT32	GPA0:4;				//	3:0		Use of GPIOA0
	UINT32	GPA1:4;				//	7:4		Use of GPIOA1
	UINT32	GPA2:4;				//	11:8	Use of GPIOA2
	UINT32	GPA3:4;				//	15:12	Use of GPIOA3
	UINT32	GPA4:4;				//	19:16	Use of GPIOA4
	UINT32	GPA5:4;				//	23:20	Use of GPIOA5
	UINT32	GPA6:4;				//	27:24	Use of GPIOA6
	UINT32	GPA7:4;				//	31:28	Use of GPIOA7
};
union 	GPIO_GPAUSE0_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE0_BITS	bit;
};

//GPIOA USE1 Bits Definition
struct	GPIO_GPAUSE1_BITS	{	// bits	description
	UINT32	GPA8:4;				//	3:0		Use of GPIOA8
	UINT32	GPA9:4;				//	7:4		Use of GPIOA9
	UINT32	GPA10:4;			//	11:8	Use of GPIOA10
	UINT32	GPA11:4;			//	15:12	Use of GPIOA11
	UINT32	GPA12:4;			//	19:16	Use of GPIOA12
	UINT32	GPA13:4;			//	23:20	Use of GPIOA13
	UINT32	GPA14:4;			//	27:24	Use of GPIOA14
	UINT32	GPA15:4;			//	31:28	Use of GPIOA15
};
union 	GPIO_GPAUSE1_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE1_BITS	bit;
};

//GPIOA USE2 Bits Definition
struct	GPIO_GPAUSE2_BITS	{	// bits	description
	UINT32	GPA16:4;			//	3:0		Use of GPIOA16
	UINT32	GPA17:4;			//	7:4		Use of GPIOA17
	UINT32	GPA18:4;			//	11:8	Use of GPIOA18
	UINT32	GPA19:4;			//	15:12	Use of GPIOA19
	UINT32	GPA20:4;			//	19:16	Use of GPIOA20
	UINT32	GPA21:4;			//	23:20	Use of GPIOA21
	UINT32	GPA22:4;			//	27:24	Use of GPIOA22
	UINT32	GPA23:4;			//	31:28	Use of GPIOA23
};
union 	GPIO_GPAUSE2_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE2_BITS	bit;
};

//GPIOA USE3 Bits Definition
struct	GPIO_GPAUSE3_BITS	{	// bits	description
	UINT32	GPA24:4;			//	3:0		Use of GPIOA24
	UINT32	GPA25:4;			//	7:4		Use of GPIOA25
	UINT32	GPA26:4;			//	11:8	Use of GPIOA26
	UINT32	GPA27:4;			//	15:12	Use of GPIOA27
	UINT32	GPA28:4;			//	19:16	Use of GPIOA28
	UINT32	GPA29:4;			//	23:20	Use of GPIOA29
	UINT32	GPA30:4;			//	27:24	Use of GPIOA30
	UINT32	GPA31:4;			//	31:28	Use of GPIOA31
};
union 	GPIO_GPAUSE3_REG	{
	UINT32	all;
	struct	GPIO_GPAUSE3_BITS	bit;
};

//-------------- GPIOB --------------------------
//GPIOB DATA/DATA SET/DATA CLR/INT STA Bits Definition
struct	GPIO_GPBDATA_BITS	{
	UINT32	GPB0:1;
	UINT32	GPB1:1;
	UINT32	GPB2:1;
	UINT32	GPB3:1;
	UINT32	GPB4:1;
	UINT32	GPB5:1;
	UINT32	GPB6:1;
	UINT32	GPB7:1;
	UINT32	GPB8:1;
	UINT32	GPB9:1;
	UINT32	GPB10:1;
	UINT32	GPB11:1;
	UINT32	GPB12:1;
	UINT32	GPB13:1;
	UINT32	GPB14:1;
	UINT32	GPB15:1;
	UINT32	GPB16:1;
	UINT32	GPB17:1;
	UINT32	GPB18:1;
	UINT32	GPB19:1;
	UINT32	GPB20:1;
	UINT32	GPB21:1;
	UINT32	GPB22:1;
	UINT32	GPB23:1;
	UINT32	GPB24:1;
	UINT32	GPB25:1;
	UINT32	GPB26:1;
	UINT32	GPB27:1;
	UINT32	GPB28:1;
	UINT32	GPB29:1;
	UINT32	GPB30:1;
	UINT32	GPB31:1;
};
union	GPIO_GPBDATA_REG	{
	UINT32	all;
	struct	GPIO_GPBDATA_BITS	bit;
};

//GPIOB USE0 Bits Definition
struct	GPIO_GPBUSE0_BITS	{	// bits	description
	UINT32	GPB0:4;				//	3:0		Use of GPIOB0
	UINT32	GPB1:4;				//	7:4		Use of GPIOB1
	UINT32	GPB2:4;				//	11:8	Use of GPIOB2
	UINT32	GPB3:4;				//	15:12	Use of GPIOB3
	UINT32	GPB4:4;				//	19:16	Use of GPIOB4
	UINT32	GPB5:4;				//	23:20	Use of GPIOB5
	UINT32	GPB6:4;				//	27:24	Use of GPIOB6
	UINT32	GPB7:4;				//	31:28	Use of GPIOB7
};
union 	GPIO_GPBUSE0_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE0_BITS	bit;
};

//GPIOB USE1 Bits Definition
struct	GPIO_GPBUSE1_BITS	{	// bits	description
	UINT32	GPB8:4;				//	3:0		Use of GPIOB8
	UINT32	GPB9:4;				//	7:4		Use of GPIOB9
	UINT32	GPB10:4;			//	11:8	Use of GPIOB10
	UINT32	GPB11:4;			//	15:12	Use of GPIOB11
	UINT32	GPB12:4;			//	19:16	Use of GPIOB12
	UINT32	GPB13:4;			//	23:20	Use of GPIOB13
	UINT32	GPB14:4;			//	27:24	Use of GPIOB14
	UINT32	GPB15:4;			//	31:28	Use of GPIOB15
};
union 	GPIO_GPBUSE1_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE1_BITS	bit;
};

//GPIOB USE2 Bits Definition
struct	GPIO_GPBUSE2_BITS	{	// bits	description
	UINT32	GPB16:4;			//	3:0		Use of GPIOB16
	UINT32	GPB17:4;			//	7:4		Use of GPIOB17
	UINT32	GPB18:4;			//	11:8	Use of GPIOB18
	UINT32	GPB19:4;			//	15:12	Use of GPIOB19
	UINT32	GPB20:4;			//	19:16	Use of GPIOB20
	UINT32	GPB21:4;			//	23:20	Use of GPIOB21
	UINT32	GPB22:4;			//	27:24	Use of GPIOB22
	UINT32	GPB23:4;			//	31:28	Use of GPIOB23
};
union 	GPIO_GPBUSE2_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE2_BITS	bit;
};

//GPIOB USE3 Bits Definition
struct	GPIO_GPBUSE3_BITS	{	// bits	description
	UINT32	GPB24:4;			//	3:0		Use of GPIOB24
	UINT32	GPB25:4;			//	7:4		Use of GPIOB25
	UINT32	GPB26:4;			//	11:8	Use of GPIOB26
	UINT32	GPB27:4;			//	15:12	Use of GPIOB27
	UINT32	GPB28:4;			//	19:16	Use of GPIOB28
	UINT32	GPB29:4;			//	23:20	Use of GPIOB29
	UINT32	GPB30:4;			//	27:24	Use of GPIOB30
	UINT32	GPB31:4;			//	31:28	Use of GPIOB31
};
union 	GPIO_GPBUSE3_REG	{
	UINT32	all;
	struct	GPIO_GPBUSE3_BITS	bit;
};

// GPIOA DRIVE0 Bits Definition
struct	GPIO_GPADRIVE0_BITS	{	// bits	description
	UINT32	GPA0:2;				//	1:0		Use of GPIOA0
	UINT32	GPA1:2;				//	3:2		Use of GPIOA1
	UINT32	GPA2:2;				//	5:4		Use of GPIOA2
	UINT32	GPA3:2;				//	7:6		Use of GPIOA3
	UINT32	GPA4:2;				//	9:8		Use of GPIOA4
	UINT32	GPA5:2;				//	11:10	Use of GPIOA5
	UINT32	GPA6:2;				//	13:12	Use of GPIOA6
	UINT32	GPA7:2;				//	15:14	Use of GPIOA7
	UINT32	GPA8:2;				//	17:16	Use of GPIOA8
	UINT32	GPA9:2;				//	19:18	Use of GPIOA9
	UINT32	GPA10:2;			//	21:20	Use of GPIOA10
	UINT32	GPA11:2;			//	23:22	Use of GPIOA11
	UINT32	GPA12:2;			//	25:24	Use of GPIOA12
	UINT32	GPA13:2;			//	27:26	Use of GPIOA13
	UINT32	GPA14:2;			//	29:28	Use of GPIOA14
	UINT32	GPA15:2;			//	31:30	Use of GPIOA15
};
union 	GPIO_GPADRIVE0_REG	{
	UINT32	all;
	struct	GPIO_GPADRIVE0_BITS	bit;
};

// GPIOA DRIVE1 Bits Definition
struct	GPIO_GPADRIVE1_BITS	{	// bits	description
	UINT32	GPA16:2;			//	1:0		Use of GPIOA16
	UINT32	GPA17:2;			//	3:2		Use of GPIOA17
	UINT32	GPA18:2;			//	5:4		Use of GPIOA18
	UINT32	GPA19:2;			//	7:6		Use of GPIOA19
	UINT32	GPA20:2;			//	9:8		Use of GPIOA20
	UINT32	GPA21:2;			//	11:10	Use of GPIOA21
	UINT32	GPA22:2;			//	13:12	Use of GPIOA22
	UINT32	GPA23:2;			//	15:14	Use of GPIOA23
	UINT32	GPA24:2;			//	17:16	Use of GPIOA24
	UINT32	GPA25:2;			//	19:18	Use of GPIOA25
	UINT32	GPA26:2;			//	21:20	Use of GPIOA26
	UINT32	GPA27:2;			//	23:22	Use of GPIOA27
	UINT32	GPA28:2;			//	25:24	Use of GPIOA28
	UINT32	GPA29:2;			//	27:26	Use of GPIOA29
	UINT32	GPA30:2;			//	29:28	Use of GPIOA30
	UINT32	GPA31:2;			//	31:30	Use of GPIOA31
};
union 	GPIO_GPADRIVE1_REG	{
	UINT32	all;
	struct	GPIO_GPADRIVE1_BITS	bit;
};

// GPIOB DRIVE0 Bits Definition
struct	GPIO_GPBDRIVE0_BITS	{	// bits	description
	UINT32	GPB0:2;				//	1:0		Use of GPIOB0
	UINT32	GPB1:2;				//	3:2		Use of GPIOB1
	UINT32	GPB2:2;				//	5:4		Use of GPIOB2
	UINT32	GPB3:2;				//	7:6		Use of GPIOB3
	UINT32	GPB4:2;				//	9:8		Use of GPIOB4
	UINT32	GPB5:2;				//	11:10	Use of GPIOB5
	UINT32	GPB6:2;				//	13:12	Use of GPIOB6
	UINT32	GPB7:2;				//	15:14	Use of GPIOB7
	UINT32	GPB8:2;				//	17:16	Use of GPIOB8
	UINT32	GPB9:2;				//	19:18	Use of GPIOB9
	UINT32	GPB10:2;			//	21:20	Use of GPIOB10
	UINT32	GPB11:2;			//	23:22	Use of GPIOB11
	UINT32	GPB12:2;			//	25:24	Use of GPIOB12
	UINT32	GPB13:2;			//	27:26	Use of GPIOB13
	UINT32	GPB14:2;			//	29:28	Use of GPIOB14
	UINT32	GPB15:2;			//	31:30	Use of GPIOB15
};
union 	GPIO_GPBDRIVE0_REG	{
	UINT32	all;
	struct	GPIO_GPBDRIVE0_BITS	bit;
};

// GPIOB DRIVE1 Bits Definition
struct	GPIO_GPBDRIVE1_BITS	{	// bits	description
	UINT32	GPB16:2;			//	1:0		Use of GPIOB16
	UINT32	GPB17:2;			//	3:2		Use of GPIOB17
	UINT32	GPB18:2;			//	5:4		Use of GPIOB18
	UINT32	GPB19:2;			//	7:6		Use of GPIOB19
	UINT32	GPB20:2;			//	9:8		Use of GPIOB20
	UINT32	GPB21:2;			//	11:10	Use of GPIOB21
	UINT32	GPB22:2;			//	13:12	Use of GPIOB22
	UINT32	GPB23:2;			//	15:14	Use of GPIOB23
	UINT32	GPB24:2;			//	17:16	Use of GPIOB24
	UINT32	GPB25:2;			//	19:18	Use of GPIOB25
	UINT32	GPB26:2;			//	21:20	Use of GPIOB26
	UINT32	GPB27:2;			//	23:22	Use of GPIOB27
	UINT32	GPB28:2;			//	25:24	Use of GPIOB28
	UINT32	GPB29:2;			//	27:26	Use of GPIOB29
	UINT32	GPB30:2;			//	29:28	Use of GPIOB30
	UINT32	GPB31:2;			//	31:30	Use of GPIOB31
};
union 	GPIO_GPBDRIVE1_REG	{
	UINT32	all;
	struct	GPIO_GPBDRIVE1_BITS	bit;
};

// GPIO Registers
struct	GPIO_REGS	{
	//-------- GPIOA --------------
	union 	GPIO_GPAUSE0_REG	GPAUSE0;	// GPIOA use0 register
	union 	GPIO_GPAUSE1_REG	GPAUSE1;	// GPIOA use1 register
	union 	GPIO_GPAUSE2_REG	GPAUSE2;	// GPIOA use2 register
	union 	GPIO_GPAUSE3_REG	GPAUSE3;	// GPIOA use3 register
	union 	GPIO_GPADATA_REG	GPADATA;	// GPIOA data register
	union 	GPIO_GPADATA_REG	GPASET;		// GPIOA data set register
	union 	GPIO_GPADATA_REG 	GPACLR;		// GPIOA data clear register
	union 	GPIO_GPADATA_REG	GPATOG;		// GPIOA data toggle register
	union 	GPIO_GPADATA_REG	GPAINT;		// GPIOA interrupt flag and clear register
	union 	GPIO_GPADATA_REG	GPAPE;		// GPIOA Pull Enable
	union 	GPIO_GPADRIVE0_REG	GPADRIVE0;	//GPIOA Dirve control	register
	union 	GPIO_GPADRIVE1_REG 	GPADRIVE1;	//GPIOA Dirve control	register 
//	UINT32 	RsvdA1[2];
	//union GPIO_GPADATA_REG	GPAPAD;		// GPIOA PAD pull up/down control register
	//--------- GPIOB -------------
	union 	GPIO_GPBUSE0_REG	GPBUSE0;	// GPIOB use0 register
	union 	GPIO_GPBUSE1_REG	GPBUSE1;	// GPIOB use1 register
	union 	GPIO_GPBUSE2_REG	GPBUSE2;	// GPIOB use2 register
	union 	GPIO_GPBUSE3_REG	GPBUSE3;	// GPIOB use3 register
	union 	GPIO_GPBDATA_REG	GPBDATA;	// GPIOB data register
	union 	GPIO_GPBDATA_REG	GPBSET;		// GPIOB data set register
	union 	GPIO_GPBDATA_REG 	GPBCLR;		// GPIOB data clear register
	union 	GPIO_GPBDATA_REG	GPBTOG;		// GPIOB data toggle register
	union 	GPIO_GPBDATA_REG	GPBINT;		// GPIOB interrupt flag and clear register
	union 	GPIO_GPBDATA_REG	GPBPE;		// GPIOB Pull Enable
	union 	GPIO_GPBDRIVE0_REG 	GPBDRIVE0;	//GPIOB Dirve control register
	union 	GPIO_GPBDRIVE1_REG 	GPBDRIVE1;	//GPIOB Dirve control register
//	UINT32 	RsvdB1[2];
	//union GPIO_GPBDATA_REG	GPBPAD;		// GPIOB PAD pull up/down control register
	//--------- GPIOC -------------
	UINT32 	RsvdC[12];
	//---------- GPIOD -----------
	UINT32 	RsvdD[12];
	//---------- GPIOE -----------
	UINT32 	RsvdE[12];
};

/*
*************************** TMR ******************************************
*/
// Define bits of TIMER_CONTROL
#define	CONTROL_WKEY	(0x67<<24)
#define CONTROL_WUFLAG	(0x55<<16)

#define	CONTROL_CLKSEL	6
#define CONTROL_EN		4
#define CONTROL_IP		3
#define CONTROL_WDT		2
#define CONTROL_IE		0
// Timer Control Register Bit Definition
struct	TIMER_CONTROL_BITS	{// bits	description
	UINT32	Ie:1;			
	UINT32	Rsvd0:1;			
	UINT32	Wdt:1;		
	UINT32	Ip:1;		
	UINT32	En:1;		
	UINT32	Rsvd1:1;		
	UINT32	ClkSel:2;		
	UINT32	Rsvd2:8;
	UINT32  Wuflag:8;
	UINT32	Wkey:8;		
};
union 	TIMER_CONTROL_REG	{
	UINT32	all;
	struct	TIMER_CONTROL_BITS	bit;
};

// Timer Registers
struct TMR_REGS	{
	UINT32	COUNT;
	union	TIMER_CONTROL_REG CONTROL;
	UINT32	LIMIT;
};

/*
*************************** USART ******************************************
*/
// USART Current Status Register Bit Definitions
struct	USART_CSTAT_BITS	{	//	bits	description
	UINT32	TxEpt:1;			//  0		Transmitter empty
	UINT32	RxAvl:1;			//	1		Receive available byte data message
	UINT32	TxFull:1;			//	2		Transmitter FIFO full status
	UINT32	Rsvd1:9;			//	11:3	Reserved
	UINT32	Si2cDa:1;			//	12		Data/address (I2C slave only)
	UINT32	Si2cRw:1;			// 	13		Read/Write info. (I2C slave only)
	UINT32	Si2cNack:1;			//	14		I2C acknowledge (I2C slave only)
	UINT32	Si2cBusy:1;			//  15		I2C bus busy (I2C slave only)
	UINT32	Rsvd2:16;			//	31:16	Reserved
};
union	USART_CSTAT_REG		{
	UINT32	all;
	struct	USART_CSTAT_BITS	bit;
};

// USART Interrupt Status Register Bit Definitions
struct	USART_INTSTAT_BITS	{	//	bits	dedcription
	UINT32	TxIntf:1;			//	0		Transmitter FIFO empty interrupt flag
	UINT32	RxIntf:1;			//	1		Receiver data available interrupt flag
	UINT32	UnderRunIntf:1;		//	2		SPI slave transmitter underrun flag (SPI slave only)
	UINT32	RxOerrIntf:1;		//  3		Receiver overrun error interrupt flag
	UINT32	RxPerrIntf:1;		//	4		Parity check error interrupt flag (UART only)
	UINT32	RxFerrIntf:1;		//  5		Framing error interrupt flag (UART only)
	UINT32	RxBrkIntf:1;		//  6		Break condition interrupt flag (UART only)
	UINT32	NackIntf:1;			//  7		No acknowledge interupt flag (I2C master only)
	UINT32	RtsIntf:1;			//	8		Request to send data flag (I2C slave transmitter only)
	UINT32	Rsvd:23;			//  31:9	Reserved
};
union	USART_INTSTAT_REG	{
	UINT32	all;
	struct	USART_INTSTAT_BITS	bit;
};

// USART Interrupt Enable Register Bits Definitions
struct	USART_INTEN_BITS	{	//	bits	description
	UINT32	TxiEn:1;			//	0		Transmitter FIFO empty interrupt enable
	UINT32	RxiEn:1;			//	1		Receiver data available interrupt enable
	UINT32	UnderRuniEn:1;		//	2		SPI slave transmitter underrun enable (SPI slave only)
	UINT32	RxOerriIEn:1;		//  3		Receiver overrun error interrupt enable
	UINT32	RxPerriEn:1;		//	4		Parity check error interrupt enable (UART only)
	UINT32	RxFerriEn:1;		//  5		Framing error interrupt enable (UART only)
	UINT32	RxBrkiEn:1;			//  6		Break condition interrupt enable (UART only)
	UINT32	NackiEn:1;			//  7		No acknowledge interupt enable (I2C master only)
	UINT32	RtsiEn:1;			//	8		Request to send data enable (I2C slave transmitter only)
	UINT32	Rsvd:23;			//  31:9	Reserved
};
union	USART_INTEN_REG	{
	UINT32	all;
	struct	USART_INTEN_BITS	bit;
};

// USART Interrupt Clear Register Bit Definition
struct	USART_INTCLR_BITS{	//	bits	description
	UINT32	TxiClr:1;		//	0		Transmitter FIFO empty interrupt clear
	UINT32	RxiClr:1;		//	1		Receiver data available interrupt clear
	UINT32	UnderRunClr:1;	//	2		SPI slave transmitter underrun clear (SPI slave only)
	UINT32	RxOerrClr:1;	//  3		Receiver overrun error interrupt clear
	UINT32	RxPerrClr:1;	//	4		Parity check error interrupt clear (UART only)
	UINT32	RxFerrClr:1;	//  5		Framing error interrupt clear (UART only)
	UINT32	RxBrkClr:1;		//  6		Break condition interrupt clear (UART only)
	UINT32	NackiClr:1;		//  7		No acknowledge interupt clear (I2C master only)
	UINT32	RtsiClr:1;		//	8		Request to send data clear (I2C slave transmitter only)
	UINT32	Rsvd:23;		//  31:9	Reserved
};
union	USART_INTCLR_REG{
	UINT32	all;
	struct	USART_INTCLR_BITS	bit;
};

// USART Global Control Register Bits Definition
struct	USART_GCTL_BITS	{	// bits	description
	UINT32	Usam:2;			//	1:0		USART mode select
	UINT32	Mm:1;			//  2		Master mode (SPI or I2C)
	UINT32	TxEn:1;			//  3		Transmit enable
	UINT32	RxEn:1;			//  4		Receive enable
	UINT32	Rxtlf:2;		//  6:5		Receive FIFO trigger level flag
	UINT32  Txtlf:2;		//  8:7		Transmit FIFO trigger level flag
	UINT32	Rsvd:23;		//	31:9	Reserved
};
union	USART_GCTL_REG	{
	UINT32	all;
	struct	USART_GCTL_BITS	bit;
};

// USART Common Control Register Bits Definition
struct	USART_CCTL_BITS	{	// bits	description
	UINT32	Ckph:1;			//	0		Clock phase select (SPI only)
	UINT32	Ckpl:1;			//	1		Clock polarity select (SPI only)
	UINT32	Lsbfe:1;		//	2		LSB first enable (SPI only)
	UINT32	SpiLen:1;		//	3		SPI character length (SPI only)
	UINT32	PEn:1;			//	4		Parity enable (UART only)
	UINT32	PSel:1;			//	5		Parity select (UART only)
	UINT32	Spb:1;			//  6		Stop bit select (UART only)
	UINT32	Char:2;			//	8:7		UART character length (UART only)
	UINT32	Brk:1;			//  9		UART break control (UART only)
	UINT32	I2cXa:1;		//	10		I2C extend addressing (I2C only)
	UINT32	I2cRstt:1;		//	11		Repeat start (I2C master only)
	UINT32	I2cStp:1;		//  12		STOP bit (I2C master only)
	UINT32	I2cStt:1;		//  13		Start bit (I2C master only)
	UINT32	NackDt:1;		//	14		Acknowledge data bit (I2C master receive only)
	UINT32	Rsvd:17;		//	31:15	Reserved
};
union	USART_CCTL_REG	{
	UINT32	all;
	struct	USART_CCTL_BITS	bit;
};

// USART Registers
struct	USART_REGS		{
	UINT32	TXREG;							// USART transmit data register
	UINT32	RXREG;							// USART receive data register
	union 	USART_CSTAT_REG		CSTAT;		// USART current status register
	union 	USART_INTSTAT_REG	INTSTAT;	// USART interrupt status register
	union 	USART_INTEN_REG		INTEN;		// USART interrupt enable register
	union 	USART_INTCLR_REG	INTCLR;		// USART interrupt clear register
	union 	USART_GCTL_REG		GCTL;		// USART global control register
	union 	USART_CCTL_REG		CCTL;		// USART common control register
	UINT32	SPBREG;							// USART baud rate generator register
	UINT32	SI2CADD;						// Addrss value in I2C slave mode
};

/*
*************************** USB ******************************************
*/
//#pragma pack(push) 		//�������״̬ 
#pragma pack(1)
//----- USB FADDR Register Bit Definition-----
struct USB_FADDR_BITS	{	//	bits	description
	UINT8 	Addr:7;			//	Address of usb
	UINT8 	Update:1;		//	Set when FAddr is written. Cleared when the new address takes effect (at the end of the current transfer).	  
};

union USB_FADDR_REG		{	//reset is 0x00
	UINT8 	all;
	struct 	USB_FADDR_BITS bit;
};

//---- USB POWER Register Bit Definition -------
struct USB_POWER_BITS	{	//	bits	description
	UINT8 	EnSuspendM:1;	//Set by the CPU to enable the SUSPENDM signal.
	UINT8 	SuspendMode:1;	//	This read-only bit is set when Suspend mode is entered. It is cleared when the CPU reads the
	                     	//interrupt register, or sets the Resume bit of this register
	UINT8 	Resume:1; 		//Set by the CPU to generate Resume signaling when the function is in Suspend mode. The CPU
	              			//should clear this bit after 10 ms (a maximum of 15 ms) to end Resume signaling.  
	UINT8 	Reset:1;		//This read-only bit is set when Reset signaling has been detected on the bus
	
	UINT8 	Rev:3;
	UINT8 	ISOUpdate:1; 
};
union USB_POWER_REG		{	//reset is 0x20
	UINT8 	all;
	struct	USB_POWER_BITS bit;
};

//----- USB Interrupt IN Register Bit Definition-----
struct USB_INTRIN_BITS	{	//	bits	description
	UINT16	EP0:1;  	    //	0 		EP0 interrupt
	UINT16	EP1In:1;   	    //	1 		EP1 In interrupt
	UINT16	EP2In:1;		//	1		EP2 In interrupt
	UINT16	EP3In:1;   	   	//	1 		EP3 In interrupt
	UINT16	EP4In:1;	 	//	1		EP4 In interrupt
	UINT16	Rev:11;
};
union USB_INTRIN_REG	{	//reset is 0x0000
	UINT16 	all;
	struct 	USB_INTRIN_BITS bit;
};

//------ USB Interrupt OUT Register Bit Definition------
struct USB_INTROUT_BITS	{	//	bits	description
	UINT16 	RESERVED:1;  	//	0 	 
	UINT16 	EP1Out:1;   	//	1 		EP1 Out interrupt
	UINT16 	EP2Out:1;		//	1		EP2 Out interrupt
	UINT16 	EP3Out:1;   	//	1 		EP3 Out interrupt
	UINT16 	EP4Out:1;		//	1		EP4 Out interrupt
	UINT16 	REV:11;
};
union USB_INTROUT_REG	{	//reset is 0x0000
	UINT16 	all;
	struct 	USB_INTROUT_BITS bit;
};

//---- USB Interrupt enable IN Register Bit Definition------
struct USB_INTRINE_BITS	{	//	bits	description
	UINT16 	EP0:1;  	    //	0 		EP0 interrupt
	UINT16 	EP1In:1;   	    //	1 		EP1 In interrupt
	UINT16 	EP2In:1;		//	1		EP2 In interrupt
	UINT16 	EP3In:1;   	    //	1 		EP3 In interrupt
	UINT16 	EP4In:1;	 	//	1		EP4 In interrupt
	UINT16 	Rev:11;
};
union USB_INTRINE_REG	{	//reset is 0xffff
	UINT16 	all;
	struct 	USB_INTRINE_BITS bit;
};
//----- USB Interrupt enable OUT Register Bit Definition-----
struct USB_INTROUTE_BITS{	//	bits	description
	UINT16 	RESERVED:1;  	//	0 	 
	UINT16 	EP1Out:1;   	//	1 		EP1 Out interrupt
	UINT16 	EP2Out:1;		//	1		EP2 Out interrupt
	UINT16 	EP3Out:1;   	//	1 		EP3 Out interrupt
	UINT16 	EP4Out:1; 		//	1		EP4 Out interrupt
	UINT16 	Rev:11;
};
union USB_INTROUTE_REG	{	//reset is 0xffff
	UINT16 	all;
	struct 	USB_INTROUTE_BITS bit;
};

//---- USB Interrupt Register Bit Definition------- 
struct USB_INTRUSB_BITS	{	//bits	description
	UINT8 	Suspend:1;		//Set when suspend signaling is detected on the bus.
	UINT8 	Resume:1; 		//Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
	UINT8 	Reset:1;		//Set when reset signaling is detected on the bus.
	UINT8 	SOF:1;			//Set at the start of each frame      
	UINT8 	Reserved:4;                       
};
union USB_INTRUSB_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INTRUSB_BITS bit;
};
//----- USB Interrupt Enable Register Bit Definition -----
struct USB_INTRUSBE_BITS{	//	bits	description
	UINT8 	Suspend:1;		//Set when suspend signaling is detected on the bus.
	UINT8 	Resume:1; 		//Set when resume signaling is detected on the bus while the MUSBHSFC is in Suspend mode..  
	UINT8 	Reset:1;		//Set when reset signaling is detected on the bus.
	UINT8 	SOF:1;			//Set at the start of each frame      
	UINT8 	Reserved:4;                       
};
union USB_INTRUSBE_REG	{	//reset is 0x06
	UINT8 	all;
	struct	USB_INTRUSBE_BITS bit;
};

//----- USB Frame Register Bit Definition------- 
struct USB_FRAME_BITS	{	//bits	description
	UINT16 	FrameNumber:11;	     
	UINT16 	Reserved:5;                      
};
union USB_FRAME_REG		{	//reset is 0x0000
	UINT16 	all;
	struct	USB_FRAME_BITS bit;
};

//----- USB Index Register Bit Definition --------
struct USB_INDEX_BITS	{	//bits	description
	UINT8 	EPIndex:4;	     
	UINT8 	Reserved:4;                     
                                              
};
union USB_INDEX_REG{		//reset is 0x0000
	UINT8 	all;
	struct	USB_INDEX_BITS bit;
};
// ----USB control and status bits for Endpoint 0 Register Bit Definition------
struct USB_CSR0_BITS	{		//	bits	description
	UINT8 	OutPktRdy:1;		//0 This bit is set when a data packet has been received.
	UINT8 	InPktRdy:1;			//1 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	SentStall:1;		//2 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	UINT8 	DataEnd:1;			//3 The CPU sets this bit:
	                        		//1. When setting InPktRdy for the last data packet.
	                        		//2. When clearing OutPktRdy after unloading the last data packet.
	                        		//3. When setting InPktRdy for a zero length data packet.
	                        		//It is cleared automatically.
	UINT8 	SetupEnd:1;			//4 This bit will be set when a control transaction ends before the DataEnd bit has been set.
	                        		//An interrupt will be generated and the FIFO flushed at this time.
	UINT8 	SendStall:1;		//5 The CPU writes a 1 to this bit to terminate the current transaction. The STALL handshake
	                        		//will be transmitted and then this bit will be cleared automatically.
	UINT8 	ServicedOutPktRdy:1;//6 The CPU writes a 1 to this bit to clear the OutPktRdy bit. It is cleared automatically.
	UINT8 	ServicedSetupEnd:1;	//7 The CPU writes a 1 to this bit to clear the SetupEnd bit. It is cleared automatically.
	     
};
union USB_CSR0_REG		{	//reset is 0x00
	UINT8 	all;
	struct	USB_CSR0_BITS bit;
};
//---- USB Count of EP0 FIFO Register Bit Definition------ 
struct USB_COUNT0_BITS	{	//bits	description
	UINT8	Count:7;	     
	UINT8	Reserved:1;                                                                  
};
union USB_COUNT0_REG	{	//reset is 0x0000
	UINT8 	all;
	struct	USB_COUNT0_BITS bit;
};

//------- USB provides control and status bits for IN transactions Register Bit Definition
struct USB_INCSRL_BITS	{	//bits	description
	UINT8 	InPktRdy:1;		//0 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	FIFONotEmpty:1;	//1 This bit is set when there is at least 1 packet in the IN FIFO.
	UINT8 	UnderRun:1;		//2 In ISO mode,
	UINT8 	FlushFIFO:1;	//3 TThe CPU writes a 1 to this bit to flush the next packet to be transmitted from the endpoint IN FIFO.
	UINT8 	SendStall:1;	//4 The CPU writes a 1 to this bit to issue a STALL handshake to an IN token.
	UINT8 	SentStall:1;	//5 This bit is set when a STALL handshake is transmitted.
	UINT8 	ClrDataTog:1;	//6 The CPU writes a 1 to this bit to reset the endpoint IN data toggle to 0.
	UINT8 	IncompTx:1;		//7 used in ISO mode
};
union USB_INCSRL_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INCSRL_BITS bit;
};

//------- USB provides additional control for IN transactions Register Bit Definition
struct USB_INCSRU_BITS	{	//bits	description
	UINT8 	Reserved1:1;	//0 The CPU sets this bit after loading a data packet into the FIFO.
	UINT8 	DPktBufDis:1;	//1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled
	UINT8 	Reserved2:1;	//2 The CPU sets this bit to select DMA Request Mode 1 and clears it to select DMA Request Mode 0.
	UINT8 	FrcDataTog:1;	//3 sets this bit to force the endpoint��s IN data toggle to switch after each data packet is sent
	UINT8 	Reserved3:1;	//4 sets this bit to enable the DMA request for the IN endpoint.
	UINT8 	Mode:1;			//5 sets this bit to enable the endpoint direction as IN, and clears it to enable the endpoint direction as OUT.
	UINT8 	ISO:1;			//6  used in ISO mode
	UINT8 	AutoSet:1;		//7 If the CPU sets this bit, InPktRdy will be automatically set when data of the maximum packet size
	                  		//(as set in InMaxP) is loaded into the IN FIFO. If a packet of less than the maximum packet size is
	                   		//loaded, InPktRdy will have to be set manually. Note:	     
};
union USB_INCSRU_REG	{	//reset is 0x00
	UINT8 	all;
	struct	USB_INCSRU_BITS bit;
};
// -----USB Count of EPx OUT Max amount of data that can be transferred through the selected IN endpoint in a single
//operation. Register Bit Definition 
struct USB_OUTMAXP_BITS	{	//bits	description
	UINT8 	MaxAmount:3;	     
	UINT8 	Reserved:5;                                              
};
union USB_OUTMAXP_REG	{	//reset is 0x0000
	UINT8 	all;
	struct	USB_OUTMAXP_BITS bit;
};

//------- USB provides control and status bits for Out transactions Register Bit Definition
struct USB_OUTCSRL_BITS	{	//bits	description
	UINT8 	OutPktRdy:1;	//0 This bit is set when a data packet has been received.
	UINT8 	FIFOFull:1;		//1 This bit is set when no more packets can be loaded into the OUT FIFO.
	UINT8 	OverRun:1;		//2 This bit is set if an OUT packet arrives while FIFOFull is set
	UINT8 	DataError:1;	//3 This bit is set at the same time that OutPktRdy is set if the data packet has a CRC error.
	UINT8 	FlushFIFO:1;	//4 writes a 1 to this bit to flush the next packet to be read from the endpoint OUT FIFO.
	UINT8 	SendStall:1;	//5 writes a 1 to this bit to issue a STALL handshake to a DATA packet.
	UINT8 	SentStall:1;	//6 This bit is set when a STALL handshake is transmitted. The CPU should clear this bit.
	UINT8 	ClrDataTog:1;	//7 writes a 1 to this bit to reset the endpoint data toggle to 0.	     
};
union USB_OUTCSRL_REG{		//reset is 0x00
	UINT8 	all;
	struct	USB_OUTCSRL_BITS bit;
};

//----- USB provides additional control for Out transactions Register Bit Definition
struct USB_OUTCSRU_BITS	{		//bits	description
	UINT8 	Reserved1:1;		//0 This bit is set in a high-bandwidth Isochronous transfer if the packet in the OUT FIFO is incomplete
	UINT8 	DPktBufDis:1;		//1 This bit is used to control the use of double packet buffering. It is ignored when Dynamic Fifo is enabled.
	UINT8 	Reserved2:1;		//2
	UINT8 	Reserved3:1;		//3 Two modes of DMA Request operation are supported:
	UINT8 	DisNYETPIDError:1;	//4 sets this bit to disable the sending of NYET handshakes.
	UINT8 	Reserved4:1;		//5 sets this bit to enable the DMA request for the OUT endpoint.
	UINT8 	ISO:1;				//6 used in ISO mode
	UINT8 	AutoClear:1;		//7 sets this bit then the OutPktRdy bit will be automatically cleared when a packet of
	                    		//OutMaxP bytes has been unloaded from the OUT FIFO.
};
union USB_OUTCSRU_REG	{		//reset is 0x00
	UINT8 	all;
	struct	USB_OUTCSRU_BITS bit;
};
 
union USB_STAS_REG{    
    union 	USB_CSR0_REG CSR0;
    union 	USB_INCSRL_REG INCSRL;
};

//=========== USB Registers=====================
struct	USB_REGS		{
	union	USB_FADDR_REG		FADDR;		//8
	union	USB_POWER_REG 		POWER;		//8
	union	USB_INTRIN_REG		INTRIN;		//16
	union	USB_INTROUT_REG		INTROUT;	//16
	union	USB_INTRUSB_REG		INTRUSB;	//8	  
	union	USB_INTRINE_REG		INTRINE;	//16	
	union	USB_INTROUTE_REG	INTROUTE;	//16
	union	USB_INTRUSBE_REG	INTRUSBE;	//8
	UINT16	FRAME;							//16//union  USB_FRAME_REG    FRAME;
    UINT8 	INDEX;							//8//union  USB_INDEX_REG     INDEX;
    UINT8 	RESERVED1; 						//8
    UINT8 	INMAXP;  						//8//union USB_INMAXP_REG    INMAXP;	
    union 	USB_STAS_REG		STAS;		//8
	union 	USB_INCSRU_REG		INCSRU;		//8
    UINT8 	OUTMAXP;						//8 
	union 	USB_OUTCSRL_REG     OUTCSRL;	//8	
	union 	USB_OUTCSRU_REG     OUTCSRU;	//8
	UINT16 	OUTCOUNT;						//16
	UINT16 	RESERVED2[4];
	UINT32 	FIFO_EP0;
	UINT32 	FIFO_EP1;
	UINT32 	FIFO_EP2;
	UINT32 	FIFO_EP3;
	UINT32 	FIFO_EP4;
};
#pragma pack()

/*
*************************** SSI ******************************************
*/
// SSI Control Register 0 Bit Definition
struct	SSI_CTRLR0_BITS	{	//	bits	description
	UINT32	Dfs:4;  		//	3:0		Data Frame Size
	UINT32	Frf:2;			//	5:4		Frame Format
	UINT32	Scph:1;			// 	6		Serial Clock Phase
	UINT32	Scpol:1;		//	7		Serial Clock Polarity
	UINT32	Tmod:2;			// 	9:8		Transfer Mode
	UINT32	SlvOe:1;		// 	10		Slave Output Enable
	UINT32	Srl:1;			//	11		Shift Register Loop
	UINT32	Cfs:4;			//	15:12	Control Frame Size
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR0_REG	{
	UINT32	all;
	struct	SSI_CTRLR0_BITS	bit;
};

// SSI Control Register 1 Bit Definition
struct	SSI_CTRLR1_BITS	{	//	bits	description
	UINT32	Ndf:16;  		//	15:0	Number of Data Frame
	UINT32	Rsvd:16; 		//	31:16 	Reserved
};
union	SSI_CTRLR1_REG	{
	UINT32	all;
	struct	SSI_CTRLR1_BITS	bit;
};

// SSI Microwire Control Register Bit Definition
struct	SSI_MWCR_BITS	{	//	bits	description
	UINT32	MwMod:1;  		//	0		Microwire Transfer Mode
	UINT32	Mod:1;  		//	1		Microwire Control
	UINT32	Mhs:1;  		//	2		Microwire Handshaking
	UINT32	Rsvd:29; 		//	31:3 	Reserved
};
union	SSI_MWCR_REG	{
	UINT32	all;
	struct	SSI_MWCR_BITS	bit;
};

// SSI Status Register Bit Definition
struct	SSI_SR_BITS		{	//	bits	description
	UINT32	Busy:1;  		//	0		SSI Busy Flag
	UINT32	Tfnf:1;			//	1		Transmit FIFO Not Full
	UINT32	Tfe:1;			// 	2		Transmit FIFO Empty
	UINT32	Rfne:1;			//	3		Receive FIFO Not Empty
	UINT32	Rff:1;			// 	4		Receive FIFO Full
	UINT32	Txe:1;			// 	5		Transmission Error
	UINT32	Dcol:1;			//	6		Data Collision Error
	UINT32	Rsvd:25; 		//	31:7 	Reserved
};
union	SSI_SR_REG		{
	UINT32	all;
	struct	SSI_SR_BITS	bit;
};

// SSI Interrupt Register Bit Definition
struct	SSI_INT_BITS	{	//	bits	description
	UINT32	Txe:1;  		//	0		Transmit FIFO Empty 
	UINT32	Txo:1;			//	1		Transmit FIFO Overflow
	UINT32	Rxu:1;			// 	2		Receive FIFO Underflow
	UINT32	Rxo:1;			//	3		Receive FIFO Overflow
	UINT32	Rxf:1;			// 	4		Receive FIFO Full
	UINT32	Mst:1;			// 	5		Transmission Error
	UINT32	Rsvd:26; 		//	31:6 	Reserved
};
union	SSI_INT_REG		{
	UINT32	all;
	struct	SSI_INT_BITS	bit;
};

// SSI DMA Control Register Bit Definition
struct	SSI_DMACR_BITS	{	//	bits	description
	UINT32	Rdmae:1;  		//	0		Receive DMA Enable
	UINT32	Tdmae:1;  		//	1		Transmit DMA Enable
	UINT32	Rsvd:30; 		//	31:2 	Reserved
};
union	SSI_DMACR_REG	{
	UINT32	all;
	struct	SSI_DMACR_BITS	bit;
};

// SSI regs
struct	SSI_REGS		{
	union 	SSI_CTRLR0_REG	CTRLR0;		// Control Register 0
	union 	SSI_CTRLR1_REG	CTRLR1;		// Control Register 1
	UINT32	SSIENR;
	union 	SSI_MWCR_REG	MWCR;

	UINT32 	SER;
	UINT32	BAUDR;
	UINT32	TXFTLR;
	UINT32	RXFTLR;

	UINT32	TXFLR;	
	UINT32	RXFLR;
	union 	SSI_SR_REG 		SR;
	union 	SSI_INT_REG		IMR;

	union 	SSI_INT_REG		ISR;
	union 	SSI_INT_REG		RISR;
	UINT32	TXOICR;
	UINT32	RXOICR;

	UINT32	RXUICR;
	UINT32	MSTICR;
	UINT32	ICR;
	union 	SSI_DMACR_REG	DMACR;

	UINT32 	DMATDLR;
	UINT32	DMARDLR;
	UINT32	IDR;
	UINT32	COMP_VERSION;

	UINT32	DR;
	UINT32	RSVD1[35];

	UINT32	RX_SAMPLE_DLY;
	UINT32	RSVD2[3];
};

/*
*************************** SWPS ******************************************
*/
#define	SWPS_INT_TX_DONE	(1<<0)
#define	SWPS_INT_RX_SOF		(1<<8)
#define	SWPS_INT_RX_DONE	(1<<9)
#define	SWPS_INT_RESUME		(1<<24)
#define	SWPS_INT_SUSPEND	(1<<25)
#define	SWPS_INT_DEACTIVE	(1<<26)
#define	SWPS_INT_CLF_START	(1<<27)

// SWPS Interrupt (Enable) Register Bit Definition
struct	SWPS_INT_BITS		{	//	bits	description
	UINT32	TxDone:1;  			//	0		transmit over
	UINT32	Rsvd0:1;			//	1		reserved
	UINT32	TxFifoNotFull:1;	// 	2		transmit FIFO not full
	UINT32	Rsvd1:5;
	UINT32	RxSof:1;			// 	8		SOF received
	UINT32	RxDone:1;			// 	9		receive over
	UINT32	Rsvd2:3;			//	12:10	reserved
	UINT32	RxFifoNotEmpty:1;	//	13		receive FIFO not empty
	UINT32	Rsvd3:10;			// 	23:14	reserved
	UINT32	Resume:1;			// 	24		Resume received
	UINT32	Suspend:1;			//	25		Suspend received
	UINT32	Deactive:1;			//	26		Deactive received
	UINT32	ClfStart:1;			// 	27		
	UINT32	Rsvd:4; 			//	31:28 	Reserved
};
union	SWPS_INT_REG		{
	UINT32	all;
	struct	SWPS_INT_BITS	bit;
};

// SWPS Status Register Bit Definition
struct	SWPS_STATUS_BITS	{	//	bits	description
	UINT32	TxFifoEmpty:1;  			
	UINT32	TxFifoFull:1;				
	UINT32	RxFifoEmpty:1;			
	UINT32	RxFifoFull:1;
	UINT32	Rsvd0:4;				
	UINT32	TxInhibit:1;		
	UINT32	TxFifoUnderFlow:1;	
	UINT32	TxFrameErr:1;		
	UINT32	Rsvd1:5;			
	UINT32	RxFrameErr:1;		
	UINT32	RxCrcErr:1;			
	UINT32	RxEofNotFoundErr:1;	
	UINT32	RxFifoOverFlow:1;			
	UINT32	Rsvd2:12; 			
};
union	SWPS_STATUS_REG		{
	UINT32	all;
	struct	SWPS_STATUS_BITS	bit;
};

// SWPS Control Register Bit Definition
struct	SWPS_CTRL_BITS		{	//	bits	description
	UINT32	TxEn:1;  			//	0		transmit enable
	UINT32	Rsvd1:3;			//	3:1		reserved
	UINT32	TxResumeBit:3;		// 	6:4		the bit 1 befor transmit SOF(reserved)	
	UINT32	TxFifoClr:1;		//	7		Tx FIFO clear
	UINT32	RxEn:1;				// 	8		receive	enable
	UINT32	SuspendIdleBit:3;	// 11:9	Suspend idle bit number
	UINT32	Rsvd2:3;			// 14:12	Reserved
	UINT32	RxFifoClr:1;		//	15		Rx FIFO clear
	UINT32	Rsvd:16; 			//	31:13 	Reserved
};
union	SWPS_CTRL_REG		{
	UINT32	all;
	struct	SWPS_CTRL_BITS	bit;
};

// SWPS Length Register Bit Definition
struct	SWPS_LEN_BITS		{	//	bits	description
	UINT32	TxLen:8;  			//	7:0		transmit length
	UINT32	RxLen:8;			//	15:8	receive length
	UINT32	Rsvd:16; 			//	31:16 	Reserved
};
union	SWPS_LEN_REG		{
	UINT32	all;
	struct	SWPS_LEN_BITS	bit;
};

// SWPS S1 Period Register Bit Definition
struct	SWPS_S1_PERIOD_BITS	{	//	bits	description
	UINT32	MaxHighTime:16;  	//	9:0		S1 maximum high level duration time
	UINT32	MaxLowTime:16;		//	25:16	S1 maximum low level duration time
};
union	SWPS_S1_PERIOD_REG	{
	UINT32	all;
	struct	SWPS_S1_PERIOD_BITS	bit;
};

// SWPS regs
struct	SWPS_REGS			{
	union 	SWPS_INT_REG		INT;
	union 	SWPS_INT_REG		IE;
	union 	SWPS_STATUS_REG		STATUS;
	union 	SWPS_CTRL_REG		CTRL;
	union 	SWPS_LEN_REG 		LEN;
	UINT32	TX_FIFO;
	UINT32	RX_FIFO;
	UINT32	SUSPEND_TM;						// 13bit
	UINT32	DEACTIVATED_TM;					// 14bit
	UINT32	INHIBIT_TM;	   					// 23bit
	union 	SWPS_S1_PERIOD_REG	S1_PERIOD; 	// 11bit
};

/*
*************************** ISO7816 ******************************************
*/
struct	ISO7816_CON0_BITS	{	// bits		description
	UINT32  CLKOE:1;            //  0       clock output enable (master)
	UINT32  INV:1;              //  1       inverse convention
	UINT32  Rsvd_1:2;           //  3:2     Reserved
	UINT32  RBRST:1;            //  4       write 1 to reset rx buffer     
	UINT32  TR:1;               //  5       0:Rx mode; 1:Tx mode
	UINT32  Rsvd_2:1;           //  6       Reserved
	UINT32  MOD:1;              //  7       0:hardware;1:direct I/O
	UINT32  Rsvd_3:24;          //  31:8 Reserved
};

union	ISO7816_CON0_REG	{
	UINT32	all;
	struct	ISO7816_CON0_BITS  bit;
};

struct	ISO7816_CON1_BITS	{   // bits            description
	UINT32  TRY:3;              // 2:0             auto tx times
	UINT32  RRY:3;              // 5:3             auto rx times			
	UINT32  DIV:8;              // 13:6            divider for clock output
	UINT32  Rsvd_1:2;           // 15:14           Reserved
	UINT32  EGT:9;              // 24:16           extra guard time for tx	
	UINT32  Rsvd_2:7;           // 31:25           Reserved
};

union	ISO7816_CON1_REG   {
	UINT32	all;
	struct	ISO7816_CON1_BITS  bit;
};

struct	ISO7816_CON2_BITS	{ 	//  bits 	description
	UINT32 	WES0:1; 			//	0      	error signal width
	UINT32 	Rsvd_1:1;			//	1       Reserved
  	UINT32  TPS:1;				// 	2   	T=0 or T=1 protocal
 	UINT32	Rsvd_2:4; 			//	6:3  	Reserved
	UINT32	SBIT:1; 			// 	7  		I/O negedge flag
	UINT32	Rsvd_3:24; 			// 	31:8 	Reserved
};

union	ISO7816_CON2_REG	{
	UINT32	all;
    struct	ISO7816_CON2_BITS  bit;
};

struct	ISO7816_STS_BITS	{	// bits    	description
	UINT32	TBE:1;				//	0       tx buffer empty flag
	UINT32	RBF:1;              //  1     	rx buffer full flag
	UINT32	RBNE:1;             //	2      	rx buffer is not empty
	UINT32	PE:1;               //	3       parity error flag
	UINT32	OE:1;               //	4       rx buffer overrun error flag
	UINT32	WAIT:1;             // 	5   	1:register access is not finished
	UINT32	Rsvd_1:26;          // 	31:6 	Reserved
};

union	ISO7816_STS_REG		{
	UINT32	all;
	struct 	ISO7816_STS_BITS	bit;
};

struct	ISO7816_BRC_BITS	{	//	bits    	description
 	UINT32 	DI:4;             	//	3:0     baud rate parameter: D
    UINT32	FI:4;               //	7:4  	baud rate parameter: F
    UINT32	Rsvd_1:24;          //	31:8 	Reserved
};

union	ISO7816_BRC_REG		{
	UINT32	all;
	struct 	ISO7816_BRC_BITS	bit;
};

struct	ISO7816_BUF_BITS	{ 	//	bits    	description
	UINT32 	DATA:8;           	//	7:0     tx or rx buffer
	UINT32  Rsvd_1:24;        	//	31:8 	Reserved
};

union	ISO7816_BUF_REG		{
	UINT32	all;
	struct 	ISO7816_BUF_BITS	bits;
};

struct	ISO7816_DIO_BITS	{	//	bits  	description
	UINT32	DIO:1;            	//	0      	data input/output
	UINT32	ENO:1;             	//	1       direct output enable
	UINT32 	Rsvd_1:30;         	//	31:2 	Reserved
};

union	ISO7816_DIO_REG 	{
	UINT32	all;
	struct 	ISO7816_DIO_BITS 	bit;
};

struct	ISO7816_MSK_BITS 	{	// bits  	description
	UINT32	MTBE:1; 			//	0      	mask for TBE
	UINT32	MRBF:1;            	// 	1      	mask for RBF
	UINT32	MRBNE:1;            //	2      	mask for RBNE
	UINT32	MPE:1;           	// 	3       mask for PE
	UINT32	MOE:1;             	// 	4      	mask for OE
	UINT32	MSBIT:1;          	// 	5      	mask for SBIT
	UINT32	Rsvd_2:26;         	// 	31:6 	Reserved
};

union	ISO7816_MSK_REG     {
	UINT32	all;
	struct 	ISO7816_MSK_BITS 	bit;
};

struct	ISO7816_TCON_BITS   { 	// bits    	description
	UINT32	TS:1;             	// 	0      	ETU counter start
	UINT32	TMOD:2;           	//	2:1    	ETU counter mode selection
	UINT32	TF:1;              	// 	3      	ETU counter overflow flag
	UINT32	Rsvd_1:28;        	// 	31:4 	Reserved 
};

union	ISO7816_TCON_REG   	{
	UINT32	all;
	struct ISO7816_TCON_BITS  bit;
};

struct	ISO7816_TMSK_BITS   {	// bits    	description
	UINT32	MTF:1;            	//	0    	mask for TF
    UINT32	Rsvd_1:31;      	//	31:1 	Reserved
};

union	ISO7816_TMSK_REG    {
	UINT32 	all;	
	struct 	ISO7816_TMSK_BITS  bit; 
};

struct	ISO7816_PROC_BITS   {	// bits    	description	  
	UINT32	DATA:8;       		//	7:0   	auto tx value	
    UINT32	Rsvd_1:24;         	//	31:8 	Reserved	
};

union	ISO7816_PROC_REG  	{
	UINT32     all;	
	struct	ISO7816_PROC_BITS  bit;	 
};

struct	ISO7816_REGS{
	union	ISO7816_CON0_REG	CON0;
    union	ISO7816_CON1_REG   	CON1;
    union	ISO7816_CON2_REG   	CON2;
    union	ISO7816_STS_REG    	STS;
    union	ISO7816_BRC_REG    	BRC;
    union	ISO7816_BUF_REG    	BUF;
    union	ISO7816_DIO_REG    	DIO;
    union	ISO7816_MSK_REG    	MSK;
    union	ISO7816_TCON_REG   	TCON0;
    UINT32   	TDAT0;
    UINT32   	TRLD0;
    union 	ISO7816_TMSK_REG   	TMSK0;
    union 	ISO7816_PROC_REG   	PROC0;
	union	ISO7816_TCON_REG   	TCON1;
    UINT32   	TDAT1;
    UINT32   	TRLD1;
    union 	ISO7816_TMSK_REG   	TMSK1;
};

/*
*************************** SCI ******************************************
*/
// SCI Data Register Bit Definition
struct	SCI_DATA_BITS	{//	bits	description
	UINT32	Data:8;  	//	7:0		read or write data
	UINT32	ParityErr:1;//	8		parity error flag
	UINT32	Rsvd:23; 	//	31:9 	Reserved
};
union	SCI_DATA_REG	{
	UINT32	all;
	struct	SCI_DATA_BITS	bit;
};

// SCI Control Register 0 Bit Definition
struct SCI_CR0_BITS 	{	//	bits	description
	UINT32  Sense :1;		//	0
	UINT32  Order :1;		// 	1
	UINT32  TxParity:1;		//	2
	UINT32  TxNak:1;		//	3
	UINT32  RxParity:1;		//	4
	UINT32  RxNak:1;		//	5
	UINT32  ClkDis:1;		//	6
	UINT32  ClkVal :1;		//	7
	UINT32  Rsvd:24;
};
union SCI_CR0_REG {
	UINT32 	all;
	struct 	SCI_CR0_BITS bit;
};

// SCI Control Register 1 Bit Definition
struct SCI_CR1_BITS {
	UINT32  ATRDEn :1;
	UINT32  BlkEn :1;
	UINT32  Mode:1;
	UINT32  Clkz1:1;
	UINT32  BgtEn:1;
	UINT32  ExDbnce:1;
	UINT32  SyncCard:1;
	UINT32	CardSocket:1;
	UINT32  Rsvd:24;
};
union SCI_CR1_REG {
	UINT32 	all;
	struct 	SCI_CR1_BITS bit;
};

// SCI Control Register 2 Bit Definition
struct SCI_CR2_BITS {
	UINT32  StartUp :1;
	UINT32  Finish :1;
	UINT32  WrReset:1;
	UINT32  Rsvd:29;
};
union SCI_CR2_REG {
	UINT32 	all;
	struct 	SCI_CR2_BITS bit;
};

// SCI Transmit and Receive Tide Register Bit Definition
struct SCI_TIDE_BITS {
	UINT32  TxTide :4;
	UINT32  RxTide :4;
	UINT32  Rsvd:24;
};
union SCI_TIDE_REG {
	UINT32 	all;
	struct 	SCI_TIDE_BITS bit;
};

// SCI Transmit and Receive Retry Register Bit Definition
struct SCI_RETRY_BITS {
	UINT32  TxRetry :3;
	UINT32  RxRetry :3;
	UINT32  Rsvd:26;
};
union SCI_RETRY_REG 	{
	UINT32 all;
	struct SCI_RETRY_BITS bit;
};

// SCI Transmit FIFO Count Register Bit Definition
struct SCI_FIFOSTATUS_BITS {
	UINT32  TxFF :1;
	UINT32  TxFE :1;
	UINT32  RxFF :1;
	UINT32  RxFE :1;
	UINT32  Rsvd:28;
};
union SCI_FIFOSTATUS_REG	{
	UINT32 	all;
	struct 	SCI_FIFOSTATUS_BITS bit;
};

// SCI Interrupt Mask/Status/Masked Status Register Bit Definition
struct SCI_INT_BITS 	{
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  RxTide :1;
	UINT32  TxTide :1;
	UINT32  Rsvd:17;
};
union SCI_INT_REG	{
	UINT32 	all;
	struct 	SCI_INT_BITS bit;
};

//SCI Interrupt Clear Register Bit Definition
struct SCI_ICR_BITS		{
	UINT32  Insert :1;
	UINT32  Remove :1;
	UINT32  PowerOn :1;
	UINT32  PowerOff :1;
	UINT32  TxErr :1;
	UINT32  ATRSTOut:1;
	UINT32  ATRDTOut:1;
	UINT32  BlkTout :1;
	UINT32  ChTOut :1;
	UINT32  RdTOut :1;
	UINT32  RxOv :1;
	UINT32  ClkStp :1;
	UINT32  ClkAct :1;
	UINT32  Rsvd:19;
};
union SCI_ICR_REG		{
	UINT32 	all;
	struct 	SCI_ICR_BITS bit;
};

// SCI Synchronous Card Activation Control Register Bit Definition
struct SCI_SYNCACT_BITS	{
	UINT32  Power :1;
	UINT32  CRERst :1;
	UINT32  ClkEn :1;
	UINT32  DataEn :1;
	UINT32  FCB :1;
	UINT32  ClkoutEn :1;
	UINT32	ExClkEn	:1;
	UINT32  Clkout :1;
	UINT32  DataOutEn :1;
	UINT32  ExDataEn:1;
	UINT32  Present :1;
	UINT32  Rsvd:21;
};
union SCI_SYNCACT_REG	{
	UINT32 	all;
	struct 	SCI_SYNCACT_BITS bit;
};

// SCI Synchronous Transmit Clock and Data Register Bit Definition
struct SCI_SYNCTX_BITS	{
	UINT32  WData :1;
	UINT32  WClk :1;
	UINT32  WDataEn :1;
	UINT32  WClkEn :1;
	UINT32  WRst :1;
	UINT32  WFCB :1;
	UINT32  Rsvd:26;
};
union SCI_SYNCTX_REG	{
	UINT32 	all;
	struct 	SCI_SYNCTX_BITS bit;
};

// SCI Synchronous Receive Clock and Data register Bit Definition
struct SCI_SYNCRX_BITS {
	UINT32  RData :1;
	UINT32  RClk :1;
	UINT32  Rsvd:30;
};
union SCI_SYNCRX_REG	{
	UINT32 	all;
	struct 	SCI_SYNCRX_BITS bit;
};

//SCI regs
struct	SCI_REGS	{
	union 	SCI_DATA_REG	DATA;
	union 	SCI_CR0_REG	CR0;
	union 	SCI_CR1_REG	CR1;
	union 	SCI_CR2_REG	CR2;
	UINT32	CLKICC;
	UINT32  VALUE;
	UINT32	BAUD;
	union 	SCI_TIDE_REG	TIDE;
	UINT32 	Rsvd1;		// Reserved for DMA control
	UINT32  STABLE;
	UINT32	ATIME;
	UINT32	DTIME;
	UINT32 	ATRSTIME;
	UINT32	ATRDTIME;
	UINT32  STOPTIME;
	UINT32	STARTTIME;
	union 	SCI_RETRY_REG	RETRY;
	UINT32	CHTIMELS;
	UINT32	CHTIMEMS;
	UINT32  BLKTIMELS;
	UINT32  BLKTIMEMS;
	UINT32  CHGUARD;
	UINT32	BLKGUARD;
	UINT32  RXTIME;
	union 	SCI_FIFOSTATUS_REG 	FIFOSTATUS;
	UINT32  TXCOUNT;
    UINT32	RXCOUNT;
    union 	SCI_INT_REG			IMSC;
    union 	SCI_INT_REG   		RIS;
    union 	SCI_INT_REG   		MIS;
    union 	SCI_ICR_REG   		ICR;
    union 	SCI_SYNCACT_REG  	SYNCACT;
    union 	SCI_SYNCTX_REG    	SYNCTX;
    union 	SCI_SYNCRX_REG		SYNCRX;
};

/*
*************************** VPWM ******************************************
*/
// VPWM Timer Register Bit Definition
struct	VPWM_TIMER_BITS	{	//	bits	     description
   UINT32 	FsPeriod:16;	//	15:0      	frequency
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_TIMER_REG	{
	UINT32	all;
	struct 	VPWM_TIMER_BITS	bit;
};

// VPWM config Register Bit Definition
struct	VPWM_CFG_BITS	{   //	bits	     description
   UINT32 	En:1;    		//	0
   UINT32 	DutySelect:1;   //	1
   UINT32 	Sample_b:1;    	//	2
   UINT32 	RLshift:1;    	//	3
   UINT32 	ShiftBits:4;    //	7:4
   UINT32 	FuncSelect:1;   //	8
   UINT32 	Empty:1;        //	9
   UINT32 	Full:1;         //	10
   UINT32 	Rsvd1:21;		//	31:11
};
union	VPWM_CFG_REG	{
	UINT32 	all;
	struct 	VPWM_CFG_BITS bit;
};

// VPWM period Register Bit Definition
struct	VPWM_PR_BITS{   	//	bits	     description
   UINT32 	Period:16;    	//	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_PR_REG	{
	UINT32	all;
	struct 	VPWM_PR_BITS	bit;
};

// VPWM duty Register Bit Definition
struct	VPWM_DR_BITS{   	//	bits	     description
   UINT32 	Duty:16;      	//	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_DR_REG	{
	UINT32	all;
	struct 	VPWM_DR_BITS	bit;
};

// VPWM int Register Bit Definition
struct	VPWM_INTCTRL_BITS{   //	bits	     description
   UINT32 	NoFullEn:1;      	//	0
   UINT32 	EmptyEn:1;      	//	1
   UINT32 	NoFullInt:1;      	//	2
   UINT32 	EmptyInt:1;      	//	3
   UINT32 	Rsvd1:28;			//	31:4
};
union	VPWM_INTCTRL{
	UINT32	all;
	struct 	VPWM_INTCTRL_BITS	bit;
};

// VPWM FIFO Register Bit Definition
struct	VPWM_FIFO_BITS{   //	bits	     description
   UINT32 	FIFO:16;      //	15:0
   UINT32 	Rsvd1:16;		//	31:16
};
union	VPWM_FIFO_REG	{
	UINT32	all;
	struct 	VPWM_FIFO_BITS	bit;
};

// VPWM Registers
struct VPWM_REGS	{
	union   VPWM_TIMER_REG         TIMER;
	union   VPWM_CFG_REG           CFG;
	union   VPWM_PR_REG            PR;
	union   VPWM_DR_REG            DR;
	union   VPWM_INTCTRL			INTCTRL;
	union   VPWM_FIFO_REG          FIFO;
};

/*
*************************** EFC ******************************************
*/
struct 	EFC_TC0_BITS	{
	UINT32	T_pgs:8;	// 7:0
	UINT32	T_nvh1:8;	// 5:15
	UINT32	T_nvh:8;	// 16:23
	UINT32	T_nvs:8;	// 24:31
};
union	EFC_TC0_REG{
	UINT32	all;
	struct 	EFC_TC0_BITS	bit;
};

struct 	EFC_TC1_BITS	{
	UINT32	T_me:8;	// 7:0
	UINT32	T_erase:8;	// 5:15
	UINT32	T_rcv:8;	// 16:23
	UINT32	T_prog:8;	// 24:31
};
union	EFC_TC1_REG{
	UINT32	all;
	struct 	EFC_TC1_BITS	bit;
};

struct 	EFC_TC2_BITS	{
	UINT32	T_rec:4;// 3:0
	UINT32	T_rh:4;	// 7:4
	UINT32	T_rse:4;// 11:8
	UINT32	T_rs:4;	// 15:12
	UINT32	T_pgh:4;// 19:16
	UINT32	T_adh:4;// 23:20
	UINT32	Rev0:8;	// 31:24	
};
union	EFC_TC2_REG{
	UINT32	all;
	struct 	EFC_TC2_BITS	bit;
};

struct 	EFC_TC3_BITS	{
	UINT32	T_sel:4;	// 3:0
	UINT32	T_acc:4;	// 7:4
	UINT32	T_accv:4;	// 11:8
	UINT32	T_ads:4;	// 15:12
	UINT32	Rev0:16;	// 31:16
};
union	EFC_TC3_REG{
	UINT32	all;
	struct 	EFC_TC3_BITS	bit;
};

struct 	EFC_STS_BITS	{
	UINT32	Busy:1;		//0
	UINT32	Buf_empty:1;//1
	UINT32	Buf_ptr:3;	//4:2
	UINT32	Key_en:1;	//5	
	UINT32	Exe_en:1;	//6		
	UINT32	Rev0:17;	// 23:7
	UINT32	Err_num:8;	// 31:24
};
union	EFC_STS_REG{
	UINT32	all;
	struct 	EFC_STS_BITS	bit;
};

struct 	EFC_PEA_BITS	{
	UINT32	Pe_addr:21;	//20:0	
	UINT32	Rev0:11;	// 31:21
};
union	EFC_PEA_REG{
	UINT32	all;
	struct 	EFC_PEA_BITS	bit;
};

#define	EFC_CTRL_ENCRYPT	(1<<1)
struct 	EFC_CTRL_BITS	{
	UINT32	Recall:1;	//0	
	UINT32	Encrypt:1;	//1
	UINT32	Rev0:2;		//3:2
	UINT32	Buf_rst:1;	//4
	UINT32	Rev1:19;	//23:5
	UINT32	EV:1;		//24
	UINT32	PV:1;		//25
	UINT32	PDM:1;		//26
	UINT32	SLM:1;		//27
	UINT32	PROG2:1;	//28
	UINT32	MAS1:1;		//29
	UINT32	Rev2:2;		//31:30
};
union	EFC_CTRL_REG{
	UINT32	all;
	struct 	EFC_CTRL_BITS	bit;
};

struct 	EFC_WEN_BITS	{
	UINT32	We_addr:21;	//20:0	
	UINT32	Rev0:10;	//30:21
	UINT32	Wen:1;		//31
};
union	EFC_WEN_REG{
	UINT32	all;
	struct 	EFC_WEN_BITS	bit;
};

struct 	EFC_MPUINTEN_BITS	{
	UINT32	MpuIntEn:1;	//0	
	UINT32	Rev0:31;	//31:1
};
union	EFC_MPUINTEN_REG{
	UINT32	all;
	struct 	EFC_MPUINTEN_BITS	bit;
};

struct 	EFC_MPUINTSTS_BITS	{
	UINT32	MpuErr:1;	//0	
	UINT32	Rev0:3;		//3:1
	UINT32	MpuJmpErr:1;	//4	
	UINT32	MpuReadErr:1;	//5	
	UINT32	MpuWriteErr:1;	//6	
	UINT32	MpuEraseErr:1;	//7
	UINT32	Rev1:24;	//31:8	
};
union	EFC_MPUINTSTS_REG{
	UINT32	all;
	struct 	EFC_MPUINTSTS_BITS	bit;
};
// EFC Registers
struct EFC_REGS	{
	
	union	EFC_TC0_REG		TC0;	//0x00
	union	EFC_TC1_REG		TC1;	//0x04
	union	EFC_TC2_REG		TC2;	//0x08
	union	EFC_TC3_REG		TC3;	//0x0C
	union	EFC_STS_REG		STS;	//0x10
	union	EFC_PEA_REG		PEA;	//0x14
	union	EFC_CTRL_REG	CTRL;	//0x18
	union	EFC_WEN_REG		WEN;	//0x1C
	UINT32	DATAPRO;				//0x20 Data area start address
	UINT32	ENCRYPT;	   			//0x24 
	UINT32  ADDR_ENC;      			//0x28	
	UINT32  CHECK;    				//0x2C//check accessing code 
	UINT32  KEY_CHK;  				//0x30  //check accessing code 
	union   EFC_MPUINTEN_REG  MPUINTEN;	  		//0x34
	union   EFC_MPUINTSTS_REG  MPUINTSTS;	  		//0x38
	UINT32  RSVD_1;	  		//0x3C
	UINT32  PRE_0;	  		//0x40
	UINT32  PRE_1;	  		//0x44
	UINT32  PRE_2;	  		//0x48
	UINT32  PRE_3;	  		//0x4C
	UINT32  PRE_4;	   		//0x50
	UINT32  PRE_5;	   		//0x54
	UINT32  PRE_6;	   		//0x58
	UINT32  PRE_7;	   		//0x5C	
};

/*
*************************** SQI ******************************************
*/
//--------------- Define Bits for Registers -----------------------------
// define bits of INST or INST_READ
#define	MODE_INST			0
#define MODE_ADDR			2
#define MODE_DUMMY			4
#define	MODE_WAIT			6
#define	MODE_DATA			8
#define NUM_ADDR			12
#define	NUM_DUMMY			16
#define NUM_WAIT			20
#define	NUM_INST			23
#define INST_DATA			24

// define bits of INST_SET
#define	START				0
#define	EN_SPEC				4
#define	EN_CHANNEL			8
#define	EN_ENCRYPT			12
#define CS_LOW				13

// define bits of SPEC
#define	REG2				0
#define	REG1				8

// define bits of CLKGEN
#define	CLK_DIV				0
#define	EN_DIV				7
// SQI INST Bits Definition
struct	SQI_INST_BITS{     //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_REG{
	UINT32	all;
	struct	SQI_INST_BITS	bit;
};

// SQI INST_SET Bits Definition
struct	SQI_INST_SET_BITS{//bits	 description
	UINT32	Start  :1;     //0       start output flag
    UINT32	Rsvd1  :3;     //3:1     Reserved
    UINT32	Spec   :1;     //4       spec flag
    UINT32	Rsvd2  :3;     //7:5     Reserved
    UINT32	Channel:1;     //8       control the channel data flag
    UINT32  Rsvd3  :3;     //11:9    Reserved
    UINT32  Encrypt:1;     //12      encrypt flash data
    UINT32  CsLow  :1;     //13      Hold the chip select
    UINT32	Rsvd4  :18;    //31:14   Reserved
};
union	SQI_INST_SET_REG{
	UINT32	all;
	struct	SQI_INST_SET_BITS	bit;
};

// SQI SPEC Bits Definition
struct	SQI_SPEC_BITS{     //bits	 description
	UINT32 	Addr      :8;   //7:0
    UINT32 	Reg2      :8;   //15:8
    UINT32 	Reg1      :8;   //23:16
    UINT32 	Rsvd      :8;   //31:24    Reserved
};
union	SQI_SPEC_REG{
	UINT32	all;
	struct	SQI_SPEC_BITS	bit;
};

// SQI CLKGEN Bits Definition
struct	SQI_CLKGEN_BITS{   //bits	 description
	UINT32 	ClkDiv  :7;     //6:0     spi CLOCK div
    UINT32 	Divide  :1;     //7
    UINT32 	Rsvd    :24;    //31:9    Reserved
};
union	SQI_CLKGEN_REG{
	UINT32	all;
	struct	SQI_CLKGEN_BITS	bit;
};

// SQI DATA_ADDR Bits Definition
struct	SQI_DATA_ADDR_BITS{   //bits	 description
	UINT32 	IdData  :8;        //7:0      read flash ID
    UINT32 	Rsvd    :24;       //31:8     Reserved
};
union	SQI_DATA_ADDR_REG{
	UINT32	all;
	struct	SQI_DATA_ADDR_BITS	bit;
};

// SQI INST_READ Bits Definition
struct	SQI_INST_READ_BITS{   //bits	 description
	UINT32	Inst   :2;     //1:0     Inst transmit mode
    UINT32	Address:2;     //3:2     Address transmit mode
    UINT32	Dummy  :2;     //5:4     Dummy transmit mode
    UINT32	Wait   :2;     //7:6     Wait transmit mode
    UINT32	Data   :2;     //9:8     Data transmit mode
    UINT32	Rsvd1  :2;     //11:10   Reserved
    UINT32  AddNum :2;     //13:12   Address number
    UINT32  Rsvd2  :2;     //15:14   Reserved
    UINT32  DuNum  :3;     //18:16   Dummy number
    UINT32  Rsvd3  :1;     //19      Reserved
    UINT32  WaiNum :3;     //22:20
    UINT32  InNum  :1;     //23      Inst number
    UINT32  InData :8;     //31:24
};
union	SQI_INST_READ_REG{
	UINT32	all;
	struct	SQI_INST_READ_BITS  bit;
};

// SQI registers
struct	SQI_REGS{
	union 	SQI_INST_REG        INST;
	union 	SQI_INST_SET_REG    INST_SET;
	union 	SQI_SPEC_REG        SPEC;
	union 	SQI_CLKGEN_REG      CLKGEN;
	union 	SQI_DATA_ADDR_REG   DATA_ADDR;
	union 	SQI_INST_READ_REG   INST_READ;
	UINT32 	ENCRYPT;
	UINT32 	CHECK;
	UINT32 	DATAPRO;
	UINT32 	DATAOPEN;
};

/*
*************************** CRC ******************************************
*/
// CRC control register Bit Definitions
struct	CRC_CTRL_BITS	{//	bits	description
	UINT32	ArithSel:1;		//  0		Algorithm Selection
	UINT32	DatRev:1;		//	1		Input data reverse
	UINT32	ResRev:1;		//	2		Output Descending
	UINT32	Rsvd1:29;		//	31:3	Reserved
};
union	CRC_CTRL_REG{
	UINT32	all;
	struct	CRC_CTRL_BITS	bit;
};

// USART Registers
struct	CRC_REGS	{
	union 	CRC_CTRL_REG	CTRL;		//CRC control register					
	UINT32	DATA;			//CRC data registers
	UINT32	INITVAL;		//CRC initial value register			
	UINT32	RESULT;			//CRC results output register
};

/*
*************************** BVD ******************************************
*/
// Define bits of BVD_CTRL
#define CTRL_CLKDIV		8
#define CTRL_SOC		4
#define	CTRL_RSTAN		2
#define CTRL_VINSEL		1

// BVD Control Register Bit Definition
struct	BVD_CTRL_BITS{  //bits	    description
	UINT32 	Rsvd0:1;	//0			reserved
	UINT32 	VinSel:1;   //1   		voltage Range Select
	UINT32 	RstAn:1;   	//2   	    Reset
	UINT32 	Rvsd1:1;	//3			reserved
	UINT32 	Soc:1;      //4         start control
	UINT32 	Rsvd2:3;	//5:7		reserved
	UINT32 	ClkDiv:16;	//23:8		clock divider
	UINT32 	Rsvd3:8;    //31:24   	reserved
};
union	BVD_CTRL_REG	{
	UINT32	all;
	struct	BVD_CTRL_BITS	bit;
};

// BVD STAT Register Bit Definition
struct	BVD_STAT_BITS{    //	bits	     description
   UINT32 	Result:3;        //  2:0           Result
   UINT32 	Rsvd1:1;   	   //	3	   	     reserved
   UINT32 	Eoc:1;		   //	4			 end 
   UINT32 	Eoclf:1;		   //	5			 Convert finish flag
   UINT32 	Rsvd2:26;    	   //  31:6			 reserved
};
union	BVD_STAT_REG	{
	UINT32	all;
	struct	BVD_STAT_BITS	bit;
};

// BVD Registers
struct BVD_REGS	{
	union 	BVD_CTRL_REG		CTRL;
	union 	BVD_STAT_REG      STAT;
};

/*
*************************** LOCSC ******************************************
*/
struct LOCSC_CTRL_BITS
{
    UINT32 	VEn:1;        	//0     Enable
    UINT32 	VActive:1;    	//1     
    UINT32 	HActive:1;    	//2
    UINT32 	PEdge:1;      	//3     0 up 1 down
    UINT32 	Rsvd1:4;    	//4-7   reserved
    UINT32 	HMode:2;      	//8 9
    UINT32 	Rsvd2:6;      	//10-15 reserved
    UINT32 	PMode:2;      	//16-17 
    UINT32 	Rsvd3:6;      	//18-23
    UINT32 	PixelInv:1;   	//24
    UINT32 	Rsvd4:7;      	//25-31
};
union LOCSC_CTRL_REG
{
	UINT32	all;
	struct	LOCSC_CTRL_BITS	bit;
};

struct LOCSC_PCOUNTER_BITS
{ 
    UINT32 	PBlankCounter:16;
    UINT32 	PStoreCounter:16;
};
union LOCSC_PCOUNTER_REG
{
	UINT32	all;
	struct	LOCSC_PCOUNTER_BITS	bit;
};

struct LOCSC_HCOUNTER_BITS
{
    UINT32 	HBlankCounter:16;
    UINT32 	HStoreCounter:16;	
};
union LOCSC_HCOUNTER_REG
{
	UINT32	all;
	struct	LOCSC_HCOUNTER_BITS	bit;
};

struct LOCSC_INT_STATUS_BITS
{
    UINT32 	FifoFull:1;   	//0
    UINT32 	Rsvd1:7;    	//reserved
    UINT32 	FifoEmpty:1;  	//0
    UINT32 	Rsvd2:7;    	//reserved
    UINT32 	Vsync:1;      	//0
    UINT32 	Rsvd3:7;    	//reserved
    UINT32 	Hsync:1;      	//0
    UINT32 	Rsvd4:7;    	//reserved        
};
union LOCSC_INT_STATUS_REG
{
	UINT32	all;
	struct	LOCSC_INT_STATUS_BITS	bit;
};

struct LOCSC_INT_BITS
{
    UINT32 	FifoFull:1;   	//0
    UINT32 	FifoEmpty:1;  	//1
    UINT32 	Rsvd1:6;    	//reserved
    UINT32 	VStart:1;     	//8
    UINT32 	VEnd:1;       	//9
    UINT32 	Rsvd2:6;    	//reserved
    UINT32 	HStart:1;     	//16
    UINT32 	HEnd:1;       	//17
    UINT32 	Rsvd3:6;    	//reserved	
    UINT32 	FIFOCnt:1;
    UINT32 	Rsvd4:7;
};

union LOCSC_INT_REG
{
	UINT32	all;
	struct	LOCSC_INT_BITS	bit;
};

struct LOCSC_REGS
{
    union 	LOCSC_CTRL_REG 			CTRL;
    union 	LOCSC_PCOUNTER_REG 		PCOUNTER;
    union 	LOCSC_HCOUNTER_REG 		HCOUNTER;
    union 	LOCSC_INT_STATUS_REG 	INT_STATUS;
    union 	LOCSC_INT_REG 			INT_FLAG;
    union 	LOCSC_INT_REG 			INT_EN;
    UINT32 	FIFO_CNT;
    UINT32 	FIFO;
    UINT32 	INT_CNT;
};

/*
*************************** TRNG ******************************************
*/
// TRNG Control Register Bit Definition
struct TRNG_CTRL_BITS	{		//	bits	description
	UINT32 	GenNewRandom:1; 	//	0 		Generate new random
	UINT32 	Rsvd:27;			//	27:1 	reserved
	UINT32 	NonceLfsrSelect:1;	//	28 		Select Nonce
	UINT32 	NonceReseedLD:1;	//	29 		load nonce seed
	UINT32 	NonceReseed:1;		//	30 		nonce reseed
	UINT32 	RandReseed:1;		//	31 		random seed
};
union TRNG_CTRL_REG{
	UINT32	all;
	struct	TRNG_CTRL_BITS bit;
};

// TRNG Status Register Bit Definition
struct TRNG_STATUS_BITS	{	//	bits		description
	UINT32 	Rsvd1:27;		//	26:0 		reserved
	UINT32 	TrngRdyIrq:1;	//	27 			trng data is ready
	UINT32 	Rsvd2:4;		//	31:28 		reserved
};
union TRNG_STATUS_REG{
	UINT32 	all;
	struct 	TRNG_STATUS_BITS bit;
};

// TRNG Interrupt Enable Register Bit Definition
struct TRNG_INTEN_BITS{		//	bits		description
	UINT32 	Rsvd1:27;		//	26:0 		reserved
	UINT32 	TrngIrqEn:1;	//	27 			enable trng irq
	UINT32 	Rsvd2:4;		//	31:28 		reserved
};
union TRNG_INTEN_REG{
	UINT32 	all;
	struct 	TRNG_INTEN_BITS bit;
};

//-----all TRNG registers--
struct	RNG_REGS{
	union 	TRNG_CTRL_REG         CTRL; 	// Control register 0
	union 	TRNG_STATUS_REG       STATUS;	// Status register  4
	union 	TRNG_INTEN_REG        INTEN;	// Int enable register 8
	UINT32 	POWERDOWN;						// reserved   c
	UINT32 	RAND0;							// 128bit random bit[31:0] 10
	UINT32 	RAND1;							// 128bit random bit[63:32] 14
	UINT32 	RAND2;							// 128bit random bit[95:64] 18
	UINT32 	RAND3;							// 128bit random bit[127:96] 1c
	UINT32 	PRCL;							// Low 16 bit for prng 20
	UINT32 	PRCH;							// High 16 bit for prng 24
	UINT32 	PRSD;							// seed of prng 28
};

/*
*************************** DMA ******************************************
*/
// define DMA Channels
#define	 DMA_CHN0 	0
#define	 DMA_CHN1	1
#define	 DMA_CHN2	2
#define	 DMA_CHN3	3

#define	 MASK_DMA_CHN0 			(1<<DMA_CHN0)
#define	 MASK_DMA_CHN1 			(1<<DMA_CHN1)
#define	 MASK_DMA_CHN2 			(1<<DMA_CHN2)
#define	 MASK_DMA_CHN3			(1<<DMA_CHN3)

#define	 DMA_CHANNEL0_INT 		(1<<0)
#define	 DMA_CHANNEL1_INT 		(1<<1)
#define	 DMA_CHANNEL2_INT 		(1<<2)
#define	 DMA_CHANNEL3_INT		(1<<3)
#define	 DMA_CHANNEL_INT_ALL		0xF
// Channel Registers
// DMA Control Register for Channel x Bits Definition
#define	DMA_INT_EN			0
#define	DMA_DST_TR_WIDTH	1
#define	DMA_SRC_TR_WIDTH	4
#define	DMA_DINC			7
#define	DMA_SINC			9
#define	DMA_DEST_MSIZE		11
#define	DMA_SRC_MSIZE		14
#define	DMA_SRC_GATHER_EN	17
#define	DMA_DST_SCATTER_EN	18
#define	DMA_TT_FC			20
#define	DMA_DMS				23
#define	DMA_SMS				25
#define	DMA_LLP_DST_EN		27
#define	DMA_LLP_SRC_EN		28

struct	DMA_CHN_CTL_REG_L_BITS	{	// bits		description
	UINT32	IntEn: 1;			   	//	0		Interrupt Enable Bit		
	UINT32	DstTrWidth:	3; 			// 	3:1		Destination Transfer Width
	UINT32	SrcTrWidth:	3; 	 		//	6:4		Source Transfer Width
	UINT32	Dinc:	2; 	   			//	8:7		Destination Address Increment
	UINT32	Sinc:	2; 			   	//	10:9	Source Address Increment
	UINT32 	DestMsize:	3;	  		//	13:11	Destination Burst Transfer Length
	UINT32	SrcMsize:	3; 		  	//	16:14	Source Burst Transfer Length
	UINT32	SrcGatherEn:	1; 		//	17		Source Gather Enable Bit
	UINT32	DstScatterEn:	1; 	 	// 	18		Destination Scatter Enable Bit
	UINT32	Rsvd0:	1; 			  	//	19		Reserved
	UINT32	TtFc:	3;				//	22:20	Transfer Type and Flow Control
	UINT32	Dms:	2;				//	24:23	Destination Master Select
	UINT32	Sms:	2; 				//	26:25	Source Master Select
	UINT32	LlpDstEn:	1;	 		//	27		block chaining is enable on the destination side
	UINT32	LlpSrcEn:	1;			//	28		block chaining is enable on the source side
	UINT32	Rsvd1:	3; 				// 	31:29	Reserved
};

union 	DMA_CHN_CTL_REG_L	{
	UINT32	all;
	struct	DMA_CHN_CTL_REG_L_BITS	bit;
};

#define	DMA_BLOCK_TS		0
#define	DMA_DONE			1
struct	DMA_CHN_CTL_REG_H_BITS	{	// bits		description
	UINT32	BlockTs:12;		   		//	11:0	Block Transfer Size
	UINT32	Done:	1;	   			//	12		Done bit
	UINT32	Rsvd0:	19;	  			//	31:13	Reserved
};
union 	DMA_CHN_CTL_REG_H	{
	UINT32	all;
	struct	DMA_CHN_CTL_REG_H_BITS	bit;
};

// DMA Configuration Register for Channel x Bits Definition
#define	DMA_CH_PRIOR		5
#define	DMA_CH_SUSP			8
#define	DMA_FIFO_EMPTY		9
#define	DMA_HS_SEL_DST		10
#define	DMA_HS_SEL_SRC		11
#define	DMA_LOCK_CH_L		12
#define	DMA_LOCK_B_L		14
#define	DMA_LOCK_CH			16
#define	DMA_LOCK_B			17
#define	DMA_DST_HS_POL		18
#define	DMA_SRC_HS_POL		19
#define	DMA_MAX_ABRST		20
#define	DMA_RELOAD_SRC		30
#define	DMA_RELOAD_DST		31
struct	DMA_CHN_CFG_REG_L_BITS{	// bits		description
	UINT32	Rsvd:		5; 		//	4:0		Reserved
	UINT32	ChPrior:	3;	   	//	7:5		Channel priority
	UINT32	ChSusp:	 	1;		//	8		Channel Suspend
	UINT32	FifoEmpty:	1;	 	//	9		Indicates if there is data left in the channel FIFO
	UINT32	HsSelDst:	1;		//	10		Destination Software or Hardware Handshaking Select
	UINT32	HsSelSrc:	1;	 	//	11		Source Software or Hardware Handshaking Select
	UINT32	LockChL:	2; 		//	13:12	Channel Lock Level
	UINT32	LockBL:		2;	 	//	15:14	Bus Lock Level
	UINT32	LockCh:		1;	 	//	16		Channel Lock Bit
	UINT32	LockB:		1;	   	//	17		Bus Lock Bit
	UINT32	DstHsPol:	1; 		//	18		Destination Handshaking Interface Polarity
	UINT32	SrcHsPol:	1;	   	//	19		Source Handshaking Interface Polarity
	UINT32	MaxAbrst:	10;	 	//	29:20	Maximum AMBA Burst Length
	UINT32	ReloadSrc:	1;		// 	30		Automatic Source Reload
	UINT32	ReloadDst:	1;		//	31		Automatic Destination Reload	
};
union 	DMA_CHN_CFG_REG_L	{
	UINT32	all;
	struct	DMA_CHN_CFG_REG_L_BITS	bit;
};

#define	DMA_FCMODE			0
#define	DMA_FIFO_MODE		1
#define	DMA_PROTCTL			2
#define	DMA_DS_UPD_EN		5
#define	DMA_SS_UPD_EN		6
#define	DMA_SRC_PER			7
#define	DMA_DEST_PER		11
struct	DMA_CHN_CFG_REG_H_BITS{	// bits		description
	UINT32	Fcmode:		1;	 	//	0		Flow Control Mode
	UINT32	FifoMode:	1;	  	//	1		FFIO Mode Select
	UINT32	ProtCtl:	3; 		//	4:2		Protection Control
	UINT32	DsUpdEn:	1;	 	//	5		Destination Status Update Enable
	UINT32	SsUpdEn:	1; 		//	6		Source Status Update Enable
	UINT32	SrcPer:		4;		//	10:7	Assigns a hardware handshaking interface to the soure
	UINT32	DestPer:	4;		//	14:11	Assigns a hardware handshaking interface to the destination
	UINT32	Rsvd:	    17;		//	31:15	Reserved	
};
union 	DMA_CHN_CFG_REG_H	{
	UINT32	all;
	struct	DMA_CHN_CFG_REG_H_BITS	bit;
};

// DMA Source Gather Register for Channel x Bits Definition
struct	DMA_CHN_SGR_REG_BITS{	// bits		description
	UINT32	Sgi:	20;	 		// 	19:0	Source gather interval
	UINT32	Sgc:	12;	  		// 	31:20	Source gather count
};
union 	DMA_CHN_SGR_REG	{
	UINT32	all;
	struct	DMA_CHN_SGR_REG_BITS	bit;
};

// DMA Destination Scatter Register for Channel x Bits Definition
struct	DMA_CHN_DST_REG_BITS{	// bits		description
	UINT32	Dsi:	20;			// 	19:0	Destination scatter interval
	UINT32	Dsc:	12;	  		// 	31:20	Destination scatter count
};
union 	DMA_CHN_DST_REG	{
	UINT32	all;
	struct	DMA_CHN_DST_REG_BITS	bit;
};

// DMA Channel Registers struct
struct DMA_CH_REG {
	UINT32	SAR;
	UINT32	RSVD0;
	UINT32	DAR;
	UINT32	RSVD1;
	UINT32  LLP;
	UINT32	RSVD2;
	union 	DMA_CHN_CTL_REG_L  CTL_L;
	union 	DMA_CHN_CTL_REG_H  CTL_H;
	UINT32	SSTAT;
	UINT32	RSVD3;
	UINT32	DSTAT;
	UINT32	RSVD4;
	UINT32	SSTATAR;
	UINT32	RSVD5;
	UINT32	DSTATAR;
	UINT32	RSVD6;
	union 	DMA_CHN_CFG_REG_L	CFG_L;
	union 	DMA_CHN_CFG_REG_H	CFG_H;	
	union	DMA_CHN_SGR_REG SGR;
	UINT32	RSVD7;
	union	DMA_CHN_DST_REG DSR;
	UINT32	RSVD8;
};

// Interrupt Registers
// DMA Raw Status / Status / Clear Register Bits Definition
struct DMA_INT_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Revd:28;
};
union DMA_INT_REG {
	UINT32	all;
	struct	DMA_INT_REG_BITS	bit;
};

// DMA Mask Register Bits Definition
struct DMA_CHN_INT_MASK_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;	
};
union DMA_CHN_INT_MASK_REG {
	UINT32	all;
	struct	DMA_CHN_INT_MASK_REG_BITS	bit;
};

// DMA Combined Interrupt Status Register Bits Definition
struct DMA_COMB_INT_STATUS_BITS{	// bits		description
	UINT32	Tfr:	1; 						
	UINT32	Block:	1;
	UINT32	Srct:	1;
	UINT32	Dstt:	1;
	UINT32	Err:	1;
	UINT32	Rsvd:	27;
};
union DMA_COMB_INT_STATUS_REG {
	UINT32	all;
	struct	DMA_COMB_INT_STATUS_BITS	bit;
};

// Software Handshaking Registers
// DMA Source Software Transaction Register Bits Definition
struct DMA_REQ_REG_BITS{
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;	
};
union DMA_REQ_REG {
	UINT32	all;
	struct	DMA_REQ_REG_BITS	bit;
};

// Miscellaneous Registers
// DMA Configuration Register Bits Definition
struct DMA_CFG_REG_BITS{		  
	UINT32	DmaEn:	1;
	UINT32	Rsvd:	31;
};
union DMA_CFG_REG {
	UINT32	all;
	struct	DMA_CFG_REG_BITS	bit;
};

// DMA Enable Register Bits Definition
struct DMA_CHN_EN_REG_BITS{	  
	UINT32	Ch0:1;
	UINT32	Ch1:1;
	UINT32	Ch2:1;
	UINT32	Ch3:1;
	UINT32	Rsvd0:4;
	UINT32	WeCh0:	1;
	UINT32	WeCh1:	1;
	UINT32	WeCh2:	1;
	UINT32	WeCh3:	1;
	UINT32	Rsvd1:	20;
};
union DMA_CH_EN_REG {
	UINT32	all;
	struct	DMA_CHN_EN_REG_BITS bit;
};


// DMA Component Parameters Register Bits Definition
struct DMA_CH_PARAMS_REG_BITS{	  
	UINT32	Dtw:3;
	UINT32	Stw:3;
	UINT32	StatDst:1;
	UINT32	StatSrc:1;
	UINT32	DstScaEn:1;
	UINT32	SrcGatEn:1;
	UINT32	LockEn:	1;
	UINT32	MultiBlkEn:1;
	UINT32	CtlWbEn:1;
	UINT32	HcLlp:1;
	UINT32	Fc:2;
	UINT32	MaxMultSize:3;
	UINT32	Dms:3;
	UINT32	Lms:3;
	UINT32	Sms:3;
	UINT32	FifoDepth:3;
	UINT32	Rsvd:1;
};
union DMA_CH_PARAMS_REG {
	UINT32	all;
	struct	DMA_CH_PARAMS_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MULTI_BLK_TYPE_REG_BITS{	  
	UINT32	Ch0:4;
	UINT32	Ch1:4;
	UINT32	Ch2:4;
	UINT32	Ch3:4;
	UINT32	Ch4:4;
	UINT32	Ch5:4;
	UINT32	Ch6:4;
	UINT32	Ch7:4;
};
union DMA_MULTI_BLK_TYPE_REG {
	UINT32	all;
	struct	DMA_MULTI_BLK_TYPE_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MAX_BLK_SIZE_REG_BITS{	  
	UINT32	Ch0:4;
	UINT32	Ch1:4;
	UINT32	Ch2:4;
	UINT32	Ch3:4;
	UINT32	Ch4:4;
	UINT32	Ch5:4;
	UINT32	Ch6:4;
	UINT32	Ch7:4;
};
union DMA_MAX_BLK_SIZE_REG {
	UINT32	all;
	struct	DMA_MAX_BLK_SIZE_REG_BITS bit;
};

// DMA Component Parameters Register Bits Definition
struct DMA_MISC_PARAMS_REG_BITS{	  
	UINT32	BigEndian:	1;
	UINT32	IntrIo:	2;
	UINT32	Mabrst:	1;
	UINT32	Rsvd0:	4;
	UINT32	NumChannels:	3;
	UINT32	NumMasterInt:	2;
	UINT32	ShdataWidth:	2;
	UINT32	M1HdataWidth:	2;
	UINT32	M2HdataWidth:	2;
	UINT32	M3HdataWidth:	2;
	UINT32	M4HdataWidth:	2;
	UINT32	NumHsInt:	5;
	UINT32	AddEnCodedParams:	1;
	UINT32	StaticEndianSelect:	1;
	UINT32	Rsvd1:	2;
};
union DMA_MISC_PARAMS_REG {
	UINT32	all;
	struct	DMA_MISC_PARAMS_REG_BITS bit;
};

// DMA Registers 
struct	DMA_REGS{
	//1.Channel Register
	struct  DMA_CH_REG CH[4];						//0x000-0x158
	struct  DMA_CH_REG CH_RSVD[4];					//0x160-0x2b8

   	//2.Interrupt Register
	union	DMA_INT_REG	  RAW_TFR;	   				//0x2c0
	UINT32	RSVD1;
	union	DMA_INT_REG	  RAW_BLOCK;				//0x2c8
	UINT32	RSVD2;
	union	DMA_INT_REG   RAW_SRC_TRAN;				//0x2d0
	UINT32	RSVD3;
	union	DMA_INT_REG   RAW_DST_TRAN;				//0x2d8
	UINT32	RSVD4;
	union	DMA_INT_REG	  RAW_ERR;					//0x2e0
	UINT32	RSVD5;

	union	DMA_INT_REG	  STATUS_TFR;				//0x2e8
	UINT32	RSVD6;
	union	DMA_INT_REG	  STATUS_BLOCK;   			//0x2f0
	UINT32	RSVD7;
	union	DMA_INT_REG	  STATUS_SRC_TRAN; 			//0x2f8
	UINT32	RSVD8;
	union	DMA_INT_REG	  STATUS_DST_TRAN; 			//0x300
	UINT32	RSVD9;
	union	DMA_INT_REG	  STSTUS_ERR;	   			//0x308
	UINT32	RSVD10;

	union	DMA_CHN_INT_MASK_REG	MASK_TFR;		//0x310
	UINT32	RSVD11;
	union	DMA_CHN_INT_MASK_REG	MASK_BLOCK;	   	//0x318
	UINT32	RSVD12;
	union	DMA_CHN_INT_MASK_REG	MASK_SRC_TRAN; 	//0x320
	UINT32	RSVD13;
	union	DMA_CHN_INT_MASK_REG	MASK_DST_TRAN;	//0x328
	UINT32	RSVD14;
	union	DMA_CHN_INT_MASK_REG	MASK_ERR;		//0x330
	UINT32	RSVD15;

	union	DMA_INT_REG 	CLEAR_TFR;	   			//0x338
	UINT32	RSVD16;
	union	DMA_INT_REG 	CLEAR_BLOCK;			//0x340
	UINT32	RSVD17;
	union	DMA_INT_REG 	CLEAR_SRC_TRAN; 		//0x348
	UINT32	RSVD18;
	union	DMA_INT_REG 	CLEAR_DST_TRAN; 		//0x350
	UINT32	RSVD19;
	union	DMA_INT_REG 	CLEAR_ERR;	   			//0x358
	UINT32	RSVD20;

	union	DMA_COMB_INT_STATUS_REG	INT_STATUS;	  	//0x360
	UINT32	RSVD21;

	//3.Software Handshaking Registers
	union	DMA_REQ_REG  	REQ_SRC_REG;	 		//0x368
	UINT32	RSVD22;
	union	DMA_REQ_REG 	REQ_DST_REG;	 		//0x370
	UINT32	RSVD23;
	union	DMA_REQ_REG 	SGL_REQ_SRC_REG; 		//0x378
	UINT32	RSVD24;
	union	DMA_REQ_REG 	SGL_REQ_DST_REG; 		//0x380
	UINT32	RSVD25;
	union	DMA_REQ_REG 	LST_SRC_REG;	 		//0x388
	UINT32	RSVD26;
	union	DMA_REQ_REG 	LST_DST_REG;	 		//0x390
	UINT32	RSVD27;

	//4.Miscellaneous Registers
	union	DMA_CFG_REG		CFG_REG;	  //0x398
	UINT32	RSVD28;
	union	DMA_CH_EN_REG	CH_EN_REG;	  //0x3a0
	UINT32	RSVD29;
	UINT32	ID_REG;	  	//0x3a8
	UINT32	RSVD30;
	UINT32	TEST_REG;	//0x3b0
	UINT32	RSVD31;
	UINT32	RSVD32[4];

  // Parameters Register
  	UINT32	RSVD33;	  	// 0x3c8
	union 	DMA_CH_PARAMS_REG		PARAMS_CH7;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH6;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH5;	
	union 	DMA_CH_PARAMS_REG		PARAMS_CH4;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH3;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH2;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH1;
	union 	DMA_CH_PARAMS_REG		PARAMS_CH0;
	union 	DMA_MULTI_BLK_TYPE_REG	PARAMS_MULTI_BLK_TYPE;
	union 	DMA_MAX_BLK_SIZE_REG	MAX_BLK_SIZE;
	union 	DMA_MISC_PARAMS_REG		MISC_PARAMS;
};

/***************************************************************************
* Include Header Files
***************************************************************************/
// all peripheral header files


/**************************************************************************
* Global Variable Declaration
***************************************************************************/
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
extern volatile struct SCM_REGS ScmRegs;
extern volatile struct GPIO_REGS GpioRegs;;
extern volatile struct TMR_REGS T0Regs;
extern volatile struct USART_REGS Spi0Regs;
extern volatile struct RNG_REGS RngRegs;
extern volatile struct TMR_REGS T1Regs;
extern volatile struct TMR_REGS T2Regs;
extern volatile struct SSI_REGS SsiRegs;
extern volatile struct USART_REGS Uart0Regs;
extern volatile struct USART_REGS I2C0Regs;
extern volatile struct SWPS_REGS SwpsRegs;
extern volatile struct ISO7816_REGS Iso7816Regs;
extern volatile struct SCI_REGS SciRegs;
extern volatile struct VPWM_REGS VpwmRegs;
extern volatile struct USART_REGS Uart1Regs;
extern volatile struct DMA_REGS DmaRegs;
extern volatile struct EFC_REGS EfcRegs;
extern volatile struct SQI_REGS SqiRegs;
extern volatile struct CRC_REGS CrcRegs;
extern volatile struct BVD_REGS BvdRegs;
extern volatile struct LOCSC_REGS LocscRegs;
extern volatile struct USART_REGS Spi1Regs;
extern volatile struct USB_REGS UsbRegs;
#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
__NO_INIT volatile struct SCM_REGS ScmRegs 			__AT(SCM_BASE);
__NO_INIT volatile struct GPIO_REGS GpioRegs 		__AT(GPIO_BASE);
__NO_INIT volatile struct TMR_REGS T0Regs			__AT(T0_BASE);
__NO_INIT volatile struct USART_REGS Spi0Regs		__AT(SPI0_BASE);
__NO_INIT volatile struct RNG_REGS RngRegs		__AT(TRNG_BASE);
__NO_INIT volatile struct TMR_REGS T1Regs			__AT(T1_BASE);
__NO_INIT volatile struct TMR_REGS T2Regs			__AT(T2_BASE);
__NO_INIT volatile struct SSI_REGS SsiRegs			__AT(SSI_BASE);
__NO_INIT volatile struct USART_REGS Uart0Regs		__AT(UART0_BASE);
__NO_INIT volatile struct USART_REGS I2C0Regs		__AT(I2C0_BASE);
__NO_INIT volatile struct SWPS_REGS SwpsRegs		__AT(SWPS_BASE);
__NO_INIT volatile struct ISO7816_REGS Iso7816Regs	__AT(ISO7816_BASE);
__NO_INIT volatile struct SCI_REGS SciRegs			__AT(SCI_BASE);
__NO_INIT volatile struct VPWM_REGS VpwmRegs		__AT(VPWM_BASE);
__NO_INIT volatile struct USART_REGS Uart1Regs		__AT(UART1_BASE);
__NO_INIT volatile struct DMA_REGS DmaRegs			__AT(DMA_BASE);
__NO_INIT volatile struct EFC_REGS EfcRegs			__AT(EFC_BASE);
__NO_INIT volatile struct SQI_REGS SqiRegs			__AT(SQI_BASE);
__NO_INIT volatile struct CRC_REGS CrcRegs			__AT(CRC_BASE);
__NO_INIT volatile struct BVD_REGS BvdRegs			__AT(BVD_BASE);
__NO_INIT volatile struct LOCSC_REGS LocscRegs		__AT(LOCSC_BASE);
__NO_INIT volatile struct USART_REGS Spi1Regs		__AT(SPI1_BASE);
__NO_INIT volatile struct USB_REGS UsbRegs			__AT(USB_BASE);

#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif



/**************************************************************************
* Global Functon Declaration
***************************************************************************/

#endif /*SY09_DEVICE_H_*/
