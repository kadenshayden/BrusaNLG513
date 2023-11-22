/*****************************************************************************
 *
 * OBC_vAdc.c
 * Typically  used to convert an analog signal such as voltage to
 * a digital form so that it can be read and processed by a microcontroller.
 *
 * In this case, it's used just for voltage calculations.
 *
 *
 *****************************************************************************/


#include <Shared.h>

static INT32 s_ai32AdcDataAcMax[31];
static INT32 s_i32AdcSumAcMax;

static INT32 s_ai32AdcDataAcRms[31];
static INT32 s_i32AdcSumAcRms;

static INT32 s_ai32DataPfcVolt[31];
static INT32 s_i32SumPfcVolt;

static INT32 s_ai32AdcDataDcVolt[31];
static INT32 s_i32AdcSumDcVolt;

static INT32 s_ai32AdcDataIbat[31];
static INT32 s_i32AdcSumIbat;

static INT32 s_ai32AdcDataDcCurr[31];
static INT32 s_i32AdcSumDcCurr;

static INT32 s_ai32AdcDataTempDC[16];
static INT32 s_i32AdcSumTempDC;

static INT32 s_ai32AdcDataTempPFC[16];
static INT32 s_i32AdcSumTempPFC;
                                     
static INT32 s_ai32AdcDataTempU[31];
static INT32 s_i32AdcSumTempU;
                                   
static UINT16 s_u16AdcPoint;
static UINT16 s_u16AdcPoint2;

ubitfloat g_lq10AcVolt;
ubitfloat g_lq10PfcVolt;
ubitfloat g_lq10PfcVoltDisp;
ubitfloat g_lq10MdlVoltNoFilter;

ubitfloat g_lq10MdlVolt;
ubitfloat g_lq10MdlCurrNoFilter;
ubitfloat g_lq10MdlCurr;
ubitfloat g_lq10MdlCurrDisp;
ubitfloat g_lq10BatCurr;
ubitfloat g_lq10TempAmbi;
ubitfloat g_lq10TempAmbiDisp;
ubitfloat g_lq10TempDC;
ubitfloat g_lq10TempDCDisp;
ubitfloat g_lq10TempPFC;
ubitfloat g_lq10TempPFCDisp;

ubitfloat g_lq10AcMAXVolt;

ubitfloat g_lq10AcVoltChge;

void vAcVoltCal(void);
void vPfcVoltCal(void);
void vPfcVoltFilter(void);
void vDcdcVoltCal(void);
void vDcdcVoltFilter(void);
void vIBatCal_Filter(void);
void vDcdcCurrCal(void);
void vDcdcCurrFilter(void);
void vOutputPowerCal(void);
void vU1TemperatureCal(void);
void vU1TemperatureFilter(void);
float NTCTempCal(float TempADFilter);
void vDCTemperatureCal(void);
void vDCTemperatureFilter(void);
void vPFCTemperatureCal(void);
void vPFCTemperatureFilter(void);


void vAdcTask(void)
{
	if (g_u16RunFlag.bit.ADC)
	{
		g_u16RunFlag.bit.ADC = 0;
		
		vAcVoltCal();

		vPfcVoltCal();
	    vPfcVoltFilter();

		vDcdcVoltCal();
		vDcdcVoltFilter();

		vIBatCal_Filter();

		vDcdcCurrCal();
		vDcdcCurrFilter();


		vU1TemperatureCal();
		vU1TemperatureFilter();

		vDCTemperatureCal();
		vDCTemperatureFilter();

		vPFCTemperatureCal();
		vPFCTemperatureFilter();


		s_u16AdcPoint++;
		if( s_u16AdcPoint >= 30)
		{
			s_u16AdcPoint = 0;
		}
        
		s_u16AdcPoint2++;
		if( s_u16AdcPoint2 >= 15)
		{
			s_u16AdcPoint2 = 0;
		}
	}
}


