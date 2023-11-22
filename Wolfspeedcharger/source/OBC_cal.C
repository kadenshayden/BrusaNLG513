/*****************************************************************************
 *
 *
 *  OBC_cal.C
 *  Contains functions for reference voltage cal, DCDC, temp power limit, etc...
 *
 *
 ******************************************************************************/

#include <Shared.h>


static ubitfloat s_lq10TempLimitPower;
static ubitfloat s_lq10ACLimitPower;
static ubitfloat s_lq10AcOldVolt;
static ubitfloat s_lq10ShortLimt;


ubitfloat g_lq10SetVoltTrue;
ubitfloat g_lq10SetLimitTrue;
ubitfloat g_lq10SetPowerTrue;
ubitfloat g_lq10SetPowerWant;
ubitfloat g_lq10MdlLimit;
ubitfloat g_lq10MdlLimitCurrent;
ubitfloat g_lq10SetPfcVolt;
ubitfloat g_lq10SetPfcVoltTrue;
INT32	g_l32IdcdcSysTrue;
UINT16   g_u16LimitStatus;

ubitfloat g_lq10PfcVoltRegulate;

INT32     g_i32PowerRate;
INT32     g_i32CurrRate;
INT32     g_i32CurrDisMax;

static  void    vDcdcVrefCal(void);
static  void    vDcdcCurrStepRegu(void);
static  void    vPowerStepRegu(void);
static  void    vTempLimPowerControl(void);
static  void    vAcLimPowerControl(void);
static  void    vPowerLimCal(void);
static  void    vPowerLimWarningHandle(void);
static  void    vCurrLimCal(void);
static  void    vPfcVoltVrefCal(void);
static  void    vPfcVoltReguAlgorithm(void); 


void  vRefTask(void)
{       
    if( (g_u16ActionReady >= NORMAL_RUN_STATE) 
     && (IsrVars._u16160usTimer >= 225))
    {
        IsrVars._u16160usTimer = 0 ;
       
        vPfcVoltVrefCal();
       	vDcdcVrefCal();
    }       
	
	
    if(g_u16ActionReady && g_u16RunFlag.bit.CAL)
    {
        g_u16RunFlag.bit.CAL = 0;
        
        vPfcVoltReguAlgorithm();
        vDcdcCurrStepRegu();
        vPowerStepRegu();               
        
        vTempLimPowerControl();
        
        vAcLimPowerControl();
        
        vPowerLimCal();
        vPowerLimWarningHandle();               
        	
        vCurrLimCal();
    }
}


void vDcdcVrefCal(void)
{
    longunion lLimitTmp;

	g_lq10SetVoltTrue.lData = __IQsat( g_lq10SetVolt.lData, \
			g_lq10SetVoltTrue.lData + DCVOLT_REGU_STEP, g_lq10SetVoltTrue.lData - DCVOLT_REGU_STEP);

    lLimitTmp.lData = (g_lq12VoltConSysb.lData
            + _IQ10mpy(g_lq10SetVoltTrue.lData, g_lq12VoltConSysa.lData) ) >> 2;
    	
    lLimitTmp.lData = _IQ20mpy(lLimitTmp.lData, LQ10_DEF_VDCFACTOR);
    
    if(lLimitTmp.lData > VDC_AD_MAX)
    {
        lLimitTmp.iData.iLD = VDC_AD_MAX;
    }
    
    IsrVars._i16VdcSet = lLimitTmp.iData.iLD; 
   
}


 void vDcdcCurrStepRegu(void)
{

    g_lq10SetLimitTrue.lData = __IQsat( g_lq10SetLimit.lData, \
    		g_lq10SetLimitTrue.lData + DCCURR_REGU_STEP, g_lq10SetLimitTrue.lData - DCCURR_REGU_STEP);

}


void vPowerStepRegu(void)
{

	g_lq10SetPowerTrue.lData = __IQsat( g_lq10SetPower.lData, \
			g_lq10SetPowerTrue.lData + POW_REGU_STEP, g_lq10SetPowerTrue.lData - POW_REGU_STEP);

}


