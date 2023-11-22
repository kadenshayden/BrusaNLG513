//###########################################################################
//
// FILE:   F28x_Project.h
//
// TITLE:  F28x Project Headerfile and Examples Include File
//
//###########################################################################
// $TI Release: F2837xD Support Library v3.04.00.00 $
// $Release Date: Sun Mar 25 13:26:04 CDT 2018 $
// $Copyright:
// Copyright (C) 2013-2018 Texas Instruments Incorporated - http://www.ti.com/
//
// Redistribution and use in source and binary forms, with or without 
// modification, are permitted provided that the following conditions 
// are met:
// 
//   Redistributions of source code must retain the above copyright 
//   notice, this list of conditions and the following disclaimer.
// 
//   Redistributions in binary form must reproduce the above copyright
//   notice, this list of conditions and the following disclaimer in the 
//   documentation and/or other materials provided with the   
//   distribution.
// 
//   Neither the name of Texas Instruments Incorporated nor the names of
//   its contributors may be used to endorse or promote products derived
//   from this software without specific prior written permission.
// 
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS 
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT 
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT 
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, 
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT 
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT 
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE 
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
// $
//###########################################################################

#ifndef F28X_PROJECT_H
#define F28X_PROJECT_H

//
// Included Files
//
#include "A_constant.h"
#include "A_macro.h"
#include "F2837xD_Cla_typedefs.h"  // F2837xD CLA Type definitions
#include "F2837xD_device.h"        // F2837xD Headerfile Include File---外设.h
#include "SFO_V8.h" //SFO()计算HRMSTEP时需要该头文件及库文件。否则不需要
#include "F2837xD_Examples.h"      // F2837xD Examples Include File ---defines.h
#include "DCL.h"					//floating-point Digital control library
#include "ExternLines.h"
#include "Inverter_Base.h"

#define EMAVG_MACRO(in, out, multiplier) out = ((in - out)*multiplier) + out;

typedef unsigned short		UINT16;
typedef short				INT16;
typedef long        		INT32;
typedef unsigned long   	UINT32;
typedef float           	FLOAT32;
typedef long double     	FLOAT64;


//---------------------Struct ubitfloat definition-------------------------------
struct intData
{
	int iLD;
	int iHD;
};
typedef struct intData intStructData;
typedef	union
{
	int32 lData;
	intStructData iData;
}longunion;

typedef	union
{
	unsigned int id ;
	struct   packed_data
	{
		 unsigned highchar : 8;
		 unsigned lowchar  : 8;
	}bitdata;
}ubitint ;


typedef	union
{
	float fd ;
	ubitint intdata[2];
	long	lData;
	intStructData iData;
}ubitfloat ;


struct MAIN_TIMER
{//including  CPU1 and CPU2 if possible
	Uint16 u16Calc;
	Uint16 u16Warn;
	Uint16 u16Adc;
	Uint16 u16Tx3SFlag;	//3s
};
extern struct MAIN_TIMER MTimer;



#endif  // end of F28X_PROJECT_H definition

//
// End of file
//
