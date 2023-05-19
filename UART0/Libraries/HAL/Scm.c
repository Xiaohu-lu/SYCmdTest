/****************************************************************************
* File name    :	Scm.c
* Function     :	the driver of System Control Module
* Author       : 	coins
* Date         :	2012/07/23
* Version      :    v1.0
* Description  :
* ModifyRecord :
****************************************************************************/
/***************************************************************************
* Include Header Files
***************************************************************************/
#include "SY09.h"

extern volatile uint32_t		SystemFrequency; 	// defined in system_SY03.c
extern volatile unsigned char 	SerInit;	 		// defined in Serial.c 

/**************************************************************************
* Local Macro Definition
***************************************************************************/
//#define	SCM_ROM_FUNC_USED
#define		CALC_MS(freq)	((freq)/4000)

/**************************************************************************
* Local Type Definition
***************************************************************************/


/**************************************************************************
* Local static Variable Declaration
***************************************************************************/
static UINT16 gCntMs = 424;
static UINT8 gCntUs = 1;


/**************************************************************************
* Global Variable Declaration
***************************************************************************/


/**************************************************************************
* Local Functon Declaration
***************************************************************************/
void _delay_ms(register UINT32 ms);

/**************************************************************************
* Functon
***************************************************************************/

/****************************************************************************
 * Subroutine:	CM_RegWrEn
 * Function:	to enable register MODPDCTRL and PWRTOPCTRL writeable 
 * Input:		None
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_RegWrEn(void)
{
	ScmRegs.REGWP0 = 0x55;
	ScmRegs.REGWP0 = 0xAA;
	ScmRegs.REGWP0 = 0x67;
	if(ScmRegs.REGWP0&0x100) return RT_OK;
	else return RT_FAIL;
}

/****************************************************************************
 * Subroutine:	SCM_AnModPowerEnable
 * Function:	to enable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerEnable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all |= (nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_AnModPowerDisable
 * Function:	to disable analog mode power 
 * Input:		UINT32 nAnMods --	to specify the analog modules. 
 * Output:		None	
 * Description:	 
 * Date:		2013.12.18
 * ModifyRecord:
 * *************************************************************************/
void SCM_AnModPowerDisable(UINT32 nAnMods)
{
	SCM_RegWrEn();
	ScmRegs.AMODENCTRL.all &= ~(nAnMods);
	ScmRegs.REGWP0 = 0;
}