void vAcVoltCal(void)
{
   ubitfloat  lTmp,lCalMAXTemp;
	s_ai32AdcDataAcMax[30] = IsrVars._i16AdAcMax;
	s_i32AdcSumAcMax += s_ai32AdcDataAcMax[30] 
	                    - s_ai32AdcDataAcMax[s_u16AdcPoint];
	s_ai32AdcDataAcMax[s_u16AdcPoint] = s_ai32AdcDataAcMax[30];
	IsrVars._i16AdAcMax = 0;

    s_ai32AdcDataAcRms[30] = (long)ClatoCpuVar._f32AcVoltSquareRms;
    s_i32AdcSumAcRms += (INT32)s_ai32AdcDataAcRms[30] 
                     -(INT32)s_ai32AdcDataAcRms[s_u16AdcPoint];
    s_ai32AdcDataAcRms[s_u16AdcPoint] = (INT32)s_ai32AdcDataAcRms[30];


	lTmp.lData = _IQ20mpy(INV30_IQ20, s_i32AdcSumAcRms);
	lTmp.lData = _IQ6sqrt(lTmp.lData<< 6);
	lTmp.lData = _IQ16mpy(LQ20_DEF_INV_VACFACTOR, lTmp.lData);//Q20*Q6>>16
	lCalMAXTemp.lData = _IQ20mpy(LQ20_DEF_INV_VACFACTOR, _IQ10mpy(INV30_IQ20, s_i32AdcSumAcMax));

	lCalMAXTemp.lData = _IQ10mpy(INV_SQRT2_IQ10, lCalMAXTemp.lData);
	
	lCalMAXTemp.lData = lTmp.lData;

	lTmp.lData = _IQ10mpy(lCalMAXTemp.lData,g_lq10AcVrmsSampSysa.lData) 
		         + g_lq10AcVrmsSampSysb.lData ;
				
	//Volt*0.95+Tmp*0.025
	if(labs(g_lq10AcVolt.lData - lTmp.lData) < VAC_10V)
	{
		lTmp.lData = (g_lq10AcVolt.lData * IQ12_ADC_VAC_FILTA 
		              + lTmp.lData * IQ12_ADC_VAC_FILTB) >> 12;
	}
	g_lq10AcVolt.lData = (INT32)lTmp.lData;
	if(g_lq10AcVolt.lData <= VAC_50V)//50v
	{
		g_lq10AcVolt.lData = VAC_0V;
	}
	
	if(g_lq10AcVolt.lData>(g_lq10AcVoltChge.lData+VAC_2V))
	{
		g_lq10AcVoltChge.lData= g_lq10AcVolt.lData;	
	}
	else if(g_lq10AcVolt.lData<(g_lq10AcVoltChge.lData-VAC_2V))
	{
		g_lq10AcVoltChge.lData= g_lq10AcVolt.lData;	
	}
    
	if(g_lq10AcVolt.lData < VAC_80V)
	{
		CputoClaVar.f32VacRmsSqrInv = 10.0f/(float)(85.0*85.0);
	}
	else
	{ 
		lTmp.lData = _IQ20mpy(g_lq10AcVolt.lData, g_lq10AcVolt.lData);
		CputoClaVar.f32VacRmsSqrInv = 10.0f/(float)lTmp.lData;
	}
}


void vPfcVoltCal(void)
{
	ubitfloat	lq10Temp;
	lq10Temp.lData = __IQmpy(LQ20_DEF_INV_VPFCFACTOR, ClatoCpuVar._iVpfcProtect, 10);
	//pfc  volt adjust
	g_lq10PfcVolt.lData = _IQ10mpy(lq10Temp.lData, g_lq10VpfcSampSys.lData);
}


void vPfcVoltFilter(void)
{
	ubitfloat	lq10Temp;

	s_ai32DataPfcVolt[30] = g_lq10PfcVolt.lData;			
	s_i32SumPfcVolt += s_ai32DataPfcVolt[30] - s_ai32DataPfcVolt[s_u16AdcPoint];
    s_ai32DataPfcVolt[s_u16AdcPoint] = s_ai32DataPfcVolt[30];

	//ave
	lq10Temp.lData = _IQ20mpy(INV30_IQ20, s_i32SumPfcVolt);

    //filter with Volt*0.95+Tmp*0.05		
	if (labs(lq10Temp.lData - g_lq10PfcVoltDisp.lData) < VPFC_0V3)
	{
		lq10Temp.lData = _IQ10mpy(g_lq10PfcVoltDisp.lData,LQ10_ADC_VPFC_FILTA) 
		                 + _IQ10mpy(lq10Temp.lData,LQ10_ADC_VPFC_FILTB);
	}
	
	//limit
	g_lq10PfcVoltDisp.lData = __IQsat(lq10Temp.lData,VPFC_500V, VPFC_1V);//VPFC_60V
}


void vDcdcVoltCal(void)
{
	g_lq10MdlVoltNoFilter.lData = (_IQ10mpy(
				_IQ10mpy(LQ20_DEF_INVVDCFACTOR, (INT32)IsrVars._lVdcDisUse.iData.iHD),
						   g_lq12VoltSampSysa.lData) + g_lq12VoltSampSysb.lData) >> 2;
}