void vTempLimPowerControl(void)
{
    ubitfloat lq10TempUse;
    static ubitfloat s_lq10TempCOld; //record the last used U1 bord temperature
    
    //set Temp limit power flag
    g_u16LimitStatus |=  POW_LIM_TEMP;        
    
    // calculate power limit of temperature
	if((g_lq10TempAmbiDisp.lData >= s_lq10TempCOld.lData)
    	||(g_lq10TempAmbiDisp.lData < (s_lq10TempCOld.lData - TEMP_3C5)))
	{
		s_lq10TempCOld.lData = g_lq10TempAmbiDisp.lData;
	}
    
	lq10TempUse.lData = s_lq10TempCOld.lData - TEMP_3C5;
    

	{
			if (lq10TempUse.lData > TEMP_50C)
    		{
    			s_lq10TempLimitPower.lData = __lmax(
    					((INT32)3363) - _IQ20mpy(((INT32)49460), lq10TempUse.lData), 0L);
    		}
    		else if (lq10TempUse.lData > TEMP_45C)
    		{
    			s_lq10TempLimitPower.lData = ((INT32)1708)
    	                      - _IQ20mpy(((INT32)15565), lq10TempUse.lData);
    		}
    		else 
    		{
    			s_lq10TempLimitPower.lData = 1024; 
    			g_u16LimitStatus &= ~POW_LIM_TEMP;
    		}  
   	}
}

void vAcLimPowerControl(void)
{       

    ubitfloat lq10AcVoltUse;
    
    g_u16LimitStatus |= POW_LIM_AC;

    if(MODE_INV == CputoClaVar.u16WorkMode)
    {
    	s_lq10ACLimitPower.lData = 1024;
    	g_u16LimitStatus &= ~POW_LIM_AC;
        return;
    }


    if (( g_lq10AcVolt.lData <=  s_lq10AcOldVolt.lData )
        ||( g_lq10AcVolt.lData > ( s_lq10AcOldVolt.lData + VAC_2V )))
    {
    	s_lq10AcOldVolt.lData = g_lq10AcVolt.lData;
    }
    
    lq10AcVoltUse.lData = s_lq10AcOldVolt.lData + VAC_3V5;
   
    
	{
 		 g_u16LimitStatus |= POW_LIM_AC; 

    	if(lq10AcVoltUse.lData >= VAC_210V)
    	{
    		s_lq10ACLimitPower.lData = 1024;
    
    		g_u16LimitStatus &= ~POW_LIM_AC; 
    	}
    	else if(lq10AcVoltUse.lData >= VAC_150V)
    	{
    		s_lq10ACLimitPower.lData = _IQ20mpy(lq10AcVoltUse.lData, ((INT32)6895))
    	                          - ((INT32)390);
    	}
		else if(lq10AcVoltUse.lData >= VAC_85V)
    	{
    		s_lq10ACLimitPower.lData = _IQ20mpy(lq10AcVoltUse.lData, ((INT32)3308))
    	                          + ((INT32)135);
    	}
    	else
    	{
    		s_lq10ACLimitPower.lData = 410;
    	}

	}
	
    if(g_u16MdlStatus.bit.ACOV)
    {
        g_u16LimitStatus &= ~POW_LIM_AC; 
    }
}


void vPowerLimCal(void)
{
    ubitfloat i32LimitData,lq10TempLimit;
    float32 f32Temp;
    int32 l32Verr;
    if(MODE_INV == CputoClaVar.u16WorkMode)
    {
    	i32LimitData.lData = g_lq10SetPowerTrue.lData;
    }
    else
    {
		i32LimitData.lData = __lmin(6351L - _IQ20mpy(12687L, g_lq10MdlVolt.lData), g_lq10SetPowerTrue.lData);
    }
    lq10TempLimit.lData = 
    		_IQ10mpy(s_lq10TempLimitPower.lData, s_lq10ACLimitPower.lData);                                                                
    
    if(i32LimitData.lData > lq10TempLimit.lData)
    {
        i32LimitData.lData = lq10TempLimit.lData;
    }
    

    if(i32LimitData.lData <= 1)
    {    
        i32LimitData.lData = 1;
    }
    
    g_lq10SetPowerWant.lData = i32LimitData.lData;

    
    lq10TempLimit.lData = _IQ10mpy(i32LimitData.lData, g_i32PowerRate) 
    		              +  (300L<<10);
    if(MODE_INV == CputoClaVar.u16WorkMode)
    {
		lq10TempLimit.lData = __IQsat(_IQ20mpy(LQ10_DEF_POWERFACTOR, lq10TempLimit.lData), POWER_LIM_UPPER, POWER_LIM_MIN);
    }
    else
    {
		lq10TempLimit.lData = __IQsat(_IQ20mpy(LQ10_DEF_POWERFACTOR_BAT, lq10TempLimit.lData), POWER_LIM_UPPER_BAT, POWER_LIM_MIN_BAT);

//------------------------------------------
		l32Verr = __IQsat(_IQ10mpy(956L, g_lq10PfcVoltDisp.lData) - g_lq10MdlVolt.lData, (10L)<<10, (-50L)<<10);
		f32Temp = -4.65f*(float32)(l32Verr) /*+ 0.274f*(float32)g_lq10MdlVolt.lData*/ - 150.0f*1024.0f + (float32)g_lq10PowerConSysb.lData;
		f32Temp *= ((float32)g_lq10SetPowerWant.lData/1024.0f);
		if(g_lq10MdlVolt.lData <= VDC_360V)
		{
			lq10TempLimit.lData -= _IQ10mpy(_IQ10mpy(50, g_lq10SetPowerWant.lData), s_lq10ACLimitPower.lData);//1.0 时,  -50
		}
		else
		{
			lq10TempLimit.lData += __IQsat((long)(f32Temp/1024.0f),200,-50);
		}
//----------------------------------------------------------
		lq10TempLimit.lData = __IQsat(lq10TempLimit.lData, POWER_LIM_UPPER_BAT, POWER_LIM_MIN_BAT);
    }

    if((IsrVars._i16PowerLimMax < (lq10TempLimit.lData -POWER_LIM_FLOOR)) 
    	&& (CputoClaVar.u16PfcOpened))
    {
    	IsrVars._i16PowerLimMax += POWER_LIM_FLOOR;
    }
    else
    {
    	IsrVars._i16PowerLimMax = lq10TempLimit.lData;
    }
    
     IsrVars._i16PowerMaxExtra = IsrVars._i16PowerLimMax + 550;
}


