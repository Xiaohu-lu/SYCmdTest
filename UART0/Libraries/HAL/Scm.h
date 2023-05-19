/***************************************************************************
* File name    :	Scm.h
* Function     :	Header of System Control Module
* Author       : 	coins
* Date         :	2012/07/03
* Version      :    v1.0
* Description  :    
* ModifyRecord :
****************************************************************************/
#ifndef SCM_H_
#define SCM_H_

/***************************************************************************
* Include Header Files
***************************************************************************/


/**************************************************************************
* Global Macro Definition
***************************************************************************/

// define the default system clock frequency
#define		F_DEF_SYSCLK	(F_HFROSC/4)

// define the system tick stclk clock frequency
#define		F_SYST_STCLK	(F_LFROSC/4)

// define the clock source for SysTick
#define		SYST_STCLK	0	// the STCLK for SysTick
#define		SYST_SCLK	1	// the SCLK for SysTick  
// define the work mode	for SysTick
#define		SYST_CNT	0	// SysTick count but no interrupt
#define		SYST_INT	1  	// SysTick count and interrupt when down to 0
	
#define POWER_MODE_SEL_ADDR  		0x100807F4
#define HF_96M_TRIM_FLAG_ADDR  		0x100807C8
#define HF_96M_TRIM_ADDR  			0x100807CA
#define HF_144M_TRIM_FLAG_ADDR  	0x100807C4
#define HF_144M_TRIM_ADDR  			0x100807C6
#define HF_TRIM_FLAG				0x89AB

// Define the analog module
#define	AN_HFROSC			(1<<0)
#define	AN_ROM				(1<<4)
#define	AN_TRNG_RING		(1<<6)
#define	AN_USB_PHY			(1<<7)
#define	AN_TPR				(1<<8)
#define	AN_BVD				(1<<16)
#define	AN_SWPPowerInLel	(1<<18)	
#define	AN_AutoPowerSwitch	(1<<19)


//Destruct type
#define	DESTRUCT_ROM		(1<<27)
#define	DESTRUCT_SRAM		(1<<26)
#define	DESTRUCT_SDI0		(1<<25)
#define	DESTRUCT_SDI1		(1<<24)
#define	DESTRUCT_SHIELD		(1<<17)
#define	DESTRUCT_COREVD		(1<<16)
#define	DESTRUCT_VDD18VD	(1<<10)
#define	DESTRUCT_TD			(1<<8)
#define	DESTRUCT_IOVD		(1<<1)	
//Rst type
#define	RST_SRAMERRSEL		(1<<26)
#define	RST_SDI0SEL			(1<<25)
#define	RST_SDI1SEL			(1<<24)
#define	RST_SHIELD			(1<<17)
#define	RST_COREVDSEL		(1<<16)
#define	RST_VDD18BODSEL		(1<<11)
#define	RST_VDD18VDSEL		(1<<10)
#define	RST_TD				(1<<8)
#define	RST_IOVDSEL			(1<<1)
#define	RST_IOBODSEL		(1<<0)

/* SysTick constants */
#define SYSTICK_ENABLE		0      			/* Config-Bit to start or stop the SysTick Timer                         */
#define SYSTICK_TICKINT		1      			/* Config-Bit to enable or disable the SysTick interrupt                 */
#define SYSTICK_CLKSOURCE	2      			/* Clocksource has the offset 2 in SysTick Control and Status Register   */
#define SYSTICK_MAXCOUNT	((1<<24) -1)	/* SysTick MaxCount*/


/**************************************************************************
* Global Type Definition
***************************************************************************/
// define the system clock for setting
// HFROSC_Grade << 4 + FclkSel
typedef	enum	{
	SYS_CLK_12M		= 0x20,
   	SYS_CLK_24M		= 0x21,
	SYS_CLK_48M		= 0x22,
	SYS_CLK_96M		= 0x23,

	SYS_CLK_18M		= 0x30,
	SYS_CLK_36M		= 0x31,
	SYS_CLK_72M		= 0x32,
	SYS_CLK_144M	= 0x33,

	SYS_CLK_6M		= 0x10,

	SYS_CLK_1_5M	= 0x00,
	SYS_CLK_3M		= 0x01,
}SYS_CLK;

// define the vector table memory map type
typedef enum	{
	VT_MEM_ROM 		= 0,
	VT_MEM_SRAM 	= 1,
	VT_MEM_FLASH 	= 2,
}VT_MEM;
/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Global Functon Declaration
***************************************************************************/

// System Clock
extern		UINT8 SCM_SetSysClk(SYS_CLK Clk);
extern		UINT32 SCM_GetSysClk(void);
extern		void SCM_SetSysClkLowFreq(void);

UINT8 SCM_RegWrEn(void);

// Analog Module Power control
void SCM_AnModPowerEnable(UINT32 nAnMods);
void SCM_AnModPowerDisable(UINT32 nAnMods);

// HFROSC
UINT8 SCM_LoadHFRoscTrim(void);

//USB
void SCM_UsbPhySet(void);
void SCM_EnableUsbPuRes(void);
void SCM_DisableUsbPuRes(void);

