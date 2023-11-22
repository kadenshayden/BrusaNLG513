/*****************************************************************************
 *
 *
 *  OBC_start.C
 *  Starts tasks associated with powering on the OBC.
 *  Includes critical calculations such as totem pole, max current, max voltage, etc...
 *
 *
 *****************************************************************************/

#include <Shared.h>

static longunion s_lSSVsetMax;
static longunion s_lSSIsetMax;
static longunion s_lq10VoltWalkIn;
static INT16 s_i16SoftStartPwm;
static INT16 s_i16SoftStartSet;

UINT16 g_u16MainRlyOpenFlag;
UINT16 g_u16MainRlyCloseFlag;

extern bool g_BoolChargerIsActive;

void vPfcSwSoftStartInitial(void);
void vCalMaxDcVoltRef(void);
void vCalMaxDcCurrRef(void);
void vResetDcOcpTripFlag(void);
void vInitialDcPwmVar(void);
void vRec2_TotemSoftStartInit(void);
void vRec3_TotemSoftStart(void);
void vRec4_CLLCSoftStartInit(void);
void vRec5_CLLCSoftStart(void);
void vInv2_CLLCSoftStartInit(void);
void vInv3_CLLCSoftStart(void);
void vInv4_TotemSoftStartInit(void);
void vInv5_TotemSoftStart(void);
void vModeSelect(void);

// State machine
void   vStartTask(void)
{
	if (g_u16ActionReady == POWON_STATE)    // If charger is in POWON state, turn off contactors and make sure everything is "off"
  	{
		CputoClaVar.u16WorkMode = UNVALID;  // POWON_STATE == UNVALID
		                                    // “Mode select” should be the same as the actual work mode. Otherwise the command will be ignored by OBC.
		mMainRelayOpen();		            // Apparently this disconnect something KG: This probably refers to the disconnecting (opening) the main contactor(relay).
		g_u16MainRlyCloseFlag = 0;          // Reset flag indicating the state of the main relay
		mOffDcdcPwm();                      // Disable DCDC pwm signal (EPWM1-EPWM8 regs)
		g_u16MdlCtrl.bit.SREnable = 0;
		mOffPfcPwm();                       // Turns off PWM PFC(for power factor correction?) (EPWM9A/EPWM10A,GPIO16/GPIO18)

		CputoClaVar.u16PfcOpened = 0;       // Indicate to CLA that PFC is not open
		                                    // TI's Control Law Accelerator (CLA) is designed to execute real-time control algorithms in parallel with the CPU

		g_u16MdlStatus.bit.OFFSTAT = 1;     // Set off status bit to true

		g_u16MdlStatus.bit.SrOn = 0;        // Disable SR (SR == Synchronous Rectifier?)

		//to next step
		g_u16ActionReady = PFC_HWSOFT_START;

		IsrVars._u16160usTimer = 0;	        // Resets 160us ticked timer value
	}
	else if(g_u16ActionReady == PFC_HWSOFT_START)
	{
		if(IsrVars._u16160usTimer >= TIME2S)    // TIME2S == 22502 corresponds to 3.6 sec
		{   // 2 seconds elapsed since moving to this state and state was not changed externally
			IsrVars._u16160usTimer = 0;
			vModeSelect();                  // We're ready to start the charging process
		} 
	}
	//---------------------
	else if (g_u16ActionReady == PFC_SWSOFT_START_INIT)     // PFC Software starting initalization
  	{
		if(g_u16MdlCtrl.bit.Rec_Inv)
		{
			mMainRelayClose();              // Close main contactor(relay), enabling AC through the circuit
			g_u16MainRlyCloseFlag = 3;

			vInv2_CLLCSoftStartInit();      // initialize charging PWM signals
		}
		else
		{
			vRec2_TotemSoftStartInit();     // initialize totem circuit
		}
	}

	else if (g_u16ActionReady == PFC_SWSOFT_START)          // PFC software is now starting
  	{
		if(g_u16MdlCtrl.bit.Rec_Inv)
		{
			vInv3_CLLCSoftStart();          // more init
		}
		else
		{
			vRec3_TotemSoftStart();         // more init
		}
	}
	else if (g_u16ActionReady == DC_SOFT_START_INIT)        // PFC hardware initialization
	{
		if(g_u16MdlCtrl.bit.Rec_Inv)
		{
			vInv4_TotemSoftStartInit();
		}
		else
		{
			vRec4_CLLCSoftStartInit();
		}
	}
	else if (g_u16ActionReady == DC_SOFT_START)             // PFC hardware start
	{
		if(g_u16MdlCtrl.bit.Rec_Inv)
		{
				g_u16ActionReady = NORMAL_RUN_STATE;
		}
		else
		{
			vRec5_CLLCSoftStart();
		}
	}
	else if (g_u16ActionReady >= NORMAL_RUN_STATE)  // Charger is charging, check TZ flags (faults) on each loop in main()
	{
		if(EPwm1Regs.TZFLG.bit.OST)
		{   // Are these TZ flags needed? why are they commented out?
 /*           mOffDcdcPwm();
			mOffPfcPwm();		
					   
			CputoClaVar.u16PfcOpened = 0;
			g_u16MdlStatus.bit.OFFSTAT = 1;
			IsrVars._u16DcdcOCP ++ ; */
		}

		 if(EPwm9Regs.TZFLG.bit.OST)        // if a TZ flag has been set "true", turn off DCDC and PFC PWM signals
		 {
	            mOffDcdcPwm();
				mOffPfcPwm();

				CputoClaVar.u16PfcOpened = 0;               // Reset flags to zero (off)
				g_u16MdlStatus.bit.OFFSTAT = 1;
				IsrVars.u16HwPFCOCP ++ ;
		 }
	} 

}