void vPowerLimWarningHandle(void)
{
    ubitfloat lTmp;
    							
    if((g_u16LimitStatus & POW_LIM_AC) || (g_u16LimitStatus & POW_LIM_TEMP))            
    {
        lTmp.lData = _IQ10mpy(g_lq10MdlCurr.lData, g_lq10MdlVolt.lData);
        
        if((lTmp.lData + POW_80W >= _IQ10mpy(g_lq10SetPowerWant.lData, g_i32PowerRate))
           && (IsrVars._u16ChoiseCon)) 
        {
            g_u16LimitStatus |= POW_LIM;
        }
        else if((lTmp.lData + POW_100W < _IQ10mpy(g_lq10SetPowerWant.lData, g_i32PowerRate))
        	&& (IsrVars._u16ChoiseCon == 0))
        {               
            g_u16LimitStatus &= ~POW_LIM;
        }
    }
    else
    {
    	g_u16LimitStatus &= ~POW_LIM;
    }       
    	
    if(g_u16MdlStatus.bit.OFFSTAT)
    {
    	g_u16LimitStatus = 0;
    }       
}


void vCurrLimCal(void)
{
    ubitfloat lq10LimitData,lq10TempLimit;
    float32 f32Temp;

	if(IsrVars._u16SoftStartEndTimer <= 50)
  	 {
		s_lq10ShortLimt.lData = LQ10_CURRLIMITMAX;
		return;
	}

	if(g_lq10MdlVolt.lData <= VDC_100V)
	{
			s_lq10ShortLimt.lData = 848;
	}
	else if(g_lq10MdlVolt.lData <= VDC_210V)
	{
		s_lq10ShortLimt.lData = (_IQ10mpy(g_lq10MdlVolt.lData, 7)) +148;
	}
	else
	{
		s_lq10ShortLimt.lData = LQ10_CURRLIMITMAX;
	}

	lq10LimitData.lData = LQ10_CURRLIMITMAX;

    if(lq10LimitData.lData > s_lq10ShortLimt.lData)
    {
        lq10LimitData.lData = s_lq10ShortLimt.lData;
    }
    	
    if(lq10LimitData.lData > g_lq10SetLimitTrue.lData)
    {
    	lq10LimitData.lData = g_lq10SetLimitTrue.lData;
    }
    
    lq10TempLimit.lData = _IQ10mpy(g_lq10SetPowerWant.lData, LQ10_CURRLIMITMAX);
    if(lq10LimitData.lData > lq10TempLimit.lData)  
    {
    	lq10LimitData.lData = lq10TempLimit.lData; 
    }
    
    g_lq10MdlLimit.lData = 
    _IQ10div(_IQ10mpy(g_lq10SetPowerWant.lData, VDC_455V),
    		__lmax(g_lq10MdlVolt.lData,VDC_200V) );
    if(lq10LimitData.lData < g_lq10MdlLimit.lData)
    {
    	g_lq10MdlLimit.lData = lq10LimitData.lData;
    }

	g_lq10MdlLimitCurrent.lData =  _IQ10mpy(g_lq10MdlLimit.lData,g_i32CurrRate); 

    lq10TempLimit.lData = _IQ10mpy(lq10LimitData.lData,g_i32CurrRate);

	if(MODE_INV == CputoClaVar.u16WorkMode)
    {
		lq10LimitData.lData = __IQsat(_IQ20mpy(lq10TempLimit.lData,LQ10_DEF_IDCSYSFACTOR),IDC_AD_MAX,IDC_AD_MIN);
    }
    else
    {
		lq10LimitData.lData = __IQsat(_IQ20mpy(lq10TempLimit.lData,LQ10_DEF_IDCSYSFACTOR_BAT) + (g_lq10CurrConSysb.lData>>10), \
								IDC_AD_MAX_BAT,IDC_AD_MIN_BAT);//B=200*1024
//------------------------------------------------------------------
/*		if(g_lq10MdlVolt.lData <= (370L<<10))
		{
			f32Temp = (8.5f*(float32)g_lq10MdlVolt.lData - 3018.0f*1024.0f);
			if(g_lq10AcVolt.lData <= VAC_195V)
			{
				f32Temp = f32Temp*(float32)s_lq10ACLimitPower.lData/1024.0f*(float32)g_lq10SetPowerWant.lData/1024.0f;

				if( (g_lq10MdlVolt.lData >= (230L<<10)) && (g_lq10MdlVolt.lData <= (330L<<10)))
				{
					f32Temp += (float32)((330L<<10) - g_lq10MdlVolt.lData)*2.5f; //大于零实际不用
				}
			}
		}
		else */
		{
			f32Temp = 0.0f;
		}
		lq10LimitData.lData += __IQsat((long)(f32Temp/1024.0f), 0, _IQ10mpy(g_lq10SetPowerWant.lData, -600));//-500
//-----------------------------------------------------------------
		lq10LimitData.lData = __IQsat(lq10LimitData.lData,IDC_AD_MAX_BAT,IDC_AD_MIN_BAT);
    }

    
    if(g_u16ActionReady == NORMAL_RUN_STATE)                    
    {               
        
        if(g_l32IdcdcSysTrue < (lq10LimitData.lData - 200))
        {
        	g_l32IdcdcSysTrue += 200;
        }
        else
        {
        	g_l32IdcdcSysTrue = lq10LimitData.lData;
        }
        IsrVars.l32IdcdcSys = g_l32IdcdcSysTrue;
        	
    }
    	
    if(IsrVars.l32IdcdcSys == g_l32IdcdcSysTrue)
    {
        IsrVars._i16CurrLimFloor -= 10 ;
    	if(IsrVars._i16CurrLimFloor < 20)
    	{
    	    IsrVars._i16CurrLimFloor = 20;                    
    	}
    }
}

