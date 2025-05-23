#ifndef _DP_RPT_DATASTRUCT_
#define _DP_RPT_DATASTRUCT_

#include <QString>
#include "../include/dp_types.h"

#define DB_SQL_CONN         "DB_Conn"

/*Add Structures*/


typedef struct _S_HARMONIC_MEASUREMENT
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dInputfreq;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpMin2ndHarmonicRejection;
    FDOUBLE m_dExpMin3rdHarmonicRejection;
    FDOUBLE m_dFundamentalFreq;
    FDOUBLE m_d2ndHarmFreq;
    FDOUBLE m_d3rdHarmFreq;
    FDOUBLE m_dFundamentalPwr;
    FDOUBLE m_d2ndHarmPwr;
    FDOUBLE m_d3rdHarmPwr;
    FDOUBLE m_dMeasured_Power;
    FDOUBLE m_dMeasPathLoss;
    FDOUBLE m_dCalculatedPower;
    FDOUBLE m_d2ndHarmonicRejection;
    FDOUBLE m_d3rdHarmonicRejection;
    U8BIT m_u8Result;
}S_HARMONIC_MEASUREMENT;

typedef struct _S_NOISEFIGURE_MEASUREMENT
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dRFfreq;
    FDOUBLE m_dExpNoiseFig;
    FDOUBLE m_dMeasNoiseFig;
    U8BIT m_u8Result;
}S_NOISEFIGURE_MEASUREMENT;

typedef struct _S_POWERDROOP_MEASUREMENT
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dRFInputFreq;
    FDOUBLE m_dMeasCabeloss;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dMeasPowerMarker1;
    FDOUBLE m_dMeasPowerMarker2;
    FDOUBLE m_dExpMaxPowerDroop;
    FDOUBLE m_dCalPowerDroop;
    U8BIT m_u8Result;
}S_POWERDROOP_MEASUREMENT;

typedef struct _S_RF_PULSE_RISE_FALL_TIME_MEAS
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dInputFrequency;
    FDOUBLE m_dExpRiseFallTime;
    FDOUBLE m_dMeasRiseTime;
    FDOUBLE m_dMeasFallTime;
    U8BIT m_u8Result;
}S_RF_PULSE_RISE_FALL_TIME_MEAS;

typedef struct _S_RX_GAIN_MEASUREMENT
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dInputFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dExpMinimumGain;
    FDOUBLE m_dExpMaximumGain;
    FDOUBLE m_dMeasuredFreq;
    FDOUBLE m_dMeasuredGain;
    U8BIT m_u8Result;
}S_RX_GAIN_MEASUREMENT;


typedef struct _S_INPUT_RETURN_LOSS
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dExpMaxReturnloss;
        FDOUBLE m_dMeasReturnLoss;
        U8BIT m_u8Result;
}S_INPUT_RETURN_LOSS;

typedef struct _S_OUTPUT_RETURN_LOSS
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dExpMaxReturnloss;
        FDOUBLE m_dMeasReturnLoss;
        U8BIT m_u8Result;
}S_OUTPUT_RETURN_LOSS;

typedef struct _S_RX_ISOLATION_MEASUREMENT
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dExpIsolation;
        FDOUBLE m_dCalcIsolation;
        FDOUBLE m_dMeasureGainRxMode;
        FDOUBLE m_dMeasureGainIdleMode;
        U8BIT m_u8Result;
}S_RX_ISOLATION_MEASUREMENT;

typedef struct _S_TX_OUTPUT_POWER_MEASUREMENT
{
    S32BIT m_s32EID;
    U8BIT m_u8ChannelNo;
    FDOUBLE m_dRFfreq;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dOutputCableLoss;
    FDOUBLE m_dExpectedOutputPower;
    FDOUBLE m_dMeasuredOutputPower;
    FDOUBLE m_dCalculatedOutputPower;
    FDOUBLE m_dMeasPowerWatt;
    U8BIT m_u8Result;
}S_TX_OUTPUT_POWER_MEASUREMENT;

typedef struct _S_CALIB_CABLOSS_RPT
{
    S32BIT m_s32EID;
    S32BIT m_s32PathID;
    char m_szPathName[1024];
    FDOUBLE m_dRadioFreq;
    FDOUBLE m_dAmp;
    FDOUBLE m_dPathLoss;
    char m_szST[1024];
}S_CALIB_CABLOSS_RPT;


typedef struct _S_POWER_TUNING_MEASUREMENT
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dMeasuredOutputPower;
        FDOUBLE m_dCurrent;
        FDOUBLE m_dPulseWidth;
        FDOUBLE m_dPulsePeriod;
        FDOUBLE m_dCalculatedOutputPower;
        FDOUBLE m_dMeasPowerWatt;
        FDOUBLE m_dEfficiency;
        U8BIT m_u8Result;
}S_POWER_TUNING_MEASUREMENT;