void vPfcSwSoftStartInitial(void)
{
	Cla1ForceTask1();
	s_i16SoftStartPwm = ClatoCpuVar._u16PfcDutyMin;

	CputoClaVar._f32PfcDutyPermit = (float32)s_i16SoftStartPwm;
	
	CputoClaVar._iVpfcSet = __f32toi16r(ClatoCpuVar._f32VpfcUse)+40;

	s_i16SoftStartSet = CputoClaVar._iVpfcSet;
	

	CputoClaVar.u32Theta = 0;
	//reset pfc pwm
	EPwm9Regs.CMPA.bit.CMPA = EPwm9Regs.TBPRD;
	EPwm10Regs.CMPA.bit.CMPA = 0u;
}

void vCalMaxDcVoltRef(void)
{
	s_lSSVsetMax.lData 
		= (_IQ10mpy(g_lq10SetVolt.lData,g_lq12VoltConSysa.lData)
		+ g_lq12VoltConSysb.lData) >> 2;
	s_lSSVsetMax.lData =
		__IQsat(_IQ10mpy(s_lSSVsetMax.lData,LQ10_DEF_VDCFACTOR),VDC_AD_MAX<<10,0);// >> 10
}

void vCalMaxDcCurrRef(void)
{
	s_lSSIsetMax.lData = _IQ10mpy(g_lq10SetLimit.lData,g_i32CurrRate);		
	s_lSSIsetMax.lData = _IQ10mpy(s_lSSIsetMax.lData,g_lq10CurrConSysa.lData) 
		+ g_lq10CurrConSysb.lData;
	s_lSSIsetMax.lData 
		= __IQsat(_IQ10mpy(s_lSSIsetMax.lData,LQ10_DEF_IDCSYSFACTOR), IDC_AD_MAX<<10, IDC_AD_MIN<<10);
}

void vResetDcOcpTripFlag(void)
{
//	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;

	EALLOW;  
	EPwm1Regs.TZCLR.bit.INT = 1;
	EPwm1Regs.TZCLR.bit.OST = 1;
	EPwm2Regs.TZCLR.bit.OST = 1;
	EPwm3Regs.TZCLR.bit.OST = 1;
	EPwm4Regs.TZCLR.bit.OST = 1;
	EPwm5Regs.TZCLR.bit.OST = 1;
	EPwm6Regs.TZCLR.bit.OST = 1;
	EPwm7Regs.TZCLR.bit.OST = 1;
	EPwm8Regs.TZCLR.bit.OST = 1;
	EDIS;
}

void vInitialDcPwmVar(void)
{

	IsrVars._i16IdcPioutPermit = DC_DUTY_MIN;
	IsrVars._lDcdcPWMTs.iData.iHD = IsrVars._lDcdcPWMTsShadow.iData.iHD 
	= DC_TBPRD_Fmax * 2;
	IsrVars._lDcdcDuty.iData.iHD = IsrVars._lDcdcDutyShadow.iData.iHD = 0;
	EPwm9Regs.ETSEL.bit.INTEN = 1;
	CpuTimer2Regs.TCR.bit.TIE = 1;
}

