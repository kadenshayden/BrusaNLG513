#ifndef INCLUDE_A_CONSTANT_H_
#define INCLUDE_A_CONSTANT_H_

#ifdef __cplusplus
extern "C" {
#endif
//------------------------------------------------------------------------------
//constant definition常量定义
//------------------------------------------------------------------------------
#ifndef TRUE
#define FALSE 0
#define TRUE  1
#endif

enum enum_pwmSwState {
	pwmSwState_normalOperation = 0,
	pwmSwState_positiveHalf = 1,
	pwmSwState_negativeZeroCrossing1=2,
	pwmSwState_negativeZeroCrossing2=3,
	pwmSwState_negativeZeroCrossing3=4,
	pwmSwState_negativeHalf = 5,
	pwmSwState_positiveZeroCrossing1=6,
	pwmSwState_positiveZeroCrossing2=7,
	pwmSwState_positiveZeroCrossing3=8,
	pwmSwState_defaultState=9,
};

typedef enum
{
    ADC_TRIGGER_SW_ONLY     = 0,     //!< Software only
    ADC_TRIGGER_CPU1_TINT0  = 1,     //!< CPU1 Timer 0, TINT0
    ADC_TRIGGER_CPU1_TINT1  = 2,     //!< CPU1 Timer 1, TINT1
    ADC_TRIGGER_CPU1_TINT2  = 3,     //!< CPU1 Timer 2, TINT2
    ADC_TRIGGER_GPIO        = 4,     //!< GPIO, ADCEXTSOC
    ADC_TRIGGER_EPWM1_SOCA  = 5,     //!< ePWM1, ADCSOCA
    ADC_TRIGGER_EPWM1_SOCB  = 6,     //!< ePWM1, ADCSOCB
    ADC_TRIGGER_EPWM2_SOCA  = 7,     //!< ePWM2, ADCSOCA
    ADC_TRIGGER_EPWM2_SOCB  = 8,     //!< ePWM2, ADCSOCB
    ADC_TRIGGER_EPWM3_SOCA  = 9,     //!< ePWM3, ADCSOCA
    ADC_TRIGGER_EPWM3_SOCB  = 10,    //!< ePWM3, ADCSOCB
    ADC_TRIGGER_EPWM4_SOCA  = 11,    //!< ePWM4, ADCSOCA
    ADC_TRIGGER_EPWM4_SOCB  = 12,    //!< ePWM4, ADCSOCB
    ADC_TRIGGER_EPWM5_SOCA  = 13,    //!< ePWM5, ADCSOCA
    ADC_TRIGGER_EPWM5_SOCB  = 14,    //!< ePWM5, ADCSOCB
    ADC_TRIGGER_EPWM6_SOCA  = 15,    //!< ePWM6, ADCSOCA
    ADC_TRIGGER_EPWM6_SOCB  = 16,    //!< ePWM6, ADCSOCB
    ADC_TRIGGER_EPWM7_SOCA  = 17,    //!< ePWM7, ADCSOCA
    ADC_TRIGGER_EPWM7_SOCB  = 18,    //!< ePWM7, ADCSOCB
    ADC_TRIGGER_EPWM8_SOCA  = 19,    //!< ePWM8, ADCSOCA
    ADC_TRIGGER_EPWM8_SOCB  = 20,    //!< ePWM8, ADCSOCB
    ADC_TRIGGER_EPWM9_SOCA  = 21,    //!< ePWM9, ADCSOCA
    ADC_TRIGGER_EPWM9_SOCB  = 22,    //!< ePWM9, ADCSOCB
    ADC_TRIGGER_EPWM10_SOCA = 23,   //!< ePWM10, ADCSOCA
    ADC_TRIGGER_EPWM10_SOCB = 24,   //!< ePWM10, ADCSOCB
    ADC_TRIGGER_EPWM11_SOCA = 25,   //!< ePWM11, ADCSOCA
    ADC_TRIGGER_EPWM11_SOCB = 26,   //!< ePWM11, ADCSOCB
    ADC_TRIGGER_EPWM12_SOCA = 27,   //!< ePWM12, ADCSOCA
    ADC_TRIGGER_EPWM12_SOCB = 28,   //!< ePWM12, ADCSOCB
    ADC_TRIGGER_CPU2_TINT0  = 29,    //!< CPU2 Timer 0, TINT0
    ADC_TRIGGER_CPU2_TINT1  = 30,    //!< CPU2 Timer 1, TINT1
    ADC_TRIGGER_CPU2_TINT2  = 31     //!< CPU2 Timer 2, TINT2
} ADC_Trigger;
typedef enum
{
    ADC_CLK_DIV_1_0 = 0,            //!< ADCCLK = (input clock) / 1.0
    ADC_CLK_DIV_2_0 = 2,            //!< ADCCLK = (input clock) / 2.0
    ADC_CLK_DIV_2_5 = 3,            //!< ADCCLK = (input clock) / 2.5
    ADC_CLK_DIV_3_0 = 4,            //!< ADCCLK = (input clock) / 3.0
    ADC_CLK_DIV_3_5 = 5,            //!< ADCCLK = (input clock) / 3.5
    ADC_CLK_DIV_4_0 = 6,            //!< ADCCLK = (input clock) / 4.0
    ADC_CLK_DIV_4_5 = 7,            //!< ADCCLK = (input clock) / 4.5
    ADC_CLK_DIV_5_0 = 8,            //!< ADCCLK = (input clock) / 5.0
    ADC_CLK_DIV_5_5 = 9,            //!< ADCCLK = (input clock) / 5.5
    ADC_CLK_DIV_6_0 = 10,           //!< ADCCLK = (input clock) / 6.0
    ADC_CLK_DIV_6_5 = 11,           //!< ADCCLK = (input clock) / 6.5
    ADC_CLK_DIV_7_0 = 12,           //!< ADCCLK = (input clock) / 7.0
    ADC_CLK_DIV_7_5 = 13,           //!< ADCCLK = (input clock) / 7.5
    ADC_CLK_DIV_8_0 = 14,           //!< ADCCLK = (input clock) / 8.0
    ADC_CLK_DIV_8_5 = 15            //!< ADCCLK = (input clock) / 8.5
} ADC_ClkPrescale;

typedef enum
{
    CLA_TRIGGER_SOFTWARE    = 0U,   //!< CLA Task Trigger Source is Software
    CLA_TRIGGER_ADCA1       = 1U,   //!< CLA Task Trigger Source is ADCA1
    CLA_TRIGGER_ADCA2       = 2U,   //!< CLA Task Trigger Source is ADCA2
    CLA_TRIGGER_ADCA3       = 3U,   //!< CLA Task Trigger Source is ADCA3
    CLA_TRIGGER_ADCA4       = 4U,   //!< CLA Task Trigger Source is ADCA4
    CLA_TRIGGER_ADCAEVT     = 5U,   //!< CLA Task Trigger Source is ADCAEVT
    CLA_TRIGGER_ADCB1       = 6U,   //!< CLA Task Trigger Source is ADCB1
    CLA_TRIGGER_ADCB2       = 7U,   //!< CLA Task Trigger Source is ADCB2
    CLA_TRIGGER_ADCB3       = 8U,   //!< CLA Task Trigger Source is ADCB3
    CLA_TRIGGER_ADCB4       = 9U,   //!< CLA Task Trigger Source is ADCB4
    CLA_TRIGGER_ADCBEVT     = 10U,  //!< CLA Task Trigger Source is ADCBEVT
    CLA_TRIGGER_ADCC1       = 11U,  //!< CLA Task Trigger Source is ADCC1
    CLA_TRIGGER_ADCC2       = 12U,  //!< CLA Task Trigger Source is ADCC2
    CLA_TRIGGER_ADCC3       = 13U,  //!< CLA Task Trigger Source is ADCC3
    CLA_TRIGGER_ADCC4       = 14U,  //!< CLA Task Trigger Source is ADCC4
    CLA_TRIGGER_ADCCEVT     = 15U,  //!< CLA Task Trigger Source is ADCCEVT

    CLA_TRIGGER_XINT1       = 29U,  //!< CLA Task Trigger Source is XINT1
    CLA_TRIGGER_XINT2       = 30U,  //!< CLA Task Trigger Source is XINT2
    CLA_TRIGGER_XINT3       = 31U,  //!< CLA Task Trigger Source is XINT3
    CLA_TRIGGER_XINT4       = 32U,  //!< CLA Task Trigger Source is XINT4
    CLA_TRIGGER_XINT5       = 33U,  //!< CLA Task Trigger Source is XINT5

    CLA_TRIGGER_EPWM1INT    = 36U,  //!< CLA Task Trigger Source is EPWM1INT
    CLA_TRIGGER_EPWM2INT    = 37U,  //!< CLA Task Trigger Source is EPWM2INT
    CLA_TRIGGER_EPWM3INT    = 38U,  //!< CLA Task Trigger Source is EPWM3INT
    CLA_TRIGGER_EPWM4INT    = 39U,  //!< CLA Task Trigger Source is EPWM4INT
    CLA_TRIGGER_EPWM5INT    = 40U,  //!< CLA Task Trigger Source is EPWM5INT
    CLA_TRIGGER_EPWM6INT    = 41U,  //!< CLA Task Trigger Source is EPWM6INT
    CLA_TRIGGER_EPWM7INT    = 42U,  //!< CLA Task Trigger Source is EPWM7INT
    CLA_TRIGGER_EPWM8INT    = 43U,  //!< CLA Task Trigger Source is EPWM8INT
    CLA_TRIGGER_EPWM9INT    = 44U,  //!< CLA Task Trigger Source is EPWM9INT

    CLA_TRIGGER_TINT0       = 68U,  //!< CLA Task Trigger Source is TINT0
    CLA_TRIGGER_TINT1       = 69U,  //!< CLA Task Trigger Source is TINT1
    CLA_TRIGGER_TINT2       = 70U,  //!< CLA Task Trigger Source is TINT2

    CLA_TRIGGER_ECAP1INT    = 75U,  //!< CLA Task Trigger Source is ECAP1INT
    CLA_TRIGGER_ECAP2INT    = 76U,  //!< CLA Task Trigger Source is ECAP2INT
    CLA_TRIGGER_ECAP3INT    = 77U,  //!< CLA Task Trigger Source is ECAP3INT
    CLA_TRIGGER_ECAP4INT    = 78U,  //!< CLA Task Trigger Source is ECAP4INT
    CLA_TRIGGER_ECAP5INT    = 79U,  //!< CLA Task Trigger Source is ECAP5INT
    CLA_TRIGGER_ECAP6INT    = 80U,  //!< CLA Task Trigger Source is ECAP6INT
    CLA_TRIGGER_ECAP7INT    = 81U,  //!< CLA Task Trigger Source is ECAP7INT

    CLA_TRIGGER_EQEP1INT    = 83U,  //!< CLA Task Trigger Source is EQEP1INT
    CLA_TRIGGER_EQEP2INT    = 84U,  //!< CLA Task Trigger Source is EQEP2INT

    CLA_TRIGGER_ECAP6INT2   = 92U,  //!< CLA Task Trigger Source is ECAP6INT2
    CLA_TRIGGER_ECAP7INT2   = 93U,  //!< CLA Task Trigger Source is ECAP7INT2

    CLA_TRIGGER_SDFM1INT    = 95U,  //!< CLA Task Trigger Source is SDFM1INT
    CLA_TRIGGER_SDFM1DRINT1 = 96U,  //!< CLA Task Trigger Source is SDFM1DRINT1
    CLA_TRIGGER_SDFM1DRINT2 = 97U,  //!< CLA Task Trigger Source is SDFM1DRINT2
    CLA_TRIGGER_SDFM1DRINT3 = 98U,  //!< CLA Task Trigger Source is SDFM1DRINT3
    CLA_TRIGGER_SDFM1DRINT4 = 99U,  //!< CLA Task Trigger Source is SDFM1DRINT4

    CLA_TRIGGER_PMBUSAINT   = 105U, //!< CLA Task Trigger Source is PMBUSAINT

    CLA_TRIGGER_SPITXAINT   = 109U, //!< CLA Task Trigger Source is SPITXAINT
    CLA_TRIGGER_SPIRXAINT   = 110U, //!< CLA Task Trigger Source is SPIRXAINT
    CLA_TRIGGER_SPITXBINT   = 111U, //!< CLA Task Trigger Source is SPITXBINT
    CLA_TRIGGER_SPIRXBINT   = 112U, //!< CLA Task Trigger Source is SPIRXBINT

    CLA_TRIGGER_LINAINT1    = 117U, //!< CLA Task Trigger Source is LINAINT1
    CLA_TRIGGER_LINAINT0    = 118U, //!< CLA Task Trigger Source is LINAINT0

    CLA_TRIGGER_CLA1PROMCRC = 121U, //!< CLA Task Trigger Source is CLA1PROMCRC

    CLA_TRIGGER_FSITXAINT1  = 123U, //!< CLA Task Trigger Source is FSITXAINT1
    CLA_TRIGGER_FSITXAINT2  = 124U, //!< CLA Task Trigger Source is FSITXAINT2
    CLA_TRIGGER_FSIRXAINT1  = 125U, //!< CLA Task Trigger Source is FSIRXAINT1
    CLA_TRIGGER_FSIRXAINT2  = 126U  //!< CLA Task Trigger Source is FSIRXAINT2

} CLA_Trigger;

typedef enum
{
    EPWM_LINK_WITH_EPWM_1 = 0,   //!< link current ePWM with ePWM1
    EPWM_LINK_WITH_EPWM_2 = 1,   //!< link current ePWM with ePWM2
    EPWM_LINK_WITH_EPWM_3 = 2,   //!< link current ePWM with ePWM3
    EPWM_LINK_WITH_EPWM_4 = 3,   //!< link current ePWM with ePWM4
    EPWM_LINK_WITH_EPWM_5 = 4,   //!< link current ePWM with ePWM5
    EPWM_LINK_WITH_EPWM_6 = 5,   //!< link current ePWM with ePWM6
    EPWM_LINK_WITH_EPWM_7 = 6,   //!< link current ePWM with ePWM7
    EPWM_LINK_WITH_EPWM_8 = 7,   //!< link current ePWM with ePWM8
    EPWM_LINK_WITH_EPWM_9 = 8,   //!< link current ePWM with ePWM9
    EPWM_LINK_WITH_EPWM_10 = 9,  //!< link current ePWM with ePWM10
    EPWM_LINK_WITH_EPWM_11 = 10, //!< link current ePWM with ePWM11
    EPWM_LINK_WITH_EPWM_12 = 11  //!< link current ePWM with ePWM12
}EPWM_CurrentLink;

typedef enum
{
	SYNC_SELECT_EPWM1SYNCOUT = 0,   //!< EPWM1SYNCOUT selected
	SYNC_SELECT_EPWM4SYNCOUT = 1,   //!< EPWM4SYNCOUT selected
	SYNC_SELECT_EPWM7SYNCOUT = 2,   //!< EPWM7SYNCOUT selected
	SYNC_SELECT_EPWM10SYNCOUT = 3,   //!< EPWM10SYNCOUT selected
	SYNC_SELECT_ECAP1SYNCOUT = 4,   //!< ECAP1SYNCOUT selected
	SYNC_SELECT_EXTSYNCIN1 = 5,   //!< EXTSYNCIN1 selected
	SYNC_SELECT_EXTSYNCIN2 = 6,   //!< EXTSYNCIN2 selected
}SYNC_SELECT;


//*****************************************************************************
//
//! Values that can be passed to EPWM_setDigitalCompareBlankingEvent() as the
//! the \e blankingPulse parameter.
//
//*****************************************************************************
typedef enum
{
    //! Time base counter equals period
    EPWM_DC_WINDOW_START_TBCTR_PERIOD = 0,
    //! Time base counter equals zero
    EPWM_DC_WINDOW_START_TBCTR_ZERO   = 1,
    //! Time base counter equals zero
    EPWM_DC_WINDOW_START_TBCTR_ZERO_PERIOD  = 2
}EPWM_DigitalCompareBlankingPulse;


//*****************************************************************************
//
//! Values that can be passed to EPWM_setDigitalCompareFilterInput()
//! as the \e filterInput parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_DC_WINDOW_SOURCE_DCAEVT1 = 0, //!< DC filter signal source is DCAEVT1
    EPWM_DC_WINDOW_SOURCE_DCAEVT2 = 1, //!< DC filter signal source is DCAEVT2
    EPWM_DC_WINDOW_SOURCE_DCBEVT1 = 2, //!< DC filter signal source is DCBEVT1
    EPWM_DC_WINDOW_SOURCE_DCBEVT2 = 3  //!< DC filter signal source is DCBEVT2
}EPWM_DigitalCompareFilterInput;


