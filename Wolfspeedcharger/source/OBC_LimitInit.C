/*****************************************************************************
 *
 *
 *  OBC_limitInit.C
 *  definitions for limit initializations.
 *
 *
 *****************************************************************************/


#include <Shared.h>
//-------------------define default value---------------------------
#define VDC_SAM_SYSA_DF_VALUE	((INT32)4096)
#define VDC_SAM_SYSB_DF_VALUE	((INT32)0)

#define VDC_CON_SYSA_DF_VALUE	((INT32)4096)
#define VDC_CON_SYSB_DF_VALUE	((INT32)0)



#define IDC_SAM_SYSA_DF_VALUE	((INT32)1024)
#define IDC_SAM_SYSB_DF_VALUE	((INT32)0)

#define IDC_CON_SYSA_DF_VALUE	((INT32)1024)
#define IDC_CON_SYSB_DF_VALUE	((INT32)0)


#define VAC_SAM_SYSA_DF_VALUE	((INT32)1024)
#define VAC_SAM_SYSB_DF_VALUE	((INT32)0)



#define VPFC_SAM_SYS_DF_VALUE	((INT32)0x400)
#define VPFC_CON_SYS_DF_VALUE	((INT32)0x400)

#define POW_CON_SYSA_DF_VALUE	((INT32)1024)
#define POW_CON_SYSB_DF_VALUE	((INT32)0)

#define LQ12_VDC_SAM_SYSA_LIMDN	  		((INT32)3277)
#define LQ12_VDC_SAM_SYSA_LIMUP  		((INT32)8192)
#define LQ12_VDC_SAM_SYSB_LIMDN	  		(((INT32)(-300)) << 12)
#define LQ12_VDC_SAM_SYSB_LIMUP  		((INT32)300 << 12)

#define LQ12_VDC_CON_SYSA_LIMDN	  		((INT32)2457)
#define LQ12_VDC_CON_SYSA_LIMUP  		((INT32)8192)
#define LQ12_VDC_CON_SYSB_LIMDN	  		(((INT32)(-300)) << 12)
#define LQ12_VDC_CON_SYSB_LIMUP  		((INT32)300 << 12)

#define LQ10_IDC_SAM_SYSA_LIMDN	  		((INT32)512)
#define LQ10_IDC_SAM_SYSA_LIMUP  		((INT32)1536)
#define LQ10_IDC_SAM_SYSB_LIMDN	  		(((INT32)(-20)) << 10)
#define LQ10_IDC_SAM_SYSB_LIMUP  		((INT32)20 << 10)

#define LQ10_IDC_CON_SYSA_LIMDN	  		((INT32)512)
#define LQ10_IDC_CON_SYSA_LIMUP  		((INT32)1536)
#define LQ10_IDC_CON_SYSB_LIMDN	  		(((INT32)(-800)) << 10)
#define LQ10_IDC_CON_SYSB_LIMUP  		((INT32)800 << 10)

#define LQ10_VAC_SAM_SYSA_LIMDN	  		((INT32)512)
#define LQ10_VAC_SAM_SYSA_LIMUP  		((INT32)1536)
#define LQ10_VAC_SAM_SYSB_LIMDN	  		(((INT32)(-120)) << 10)
#define LQ10_VAC_SAM_SYSB_LIMUP  		((INT32)120 << 10)


#define LQ10_VPFC_SAM_SYS_LIMDN	  		((INT32)819)
#define LQ10_VPFC_SAM_SYS_LIMUP  		((INT32)1250)

#define LQ10_VPFC_CON_SYS_LIMDN	  		((INT32)819)
#define LQ10_VPFC_CON_SYS_LIMUP  		((INT32)1250)

#define LQ10_POW_CON_SYSA_LIMDN			((INT32)512)
#define LQ10_POW_CON_SYSA_LIMUP			((INT32)1536)
#define LQ10_POW_CON_SYSB_LIMDN			(((INT32)(-500)) << 10)
#define LQ10_POW_CON_SYSB_LIMUP			((INT32)500 << 10)

#define LQ10_POW_LIM_DN		       		((INT32)10)
#define LQ10_POW_LIM_UP		       		((INT32)1250)
#define LQ10_POW_LIM_DFT_VALUE			((INT32)1024)

