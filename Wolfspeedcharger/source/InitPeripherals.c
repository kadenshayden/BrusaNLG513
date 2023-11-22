/*****************************************************************************
 *	  File Name:InitPeripherals.c
 *	Description: HW initialization.
 *	Created on: 2018年4月12日
 *      Author: hasee
 *	Current Version:
 *	Date Modified:
*****************************************************************************/

// Included Files
//
#include <Shared.h> //only useful for InitEPWM()
//----------------CMPSS & DAC------------------
//definitions for selecting DACH reference
#define REFERENCE_VDDA     0	//3.3V基准
#define REFERENCE_VDAC     1 //与B0复用
//definitions for COMPH input selection
#define NEGIN_DAC          0
#define NEGIN_PIN          1

#define REFERENCE_VREF      1
#define DACA         1
#define DACB         2
#define DACC         3
#define REFERENCE            REFERENCE_VDAC
#define DAC_NUM                DACA


#define GLD_DC 1

// Function Prototypes
void InitPowerSupply(void);
void InitRAM(void);
void EnableDog(void);
void InitI2C(void);
inline void I2CA_Init(void);
void InitCMPSS(void);
void InitADC(void);
inline void ConfigureADC(void);
inline void SetupADCSoftware(void);
void InitEPWM(void);

void InitPowerSupply(void) //PFC Supply
{
	ECap1Regs.TSCTR = 0;//just in case
	do //only ECAP1 used. ECAP2OUT is always low.<--->Sigle polarity
	{
		if(ECap1Regs.ECFLG.bit.CTR_PRD)
		{
			if((ECap1Regs.CAP2 + 5) >= ((ECap2Regs.CAP1)>>1))
			{
				ECap1Regs.CAP2 = (ECap2Regs.CAP1)>>1; //50% duty
				break; //
			}
			else
			{
				ECap1Regs.CAP2 += 5;
			}
			ECap1Regs.ECCLR.bit.CTR_PRD = 1; //clear flag
		}
	}while(1);
}


void InitRAM(void) //Cpu1toCpu2MSGRAM
{
	Uint16 *uiPTmp;
	for(uiPTmp = (Uint16 *)0x03FC00;uiPTmp<(Uint16 *)0x040000;uiPTmp++)
	{
		*uiPTmp = 0;
	}
}
//
// Enable the watchdog
//
void EnableDog(void)
{
    EALLOW;
    WdRegs.WDCR.all = 0x002F;
    EDIS;
}




void InitI2C(void)
{
	mWrDisable();
	//------GPIO----------
	I2cAGpioConfig(I2C_A_GPIO91_GPIO92);
	//-------Setup---------
	I2CA_Init();
}

//
// I2CA_Init - Initialize I2CA settings
//
inline void I2CA_Init(void)
{
	// Initialize I2C
	I2caRegs.I2CMDR.bit.IRS = 0;

    I2caRegs.I2CSAR.all = 0x0050;

    I2caRegs.I2CPSC.all = 19;
    I2caRegs.I2CCLKL = 45;
    I2caRegs.I2CCLKH = 70;
    I2caRegs.I2CIER.all = 0x0;

    //IRS bit
    I2caRegs.I2CMDR.all = 0x0020;     // Take I2C out of reset
                                      // Stop I2C when suspended

    I2caRegs.I2CFFTX.all = 0x6000;    // Enable FIFO mode and TXFIFO
    I2caRegs.I2CFFRX.all = 0x2040;    // Enable RXFIFO, clear RXFFINT,

	I2caRegs.I2CCNT = 0x0000;
	I2caRegs.I2CSTR.all = 0xFFFF;
}


