/******************************************************************************
 *
 *		VERSION:V1.0
 *      Author:Tesla
 *
 ******************************************************************************/
#include <Shared.h>

struct	EPLL	ePLL;

// Core timer 2 interrupt (each 20 us)
// NOTE: not a good solution to have such a long ISR

#pragma CODE_SECTION(dcdc_ISR, "IsrRamfuncs");
interrupt void dcdc_ISR(void)       // dcdc_ISR -
{
	CpuTimer2Regs.TCR.bit.TIF = 1;

	switch (IsrVars._u16IsrTimer++ & 0x7u)
	{   // Each "case" is handled every 8th interrupt = once in 160 us
		case 0x00:
			if ( (1 == CputoClaVar.u16PfcOpened) && (ADC_PFCOVP <= 2149)    // execute if PFC is opened, overvoltage on PFC, MODE_INV does match
					&& (MODE_INV != g_u16MdlCtrl.bit.Rec_Inv)               //  Rec_Inv, AND the phase is locked
					&& (0 == g_u16MdlStatus.bit.PhaseUnlock)
					)
			{
				mOffDcdcPwm();      // Turn off DCDC PWM, turning off DCDC Conv
				mOffPfcPwm();       // Turn off PFC PWM, disabling all power correction and output on OBC
				g_u16ActionReady = POWON_STATE;     // POWON_STATE likely refers to the period where the charger is on, but not charging

				CputoClaVar._uiLowPfcVolt = 1;
				mMainRelayOpen();   // Disconnect main contactor (relay)
			}
		break;
		case 0x01:
			IsrVars._lDCTemp.lData += __mpyxu(((int16)164) , IsrVars._u16ADDCTemp + ADC_TEMPSR)
			+ __mpy( IsrVars._lDCTemp.iData.iHD,((int16)-328));
			IsrVars._u16ADDCTemp = ADC_TEMPSR;
		break;
		case 0x02:
			IsrVars._lPFCTemp.lData += __mpyxu(((int16)164) , IsrVars._u16ADPFCTemp + ADC_TEMPPFC)
			+ __mpy( IsrVars._lPFCTemp.iData.iHD,((int16)-328));
			IsrVars._u16ADPFCTemp = ADC_TEMPPFC;
		break;
		case 0x03:
			IsrVars._i16AdAcMax = __max(IsrVars._i16AdAcMax, abs(ePLL.i16Vac_ADC));     //ADC_VACIN
		break;
		case 0x04:
			IsrVars.l32IdcBatDisUse.lData += __mpy(IsrVars.i16AdIdcBat + ADC_BATCURR_DC , ((int16)164))
			+ __mpy(IsrVars.l32IdcBatDisUse.iData.iHD, ((int16)-328));
			IsrVars.i16AdIdcBat = ADC_BATCURR_DC;
		break;
		case 0x05:
			IsrVars._lU1Temp.lData += __mpyxu(((int16)164), IsrVars._u16ADU1Temp + ADC_TEMPAIR)
			+ __mpy( IsrVars._lU1Temp.iData.iHD,((int16)-328));
			IsrVars._u16ADU1Temp = ADC_TEMPAIR;

		break;
		case 0x06: // AC OVP + PFC UVP
			if ( (abs(ePLL.i16Vac_ADC) > VAC_AD_300VPK) && (__max(ADC_PFCVSAMP, ADC_PFCOVP) > 3600) )
			{
				CputoClaVar._uiOverAcVolt = 1;      // shut off PWM and stop charging if we get over voltage on AC and under voltage on PFC
				mOffDcdcPwm();
				mOffPfcPwm();
				__asm("  NOP");
				g_u16ActionReady = POWON_STATE;
				mMainRelayOpen();   // Disconnect main contactor (relay)
			}
		break;
		case 0x07:
			IsrVars._u16160usTimer++;
			IsrVars._u16MainRly_160usTimer++;

			IsrVars._lIdcDisUse.lData += __mpy(IsrVars._i16AdIdc + IsrVars._i16AdIdcDis , ((int16)164))
			+ __mpy(IsrVars._lIdcDisUse.iData.iHD, ((int16)-328));
			IsrVars._i16AdIdcDis = IsrVars._i16AdIdc; //sampled below
		break;
		default:
		break;
	}
//---------always run------------------
{
	ePLL.i16Vac_ADC = ((int16)ADC_VACIN - 2100);
	ePLL.f32Verr =  ((float32)ePLL.i16Vac_ADC)/4.242f - ePLL.f32Vamp_Est * ePLL.f32Sin_Theta;
	ePLL.f32Vamp_Est += 0.00001f * 44.0f * ePLL.f32Verr * ePLL.f32Sin_Theta;
	ePLL.f32deltaW_Est = __fsat(0.00001f * 41.0f * ePLL.f32Verr * ePLL.f32Cos_Theta
		+ ePLL.f32deltaW_Est, 10.0f*PI_2_VALUE, -10.0f*PI_2_VALUE);
	ePLL.f32Theta += 0.00001f * ( 55.0f*PI_2_VALUE + ePLL.f32deltaW_Est + \
		0.74f * ePLL.f32Verr * ePLL.f32Cos_Theta );

	ePLL.f32Theta_Active = ePLL.f32Theta + 0.030f;

	if(ePLL.f32Theta >= PI_2_VALUE)
	{
		ePLL.f32Theta -= PI_2_VALUE;
	}
	else if((ePLL.f32Theta < 0.0f))
	{
		ePLL.f32Theta += PI_2_VALUE;
	}

	if(ePLL.f32Theta_Active >= PI_2_VALUE)
	{
		ePLL.f32Theta_Active -= PI_2_VALUE;
	}
//----------------------
	ePLL.f32Theta_pu = __div2pif32(ePLL.f32Theta);
	ePLL.f32Theta_Act_pu = __div2pif32(ePLL.f32Theta_Active);

	ePLL.f32Cos_Theta = __cospuf32(ePLL.f32Theta_pu);
	ePLL.f32Sin_Theta = __sinpuf32(ePLL.f32Theta_pu);

	ePLL.i16Vac_Est = __f32toi16r(ePLL.f32Vamp_Est * ePLL.f32Sin_Theta);


	if((fabs(ePLL.f32Verr)> 60.0f) || (g_lq10AcVolt.lData <= VAC_80V))
	{
		ePLL.u16ErrCnt++;
		if((ePLL.u16ErrCnt >= 100))
		{
			ePLL.u16ErrCnt = 100;
			if(mMainRelayCloseCheck() && (MODE_REC == CputoClaVar.u16WorkMode))     // If main relay is closed & workmode matches MODE_REC, proceed
			{
				mOffPfcPwm();
				g_u16MdlStatus.bit.PhaseUnlock = 1;     // Unlock Phase, I think this has to do with input of AC/DC
			}
		}
	}
	else if(ePLL.u16ErrCnt >= 1)
	{
			ePLL.u16ErrCnt--;
	}

	if(IsrVars.u16Latch)
	{
		IsrVars.i16DI_Vac = DI_VAC_PLORITY;//pin sample.
	}
//--------------
	if((abs(ePLL.i16Vac_ADC) < 220) || (IsrVars.u16FreqCnt <= 10) || (IsrVars.u16FreqCnt >= 990) || (g_u16ActionReady <= PFC_SWSOFT_START))// || (fabs(CputoClaVar.f32Sin_Theta) < 0.035)
	{
		CputoClaVar.i16Polarity = (cla_16bits_s)0;
	}
	else
	{
		CputoClaVar.i16Polarity = (1 == IsrVars.i16DI_Vac)?POSITIVE_HALF:NEGATIVE_HALF;
	}
	//-----------------------------

	if(IsrVars.i16DI_VacLast != IsrVars.i16DI_Vac)
	{
		if((IsrVars.u16FreqCnt > 790 ) && (IsrVars.u16FreqCnt <1200))//同一状态持续时间大于5ms=5000/10
		{
			IsrVars.u16FreqOut = IsrVars.u16FreqCnt;
		}
		IsrVars.u16Latch = 0;
		IsrVars.u16FreqCnt = 0;

		CputoClaVar.i16Polarity = (cla_16bits_s)0;
	}
	else
	{
		IsrVars.u16FreqCnt = __min(IsrVars.u16FreqCnt + 1, 1500);
	}

	if(IsrVars.u16FreqCnt > 300)
	{
		IsrVars.u16Latch = 1;
	}

	IsrVars.i16DI_VacLast = IsrVars.i16DI_Vac;
}
{
	CputoClaVar.u32Theta += 2147484ul;
	if(MODE_INV == CputoClaVar.u16WorkMode)
	{
		CputoClaVar.f32Sin_Theta = __sinpuf32(1.0f/65536.0f * (float32)(CputoClaVar.u32Theta >> 16));//MPYF32       R0H, #0x3780, R0H
	}
	else
	{
		CputoClaVar.f32Sin_Theta = __sinpuf32(ePLL.f32Theta_Act_pu);
	}
}

	//------------------------------------------------------------------------------
	IsrVars._i16AdVdcSam0 = ADC_VDC<<3;
	IsrVars.i16AdVpfc0 = _IQ12mpy(ADC_PFCOVP_B, 4282);



	IsrVars.f32Vin = ((float32)ADC_PFCOVP_B)*1.0455f/8.076f;
	IsrVars.f32Vo = ((float32)ADC_VDC)/5.114f;


	if(MODE_INV == CputoClaVar.u16WorkMode)
	{
		IsrVars.f32Vin_Vo = IsrVars.f32Vo * 1.0714286f - IsrVars.f32Vin;//15:14

		if(IsrVars.i16AdVpfc0 <= 500)
		{
			IsrVars.l32IdcdcSys = 0;

			IsrVars._i16CurrLimFloor = __min(
			(int16) (__IQmpy( 200, IsrVars.i16AdVpfc0,13) +  150) ,
						0x900 );
		}
	}
	else
	{
		IsrVars.f32Vin_Vo = IsrVars.f32Vin * 0.9333333f - IsrVars.f32Vo;//15:14
		if( ((IsrVars._i16AdVdcSam0 <= 3728) && (1 == hiDCShort))
				|| (g_u16MdlStatus.bit.PhaseUnlock)
			)
		{
			IsrVars.l32IdcdcSys = 0;
			IsrVars._i16CurrLimFloor = __min(
			(int16) (__IQmpy( 200, IsrVars._lVdcUse.lData,26) +  150) ,
						0x900 );
		}
	}

		if ( (1 == hiHvsd) && (IsrVars._i16AdVdcSam0 <= 3262) )
		{
			g_u16ActionReady = POWON_STATE;
			mOffDcdcPwm();
		}

//--------------feedback
		IsrVars._lVpfcUse.lData +=
				( (int32) IsrVars.i16AdVpfc0 + (int32)IsrVars.i16AdVpfc1 ) * (int32)IsrVars._i16VpfcFilterK2
						+ _IQ16mpy( IsrVars._lVpfcUse.lData, IsrVars._lVpfcFilterK1.lData);
		IsrVars.i16AdVpfc1 = IsrVars.i16AdVpfc0;

//--------------feedback
		IsrVars._lVdcUse.lData = (int32)IsrVars._i16AdVdcSam0 << 16; //用于取消对消滤波
		IsrVars._i16AdVdcSam1 = IsrVars._i16AdVdcSam0;
//-----------------------
		IsrVars.LQ16CurrLim = __lmax(
				(int32) IsrVars._i16CurrLimFloor << 16,
				IsrVars.l32IdcdcSys*(int32)IsrVars._lVdcDisUse.iData.iHD);
		if(MODE_INV == CputoClaVar.u16WorkMode)
		{
	        IsrVars._i16VdcLoopK3 = IsrVars._i16INV_VdcLoopK3;

	        if(g_lq10MdlVolt.lData <= VDC_330V)                      //INV mode DCDC Volt loop PI self-adjusting
	        {
	            IsrVars._i16VdcLoopK4 = IsrVars._i16INV_VdcLoopK4;
	        }
	        else if(g_lq10MdlVolt.lData >= VDC_450V)
	        {
	            IsrVars._i16VdcLoopK4 = IsrVars._i16INV_VdcLoopK4+26;
	        }
	        else
	        {
	            IsrVars._i16VdcLoopK4 = IsrVars._i16INV_VdcLoopK4 + 26*(g_lq10MdlVolt.lData-VDC_330V)/(VDC_450V-VDC_330V);
	        }

	        if(abs(IsrVars._i16VdcLoopK4) >= IsrVars._i16VdcLoopK3-1)
	        {
	            IsrVars._i16VdcLoopK4 = -(IsrVars._i16VdcLoopK3-1);
	        }

			IsrVars._i16VdcErr0 =
						(CputoClaVar._iVpfcSet - IsrVars.i16AdVpfc0);
		}
		else
		{
	        IsrVars._i16VdcLoopK3 = IsrVars._i16CHG_VdcLoopK3;
	        IsrVars._i16VdcLoopK4 = IsrVars._i16CHG_VdcLoopK4;

			IsrVars._i16VdcErr0 = (IsrVars._i16VdcSet - IsrVars._lVdcUse.iData.iHD)>>3;//适用于350V以下电压输出
		}

		IsrVars.LQ10VdcPiOut = __IQsat(
                IsrVars.LQ10VdcPiOut + IsrVars._i16VdcErr0 *(long) IsrVars._i16VdcLoopK3 +
                IsrVars._i16VdcErr1 * (long)IsrVars._i16VdcLoopK4,
		(int32) IsrVars._i16PowerMaxExtra << 10, (int32)(-30) << 10);
		IsrVars._i16VdcErr1 = IsrVars._i16VdcErr0;

		IsrVars._lVdcDisUse.lData += __mpy(IsrVars._lVdcUse.iData.iHD, ((int16)23))
				+ __mpy(IsrVars._i16AdVdcDis, ((int16)23))
				+	_IQ16mpy(IsrVars._lVdcDisUse.lData, (int32)(-46));
		IsrVars._i16AdVdcDis = IsrVars._lVdcUse.iData.iHD;

		CputoClaVar.f32AdVdcDis = (float32)IsrVars._i16AdVdcDis;

		IsrVars._u16ChoiseCon = 0;
		IsrVars._reserveF4.lData = IsrVars.LQ10VdcPiOut; //用于保存最终结果

		IsrVars._reserveF2.lData = ( (int32) IsrVars._i16PowerLimMax<< 10 ); //IQ16 to IQ10
		if (IsrVars._reserveF2.lData < IsrVars._reserveF4.lData)
		{
			IsrVars._u16ChoiseCon = 1;
			IsrVars._reserveF4.lData = IsrVars._reserveF2.lData;
		}

		IsrVars._reserveF2.lData =  IsrVars.LQ16CurrLim >>6;

		if (IsrVars._reserveF2.lData < IsrVars._reserveF4.lData)
		{
			IsrVars._u16ChoiseCon = 2;
			IsrVars._reserveF4.lData = IsrVars._reserveF2.lData;
		}

		IsrVars._i16DCDCPowerSet = (int16) (IsrVars._reserveF4.lData >> 10);

		if(MODE_INV == CputoClaVar.u16WorkMode)
		{
			IsrVars._i16AdIdc = __max(1990 - ADC_DCCUR_B, 0);

			IsrVars._i16DCDCPowerFeedback =	(int16)((int32)IsrVars._i16AdIdc * (int32)IsrVars.i16AdVpfc0 >> 10);


			IsrVars._i16PowerErr0 = IsrVars._i16DCDCPowerSet - IsrVars._i16DCDCPowerFeedback;
			IsrVars._i16PowerErrUse0 =
					(int16) (IsrVars._i16PowerErrUse0*(long)15679 +
							 IsrVars._i16PowerErr1*(long)(-9786) +
							 IsrVars._i16PowerErr0*(long)11674
							  >> 14);
		}
		else
		{
			IsrVars._i16AdIdc = __max(ADC_DCCUR_B - 1990, 0);

			IsrVars._i16DCDCPowerFeedback =	(int16)(((int32)ADC_BATCURR_DC * (int32)(IsrVars._lVdcDisUse.iData.iHD)) >> 13);//电压CNT*电流CNT/2^10

			IsrVars._i16PowerErr0 = IsrVars._i16DCDCPowerSet - IsrVars._i16DCDCPowerFeedback;
			IsrVars._i16PowerErrUse0 =
					(int16) (IsrVars._i16PowerErrUse0*(long)15483 +
							 IsrVars._i16PowerErr1*(long)(-15032) +
							 IsrVars._i16PowerErr0*(long)16835
							>> 14);
		}


		IsrVars._i16PowerErr1 = IsrVars._i16PowerErr0;
		// PI Controller
		IsrVars._lIdcPiout.lData = __IQsat(
				IsrVars._lIdcPiout.lData +
				(( IsrVars._i16PowerErrUse1*(long)-5595 +
				   IsrVars._i16PowerErrUse0*(long)6548 ) >> 4),
				(long)IsrVars._i16IdcPioutPermit<<16 , 0L);
		IsrVars._i16PowerErrUse1 = IsrVars._i16PowerErrUse0;

		IsrVars._reserveF4.lData = __IQsat(	IsrVars._lIdcPiout.lData,
											(long)IsrVars._i16DcdcPWMTsMax << 16,
											(long)IsrVars._i16DcdcPWMTsMin<<16);

		IsrVars._lDcdcPWMDutyMax.lData = IsrVars._reserveF4.lData >> 1;

		IsrVars._reserveF2.lData = __lmin(IsrVars._lIdcPiout.lData, IsrVars._reserveF4.lData);
		IsrVars._reserveF2.lData = __lmin(IsrVars._lIdcPiout.lData>>1, IsrVars._lDcdcPWMDutyMax.lData);

		IsrVars._u16DcdcPWMChgFlag = 0;
		if (IsrVars._reserveF2.lData <= (20L<<16))
		{
			IsrVars._reserveF2.lData = (20L<<16);
			IsrVars._u16DcdcPWMChgFlag = 1;
		}

		IsrVars._lDcdcPWMTsShadow.lData = IsrVars._reserveF4.lData;//保护有使用，不能随便更改
		IsrVars._lDcdcPWMTs.lData = (IsrVars._lDcdcPWMTsShadow.lData>>1) & 0xFFFFFF00;
		IsrVars._lDcdcDuty.lData = (IsrVars._reserveF2.lData>>1) & 0xFFFFFF00;//cmp mathes duty
		IsrVars._lDcdcDutyShadow.lData = (IsrVars._lDcdcPWMTs.lData - IsrVars._lDcdcDuty.lData) & 0xFFFFFF00;//matched cmp

		*((Uint32 *)(&EPwm1Regs.TBPRDHR)) = IsrVars._lDcdcPWMTs.lData;//PWMLINK used for EPWM2~8. Write shadow


if(MODE_INV != CputoClaVar.u16WorkMode)// Rectifier mode
	{
		if (IsrVars._u16DcdcPWMChgFlag == 1)
		{
			EPwm1Regs.CMPB.all = EPwm1Regs.CMPA.all = EPwm4Regs.CMPB.all = EPwm4Regs.CMPA.all = IsrVars._lDcdcDutyShadow.lData;
			EPwm2Regs.CMPB.all = EPwm2Regs.CMPA.all = EPwm3Regs.CMPB.all = EPwm3Regs.CMPA.all = IsrVars._lDcdcDuty.lData;
		}
		else
		{
			EPwm2Regs.CMPB.all = EPwm1Regs.CMPA.all = EPwm4Regs.CMPB.all = EPwm3Regs.CMPA.all = IsrVars._lDcdcDutyShadow.lData;
			EPwm1Regs.CMPB.all = EPwm2Regs.CMPA.all = EPwm3Regs.CMPB.all = EPwm4Regs.CMPA.all = IsrVars._lDcdcDuty.lData;
		}


		if(IsrVars._lDcdcDuty.lData >= ((int32)(DC_DB_TIME_SR + 6)<<16))
		{
			IsrVars.f32SRDelay = __fsat(
					-0.6128f*IsrVars.f32Vin_Vo - 0.3377f*IsrVars.f32Vin + 1650.0f\
					+ 0.4805f*((float32)ADC_BATCURR_DC) \
					- 3.06f*((float32)IsrVars._lDcdcPWMTsShadow.iData.iHD), \
					700.0f, 350.0f);//408.0f- 46.99f
			IsrVars.i16SROnDelay = __f32toi16r(IsrVars.f32SRDelay*0.1f);//ns to DB
			IsrVars.f32SRDelay = (float32)(__min(IsrVars.u16SROnDelay,IsrVars.i16SROnDelay)*10);//
			if(IsrVars._lDcdcDutyShadow.iData.iHD == IsrVars._lDcdcDuty.iData.iHD)
			{
				//off-delay use cmpa
				IsrVars.f32SRDelay = __fsat(
						0.5749f*IsrVars.f32Vin_Vo - 0.7125f*IsrVars.f32Vin  + 1200.0f\
						+ 0.3687f*((float32)ADC_BATCURR_DC) \
							- 2.818f*((float32)IsrVars._lDcdcPWMTsShadow.iData.iHD), \
						IsrVars.f32SRDelay-400.0f, -250.0f);
				if(ADC_BATCURR_DC < 300)
				{
					IsrVars.f32SRDelay = __fmin(IsrVars.f32SRDelay, (float32)ADC_BATCURR_DC - 300.0f);
				}
				else
				{
					IsrVars.f32SRDelay = __fmin(IsrVars.f32SRDelay, 200.0f);//限制调频态后移的最大时间
				}

				if((IsrVars._lDcdcPWMTsShadow.iData.iHD > 500) && (IsrVars.f32Vin_Vo <= 5.0f))//200K
				{
					IsrVars.f32SRDelay = __fmin(IsrVars.f32SRDelay, -250.0f);//-150ns min
				}
			}
			else
			{
				IsrVars.f32SRDelay = __fsat(
						2.932f*IsrVars.f32Vin_Vo + 0.7252f*((float32)ADC_BATCURR_DC) - 300.0f, \
						(float32)(IsrVars._lDcdcDutyShadow.iData.iHD - IsrVars._lDcdcDuty.iData.iHD + IsrVars.i16SROnDelay - 40 ) *10.0f, \
						-350.0f);//
			}
			IsrVars.i16SROffDelay = __f32toi16r(IsrVars.f32SRDelay*0.1f);//10ns per cnt


			if(IsrVars.i16SROnDelay >= IsrVars.u16SROnDelay)
			{
				EPwm5Regs.DBRED.all =  EPwm6Regs.DBRED.all = \
				EPwm7Regs.DBRED.all = EPwm8Regs.DBRED.all = \
				IsrVars.u16SROnDelay = IsrVars.i16SROnDelay;
			}
			else
			{
				EPwm5Regs.DBRED.all =  EPwm6Regs.DBRED.all = \
				EPwm7Regs.DBRED.all = EPwm8Regs.DBRED.all = \
				IsrVars.u16SROnDelay = __max(__min(IsrVars.u16SROnDelay - 1 ,100), DC_DB_TIME_SR);
			}

			if(((long)IsrVars.i16SROffDelay << 16) <= IsrVars.l32SR_DC_FED_DELAY)//
			{
				IsrVars.l32SR_DC_FED_DELAY = __lmin(((long)IsrVars.i16SROffDelay) << 16, IsrVars._lDcdcDutyShadow.lData - (7L<<16));
			}
			else
			{
				IsrVars.l32SR_DC_FED_DELAY = __lmin(IsrVars.l32SR_DC_FED_DELAY + (1L<<16),
						IsrVars._lDcdcDutyShadow.lData - (7L<<16));
			}
			IsrVars._reserveF2.lData = EPwm6Regs.CMPA.all = __IQsat(
			((int32)DC_TBPRD_200K<<16) - IsrVars._lDcdcDuty.lData + IsrVars.l32SR_DC_FED_DELAY, //与Duty反比
			IsrVars._lDcdcDuty.lData + IsrVars.l32SR_DC_FED_DELAY,//SR同步调宽,与Duty正比
			7L<<16);

			EPwm5Regs.CMPA.all = IsrVars._lDcdcPWMTs.lData - IsrVars._reserveF2.lData;
		}
		else
		{
			EPwm5Regs.CMPA.all = (IsrVars._lDcdcPWMTs.lData - (6L<<16))&0xFFFF0000;//CAD clear
			EPwm6Regs.CMPA.all = 6L<<16;//CAU clear
		}
	}
else //Inverter mode
	{
		if (IsrVars._u16DcdcPWMChgFlag == 1)
		{
			EPwm5Regs.CMPB.all = EPwm5Regs.CMPA.all = EPwm8Regs.CMPB.all = EPwm8Regs.CMPA.all = IsrVars._lDcdcDutyShadow.lData;
			EPwm6Regs.CMPB.all = EPwm6Regs.CMPA.all = EPwm7Regs.CMPB.all = EPwm7Regs.CMPA.all = IsrVars._lDcdcDuty.lData;
		}
		else
		{
			EPwm6Regs.CMPB.all = EPwm5Regs.CMPA.all = EPwm8Regs.CMPB.all = EPwm7Regs.CMPA.all = IsrVars._lDcdcDutyShadow.lData;
			EPwm5Regs.CMPB.all = EPwm6Regs.CMPA.all = EPwm7Regs.CMPB.all = EPwm8Regs.CMPA.all = IsrVars._lDcdcDuty.lData;
		}


		if(IsrVars._lDcdcDuty.lData >= ((int32)(SR_DC_FED_AHEAD + 6)<<16))
		{
			IsrVars.l32SR_DC_FED_AHEAD = __IQsat(
					IsrVars._lDcdcDuty.lData - (100L<<16), 15L<<16, 3L<<16);

			IsrVars.l32SR_DC_FED_AHEAD +=
					(long)(__max(__min(300 - IsrVars._i16AdIdc,100), 0))<<13;

			IsrVars._reserveF2.lData = EPwm2Regs.CMPA.all = __IQsat(
					((int32)DC_TBPRD_200K<<16) - IsrVars._lDcdcDuty.lData - IsrVars.l32SR_DC_FED_AHEAD, //与Duty反比
					IsrVars._lDcdcDuty.lData - IsrVars.l32SR_DC_FED_AHEAD,
					7L<<16);

			IsrVars.f32SRDelay = __fsat(
					5.79f*IsrVars.f32Vin_Vo - 0.305f*IsrVars.f32Vin - 150.0f\
					- 0.1041f*((float32)IsrVars._i16AdIdc) \
					+ 0.5f*((float32)IsrVars._lDcdcPWMTsShadow.iData.iHD), \
					700.0f, 370.0f);
			IsrVars.i16SROnDelay = __f32toi16r(IsrVars.f32SRDelay*0.1f);
			if(IsrVars.i16SROnDelay >= IsrVars.u16SROnDelay)
			{
				EPwm1Regs.DBRED.all =  EPwm2Regs.DBRED.all = \
				EPwm3Regs.DBRED.all = EPwm4Regs.DBRED.all = \
				IsrVars.u16SROnDelay = IsrVars.i16SROnDelay;
			}
			else
			{
				EPwm1Regs.DBRED.all =  EPwm2Regs.DBRED.all = \
				EPwm3Regs.DBRED.all = EPwm4Regs.DBRED.all = \
				IsrVars.u16SROnDelay = __max(__min(IsrVars.u16SROnDelay - 1 ,100), DC_DB_TIME_SR);
			}
			EPwm1Regs.CMPA.all = IsrVars._lDcdcPWMTs.lData - IsrVars._reserveF2.lData;
		}
		else
		{
			EPwm1Regs.CMPA.all = (IsrVars._lDcdcPWMTs.lData - (6L<<16))&0xFFFF0000;//CAD clear
			EPwm2Regs.CMPA.all = 6L<<16;
		}
	}
		EALLOW;
		EPwm1Regs.GLDCTL2.bit.OSHTLD = 1;
		EDIS;
}

__attribute__((ramfunc))
//
// epwm1_tzint_isr - EPWM1 TZ ISR
//
__interrupt void epwm1_tzint_isr(void)
{

	mOffDcdcPwm();
	mOffPfcPwm();
	PieCtrlRegs.PIEIER2.all &= 0xFFFE;
   //
    // Acknowledge this interrupt to receive more interrupts from group 2
    //
    PieCtrlRegs.PIEACK.all = PIEACK_GROUP2;
}

