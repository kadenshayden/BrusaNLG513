/*
 * F2837xD_eCap_defines.h
 *
 *  Created on: 2018Äê9ÔÂ6ÈÕ
 *      Author: hasee
 */

#ifndef INCLUDE_F2837XD_ECAP_DEFINES_H_
#define INCLUDE_F2837XD_ECAP_DEFINES_H_

	// ECCTL1 (ECAP Control Reg 1)
	//==========================
	// CAPxPOL bits
	#define EC_RISING 0x0
	#define EC_FALLING 0x1
	// CTRRSTx bits
	#define EC_ABS_MODE 0x0 //Don't reset counter on Capture Eventx (absolute time stamp)
	#define EC_DELTA_MODE 0x1 //Reset counter after Eventx time-stamp has been captured (used in difference mode operation)
	// PRESCALE bits
	#define EC_BYPASS 0x0 //Divide by 1 (i.e,. no prescale, by-pass the prescaler)
	#define EC_DIV1 0x0 //Divide by 2
	#define EC_DIV2 0x1
	#define EC_DIV4 0x2
	#define EC_DIV6 0x3
	#define EC_DIV8 0x4 //Divide by 8
	#define EC_DIV10 0x5 //Divide by 10
	// ECCTL2 ( ECAP Control Reg 2)
	//==========================
	// CONT/ONESHOT bit
	#define EC_CONTINUOUS 0x0 //Operate in continuous mode
	#define EC_ONESHOT 0x1 //Operate in one-Shot mode
	// STOPVALUE bit
	#define EC_EVENT1 0x0 //Stop after Capture Event 1 in one-shot mode;Wrap after Capture Event 1 in continuous mode.
	#define EC_EVENT2 0x1 //Stop after Capture Event 2 in one-shot mode;Wrap after Capture Event 2 in continuous mode.
	#define EC_EVENT3 0x2 //Stop after Capture Event 3 in one-shot mode;Wrap after Capture Event 3 in continuous mode.
	#define EC_EVENT4 0x3 //Stop after Capture Event 4 in one-shot mode;Wrap after Capture Event 4 in continuous mode.
	 // RE-ARM bit
	#define EC_ARM 0x1//Arms the one-shot sequence as follows:
					  //1) Resets the Mod4 counter to zero
					  //2) Unfreezes the Mod4 counter
					  //3) Enables capture register loads
	// TSCTRSTOP bit:Time Stamp (TSCTR) Counter Stop (freeze) Control
	#define EC_FREEZE 0x0
	#define EC_RUN 0x1
	// SYNCO_SEL bit
	#define EC_SYNCIN 0x0
	#define EC_CTR_PRD 0x1
	#define EC_SYNCO_DIS 0x2
	// CAP/APWM mode bit
	#define EC_CAP_MODE 0x0	//- Inhibits(½ûÖ¹) TSCTR resets via CTR = PRD event
							//- Inhibits(½ûÖ¹) shadow loads on CAP1 and 2 registers
							//- Permits user to enable CAP1-4 register load
							//- CAPx/APWMx pin operates as a capture input
	#define EC_APWM_MODE 0x1	//- Resets TSCTR on CTR = PRD event (period boundary
								//- Permits shadow loading on CAP1 and 2 registers
								//- Disables loading of time-stamps into CAP1-4 registers
								//- CAPx/APWMx pin operates as a APWM output
	// APWMPOL bit
	#define EC_ACTV_HI 0x0 //Output is active high (Compare value defines high time)
	#define EC_ACTV_LO 0x1 //Output is active low (Compare value defines low time)


	// Generic
	#define EC_DISABLE 0x0
	#define EC_ENABLE 0x1
	#define EC_FORCE 0x1


#endif /* INCLUDE_F2837XD_ECAP_DEFINES_H_ */