void vMainRlyOnOffCtrl(void)
{
	if(g_u16MainRlyCloseFlag == MAINRLY_ZVS_CTL_OPEN)      
	{
		g_u16MainRlyCloseFlag = MAINRLY_ZVS_CTL_START;
	    IsrVars._u16MainRly_160usTimer = 0;	        
	}
    else if((g_u16MainRlyCloseFlag == MAINRLY_ZVS_CTL_START)
	    && ((abs(ePLL.i16Vac_ADC) <= VAC_AD_20V)
		|| (IsrVars._u16MainRly_160usTimer >= DefWaveProidMax)))
	{			
		g_u16MainRlyCloseFlag = MAINRLY_ZVS_CTL_CHECK;	
		IsrVars._u16MainRly_160usTimer = 0;		
	}
	else if((g_u16MainRlyCloseFlag == MAINRLY_ZVS_CTL_CHECK) 			
		&& (IsrVars._u16MainRly_160usTimer >= DefRlyDelayAct))     		
	{				
		if((g_u16MdlStatus.bit.ACUV) || (g_u16MdlStatus.bit.ACOV)
			|| (g_u16MdlCtrl.bit.SHORTRELAY))       // If AC under voltage OR AC over voltage OR short relay -> open relay, disconnecting the AC input
	   	{                                           //ACUV and ACOV are referencing the target voltage as set by Cree Monitor / the user
			mMainRelayOpen();
	   	}
	   	else
	   	{	 
			mMainRelayClose();                      // If none of those are true, charger is working correct. close and mark flag as closed
			g_u16MainRlyCloseFlag = MAINRLY_ZVS_CTL_CLOSE;
		}
		
		IsrVars._u16MainRly_160usTimer = 0;
	}
}

void vRec3_TotemSoftStart(void)
{
	ubitfloat lTmp;

	if(IsrVars._u16160usTimer >= TIME1MS)
 	{
		IsrVars._u16160usTimer = 0;

		s_i16SoftStartPwm -= SSTART_PFCPWM_STEP;
		if(s_i16SoftStartPwm <= PFC_DUTY_MAX)
		{
			s_i16SoftStartPwm = PFC_DUTY_MAX;
		}

		CputoClaVar._f32PfcDutyPermit = (float32)s_i16SoftStartPwm ;

        g_lq10SetPfcVolt.lData = VPFC_395V;

		lTmp.lData = _IQ10mpy(g_lq10VpfcConSys.lData,g_lq10SetPfcVolt.lData);
		lTmp.lData = (_IQ10mpy(LQ10_DEF_VPFCFACTOR,lTmp.lData))>>10;


		s_i16SoftStartSet += SSTART_PFCVOL_STEP;
		if(s_i16SoftStartSet >= lTmp.iData.iLD)
		{
			//Pfc soft start end
			s_i16SoftStartSet = lTmp.iData.iLD;


			g_lq10SetPfcVoltTrue.lData = g_lq10SetPfcVolt.lData;

			CputoClaVar._f32PfcDutyPermit = (float32)PFC_DUTY_MAX;
		  	CputoClaVar._u16PfcLoopCount = 1 ;
			CputoClaVar.u16PfcOpened = 1;



			//to next step
			g_u16ActionReady = DC_SOFT_START_INIT;
			IsrVars._u16160usTimer = 0;
		}

		CputoClaVar._iVpfcSet = s_i16SoftStartSet;

	}
}

void vInv5_TotemSoftStart(void)
{

}

void vRec5_CLLCSoftStart(void)
{
		vCalMaxDcVoltRef();
		if(IsrVars._u16160usTimer >= TIME711US)
		{
			IsrVars._u16160usTimer = 0;

			s_i16SoftStartPwm += SSTART_DCDCPWM_STEP2;
			if(s_i16SoftStartPwm >= IsrVars._i16DcdcPWMTsMax + 60)
			{
				s_i16SoftStartPwm = IsrVars._i16DcdcPWMTsMax + 60;
			}
			IsrVars._i16IdcPioutPermit = s_i16SoftStartPwm;

			s_lq10VoltWalkIn.lData +=  NON_WALKIN_STEP;

			if(s_lq10VoltWalkIn.lData >= s_lSSVsetMax.lData)
			{

				s_lq10VoltWalkIn.lData = s_lSSVsetMax.lData ;
				g_u16ActionReady = NORMAL_RUN_STATE;
				IsrVars._i16CurrLimFloor = POWER_LIM_FLOOR;
				g_lq10SetVoltTrue.lData = g_lq10SetVolt.lData;

			}
			g_l32IdcdcSysTrue = s_lSSIsetMax.lData >> 10; //
			IsrVars._i16VdcSet = (INT16)(s_lq10VoltWalkIn.lData >> 10);
			IsrVars.l32IdcdcSys = g_l32IdcdcSysTrue;

		}
}

