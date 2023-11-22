/*****************************************************************************
 *
 *
 * OBC_warn.c
 * For use with IDLE loop
 * Used for fault checks and warn task
 *
 *
*****************************************************************************/

#include <Shared.h>


static	UINT16	s_u16MdlOpenBak;

UINT16 g_u16CanFailTime;
UINT16 g_u1660sTimer;


//fault check
static	void  vPfcFaultCheck();
static	void  vAcFaultCheck();
static	void  vAmbiTempCheck();
static	void  vDCTempCheck();
static	void  vPFCTempCheck();
static	void  vDcVoltOverHandle(void);			
static	void  vCanFailChk(void);	
static	void  vOnOffCtrl(void);					
static	void  vRelayCtrl(void);
static	void  vLoopCtrl(void);
static	void  vSrOnOffCtrl(void);
static  void  vPLLStopClear(void);

UINT16	uiOverLevelChk(INT32 lCompareData, INT32 lHighLever, UINT16 u16Filter,
                       UINT16 *pCounter);

UINT16	uiUnderLevelChk(INT32 lCompareData, INT32 lLowLever, UINT16 u16Filter,
                        UINT16 *pCounter);


void  vWarnTask(void)               // Manages tasks that monitor charging stats (overvolt, relay, SR, temps, PFC)
{
	if (g_u16RunFlag.bit.WARN)
	{
		g_u16RunFlag.bit.WARN = 0;    
		
		
		vPLLStopClear();

		if(MODE_INV != CputoClaVar.u16WorkMode)
		{
			vPfcFaultCheck();
		}
		else
		{
			g_u16MdlStatus.bit.PFCOV = 0;
		}
		vAcFaultCheck();

		vAmbiTempCheck();


		vDCTempCheck();

		vPFCTempCheck();

	
		vDcVoltOverHandle();

		vCanFailChk();


		vOnOffCtrl();	
		
		vRelayCtrl();

		vLoopCtrl();

        vSrOnOffCtrl();
		
	}
}

static	void  vPfcFaultCheck()
{
	static UINT16 s_u16PfcOVChkCnt, s_u16PfcUnChkCnt;

    if(g_u16MdlStatus.bit.PFCOV == 0)
    {
        if(uiOverLevelChk(g_lq10PfcVoltDisp.lData, VPFC_HLOSS, 320,
        				  &s_u16PfcOVChkCnt))
        {
            g_u16MdlStatus.bit.PFCOV = 1;
        }
    }
    
    else
    {
        if(uiUnderLevelChk(g_lq10PfcVoltDisp.lData,VPFC_HBACK,1,
        				   &s_u16PfcOVChkCnt))
        {
            g_u16MdlStatus.bit.PFCOV = 0;
        }
    }
	if(CputoClaVar.u16PfcOpened == 1)
	{
		if((uiUnderLevelChk(g_lq10PfcVoltDisp.lData, VPFC_LLOSS, 320,
						   &s_u16PfcUnChkCnt)) ||(CputoClaVar._uiLowPfcVolt))
		{
			g_u16MdlStatus.bit.PFCUV = 1;
		}
	}
	else
	{
		g_u16MdlStatus.bit.PFCUV = 0;
	}
}

