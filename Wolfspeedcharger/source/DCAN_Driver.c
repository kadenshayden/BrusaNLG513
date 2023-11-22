/******************************************************************************
 * DCAN_Driver.c
 *
 *  Created on: 2018Äê6ÔÂ7ÈÕ
 *      Author: Tesla
 *****************************************************************************/


#include "F28x_Project.h"
#include "ChargeCan.h"
#include "DSP280x_device.h"

//#define CAN_MSG_ID_SHIFT        18U

static const uint16_t canBitValues[] =
{
    0x1100, // TSEG2 2, TSEG1 2, SJW 1, Divide 5
    0x1200, // TSEG2 2, TSEG1 3, SJW 1, Divide 6
    0x2240, // TSEG2 3, TSEG1 3, SJW 2, Divide 7
    0x2340, // TSEG2 3, TSEG1 4, SJW 2, Divide 8
    0x3340, // TSEG2 4, TSEG1 4, SJW 2, Divide 9
    0x3440, // TSEG2 4, TSEG1 5, SJW 2, Divide 10
    0x3540, // TSEG2 4, TSEG1 6, SJW 2, Divide 11
    0x3640, // TSEG2 4, TSEG1 7, SJW 2, Divide 12
    0x3740  // TSEG2 4, TSEG1 8, SJW 2, Divide 13
};

//-----Function Declaration-----
void Init_ConfigCAN(void);
void ConfigCAN(void);
void InitECanaIDPointer(void);//InitECanaID
uint32_t setCANBitRate(uint32_t sourceClock, uint32_t bitRate);
bool bGetCANMessage(MsgObjN objNUM, uint16_t messageSize, struct Charge_MESSAGE *pdata);
void vSendCANMessage(MsgObjN objNUM, uint16_t messageSize, struct Charge_MESSAGE *pdata);

void Init_ConfigCAN(void)
{
    GPIO_SetupPinMux(62, GPIO_MUX_CPU1, 6);  //GPIO62 - CANRXA
    GPIO_SetupPinMux(63, GPIO_MUX_CPU1, 6);  //GPIO63 - CANTXA
    GPIO_SetupPinOptions(62, GPIO_INPUT, GPIO_ASYNC);
    GPIO_SetupPinOptions(63, GPIO_OUTPUT, GPIO_PUSHPULL);

	InitCAN();
	ConfigCAN();
	InitECanaIDPointer();

    CanaRegs.CAN_CTL.bit.Init = 0;

}

void InitECanaIDPointer(void)//InitECanaID
{
	Charge_InitID(); //CAN MSG ID
	InitID();
}


void ConfigCAN(void)
{
    //
    // Setup CAN to be clocked off the SYSCLKOUT
    //
    ClkCfgRegs.CLKSRCCTL2.bit.CANABCLKSEL = 0;

    //
    // Set up the bit rate for the CAN bus.  This function sets up the CAN
    // bus timing for a nominal configuration.
    // In this example, the CAN bus is set to 500 kHz.
    //
    // Consult the data sheet for more information about
    // CAN peripheral clocking.
    //
    uint32_t status = setCANBitRate(200000000, 500000); // Original CAN bit rate was 250000 - changed to 500000 to match BMS

    //
    // If values requested are too small or too large, catch error
    //
    if(status == 0)
    {
    	ESTOP0;         // Stop here and handle error
    }

}

//
// setCANBitRate - Set the CAN bit rate based on device clock (Hz)
//                 and desired bit rate (Hz)
//
uint32_t setCANBitRate(uint32_t sourceClock, uint32_t bitRate)
{
#define CAN_MAX_BIT_DIVISOR     (13)   // The maximum CAN bit timing divisor
#define CAN_MIN_BIT_DIVISOR     (5)    // The minimum CAN bit timing divisor
#define CAN_MAX_PRE_DIVISOR     (1024) // The maximum CAN pre-divisor
#define CAN_MIN_PRE_DIVISOR     (1)    // The minimum CAN pre-divisor
#define CAN_BTR_BRP_M           (0x3F)
#define CAN_BTR_BRPE_M          (0xF0000)

    uint32_t desiredRatio;
    uint32_t canBits;
    uint32_t preDivide;
    uint32_t regValue;
    uint16_t canControlValue;

    //
    // Calculate the desired clock rate.
    //
    desiredRatio = sourceClock / bitRate;

    //
    // Make sure that the Desired Ratio is not too large.  This enforces the
    // requirement that the bit rate is larger than requested.
    //
    if((sourceClock / desiredRatio) > bitRate)
    {
        desiredRatio += 1;
    }

    //
    // Check all possible values to find a matching value.
    //
    while(desiredRatio <= CAN_MAX_PRE_DIVISOR * CAN_MAX_BIT_DIVISOR)
    {
        //
        // Loop through all possible CAN bit divisors.
        //
        for(canBits = CAN_MAX_BIT_DIVISOR;
            canBits >= CAN_MIN_BIT_DIVISOR;
            canBits--)
        {
            //
            // For a given CAN bit divisor save the pre divisor.
            //
            preDivide = desiredRatio / canBits;

            //
            // If the calculated divisors match the desired clock ratio then
            // return these bit rate and set the CAN bit timing.
            //
            if((preDivide * canBits) == desiredRatio)
            {
                //
                // Start building the bit timing value by adding the bit timing
                // in time quanta.
                //
                regValue = canBitValues[canBits - CAN_MIN_BIT_DIVISOR];

                //
                // To set the bit timing register, the controller must be
                // placed
                // in init mode (if not already), and also configuration change
                // bit enabled.  The state of the register should be saved
                // so it can be restored.
                //
                canControlValue = CanaRegs.CAN_CTL.all;
                CanaRegs.CAN_CTL.bit.Init = 1;
                CanaRegs.CAN_CTL.bit.CCE = 1;

                //
                // Now add in the pre-scalar on the bit rate.
                //
                regValue |= ((preDivide - 1) & CAN_BTR_BRP_M) |
                            (((preDivide - 1) << 10) & CAN_BTR_BRPE_M);

                //
                // Set the clock bits in the and the bits of the
                // pre-scalar.
                //
                CanaRegs.CAN_BTR.all = regValue;

                //
                // Restore the saved CAN Control register.
                //
                CanaRegs.CAN_CTL.all = canControlValue;

                //
                // Return the computed bit rate.
                //
                return(sourceClock / ( preDivide * canBits));
            }
        }

        //
        // Move the divisor up one and look again.  Only in rare cases are
        // more than 2 loops required to find the value.
        //
        desiredRatio++;
    }
    return 0;
}



