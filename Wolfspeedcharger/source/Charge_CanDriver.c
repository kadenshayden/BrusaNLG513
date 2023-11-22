/******************************************************************************
 *
 *
 * Charge_CanDriver.c
 * Structs and drivers for CAN. Simple mailbox, send, receive, etc... functions
 *
 *
  ****************************************************************************/


/*=============================================================================*
 *============================================================================*/
#include "DSP280x_Device.h"		// DSP280x Headerfile Include File
#include "ChargeCan.h"

#define Charge_CAN_RXBUF_SIZE	 15
#define Charge_CAN_TXBUF_SIZE	 15

//-------Module related


//-----Variables defined out of this file-------------
UINT16	g_u16MdlRunMode;


/*******************************************************************************
*variables definition
*******************************************************************************/
//variables for can data read and write
static struct Charge_MESSAGE		Charge_s_aCanRxBuf[Charge_CAN_RXBUF_SIZE];
static struct Charge_MESSAGE 		Charge_s_aCanTxBuf[Charge_CAN_TXBUF_SIZE];
static struct Charge_MESSAGE*	Charge_s_pCanRxIn;
static struct Charge_MESSAGE*	Charge_s_pCanRxOut;
static struct Charge_MESSAGE*	Charge_s_pCanTxIn;
static struct Charge_MESSAGE*	Charge_s_pCanTxOut;
static UINT16   Charge_s_u16CanRxLength;
static UINT16   Charge_s_u16CanTxLength;
//---------------------
//variables for can data decode and encode
struct Charge_MESSAGE  Charge_g_stCanTxData;
struct Charge_MESSAGE  Charge_g_stCanRxData;
union Charge_MSGID Charge_ID;

//static struct ECAN_REGS s_ECanaShadow;

//-------Module ralted
UINT16	g_u16GroupSaved;
/*******************************************************************************
*functions declare                                      
*******************************************************************************/
/*******************************************************************************
*Description:      initial can receive and transmit buffer                           
*******************************************************************************/
void Charge_InitID(void)
{
	setupMessageObject(Msg3, 8, 0x18A0E5F4, 0x1F00FFFF, MSG_OBJ_TYPE_RECEIVE); //MBX2 receive ID

	setupMessageObject(Msg4, 8, REPLYID_STATUS_B3, 0, MSG_OBJ_TYPE_TRANSMIT);	//MBX3 send ID
}
void Charge_sCanBufferInitial(void)
{
	Charge_s_pCanRxIn = Charge_s_aCanRxBuf;
	Charge_s_pCanRxOut = Charge_s_aCanRxBuf;
	Charge_s_pCanTxIn = Charge_s_aCanTxBuf;
	Charge_s_pCanTxOut = Charge_s_aCanTxBuf;
	Charge_s_u16CanTxLength = 0;
	Charge_s_u16CanRxLength = 0;
}

/*******************************************************************************
*Description:      Read message from the mailbox                             
*******************************************************************************/
void Charge_sCanHdRead2(struct Charge_MESSAGE *pdata)
{
	bGetCANMessage(Msg3, 8, pdata);
}

/*******************************************************************************
*Description:      transmit message to the mailbox                           
*******************************************************************************/
void    Charge_sCanHdSend3(struct Charge_MESSAGE *pdata)
{
	vSendCANMessage(Msg4, 8, pdata);
}

/*******************************************************************************
*Function name:		Charge_sCanCopy
*Parameters:		pSource:pointer to the source data						  
*					pDestination:pointer to the destination data			  
*Description:		This fucntion copy the content of psource to pDestination 
*******************************************************************************/
static void	Charge_sCanCopy(struct Charge_MESSAGE *pSource,struct Charge_MESSAGE *pDestination)
{
	pDestination->id.all = pSource->id.all;

	pDestination->DataAll.MBoxDataBits.MDL.all = pSource->DataAll.MBoxDataBits.MDL.all;
	pDestination->DataAll.MBoxDataBits.MDH.all = pSource->DataAll.MBoxDataBits.MDH.all;
}