//*****************************************************************************
//
//! Values that can be passed to EPWM_setValleyTriggerSource() as the \e
//! trigger parameter.
//
//*****************************************************************************
typedef enum
{
    //! Valley capture trigged by software.
    EPWM_VALLEY_TRIGGER_EVENT_SOFTWARE = 0U,
    //! Valley capture trigged by when counter is equal to zero.
    EPWM_VALLEY_TRIGGER_EVENT_CNTR_ZERO = 1U,
    //! Valley capture trigged by when counter is equal period.
    EPWM_VALLEY_TRIGGER_EVENT_CNTR_PERIOD = 2U,
    //! Valley capture trigged when counter is equal to zero or period.
    EPWM_VALLEY_TRIGGER_EVENT_CNTR_ZERO_PERIOD = 3U,
    //! Valley capture trigged by DCAEVT1 (Digital Compare A event 1)
    EPWM_VALLEY_TRIGGER_EVENT_DCAEVT1 = 4U,
    //! Valley capture trigged by DCAEVT2 (Digital Compare A event 2)
    EPWM_VALLEY_TRIGGER_EVENT_DCAEVT2 = 5U,
    //! Valley capture trigged by DCBEVT1 (Digital Compare B event 1)
    EPWM_VALLEY_TRIGGER_EVENT_DCBEVT1 = 6U,
    //! Valley capture trigged by DCBEVT2 (Digital Compare B event 2)
    EPWM_VALLEY_TRIGGER_EVENT_DCBEVT2 = 7U
}EPWM_ValleyTriggerSource;

