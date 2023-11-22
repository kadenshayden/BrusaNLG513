#ifndef MAIN_H
#define MAIN_H

#ifdef __cplusplus
extern "C" {
#endif
//the functions of interrupt
extern interrupt void dcdc_ISR(void);
extern interrupt void epwm1_tzint_isr(void);
extern void vCan_Isr(void);

//Isr Vars initialization
extern void vIsr_init(void);
extern void vCLATaskinit(void);
extern void vCputoClaVar_init(void);

extern void	vDataInit(void);

extern void vScheduler(void);

extern UINT16 g_u16ActionReady;
extern UINT16 g_u16MainRlyCloseFlag;

extern void vMainRlyOnOffCtrl(void);	

extern UINT16 g_u16AddIdentifyFlag;
extern UINT16 g_u16AddIdentifyNum;
extern ubitinta  g_u16MdlCtrl;
extern ubitintc	g_u16RunFlag;

extern UINT16 g_u16VersionNoHw;	
extern UINT16 g_u16VersionNoSw;

extern ubitfloat g_lq10SetVoltTrue;
extern ubitfloat g_lq10SetVolt;
extern ubitfloat g_lq10SetLimit;
extern ubitfloat g_lq10SetLimitTrue;
extern ubitfloat g_lq10SetPower;
extern ubitfloat g_lq10SetPowerTrue;
extern ubitfloat g_lq10SetPowerWant;
extern ubitfloat g_lq10MdlLimit;
extern ubitfloat g_lq10MdlLimitCurrent;
extern ubitfloat g_lq10SetPfcVolt;
extern ubitfloat g_lq10SetPfcVoltTrue;
extern INT32	g_l32IdcdcSysTrue;
                 
extern ubitfloat g_lq12VoltConSysa;
extern ubitfloat g_lq12VoltConSysb;
extern ubitfloat g_lq10CurrConSysa;
extern ubitfloat g_lq10CurrConSysb;
extern ubitfloat g_lq10PowerConSysa;
extern ubitfloat g_lq10PowerConSysb;
extern ubitfloat g_lq10VpfcConSys;

extern UINT16 g_u16LimitStatus;

extern ubitfloat g_lq10PfcVoltRegulate;

extern INT32     g_i32PowerRate;
extern INT32     g_i32CurrRate;
extern INT32     g_i32CurrDisMax;

extern void	vRefTask(void);

extern ubitfloat g_lq10AcVolt;
extern ubitfloat g_lq10AcVrmsSampSysa;
extern ubitfloat g_lq10AcVrmsSampSysb;
extern ubitfloat g_lq10PfcVolt;
extern ubitfloat g_lq10PfcVoltDisp;
extern ubitfloat g_lq10VpfcSampSys;
extern ubitfloat g_lq10MdlVoltNoFilter;
extern ubitfloat g_lq10MdlVolt;
extern ubitfloat g_lq12VoltSampSysa;
extern ubitfloat g_lq12VoltSampSysb;
extern ubitfloat g_lq10MdlCurrNoFilter;
extern ubitfloat g_lq10MdlCurr;
extern ubitfloat g_lq10MdlCurrDisp;
extern ubitfloat g_lq10BatCurr;
extern ubitfloat g_lq10CurrSampSysa;
extern ubitfloat g_lq10CurrSampSysb;
extern ubitfloat g_lq10TempAmbi;
extern ubitfloat g_lq10TempAmbiDisp;
extern ubitfloat g_lq10TempDC;
extern ubitfloat g_lq10TempDCDisp;
extern ubitfloat g_lq10TempPFC;
extern ubitfloat g_lq10TempPFCDisp;



extern void	vAdcTask(void);

/*******************************************************************************
*variables and functions declare for warn treatment														  
*******************************************************************************/

extern ubitintb	g_u16MdlStatus;
extern ubitinte	g_u16MdlStatusH;
extern UINT16 g_u16CanFailTime;
extern UINT16 g_u1660sTimer;

extern void	vWarnTask(void);


extern ubitfloat g_lq10MdlVoltUp;
extern ubitfloat g_lq10DCTempUp;
extern ubitfloat g_lq10PFCTempUp;


extern void	vLimitInit(void);


#ifdef __cplusplus
}
#endif 


#endif 