/****************************************************************************
 * Subroutine:	SCM_SetSysClk
 * Function:	to set system clock 
 * Input:		UINT8 Clk --	to specify the system clock. 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_SetSysClk(SYS_CLK Clk)
{
	UINT8 nOscGrade = (Clk>>4)&0xF;
	UINT8 nFclkSel =  Clk&0xF;
	UINT8 nHFROscCRE;
	if(ScmRegs.AMODENCTRL.bit.Hfrosc == 0)
	{
		SCM_RegWrEn();
		ScmRegs.AMODENCTRL.bit.Hfrosc = 1;
		ScmRegs.REGWP0 = 0x00;
	}

	EFC_ConfigTiming(TC0_144M, TC1_144M, TC2_144M, TC3_144M);
	// check HFRosc Grade
	if(ScmRegs.HFROSCCTRL.bit.HFROscGrade != nOscGrade)	
	{
		ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & 0xFFFFFFF2) | 0x1 | (1<<2);	// run in HFROSC/4, because HFROSC frequency may be too large before trimming
		nHFROscCRE = ScmRegs.HFROSCCTRL.bit.HFROscCRE;
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = 0;
		ScmRegs.HFROSCCTRL.bit.HFROscGrade = nOscGrade;//change HFRosc Grade must disable HFROscCRE.
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = nHFROscCRE;	
		SCM_LoadHFRoscTrim();	// reload trimming value		
	}

	ScmRegs.SYSCLKCTRL.all = (ScmRegs.SYSCLKCTRL.all & 0xFFFFFFF2) | 0x1 | (nFclkSel<<2);
	__NOP();__NOP();__NOP();__NOP();__NOP();
	
	// USB clock setting
	if(ScmRegs.HFROSCCTRL.bit.HFROscGrade == 3)
	{
		ScmRegs.SYSCLKCTRL.all |= 1<<25;		
	}
	else
	{
		ScmRegs.SYSCLKCTRL.all &= ~(1<<25);	
	}
	SerInit = 0;
	switch(Clk)
	{
		case SYS_CLK_12M: 	
			EFC_ConfigTiming(TC0_12M, TC1_12M, TC2_12M, TC3_12M);
			gCntMs = 3010;		//测试参数10,实际测试10
			gCntUs = 3;			//测试参数10,实际测试12
			SystemFrequency = F_HFROSC_2/8;
			break;
		case SYS_CLK_24M: 
			EFC_ConfigTiming(TC0_24M, TC1_24M, TC2_24M, TC3_24M);
			gCntMs = 6020;		//测试参数10,实际测试10
			gCntUs = 6;			//测试参数10,实际测试11
			SystemFrequency = F_HFROSC_2/4;
			break;
		case SYS_CLK_48M: 
			EFC_ConfigTiming(TC0_48M, TC1_48M, TC2_48M, TC3_48M);
			gCntMs = 12040;		//测试参数10,实际测试10
			gCntUs = 12;		//测试参数10,实际测试10
			SystemFrequency = F_HFROSC_2/2;
			break;
		case SYS_CLK_96M: 
			EFC_ConfigTiming(TC0_96M, TC1_96M, TC2_96M, TC3_96M);
			gCntMs = 24020;		//测试参数10,实际测试10
			gCntUs = 24;		//测试参数10,实际测试10
			SystemFrequency = F_HFROSC_2;
			break;
		case SYS_CLK_18M:
			EFC_ConfigTiming(TC0_18M, TC1_18M, TC2_18M, TC3_18M);
			gCntMs = 4540;		//测试参数10,实际测试10
			gCntUs = 4;		//测试参数10,实际测试10	
			SystemFrequency = F_HFROSC_3/8;
			break;
		case SYS_CLK_36M:
			EFC_ConfigTiming(TC0_36M, TC1_36M, TC2_36M, TC3_36M);
			gCntMs = 9080;		//测试参数10,实际测试10
			gCntUs = 9;		//测试参数10,实际测试10
			SystemFrequency = F_HFROSC_3/4;
			break;
		case SYS_CLK_72M:
			EFC_ConfigTiming(TC0_72M, TC1_72M, TC2_72M, TC3_72M);
			gCntMs = 18150;		//测试参数10,实际测试10
			gCntUs = 18;		//测试参数10,实际测试10
			SystemFrequency = F_HFROSC_3/2;
			break;
		case SYS_CLK_144M:
			EFC_ConfigTiming(TC0_144M, TC1_144M, TC2_144M, TC3_144M);
			gCntMs = 36200;		//测试参数10,实际测试10
			gCntUs = 36;		//测试参数10,实际测试10
			SystemFrequency = F_HFROSC_3;
			break;
		case SYS_CLK_6M:
			EFC_ConfigTiming(TC0_6M, TC1_6M, TC2_6M, TC3_6M);
			gCntMs = 1564;		//测试参数10,实际测试10
			gCntUs = 1;		//测试参数10,实际测试24；
			SystemFrequency = F_HFROSC_1/8;
			break;
		case SYS_CLK_1_5M:
			EFC_ConfigTiming(TC0_1_5M, TC1_1_5M, TC2_1_5M, TC3_1_5M);
			gCntMs = 370;		//测试参数10,实际测试10；	
			gCntUs = 1;		//测试参数10,实际测试82；
			SystemFrequency = F_HFROSC_0/8;
			break;
		case SYS_CLK_3M:
			EFC_ConfigTiming(TC0_3M, TC1_3M, TC2_3M, TC3_3M);
			gCntMs = 742;		//测试参数10,实际测试10；
			gCntUs = 1;		//测试参数10,实际测试47；
			SystemFrequency = F_HFROSC_0/4;
			break;
		default:
			return RT_FAIL;
	}
	return RT_OK;
}
/****************************************************************************
 * Subroutine:	SCM_SetSysClkLowFreq
 * Function:	to set system clock to LFROSC 
 * Input:		None 
 * Output:		None	
 * Description:	 
 * Date:		2013.01.05
 * ModifyRecord:
 * *************************************************************************/
