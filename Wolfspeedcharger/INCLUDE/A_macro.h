/*
 * GridSide.h
 *
 *  Created on: 2018Äê4ÔÂ11ÈÕ
 */

#ifndef INCLUDE_A_MACRO_H_
#define INCLUDE_A_MACRO_H_

#ifdef __cplusplus
extern "C" {
#endif

#define POW_RATE		(((INT32)6600) << 10)
#define CURR_RATE       ((INT32) (POW_RATE/450))
#define MAX_CURR        ((INT32) (POW_RATE/320))

//-----------dependent constant setup--------------------
// Timer definitions based on System Clock
// 200 MHz devices
#define SYSTEM_FREQUENCY	(DEVICE_SYSCLK_FREQ/1000000) //1us  ----device.h

#define		mSec0_5		0.5*SYSTEM_FREQUENCY*1000		// 0.5 mS
#define		mSec1		1*SYSTEM_FREQUENCY*1000		    // 1.0 mS
#define		mSec2		2.0*SYSTEM_FREQUENCY*1000		// 2.0 mS
#define		mSec5		5*SYSTEM_FREQUENCY*1000		    // 5.0 mS
#define		mSec7_5		7.5*SYSTEM_FREQUENCY*1000		// 7.5 mS
#define		mSec10		10*SYSTEM_FREQUENCY*1000		// 10 mS
#define		mSec20		20*SYSTEM_FREQUENCY*1000		// 20 mS
#define		mSec50		50*SYSTEM_FREQUENCY*1000		// 50 mS
#define		mSec100		100*SYSTEM_FREQUENCY*1000		// 100 mS
#define		mSec500		500*SYSTEM_FREQUENCY*1000	    // 500 mS
#define		mSec1000	1000*SYSTEM_FREQUENCY*1000	    // 1000 mS

//
//-----------Register MAPPING£ºADC--------------------
#define VPFCOVP		    0x02
#define SRTEMP		    0x03
#define AIRTEMP  	    0x04
#define PFCTEMP  	    0x05

#define PFCCURR	     	0x0E
//-------------------------
#define VPFCCTRL		0x2
#define VAC		        0x4
//----------------------
#define DCVOLT		    0x00
#define DCCURR		    0x02
#define VPFCOVP_B		0x03
//----------------------
#define BATCURR_DC		    0x0
#define BATCURR_SINE	    0x2

//AdcA--->PFC
#define     ADC_TEMPPFC				((short)AdcaResultRegs.ADCRESULT0)
#define     ADC_PFCCUR				((short)AdcaResultRegs.ADCRESULT1)
#define     ADC_PFCOVP				((short)AdcaResultRegs.ADCRESULT2)
#define     ADC_TEMPSR				((short)AdcaResultRegs.ADCRESULT3)
#define     ADC_TEMPAIR				((short)AdcaResultRegs.ADCRESULT4)
//AdcC--->PFC
#define     ADC_VACIN				((short)AdccResultRegs.ADCRESULT0)
#define     ADC_PFCVSAMP			((short)AdccResultRegs.ADCRESULT1)
#define     ADC_PFCCUR_C			((short)AdccResultRegs.ADCRESULT2)
//AdcB--->DC
#define     ADC_VDC					((short)AdcbResultRegs.ADCRESULT0)
#define     ADC_DCCUR_B				((short)AdcbResultRegs.ADCRESULT1)
#define     ADC_PFCOVP_B			((short)AdcbResultRegs.ADCRESULT2)
//AdcD--->DC
#define     ADC_BATCURR_DC			((short)AdcdResultRegs.ADCRESULT0)
#define     ADC_BATCURR_SINE		((short)AdcdResultRegs.ADCRESULT1)

//-----------Register MAPPING£ºPassword--------------------

//-----------Register MAPPING£ºTimer--------------------
#define	mGet5msTiming()		CpuTimer0Regs.TCR.bit.TIF
#define	mClr5msTimFlag()		(CpuTimer0Regs.TCR.bit.TIF = 1)


//-----------Register MAPPING£ºISR--------------------
#define mStartPFCIsrCal()		(EPwm9Regs.ETSEL.bit.INTEN = 1)

#define	DI_VAC_PLORITY		(GpioDataRegs.GPADAT.bit.GPIO27)
#define POSITIVE_HALF		(cla_16bits_s)1
#define NEGATIVE_HALF		(cla_16bits_s)-1

//-----------Register MAPPING£ºPower Stage Related--------------------
#define PFC_PWM_SWITCHING_FREQUENCY ((float)100*1000)
#define PFC_PWM_PERIOD (PWMSYSCLOCK_FREQ)/(PFC_PWM_SWITCHING_FREQUENCY)

#define LOW_FREQ_PWM_BASE                 12
#define HIGH_FREQ_1H_BASE		9
#define HIGH_FREQ_1L_BASE		10
#define SET_PWM_DBRED(m,n)					EPwm##m##Regs.DBRED.all = n
#define SET_PWM_DBFED(m,n)					EPwm##m##Regs.DBFED.all = n
#define AQ_SW_FORCE_PWMxA_HIGH_PWMxB_LOW(m)	EPwm##m##Regs.AQCSFRC.all = 0x06
#define AQ_SW_FORCE_PWMxA_LOW_PWMxB_HIGH(m)	EPwm##m##Regs.AQCSFRC.all = 0x09
#define AQ_SW_FORCE_PWMxA_LOW_PWMxB_LOW(m)	EPwm##m##Regs.AQCSFRC.all = 0x05

#define	clearPFCTripFlags()					EPwm9Regs.TZCLR.all = 8u|4U|2U;\
											EPwm10Regs.TZCLR.all = 8u|4U|2U;\
											EPwm12Regs.TZCLR.all = 8u|4U|2U


//dcdc pwm control
#define mOffDcdcPwm()		{EPwm1Regs.AQCSFRC.all = 0x05; EPwm2Regs.AQCSFRC.all = 0x05;\
							 EPwm3Regs.AQCSFRC.all = 0x05; EPwm4Regs.AQCSFRC.all = 0x05;\
							 EPwm5Regs.AQCSFRC.all = 0x05; EPwm6Regs.AQCSFRC.all = 0x05;\
							 EPwm7Regs.AQCSFRC.all = 0x05; EPwm8Regs.AQCSFRC.all = 0x05;}