//
// setupMessageObject - Setup message object as Transmit or Receive
//
void setupMessageObject(MsgObjN objNUM, uint16_t messageSize, uint32_t msgID, \
										uint32_t msgMaskID, msgObjType msgType)
{
    //
    // Wait for busy bit to clear.
    //
    while (CanaRegs.CAN_IF1CMD.all & 0x8000) {;}


    //
    // Clear and Write out the registers to program the message object.
    //
    CanaRegs.CAN_IF1CMD.all = 0;
    CanaRegs.CAN_IF1MSK.all = 0;
    CanaRegs.CAN_IF1ARB.all = 0;
    CanaRegs.CAN_IF1MCTL.all = 0;

    //
    // Set the Control, Mask, and Arb bit so that they get transferred to the
    // Message object.
    //
    CanaRegs.CAN_IF1CMD.bit.Control = 1;
    CanaRegs.CAN_IF1CMD.bit.Arb = 1;
    CanaRegs.CAN_IF1CMD.bit.Mask = 1;
    CanaRegs.CAN_IF1CMD.bit.DIR = 1;

    if(msgType == MSG_OBJ_TYPE_RECEIVE)
    {
		CanaRegs.CAN_IF1MSK.bit.MXtd = 1;
		CanaRegs.CAN_IF1MSK.bit.Msk = msgMaskID;
		CanaRegs.CAN_IF1MSK.bit.MDir = 1;//
    }
    else
    {
        CanaRegs.CAN_IF1ARB.bit.Dir = 1;
    }
    //
    // Set Message ID
    CanaRegs.CAN_IF1ARB.bit.Xtd = 1;
    CanaRegs.CAN_IF1ARB.bit.ID = msgID;

    //
    // Set the data length since this is set for all transfers.
    CanaRegs.CAN_IF1MCTL.bit.DLC = messageSize;
    //This is also a single transfer and not a FIFO transfer so set EOB bit.
    CanaRegs.CAN_IF1MCTL.bit.EoB = 1; //End of Block.
    if(msgType == MSG_OBJ_TYPE_RECEIVE)
    {
		//CanaRegs.CAN_IF1MCTL.bit.RmtEn = 0;
		CanaRegs.CAN_IF1MCTL.bit.UMask = 1;
    }

    //This bit must be also be reset before ID,Xtd,Dir,DLC are modified.
    CanaRegs.CAN_IF1ARB.bit.MsgVal = 1; //Mailbox enable
    //
    // Transfer data to message object RAM:  4~14 clocks needed.
    // Transfer is started when the CPU writes MSG_NUM.
    CanaRegs.CAN_IF1CMD.bit.MSG_NUM = objNUM;
}


