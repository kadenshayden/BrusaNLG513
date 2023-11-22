/*============================================================================*
 * constant definition
 *============================================================================*/




#define REC_DIS_ENABLE 1
#define MODE_INV	1u
#define MODE_REC	0u
#define UNVALID		0x00FF
#define POWON_WAIT  2u

#define	DC_DB_TIME_200NS		(UINT16)20
#define	DC_DB_TIME_250NS		(UINT16)25
#define	DC_DB_TIME_300NS		(UINT16)30

#define	DC_DB_TIME_SR_250NS		(UINT16)25
#define	DC_DB_TIME_SR_280NS		(UINT16)28
#define	DC_DB_TIME_SR_300NS		(UINT16)30
#define	DC_DB_TIME_SR_350NS		(UINT16)35
#define	DC_DB_TIME_SR_370NS		(UINT16)37
#define	DC_DB_TIME_SR_400NS		(UINT16)40
#define	DC_DB_TIME_SR_420NS		(UINT16)42
#define	DC_DB_TIME_SR_450NS		(UINT16)45

#define SR_DC_FED_AHEAD_300NS		30
#define SR_DC_FED_AHEAD_250NS		25
#define SR_DC_FED_AHEAD_150NS		15

#define	DC_DB_TIME			DC_DB_TIME_200NS
#define	DC_DB_TIME_SR		DC_DB_TIME_SR_350NS
#define SR_DC_FED_AHEAD		SR_DC_FED_AHEAD_150NS


#define	DC_TBPRD_Fmax				167uL
#define	DC_PRD_250K				200uL
#define	DC_TBPRD_222K			224L
#define	DC_TBPRD_200K			250L
#define	DC_DUTY_MIN				20uL
#define	CMP_MIN_uL				10uL
#define	DC_Ts_Max_101K			(UINT16)990
#define DC_Ts_Max_148K          (UINT16)676
#define	DC_Ts_Max_150K			(UINT16)666
#define DC_Ts_Max_152K          (UINT16)658
#define DC_Ts_Max_154K          (UINT16)649
#define	DC_Ts_Max_156K			(UINT16)640
#define	DC_Ts_Max_160K			(UINT16)626
#define	DC_Ts_Min_250K			(UINT16)400
#define	DC_Ts_Min_350K			(UINT16)286
#define	DC_Ts_Min_300K			(UINT16)334

#define	PFC_PRD_67K				746uL
#define	PFC_PRD_70K				715uL
#define	PFC_PRD				PFC_PRD_67K
#define	PFC_DUTY_MIN			(PFC_PRD-1u)
#define	PFC_DUTY_MAX			70u
#define	PFC_DB_TIME_300NS		(UINT16)30
#define	PFC_DB_TIME_500NS		(UINT16)50
#define	PFC_DB_TIME		PFC_DB_TIME_500NS


#define VSET_LST		VDC_198V
#define VSET_LST_DFT	VDC_200V
#define VSET_HST_DFT	VDC_450V
#define VSET_HST		VDC_455V

#define OVP_LST			VDC_400V
#define OVP_HST			VDC_510V
#define VDIS_HST		VDC_550V

#define LQ10_VDC_DFT_LIM_DN		       	VDC_200V
#define LQ10_VDC_DFT_LIM_UP		       	VDC_500V
#define LQ10_VDC_DEFAULT				VDC_366V

#define LQ10_VDC_UP_LIM_DN		       	VDC_490V
#define LQ10_VDC_UP_LIM_UP		       	VDC_500V
#define LQ10_VDC_UP_DFT		       		VDC_495V
//------------------------------------------------------------------------------
//setting Ratio coefficient                                         
//------------------------------------------------------------------------------
#define LQ10_CURRLIMITMAX		(INT32)1291
#define LQ10_CURRLIMITMAX_F		(float32)((float32)LQ10_CURRLIMITMAX/1024.0f)
#define LQ10_POWERLIMITMAX		(INT32)1249
#define LQ10_POWERLIMITRATE		(INT32)1024
#define LQ10_CURRLIM20PERCENT	(INT32)205

#define MAX_EFFICIENCY			(INT32)98

//----------------------------------------------------------------------
//define constant for e2prom default data and up down limt
//------------------------------------------------------------------------
#define LQ10_DCTEMP_UP_DFT			(((INT32)65) << 10)   //DCDC temperature,  90C~EPCOS 3744CNT 2.916V
#define LQ10_PFCTEMP_UP_DFT			(((INT32)65) << 10)   //PFC  temperature,  90C~EPCOS 3744CNT 2.916V


//------------------------------------------------------------------------------
//define sample Ratio coefficient                                         
//------------------------------------------------------------------------------
#define	LQ20_DEF_INV_VACFACTOR  	(INT32)247811

#define LQ10_DEF_VPFCFACTOR   		(INT32)8270
#define LQ20_DEF_INV_VPFCFACTOR   	(INT32)129843

#define	LQ10_DEF_VDCFACTOR 			(INT32)42318
#define	LQ20_DEF_INVVDCFACTOR 		(INT32)25373

#define LQ10_DEF_IDCFACTOR     	(INT32)46670
#define LQ20_DEF_INVIDCFACTOR   (INT32)23007

#define LQ10_DEF_POWERFACTOR  	(INT32)442
#define LQ10_DEF_IDCSYSFACTOR 	(INT32)700393

#define POWER_LIM_FLOOR 		(INT32)300
#define POWER_LIM_UPPER 		(INT32)4600
#define POWER_LIM_MIN			(INT32)50