#define LQ10_IDC_LIM_DN		       		((INT32)10)
#define LQ10_IDC_LIM_UP		       		((INT32)1250)
#define LQ10_IDC_LIM_DFT_VALUE			((INT32)1250)

ubitfloat g_fRdTemp;
UINT16 g_u16I2CErrorType;
UINT16 g_u16RdEepromOk;
UINT16 g_u16WriteNumber;

extern UINT16 u16WrFloatDataThree(UINT16 u16Address, ubitfloat fTemp);

extern ubitfloat fRdFloatDataThreePointer(UINT16 u16Address);
extern UINT16 u16I2caRdDataPointer(UINT16 u16Address);


void  RdE2prom(void);


void vLimitInit(void)
{
	ubitfloat	ftemp1;
	ubitfloat	ubitfTmp;
	UINT16		u16CodeTmp;

	//enable eeprom write
	mWrEnable();

	g_u16WriteNumber = 0;
	g_u16MdlStatus.all = 0;
    g_u16MdlStatusH.all = 0;

	ftemp1.lData = 0;

	if (u16I2caRdDataPointer(0) == I2C_SUCCESS)
	{
		ftemp1.lData = g_fRdTemp.lData;
	}

	// avoid watchdog reset, feed dog
	WatchDogKickPointer();

    	g_lq10DCTempUp.lData = LQ10_DCTEMP_UP_DFT;
		g_lq10PFCTempUp.lData = LQ10_PFCTEMP_UP_DFT;
		g_lq10MdlVoltUp.lData = LQ10_VDC_UP_DFT;
		g_lq10DCTempUp.lData = LQ10_DCTEMP_UP_DFT;

		g_u16MdlCtrl.all = 0;
		g_lq10SetVolt.lData = LQ10_VDC_DEFAULT;
		g_lq10SetPower.lData = LQ10_POWERLIMITRATE;
		g_lq10SetLimit.lData = LQ10_CURRLIMITMAX;




	if (ftemp1.lData == EPROM_READY_LONG)
	{
		RdE2prom();
	}
	else
	{
		ubitfTmp.intdata[0].id = EPROM_READY_UINT;
		ubitfTmp.intdata[1].id = EPROM_READY_UINT;
		u16CodeTmp = u16WrFloatDataThree(0,ubitfTmp);
		if (u16CodeTmp == 0xf2)
		{
			u16CodeTmp = u16WrFloatDataThree(0,ubitfTmp);
		}

		WatchDogKickPointer();
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;


		g_lq10AcVrmsSampSysa.lData = VAC_SAM_SYSA_DF_VALUE;
		g_lq10AcVrmsSampSysb.lData = VAC_SAM_SYSB_DF_VALUE;
		g_lq10VpfcSampSys.lData = VPFC_SAM_SYS_DF_VALUE;
		g_lq10VpfcConSys.lData = VPFC_CON_SYS_DF_VALUE;
		g_lq12VoltSampSysa.lData = VDC_SAM_SYSA_DF_VALUE;
		g_lq12VoltSampSysb.lData = VDC_SAM_SYSB_DF_VALUE;
		g_lq12VoltConSysa.lData = VDC_CON_SYSA_DF_VALUE;
		g_lq12VoltConSysb.lData = VDC_CON_SYSB_DF_VALUE;
		g_lq10CurrSampSysa.lData = IDC_SAM_SYSA_DF_VALUE;
		g_lq10CurrSampSysb.lData = IDC_SAM_SYSB_DF_VALUE;
		g_lq10CurrConSysa.lData = IDC_CON_SYSA_DF_VALUE;
		g_lq10CurrConSysb.lData = IDC_CON_SYSB_DF_VALUE;
		g_lq10PowerConSysa.lData = POW_CON_SYSA_DF_VALUE;
		g_lq10PowerConSysb.lData = POW_CON_SYSB_DF_VALUE;
	}

	//WP=1,disable write
	mWrDisable();
}