// System Tick
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode);

// Delay
void _delay_ms(UINT32 ms);
void _delay_us(UINT32 us);

extern	void DeepSleepAutoSelClkEn(void);

/**************************************************************************
* Functon
***************************************************************************/
/****************************************************************************
 * Subroutine:	SCM_GetSOCID
 * Function:	to get SOC ID Code
 * Input:		None 
 * Output:		return SOC ID	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT32 SCM_GetSOCID(void)
{
	return ScmRegs.SOCID;
}

/****************************************************************************
 * Subroutine:	SCM_DisableModClk
 * Function:	to disable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableModClk(UINT8 ModId)
{
	ScmRegs.MODCLKCTRL &= ~(1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_EnableModClk
 * Function:	to enable Module clock
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_EnableModClk(UINT8 ModId)
{
	ScmRegs.MODCLKCTRL |= (1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_KeepModRst
 * Function:	to keep Module in reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_KeepModRst(UINT8 ModId)
{
	ScmRegs.MODRSTCTRL &= ~(1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_ReleaseModRst
 * Function:	to release Module from reset state
 * Input:		UINT8 ModId --	to specify the module ID 
 * Output:		None	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_ReleaseModRst(UINT8 ModId)
{
	ScmRegs.MODRSTCTRL |= (1<<ModId);
}

/****************************************************************************
 * Subroutine:	SCM_EnableRst
 * Function:	Enable Reset
 * Input:		UINT32 RstType -- Rst type
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_EnableRst(UINT32 RstType)
{
	ScmRegs.SYSRSTCTRL.all |= (RstType);
	if(RstType == RST_SDI1SEL)
		GpioRegs.GPBUSE0.bit.GPB2 = MUX_SDIN;
	else if(RstType == RST_SDI0SEL)
		GpioRegs.GPAUSE0.bit.GPA4 = MUX_SDIN;
}

/****************************************************************************
 * Subroutine:	SCM_DisableRst
 * Function:	Disable Reset
 * Input:		UINT32 RstType --	Rst type
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableRst(UINT32 RstType)
{
	ScmRegs.SYSRSTCTRL.all &= ~(RstType);
}

/****************************************************************************
 * Subroutine:	SCM_GetRstStatus
 * Function:	to get reset status 
 * Input:		None 
 * Output:		reset status	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT32 SCM_GetRstStatus(void)
{
	return ScmRegs.SYSRSTSTATUS.all;
}

/****************************************************************************
 * Subroutine:	SCM_ClrRstStatus
 * Function:	clear Reset Status
 * Input:		UINT32 RstType --  Rst type
 * Output:		None	
 * Description:	 
 * Date:		2014.09.25
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_ClrRstStatus(UINT32 RstType)
{
	ScmRegs.SYSRSTSTATUS.all |= (RstType);
}

/****************************************************************************
 * Subroutine:	SCM_RoscCRE
 * Function:	to enable HF ROSC clock recover 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE  void SCM_HFRoscCRE(void)
{
	ScmRegs.HFROSCCTRL.bit.HFROscCRE = 1;
}

/****************************************************************************
 * Subroutine:	SCM_SetHFRoscTrimMax
 * Function:	to set HF ROSC trimming maximum value 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetHFRoscTrimMax(UINT16 nMax)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.HFROSCCTRL.bit.TrimHLimit = nMax;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetClkout
 * Function:	to set clock out
 * Input:		UINT8 nDiv -- to specify the divider of clock out (0~255) 
 * Output:		None	
 * Description:	 
 * Date:		2014.09.16
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetClkout(UINT8 nDiv)
{
	GpioRegs.GPAUSE1.bit.GPA11 = MUX_CLKOUT;
	ScmRegs.CLKOUTCTRL.all &= ~(UINT32)(0xFF);  
	ScmRegs.CLKOUTCTRL.all |= (nDiv | 0x100);
}

/****************************************************************************
 * Subroutine:	SCM_EnableClkout
 * Function:	to enable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_EnableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all |= 0x100;
}

/****************************************************************************
 * Subroutine:	SCM_DisableClkout
 * Function:	to disable clock out
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableClkout(void)
{
	ScmRegs.CLKOUTCTRL.all &= (UINT32)(~0x100);
}

/****************************************************************************
 * Subroutine:	SCM_EnableUsbClk
 * Function:	to enable USB 48MHz clock (enabled by default)
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_EnableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 1;	
}

/****************************************************************************
 * Subroutine:	SCM_DisableUsbClk
 * Function:	to disable USB 48MHz clock
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.10.14
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableUsbClk(void)
{
	ScmRegs.SYSCLKCTRL.bit.UsbClk48mEn = 0;	
}

/****************************************************************************
 * Subroutine:	SCM_DisableDestruct
 * Function:	Disable self-destruct
 * Input:		UINT32 Destructtype  --
 * Output:		None
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableDestruct(UINT32 Destructtype)
{
	ScmRegs.SYSSDCTRL.all &= ~(Destructtype);
}

/****************************************************************************
 * Subroutine:	SCM_EnableDestruct
 * Function:	Enable self-destruct
 * Input:		UINT32 Destructtype  --
 * Output:		None	
 * Description:	 
 * Date:		2014.07.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_EnableDestruct(UINT32 Destructtype)
{
	ScmRegs.SYSSDCTRL.all |= (Destructtype);
}

/****************************************************************************
 * Subroutine:	SCM_DisableSramExe
 * Function:	to disable SRAM execute function
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.11.18
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableSramExe(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.SRAM_EXT_INHIBIT = 0x45239876;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetTdLowTrim
 * Function:	to set Td low trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetTdLowTrim(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDLTrim = nTrim;
}

/****************************************************************************
 * Subroutine:	SCM_SetTdHighTrim
 * Function:	to set Td high trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetTdHighTrim(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDHTrim = nTrim;
}

/****************************************************************************
 * Subroutine:	SCM_GetTdLowTrim
 * Function:	to get Td low trimming
 * Input:		None
 * Output:		the low trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT8 SCM_GetTdLowTrim(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDLTrim;
}

/****************************************************************************
 * Subroutine:	SCM_GetTdHighTrim
 * Function:	to get Td high trimming
 * Input:		None
 * Output:		the high trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT8 SCM_GetTdHighTrim(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDHTrim;
}

/****************************************************************************
 * Subroutine:	SYST_GetCnt
 * Function:	to get SysTick current count value
 * Input:		None
 * Output:		UINT32 -- the value of current count	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT32 SYST_GetCnt(void)
{
	return SysTick->VAL;
}

/****************************************************************************
 * Subroutine:	SYST_Stop
 * Function:	to stop SysTick
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SYST_Stop(void)
{
	SysTick->CTRL &= ~(UINT32)(1<<SYSTICK_ENABLE);
}

/****************************************************************************
 * Subroutine:	SYST_Continue
 * Function:	to Continue SysTick count
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2012.10.22
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SYST_Continue(void)
{
	SysTick->CTRL |= (1<<SYSTICK_ENABLE);
}

/****************************************************************************
 * Subroutine:	SCM_SetTdLow
 * Function:	to set Td low trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetTdLow(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDLTrim = nTrim;
}

/****************************************************************************
 * Subroutine:	SCM_SetTdHigh
 * Function:	to set Td high trimming
 * Input:		UINT8 nTrim -- to specify the trimming value (0~7)
 * Output:		None	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_SetTdHigh(UINT8 nTrim)
{
	ScmRegs.TPRCTRL.bit.TDHTrim = nTrim;
}

/****************************************************************************
 * Subroutine:	SCM_GetTdLow
 * Function:	to get Td low trimming
 * Input:		None
 * Output:		the low trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT8 SCM_GetTdLow(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDLTrim;
}

/****************************************************************************
 * Subroutine:	SCM_GetTdHigh
 * Function:	to get Td high trimming
 * Input:		None
 * Output:		the high trimming value	
 * Description:	 
 * Date:		2013.07.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE UINT8 SCM_GetTdHigh(void)
{
	return 	ScmRegs.TPRCTRL.bit.TDHTrim;
}

/****************************************************************************
 * Subroutine:	SCM_DisableSramKeyAcces
 * Function:	Prohibit access to the secret key area SRAM
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2015.03.09
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_DisableSramKeyAcces(void)
{
	ScmRegs.REGWP1 = 0x32107654;
	ScmRegs.KEY_ACCESS_INHIBIT = 0x54329876;
	ScmRegs.REGWP1 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_VectReMapTo
 * Function:	to re-map vector to specified memory
 * Input:		VT_MEM mem -- to specify the memory type  
 * Output:		None	
 * Description:	 
 * Date:		2012.08.10
 * ModifyRecord:
 * *************************************************************************/