//
// InitCMPSS - Initialize CMPSS4 and configure settings
void InitCMPSS(void)
{
    EALLOW;
    Cmpss4Regs.COMPCTL.all = 0;
    Cmpss4Regs.COMPCTL.bit.COMPDACE = 1;	//comparator/DAC enable

    Cmpss4Regs.COMPDACCTL.all = 0;

    Cmpss4Regs.DACHVALS.bit.DACVAL = 2048+800;
	Cmpss4Regs.DACLVALS.all =  2048-800;

   Cmpss4Regs.COMPCTL.bit.COMPHSOURCE = NEGIN_DAC;
   Cmpss4Regs.COMPCTL.bit.COMPHINV = 0;
   Cmpss4Regs.COMPCTL.bit.ASYNCHEN = 0;
   Cmpss4Regs.COMPCTL.bit.COMPLSOURCE = NEGIN_DAC;
   Cmpss4Regs.COMPCTL.bit.COMPLINV = 1;
   Cmpss4Regs.COMPCTL.bit.ASYNCLEN = 0;

   Cmpss4Regs.CTRIPHFILCTL.bit.SAMPWIN = 9;
   Cmpss4Regs.CTRIPHFILCTL.bit.THRESH = 7;
   Cmpss4Regs.CTRIPHFILCLKCTL.bit.CLKPRESCALE = 2;
   Cmpss4Regs.CTRIPLFILCTL.bit.SAMPWIN = 9;
   Cmpss4Regs.CTRIPLFILCTL.bit.THRESH = 7;
   Cmpss4Regs.CTRIPLFILCLKCTL.bit.CLKPRESCALE = 2;

	//Reset filter logic & start filtering
   Cmpss4Regs.CTRIPHFILCTL.bit.FILINIT = 1;
   Cmpss4Regs.CTRIPLFILCTL.bit.FILINIT = 1;


   // Configure CTRIPOUT path
    Cmpss4Regs.COMPCTL.bit.CTRIPHSEL = 2;
    Cmpss4Regs.COMPCTL.bit.CTRIPOUTHSEL = 0;
    Cmpss4Regs.COMPCTL.bit.CTRIPLSEL = 2;
    Cmpss4Regs.COMPCTL.bit.CTRIPOUTLSEL = 0;

    Cmpss4Regs.COMPHYSCTL.bit.COMPHYS = 2;


	// Clear the latched comparator events
    Cmpss4Regs.COMPSTSCLR.bit.LLATCHCLR = 1;
    Cmpss4Regs.COMPSTSCLR.bit.HLATCHCLR = 1;

    EPwmXbarRegs.TRIP4MUX0TO15CFG.bit.MUX6 = 1;
    EPwmXbarRegs.TRIP4MUXENABLE.bit.MUX6 = 1;
//-----------------------------
    EDIS;
}


//
// ADC_Init - Initialize ADC settings
//
void InitADC(void)
{
	//Configure the ADCs and power them up
	    ConfigureADC();
	//Setup the ADCs for software conversions
	    SetupADCSoftware();
}

//Write ADC configurations and power up the ADC for both ADC A and ADC B
inline void ConfigureADC(void) // no need to modify this function
{
	EALLOW;

	//step1: write configurations
	AdcaRegs.ADCCTL2.bit.PRESCALE = ADC_CLK_DIV_4_0; //set ADCCLK divider to /4
	AdcbRegs.ADCCTL2.bit.PRESCALE = ADC_CLK_DIV_4_0; //set ADCCLK divider to /4
	AdccRegs.ADCCTL2.bit.PRESCALE = ADC_CLK_DIV_4_0; //set ADCCLK divider to /4
	AdcdRegs.ADCCTL2.bit.PRESCALE = ADC_CLK_DIV_4_0; //set ADCCLK divider to /4
	AdcSetMode(ADC_ADCA, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCB, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCC, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);
    AdcSetMode(ADC_ADCD, ADC_RESOLUTION_12BIT, ADC_SIGNALMODE_SINGLE);

	//step2: Set pulse positions to late
	AdcaRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcbRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdccRegs.ADCCTL1.bit.INTPULSEPOS = 1;
	AdcdRegs.ADCCTL1.bit.INTPULSEPOS = 1;

	//step3: power up the ADCs
	AdcaRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcbRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdccRegs.ADCCTL1.bit.ADCPWDNZ = 1;
	AdcdRegs.ADCCTL1.bit.ADCPWDNZ = 1;

	//step4: delay for 1ms to allow ADC time to power up
	DELAY_US(1000);

	EDIS;
}