void  RdE2prom(void)
{
	
	ubitfloat	lq10temp1,lq10temp2;
	UINT16		u16CodeTmp;
//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(4);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(8);

	if (((lq10temp1.lData >= LQ12_VDC_SAM_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ12_VDC_SAM_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ12_VDC_SAM_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ12_VDC_SAM_SYSB_LIMUP))
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq12VoltSampSysa.lData = lq10temp1.lData; 
		g_lq12VoltSampSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq12VoltSampSysa.lData = VDC_SAM_SYSA_DF_VALUE; 
		g_lq12VoltSampSysb.lData = VDC_SAM_SYSB_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
	
	//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(12);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(16);

	if (((lq10temp1.lData >= LQ12_VDC_CON_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ12_VDC_CON_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ12_VDC_CON_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ12_VDC_CON_SYSB_LIMUP))
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq12VoltConSysa.lData = lq10temp1.lData; 
		g_lq12VoltConSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq12VoltConSysa.lData = VDC_CON_SYSA_DF_VALUE; 
		g_lq12VoltConSysb.lData = VDC_CON_SYSB_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}

	//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(20);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(24);

	if (((lq10temp1.lData >= LQ10_IDC_SAM_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ10_IDC_SAM_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ10_IDC_SAM_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ10_IDC_SAM_SYSB_LIMUP))
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10CurrSampSysa.lData = lq10temp1.lData; 
		g_lq10CurrSampSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq10CurrSampSysa.lData = IDC_SAM_SYSA_DF_VALUE; 
		g_lq10CurrSampSysb.lData = IDC_SAM_SYSB_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
	
	//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(28);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(32);

	if (((lq10temp1.lData >= LQ10_IDC_CON_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ10_IDC_CON_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ10_IDC_CON_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ10_IDC_CON_SYSB_LIMUP))
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10CurrConSysa.lData = lq10temp1.lData; 
		g_lq10CurrConSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq10CurrConSysa.lData = IDC_CON_SYSA_DF_VALUE; 
		g_lq10CurrConSysb.lData = IDC_CON_SYSB_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
	
	//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(40);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(44);

	if ((lq10temp1.lData >= LQ10_VAC_SAM_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ10_VAC_SAM_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ10_VAC_SAM_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ10_VAC_SAM_SYSB_LIMUP)
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10AcVrmsSampSysa.lData = lq10temp1.lData; 
		g_lq10AcVrmsSampSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq10AcVrmsSampSysa.lData = VAC_SAM_SYSA_DF_VALUE; 
		g_lq10AcVrmsSampSysb.lData = VAC_SAM_SYSB_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
	
	//--------------------------pfc sample---------------------------------
	lq10temp1 = fRdFloatDataThreePointer(56);

	if ((lq10temp1.lData >= LQ10_VPFC_SAM_SYS_LIMDN) 
		&& (lq10temp1.lData <= LQ10_VPFC_SAM_SYS_LIMUP)
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10VpfcSampSys.lData = lq10temp1.lData; 
	}
	else
	{
		g_lq10VpfcSampSys.lData = VPFC_SAM_SYS_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}

	lq10temp1=fRdFloatDataThreePointer(60);

	if ((lq10temp1.lData >= LQ10_VPFC_CON_SYS_LIMDN) 
		&& (lq10temp1.lData <= LQ10_VPFC_CON_SYS_LIMUP)
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10VpfcConSys.lData = lq10temp1.lData; 
	}
	else
	{
		g_lq10VpfcConSys.lData = VPFC_CON_SYS_DF_VALUE; 
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
	
	//---------------------------------------------------------------------
	lq10temp1 = fRdFloatDataThreePointer(64);
	u16CodeTmp = g_u16RdEepromOk;
	lq10temp2 = fRdFloatDataThreePointer(68);

	if ((lq10temp1.lData >= LQ10_POW_CON_SYSA_LIMDN) 
		&& (lq10temp1.lData <= LQ10_POW_CON_SYSA_LIMUP)
		&& (lq10temp2.lData >= LQ10_POW_CON_SYSB_LIMDN)
		&& (lq10temp2.lData <= LQ10_POW_CON_SYSB_LIMUP)
		&& (u16CodeTmp == READ_E2PROM_SUCCESS) 
		&& (g_u16RdEepromOk == READ_E2PROM_SUCCESS))
	{
		g_lq10PowerConSysa.lData = lq10temp1.lData; 
		g_lq10PowerConSysb.lData = lq10temp2.lData; 
	}
	else
	{
		g_lq10PowerConSysa.lData = POW_CON_SYSA_DF_VALUE;
		g_lq10PowerConSysb.lData = POW_CON_SYSB_DF_VALUE;
		g_u16MdlStatus.bit.CALIBRFAIL = CALIBR_FAIL;
	}
}
//===========================================================================
// No more.
//===========================================================================