__STATIC_INLINE void SCM_VectReMapTo(VT_MEM mem)
{
	SCB->VTOR = mem;
}

///****************************************************************************
// * Subroutine:	SramAddrEncryption
// * Function:	Sram address encrypt
// * Input:		nAddrKey  --  Sram address
// * Output:		None	
// * Description:	 
// * Date:		2015.09.12
// * ModifyRecord:
// * *************************************************************************/
//__STATIC_INLINE void SramAddrEncryption(UINT32 nAddrKey)
//{
//	ScmRegs.REGWP1 = 0x32107654;
//	ScmRegs.SRAM_ADDRKEY = nAddrKey;
//	ScmRegs.REGWP1 = 0;
//}
//
///****************************************************************************
// * Subroutine:	SramDataEncryption
// * Function:	Sram data encrypt
// * Input:		nDataKey  -- Sram address
// * Output:		None	
// * Description:	 
// * Date:		2015.09.12
// * ModifyRecord:
// * *************************************************************************/
//__STATIC_INLINE void SramDataEncryption(UINT32 nDataKey)
//{
//	ScmRegs.REGWP1 = 0x32107654;
//	ScmRegs.SRAM_DATAKEY = nDataKey;
//	ScmRegs.REGWP1 = 0;
//}

#endif /*SCM_H_*/