static	void  vAcFaultCheck()
{
	static UINT16 s_u16BatUnChkCnt, s_u16AcUnChkCnt, s_u16AcOVChkCnt;

	if(MODE_INV == CputoClaVar.u16WorkMode)
	{
		if(g_u16ActionReady >= PFC_SWSOFT_START_INIT && g_u16MdlCtrl.bit.SHORTRELAY == 0)
		{
			if(g_u16MdlStatus.bit.ACUV == 0)
			{
				if(uiUnderLevelChk(g_lq10MdlVolt.lData, 295L<<10, 10,
								   &s_u16BatUnChkCnt))
				{
					g_u16MdlStatus.bit.ACUV = 1;
				}
			}
			else
			{
				if(uiOverLevelChk(g_lq10MdlVolt.lData, 310L<<10, 200,
								   &s_u16BatUnChkCnt))
				{
					g_u16MdlStatus.bit.ACUV = 0;
				}
			}
		}
		else
		{
			g_u16MdlStatus.bit.ACUV = 0;
		}
	}
	else
	{
		if(g_u16ActionReady >= PFC_SWSOFT_START_INIT)
		{
			if(g_u16MdlStatus.bit.ACUV == 0)
			{
				if(uiUnderLevelChk(g_lq10AcVolt.lData, VAC_LLOSS, 10,
								   &s_u16AcUnChkCnt))
				{
					g_u16MdlStatus.bit.ACUV = 1;
				}
			}
			else
			{
				if(uiOverLevelChk(g_lq10AcVolt.lData, VAC_LBACK, 200,
								   &s_u16AcUnChkCnt))
				{
					g_u16MdlStatus.bit.ACUV = 0;
				}
			}
		}
		else
		{
			g_u16MdlStatus.bit.ACUV = 0;
		}

		if(g_u16MdlStatus.bit.ACOV == 0)
		{
			if(uiOverLevelChk(g_lq10AcVolt.lData, VAC_HLOSS, 10,
							  &s_u16AcOVChkCnt))
			{
				g_u16MdlStatus.bit.ACOV = 1;

				g_u16MdlStatus.bit.ACUV = 0;
			}

		}
		else
		{
			if(uiUnderLevelChk(g_lq10AcVolt.lData, VAC_HBACK, 200,
							   &s_u16AcOVChkCnt))
			{
				g_u16MdlStatus.bit.ACOV = 0;
			}
		}
		if(CputoClaVar._uiOverAcVolt == 1)
		{
			g_u16MdlStatus.bit.ACOV = 1;
		}
	}
}

static	void	vOnOffCtrl(void)	
{
	if((g_u16MdlStatus.bit.DCOV||g_u16MdlStatusH.bit.DCHWOV			
		||g_u16MdlCtrl.bit.OFFCTRL)
		)
	{
		s_u16MdlOpenBak |=  DCDC_OFF;

		mOffDcdcPwm();

		if(MODE_INV == CputoClaVar.u16WorkMode)
		{
			mOffPfcPwm();
		}


		if((g_u16ActionReady == NORMAL_RUN_STATE) 
		    ||(g_u16ActionReady == DC_SOFT_START))
		{
			g_u16ActionReady = DC_CLOSE_STATE;
		}			
		
		g_u16MdlStatus.bit.OFFSTAT = 1;
		g_u16MdlStatus.bit.SrOn = 0;
	}
	else
	{
	
		if((s_u16MdlOpenBak & DCDC_OFF) 
		&&(g_u16ActionReady >= DC_SOFT_START_INIT)&&(g_u16MdlStatusH.bit.DCHWOV == 0 ))
		{
			g_u16ActionReady = DC_SOFT_START_INIT; 

			g_u16MdlStatus.bit.SrOn = 0;

			s_u16MdlOpenBak &=~ DCDC_OFF;
		}
	}


	//turn off
	if(g_u16MdlStatus.bit.HVSDLOCK || g_u16MdlStatus.bit.ACOV
	   || g_u16MdlStatus.bit.ACUV
	   || g_u16MdlStatus.bit.PhaseUnlock || g_u16MdlStatus.bit.AMBIOT
       || g_u16MdlStatusH.bit.DCOT || g_u16MdlStatusH.bit.PFCOT
	   || g_u16MdlStatusH.bit.SROT || g_u16MdlStatus.bit.PFCUV
	   || g_u16MdlStatus.bit.PFCOV || g_u16MdlCtrl.bit.SHORTRELAY
	   )
	{
		s_u16MdlOpenBak |= PFC_OFF;			   
		if( (MODE_INV == CputoClaVar.u16WorkMode)
				&& (g_lq10PfcVolt.lData >= VPFC_50V)
				&& mMainRelayCloseCheck()
		)
		{
			mOffDcdcPwm();
		}
		else if( (MODE_REC == CputoClaVar.u16WorkMode)
					&& (1 == g_u16MdlStatus.bit.PhaseUnlock)
					&& (g_lq10PfcVolt.lData >= VPFC_50V)
					&& mMainRelayCloseCheck()
			)
		{
			mOffPfcPwm();
			mOffDcPwm_Bat();
		}
		else
		{
			mOffDcdcPwm();
			mOffPfcPwm();
	 		mMainRelayOpen();
		}
		
		if((g_u16ActionReady <= NORMAL_RUN_STATE) && (UNVALID != CputoClaVar.u16WorkMode))
		{
			g_u16ActionReady = AC_CLOSE_STATE;
		}		 
	
		g_u16MdlStatus.bit.OFFSTAT = 1;
		g_u16MdlStatus.bit.SrOn = 0;


		CputoClaVar._uiLowPfcVolt = 0;
		CputoClaVar._uiOverAcVolt = 0;
		CputoClaVar.u16PfcOpened = 0;
	}

	else
	{
		if (s_u16MdlOpenBak & PFC_OFF)
		{
			g_u16ActionReady = POWON_STATE;	
			s_u16MdlOpenBak &=~ PFC_OFF;
		}
	}
}