void vDcdcVoltFilter(void)
{
	ubitfloat	lq10Temp;

	s_ai32AdcDataDcVolt[30] = g_lq10MdlVoltNoFilter.lData;
	s_i32AdcSumDcVolt += s_ai32AdcDataDcVolt[30] 
	                     - s_ai32AdcDataDcVolt[s_u16AdcPoint];
	s_ai32AdcDataDcVolt[s_u16AdcPoint] = s_ai32AdcDataDcVolt[30];

	//ave
	lq10Temp.lData = _IQ20mpy(INV30_IQ20, s_i32AdcSumDcVolt);
	

	//filter with Volt*0.95+Tmp*0.05		
	if(labs(lq10Temp.lData - g_lq10MdlVolt.lData) < VDC_0V2)
	{
		lq10Temp.lData = (g_lq10MdlVolt.lData * LQ10_ADC_VDC_FILTA
		                  + lq10Temp.lData * LQ10_ADC_VDC_FILTB) >> 10;
	}
	
	//limit display
	g_lq10MdlVolt.lData = __IQsat(lq10Temp.lData,VDIS_HST,VDC_1MV);
}


void vIBatCal_Filter(void)
{

	ubitfloat	lq10Temp;
	float32 f32Temp;

	lq10Temp.lData = __IQsat(_IQ10mpy(956L, g_lq10PfcVoltDisp.lData) - g_lq10MdlVolt.lData, (100L)<<10, (-50L)<<10);
	if(IsrVars._lIdcPiout.iData.iHD >= DC_Ts_Min_300K)
	{
		f32Temp = 0.02299f*(float32)IsrVars.l32IdcBatDisUse.iData.iHD*1024.0f
					+ 0.0161f*(float32)(lq10Temp.lData) - 0.1909f*1024.0f;
	}
	else
	{
		f32Temp = 0.02399f*(float32)IsrVars.l32IdcBatDisUse.iData.iHD*1024.0f
					+ 0.0004266f*(float32)(lq10Temp.lData) - 0.34f*1024.0f;
	}
	s_ai32AdcDataIbat[30] = (long)f32Temp;

	s_i32AdcSumIbat += s_ai32AdcDataIbat[30] - s_ai32AdcDataIbat[s_u16AdcPoint];
	s_ai32AdcDataIbat[s_u16AdcPoint] = s_ai32AdcDataIbat[30];

	//cal ave
	lq10Temp.lData = _IQ20mpy(INV30_IQ20, s_i32AdcSumIbat);

	if(labs(lq10Temp.lData - g_lq10BatCurr.lData) < IDC_0A5)
	{
		lq10Temp.lData = (g_lq10BatCurr.lData * LQ10_ADC_DCCURR_FILTA
		                  + lq10Temp.lData * LQ10_ADC_DCCURR_FILTB) >> 10;
	}

	//limit BAT volt
	lq10Temp.lData = __IQsat(lq10Temp.lData,g_i32CurrDisMax,IDC_0A);

	if(lq10Temp.lData < IDC_0A2)
	{
		g_lq10BatCurr.lData = IDC_0A;
	}
	else
	{
		g_lq10BatCurr.lData = lq10Temp.lData;
	}
}

void vDcdcCurrCal(void)
{
	ubitfloat	lq10Temp;
	lq10Temp.lData =  _IQ10mpy(LQ20_DEF_INVIDCFACTOR, (INT32)IsrVars._lIdcDisUse.iData.iHD);
	g_lq10MdlCurrNoFilter.lData =
						 _IQ10mpy(lq10Temp.lData,g_lq10CurrSampSysa.lData) 
	                    + g_lq10CurrSampSysb.lData;
}

void vDcdcCurrFilter(void)
{
	ubitfloat	lq10Temp;

	s_ai32AdcDataDcCurr[30] = g_lq10MdlCurrNoFilter.lData; 
	s_i32AdcSumDcCurr += s_ai32AdcDataDcCurr[30] 
	                     - s_ai32AdcDataDcCurr[s_u16AdcPoint];
	s_ai32AdcDataDcCurr[s_u16AdcPoint] = s_ai32AdcDataDcCurr[30];

	//ave
	lq10Temp.lData = _IQ20mpy(INV30_IQ20, s_i32AdcSumDcCurr);
		
	//filter with Volt*0.95+Tmp*0.05	
	if(labs(lq10Temp.lData - g_lq10MdlCurr.lData) < IDC_0A5)
	{
		lq10Temp.lData = (g_lq10MdlCurr.lData * LQ10_ADC_DCCURR_FILTA 
		                  + lq10Temp.lData * LQ10_ADC_DCCURR_FILTB) >> 10;
	}
	
	//limit display
	g_lq10MdlCurr.lData = __IQsat(lq10Temp.lData,g_i32CurrDisMax,IDC_0A);

	if((g_u16MdlStatus.bit.OFFSTAT) || (g_lq10MdlCurr.lData < IDC_0A2))

	{
		g_lq10MdlCurr.lData = IDC_0A;
	}

	if(MODE_REC == CputoClaVar.u16WorkMode)
	{
		g_lq10MdlCurr.lData = g_lq10BatCurr.lData;
	}
	
   g_lq10MdlCurrDisp.lData = g_lq10MdlCurr.lData;
}