typedef struct _S_RX_OUTOFBAND_REJECTION_MEASUREMENT
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dObservedGain;
        FDOUBLE m_dExpRejection;
        FDOUBLE m_dCalculatedRejection;
        U8BIT m_u8Result;
}S_RX_OUTOFBAND_REJECTION_MEASUREMENT;

typedef struct _S_PHASE_NOISE_MEASUREMENT
{
    S32BIT m_s32EID;
    S8BIT m_s8ConnectorID;
    FDOUBLE m_dInputFrequency;
    FDOUBLE m_dInputPower;
    FDOUBLE m_dStimulasPathLoss;
    FDOUBLE m_dExpectedOutputFrequency;
    FDOUBLE m_dOutputFrequency;
    FDOUBLE m_dExpectedPhaseNoiseLevel;
    FDOUBLE m_dObservedPhaseNoiseLevel;
    U8BIT m_u8Result;
}S_PHASE_NOISE_MEASUREMENT;

typedef struct _S_IMAGE_REJECTION
{
        S32BIT m_s32EID;
        char m_szRFIFPort[1024];
        char m_szBandDetails[1024];
        S32BIT m_s32ChannelNo;
        S32BIT m_s32ImageNo;
        FDOUBLE m_dMSTCAtten;
        FDOUBLE m_dAGCAtten;
        FDOUBLE m_dRfFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dRFCableloss;
        FDOUBLE m_dImageFreq;
        FDOUBLE m_dMeasActpwr;
        FDOUBLE m_dImagePwr;
        FDOUBLE m_dExpMinRej;
        FDOUBLE m_dImageRej;
        S32BIT m_s32Result;
}S_IMAGE_REJECTION;

typedef struct _S_ONEDBCOMPRESSIONPOINT
{
        S32BIT m_s32EID;
        S32BIT m_s32ChannelNo;
        char m_szRFIFPort[1024];
        char m_szBandDetails[1024];
        FDOUBLE m_dDCA1Atten;
        FDOUBLE m_dDCA2Atten;
        FDOUBLE m_dRFInptFreq;
        FDOUBLE m_dRFCableloss;
        FDOUBLE m_dRFpwr;
        FDOUBLE m_dMeasCableloss;
        FDOUBLE m_dMeasActPwr;
        FDOUBLE m_dRefGain;
        FDOUBLE m_dCalcGain;
        FDOUBLE m_d1dBComPoint;
        S32BIT m_s32Result;
}S_ONEDBCOMPRESSIONPOINT;

typedef struct _S_RX_ATTENUATOR_VALIDATION
{
        S32BIT m_s32EID;
        char m_szRFIFPort[1024];
        char m_szBandDetails[1024];
        S32BIT m_s32ChannelNo;
        S32BIT m_s32BandSel;
        FDOUBLE m_dDCA;
        FDOUBLE m_dRFinputfreq;
        FDOUBLE m_dRFinputPower;
        FDOUBLE m_dRFCableloss;
        FDOUBLE m_dRefPower;
        FDOUBLE m_dMeaspower;
        FDOUBLE m_dMeasCBloss;
        FDOUBLE m_dCalcPower;
        FDOUBLE m_dExpMinAtten;
        FDOUBLE m_dExpMaxAtten;
        FDOUBLE m_dCalcAttenuation;
        S32BIT m_s32Result;
}S_RX_ATTENUATOR_VALIDATION;

typedef struct _S_RX_GAIN_MEASUREMENT_SA
{
        S32BIT m_s32EID;
        char m_szRFIFPort[1024];
        char m_szBandDetails[1024];
        S32BIT m_s32ChannelNo;
        FDOUBLE m_dDCA1Atten;
        FDOUBLE m_dDCA2Atten;
        FDOUBLE m_dIFFreq;
        FDOUBLE m_dRf_inputfreq;
        FDOUBLE m_dRFCableloss;
        FDOUBLE m_dRf_inputPower;
        FDOUBLE m_dMeasCableloss;
        FDOUBLE m_dMesured_Power;
        FDOUBLE m_dCalculated_Power;
        FDOUBLE m_dExpMinGain;
        FDOUBLE m_dExpMaxGain;
        FDOUBLE m_dCalculated_Gain;
        S32BIT m_s32Result;
}S_RX_GAIN_MEASUREMENT_SA;