static	void  vRelayCtrl(void)

{
	if((g_u16MdlStatus.bit.ACUV) ||
	   (g_u16MdlStatus.bit.ACOV)||(g_u16MdlCtrl.bit.SHORTRELAY))
	{
		if(g_lq10PfcVolt.lData >= VPFC_50V)
		{
			if( (MODE_INV == CputoClaVar.u16WorkMode) //off cmd received for INV
					|| ( (MODE_REC == CputoClaVar.u16WorkMode)
							&& (1 == g_u16MdlStatus.bit.PhaseUnlock) )
			)
			{return;}
		}
	   	//control main relay zvs off
		vMainRlyOnOffCtrl();		
		if(g_u16MainRlyCloseFlag == MAINRLY_ZVS_CTL_CLOSE)     
		{			
	    	g_u16MainRlyCloseFlag = MAINRLY_ZVS_CTL_OPEN;
	    }   		
	}
	
}


static	void  vAmbiTempCheck()
{
	static UINT16 s_u16AmbiTempChkCnt;

    if(g_u16MdlStatus.bit.AMBIOT == 0)
    {
        if(uiOverLevelChk(g_lq10TempAmbiDisp.lData, TEMP_AMBI_HLOSS, 10,
           &s_u16AmbiTempChkCnt))
        {
            g_u16MdlStatus.bit.AMBIOT = 1;
        }
    }
    
    else
    {
        if(uiUnderLevelChk(g_lq10TempAmbiDisp.lData,TEMP_AMBI_HBACK,10,
           &s_u16AmbiTempChkCnt))
        {
            g_u16MdlStatus.bit.AMBIOT = 0;
        }
    }
}

static	void  vDCTempCheck()
{
	static UINT16 s_u16DCTempChkCnt;

    if(g_u16MdlStatusH.bit.DCOT == 0)
    {
        if(uiOverLevelChk(g_lq10TempDCDisp.lData, g_lq10DCTempUp.lData, 10,
           &s_u16DCTempChkCnt))
        {
            g_u16MdlStatusH.bit.DCOT = 1;
        }
    }
    
    else
    {
        if(uiUnderLevelChk(g_lq10TempDCDisp.lData,
        				  (g_lq10DCTempUp.lData - ((INT32)20<<10)),
        				  10,&s_u16DCTempChkCnt))
        {
            g_u16MdlStatusH.bit.DCOT = 0;
        }
    } 
}        

static	void  vPFCTempCheck()
{
	static UINT16 s_u16PFCTempChkCnt;

    if(g_u16MdlStatusH.bit.PFCOT == 0)
    {
        if(uiOverLevelChk(g_lq10TempPFCDisp.lData, g_lq10PFCTempUp.lData, 10,
           &s_u16PFCTempChkCnt))
        {
            g_u16MdlStatusH.bit.PFCOT = 1;
        }
    }
    
    else
    {
        if(uiUnderLevelChk(g_lq10TempPFCDisp.lData,
        				  (g_lq10PFCTempUp.lData - ((INT32)20<<10)),
        				  10,&s_u16PFCTempChkCnt))
        {
            g_u16MdlStatusH.bit.PFCOT = 0;
        }
    } 
}        