//------20190715-----------------
#define LQ10_DEF_POWERFACTOR_BAT  	(INT32)221
#define LQ10_DEF_IDCSYSFACTOR_BAT 	(INT32)349525
#define POWER_LIM_UPPER_BAT 		(INT32)2131
#define POWER_LIM_MIN_BAT			(INT32)50
#define IDC_AD_MAX_BAT     (INT32)7850
#define IDC_AD_MIN_BAT     (INT32)2
//------------------------------------------------------------------------------
//define constant for dcdc voltage                                         
//------------------------------------------------------------------------------
#define VDC_AD_20V      (INT32)9319
#define VDC_AD_MAX     	(INT32)30712
#define VDC_AD_03V      (INT32)139
#define VDC_AD_0V5      (INT32)233
#define VDC_AD_W_BATT   VDC_AD_20V

//------------------------------------------------------------------------------
//define constant for pfc voltage                                         
//------------------------------------------------------------------------------
#define SSTART_PFCPWM_STEP    	((INT16)10)
#define SSTART_PFCVOL_STEP    	((INT16)8)
#define SSTART_DCDCPWM_STEP   	((INT16)3)
#define SSTART_DCDCPWM_STEP2   	((INT16)1)

#define WALKIN_STEP_CNT         ((INT32)150)
#define NON_WALKIN_STEP			((INT32)4000)



#define	VPFCAd_380v     (INT16)3000
#define	VPFCAd_330v     (INT16)2650

#define VPFC_HLOSS      (((INT32)440) << 10)
#define VPFC_HBACK      (((INT32)430) << 10)
#define VPFC_LLOSS      (((INT32)280) << 10)
#define VPFC_LBACK      (((INT32)300) << 10)


//define for mixed system use
#define IDC_AD_MAX     (INT32)22000
#define IDC_AD_MIN     (INT32)7



#define TEMP_AMBI_HLOSS (((INT32)79) << 10)
#define TEMP_AMBI_HBACK (((INT32)77) << 10)


#define	VAC_AD_300VPK		(INT16)1800
#define	VAC_AD_20V      	(INT16)100


#define VAC_HLOSS       (((INT32)285) << 10)
#define VAC_HBACK       (((INT32)270) << 10)
#define VAC_LLOSS       (((INT32)80) << 10)
#define VAC_LBACK       (((INT32)88) << 10)


//------------------------------------------------------------------------------
#define SQRT2       			((INT32)1448) 			// 1.414<<10
#define IQ12_ADC_VAC_FILTA  	((INT32)3994)			// 0.975<<12
#define IQ12_ADC_VAC_FILTB  	((INT32)102) 			// 0.025<<12
#define LQ10_ADC_VPFC_FILTA		((INT32)973)  			// 0.95<<10
#define LQ10_ADC_VPFC_FILTB		((INT32)51) 			// 0.05<<10
#define LQ10_ADC_DCCURR_FILTA	((INT32)973)  			// 0.95<<10
#define LQ10_ADC_DCCURR_FILTB	((INT32)51) 			// 0.05<<10
#define IQ10_ADC_PFCCURR_FILTA	((INT32)973)  			// 0.95<<10
#define IQ10_ADC_PFCCURR_FILTB	((INT32)51) 			// 0.05<<10
#define LQ10_ADC_TEMP_FILTA		((INT32)973)  			// 0.95<<10
#define LQ10_ADC_TEMP_FILTB		((INT32)51) 			// 0.05<<10
#define LQ10_ADC_VDC_FILTA  	((INT32)973)  		// 0.95<<10
#define LQ10_ADC_VDC_FILTB		((INT32)51) 			// 0.05<<10

//------------------------------------------------------------------------------
//define constant for time                                         
//------------------------------------------------------------------------------
#define  TIME5MS     	(UINT32)5500

#define  LQ10TIME300S  	(((INT32)300) << 10)
#define  LQ10TIME50S   	(((INT32)50) << 10)
#define  LQ10TIME10S   	(((INT32)10) << 10)

#define  TIME150S     	(UINT32)15000
#define  TIME60S     	(UINT32)6000
#define  TIME40S     	(UINT32)4000
#define  TIME20S     	(UINT32)2000
#define  TIME15S     	((UINT16)1500)
#define  TIMES10S     	(UINT32)1000
#define  TIME8S     	(UINT32)800
#define  TIME5S     	(UINT32)500
#define  TIMES2S     	(UINT32)200
#define  TIMES3S     	(UINT32)300
#define  TIMES3Min     	(UINT32)18000
#define  TIME500MS      (UINT32)50
#define  TIME250MS      (UINT32)25
#define  LQ10TIME8S    	(INT32)819200
#define  LQ10TIME200S   (INT32)20480000


#define  TIME1S5     	(UINT32)150
#define  TIME2S5     	(UINT32)250
#define  TIME10S		(UINT16)87

#define  TIME5S_BASE    (UINT32)56256
#define  TIME2S    		(UINT32)22502
#define  TIMES5MS     	(UINT32)56
#define  TIME1MS     	(UINT32)11
#define	 TIME711US		(UINT32)8

//below need test 
#define  DefRlyDelayAct 	(INT16)169
#define  DefWaveProidMax    (INT16)338

// define current sharing delta max value 
#define IDELTA_MAX		((INT16)200)    		//
#define N_IDELTA_MAX	((INT16)(-200))    		//



//------------------------------------------------------------------------------
//define constant for Circuit Cal                                         
//------------------------------------------------------------------------------
#define DCVOLT_REGU_STEP 		(INT32)100     
#define DCCURR_REGU_STEP 		(INT32)100
#define POW_REGU_STEP    		(INT32)100
#define PFCVOLT_REGU_STEP 		 VPFC_0V5