//*****************************************************************************
//
//! Values that can be passed to EPWM_getValleyCountEdgeStatus() as the \e edge
//! parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_VALLEY_COUNT_START_EDGE = 0, //!< Valley count start edge
    EPWM_VALLEY_COUNT_STOP_EDGE  = 1  //!< Valley count stop edge
}EPWM_ValleyCounterEdge;

//*****************************************************************************
//
//! Values that can be passed to EPWM_setValleyDelayValue() as the \e delayMode
//! parameter.
//
//*****************************************************************************
typedef enum
{
    //! Delay value equals the offset value defines by software
    EPWM_VALLEY_DELAY_MODE_SW_DELAY   = 0U,
    //! Delay value equals the sum of the Hardware counter value and the offset
    //! value defines by software
    EPWM_VALLEY_DELAY_MODE_VCNT_DELAY_SW_DELAY = 1U,
    //! Delay value equals the the Hardware counter shifted by
    //! (1 + the offset value defines by software)
    EPWM_VALLEY_DELAY_MODE_VCNT_DELAY_SHIFT_1_SW_DELAY = 2U,
    //! Delay value equals the the Hardware counter shifted by
    //! (2 + the offset value defines by software)
    EPWM_VALLEY_DELAY_MODE_VCNT_DELAY_SHIFT_2_SW_DELAY = 3U,
    //! Delay value equals the the Hardware counter shifted by
    //! (4 + the offset value defines by software)
    EPWM_VALLEY_DELAY_MODE_VCNT_DELAY_SHIFT_4_SW_DELAY = 4U
}EPWM_ValleyDelayMode;