/*******************************************************************************
*Function name: sCanRxISR											  
*Parameters:		none													  
*Description:		This fucntion should be executed in the can receiving 	  
*					interrupt,it save the received data to the queue 
*******************************************************************************/
void Charge_sCanRxISR2(void)
{	
    if(Charge_s_u16CanRxLength == Charge_CAN_RXBUF_SIZE)  //Buffer full
	{
		return;
	}
	Charge_sCanHdRead2(Charge_s_pCanRxIn); //�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�7�1�7�1�71�1�77�1�71�1�77�1�71�1�77�1�70�1�72�1�71�1�77�1�71�1�77�1�71�1�77�1�70�1�72�1�71�1�77�1�71�1�77�1�7�0�01�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�7�1�7�1�70�1�72�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�70�1�78�1�71�1�77�1�71�1�77�1�71�1�77
	// Orif Negmatov comment
	//else  // I added "else" here. I was confused that there was no if/else statement, but the 2 curly brackets were present. I assume it should be if else statement. If buffer is full, return. If not full, run other commands.
	{
		if((++Charge_s_pCanRxIn) > &Charge_s_aCanRxBuf[Charge_CAN_RXBUF_SIZE - 1])//�1�70�1�78�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�70�1�75�1�71�1�77�1�71�1�77
		{
			Charge_s_pCanRxIn = Charge_s_aCanRxBuf;//�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�70�1�78�1�71�1�77�1�71�1�77
		}
		Charge_s_u16CanRxLength++;
	}	
}

/*******************************************************************************
*Function name:		Charge_sCanRead
*Parameters:		pdata:the address where to save the data				  
*Returns:			CAN_RXBUF_EMPTY:there is no data in the receive buffer	  
*					CAN_RXBUF_RDY:data is read successdully from buffer		  
*Description:		This fucntion get the data from receiving buffer and save 
*					to user's buffer.it should be executed in app			  
*******************************************************************************/
UINT16	Charge_sCanRead(struct Charge_MESSAGE *pdata)
{
	if(Charge_s_u16CanRxLength == 0)  // If CanRx (Receive data) is Empty, return Rx Buffer is empty
	{
		return(CAN_RXBUF_EMPTY);
	}
		
	Charge_sCanCopy(Charge_s_pCanRxOut,pdata);
	
	Charge_s_u16CanRxLength--;
	
	if((++Charge_s_pCanRxOut) > &Charge_s_aCanRxBuf[Charge_CAN_RXBUF_SIZE - 1])
	{
		Charge_s_pCanRxOut = Charge_s_aCanRxBuf;
	}
	
	return(CAN_RXBUF_RDY);    // Receive Buffer is Ready
}

/********************************************************************************
*Function name:		sCanTxISR4													
*Parameters:		none														
*Description:		This function should be executed in the can transmit
*					interrupt,it changes the status of transmission
*********************************************************************************/
void Charge_sCanTxISR3(void)
{
	if(Charge_s_u16CanTxLength >= 1)
	{
		Charge_sCanHdSend3(Charge_s_pCanTxOut);
		Charge_s_u16CanTxLength--;//move here to guarantee 1s interval

		if((++Charge_s_pCanTxOut) > &Charge_s_aCanTxBuf[Charge_CAN_TXBUF_SIZE - 1])
		{
			Charge_s_pCanTxOut = Charge_s_aCanTxBuf;
		}
	}
}

/********************************************************************************
*Function name:		Charge_sCanWrite3
*Parameters:		pdata:the start address to be sent							
*Returns:			cCanTxBusy:the can bus is busy								
*					cCanTxRdy:can bus is ready to transmit this frame			
*Description:		This fucntion send the data to the can registers			
*********************************************************************************/
void	Charge_sCanWrite3(struct Charge_MESSAGE	*pdata)
{
	if(Charge_s_u16CanTxLength == Charge_CAN_TXBUF_SIZE)  // If CanTxLength is the same as Transmit Buffer Size, then transmit data. Tx is transmit.
	{
		Charge_sCanTxISR3();
		return;
	}	
	
	Charge_sCanCopy(pdata,Charge_s_pCanTxIn);//�1�71�1�77�1�71�1�77Buffer
	
	Charge_s_u16CanTxLength++;
	
	if((++Charge_s_pCanTxIn) > &Charge_s_aCanTxBuf[Charge_CAN_TXBUF_SIZE - 1])
	{
		Charge_s_pCanTxIn = Charge_s_aCanTxBuf;//�1�71�1�77�1�71�1�77�1�7�1�7
	}
}


void	Charge_sCanSendTrigger3(Uint16 FrameCnt)
{
    if(CanaRegs.CAN_TXRQ_21 & 0x0008) //�1�71�1�77�1�71�1�77�1�71�1�77�1�71�1�77�1�7�0�11�1�77�1�71�1�77�1�71�1�77
    {
    	return;
    }
    else
    {
    	Charge_sCanTxISR3();//reset TA and send 1 frame if buffer is not empty
    	g_u16CanFailTime = 0;
    }
}

//========================end of file====================