void vPfcVoltVrefCal(void)
{
    longunion   iTmp;

	g_lq10SetPfcVoltTrue.lData = __IQsat( g_lq10PfcVoltRegulate.lData, \
			g_lq10SetPfcVoltTrue.lData + PFCVOLT_REGU_STEP, g_lq10SetPfcVoltTrue.lData - PFCVOLT_REGU_STEP);
    
    iTmp.lData = _IQ10mpy(g_lq10VpfcConSys.lData, g_lq10SetPfcVoltTrue.lData);
    iTmp.lData = (_IQ10mpy(LQ10_DEF_VPFCFACTOR, iTmp.lData)) >> 10;    
    CputoClaVar._iVpfcSet = iTmp.iData.iLD;
}

void vPfcVoltReguAlgorithm(void)
{
	longunion   lTmpVpfc_A, lTmpVpfc_B,lTmpVpfc_C;
	if(MODE_INV == CputoClaVar.u16WorkMode)
	{

    	g_lq10PfcVoltRegulate.lData = g_lq10SetPfcVolt.lData
    			=__IQsat(_IQ10mpy(g_lq10MdlVolt.lData, 1097) + VDC_3V5, VPFC_420V, VPFC_350V);
	}
	else if(IsrVars._u16SoftStartEndTimer >= 100)//60
	{
		  lTmpVpfc_A.lData = __IQsat(_IQ10mpy(g_lq10MdlVolt.lData, 1097) + VDC_3V5, VPFC_430V, VPFC_380V);
		  //lTmpVpfc_A.lData = lTmpVpfc_C.lData;
		  lTmpVpfc_B.lData =  _IQ10mpy(g_lq10AcVolt.lData,1448) + 2048;

		  lTmpVpfc_C.lData =  __IQsat(
				  __lmax(lTmpVpfc_A.lData, lTmpVpfc_B.lData), VPFC_430V, VPFC_380V);

          if(g_lq10PfcVoltRegulate.lData >= lTmpVpfc_C.lData+_IQ10(3.0))
          {
              g_lq10PfcVoltRegulate.lData -= _IQ10(3.0);
          }
          else
          {
              g_lq10PfcVoltRegulate.lData = lTmpVpfc_C.lData;
          }
	}
	else
    {
    	g_lq10PfcVoltRegulate.lData = g_lq10SetPfcVolt.lData;
    }
}

//===========================================================================
// No more.
//===========================================================================