//*****************************************************************************
//
//! Values that can be passed to EPWM_setTripZoneAction() as the
//! \e tzAction parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_TZ_ACTION_HIGH_Z  = 0, //!< high impedance output
    EPWM_TZ_ACTION_HIGH    = 1, //!< high voltage state
    EPWM_TZ_ACTION_LOW     = 2, //!< low voltage state
    EPWM_TZ_ACTION_DISABLE = 3  //!< disable action
}EPWM_TripZoneAction;

//*****************************************************************************
//
//! Values that can be passed to EPWM_setTripZoneAdvDigitalCompareActionA(),
//! EPWM_setTripZoneAdvDigitalCompareActionB(),EPWM_setTripZoneAdvAction()
//! as the \e tzAdvDCAction parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_TZ_ADV_ACTION_HIGH_Z  = 0, //!< high impedance output
    EPWM_TZ_ADV_ACTION_HIGH    = 1, //!< high voltage state
    EPWM_TZ_ADV_ACTION_LOW     = 2, //!< low voltage state
    EPWM_TZ_ADV_ACTION_TOGGLE  = 3, //!< toggle the output
    EPWM_TZ_ADV_ACTION_DISABLE = 7  //!< disable action
}EPWM_TripZoneAdvancedAction;

//*****************************************************************************
//
//! Values that can be passed to EPWM_selectCycleByCycleTripZoneClearEvent() as
//! the \e clearMode parameter.
//
//*****************************************************************************
typedef enum
{
    //! Clear CBC pulse when counter equals zero
    EPWM_TZ_CBC_PULSE_CLR_CNTR_ZERO = 0,
    //! Clear CBC pulse when counter equals period
    EPWM_TZ_CBC_PULSE_CLR_CNTR_PERIOD = 1,
    //! Clear CBC pulse when counter equals zero or period
    EPWM_TZ_CBC_PULSE_CLR_CNTR_ZERO_PERIOD = 2
}EPWM_CycleByCycleTripZoneClearMode;

