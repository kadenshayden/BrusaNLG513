/******************************************************************************
 *
 * main.c
 *
 *  Created on: 2018/02/24
 *
******************************************************************************/

#include "F28x_Project.h"
#include "F2837xD_Ipc_drivers.h"
#include "DSP280x_device.h"
#include "ChargeCan.h"

struct MAIN_TIMER MTimer;
ubitintc	g_u16RunFlag;

// GSeibt
// Added for Global Heartbeat counter.
struct MAIN_TIMER HeartbeatTimer;
bool  g_BoolGotHeartbeat;
UINT16  g_u16HeartBeatMissingCounter = 0;
bool  g_BoolChargerIsActive = 0;


//  Display Values, float in ISO Units
//volatile float guiVbus, guiVbusMax;
//volatile float guiACFreq;
//volatile float guiPrms;
//volatile float guiIrms;
//volatile float guiVrms;
//volatile float guiVavg;
//volatile float guiIi;
//volatile float guiIo;
//volatile float guiPowerFactor;
//volatile float guiVA;
//volatile float guiFreqAvg;
//volatile float guiVema;

// Main
//
void main(void)
{
    InitSysCtrl();      // System configuration (CPUCLK = 200MHz, SYSCLK = 100MHz)

    InitGpio();         // Initialize GPIO (General Purpose Input/Output)

    DINT;

    InitPieCtrl();   // Initialize Pie Control

    IER = 0x0000;	 // Reset Interrupt Enable Register
    IFR = 0x0000;	 // Reset Interrupt Flag Register

    InitPieVectTable();

    vInitialInterrupts();

    InitRAM();         // Initialize RAM
	EnableDog();       // Initialize watchdog timer
	ServiceDog();      // Feed the dog to avoid resets on the system
    InitADC();         // Initialize ADC (Analog to Digital Conversion)
    InitI2C();         // Initialize I2C (Inter-Integrated Circuit) Communication Protocol
    InitCMPSS();       // Initialize Comparator Subsystem
    InitEPWM();        // Initialize enhanced Pulse Width Modulator
    Init_ConfigCAN();  // Initialize CAN
    InitCpuTimers();   // Base
    InitECap();        //
    InitPowerSupply(); // Ecap soft-start
	vCLATaskinit();


 	vDataInit();            //variable initialization
 	                        //limit variable and converter information initialization
 	vLimitInit();           //ReadEEPROM
	vCanBufferInitial();
	Charge_sCanBufferInitial();
 	vIsr_init();            //
 	vCputoClaVar_init();    //cla

    MyEnableInterrupts();

    HeartbeatTimer.u16Tx3SFlag = 0;

    // Enable global Interrupts and higher priority real-time debug events:
    EINT;  // Enable Global interrupt INTM
    ERTM;  // Enable Global realtime interrupt DBGM

// IDLE loop. Just sit and loop forever (optional):
    for(;;)
    {
    	vStartTask();			// start control
    	                        //  Responsible for initalization and start of HW/SW power circuits.

		vScheduler();			// timer - Starts watchdog and monitors faults

		// vIPCProcess();       // Ignore, not used

		vWarnTask();			// IO samp and control - Responsible for monitor
		                        //  Manages tasks that monitor charging stats (overvolt, relay, SR, temps, PFC)

		Charge_main();          // Manage CAN, also sends warnings/errors over CAN

		vAdcTask();		        // AD sample and calculation (cal PFC,DCDC,and PFC voltages & filter. also manages temperature checking)

		vRefTask();			    // voltage reference,curr,power calculation
    }
}

/* ****************************************************************************************************************************
*******************************************************************************************************************************
* \ Scheduler which flags other tasks (vWarnTask(), Charge_main(), vAdcTask() and vRefTask()) to run their dedicated functions
* \ It turns on or off (1 or 0) the flag bits of WARN, CAL, ADC ...
* \ param none
* \ return none  (because it is a void function)
* ************************************************************************************************************************** */
void vScheduler(void)
{
	// 5mS flag
	if (mGet5msTiming())
	{   // 5ms timer interrupt fired
		mClr5msTimFlag();   // Reset the flag interrupt flag

		// avoid watchdog reset, feed dog
		WatchDogKick();

		if (++MTimer.u16Calc >= TIME_MT_115MS)
		{   // It's time to launch energy control algorithms in vRefTask() (once in 115 ms)
			MTimer.u16Calc = 0;
			g_u16RunFlag.bit.CAL = 1;
		}


		if (++MTimer.u16Warn >= TIME_MT_10MS)
		{   // It's time to check parameters and faults of the device (vWarnTask())
			MTimer.u16Warn = 0;
			g_u16RunFlag.bit.WARN = 1;
		}

		if (++MTimer.u16Adc  >=  TIME_MT_20MS)
		{   // It's time to calculate parameters from the acquired data (vAdcTask())
			MTimer.u16Adc = 0;
			g_u16RunFlag.bit.ADC = 1;
		}
		

		//  Check if charger is in "charging" state - KG
		// Todo: adjust timing of CAN messages so that it doesn't constantly shut off.
		//      Messages from OBC happen every 3 seconds, BMS might be similar. so 500 ms is too small.
		if (g_BoolChargerIsActive == 1)
		{
		    // check here every 500 ms for last CAN Message
		    if (++HeartbeatTimer.u16Tx3SFlag >= TIME_MT_500MS)
		    {
            // Got a heart beat in the last 500ms
            if (g_BoolGotHeartbeat)
            {
                // Heartbeat was detected in the last 500ms
                g_u16HeartBeatMissingCounter = 0;
                HeartbeatTimer.u16Tx3SFlag = TIME_MT_500MS; // To avoid overrun
            }
            else
            {   // No Heartbeat in the last 500ms
                // So increment the counter by one.
                g_u16HeartBeatMissingCounter++;
                // Reset Timer
                HeartbeatTimer.u16Tx3SFlag = 0;
            }
            g_BoolGotHeartbeat = false;
        }
            // Missed 3 consecutive Heartbeat so we will stop the Charger to avoid anything bad.
            if (g_u16HeartBeatMissingCounter >= 3)
            {
                    g_u16MdlCtrl.bit.SHORTRELAY = 1;    // PriSide/Sec side OFF - Will also shut off main relay
                    g_u16MdlCtrl.bit.OFFCTRL = 1;       // Might not be necessary
                    g_u16HeartBeatMissingCounter = 0;
                    g_BoolChargerIsActive = 0;
            }
        }

        if (++MTimer.u16Tx3SFlag >= TIME_MT_3000MS)
        {   // It's time to prepare and transmit some data (Charge_main())
            //  The details of the transmission process are not clear as there are no sources for that, only binary library
            //  If there's an issue here, its going to be very difficult to debug.
            MTimer.u16Tx3SFlag = 0;
            g_u16RunFlag.bit.Tx3SFlag = 1;
        }
	}
}


//
// End of file
//
