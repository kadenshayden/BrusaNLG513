#ifndef IsrVars_H
#define IsrVars_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************************************************************
*            Variables Definition
***************************************************************************/
struct IsrVarsStruct
{
int16     _i16AdVdcDis;
int16	  _i16VdcSet;
int16	  _i16AdVdcSam0;        
int16     _i16AdVdcSam1;         
longunion _lVdcDisUse; 
longunion _lVdcUse;
int16     _i16VdcErr0;           
int16 	  _i16VdcErr1;  
int16     rsvd0C;
int16     rsvd0D;
int16     _i16AdIdcDis;
int16     _i16AdIdc;
longunion _lIdcDisUse;
int32     l32IdcdcSys;
int16 	  _i16PowerErrUse0;
int16     _i16PowerErrUse1;
int32		LQ16CurrLim;
int16     _i16CurrLimFloor;
int16     _i16PowerMaxExtra;
int32		LQ10VdcPiOut;
Uint16	  _u16DcdcOCP;
Uint16    Rsd1A;
int16     _i16PowerLimMax;  
int16	  _i16DCDCPowerSet;
int16     _i16DCDCPowerFeedback;
int16     Rsd1F;		//
int16     _i16PowerErr0;         
int16     _i16PowerErr1;
int16     rsvd22;
Uint16 	  Rsd23;
Uint16    Rsd24;
int16 	  _i16IdcPioutPermit;
longunion _lIdcPiout;
int16 	  _i16DcdcPWMTsMin;
int16 	  _i16DcdcPWMTsMax;
longunion	_lDcdcPWMDutyMax;
longunion _lDcdcDutyShadow;
longunion _lDcdcPWMTsShadow;
longunion _lDcdcDuty;
longunion _lDcdcPWMTs;
longunion	_reserveF2;//used in isr
longunion	_reserveF4;//used in isr
int16 	  _i16VdcOVPTimes;
Uint16	  _u16DcdcPWMChgFlag;
Uint16    rsvd3A;
Uint16    rsvd3B;
Uint16    _u16ChoiseCon;
Uint16    rsvd3D;
int16     rsvd3E;
int16		u16HwPFCOCP;
Uint16    _u16IsrTimer;    
Uint16    u16SROnDelay;//u16Rsvd42; last value
int32		l32SR_DC_FED_AHEAD;
float32	  f32Vin;
float32	  f32Vo;
float32	  f32Vin_Vo;
float32	  f32SRDelay;
int32		l32SR_DC_FED_DELAY;//_i16VpfcPiTmp;//_i16VpfcPiTmpOld;
int16		i16SROnDelay; //next value
int16		i16SROffDelay;
int16	  i16AdVpfc0;
int16	  i16AdVpfc1;//
int16     i16VpfcSet;
Uint16    _i16PFCQuickLoopForce;
Uint16    rsvd7x;
int16 	_i16VpfcFilterK2;
longunion _lVpfcFilterK1;
longunion _lVpfcUse;
int16     _reservePFCISR;
int16     u16Latch;
int16     i16DI_Vac;
int16     i16DI_VacLast;
int16     u16FreqCnt;
int16     u16FreqOut;
Uint16    _u16160usTimer;
Uint16	  _u16MainRly_160usTimer;
Uint16    _u16SoftStartEndTimer;
int16     _i16AdAcMax;
Uint16    _u16PfcQuickLoop;
Uint16    _u16QuickAcLimt;
Uint16	  rsvd66;
Uint16	  _u16ADU1Temp;
Uint16	  _u16ADDCTemp;
Uint16	  _u16ADPFCTemp;
longunion _lU1Temp;
longunion _lDCTemp;
longunion _lPFCTemp;
longunion _lVBATUse;
longunion l32IdcBatDisUse;
int16     _i16AdVBATSam0;
int16     i16AdIdcBatDis;
int16     i16AdIdcBat;
int16     _i16VdcLoopK3;
int16     _i16VdcLoopK4;
int16     _i16CHG_VdcLoopK3;
int16     _i16CHG_VdcLoopK4;
int16     _i16INV_VdcLoopK3;
int16     _i16INV_VdcLoopK4;
};               

extern volatile struct IsrVarsStruct IsrVars;               

#ifdef __cplusplus
}
#endif /* extern "C" */

#endif