//*****************************************************************************
//
//! Values that can be passed to EPWM_selectDigitalCompareTripInput() as the
//! \e tripSource parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_DC_TRIP_TRIPIN1 = 0,   //!< Trip 1
    EPWM_DC_TRIP_TRIPIN2 = 1,   //!< Trip 2
    EPWM_DC_TRIP_TRIPIN3 = 2,   //!< Trip 3
    EPWM_DC_TRIP_TRIPIN4 = 3,   //!< Trip 4
    EPWM_DC_TRIP_TRIPIN5 = 4,   //!< Trip 5
    EPWM_DC_TRIP_TRIPIN6 = 5,   //!< Trip 6
    EPWM_DC_TRIP_TRIPIN7 = 6,   //!< Trip 7
    EPWM_DC_TRIP_TRIPIN8 = 7,   //!< Trip 8
    EPWM_DC_TRIP_TRIPIN9 = 8,   //!< Trip 9
    EPWM_DC_TRIP_TRIPIN10 = 9,  //!< Trip 10
    EPWM_DC_TRIP_TRIPIN11 = 10, //!< Trip 11
    EPWM_DC_TRIP_TRIPIN12 = 11, //!< Trip 12
    EPWM_DC_TRIP_TRIPIN14 = 13, //!< Trip 14
    EPWM_DC_TRIP_TRIPIN15 = 14, //!< Trip 15
    EPWM_DC_TRIP_COMBINATION = 15 //!< All Trips (Trip1 - Trip 15) are selected
}EPWM_DigitalCompareTripInput;

//*****************************************************************************
//
//! Values that can be passed to EPWM_setTripZoneDigitalCompareEventCondition()
//! as the \e dcEvent parameter.
//
//*****************************************************************************
typedef enum
{
    EPWM_TZ_EVENT_DC_DISABLED = 0,       //!< Event is disabled
    EPWM_TZ_EVENT_DCXH_LOW    = 1,       //!< Event when DCxH low
    EPWM_TZ_EVENT_DCXH_HIGH   = 2,       //!< Event when DCxH high
    EPWM_TZ_EVENT_DCXL_LOW    = 3,       //!< Event when DCxL low
    EPWM_TZ_EVENT_DCXL_HIGH   = 4,       //!< Event when DCxL high
    EPWM_TZ_EVENT_DCXL_HIGH_DCXH_LOW = 5 //!< Event when DCxL high DCxH low
}EPWM_TripZoneDigitalCompareOutputEvent;


//*****************************************************************************
//
//! Values that can be passed to EPWM_setDigitalCompareEventSource() as the
//! \e dcEventSource parameter.
//
//*****************************************************************************
typedef enum
{
    //! signal source is unfiltered (DCAEVT1/2)
    EPWM_DC_EVENT_SOURCE_ORIG_SIGNAL = 0,
    //! signal source is filtered (DCEVTFILT)
    EPWM_DC_EVENT_SOURCE_FILT_SIGNAL = 1
}EPWM_DigitalCompareEventSource;


//*****************************************************************************
//
//! Values that can be passed to EPWM_setDigitalCompareEventSyncMode() as the
//! \e syncMode parameter.
//
//*****************************************************************************
typedef enum
{
    //! DC input signal is synced with TBCLK
    EPWM_DC_EVENT_INPUT_SYNCED = 0,
    //! DC input signal is not synced with TBCLK
    EPWM_DC_EVENT_INPUT_NOT_SYNCED = 1
}EPWM_DigitalCompareSyncMode;