inline void SetupADCSoftware(void)
{
	Uint16 acqps;

	//determine minimum acquisition window (in SYSCLKS) based on resolution
	if(ADC_RESOLUTION_12BIT == AdcaRegs.ADCCTL2.bit.RESOLUTION)
	{
		acqps = 29;
	}
	else //resolution is 16-bit
	{
		acqps = 63; //320ns
	}

//Select the channels to convert and end of conversion flag
    //-----------ADCA: DCCUR sampled at DC freq can be used directly by PFC.-------------------------------
    EALLOW;
    AdcaRegs.ADCSOC0CTL.bit.CHSEL = PFCTEMP;  //SOC0 will convert pin A0
    AdcaRegs.ADCSOC0CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdcaRegs.ADCSOC1CTL.bit.CHSEL = PFCCURR;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdcaRegs.ADCSOC2CTL.bit.CHSEL = VPFCOVP;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC2CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdcaRegs.ADCSOC3CTL.bit.CHSEL = SRTEMP;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC3CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC3CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdcaRegs.ADCSOC4CTL.bit.CHSEL = AIRTEMP;  //SOC1 will convert pin A1
    AdcaRegs.ADCSOC4CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcaRegs.ADCSOC4CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    //-----------ADCC-------------------------------
    AdccRegs.ADCSOC0CTL.bit.CHSEL = VAC;  //SOC1 will convert pin A1
    AdccRegs.ADCSOC0CTL.bit.ACQPS = acqps+10; //sample window is acqps + 1 SYSCLK cycles
    AdccRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdccRegs.ADCSOC1CTL.bit.CHSEL = VPFCCTRL;  //SOC1 will convert pin A1
    AdccRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdccRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCA;

    AdccRegs.ADCSOC2CTL.bit.CHSEL = PFCCURR;  //SOC1 will convert pin A1
    AdccRegs.ADCSOC2CTL.bit.ACQPS = acqps+10; //sample window is acqps + 1 SYSCLK cycles
    AdccRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM9_SOCB;
    //---ADCA中断：4 PIE Vectors total
    //-----------ADCB-------------------------------
    //ADCB
    AdcbRegs.ADCSOC0CTL.bit.CHSEL = DCVOLT;  //SOC0 will convert pin B0
    AdcbRegs.ADCSOC0CTL.bit.ACQPS = acqps + 10; //+50ns for first sample
    AdcbRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM1_SOCA;

    AdcbRegs.ADCSOC1CTL.bit.CHSEL = DCCURR;  //SOC1 will convert pin B1
    AdcbRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcbRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM1_SOCA;

    AdcbRegs.ADCSOC2CTL.bit.CHSEL = VPFCOVP_B;  //SOC1 will convert pin A1
    AdcbRegs.ADCSOC2CTL.bit.ACQPS = acqps + 30; //sample window is acqps + 1 SYSCLK cycles
    AdcbRegs.ADCSOC2CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM1_SOCA;

    //-----------ADCD-------------------------------
    AdcdRegs.ADCSOC0CTL.bit.CHSEL = BATCURR_DC;  //SOC1 will convert pin A1
    AdcdRegs.ADCSOC0CTL.bit.ACQPS = acqps+10; //sample window is acqps + 1 SYSCLK cycles
    AdcdRegs.ADCSOC0CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM1_SOCA;

    AdcdRegs.ADCSOC1CTL.bit.CHSEL = BATCURR_SINE;  //SOC1 will convert pin A1
    AdcdRegs.ADCSOC1CTL.bit.ACQPS = acqps; //sample window is acqps + 1 SYSCLK cycles
    AdcdRegs.ADCSOC1CTL.bit.TRIGSEL = ADC_TRIGGER_EPWM1_SOCA;

    EDIS;
}