//
// getCANMessage - Check the message object for new data.
//                 If new data, data written into array and return true.
//                 If no new data, return false.
//
__attribute__((ramfunc))
bool bGetCANMessage(MsgObjN objNUM, uint16_t messageSize, struct Charge_MESSAGE *pdata)
{
    bool status;

    //
    // Set the Message Data A, Data B, and control values to be read
    // on request for data from the message object.
    //
    CanaRegs.CAN_IF2CMD.all = 0;
    CanaRegs.CAN_IF2CMD.bit.Control = 1;
    CanaRegs.CAN_IF2CMD.bit.DATA_A = 1;
    CanaRegs.CAN_IF2CMD.bit.DATA_B = 1;
    CanaRegs.CAN_IF2CMD.bit.Arb = 1;

    //
    // Transfer the message object to the message object IF register.
    //
    CanaRegs.CAN_IF2CMD.bit.MSG_NUM = objNUM;

    //
    // Wait for busy bit to clear.
    //
    while (CanaRegs.CAN_IF2CMD.all & 0x8000) {;}//CanaRegs.CAN_IF2CMD.bit.Busy

    //
    // See if there is new data available.
    //
    if(CanaRegs.CAN_IF2MCTL.bit.NewDat == 1)
    {
    	pdata->id.all = CanaRegs.CAN_IF2ARB.bit.ID;
        //
        // Read out the data from the CAN registers.
        //
        uint16_t index;
        for(index = 0; index < messageSize; index++)
        {
            switch(index)
            {
                case 0:
                	pdata->DataAll.MBoxDataBits.MDL.byte.BYTE0 = \
										CanaRegs.CAN_IF2DATA.bit.Data_0;
                break;
                case 1:
                	pdata->DataAll.MBoxDataBits.MDL.byte.BYTE1 = \
										CanaRegs.CAN_IF2DATA.bit.Data_1;
                break;
                case 2:
                	pdata->DataAll.MBoxDataBits.MDL.byte.BYTE2 = \
										CanaRegs.CAN_IF2DATA.bit.Data_2;
                break;
                case 3:
                	pdata->DataAll.MBoxDataBits.MDL.byte.BYTE3 = \
										CanaRegs.CAN_IF2DATA.bit.Data_3;
                break;
                case 4:
                	pdata->DataAll.MBoxDataBits.MDH.byte.BYTE4 = \
										CanaRegs.CAN_IF2DATB.bit.Data_4;
                break;
                case 5:
                	pdata->DataAll.MBoxDataBits.MDH.byte.BYTE5 = \
										CanaRegs.CAN_IF2DATB.bit.Data_5;
                break;
                case 6:
                	pdata->DataAll.MBoxDataBits.MDH.byte.BYTE6 = \
										CanaRegs.CAN_IF2DATB.bit.Data_6;
                break;
                case 7:
                	pdata->DataAll.MBoxDataBits.MDH.byte.BYTE7 = \
										CanaRegs.CAN_IF2DATB.bit.Data_7;
                break;
            }
        }

        //
        // Clear New Data Flag
        //
        CanaRegs.CAN_IF2CMD.bit.TxRqst = 1;

        //
        // Wait for busy bit to clear.
        //busy bit = bit15
        while (CanaRegs.CAN_IF2CMD.all & 0x8000) {;}

        //
        // Transfer the message object to the message object IF register.
        //
        CanaRegs.CAN_IF2CMD.bit.MSG_NUM = objNUM;

        status = true;
    }
    else
    {
        status = false;
    }

    return(status);
}


//
// sendCANMessage - Transmit data from the specified message object
//
__attribute__((ramfunc))
void vSendCANMessage(MsgObjN objNUM, uint16_t messageSize, struct Charge_MESSAGE *pdata)
{
    //
    // Wait for busy bit to clear.
    //
    while (CanaRegs.CAN_IF1CMD.all & 0x8000) {;}

    CanaRegs.CAN_IF1ARB.all = pdata->id.all;
    CanaRegs.CAN_IF1MCTL.all = 0x80|messageSize;
    //
    // Write data to transfer into DATA-A and DATA-B interface registers
    //
    uint16_t index;
    for(index = 0; index < messageSize; index++)
    {
        switch(index)
        {
            case 0:
                CanaRegs.CAN_IF1DATA.bit.Data_0 = \
							pdata->DataAll.MBoxDataBits.MDL.byte.BYTE0;
                break;
            case 1:
                CanaRegs.CAN_IF1DATA.bit.Data_1 = \
							pdata->DataAll.MBoxDataBits.MDL.byte.BYTE1;
                break;
            case 2:
                CanaRegs.CAN_IF1DATA.bit.Data_2 = \
							pdata->DataAll.MBoxDataBits.MDL.byte.BYTE2;
                break;
            case 3:
                CanaRegs.CAN_IF1DATA.bit.Data_3 = \
							pdata->DataAll.MBoxDataBits.MDL.byte.BYTE3;
                break;
            case 4:
                CanaRegs.CAN_IF1DATB.bit.Data_4 = \
                			pdata->DataAll.MBoxDataBits.MDH.byte.BYTE4;
                break;
            case 5:
                CanaRegs.CAN_IF1DATB.bit.Data_5 = \
    						pdata->DataAll.MBoxDataBits.MDH.byte.BYTE5;
                break;
            case 6:
                CanaRegs.CAN_IF1DATB.bit.Data_6 = \
    						pdata->DataAll.MBoxDataBits.MDH.byte.BYTE6;
                break;
            case 7:
                CanaRegs.CAN_IF1DATB.bit.Data_7 = \
							pdata->DataAll.MBoxDataBits.MDH.byte.BYTE7;
                break;
        }
    }
    CanaRegs.CAN_IF1CMD.all = 0x00B70000|objNUM;
}