//------------DCAN Driver------------------
typedef enum
{
        //! Transmit message object.
        MSG_OBJ_TYPE_TRANSMIT,

        //! Receive message object.
        MSG_OBJ_TYPE_RECEIVE
}
msgObjType;

typedef enum
{
	MsgInvalid = 0,
	Msg1,	Msg2,	Msg3,	Msg4,	Msg5,	Msg6,	Msg7,	Msg8,	Msg9,	Msg10,
	Msg11,	Msg12,	Msg13,	Msg14,	Msg15,	Msg16,	Msg17,	Msg18,	Msg19,	Msg20,
	Msg21,	Msg22,	Msg23,	Msg24,	Msg25,	Msg26,	Msg27,	Msg28,	Msg29,	Msg30,
	Msg31,	Msg32
}MsgObjN; //MsgN for Mbx[N-1]

//------------constants for calculation----------------
#define INV100_IQ20	 	 (INT32)10486// 1/100*2^20
#define INV30_IQ20		 (INT32)34953// 1/30*2^20
#define INV15_IQ20		 (INT32)69905// 1/15*2^20
#define INV10_IQ20		 (INT32)104858// 1/10*2^20
#define INV_SQRT2_IQ10			(INT32)724			//1/sqrt(2)*2^10

#define PI_VALUE	(float)3.141592653589f
#define PI_2_VALUE	(float)6.28318530718f//2pi = 6.28318530718 = 1.570796326795 * 2^2


#define  CALIBR_FAIL       		0x0001
#define  READ_E2PROM_SUCCESS  	0x0000
#define  READ_E2PROM_FAIL     	0x0001

//------------------------------------------------------------------------------
//define constant for time                                         
//------------------------------------------------------------------------------
#define LQ10TIME200000H			(((INT32)200000) << 10)	

//1s base
#define ADDR_TIME20S   			((UINT16)20)    		//20
#define TIME2000S     			((UINT16)2000)    		//2000
#define LQ10TIME300S			(((INT32)300) << 10)
#define LQ10TIME50S				(((INT32)50) << 10)
#define LQ10TIME10S				(((INT32)10) << 10)

//walkin 100ms base
#define WALKTIME8S 				(((INT32)800) << 10)
#define WALKTIME10S				(((INT32)1000) << 10)
#define WALKTIME128S			(((INT32)12800) << 10)
#define WALKTIME200S			(((INT32)20000) << 10)

//5ms base
#define TIME_MT_3300MS		((UINT16)660)		//660 * 5ms
#define TIME_MT_3035MS		((UINT16)607)		//607 * 5ms
#define TIME_MT_3000MS		((UINT16)600)		//600 * 5ms
#define TIME_MT_1500MS		((UINT16)300)		//300 * 5ms
#define TIME_MT_1000MS		((UINT16)200)		//200 * 5ms
#define TIME_MT_500MS		((UINT16)100)		//100 * 5ms
#define TIME_MT_395MS		((UINT16)79)		//79 * 5ms
#define TIME_MT_115MS		((UINT16)23)		//23 * 5ms
#define TIME_MT_95MS		((UINT16)19)		//23 * 5ms
#define TIME_MT_20MS		((UINT16)4)			//4 * 5ms
#define TIME_MT_10MS		((UINT16)2)			//2 * 5ms

//500ms base
#define TIME_FU_10S		((UINT16)20)			//20 * 500ms

//------------------------------------------------------------------------------
//define constant for on off control                                          
//------------------------------------------------------------------------------
#define DCDC_OFF        ((UINT16)0x01)
#define PFC_OFF         ((UINT16)0x02)

//------------------------------------------------------------------------------
//define constant for power limit flag                                        
//------------------------------------------------------------------------------
#define POW_LIM         		((UINT16)0x0001)
#define POW_LIM_TEMP    		((UINT16)0x0002)
#define POW_LIM_AC      		((UINT16)0x0004)

//------------------------------------------------------------------------------
//define constant for module soft start states                                         
//------------------------------------------------------------------------------
#define POWON_STATE   	  		((UINT16)0x0000)
#define PFC_HWSOFT_START  		((UINT16)0x0001)
#define PFC_SWSOFT_START_INIT  	((UINT16)0x0002)
#define PFC_SWSOFT_START       	((UINT16)0x0003)
#define DC_SOFT_START_INIT  	((UINT16)0x0004)
#define DC_SOFT_START        	((UINT16)0x0005)
#define NORMAL_RUN_STATE        ((UINT16)0x0006)
#define DC_CLOSE_STATE        ((UINT16)0x0010)
#define AC_CLOSE_STATE        ((UINT16)0x0020)


//-------------------------------------------------------
//define constant for main relay zvs close flag
//---------------------------------------------------------
#define  MAINRLY_ZVS_CTL_OPEN   ((UINT16)0x0000)
#define  MAINRLY_ZVS_CTL_START  ((UINT16)0x0001)
#define  MAINRLY_ZVS_CTL_CHECK  ((UINT16)0x0002)
#define  MAINRLY_ZVS_CTL_CLOSE  ((UINT16)0x0003)