//
// CLA_configClaMemory - Configure CLA memory sections
//
void CLA_configClaMemory(void)
{
    extern uint32_t Cla1funcsRunStart, Cla1funcsLoadStart, Cla1funcsLoadSize;
    EALLOW;

#ifdef _FLASH
    //
    // Copy over code from FLASH to RAM
    //
    memcpy((uint32_t *)&Cla1funcsRunStart, (uint32_t *)&Cla1funcsLoadStart,
           (uint32_t)&Cla1funcsLoadSize);
#endif //_FLASH

    //
    // Initialize and wait for CLA1ToCPUMsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CLA1TOCPU = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CLA1TOCPU != 1){};

    //
    // Initialize and wait for CPUToCLA1MsgRAM
    //
    MemCfgRegs.MSGxINIT.bit.INIT_CPUTOCLA1 = 1;
    while(MemCfgRegs.MSGxINITDONE.bit.INITDONE_CPUTOCLA1 != 1){};

    //
    // Select LS4RAM and LS5RAM to be the programming space for the CLA
    // First configure the CLA to be the master for LS4 and LS5 and then
    // set the space to be a program block
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS4 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS4 = 1;
    MemCfgRegs.LSxMSEL.bit.MSEL_LS5 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS5 = 1;

    //
    // Next configure LS0RAM and LS1RAM as data spaces for the CLA
    // First configure the CLA to be the master for LS0(1) and then
    // set the spaces to be code blocks
    //
    MemCfgRegs.LSxMSEL.bit.MSEL_LS0 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS0 = 0;

    MemCfgRegs.LSxMSEL.bit.MSEL_LS1 = 1;
    MemCfgRegs.LSxCLAPGM.bit.CLAPGM_LS1 = 0;

    EDIS;
}

