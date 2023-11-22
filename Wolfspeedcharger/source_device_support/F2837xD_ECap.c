//###########################################################################
//
// FILE:   F2837xD_ECap.c
//
// TITLE:  F2837xD eCAP Initialization & Support Functions.
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

//
// Included Files
//
#include "F2837xD_device.h"
#include "F2837xD_Examples.h"

//
// InitECap - This function initializes the eCAP(s) to a known state.
//
void InitECap(void)
{
	EALLOW;
	//
	// Setup APWM mode on CAP1, set period and compare registers
	//
	ECap1Regs.ECCTL2.bit.CAP_APWM = 1;      // Enable APWM mode
	ECap1Regs.ECCTL2.bit.APWMPOL = 0;      // CMP defines high time
	ECap1Regs.CAP1 = 2000;            // Set Period value 100Khz
	ECap1Regs.CAP2 = 0;            // Set Compare value
	ECap1Regs.ECCLR.all = 0x0FF;            // Clear pending __interrupts
	ECap1Regs.ECEINT.all = 0;    // Disable Int

	ECap1Regs.ECCTL2.bit.SYNCO_SEL = 1;//CTR=PRD Í¬²½


	//
	// Setup APWM mode on CAP2, set period and compare registers
	//
	ECap2Regs.ECCTL2.bit.CAP_APWM = 1;      // Enable APWM mode
	ECap2Regs.ECCTL2.bit.APWMPOL = 0;      // CMP defines high time
	ECap2Regs.CAP1 = 2000;            // Set Period value 100Khz
	ECap2Regs.CAP2 = 0;            // Set Compare value
	ECap2Regs.ECCLR.all = 0x0FF;            // Clear pending __interrupts
	ECap2Regs.ECEINT.all = 0;    // Disable Int

	ECap2Regs.CTRPHS = 1; // one clock compensation
	ECap2Regs.ECCTL2.bit.SYNCO_SEL = 2;//disable syn output
	ECap2Regs.ECCTL2.bit.SYNCI_EN = 1;

	//
	// Start counters
	//
	ECap2Regs.ECCTL2.bit.TSCTRSTOP = 1;               //0;//stop
	ECap1Regs.ECCTL2.bit.TSCTRSTOP = 1; //free runing
	ECap1Regs.ECCTL2.bit.SWSYNC = 1;

	EDIS;
}

//
// InitECapGpio - This function initializes GPIO pins to function as ECAP pins
//                Each GPIO pin can be configured as a GPIO pin or up to 3
//                different peripheral functional pins. By default all pins
//                come up as GPIO inputs after reset.
//                Caution:
//                For each eCAP peripheral
//                Only one GPIO pin should be enabled for ECAP operation.
//                Comment out other unwanted lines.
//
void InitECapGpio()
{
	InitAPwm1Gpio();

	//todo  add GPIO config for Vds detect
//   InitECap4Gpio(28);
//   GPIO_SetupPinOptions(28, GPIO_INPUT, GPIO_ASYNC);
//   InitECap4Gpio(29);
//   GPIO_SetupPinOptions(29, GPIO_INPUT, GPIO_ASYNC);
}

//
// InitECap1Gpio - Initialize ECAP1 GPIOs
//
void InitECap1Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT7SELECT = pin;         // Set eCAP1 source to GPIO-pin
    EDIS;
}

//
// InitECap2Gpio - Initialize ECAP2 GPIOs
//
void InitECap2Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT8SELECT = pin;         // Set eCAP2 source to GPIO-pin
    EDIS;
}

//
// InitECap3Gpio - Initialize ECAP3 GPIOs
//
void InitECap3Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT9SELECT = pin;         // Set eCAP3 source to GPIO-pin
    EDIS;
}

//
// InitECap4Gpio - Initialize ECAP4 GPIOs
//
void InitECap4Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT10SELECT = pin;         // Set eCAP4 source to GPIO-pin
    EDIS;
}

//
// InitECap5Gpio - Initialize ECAP5 GPIOs
//
void InitECap5Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT11SELECT = pin;         // Set eCAP5 source to GPIO-pin
    EDIS;
}

//
// InitECap6Gpio - Initialize ECAP6 GPIOs
//
void InitECap6Gpio(Uint16 pin)
{
    EALLOW;
    InputXbarRegs.INPUT12SELECT = pin;         // Set eCAP6 source to GPIO-pin
    EDIS;
}

//
// InitAPwm1Gpio - Initialize EPWM1 GPIOs
//
void InitAPwm1Gpio()
{
    EALLOW;
    OutputXbarRegs.OUTPUT1MUX0TO15CFG.bit.MUX0 = 3; // Select ECAP1.OUT on Mux0
    OutputXbarRegs.OUTPUT1MUXENABLE.bit.MUX0 = 1;  // Enable MUX0 for ECAP1.OUT

    OutputXbarRegs.OUTPUT2MUX0TO15CFG.bit.MUX2 = 3; // Select ECAP2.OUT on Mux2
    OutputXbarRegs.OUTPUT2MUXENABLE.bit.MUX2 = 1;  // Enable MUX0 for ECAP1.OUT

    GpioCtrlRegs.GPAQSEL2.bit.GPIO24 = 0; //Syn to CLK
    GpioCtrlRegs.GPAQSEL2.bit.GPIO25 = 0; //Syn to CLK

    GpioCtrlRegs.GPAPUD.bit.GPIO24 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO24 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO24 = 1;    // Select OUTPUTXBAR1 on GPIO24

    GpioCtrlRegs.GPAPUD.bit.GPIO25 = 1;
    GpioCtrlRegs.GPAGMUX2.bit.GPIO25 = 0;
    GpioCtrlRegs.GPAMUX2.bit.GPIO25 = 1;    // Select OUTPUTXBAR2 on GPIO25
    EDIS;
}

//
// End of file
//