#define mOnDcPwm_Bus()		{EPwm1Regs.AQCSFRC.all = 0x0; EPwm2Regs.AQCSFRC.all = 0x0;\
							 EPwm3Regs.AQCSFRC.all = 0x0; EPwm4Regs.AQCSFRC.all = 0x0;}
#define mOnDcPwm_Bat()		{EPwm5Regs.AQCSFRC.all = 0x0; EPwm6Regs.AQCSFRC.all = 0x0;\
							 EPwm7Regs.AQCSFRC.all = 0x0; EPwm8Regs.AQCSFRC.all = 0x0;}
#define mOffDcPwm_Bat()	  	{EPwm5Regs.AQCSFRC.all = 0x05; EPwm6Regs.AQCSFRC.all = 0x05;\
						 	 EPwm7Regs.AQCSFRC.all = 0x05; EPwm8Regs.AQCSFRC.all = 0x05;}
#define mOffDcPwm_Bus()	  	{EPwm1Regs.AQCSFRC.all = 0x05; EPwm2Regs.AQCSFRC.all = 0x05;\
						 	 EPwm3Regs.AQCSFRC.all = 0x05; EPwm4Regs.AQCSFRC.all = 0x05;}

//pfc pwm control
#define mOnPfcPwm()       {EPwm9Regs.AQCSFRC.all = 0x00; EPwm10Regs.AQCSFRC.all = 0x00;}    //
#define mOffPfcPwm()      {EPwm9Regs.AQCSFRC.all = 0x05;EPwm10Regs.AQCSFRC.all = 0x05;\
							GpioDataRegs.GPACLEAR.all = C28X_BIT22|C28X_BIT23;}
#define mOffHericPwm()		{EPwm11Regs.AQCSFRC.all = 0x05;}
#define mPFCFail()			mOffDcdcPwm()
//------------GPIO MAPPING--------------------
#define hiDCShort       GpioDataRegs.GPADAT.bit.GPIO13
#define liDCOCP	        GpioDataRegs.GPADAT.bit.GPIO11
#define liPFCOCP  	    GpioDataRegs.GPADAT.bit.GPIO15
#define hiHvsd	        GpioDataRegs.GPCDAT.bit.GPIO94
#define mWrEnable()		  (GpioDataRegs.GPCCLEAR.bit.GPIO93 = 1)
#define mWrDisable()	  (GpioDataRegs.GPCSET.bit.GPIO93 = 1)
#define mMainRelayClose() (GpioDataRegs.GPCSET.bit.GPIO67 = 1)                              // Main relay is the contactor controls input voltage.
#define mMainRelayOpen()  (GpioDataRegs.GPCCLEAR.bit.GPIO67 = 1)                            //  There is no relay on the output.
#define mMainRelayCloseCheck() (1 == GpioDataRegs.GPCDAT.bit.GPIO67)
#define mGreenLedOn()	  (GpioDataRegs.GPCSET.bit.GPIO76 = 1)
#define mGreenLedOff()	  (GpioDataRegs.GPCCLEAR.bit.GPIO76 = 1)
#define mGreenLedTwink()  (GpioDataRegs.GPCTOGGLE.bit.GPIO76 = 1)
#define mWDLedOn()	      (GpioDataRegs.GPCSET.bit.GPIO77 = 1)
#define mWDLedTwink()	  (GpioDataRegs.GPCTOGGLE.bit.GPIO77 = 1)


#define	WatchDogKick()	ServiceDog()
#define	WatchDogKickPointer()	ServiceDog()
//*****************************************************************************
// the function prototypes
//*****************************************************************************

#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif /* INCLUDE_A_MACRO_H_ */