//
// CLA_initCpu1Cla1 - Initialize CLA1 task vectors and end of task interrupts
//
void CLA_initCpu1Cla1(void)
{
    //
    // Compute all CLA task vectors
    // On Type-1 CLAs the MVECT registers accept full 16-bit task addresses as
    // opposed to offsets used on older Type-0 CLAs
    //
    EALLOW;
    Cla1Regs.MVECT1 = (uint16_t)(&Cla1Task1);
    Cla1Regs.MVECT2 = (uint16_t)(&Cla1Task2);
//    Cla1Regs.MVECT3 = (uint16_t)(&Cla1Task3);
//    Cla1Regs.MVECT4 = (uint16_t)(&Cla1Task4);
//    Cla1Regs.MVECT5 = (uint16_t)(&Cla1Task5);
//    Cla1Regs.MVECT6 = (uint16_t)(&Cla1Task6);
//    Cla1Regs.MVECT7 = (uint16_t)(&Cla1Task7);
    Cla1Regs.MVECT8 = (uint16_t)(&Cla1Task8);

	DmaClaSrcSelRegs.CLA1TASKSRCSEL1.bit.TASK2 = CLA_TRIGGER_EPWM9INT;//

    //
    // Enable the IACK instruction to start a task on CLA in software
    // for all  8 CLA tasks. Also, globally enable all 8 tasks (or a
    // subset of tasks) by writing to their respective bits in the
    // MIER register
    //
    Cla1Regs.MCTL.bit.IACKE = 1;
    Cla1Regs.MIER.all = (M_INT8 | M_INT7 | M_INT2 | M_INT1);//0x00FF;

//    //
//    // Configure the vectors for the end-of-task interrupt for all
//    // 8 tasks
//    //
//    PieVectTable.CLA1_1_INT = &cla1Isr1;
//    PieVectTable.CLA1_2_INT = &cla1Isr2;
//    PieVectTable.CLA1_3_INT = &cla1Isr3;
//    PieVectTable.CLA1_4_INT = &cla1Isr4;
//    PieVectTable.CLA1_5_INT = &cla1Isr5;
//    PieVectTable.CLA1_6_INT = &cla1Isr6;
//    PieVectTable.CLA1_7_INT = &cla1Isr7;
//    PieVectTable.CLA1_8_INT = &cla1Isr8;
//
//    //
//    // Enable CLA interrupts at the group and subgroup levels
//    //
//    PieCtrlRegs.PIEIER11.all = 0xFFFF;
//    IER |= (M_INT11 );
    EDIS;
}
//
// InitEPWM - Initialize EPWM module settings
void InitEPWM(void)
{
	volatile struct EPWM_REGS* PWM_PTR[13] = {&EPwm1Regs,\
			&EPwm1Regs,&EPwm2Regs,&EPwm3Regs,&EPwm4Regs,&EPwm5Regs,&EPwm6Regs,\
			&EPwm7Regs,&EPwm8Regs,&EPwm9Regs,&EPwm10Regs,&EPwm11Regs,&EPwm12Regs};

	volatile struct EPWM_REGS* PWM_PTR_INV[13] = {&EPwm1Regs,\
			&EPwm5Regs,&EPwm6Regs,&EPwm7Regs,&EPwm8Regs,\
			&EPwm1Regs,&EPwm2Regs,&EPwm3Regs,&EPwm4Regs,
			&EPwm9Regs,&EPwm10Regs,&EPwm11Regs,&EPwm12Regs};


	Uint16 num=1;

	mOffDcdcPwm();
	mOffPfcPwm();
	mOffHericPwm();

	EALLOW;
    EPwm1Regs.HRMSTEP.bit.HRMSTEP = 65;
    EDIS;

    if	(MODE_INV == CputoClaVar.u16WorkMode)//only inv mode
    {
		for(num=1;num<=12;num++)
		{
			PWM_PTR[num] = PWM_PTR_INV[num];
		}
    }

    EALLOW;
    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 0;

    ClkCfgRegs.PERCLKDIVSEL.bit.EPWMCLKDIV = 1; //default:1
	EDIS;
	for(num=1;num<=8;num++)
	{
		PWM_PTR[num]->TBCTR = 0;
		PWM_PTR[num]->TBPRD = DC_TBPRD_Fmax;
		PWM_PTR[num]->TBPRDHR = 0;
		PWM_PTR[num]->TBCTL.all = 0;
		PWM_PTR[num]->TBCTL.bit.FREE_SOFT = 3;
		PWM_PTR[num]->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
		//if(1 == num)
		if(&EPwm1Regs == PWM_PTR[num])
		{
			PWM_PTR[num]->TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;

			PWM_PTR[num]->TBPHS.all = 0uL;
			PWM_PTR[num]->TBCTL.bit.PHSEN = TB_DISABLE;
		}
		else
		{
			if((&EPwm3Regs == PWM_PTR[num]) || (&EPwm6Regs == PWM_PTR[num]) || (&EPwm8Regs == PWM_PTR[num])) //see syn patch below
			{
				PWM_PTR[num]->TBCTL.bit.SYNCOSEL = TB_SYNC_DISABLE;
			}
			else
			{
				PWM_PTR[num]->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
			}
				PWM_PTR[num]->TBCTL.bit.PHSEN = TB_ENABLE;
				PWM_PTR[num]->TBCTL.bit.PHSDIR = TB_UP;

				PWM_PTR[num]->TBPHS.all = 2uL<<16;
		}


		PWM_PTR[num]->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		PWM_PTR[num]->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
		PWM_PTR[num]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO;
		PWM_PTR[num]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO;

		if((2 == num) ||(3 == num) || (6 == num) || (7 == num))
		{
			PWM_PTR[num]->AQCTLA.bit.CBD = AQ_SET;
			PWM_PTR[num]->AQCTLA.bit.CAU = AQ_CLEAR;
			PWM_PTR[num]->CMPA.all = CMP_MIN_uL<<16;
			PWM_PTR[num]->CMPB.all = CMP_MIN_uL<<16;
		}
		else
		{
			PWM_PTR[num]->AQCTLA.bit.CBU = AQ_SET;
			PWM_PTR[num]->AQCTLA.bit.CAD = AQ_CLEAR;
			PWM_PTR[num]->CMPA.all = (DC_TBPRD_Fmax-CMP_MIN_uL)<<16;
			PWM_PTR[num]->CMPB.all = (DC_TBPRD_Fmax-CMP_MIN_uL)<<16;
		}

		PWM_PTR[num]->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
		PWM_PTR[num]->DBCTL.bit.POLSEL = DB_ACTV_HIC;
		if(num<=4)
		{
			PWM_PTR[num]->DBFED.all = DC_DB_TIME;
			PWM_PTR[num]->DBRED.all = DC_DB_TIME;
		}
		else
		{
			PWM_PTR[num]->DBFED.all = DC_DB_TIME_SR;
			PWM_PTR[num]->DBRED.all = DC_DB_TIME_SR;
		}

		PWM_PTR[num]->EPWMXLINK.bit.TBPRDLINK = EPWM_LINK_WITH_EPWM_1;
	}
    EALLOW;
	SyncSocRegs.SYNCSELECT.bit.EPWM4SYNCIN = SYNC_SELECT_EPWM1SYNCOUT;
	SyncSocRegs.SYNCSELECT.bit.EPWM7SYNCIN = SYNC_SELECT_EPWM1SYNCOUT;
	EDIS;

	if	(MODE_INV != CputoClaVar.u16WorkMode)//rec or invalid
	{
		EPwm5Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_4;
		EPwm6Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_3;
		EPwm7Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_3;
		EPwm8Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_4;

		EPwm7Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_6;
		EPwm8Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_5;

		EPwm1Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_1;
		EPwm2Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_2;
		EPwm3Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_3;
		EPwm4Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_4;

		EPwm3Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_3;
		EPwm4Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_4;

	}
	else
	{
		EPwm1Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_8;
		EPwm2Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_7;
		EPwm3Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_7;
		EPwm4Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_8;

		EPwm3Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_2;
		EPwm4Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_1;

		EPwm5Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_5;
		EPwm6Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_6;
		EPwm7Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_7;
		EPwm8Regs.EPWMXLINK.bit.CMPBLINK = EPWM_LINK_WITH_EPWM_8;

		EPwm7Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_7;
		EPwm8Regs.EPWMXLINK.bit.CMPALINK = EPWM_LINK_WITH_EPWM_8;
	}



	for(num=9;num<=11;num++)
	{
		PWM_PTR[num]->TBCTR = 0;
		PWM_PTR[num]->TBPRD = PFC_PRD;
		PWM_PTR[num]->TBPHS.all = 0uL;
		PWM_PTR[num]->TBCTL.all = 0;
		PWM_PTR[num]->TBCTL.bit.FREE_SOFT = 3;
		PWM_PTR[num]->TBCTL.bit.CTRMODE = TB_COUNT_UPDOWN;
		if(&EPwm9Regs == PWM_PTR[num])
		{
			PWM_PTR[num]->TBCTL.bit.SYNCOSEL = TB_CTR_ZERO;
		}
		else
		{
			PWM_PTR[num]->TBCTL.bit.SYNCOSEL = TB_SYNC_IN;
		}

		PWM_PTR[num]->CMPCTL.bit.SHDWAMODE = CC_SHADOW;
		PWM_PTR[num]->CMPCTL.bit.SHDWBMODE = CC_SHADOW;
		PWM_PTR[num]->CMPCTL.bit.LOADAMODE = CC_CTR_ZERO_PRD;
		PWM_PTR[num]->CMPCTL.bit.LOADBMODE = CC_CTR_ZERO_PRD;
		if(9 == num)
		{
			PWM_PTR[num]->AQCTLA.bit.CAU = AQ_SET;
			PWM_PTR[num]->AQCTLA.bit.CAD = AQ_CLEAR;
			PWM_PTR[num]->CMPA.all = (PFC_PRD - CMP_MIN_uL)<<16;
		}
		else
		{
			PWM_PTR[num]->AQCTLA.bit.CAU = AQ_CLEAR;
			PWM_PTR[num]->AQCTLA.bit.CAD = AQ_SET;
			PWM_PTR[num]->CMPA.all = CMP_MIN_uL<<16;
		}
		PWM_PTR[num]->DBCTL.bit.OUT_MODE = DB_FULL_ENABLE;
		PWM_PTR[num]->DBCTL.bit.POLSEL = DB_ACTV_HIC;
		PWM_PTR[num]->DBFED.all = PFC_DB_TIME;		PWM_PTR[num]->DBRED.all = PFC_DB_TIME;
 	}


    EPwm1Regs.ETSEL.bit.SOCASEL = ET_CTR_ZERO;
    EPwm1Regs.ETSEL.bit.SOCAEN = 1;
    EPwm1Regs.ETPS.bit.SOCAPRD = ET_1ST;
    EPwm1Regs.ETCLR.all = 0x0F;

    EPwm9Regs.ETSEL.bit.INTSEL = ET_CTR_PRD;
    EPwm9Regs.ETSEL.bit.INTEN = 0;
    EPwm9Regs.ETPS.bit.INTPRD = ET_1ST;
    EPwm9Regs.ETCLR.all = 0x1;

    EPwm9Regs.ETSEL.bit.SOCASEL = ET_CTR_PRD;
    EPwm9Regs.ETSEL.bit.SOCAEN = 1;
    EPwm9Regs.ETPS.bit.SOCAPRD = ET_1ST;

    EPwm9Regs.ETSEL.bit.SOCBSEL = ET_CTR_ZERO;
    EPwm9Regs.ETSEL.bit.SOCBEN = 1;
    EPwm9Regs.ETPS.bit.SOCBPRD = ET_1ST;

    EPwm9Regs.ETCLR.all = 0x0F;
//------------------------------------


    EALLOW;
    EPwm11Regs.TZSEL.bit.DCAEVT2 = TZ_ENABLE;
    EPwm11Regs.TZSEL.bit.OSHT2 = TZ_ENABLE;
//    EPwm11Regs.TZSEL.bit.OSHT1 = TZ_ENABLE;

    EPwm11Regs.TZCTL.bit.TZA = TZ_FORCE_LO;
    EPwm11Regs.TZCTL.bit.TZB = TZ_FORCE_LO;

    EPwm11Regs.TZFRC.bit.OST = 1;

	EPwm11Regs.TZEINT.all = 0;

	for(num=1;num<=8;num++)
	{
//---------------------------------
		PWM_PTR[num]->HRCNFG.all = 0x0;
		PWM_PTR[num]->HRCNFG.bit.EDGMODE = HR_BEP;
		PWM_PTR[num]->HRCNFG.bit.CTLMODE = HR_CMP;
		PWM_PTR[num]->HRCNFG.bit.HRLOAD = HR_CTR_ZERO_PRD;

		PWM_PTR[num]->HRCNFG.bit.EDGMODEB = HR_BEP;
		PWM_PTR[num]->HRCNFG.bit.CTLMODEB = HR_CMP;
		PWM_PTR[num]->HRCNFG.bit.HRLOADB = HR_CTR_ZERO_PRD;

		PWM_PTR[num]->HRCNFG.bit.AUTOCONV = 1;
		PWM_PTR[num]->HRPCTL.bit.TBPHSHRLOADE = 1;
		PWM_PTR[num]->HRPCTL.bit.HRPE = 1;
//-------------------------------------

		PWM_PTR[num]->GLDCTL.bit.OSHTMODE = 1;
		PWM_PTR[num]->GLDCTL.bit.GLD = 1;
		PWM_PTR[num]->GLDCTL.bit.GLDMODE = 4;

		PWM_PTR[num]->GLDCFG.bit.CMPA_CMPAHR = 1;
		PWM_PTR[num]->GLDCFG.bit.CMPB_CMPBHR = 1;
		PWM_PTR[num]->GLDCFG.bit.TBPRD_TBPRDHR = 1;

		PWM_PTR[num]->GLDCFG.bit.AQCTLA_AQCTLA2 = 0;

		PWM_PTR[num]->EPWMXLINK.bit.GLDCTL2LINK = EPWM_LINK_WITH_EPWM_1;

//	    PWM_PTR[num]->TZSEL.bit.OSHT1 = TZ_ENABLE;
	    PWM_PTR[num]->TZCTL.bit.TZA = TZ_FORCE_LO;
	    PWM_PTR[num]->TZCTL.bit.TZB = TZ_FORCE_LO;
	    PWM_PTR[num]->TZCLR.bit.OST = 1;
	    PWM_PTR[num]->TZCLR.bit.INT = 1;
	}
//	EPwm1Regs.TZEINT.bit.OST = 1;


	EPwm1Regs.GLDCTL2.bit.OSHTLD = 1;

	for(num=9;num<=10;num++)
	{
	    PWM_PTR[num]->DCTRIPSEL.bit.DCAHCOMPSEL = EPWM_DC_TRIP_TRIPIN4;
	    PWM_PTR[num]->TZDCSEL.bit.DCAEVT2 = EPWM_TZ_EVENT_DCXH_HIGH;
	    PWM_PTR[num]->DCACTL.bit.EVT2SRCSEL = EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL;
	    PWM_PTR[num]->DCACTL.bit.EVT2FRCSYNCSEL = EPWM_DC_EVENT_INPUT_NOT_SYNCED;


	    PWM_PTR[num]->TZSEL.bit.DCAEVT2 = TZ_ENABLE;
//	    PWM_PTR[num]->TZSEL.bit.OSHT1 = TZ_ENABLE;
	    PWM_PTR[num]->TZCTL.bit.TZA = TZ_FORCE_LO;
	    PWM_PTR[num]->TZCTL.bit.TZB = TZ_FORCE_LO;

	    if(9 == num)
	    {
	    	PWM_PTR[num]->TZCLR.bit.CBCPULSE = 0;
	    }
	    else
	    {
	    	PWM_PTR[num]->TZCLR.bit.CBCPULSE = 1;
	    }

	    PWM_PTR[num]->TZCLR.bit.DCAEVT2 = 1;
	    PWM_PTR[num]->TZCLR.bit.CBC = 1;
	    PWM_PTR[num]->TZCLR.bit.INT = 1;
	    PWM_PTR[num]->TZCLR.bit.OST = 1;
	    PWM_PTR[num]->TZEINT.all = 0;
	}
	EPwm11Regs.TZFRC.bit.OST = 1;


    CpuSysRegs.PCLKCR0.bit.TBCLKSYNC = 1;
	EDIS;

}