void SCM_SetSysClkLowFreq(void)
{
	ScmRegs.SYSCLKCTRL.bit.FclkSrc = 0x0;
	__NOP();__NOP();__NOP();__NOP();__NOP();
}
/****************************************************************************
 * Subroutine:	SCM_GetSysClk
 * Function:	to get system clock 
 * Input:		None 
 * Output:		return the system clock frequency in Hz.	
 * Description:	 
 * Date:		2012.07.03
 * ModifyRecord:
 * *************************************************************************/
UINT32 SCM_GetSysClk(void)
{
	return SystemFrequency;
}

/****************************************************************************
 * Subroutine:	SCM_UsbPhySet
 * Function:	to set usb phy 
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_UsbPhySet(void)
{
	ScmRegs.USBPHYCTRL.bit.PhyMod = 1;
}

/****************************************************************************
 * Subroutine:	SCM_EnableUsbPuRes
 * Function:	to enable USB 1.5K Pull up Resistor
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_EnableUsbPuRes(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbPuRes = 1;
}

/****************************************************************************
 * Subroutine:	SCM_DisableUsbPuRes
 * Function:	to disable USB 1.5K Pull up Resistor
 * Input:		None
 * Output:		None	
 * Description:	 
 * Date:		2013.01.29
 * ModifyRecord:
 * *************************************************************************/
void SCM_DisableUsbPuRes(void)
{
	ScmRegs.USBPHYCTRL.bit.UsbPuRes = 0;
}

/****************************************************************************
 * Subroutine:	SCM_LoadHFRoscTrim
 * Function:	to load default HF ROSC Trimming value from OTP and update to register 
 * Input:		None 
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.10.25
 * ModifyRecord:
 * *************************************************************************/
UINT8 SCM_LoadHFRoscTrim(void)
{
	UINT8 nPowerSelData = *((UINT8 *)POWER_MODE_SEL_ADDR);
	UINT8 nOffset = ScmRegs.HFROSCCTRL.bit.HFROscGrade;
	UINT16 *pDefaultTrim;  	// trimming field address in OTP
	UINT16 DefaultTrim; 
	union SCM_HFROSCCTRL_REG  uHfOscCtrl;
	UINT8 nHFROscCRE;

	if(nPowerSelData != 0xff)//1.8
		pDefaultTrim = (UINT16 *)(0x10080428);
	else//3.3
		pDefaultTrim = (UINT16 *)(0x10080420);
	DefaultTrim = pDefaultTrim[nOffset]&0xFFF;

	if(nPowerSelData == 0xff)//3.3
	{
		if((nOffset == 2)&&(*((UINT16 *)(HF_96M_TRIM_FLAG_ADDR)) == HF_TRIM_FLAG))
			DefaultTrim = (*((UINT16*)(HF_96M_TRIM_ADDR)))&0xFFF;
		if((nOffset == 3)&&(*((UINT16 *)(HF_144M_TRIM_FLAG_ADDR)) == HF_TRIM_FLAG))
			DefaultTrim = (*((UINT16*)(HF_144M_TRIM_ADDR)))&0xFFF;	
	}
	if(DefaultTrim != 0xFFF)
	{
		uHfOscCtrl.all = ScmRegs.HFROSCCTRL.all;
		uHfOscCtrl.bit.HFROscGrade = DefaultTrim>>10;
		uHfOscCtrl.bit.HFROscTrim = DefaultTrim&0x3FF;
		nHFROscCRE = ScmRegs.HFROSCCTRL.bit.HFROscCRE;
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = 0;
		ScmRegs.HFROSCCTRL.all = uHfOscCtrl.all;//change HFRosc Trim must disable HFROscCRE.
		ScmRegs.HFROSCCTRL.bit.HFROscCRE = nHFROscCRE;
		return RT_OK;
	}
	else
	{
		return RT_FAIL;
	}	
}

