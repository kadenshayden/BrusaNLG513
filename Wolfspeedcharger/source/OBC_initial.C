/*****************************************************************************
*
*
* OBC_initial
* To initialize set values on startup for OBC
*
*
*****************************************************************************/

#include <Shared.h>


void vDataInit(void)
{
	UINT16	i;
	
	WatchDogKickPointer();

	for(i = MAIN_RAM_START; i < MAIN_RAM_END; i++) //RAMLS2£ºCAN FIFO and its pointers
	{
		*(UINT16 *)i = 0;
	}

	for(i = MAIN_ISR_RAM_START; i < MAIN_ISR_RAM_END; i++) //Isr±äÁ¿
	{
		*(UINT16 *)i = 0;
	}

	for(i=0x1480; i<0x14F0; i++)
	{
		*(UINT16 *)i = 0;
	}

	for(i=0x1500; i<0x1580; i++)
	{
		*(UINT16 *)i = 0;
	}


	WatchDogKickPointer();
	
	g_lq10SetVolt.lData = VDC_330V;
	g_lq10SetVoltTrue.lData = 0;
	g_lq10MdlLimit.lData = LQ10_CURRLIMITMAX;
	g_lq10SetLimit.lData = LQ10_CURRLIMITMAX;
	g_lq10SetLimitTrue.lData = LQ10_CURRLIMITMAX;
	g_lq10SetPower.lData = LQ10_POWERLIMITRATE;
	g_lq10SetPowerTrue.lData = LQ10_POWERLIMITRATE;

	g_i32PowerRate = POW_RATE;
	g_i32CurrRate = CURR_RATE;
	g_i32CurrDisMax = MAX_CURR;

	g_u16AddIdentifyFlag = 0;
	g_u16AddIdentifyNum = 3;


	g_lq10SetPfcVolt.lData = VPFC_395V;
	CputoClaVar._uiLowPfcVolt = 0; 


	g_u16ActionReady = POWON_STATE;     //  Initial power on "action" - originally set to "POWON_STATE". This automatically starts charging when AC
	                                    // is applied - NOT SAFE

	g_u16VersionNoHw = 0x101;
	g_u16VersionNoSw = 105;
	


    g_u16RunFlag.all = 0;
}


void vIsr_init(void)
{
	IsrVars._lVpfcFilterK1.lData = -136;
    IsrVars._i16VpfcFilterK2 = 68;

 /*******************************  DCDC  ***********************/
    IsrVars._i16CHG_VdcLoopK3 =  1469;
    IsrVars._i16CHG_VdcLoopK4 =  -1465;
    IsrVars._i16INV_VdcLoopK3 = 1469;
    IsrVars._i16INV_VdcLoopK4 = -1465;
    IsrVars._i16VdcLoopK3 = IsrVars._i16CHG_VdcLoopK3;
    IsrVars._i16VdcLoopK4 = IsrVars._i16CHG_VdcLoopK4;

//----------------------------------------------------------	
	IsrVars._i16CurrLimFloor = POWER_LIM_FLOOR ;
 	IsrVars._i16PowerLimMax = POWER_LIM_UPPER ;
    IsrVars._i16PowerMaxExtra = IsrVars._i16PowerLimMax + 468;
//---------------------------------------------------------	

	IsrVars._lDcdcPWMTs.iData.iHD = DC_TBPRD_Fmax * 2;
	IsrVars._lDcdcPWMTsShadow.iData.iHD = DC_TBPRD_Fmax * 2;
	IsrVars._lDcdcDuty.iData.iHD = 0;
	IsrVars._lDcdcDutyShadow.iData.iHD = 0; 
	
	IsrVars._i16DcdcPWMTsMax = DC_Ts_Max_152K;//DC_Ts_Max_154K;
	IsrVars._i16DcdcPWMTsMin = DC_Ts_Min_300K;
//-------------------------------------------------

   IsrVars.l32SR_DC_FED_AHEAD = (int32)SR_DC_FED_AHEAD<<16;
   IsrVars.l32SR_DC_FED_DELAY = -(int32)SR_DC_FED_AHEAD<<16;

   IsrVars.u16SROnDelay = DC_DB_TIME_SR;
}


void vCputoClaVar_init(void)
{
	CputoClaVar._f32VpfcFilterK2 = 0.00103760f;
	CputoClaVar._f32VpfcFilterK1 = 0.9979248f;

	CputoClaVar._f32VpfcLoopKp = (float32)3.43833705;
    CputoClaVar._f32VpfcLoopKi = (float32)0.00094168543;

	
	CputoClaVar.f32VacRmsSqrInv = 10.0f/220.0/220.0f;

	CputoClaVar._iIpfcOffset = 2000;

	CputoClaVar._f32PfcDutyPermit = (float32)PFC_DUTY_MIN;


	CputoClaVar._f32PiDynTempFilter1 = (float32)0.9657;
    CputoClaVar._f32PiDynTempFilter2 = (float32)0.01715;
    CputoClaVar._f32DynIdcFilter1 = (float32)0.6538;
    CputoClaVar._f32DynIdcFilter2 = (float32)0.1731;
	

    CputoClaVar.u16WorkMode = UNVALID;
}




void vCLATaskinit(void)
{    
	CLA_configClaMemory();
	CLA_initCpu1Cla1();
	Cla1ForceTask8();
}

//===========================================================================
// No more.
//===========================================================================