#define EPROM_READY_LONG  		((INT32)0x55AA55AA)
#define EPROM_READY_UINT  		((UINT16)0x55AA)
#define EPROM_DATA_EMPTY  		((INT32)0xFFFFFFFF)

#define MAIN_ISR_RAM_START		((UINT16)0x700)
#define MAIN_ISR_RAM_END		((UINT16)0x7E0)

#define MAIN_RAM_START		((UINT16)0x9000)
#define MAIN_RAM_END		((UINT16)0x9800)




/*******************************************
* IQ10 numbers
****************************************/
//------------------------------------------------------------------------------
//define constant for power                                         
//------------------------------------------------------------------------------
#define POW_15W        	(((INT32)15) << 10)
#define POW_25W        	(((INT32)25) << 10)
#define POW_30W        	(((INT32)30) << 10)  
#define POW_80W        	(((INT32)80) << 10)   
#define POW_100W       	(((INT32)100) << 10)
#define POW_150W       	(((INT32)150) << 10)
#define POW_200W       	(((INT32)200) << 10)
#define POW_300W       	(((INT32)300) << 10)
#define POW_350W       	(((INT32)350) << 10)
#define POW_400W       	(((INT32)400) << 10)
#define POW_2900W		(((INT32)2900) << 10)
#define POW_3200W		(((INT32)3200) << 10)

//------------------------------------------------------------------------------
//define constant for dcdc voltage                                         
//------------------------------------------------------------------------------
#define VDC_1MV         ((INT32)1)
#define VDC_0V2        	((INT32)204)
#define VDC_0V5        	((INT32)512)
#define VDC_2V  	   	(((INT32)2) << 10)
#define VDC_3V5			((INT32)3580)
#define VDC_10V  	    (((INT32)10) << 10)
#define VDC_20V  	    (((INT32)20) << 10)
#define VDC_45V  	    (((INT32)45) << 10)
#define VDC_46V  	    (((INT32)46) << 10)
#define VDC_47V  	    (((INT32)47) << 10)
#define VDC_47V5  	    (((INT32)95) << 9)
#define VDC_48V5  	    (((INT32)97) << 9)
#define VDC_48V75  	    (((INT32)195) << 8)
#define VDC_49V  	    (((INT32)49) << 10)
#define VDC_49V5  	    (((INT32)99) << 9)	
#define VDC_50V  	    (((INT32)50) << 10)	
#define VDC_50V5  	    (((INT32)101) << 9)
#define VDC_100V		(((INT32)100) << 10)
#define VDC_150V		(((INT32)150) << 10)
#define VDC_198V		(((INT32)198) << 10)
#define VDC_200V		(((INT32)200) << 10)
#define VDC_210V		(((INT32)210) << 10)
#define VDC_250V		(((INT32)250) << 10)
#define VDC_280V		(((INT32)280) << 10)
#define VDC_300V		(((INT32)300) << 10)
#define VDC_318V        (((INT32)318) << 10)
#define VDC_330V		(((INT32)330) << 10)
#define VDC_340V		(((INT32)340) << 10)
#define VDC_345V		(((INT32)345) << 10)
#define VDC_360V		(((INT32)360) << 10)
#define VDC_366V		(((INT32)366) << 10)
#define VDC_370V		(((INT32)370) << 10)
#define VDC_380V		(((INT32)380) << 10)
#define VDC_400V		(((INT32)400) << 10)
#define VDC_410V		(((INT32)410) << 10)
#define VDC_420V		(((INT32)420) << 10)
#define VDC_435V		(((INT32)435) << 10)
#define VDC_450V		(((INT32)450) << 10)
#define VDC_455V		(((INT32)455) << 10)
#define VDC_465V		(((INT32)465) << 10)
#define VDC_485V		(((INT32)485) << 10)
#define VDC_490V		(((INT32)490) << 10)
#define VDC_495V		(((INT32)495) << 10)
#define VDC_500V		(((INT32)500) << 10)
#define VDC_505V		(((INT32)505) << 10)
#define VDC_510V		(((INT32)510) << 10)
#define VDC_550V		(((INT32)550) << 10)
#define VDC_750V 		(750L<<10)