/****************************************************************************
 * Subroutine:	_delay_ms
 * Function:	to delay ms
 * Input:		UINT32 ms --	to specify the time of ms;
 * Output:		None	
 * Description:	this delay loops about [(4 * gCntMs + 7) * ms + 18] cycles 
 * Date:		2012.07.24
 * ModifyRecord:
 * *************************************************************************/
#pragma arm section code = ".align_code1"
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
#pragma push
#pragma O3
#pragma Ospace
void _delay_ms(register UINT32 ms)
{
	UINT32 i;	
	while(ms--)
	{
		i =  gCntMs;
		while(i--)
		{
			__NOP();
		}
	}
}
#pragma pop

#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
#pragma optimize = speed high
void _delay_ms(register UINT32 ms)
{
	UINT32 i;	
	while(ms--)
	{
		i =  gCntMs;
		while(i--)
		{
			__NOP();
		}
	}
}
#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif
#pragma arm section code

/****************************************************************************
 * Subroutine:	_delay_us
 * Function:	to delay us
 * Input:		UINT32 us --	to specify the time of us;
 * Output:		None	
 * Description:	the delay loops about [(5 * gCntUs + 12) * us + 2]
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
#pragma arm section code = ".align_code2"
#if defined ( __CC_ARM   )	/*------------------RealView Compiler -----------------*/
#pragma push
#pragma O3
#pragma Ospace
void _delay_us(register UINT32 us)
{
	register UINT32 i, j;
	j = gCntUs;	
	while(j--)
	{
		i =  us;
		while(i--)
		{
            __NOP();
		}
	}
}
#pragma pop

#elif defined ( __ICCARM__ )	/*------------------ ICC Compiler -------------------*/
#pragma optimize = speed high
void _delay_us(register UINT32 us)
{
	register UINT32 i, j;
	j = gCntUs;	
	while(j--)
	{
		i =  us;
		while(i--)
		{
            __NOP();
		}
	}
}

#elif defined   (  __GNUC__  )	/*------------------ GNU Compiler ---------------------*/
#elif defined   (  __TASKING__  )	/*------------------ TASKING Compiler ---------------------*/
#endif
#pragma arm section code

/****************************************************************************
 * Subroutine:	SYST_Config
 * Function:	to set system tick  
 * Input:		UINT8 nClkSource --	to specify the clock source for SysTick;
 *									0 -- STCLK; 1 -- SCLK
 *				UINT32 nTicks -- to specify the initialization counter value;
 *				UINT8 nMode -- to specify the work mode for SysTick
 *								0 -- counter mode; 1 -- interrupt mode		
 * Output:		RT_OK or RT_FAIL	
 * Description:	 
 * Date:		2012.07.23
 * ModifyRecord:
 * *************************************************************************/
UINT8 SYST_Config(UINT8 nClkSource, UINT32 nTicks, UINT8 nMode)
{
	if ((nTicks > SYSTICK_MAXCOUNT) || (nClkSource > SYST_SCLK) || (nMode > SYST_INT)) 
		return RT_FAIL;                                               

  	SysTick->LOAD  =  (nTicks & SYSTICK_MAXCOUNT) - 1;   	/* set reload register */
	/* set Priority for Cortex-M0 System Interrupts */
  	NVIC_SetPriority (SysTick_IRQn, (1<<__NVIC_PRIO_BITS) - 1); 
  	SysTick->VAL   =  (0x00);  /* Load the SysTick Counter Value */
	/* Enable SysTick Timer */
  	SysTick->CTRL = (nClkSource << SYSTICK_CLKSOURCE) | (1<<SYSTICK_ENABLE) | (nMode<<SYSTICK_TICKINT);    
  	return RT_OK;                                                    
}

/****************************************************************************
 * Subroutine:	DeepSleepAutoSelClkEn
 * Function:	deep sleep/wakeup clock is auto selected
 * Input:		None
 * Output:		None	
 * Description:	 ?????,FCLK???????OSC32K;???,FCLK?????????????
 * Date:		2015.10.12
 * ModifyRecord:
 * *************************************************************************/
void DeepSleepAutoSelClkEn(void)
{
	ScmRegs.SYSCLKCTRL.bit.AutoClkSwitch = 1;
}