//
// InitECapture - Initialize ECAP1 configurations
//
void InitECapture()
{
	EALLOW;
	ECap4Regs.ECEINT.all = 0x0000;          // Disable all capture __interrupts
	ECap4Regs.ECCLR.all = 0xFFFF;           // Clear all CAP __interrupt flags
	ECap4Regs.ECCTL1.bit.CAPLDEN = 0;       // Disable CAP1-CAP4 register loads
	ECap4Regs.ECCTL2.bit.TSCTRSTOP = EC_FREEZE;     // Make sure the counter is stopped

	ECap4Regs.TSCTR = 0uL;
	ECap4Regs.CTRPHS = 2uL;//to do
	//
	// Configure peripheral registers
	//
	ECap4Regs.ECCTL2.bit.CONT_ONESHT = EC_ONESHOT;
	ECap4Regs.ECCTL2.bit.STOP_WRAP = EC_EVENT4;
	ECap4Regs.ECCTL1.bit.CAP1POL = EC_FALLING;
	ECap4Regs.ECCTL1.bit.CAP2POL = EC_RISING;
	ECap4Regs.ECCTL1.bit.CAP3POL = EC_FALLING;
	ECap4Regs.ECCTL1.bit.CAP4POL = EC_RISING;
	ECap4Regs.ECCTL1.bit.CTRRST1 = EC_DELTA_MODE;	// Difference operation
	ECap4Regs.ECCTL1.bit.CTRRST2 = EC_DELTA_MODE;	// Difference operation
	ECap4Regs.ECCTL1.bit.CTRRST3 = EC_DELTA_MODE;	// Difference operation
	ECap4Regs.ECCTL1.bit.CTRRST4 = EC_DELTA_MODE;	// Difference operation
	ECap1Regs.ECCTL1.bit.PRESCALE = EC_DIV1;
	ECap1Regs.ECCTL2.bit.CAP_APWM = EC_CAP_MODE;
	ECap4Regs.ECCTL2.bit.SYNCI_EN = EC_ENABLE; // Enable sync in
	ECap4Regs.ECCTL2.bit.SYNCO_SEL = EC_SYNCIN; //Select sync-in event to be the sync-out signal pass through
	ECap4Regs.ECCTL1.bit.CAPLDEN = 1;	// Enable capture units

	ECap4Regs.ECCTL2.bit.TSCTRSTOP = EC_RUN;     // Start Counter
	ECap4Regs.ECCTL2.bit.REARM = EC_ARM;         // arm one-shot
	ECap4Regs.ECCTL1.bit.CAPLDEN = 1;       // Enable CAP1-CAP4 register loads

	SyncSocRegs.SYNCSELECT.bit.ECAP4SYNCIN = SYNC_SELECT_EPWM1SYNCOUT;
	EDIS;
}

//
// End of file
//
