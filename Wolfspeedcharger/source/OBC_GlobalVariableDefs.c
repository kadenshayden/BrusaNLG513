/*****************************************************************************
 *
 *  FILE:    OBC_GlobalVariableDefs.c
 *
 *   TITLE:   OBC Global Variables and Data Section Pragmas.
 *
*****************************************************************************/
#include <Shared.h>
//---------------------------------------------------------------------------
// Define Global Peripheral Variables:
//
//-------------------------------------------------
#pragma DATA_SECTION(ClatoCpuVar,"Cla1ToCpuMsgRAM");
volatile struct ClatoCpuVarStruct ClatoCpuVar;
//
//----------------------------------------
#pragma DATA_SECTION(CputoClaVar,"CpuToCla1MsgRAM");
volatile struct CputoClaVarStruct CputoClaVar;

//----------------------------------------
#pragma DATA_SECTION(IsrVars,"IsrVariableFile");
volatile struct IsrVarsStruct IsrVars;

#pragma DATA_SECTION(Boot_Version,".bootvec")
const int16 Boot_Version[2] = {0x100, 0x100};

ubitintb	g_u16MdlStatus;
ubitinte	g_u16MdlStatusH;

ubitfloat g_lq10MdlVoltUp;
ubitfloat g_lq10DCTempUp;
ubitfloat g_lq10PFCTempUp;
UINT16 g_u16ActionReady;

ubitfloat g_lq10AcVrmsSampSysa;
ubitfloat g_lq10AcVrmsSampSysb;
ubitfloat g_lq10VpfcSampSys;
ubitfloat g_lq10VpfcConSys;
ubitfloat g_lq12VoltSampSysa;
ubitfloat g_lq12VoltSampSysb;
ubitfloat g_lq12VoltConSysa;
ubitfloat g_lq12VoltConSysb;
ubitfloat g_lq10CurrSampSysa;
ubitfloat g_lq10CurrSampSysb;
ubitfloat g_lq10CurrConSysa;
ubitfloat g_lq10CurrConSysb;
ubitfloat g_lq10PowerConSysa;
ubitfloat g_lq10PowerConSysb;


ubitfloat g_lq10SetVolt;
ubitfloat g_lq10SetLimit;
ubitfloat g_lq10SetPower;
ubitinta  g_u16MdlCtrl;

UINT16 g_u16AddIdentifyFlag;
UINT16 g_u16AddIdentifyNum;

UINT16 g_u16VersionNoHw;
UINT16 g_u16VersionNoSw;

//===========================================================================
// End of file.
//===========================================================================