void vU1TemperatureCal(void)
{
	g_lq10TempAmbi.lData =  __mpyxu(IsrVars._lU1Temp.iData.iHD, 80) - TEMP_50C;
}

void vU1TemperatureFilter(void)
{
	ubitfloat	lq10Temp;
		
	s_ai32AdcDataTempU[30] = g_lq10TempAmbi.lData;
	s_i32AdcSumTempU += s_ai32AdcDataTempU[30] 
	                    - s_ai32AdcDataTempU[s_u16AdcPoint];
	s_ai32AdcDataTempU[s_u16AdcPoint] = s_ai32AdcDataTempU[30];

	//ave/
	lq10Temp.lData = _IQ20mpy(INV30_IQ20, s_i32AdcSumTempU);

	//filter with Volt*0.95+Tmp*0.05	
	lq10Temp.lData = (g_lq10TempAmbiDisp.lData * LQ10_ADC_TEMP_FILTA 
	                  + lq10Temp.lData * LQ10_ADC_TEMP_FILTB) >> 10;
	
	//limit
    g_lq10TempAmbiDisp.lData = \
    __IQsat(lq10Temp.lData,TEMP_125C,N_TEMP_40C);
}



float NTCTempCal(float TempADFilter)
{
	float TempNTC1=0,TempNTC2=0,TempNTC3=0,TempNTC4=0,TempNTC5=0;

	TempNTC1 = TempADFilter*3.2f/4096.0f;
	TempNTC2 = 	TempNTC1*TempNTC1;
	TempNTC3 = 	TempNTC1*TempNTC2;
	TempNTC4 = 	TempNTC1*TempNTC3;
	TempNTC5 = 	TempNTC1*TempNTC4;

	return (9.8945f*TempNTC5 - 76.115f*TempNTC4 + 216.82f*TempNTC3 - 279.39f*TempNTC2 + 188.39f*TempNTC1 - 54.541f);
}




void vDCTemperatureCal(void)
{
     g_lq10TempDC.lData = NTCTempCal(IsrVars._lDCTemp.iData.iHD)*1024.0f;
}


void vDCTemperatureFilter(void)
{
	ubitfloat	lq10Temp;
		
	s_ai32AdcDataTempDC[15] = g_lq10TempDC.lData;
	s_i32AdcSumTempDC += s_ai32AdcDataTempDC[15] 
	                    - s_ai32AdcDataTempDC[s_u16AdcPoint2];
	s_ai32AdcDataTempDC[s_u16AdcPoint2] = s_ai32AdcDataTempDC[15];

	//cal ave
	lq10Temp.lData = _IQ20mpy(INV15_IQ20, s_i32AdcSumTempDC);
	
	//filter with Volt*0.95+Tmp*0.05	
	lq10Temp.lData = (g_lq10TempDCDisp.lData * LQ10_ADC_TEMP_FILTA 
	                  + lq10Temp.lData * LQ10_ADC_TEMP_FILTB) >> 10;


	g_lq10TempDCDisp.lData = __IQsat(lq10Temp.lData,((INT32)125<<10),(INT32)10<<10);
}



void vPFCTemperatureCal(void)
{
	g_lq10TempPFC.lData = NTCTempCal(IsrVars._lPFCTemp.iData.iHD)*1024.0f;
}

void vPFCTemperatureFilter(void)
{
	ubitfloat	lq10Temp;
		
	s_ai32AdcDataTempPFC[15] = g_lq10TempPFC.lData;
	s_i32AdcSumTempPFC += s_ai32AdcDataTempPFC[15] 
	                    - s_ai32AdcDataTempPFC[s_u16AdcPoint2];
	s_ai32AdcDataTempPFC[s_u16AdcPoint2] = s_ai32AdcDataTempPFC[15];

	//cal PFC temperture ave/
	lq10Temp.lData = _IQ20mpy(INV15_IQ20, s_i32AdcSumTempPFC);
	
	//filter with Volt*0.95+Tmp*0.05	
	lq10Temp.lData = (g_lq10TempPFCDisp.lData * LQ10_ADC_TEMP_FILTA 
	                  + lq10Temp.lData * LQ10_ADC_TEMP_FILTB) >> 10;

	//limit the temp value

	g_lq10TempPFCDisp.lData = __IQsat(lq10Temp.lData,((INT32)125<<10),(INT32)10<<10);
}
//===========================================================================
// No more.
//===========================================================================