static	void  vDcVoltOverHandle(void)
{	
	static UINT16 s_u16HvsdChkCnt;
	static UINT16 s_u16DcovpCheckCnt;
	if(g_u16MdlStatus.bit.DCOV == 0)
	{

		if(uiOverLevelChk(g_lq10MdlVolt.lData, g_lq10MdlVoltUp.lData,
						  15, &s_u16HvsdChkCnt))
		{
			g_u16MdlStatus.bit.DCOV = 1;
		}
	}

	else
	{

		if(uiUnderLevelChk(g_lq10MdlVolt.lData,
						   (g_lq10MdlVoltUp.lData - VDC_10V/*VDC_0V5*/),
						   15,&s_u16HvsdChkCnt))
		{
			g_u16MdlStatus.bit.DCOV = 0;
		}
	}

	
	if(g_u16MdlStatusH.bit.DCHWOV == 0)
	{

		if((hiHvsd)&&(( IsrVars._lVdcUse.iData.iHD - IsrVars._i16VdcSet )<= VDC_AD_03V))
		{
			s_u16DcovpCheckCnt ++;

			if(s_u16DcovpCheckCnt >= 15)
			{
				s_u16DcovpCheckCnt = 15;
				g_u16MdlStatusH.bit.DCHWOV = 1;
			}	
		}
		else
		{
				s_u16DcovpCheckCnt = 0;
		}		
	 }
	else
	{
		if(hiHvsd == 0)
		{
			g_u16MdlStatusH.bit.DCHWOV = 0;

		}
	}

	if((g_u16MdlStatus.bit.DCOV)&& (g_u16MdlStatusH.bit.DCHWOV))
	{
		g_u16MdlStatus.bit.HVSDLOCK = 1;
	}
}

static	void	vCanFailChk(void)
{
	g_u16CanFailTime ++;
	
	if(g_u16CanFailTime > TIME40S)
	{
		g_u16MdlStatus.bit.NOCONTROLLER = 1;
		
		g_u16MdlCtrl.bit.OFFCTRL = 0;	
		
		g_u16MdlCtrl.bit.SHORTRELAY = 0;
		
		g_u16CanFailTime = 0;

		if(0 == CanaRegs.CAN_CTL.bit.Init)
		{
			InitECanaIDPointer();
		}
	}
}



static	void	vLoopCtrl(void)
{
	static UINT16 s_u16LedTimer;

	if(CputoClaVar._u16PfcLoopCount)
	{
		CputoClaVar._u16PfcLoopCount ++ ;
	}
	CputoClaVar._u16PfcLoopCount = __min(CputoClaVar._u16PfcLoopCount, TIME2S5); //оч╥Ы

	if(g_u16ActionReady != NORMAL_RUN_STATE)
	{
 	    IsrVars._u16SoftStartEndTimer = 0;
	}
 	else
 	{
 		IsrVars._u16SoftStartEndTimer++;
 		if(IsrVars._u16SoftStartEndTimer >= 300)
		{
			IsrVars._u16SoftStartEndTimer = 300;
		}
 	}



	if (++s_u16LedTimer >= TIME500MS)
	{
		g_u16RunFlag.bit.LIGHT = 1;
		s_u16LedTimer = 0;
	}

	//Watch-dog LED  twinkle control
	if (g_u16RunFlag.bit.LIGHT)
	{
		mWDLedTwink();
	}


	if (g_u16MdlStatus.bit.HVSDLOCK
		|| g_u16MdlStatus.bit.ACOV || g_u16MdlStatus.bit.ACUV
		|| g_u16MdlStatus.bit.PFCOV || g_u16MdlStatus.bit.PFCUV
		||g_u16MdlStatus.bit.PhaseUnlock || g_u16MdlStatus.bit.AMBIOT
		|| g_u16MdlStatusH.bit.DCOT || g_u16MdlStatusH.bit.PFCOT
		|| g_u16MdlStatusH.bit.SROT)
	{
		if(g_u16RunFlag.bit.LIGHT)
		{
			mGreenLedTwink();
		}
	}
	else if(IsrVars._u16DcdcOCP != 0)
	{
		mGreenLedOn();
	}
	else
	{
		mGreenLedOff();
	}


	g_u16RunFlag.bit.LIGHT = 0;
}