void vInv3_CLLCSoftStart(void)
{
	ubitfloat lTmp;

	if(IsrVars._u16160usTimer >= TIME711US)
	{
		IsrVars._u16160usTimer = 0;

		s_i16SoftStartPwm += SSTART_DCDCPWM_STEP;
		if(s_i16SoftStartPwm >= IsrVars._i16DcdcPWMTsMax + 60)
		{
			s_i16SoftStartPwm = IsrVars._i16DcdcPWMTsMax + 60;

		}
		IsrVars._i16IdcPioutPermit = s_i16SoftStartPwm;

    	g_lq10PfcVoltRegulate.lData = g_lq10SetPfcVolt.lData
    			=__IQsat(g_lq10MdlVolt.lData, VPFC_420V, VPFC_350V);

		lTmp.lData = _IQ10mpy(g_lq10VpfcConSys.lData,g_lq10SetPfcVolt.lData);
		lTmp.lData = (_IQ10mpy(LQ10_DEF_VPFCFACTOR,lTmp.lData))>>10;

		s_i16SoftStartSet += SSTART_PFCVOL_STEP;
		if(s_i16SoftStartSet >= lTmp.iData.iLD)
		{
			s_i16SoftStartSet = lTmp.iData.iLD;


			g_lq10SetPfcVoltTrue.lData = g_lq10SetPfcVolt.lData;

		  	CputoClaVar._u16PfcLoopCount = 1 ;
			CputoClaVar.u16PfcOpened = 1;


			//to next step
			g_u16ActionReady = DC_SOFT_START_INIT;
			IsrVars._u16160usTimer = 0;


			IsrVars._i16CurrLimFloor = POWER_LIM_FLOOR;
			g_lq10SetVoltTrue.lData = g_lq10SetVolt.lData;


		}
		CputoClaVar._iVpfcSet = s_i16SoftStartSet;

		g_l32IdcdcSysTrue = s_lSSIsetMax.lData >> 10; //
		IsrVars.l32IdcdcSys = g_l32IdcdcSysTrue;

	}
}

void vInv4_TotemSoftStartInit(void)
{
	CputoClaVar.u32Theta = 0;
	if(
		(ClatoCpuVar._iVpfcProtect >= VPFCAd_330v) &&
		(g_u16MdlStatus.bit.DCOV == 0) && (g_u16MdlCtrl.bit.OFFCTRL == 0) &&
		(g_lq10AcVolt.lData <= VAC_80V) &&
		(g_lq10PfcVolt.lData >= VPFC_330V)
		)
	{
		vMainRlyOnOffCtrl();

		if(g_u16MainRlyCloseFlag == 3)
	    {
		    g_u16MainRlyCloseFlag = 0;

			Cla1ForceTask1();

			CputoClaVar.u32Theta = 0;

			EPwm9Regs.CMPA.bit.CMPA = EPwm9Regs.TBPRD;
			EPwm10Regs.CMPA.bit.CMPA = 0u;

		    CputoClaVar._u16PfcLoopCount = 0;


		    g_u16ActionReady = DC_SOFT_START;
			IsrVars._u16160usTimer = 0;

	    	mOnPfcPwm();
	   }
	}
}

void vRec2_TotemSoftStartInit(void)
{
	if((g_lq10AcVolt.lData >= VAC_80V)
		&& (g_lq10PfcVolt.lData >= VPFC_80V)
		&& ( (CputoClaVar._uiOverAcVolt == 0) && (g_u16MdlStatus.bit.ACOV == 0))
		)
	{
		vMainRlyOnOffCtrl();

		if((g_u16MainRlyCloseFlag == 3) && (IsrVars.u16FreqCnt <= 10) && (0 == ePLL.u16ErrCnt))
	    {
		    g_u16MainRlyCloseFlag = 0;

			vPfcSwSoftStartInitial();

		    CputoClaVar._u16PfcLoopCount = 0;

		    //to next step
		    g_u16ActionReady = PFC_SWSOFT_START;
			IsrVars._u16160usTimer = 0;

	    	mOnPfcPwm();
	   }
   }
}