typedef struct _S_VSWR_MEASUREMENT
{
        S32BIT m_s32EID;
        char m_szRFIFPort[1024];
        char m_szBandDetails[1024];
        S32BIT m_s32ChannelNo;
        S32BIT m_s32StartFreq;
        S32BIT m_s32StopFreq;
        FDOUBLE m_dInputPortMaxFreq;
        FDOUBLE m_dinputPower;
        FDOUBLE m_dOutputPortMaxFreq;
        FDOUBLE m_dExpectedVSWR;
        FDOUBLE m_dInputPortVSWR;
        FDOUBLE m_dOutputPortVSWR;
        S32BIT m_s32Result;
}S_VSWR_MEASUREMENT;

typedef struct _S_SPURIOUS_MEASUREMENT
{
        S32BIT m_s32EID;
        FDOUBLE m_dSpan;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dStimulasPathLoss;
        FDOUBLE m_dOutputFrequency;
        FDOUBLE m_dOutputPower;
        FDOUBLE m_dMeasuredPathLoss;
        FDOUBLE m_dExpMinSpurious;
        FDOUBLE m_dObservedOffsetFreq;
        FDOUBLE m_dObservedspurious;
        U8BIT m_u8Result;
}S_SPURIOUS_MEASUREMENT;

typedef struct _S_MODULEID_BITEISOLATION_MEAS
{
        S32BIT m_s32EID;
        S32BIT m_s32ChannelNo;
        FDOUBLE m_dInputfreq;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dStimulusPathloss;
        FDOUBLE m_dMeasuredPathloss;
        FDOUBLE m_dSetPower;
        FDOUBLE m_dMeasured_PowerANTIN;
        FDOUBLE m_dMeasured_Power;
        FDOUBLE m_dExpIsolation;
        FDOUBLE m_dMeasIsolation;
        S32BIT m_s32Result;
}S_MODULEID_BITEISOLATION_MEAS;

typedef struct _S_MODULEID_BLANKING_ISOLATION_MEAS
{
        S32BIT m_s32EID;
        S32BIT m_s32ChannelNo;
        FDOUBLE m_dInputfreq;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dStimulusPathLoss;
        FDOUBLE m_dMeasuredPathLoss;
        FDOUBLE m_dSetPower;
        FDOUBLE m_dMeasBypassPower;
        FDOUBLE m_dMeasBlankingPower;
        FDOUBLE m_dExpIsolation;
        FDOUBLE m_dMeasIsolation;
        S32BIT m_s32Result;
}S_MODULEID_BLANKING_ISOLATION_MEAS;

typedef struct _S_MODULEID_FILTER_REJECTION
{
        S32BIT m_s32EID;
        S32BIT m_s32ChannelNo;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dHPFLoss;
        FDOUBLE m_dNotchLoss;
        FDOUBLE m_dLPFLoss;
        FDOUBLE m_dMaxGain500_2000MHz;
        FDOUBLE m_dMaxGain10_18GHz;
        FDOUBLE m_dHPFRejection;
        FDOUBLE m_dNotchRejection;
        FDOUBLE m_dLPFRejection;
        FDOUBLE m_dExpRejection;
        S32BIT m_s32Result;
}S_MODULEID_FILTER_REJECTION;

typedef struct _S_MODULEID_TX_ATTENUATOR_VALIDATION
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dAppliedAttenuation;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dMeasuredPower;
        FDOUBLE m_dExpMinAttenuation;
        FDOUBLE m_dExpMaxAttenuation;
        FDOUBLE m_dCalcAttenuation;
        U8BIT m_u8Result;
}S_MODULEID_TX_ATTENUATOR_VALIDATION;

typedef struct _S_MODULEID_TX_PHASESHIFTER_CALIBRATION
{
        S32BIT m_s32EID;
        U8BIT m_u8Channelno;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dPhase;
        FDOUBLE m_dExpMinPhase;
        FDOUBLE m_dExpMaxPhase;
        FDOUBLE m_dMeasuredPhase;
        U8BIT m_u8Result;
}S_MODULEID_TX_PHASESHIFTER_CALIBRATION;

typedef struct _S_MODULEID_TX_PHASESHIFTER_VALIDATION
{
        S32BIT m_s32EID;
        U8BIT m_u8ChannelNo;
        FDOUBLE m_dInputFrequency;
        FDOUBLE m_dInputPower;
        FDOUBLE m_dPhase;
        FDOUBLE m_dExpMinPhase;
        FDOUBLE m_dExpMaxPhase;
        FDOUBLE m_dMeasuredPhase;
        U8BIT m_u8Result;
}S_MODULEID_TX_PHASESHIFTER_VALIDATION;
#endif //_DP_RPT_DATASTRUCT_