//------------------------------------------------------------------------------
//define constant for Vpfc voltage                                         
//------------------------------------------------------------------------------
#define VPFC_500V       (((INT32)500) << 10)
#define VPFC_450V       (((INT32)450) << 10)
#define VPFC_435V      	(((INT32)435) << 10)
#define VPFC_432V       (((INT32)432) << 10)
#define VPFC_430V       (((INT32)430) << 10)
#define VPFC_428V       (((INT32)428) << 10)
#define VPFC_425V      	(((INT32)425) << 10)
#define VPFC_420V      	(((INT32)420) << 10)
#define VPFC_405V       (((INT32)405) << 10)
#define VPFC_395V       (((INT32)395) << 10)
#define VPFC_385V       (((INT32)385) << 10)
#define VPFC_380V       (((INT32)380) << 10)
#define VPFC_375V       (((INT32)375) << 10)
#define VPFC_376V       (((INT32)376) << 10)
#define VPFC_375V       (((INT32)375) << 10)
#define VPFC_365V       (((INT32)365) << 10)
#define VPFC_350V       (((INT32)350) << 10)
#define VPFC_330V       (((INT32)330) << 10)
#define VPFC_200V       (((INT32)200) << 10)
#define VPFC_180V       (((INT32)180) << 10) 
#define VPFC_80V        (((INT32)80) << 10)
#define VPFC_60V        (((INT32)60) << 10) 
#define VPFC_50V        (((INT32)50) << 10)
#define VPFC_3V        (((INT32)3) << 10)
#define VPFC_1V		     (((INT32)1) << 10)
#define VPFC_0V5        (((INT32)1) << 9)				 
#define VPFC_0V3        (INT32)307				//0.3 << 10
//------------------------------------------------------------------------------
//define constant for ac voltage                                         
//------------------------------------------------------------------------------
#define VAC_300V       	(((INT32)300) << 10)
#define VAC_295V       	(((INT32)295) << 10) 
#define VAC_282V       	(((INT32)282) << 10)
#define VAC_280V       	(((INT32)280) << 10)
#define VAC_264V       	(((INT32)264) << 10)
#define VAC_220V       	(((INT32)220) << 10)
#define VAC_210V       	(((INT32)210) << 10)
#define VAC_203V       	(((INT32)203) << 10)
#define VAC_200V       	(((INT32)200) << 10)
#define VAC_195V       	(((INT32)195) << 10)
#define VAC_185V       	(((INT32)185) << 10)
#define VAC_180V       	(((INT32)180) << 10) 
#define VAC_176V       	(((INT32)176) << 10) 
#define VAC_150V       	(((INT32)150) << 10)
#define VAC_120V       	(((INT32)120) << 10)
#define VAC_117V       	(((INT32)117) << 10)
#define VAC_85V         (((INT32)85) << 10)
#define VAC_80V         (((INT32)80) << 10)
#define VAC_55V         (((INT32)55) << 10)
#define VAC_50V         (((INT32)50) << 10)
#define VAC_25V         (((INT32)25) << 10)
#define VAC_15V         (((INT32)15) << 10)
#define VAC_10V         (((INT32)10) << 10)
#define VAC_3V5         (((INT32)7) << 9)  			
#define VAC_2V          (((INT32)2) << 10)
#define VAC_0V			(INT32)0       			// 0<<10 


//------------------------------------------------------------------------------
//define constant for dcdc current
//------------------------------------------------------------------------------
#define IDC_0A			(INT32)0       			// 0<<10
#define IDC_0A2			(INT32)205       			// 0.2<<10
#define IDC_0A5         (INT32)512   			//0.5<<10
#define IDC_1A         	(INT32)1024
#define IDC_1A5         (((INT32)3) << 9)
#define IDC_2A         (((INT32)2) << 10)
#define IDC_2A5         (((INT32)5) << 9)
#define IDC_3A         (((INT32)3) << 10)
#define IDC_5A         (((INT32)5) << 10)
#define IDC_25A         (((INT32)25) << 10)
#define IDC_40A         (((INT32)40) << 10)
//------------------------------------------------------------------------------
//define constant for ac current
//------------------------------------------------------------------------------
#define IAC_30A        	(((INT32)30) << 10)
#define IAC_31A        	(((INT32)31) << 10)
#define IAC_1A        	(((INT32)1) << 10)

//------------------------------------------------------------------------------
//define constant for temperture
//------------------------------------------------------------------------------
#define TEMP_150C       (((INT32)150) << 10)
#define TEMP_125C       (((INT32)125) << 10)
#define TEMP_95C		(((INT32)95) << 10)
#define TEMP_76C       	(((INT32)76) << 10)
#define TEMP_75C       	(((INT32)75) << 10)
#define TEMP_70C       	(((INT32)70) << 10)
#define TEMP_65C      	(((INT32)65) << 10)
#define TEMP_60C      	(((INT32)60) << 10)
#define TEMP_55C       	(((INT32)55) << 10)
#define TEMP_50C       	(((INT32)50) << 10)
#define TEMP_45C       	(((INT32)45) << 10)
#define TEMP_35C       	(((INT32)35) << 10)
#define TEMP_33C       	(((INT32)33) << 10)
#define TEMP_30C       	(((INT32)30) << 10)
#define TEMP_28C       	(((INT32)28) << 10)
#define TEMP_10C       	(((INT32)10) << 10)
#define TEMP_3C5        (((INT32)7) << 9)
#define TEMP_2C         (((INT32)2) << 10)
#define NM_TEMP_7C       (((INT32)(-7)) << 10)
#define N_TEMP_23C      (((INT32)(-23)) << 10)
#define N_TEMP_27C      (((INT32)(-27)) << 10)
#define N_TEMP_30C      (((INT32)(-30)) << 10)
#define N_TEMP_40C      (((INT32)(-40)) << 10)
//===========================================================================
// End of file.
//===========================================================================
#ifdef __cplusplus
}
#endif                                  /* extern "C" */

#endif /* INCLUDE_A_CONSTANT_H_ */
