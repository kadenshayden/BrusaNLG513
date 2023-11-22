#ifndef SHARED_H
#define SHARED_H

#ifdef __cplusplus
extern "C" {
#endif

#include "DSP280x_Device.h"

// char and short are identical to each other
typedef short				cla_16bits_s;
typedef char				cla_16bits_c;
typedef unsigned short		cla_16bits_us;
typedef unsigned char		cla_16bits_uc;
typedef long           		cla_32bits_l;

typedef struct
{
	cla_16bits_s iLD;
	cla_16bits_s iHD;
}
cla_16bits_strucData;

typedef	union
{
	cla_32bits_l lData;
	cla_16bits_strucData iData;
}CLAlongunion;



struct CputoClaVarStruct
{
float32	  _f32VpfcFilterK2;              
float32   _f32VpfcFilterK1;
float32   _f32VpfcLoopKp;
float32   _f32VpfcLoopKi;
float32   rsvd08;
cla_16bits_s     _iVpfcSet;
cla_16bits_us    u16PfcOpened;
float32		f32VacRmsSqrInv;
cla_16bits_s     _iIpfcOffset;
cla_16bits_us    _u16PfcLoopCount;
float32   rsvd10;
float32   rsvd12;
float32   rsvd14;
float32   _f32PfcDutyPermit;
float32   rsvd18;
float32   rsvd1A;
float32		rsvd1C;
float32		f32VpfcFilterDis;
int32 	  rsvd20;
float32   _f32PiDynTempFilter1;
float32   _f32PiDynTempFilter2;
float32   _f32DynIdcFilter1;
float32   _f32DynIdcFilter2;
float32		f32AdVdcDis;
cla_16bits_us    rsvd21;
cla_16bits_us    _uiPermitOverAcFlag;
cla_16bits_us    _uiOverAcVolt;
cla_16bits_us    _uiLowPfcVolt;
cla_16bits_us   u16WorkMode;
cla_16bits_us	 rsvd31;
float32		_f32IpfcSet;
Uint32		u32Theta;
float32		f32Sin_Theta;
cla_16bits_s i16Polarity;

};               
extern volatile struct CputoClaVarStruct CputoClaVar;

//CLA to CPU RAM 0x1480-0x14FF
struct ClatoCpuVarStruct
{
float32   _f32AdVpfc0;         
float32   _f32AdVpfc1;               
float32   _f32VpfcUse;       
float32   _f32VpfcErr0;
float32   _f32VpfcErr1;              
float32   _f32VpfcPiOut;  
float32   _f32VpfcPiTmp;
float32   _f32AdAcVolt;
cla_16bits_s     _iVpfcProtect;
cla_16bits_us	 _u16PfcDutyMin;
float32		f32CurrErrorSign;//;
float32   _f32IpfcSet;     
float32   _f32AdIpfc;              
float32   _f32IpfcErr0;          
float32   _f32IpfcErr1;
float32   _f32IpfcErrUse0;
float32   _f32IpfcErrUse1;           
float32   _f32IpfcLoopK3;      
float32   _f32IpfcLoopK4;
float32   _f32IpfcPiOut;
float32   _f32AcSamVoltSquare0;
float32   _f32AcSamVoltSquare1;
float32   _f32AcVoltSquareFilt;   
float32   _f32AcVoltSquareFilt1; 
float32	  _f32AcVoltSquareRms;
float32   f32Mod_Inv;//unused
float32   f32SoftStartCtrl;
float32   _f32VpfcProtectTmp;
float32   f32IpfcPiOutUse;
float32   _f32VpfcPara;
float32   _f32PfcQuickLoop;
float32   f32Rsvd_BE;//f32VpfcQn;
float32   f32VpfcFilterDis;
float32   _f32DynIdcUse;
float32   _f32IdcSample0;
float32   _f32IdcSample1;
float32   _f32Idc0;
float32   _f32Idc1;
float32   _f32Idc2;
float32   _f32Idc3;
float32   _f32Idc4;
float32   _reservePFCISR;
float32   _f32Rsvd4; //used in CLA
float32   _f32Rsvd5;
int32	  rsvd5x;
float32   _f32LoadDynTimer;
float32   _f32SwitchCnt;
float32   f32IpfcPi_P;
float32   f32IpfcPi_I;
float32   f32PfcQuickLoopForce;
float32   _f32VacRmsSqrInvCla;
float32   _f32AdVpfcOVP0;
float32   _f32AdVpfcOVP1;
float32   f32VpfcPiTmpOld;
float32   _f32PiDynTmpUse;
float32   _f32PioutUse;
float32   f32Flag;
};               
extern volatile struct ClatoCpuVarStruct ClatoCpuVar;  


#ifdef __cplusplus
}
#endif


#endif