static	void  vSrOnOffCtrl(void)
{
	if( (g_lq10MdlCurr.lData <= IDC_1A5) || (g_u16ActionReady != NORMAL_RUN_STATE) || (UNVALID == CputoClaVar.u16WorkMode))
	{
		g_u16MdlCtrl.bit.SREnable = 0;
		if(MODE_REC == CputoClaVar.u16WorkMode)
		{
			mOffDcPwm_Bat();
		}
		else if(MODE_INV == CputoClaVar.u16WorkMode)
		{
			mOffDcPwm_Bus();
		}
		else
		{
			mOffDcPwm_Bus();
			mOffDcPwm_Bat();
		}
		g_u16MdlStatus.bit.SrOn = 0;
	}
	else if(g_lq10MdlCurr.lData >= IDC_2A5)
	{
		if( (g_u16MdlCtrl.bit.SREnable) || (g_lq10MdlCurr.lData >= IDC_5A) )
		{
			g_u16MdlCtrl.bit.SREnable = 0;

			if(MODE_REC == CputoClaVar.u16WorkMode)
			{
				if((0 == EPwm1Regs.AQCSFRC.all) && (0x5 == EPwm5Regs.AQCSFRC.all))
				{mOnDcPwm_Bat(); g_u16MdlStatus.bit.SrOn = 1;}
			}
			else if(MODE_INV == CputoClaVar.u16WorkMode)
			{
				if((0x5 == EPwm1Regs.AQCSFRC.all) && (0 == EPwm5Regs.AQCSFRC.all))
				{mOnDcPwm_Bus(); g_u16MdlStatus.bit.SrOn = 1;}
			}
		}
	}
}


static  void  vPLLStopClear(void)
{
	if (mMainRelayCloseCheck()
		|| (MODE_INV == CputoClaVar.u16WorkMode)
		)
	{;}
	else if(g_u16MdlStatus.bit.PhaseUnlock)
	{
		if( (g_lq10AcVolt.lData >= VAC_80V)
			&& (ePLL.f32Vamp_Est >= 115.0f)
			&& (0 == ePLL.u16ErrCnt)
			&& (g_lq10PfcVolt.lData >= VPFC_80V)
		 )
		{
			ePLL.u16ErrCnt = 70;
			g_u16MdlStatus.bit.PhaseUnlock = 0;
		}
	}
}


void MyEnableInterrupts(void)
{
	StartCpuTimer2();
	CpuTimer2Regs.TCR.bit.TIF = 1;
	CpuTimer2Regs.TCR.bit.TIE = 1;
	EPwm9Regs.ETSEL.bit.INTEN = 1;
	IER |= M_INT2;
	IER |= M_INT14;
	
	EINT;
	ERTM;

}




void vInitialInterrupts(void)
{

    EALLOW;
	PieVectTable.TIMER2_INT = &dcdc_ISR;
	PieVectTable.EPWM1_TZ_INT = &epwm1_tzint_isr;

	EDIS;

//	PieCtrlRegs.PIEIER2.bit.INTx1 = 1;

}

UINT16	uiOverLevelChk(INT32 lCompareData, INT32 lHighLever, UINT16 u16Filter, 
					   UINT16 *pCounter)
{
    if(lCompareData > lHighLever)
	{
		(*pCounter) ++;
	}
    
    else
	{
		(*pCounter) = 0;
	}

    if((*pCounter) >= u16Filter)
    {
		(*pCounter) = 0;
		return(true);
    }
   	
   	else
	{
    	return(false);
	}
}


UINT16	uiUnderLevelChk(INT32 lCompareData, INT32 lLowLever, UINT16 u16Filter, 
						UINT16 *pCounter)
{
    
    if(lCompareData <  lLowLever)    
	{                                
		(*pCounter) ++;               
	}                                
    
    else
	{
		(*pCounter) = 0;
	}

    if((*pCounter) >= u16Filter)
    {
		(*pCounter) = 0;
		return(true);
    }
    
    else
	{
    	return(false);
	}
}

//===========================================================================
// No more.
//===========================================================================