void vInv2_CLLCSoftStartInit(void)
{
	if(
	   (g_lq10MdlVolt.lData >= VDC_318V) &&
	   (g_u16MdlStatus.bit.DCOV == 0) && (g_u16MdlCtrl.bit.OFFCTRL == 0) &&
	   (g_u16MdlStatus.bit.HVSDLOCK == 0)&&(g_u16MdlStatusH.bit.DCHWOV  == 0))
	{
		vCalMaxDcCurrRef();

		s_lq10VoltWalkIn.lData = 0;

		g_l32IdcdcSysTrue = s_lSSIsetMax.lData >> 10;
		IsrVars._i16CurrLimFloor = POWER_LIM_FLOOR;


		IsrVars.l32IdcdcSys = g_l32IdcdcSysTrue;

		vInitialDcPwmVar();

		DELAY_US(20);

		vResetDcOcpTripFlag();

		g_u16MdlStatus.bit.OFFSTAT = 0;
		CputoClaVar._u16PfcLoopCount = 0;

		//to next step
		g_u16ActionReady = PFC_SWSOFT_START;
		IsrVars._u16160usTimer = 0;

		s_i16SoftStartPwm = 120;
		s_i16SoftStartSet = 0;

		CputoClaVar._u16PfcLoopCount = 1 ;

		mOnDcPwm_Bat();
	}
}

void vRec4_CLLCSoftStartInit(void)
{
	if(
		(ClatoCpuVar._iVpfcProtect >= VPFCAd_380v) &&
	   (g_u16MdlStatus.bit.DCOV == 0) && (g_u16MdlCtrl.bit.OFFCTRL == 0) &&
	   (g_u16MdlStatus.bit.HVSDLOCK == 0)&&(g_u16MdlStatusH.bit.DCHWOV  == 0))
	{

		vCalMaxDcVoltRef();

        vCalMaxDcCurrRef();

		s_lq10VoltWalkIn.lData = 0;

		g_l32IdcdcSysTrue = s_lSSIsetMax.lData >> 10;
		IsrVars._i16CurrLimFloor = POWER_LIM_FLOOR;

		IsrVars.l32IdcdcSys = g_l32IdcdcSysTrue;

		vInitialDcPwmVar();

		DELAY_US(20);

		vResetDcOcpTripFlag();

		g_u16MdlStatus.bit.OFFSTAT = 0;

		//to next step
		g_u16ActionReady = DC_SOFT_START;
		IsrVars._u16160usTimer = 0;
		s_i16SoftStartPwm = 120;

		CputoClaVar._u16PfcLoopCount = 1 ;
		mOnDcPwm_Bus();
	}

}



void vModeSelect(void)
{
	mOffDcdcPwm();  //  This acts as a "menu" before starting charging processes
	mOffPfcPwm();   // Shut down all outputs, except mainRelay
	mOffHericPwm(); // HERIC (Highly Efficient and Reliable Inverter Concept) inverter
	if (g_BoolChargerIsActive == 1) // -> check if charger is allowed to turn on. Used to automatically turn on.
	{
	if( (g_lq10AcVolt.lData >= VAC_80V)	&& (g_lq10PfcVolt.lData >= VPFC_80V)    // If AC in is greater than 80V and no under/over voltage flags have been triggered
		&& ( (CputoClaVar._uiOverAcVolt == 0) && (g_u16MdlStatus.bit.ACOV == 0)
		)
	 )
	{
		g_u16ActionReady = PFC_SWSOFT_START_INIT;   // Change action (menu choice) to PFC software start initilization

		CputoClaVar.u16WorkMode = MODE_REC;//rec

		g_u16MdlCtrl.bit.Rec_Inv = MODE_REC;

		InitEPWM();                         // Start EPWM output (square wave forms viewable on controlboard headers and mainboard gate drivers

		EPwm9Regs.ETSEL.bit.INTEN = 1;
	}
	else if( (g_lq10MdlVolt.lData >= VDC_318V) && (g_lq10PfcVolt.lData <= VPFC_60V) //  VDC refers to the voltage output, after relay and bus
			 && (g_u16MdlStatus.bit.DCOV == 0) && (g_u16MdlCtrl.bit.OFFCTRL == 0)   // Similar to last, check output and flags for faults
			 && (g_u16MdlStatus.bit.HVSDLOCK == 0) && (g_u16MdlStatusH.bit.DCHWOV  == 0)
			 )
	{
		g_u16ActionReady = PFC_SWSOFT_START_INIT;

		CputoClaVar.u16WorkMode = MODE_INV;//inv

		g_u16MdlCtrl.bit.Rec_Inv = MODE_INV;//Inv Mode

		InitEPWM();

		EPwm9Regs.ETSEL.bit.INTEN = 1;
	}
	else
	{
		CputoClaVar.u16WorkMode = UNVALID;

		g_u16ActionReady = PFC_HWSOFT_START;

		g_u16MdlCtrl.bit.Rec_Inv = MODE_REC;
	}
	}
}


